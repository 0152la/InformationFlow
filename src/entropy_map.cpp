#include "entropy_map.hpp"

const std::string instr_simple_prefix = "i";

/*******************************************************************************
 * IF_EntropyMap::Instruction
 ******************************************************************************/

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
                std::cout << " -- " << em_fn_instr->get_retained_entropy()
                          << '\n';
                // TODO arguments
            }
        }
    }
}
