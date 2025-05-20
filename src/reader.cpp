#include "reader.hpp"

/* For certain instructions, we can directly calculate the entropy, as they
 * either affect only control flow, or move data around.
 */
std::map<uint16_t, double> set_entropy {
    // Terminator Instructions
    { llvm::Instruction::Ret, 1.0 }, { llvm::Instruction::Br, 1.0 },
    //{ llvm::Instruction::Switch, }, // TODO
    { llvm::Instruction::IndirectBr, 1.0 },
    { llvm::Instruction::Invoke, 1.0 }, /* This seems like a `Call` instruction
                                           in most cases, but consider edge
                                           cases
                                           */
    { llvm::Instruction::CallBr, 1.0 }, { llvm::Instruction::Resume, 1.0 },
    //{ llvm::Instruction::CatchSwitch, }, // TODO
    { llvm::Instruction::CatchRet, 1.0 },
    { llvm::Instruction::CleanupRet, 1.0 },
    { llvm::Instruction::Unreachable, 0.0 },

    // Unary Instructions
    { llvm::Instruction::FNeg, 1.0 }, /* The entropy here is actually off by
                                         one sample, due to two's complement
                                         and negating the maximum representable
                                         value; however, we estimate it to 1.0
                                         */
    // Vector Operations
    { llvm::Instruction::InsertElement, 1.0 },
    { llvm::Instruction::ShuffleVector, 0.5 },

    // Aggregate Operations
    //{ llvm::Instruction::ExtractValue, 1.0 },
    { llvm::Instruction::InsertValue, 1.0 },

    // Memory Operations
    { llvm::Instruction::Alloca, 1.0 },
    { llvm::Instruction::Load, 1.0 }, /* TODO does the value overwritten lose
                                         information?
                                         */
    { llvm::Instruction::Store, 1.0 }, { llvm::Instruction::Fence, 1.0 },
    { llvm::Instruction::AtomicCmpXchg, 1.0 }, // TODO same as Load
    { llvm::Instruction::GetElementPtr,
        1.0 }, // This only computes an address, does not dereference any memory

    // Conversion Operations
    { llvm::Instruction::ZExt, 1.0 }, { llvm::Instruction::SExt, 1.0 },
    //{ llvm::Instruction::FPTrunc, 1.0 },
    { llvm::Instruction::FPExt, 1.0 },

    //{ llvm::Instruction::FPToUI, 1.0 }, // TODO might be fine to approximate
    // to 1, but I think there is quite a bit of lost entropy from rounding

    //{ llvm::Instruction::FPToSI, 1.0 }, // TODO same as above
    { llvm::Instruction::UIToFP, 1.0 }, // TODO any unrepresentable integer?
    { llvm::Instruction::SIToFP, 1.0 }, // TODO same as above
    //{ llvm::Instruction::IntToPtr, 1.0 }, // TODO same as above
    { llvm::Instruction::BitCast, 1.0 },
    { llvm::Instruction::AddrSpaceCast, 1.0 },

    // Other Operations
    { llvm::Instruction::Select, 0.5 }, // TODO handle constant case (for all)
    { llvm::Instruction::Call, 1.0 },
    { llvm::Instruction::LandingPad, 1.0 }, // TODO need double checking
    //{ llvm::Instruction::Freeze, 1.0 }, // XXX

};

extern std::map<uint16_t, std::function<double(const llvm::Instruction&)>>
    estimate_entropy;

/*******************************************************************************
 * IF_Parser
 ******************************************************************************/

IF_Parser::IF_Parser()
{
    this->data = IF_Histogram<if_in_t, if_out_t>();
    this->in_gen = IF_Randgen();
}

IF_Parser::IF_Parser(int seed)
{
    this->data = IF_Histogram<if_in_t, if_out_t>();
    this->in_gen = IF_Randgen(seed);
}

std::unique_ptr<IF_EntropyMap>
IF_Parser::make_entropy_map(const llvm::Module& llvm_module)
{
    uint32_t instr_idx = 0;
    auto em = std::make_unique<IF_EntropyMap>(llvm_module);
    std::map<IF_EntropyMap_Instr*, std::vector<const llvm::Instruction*>>
        instr_succ_map;
    std::map<const llvm::Instruction*, const IF_EntropyMap_Instr*> em_instr_map;
    // Iterate over functions ...
    for (const auto& fn : llvm_module.getFunctionList())
    {
        if (fn.isDeclaration())
        {
            continue;
        }

        auto em_fn = std::make_unique<IF_EntropyMap_Func>(fn);
        IF_EntropyMap_Instr* em_instr_prev = nullptr;

        // ... and instructions
        for (const auto& fn_inst : llvm::instructions(fn))
        {
            auto em_instr
                = std::make_unique<IF_EntropyMap_Instr>(instr_idx, fn_inst);
            double retained_entropy;
            em_instr_map.emplace(&fn_inst, em_instr.get());

            // We first search whether this instruction is one we have a
            // default entropy for. If we do, then we simply set the entropy to
            // that value.
            if (auto fn_set_entropy = set_entropy.find(fn_inst.getOpcode());
                fn_set_entropy != set_entropy.end())
            {
                retained_entropy = fn_set_entropy->second;
            }
            // Then we check whether we can estimate the entropy, without having
            // to fuzz
            else if (auto fn_est_entropy
                = estimate_entropy.find(fn_inst.getOpcode());
                fn_est_entropy != estimate_entropy.end())
            {
                retained_entropy = (fn_est_entropy->second)(fn_inst);
            }
            // Otherwise, we perform fuzzing, via emulated instructions.
            else
            {
                IF_FuzzEngine if_fe;
                retained_entropy = if_fe.fuzz_retained_entropy(fn_inst);
            }

            // Record special instruction successors, such as from calls or
            // branch instructions
            if (llvm::isa<llvm::CallBase>(&fn_inst))
            {
                const llvm::Function* callee
                    = llvm::dyn_cast<llvm::CallBase>(&fn_inst)
                          ->getCalledFunction();
                if (callee->isDeclaration())
                {
                    em->insert_external_func(callee->getName().str());
                    em_instr->add_external_succ(callee->getName().str());
                }
                else
                {
                    instr_succ_map.emplace(em_instr.get(),
                        std::vector<const llvm::Instruction*>());
                    instr_succ_map.at(em_instr.get())
                        .push_back(&(callee->getEntryBlock().front()));
                }
            }
            else if (llvm::isa<llvm::BranchInst>(&fn_inst))
            {
                instr_succ_map.emplace(
                    em_instr.get(), std::vector<const llvm::Instruction*>());
                const llvm::BranchInst* bi
                    = llvm::dyn_cast<llvm::BranchInst>(&fn_inst);
                for (const auto& succ : bi->successors())
                {
                    instr_succ_map.at(em_instr.get())
                        .push_back(&(succ->front()));
                }
            }

            // If this is not the last instruction in a function, then set next
            // instruction as successor
            if (em_instr_prev)
            {
                em_instr_prev->add_successor(em_instr.get());
            }

            em_instr_prev = em_instr.get();
            em_instr->set_retained_entropy(retained_entropy);
            em_fn->insert(std::move(em_instr));
            instr_idx += 1;
        }
        em->insert(std::move(em_fn));
    }

    // Resolve instruction successors to `IF_EntropyMap_Instr`s
    for (auto& [em_instr, llvm_instrs] : instr_succ_map)
    {
        for (const llvm::Instruction* instr : llvm_instrs)
        {
            try
            {
                em_instr->add_successor(em_instr_map.at(instr));
            }
            catch (const std::out_of_range& ex)
            {
                std::cout << "Could not find entropy map entry for LLVM "
                             "instruction: ";
                instr->print(llvm::outs());
            }
        }
    }

    return em;
}

std::unique_ptr<IF_LLVM_Module>
IF_Parser::parse_ll(const std::string& ll_path)
{
    llvm::LLVMContext* llvm_ctx = new llvm::LLVMContext();
    llvm::SMDiagnostic* llvm_smd = new llvm::SMDiagnostic();
    std::unique_ptr<llvm::Module> ir_module
        = llvm::parseIRFile(ll_path, *llvm_smd, *llvm_ctx);

    if (!ir_module)
    {
        llvm_smd->print("iflow", llvm::errs());
        throw std::runtime_error("Could not parse given path " + ll_path);
    }

    return std::make_unique<IF_LLVM_Module>(
        llvm_ctx, llvm_smd, std::move(ir_module));
}

void
IF_Parser::print_instrs(const llvm::Module& llvm_module)
{
    for (const auto& fn : llvm_module.getFunctionList())
    {
        llvm::errs() << "Function " << fn.getName() << " -- "
                     << fn.getValueName() << '\n';

        for (const auto& fn_arg : fn.args())
        {
            llvm::errs() << "\t Arg " << fn_arg.getArgNo() << " : "
                         << fn_arg.getName() << " : " << fn_arg.getType()
                         << '\n';
        }

        llvm::errs() << "\t======\n";

        for (const auto& fn_inst : llvm::instructions(fn))
        {
            llvm::errs() << "\t Instruction " << fn_inst.getOpcodeName() << " ";
            for (const auto& fn_inst_arg : fn_inst.operand_values())
            {
                if (const llvm::Value* v
                    = llvm::dyn_cast<llvm::Value>(fn_inst_arg))
                {
                    llvm::errs() << "[ ";
                    v->printAsOperand(llvm::errs());
                    llvm::errs() << " --- ";
                    v->printAsOperand(llvm::errs(), false, &llvm_module);
                    llvm::errs() << " ]";
                }
                else
                {
                    throw std::runtime_error("Got non-Value argument");
                }
                // This is a function (TODO check)
                if (fn_inst_arg->hasName())
                {
                    llvm::errs() << fn_inst_arg->getName();
                }
                // This is a constant
                else if (const llvm::ConstantData* cd
                    = llvm::dyn_cast<llvm::ConstantData>(fn_inst_arg))
                {
                    if (const llvm::ConstantInt* ci
                        = llvm::dyn_cast<llvm::ConstantInt>(fn_inst_arg))
                    {
                        ci->getValue().print(llvm::errs(), true);
                    }
                    else if (const llvm::ConstantFP* cfp
                        = llvm::dyn_cast<llvm::ConstantFP>(fn_inst_arg))
                    {
                        cfp->getValue().print(llvm::errs());
                    }
                    else
                    {
                        throw std::runtime_error("Unhandled ConstantData type");
                    }
                }
                else
                {
                    llvm::errs() << fn_inst_arg;
                }
                llvm::errs() << " ";
            }
            llvm::errs() << '\n';
        }
    }
}
