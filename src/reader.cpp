#include "reader.hpp"

extern set_entropy_t set_entropy;
extern estimate_entropy_t estimate_entropy;

/*******************************************************************************
 * IF_Parser
 ******************************************************************************/

std::unique_ptr<IF_EntropyMap::Map>
IF_Parser::make_entropy_map(llvm::Module& llvm_module)
{
    uint32_t instr_idx = 0;
    auto em = std::make_unique<IF_EntropyMap::Map>(llvm_module);

    // Map holding successors of Instructions, for stuff like branches and
    // jumps; used to resolve to `IF_EntropyMap::Instruction` later
    std::map<IF_EntropyMap::Instruction*, std::vector<const llvm::Instruction*>>
        instr_succ_map;

    // Map holding which `LLVM Instruction` maps to which
    // `IF_EntropyMap::Instruction`. Used in resolving other dependencies
    IF_EntropyMap::insts_llvm_mapping_t em_instr_map;
    // std::map<const llvm::Instruction*, const IF_EntropyMap::Instruction*>
    // em_instr_map;

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

    IF_EntropyMap::UseMap::insts_pair_t em_usemap_em_insts;
    IF_EntropyMap::UseMap::llvm_to_insts_map_t em_usemap_llvm_insts_map;

    // TODO
    auto unc_coef_getter = IF_Entropy_Vals::Getter { };
    auto em_usemap_mem_deps = IF_EntropyMap::UseMap::MemDeps { };

    // Iterate over functions ...
    for (llvm::Function& fn : llvm_module.getFunctionList())
    {
        if (fn.isDeclaration())
        {
            continue;
        }

        llvm::outs() << "** FUNCTION " << fn.getName().str()
                     << " ****************************************\n";

        auto em_fn = std::make_unique<IF_EntropyMap::Function>(fn);
        IF_EntropyMap::Instruction* em_instr_prev = nullptr;

        auto llvm_dt = llvm::DominatorTree(fn);
        auto llvm_ac = llvm::AssumptionCache(fn);
        // auto llvm_triple = llvm_module.getTargetTriple();
        // auto llvm_tli_impl
        //= llvm::TargetLibraryInfoImpl(llvm_triple);
        auto llvm_tli_impl = llvm::TargetLibraryInfoImpl(llvm::Triple());
        auto llvm_tli = llvm::TargetLibraryInfo(llvm_tli_impl);
        auto llvm_aar = llvm::AAResults(llvm_tli);
        auto llvm_mssa = llvm::MemorySSA(fn, &llvm_aar, &llvm_dt);

        // TODO
        auto* llvm_mssa_walker = llvm_mssa.getWalker();

        // ... and instructions
        for (const auto& fn_inst : llvm::instructions(fn))
        {
            auto em_instr = std::make_unique<IF_EntropyMap::Instruction>(
                instr_idx, fn_inst);
            double retained_entropy
                = unc_coef_getter.get_entropy_for_inst(fn_inst);
            em_usemap_mem_deps.log_mem_deps(&fn_inst, llvm_mssa);

            if (retained_entropy < std::numeric_limits<double>::epsilon())
            {
                std::ostringstream err;
                err << "Seen entropy under ulp: " << retained_entropy << " < "
                    << std::numeric_limits<double>::epsilon();
                // throw std::runtime_error(err.str());
                llvm::errs() << "Warning: " << err.str() << '\n';
            }
            em_instr_map.emplace(&fn_inst, em_instr.get());
            em_usemap_em_insts.emplace_back(em_instr.get(), &fn_inst);
            em_usemap_llvm_insts_map.emplace(&fn_inst, em_instr.get());

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

        // TODO untangle EntropyMap and UseMap from this
        // auto use_em = std::make_unique<IF_EntropyMap::UseMap>(
        // em_usemap_em_insts, em_usemap_llvm_insts_map, llvm_mssa);
    }
    em->set_instruction_count(instr_idx);
    const auto em_usemap = std::make_unique<IF_EntropyMap::UseMap>(
        em_usemap_em_insts, em_usemap_mem_deps, em_usemap_llvm_insts_map);

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

    // em->init_stats();
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
            // llvm::outs() << "\t Instruction " << fn_inst.getOpcodeName() << "
            // ";
            llvm::outs() << fmt::format("\tInstruction {} (op {}) ",
                fn_inst.getOpcodeName(), fn_inst.getOpcode());
            for (const auto& fn_inst_arg : fn_inst.operand_values())
            {
                if (const llvm::Value* v
                    = llvm::dyn_cast<llvm::Value>(fn_inst_arg))
                {
                    llvm::outs() << "VALUE [ ";
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
                    llvm::outs() << "NAME " << fn_inst_arg->getName();
                }
                // This is a constant
                else if (const llvm::ConstantData* cd
                    = llvm::dyn_cast<llvm::ConstantData>(fn_inst_arg))
                {
                    llvm::outs() << "CONST ";
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
