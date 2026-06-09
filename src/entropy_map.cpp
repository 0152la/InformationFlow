#include "entropy_map.hpp"

const std::string instr_simple_prefix = "i";

/*******************************************************************************
 * IF_EntropyMap::Instruction
 ******************************************************************************/

IF_EntropyMap::Instruction::Instruction(
    uint32_t _idx, const llvm::Instruction& _inst) :
    idx(_idx),
    opcode(_inst.getOpcode()),
    ret_ty_bit_sz(_inst.getType()->isVoidTy()
            ? 0
            : _inst.getDataLayout()
                  .getTypeSizeInBits(_inst.getType())
                  .getFixedValue()),
    llvm_no_uses(_inst.use_empty()) { };

void
IF_EntropyMap::Instruction::add_successor(
    const IF_EntropyMap::Instruction* to_add)
{
    this->succs_instr.insert(to_add);
}

void
IF_EntropyMap::Instruction::add_external_succ(std::string ex_fn)
{
    this->succs_extern.insert(ex_fn);
}

const std::string
IF_EntropyMap::Instruction::to_str(void) const
{
    std::ostringstream oss;
    oss << llvm::Instruction::getOpcodeName(this->get_opcode());
    oss << " -- Entropy " << this->get_retained_entropy() << '\n';
    return oss.str();
}

const std::string
IF_EntropyMap::Instruction::to_str_simple(void) const
{
    return instr_simple_prefix + std::to_string(this->get_idx());
}

/*******************************************************************************
 * IF_EntropyMap::Function
 ******************************************************************************/

const IF_EntropyMap::Instruction*
IF_EntropyMap::Function::get_first_instr() const
{
    return this->instrs.front().get();
}

const std::string
IF_EntropyMap::Function::set_demangled_name(const llvm::Function& _fn)
{
    const llvm::DISubprogram* dis = _fn.getSubprogram();
    if (!dis)
    {
        return std::string();
    }
    return dis->getName().str();
}

const std::string
IF_EntropyMap::Function::get_representing_name(void) const
{
    if (this->get_demangled_name().empty())
    {
        return this->get_name();
    }
    return this->get_demangled_name();
}

const std::string
IF_EntropyMap::Function::to_str(void) const
{
    std::ostringstream oss;
    oss << this->get_name();
    if (this->get_demangled_name().empty())
    {
        oss << "[ " << this->get_demangled_name() << " ]";
    }
    oss << '\n';
    for (const auto& instr : this->get_instrs())
    {
        oss << "\t" << instr->to_str();
    }
    return oss.str();
}

/*******************************************************************************
 * IF_EntropyMap::Map
 ******************************************************************************/

const IF_EntropyMap::Instruction*
IF_EntropyMap::Map::get_first_instr() const
{
    for (const auto& func : this->funcs)
    {
        if (!func->get_demangled_name().compare("main")
            || !func->get_name().compare("main"))
        {
            return func->get_first_instr();
        }
    }
    throw std::runtime_error("Couldn't find `main`!");
}

auto
IF_EntropyMap::Map::get_nontrivial_instruction_count(void) const
    -> decltype(this->instr_count)
{
    size_t i_count = 0;
    for (const auto& fn : this->funcs)
    {
        i_count
            += std::count_if(fn->get_instrs().begin(), fn->get_instrs().end(),
                [](const auto& inst) { return !inst->is_trivial(); });
    }
    return i_count;
}

void
IF_EntropyMap::Map::insert_external_func(std::string ex_fn)
{
    this->external_funcs.insert(ex_fn);
}

std::tuple<size_t, size_t, size_t>
IF_EntropyMap::Map::compute_cyclomatic_complexity(void) const
{
    std::vector<bool> seen_idxs(this->instr_count, false);
    seen_idxs.at(this->get_first_instr()->get_idx()) = true;

    std::queue<const Instruction*> to_follow;
    to_follow.push(this->get_first_instr());

    const Instruction* curr_inst;
    size_t edges = 0;
    size_t nodes = 0;

    while (!to_follow.empty())
    {
        curr_inst = to_follow.front();
        to_follow.pop();
        seen_idxs.at(curr_inst->get_idx()) = true;
        nodes += 1;

        for (IF_EntropyMap::Instruction::succs_t::const_reference succ :
            curr_inst->get_succs_inst())
        {
            edges += 1;
            if (!seen_idxs.at(succ->get_idx()))
            {
                to_follow.push(succ);
            }
        }
    }

    return std::tuple(edges, nodes, edges - nodes + 2);
}

const std::string
IF_EntropyMap::Map::to_str(void) const
{
    std::ostringstream oss;
    for (const auto& em_fn : this->get_funcs())
    {
        oss << em_fn->to_str();
    }
    return oss.str();
}

void
IF_EntropyMap::Map::print(void) const
{
    for (const auto& em_fn : this->get_funcs())
    {
        std::cout << "Fn " << em_fn->get_name();
        if (!em_fn->get_demangled_name().empty())
        {
            std::cout << " -- " << em_fn->get_demangled_name();
        }
        std::cout << '\n';

        for (const auto& em_fn_instr : em_fn->get_instrs())
        {
            if (this->verbose || !em_fn_instr->is_trivial())
            {
                std::cout << "\t"
                          << llvm::Instruction::getOpcodeName(
                                 em_fn_instr->get_opcode());
                std::cout << " -- BS " << em_fn_instr->ret_ty_bit_sz;
                std::cout << " -- UC " << em_fn_instr->get_retained_entropy()
                          << '\n';
                // TODO arguments
            }
        }
    }
}

/*******************************************************************************
 * IF_EntropyMap::UseMap
 ******************************************************************************/

/* Node ***********************************************************************/

IF_EntropyMap::UseMap::Node::Node(const Instruction* _inst) :
    em_inst(_inst),
    is_used(false) { };

double
IF_EntropyMap::UseMap::Node::get_unc_coef(void) const
{
    return this->em_inst->get_retained_entropy();
}

auto
IF_EntropyMap::UseMap::Node::get_idx(void) const
    -> IF_EntropyMap::Instruction::idx_t
{
    return this->em_inst->get_idx();
}

std::string
IF_EntropyMap::UseMap::Node::to_str(void) const
{
    auto oss = std::ostringstream { };
    oss << fmt::format("NODE {}\n", this->get_idx());
    if (!this->uses.empty())
    {
        oss << fmt::format("\tUSES {}\n",
            std::accumulate(std::next(this->uses.begin()), this->uses.end(),
                std::to_string((*this->uses.begin())->get_idx()),
                [](std::string s, const IF_EntropyMap::UseMap::Node* n)
                { return s + ", " + std::to_string(n->get_idx()); }));
    }
    return oss.str();
}

std::string
IF_EntropyMap::UseMap::Node::to_str_path(
    uint32_t _indent, double _uc) const
{
    const auto indent_str = std::string(_indent, '-');
    _uc *= this->em_inst->get_retained_entropy();
    std::ostringstream oss;
    oss << fmt::format(
        "{}NODE {} == UC {}\n", indent_str, this->get_idx(), _uc);
    for (const auto& user : this->uses)
    {
        oss << user->to_str_path(_indent + 1, _uc);
    }
    return oss.str();
}

/* UC_Path ********************************************************************/

IF_EntropyMap::UseMap::UC_Path::UC_Path(
    const IF_EntropyMap::UseMap::Node* _node) :
    last_node(_node),
    node_path(std::to_string(_node->get_idx())),
    unc_coef(1.0) { };

std::string
IF_EntropyMap::UseMap::UC_Path::to_str(void) const
{
    return fmt::format("PATH {} -- UC {}", this->node_path, this->unc_coef);
}

/* MemDeps ********************************************************************/

void
IF_EntropyMap::UseMap::MemDeps::log_mem_deps(
    const llvm::Instruction* _inst, llvm::Function& _fn)
{
    auto llvm_dt = llvm::DominatorTree(_fn);
    auto llvm_ac = llvm::AssumptionCache(_fn);
    // TODO fix this?
    // auto llvm_triple = llvm_module.getTargetTriple();
    // auto llvm_tli_impl = llvm::TargetLibraryInfoImpl(llvm_triple);
    auto llvm_tli_impl = llvm::TargetLibraryInfoImpl(llvm::Triple());
    auto llvm_tli = llvm::TargetLibraryInfo(llvm_tli_impl);
    auto llvm_aar = llvm::AAResults(llvm_tli);
    auto llvm_mssa = llvm::MemorySSA(_fn, &llvm_aar, &llvm_dt);

    if (const auto* inst_ma = llvm_mssa.getMemoryAccess(_inst))
    {
        if (!llvm::isa<llvm::MemoryUse>(inst_ma))
        {
            return;
        }

        if (const auto* inst_ma_clobber
            = llvm_mssa.getWalker()->getClobberingMemoryAccess(_inst))
        {
            if (llvm_mssa.isLiveOnEntryDef(inst_ma_clobber))
            {
                return;
            }

            this->mem_deps.emplace(_inst,
                IF_EntropyMap::UseMap::MemDeps::mem_deps_t::mapped_type { });
            const auto& clobbers = this->get_mem_acc_defs(inst_ma_clobber);

            for (const auto* dep : clobbers)
            {
                this->mem_deps[_inst].insert(dep->getMemoryInst());
            }
        }
    }
}

const std::unordered_set<const llvm::MemoryDef*>
IF_EntropyMap::UseMap::MemDeps::get_mem_acc_local_defs(
    const llvm::MemoryAccess* _ma) const
{
    auto res = std::unordered_set<const llvm::MemoryDef*> { };
    auto to_process = std::queue<const llvm::MemoryAccess*> { };
    to_process.push(_ma);

    while (!to_process.empty())
    {
        auto* curr_ma = to_process.front();
        to_process.pop();

        // Handle `MemoryPhi`s
        if (const auto* curr_mphi = llvm::dyn_cast<llvm::MemoryPhi>(curr_ma))
        {
            for (size_t i = 0; i < curr_mphi->getNumIncomingValues(); ++i)
            {
                to_process.push(curr_mphi->getIncomingValue(i));
            }
        }

        // Handle external function call clobbers
        else if (const auto* curr_md = llvm::dyn_cast<llvm::MemoryDef>(curr_ma))
        {
            bool is_external = false;
            if (const auto* cb_inst
                = llvm::dyn_cast<llvm::CallBase>(curr_md->getMemoryInst()))
            {
                const auto* cb_inst_fn = cb_inst->getCalledFunction();
                if (!cb_inst_fn || cb_inst_fn->isDeclaration())
                {
                    is_external = true;
                    to_process.push(curr_md->getDefiningAccess());
                }
            }

            if (!is_external)
            {
                res.insert(curr_md);
            }
        }
    }

    return res;
}

const std::unordered_set<const llvm::MemoryDef*>
IF_EntropyMap::UseMap::MemDeps::get_mem_acc_defs(
    const llvm::MemoryAccess* _ma) const
{
    auto res = std::unordered_set<const llvm::MemoryDef*> { };
    auto to_process = std::queue<const llvm::MemoryAccess*> { };
    to_process.push(_ma);

    while (!to_process.empty())
    {
        auto* curr_ma = to_process.front();
        to_process.pop();

        // Handle `MemoryPhi`
        if (const auto* curr_mphi = llvm::dyn_cast<llvm::MemoryPhi>(curr_ma))
        {
            for (size_t i = 0; i < curr_mphi->getNumIncomingValues(); ++i)
            {
                to_process.push(curr_mphi->getIncomingValue(i));
            }
        }
        // Handle `MemoryDef`
        else if (const auto* curr_md = llvm::dyn_cast<llvm::MemoryDef>(curr_ma))
        {
            res.insert(curr_md);
        }
        else
        {
            llvm::outs() << "Unhandled MemoryAccess` " << curr_ma << "!\n";
            throw std::runtime_error("");
        }
    }

    return res;
}

/* UseMap *********************************************************************/

IF_EntropyMap::UseMap::UseMap(
    const IF_EntropyMap::UseMap::insts_pair_t& _em_insts,
    const IF_EntropyMap::UseMap::MemDeps& _em_mem_deps,
    const IF_EntropyMap::UseMap::llvm_to_insts_map_t& _llvm_insts_map)
{
    auto seen_insts = std::vector<bool>(_em_insts.size(), false);
    auto inst_to_node_map
        = std::unordered_map<const IF_EntropyMap::Instruction*,
            IF_EntropyMap::UseMap::Node*>();

    auto get_or_make_node
        = [&_em_insts = std::as_const(_em_insts), &inst_to_node_map,
              &seen_insts, this](const IF_EntropyMap::Instruction* _em_inst)
        -> IF_EntropyMap::UseMap::Node*
    {
        if (seen_insts[_em_inst->get_idx()])
        {
            return inst_to_node_map[_em_inst];
        }
        else
        {
            auto new_node = new IF_EntropyMap::UseMap::Node(_em_inst);
            inst_to_node_map.emplace(_em_inst, new_node);
            seen_insts[_em_inst->get_idx()] = true;
            return new_node;
        }
    };

    auto insert_node_from_llvm_inst
        = [&_llvm_insts_map = std::as_const(_llvm_insts_map),
              &get_or_make_node](IF_EntropyMap::UseMap::Node* _curr_node,
              const llvm::Instruction* _inst) -> void
    {
        auto* em_inst = _llvm_insts_map.at(_inst);
        auto* em_node = get_or_make_node(em_inst);
        _curr_node->uses.insert(em_node);
        em_node->is_used = true;
    };

    for (const auto& [em_inst, llvm_inst] : _em_insts | std::views::reverse)
    {
        auto curr_node = get_or_make_node(em_inst);
        llvm::outs() << "INST " << em_inst->get_idx() << " == " << *llvm_inst
                     << '\n';

        // Add edge towards pre-computed memory dependent instructions
        if (const auto inst_deps_it = _em_mem_deps.mem_deps.find(llvm_inst);
            inst_deps_it != _em_mem_deps.mem_deps.end())
        {
            for (const auto* dep_inst : inst_deps_it->second)
            {
                insert_node_from_llvm_inst(curr_node, dep_inst);
            }
        }

        // If this is a `store` instruction, we will create a dependency
        // between the value we are storing (if it is an `llvm::Instruction`)
        // and this `store`
        if (const auto* llvm_inst_store
            = llvm::dyn_cast<llvm::StoreInst>(llvm_inst))
        {
            llvm::outs() << "TEST STORE VALUE "
                         << llvm_inst_store->getValueOperand() << '\n';
            if (const auto* llvm_inst_store_value
                = llvm::dyn_cast<llvm::Instruction>(
                    llvm_inst_store->getValueOperand()))
            {
                llvm::outs()
                    << "ADD STORE VALUE INST " << llvm_inst_store_value << '\n';
                insert_node_from_llvm_inst(curr_node, llvm_inst_store_value);
            }
        }
        // If this is a `load` instruction, the dependency is taken care of via
        // `MemorySSA`
        else if (llvm::isa<llvm::LoadInst>(llvm_inst))
        {
            continue;
        }
        // Otherwise, for other instructions, draw a dependency link between
        // their operands and other instructions
        else
        {

            // Add edge towards direct operands consumed by this instruction
            for (const auto& op : llvm_inst->operands())
            {
                llvm::outs() << "\tOP " << op << '\n';

                const auto* op_inst = llvm::dyn_cast<llvm::Instruction>(op);
                if (const auto op_inst = llvm::dyn_cast<llvm::Instruction>(op))
                {
                    llvm::outs() << "\tINST!!!\n";
                    insert_node_from_llvm_inst(curr_node, op_inst);
                }
            }
        }
    }

    for (const auto& [em_inst, em_node] : inst_to_node_map)
    {
        if (!em_node->is_used)
        {
            this->root_nodes.insert(em_node);
        }
    }

    // for (const auto& [em_inst, em_node] : inst_to_node_map)
    //{
    // std::cout << "===============\n";
    // std::cout << em_node->to_str();
    // std::cout << "===============\n";
    //}

    llvm::outs() << this->to_str() << '\n';
}

std::string
IF_EntropyMap::UseMap::UseMap::to_str(void) const
{
    std::ostringstream ss;
    const std::string line_delim = "----------\n";

    ss << "USEMAP\n" << line_delim;
    for (const auto& n : this->root_nodes)
    {
        ss << n->to_str_path(0, 1.0);
        ss << line_delim;
    }
    return ss.str();
}
