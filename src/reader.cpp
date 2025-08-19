#include "reader.hpp"

extern set_entropy_t set_entropy;
extern estimate_entropy_t estimate_entropy;

/*******************************************************************************
 * IF_Parser
 ******************************************************************************/

double
IF_Parser::compute_instr_entropy(
    const llvm::Instruction& instr, IF_FuzzEngine& if_fe) const
{
    const std::string fn_snip_name = IF_Emulator::make_emulated_fn_name(instr);

    // We first search whether this instruction is one we have a
    // default entropy for. If we do, then we simply set the entropy to
    // that value.
    if (auto fn_set_entropy = set_entropy.find(fn_snip_name);
        fn_set_entropy != set_entropy.end())
    {
        return fn_set_entropy->second;
    }

    // Then we check whether we can estimate the entropy, without having
    // to fuzz
    // TODO decltype?
    if (auto fn_est_entropy = estimate_entropy.find(fn_snip_name);
        fn_est_entropy != estimate_entropy.end())
    {
        return (fn_est_entropy->second)(instr);
    }

    // Otherwise, we perform fuzzing, via emulated instructions.
    return if_fe.fuzz_retained_entropy(instr);
}

std::unique_ptr<IF_EntropyMap::Map>
IF_Parser::make_entropy_map(
    const llvm::Module& llvm_module, IF_FuzzEngine& if_fe)
{
    uint32_t instr_idx = 0;
    auto em = std::make_unique<IF_EntropyMap::Map>(llvm_module);

    // Map holding successors of Instructions, for stuff like branches and
    // jumps; used to resolve to `IF_EntropyMap::Instruction` later
    std::map<IF_EntropyMap::Instruction*, std::vector<const llvm::Instruction*>>
        instr_succ_map;

    // Map holding which `LLVM Instruction` maps to which
    // `IF_EntropyMap::Instruction`. Used in resolving other dependencies
    std::map<const llvm::Instruction*, const IF_EntropyMap::Instruction*>
        em_instr_map;

    // Map holding `llvm::Function`s and their `LLVM::ReturnInst`s (in
    // `IF_EntropyMap::Instruction` format). When we call a function, we need to
    // add a dependency edge between the `return`s of that function, and any
    // caller instructions.
    std::map<const llvm::Function*, std::vector<IF_EntropyMap::Instruction*>>
        func_returns;

    // Map holding `llvm::CallInst`s (in their `IF_EntropyMap::Instruction`
    // format) and the `llvm::Functions` they are calling. This is used to
    // resolve `return` dependencies.
    std::map<const IF_EntropyMap::Instruction*, const llvm::Function*>
        func_calls;

    // Iterate over functions ...
    for (const auto& fn : llvm_module.getFunctionList())
    {
        if (fn.isDeclaration())
        {
            continue;
        }

        auto em_fn = std::make_unique<IF_EntropyMap::Function>(fn);
        IF_EntropyMap::Instruction* em_instr_prev = nullptr;

        // ... and instructions
        for (const auto& fn_inst : llvm::instructions(fn))
        {
            auto em_instr = std::make_unique<IF_EntropyMap::Instruction>(
                instr_idx, fn_inst);
            double retained_entropy = compute_instr_entropy(fn_inst, if_fe);
            if (retained_entropy < std::numeric_limits<double>::epsilon())
            {
                std::ostringstream err;
                err << "Seen entropy under ulp: " << retained_entropy << " < "
                    << std::numeric_limits<double>::epsilon();
                // throw std::runtime_error(err.str());
                llvm::errs() << "Warning: " << err.str() << '\n';
            }
            em_instr_map.emplace(&fn_inst, em_instr.get());

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
                    func_calls.emplace(em_instr.get(), callee);
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
            else if (llvm::isa<llvm::ReturnInst>(&fn_inst))
            {
                if (!func_returns.contains(&fn))
                {
                    func_returns.emplace(
                        &fn, std::vector<IF_EntropyMap::Instruction*>());
                }
                func_returns.at(&fn).push_back(em_instr.get());
            }

            // If this is not the last instruction in a function, nor is it an
            // `unreachable` instruction, then set next instruction as successor
            if (em_instr_prev
                && em_instr_prev->get_opcode()
                    != llvm::Instruction::Unreachable)
            {
                em_instr_prev->set_natural_successor(em_instr.get());
                em_instr_prev->add_successor(em_instr.get());
            }

            em_instr_prev = em_instr.get();
            em_instr->set_retained_entropy(retained_entropy);
            em_fn->insert(std::move(em_instr));
            instr_idx += 1;
        }
        em->insert(std::move(em_fn));
    }
    em->set_instruction_count(instr_idx);

    // Resolve instruction successors to `IF_EntropyMap::Instruction`s
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
                throw std::runtime_error("Error adding successor!");
            }
        }
    }

    // Resolve `return` successors
    for (const auto& [em_instr, fn] : func_calls)
    {
        decltype(func_returns)::const_iterator fn_ret_insts
            = func_returns.find(fn);
        if (fn_ret_insts != func_returns.end())
        {
            for (IF_EntropyMap::Instruction* em_ret_instr :
                (*fn_ret_insts).second)
            {
                if (!em_instr->get_natural_successor())
                {
                    throw std::runtime_error("Unset natural successor!");
                }
                em_ret_instr->add_successor(em_instr->get_natural_successor());
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
        llvm::outs() << "Function " << fn.getName() << " -- "
                     << fn.getValueName() << '\n';

        for (const auto& fn_arg : fn.args())
        {
            llvm::outs() << "\t Arg " << fn_arg.getArgNo() << " : "
                         << fn_arg.getName() << " : " << fn_arg.getType()
                         << '\n';
        }

        llvm::outs() << "\t======\n";

        for (const auto& fn_inst : llvm::instructions(fn))
        {
            llvm::outs() << "\t Instruction " << fn_inst.getOpcodeName() << " ";
            for (const auto& fn_inst_arg : fn_inst.operand_values())
            {
                if (const llvm::Value* v
                    = llvm::dyn_cast<llvm::Value>(fn_inst_arg))
                {
                    llvm::outs() << "[ ";
                    v->printAsOperand(llvm::outs());
                    llvm::outs() << " --- ";
                    v->printAsOperand(llvm::outs(), false, &llvm_module);
                    llvm::outs() << " ] ";
                }
                else
                {
                    throw std::runtime_error("Got non-Value argument");
                }
                // This is a function (TODO check)
                if (fn_inst_arg->hasName())
                {
                    llvm::outs() << fn_inst_arg->getName();
                }
                // This is a constant
                else if (const llvm::ConstantData* cd
                    = llvm::dyn_cast<llvm::ConstantData>(fn_inst_arg))
                {
                    if (const llvm::ConstantInt* ci
                        = llvm::dyn_cast<llvm::ConstantInt>(fn_inst_arg))
                    {
                        ci->getValue().print(llvm::outs(), true);
                    }
                    else if (const llvm::ConstantFP* cfp
                        = llvm::dyn_cast<llvm::ConstantFP>(fn_inst_arg))
                    {
                        cfp->getValue().print(llvm::outs());
                    }
                    else if (llvm::isa<llvm::UndefValue>(fn_inst_arg))
                    {
                        llvm::outs() << "undef";
                    }
                    else if (llvm::isa<llvm::ConstantPointerNull>(fn_inst_arg))
                    {
                        llvm::outs() << "nullptr";
                    }
                    else
                    {
                        throw std::runtime_error("Unhandled ConstantData type");
                    }
                }
                else
                {
                    llvm::outs() << fn_inst_arg;
                }
                llvm::outs() << " ";
            }
            llvm::outs() << '\n';
        }

        llvm::outs() << "=== DONE Function " << fn.getName() << '\n';
    }
}
