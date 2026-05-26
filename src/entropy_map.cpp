#include "entropy_map.hpp"

const std::string instr_simple_prefix = "i";

/*******************************************************************************
 * IF_EntropyMap::Instruction
 ******************************************************************************/

IF_EntropyMap::Instruction::Instruction(
    uint32_t _idx, const llvm::Instruction& _inst) :
    idx(_idx),
    opcode(_inst.getOpcode()),
    ret_ty_bit_sz(_inst.getType()->getPrimitiveSizeInBits().getFixedValue()),
    llvm_no_uses(_inst.use_empty()),
    llvm_is_terminator(_inst.isTerminator()) { };

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
    em_inst(_inst) { };

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
IF_EntropyMap::UseMap::Node::to_str_path(uint32_t _indent) const
{
    const auto indent_str = std::string(_indent, '-');
    std::ostringstream oss;
    oss << fmt::format("{}NODE {}{}\n", indent_str, this->get_idx(),
        this->em_inst->llvm_is_terminator ? " TERM" : "");
    for (const auto& user : this->uses)
    {
        oss << user->to_str_path(_indent + 1);
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

/* UseMap *********************************************************************/

IF_EntropyMap::UseMap::UseMap(
    const IF_EntropyMap::UseMap::insts_pair_t& _em_insts,
    const IF_EntropyMap::UseMap::llvm_to_insts_map_t& _llvm_insts_map,
    llvm::MemorySSA& _llvm_mem_ssa)
{
    auto seen_insts = std::vector<bool>(_em_insts.size(), false);
    auto inst_to_node_map
        = std::unordered_map<const IF_EntropyMap::Instruction*,
            IF_EntropyMap::UseMap::Node*>();

    auto mssa_walker = _llvm_mem_ssa.getWalker();

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
            if (_em_inst->llvm_is_terminator)
            {
                this->root_nodes.push_back(new_node);
            }
            return new_node;
        }
    };

    auto insert_node_from_llvm_inst
        = [&_llvm_insts_map = std::as_const(_llvm_insts_map),
              &get_or_make_node](IF_EntropyMap::UseMap::Node* _curr_node,
              const llvm::Instruction* _inst) -> void
    {
        auto em_inst = _llvm_insts_map.at(_inst);
        auto em_node = get_or_make_node(em_inst);
        _curr_node->uses.emplace_back(em_node);
    };

    for (const auto& [em_inst, llvm_inst] : _em_insts | std::views::reverse)
    {
        auto curr_node = get_or_make_node(em_inst);
        llvm::outs() << "INST " << em_inst->get_idx();
        llvm_inst->print(llvm::outs());
        llvm::outs() << '\n';

        // If this is a `load` instruction, we handle it specially, as we need
        // to look for the last clobber of the accessed memory address, rather
        // than just looking at operands
        if (const auto li = llvm::dyn_cast<llvm::LoadInst>(llvm_inst))
        {
            const auto ma = _llvm_mem_ssa.getMemoryAccess(li);
            llvm::outs() << "\tMEM ";
            // TODO check if Instruction
            ma->getDefiningAccess()->print(llvm::outs());
            auto md = llvm::dyn_cast<llvm::MemoryDef>(
                mssa_walker->getClobberingMemoryAccess(ma));
            llvm::outs() << "\n\t";
            md->getMemoryInst()->print(llvm::outs());
            llvm::outs() << "\n";

            insert_node_from_llvm_inst(curr_node, md->getMemoryInst());
            // auto md_em_inst = _llvm_insts_map.at(md->getMemoryInst());
            // auto md_em_node = get_or_make_node(md_em_inst);
            // curr_node->uses.emplace_back(md_em_node);
        }
        // If this is a `store` instruction, we only care about using the value
        // operand, not the pointer one; that is handled by the dependency for
        // `load` instructions
        //
        // XXX this ignores the `alloca`s where the `store` comes from, but we
        // don't model memory information flow at this stage
        else if (const auto si = llvm::dyn_cast<llvm::StoreInst>(llvm_inst))
        {
            if (const auto si_val_inst
                = llvm::dyn_cast<llvm::Instruction>(si->getValueOperand()))
            {
                insert_node_from_llvm_inst(curr_node, si_val_inst);
            }
        }
        // Otherwise, we iterate over operands, to get the `use-def` chain
        else
        {
            for (const auto& op : llvm_inst->operands())
            {
                llvm::outs() << "\tOP ";
                op->print(llvm::outs());
                llvm::outs() << '\n';

                const auto op_inst = llvm::dyn_cast<llvm::Instruction>(op);
                if (!op_inst)
                {
                    continue;
                }

                llvm::outs() << "\tINST!!!\n";
                insert_node_from_llvm_inst(curr_node, op_inst);
            }
        }
    }

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
        ss << n->to_str_path(0);
        ss << line_delim;
    }
    return ss.str();
}
