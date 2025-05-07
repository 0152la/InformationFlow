#include "entropy_map.hpp"

/*******************************************************************************
 * IF_EntropyMap_Instr
 ******************************************************************************/

void
IF_EntropyMap_Instr::add_successor(const IF_EntropyMap_Instr* to_add)
{
    this->succs.insert(to_add->get_idx());
}

void
IF_EntropyMap_Instr::add_external_succ(std::string ex_fn)
{
    this->succs_extern.insert(ex_fn);
}

const std::string
IF_EntropyMap_Instr::to_str(void) const
{
    std::ostringstream oss;
    oss << llvm::Instruction::getOpcodeName(this->get_opcode());
    oss << " -- Entropy " << this->get_retained_entropy() << '\n';
    return oss.str();
}

/*******************************************************************************
 * IF_EntropyMap_Func
 ******************************************************************************/

const std::string
IF_EntropyMap_Func::set_demangled_name(const llvm::Function& _fn)
{
    const llvm::DISubprogram* dis = _fn.getSubprogram();
    if (!dis)
    {
        return std::string();
    }
    return dis->getName().str();
}

const std::string
IF_EntropyMap_Func::get_representing_name(void) const
{
    if (this->get_demangled_name().empty())
    {
        return this->get_name();
    }
    return this->get_demangled_name();
}

const std::string
IF_EntropyMap_Func::to_str(void) const
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
 * IF_EntropyMap
 ******************************************************************************/

void
IF_EntropyMap::insert_external_func(std::string ex_fn)
{
    this->external_funcs.insert(ex_fn);
}

const std::string
IF_EntropyMap::to_str(void) const
{
    std::ostringstream oss;
    for (const auto& em_fn : this->get_funcs())
    {
        oss << em_fn->to_str();
    }
    return oss.str();
}

void
IF_EntropyMap::print(void) const
{
    for (const auto& em_fn : this->get_funcs())
    {
        std::cout << "Fn " << em_fn->get_name();
        if (!em_fn->get_demangled_name().empty())
        {
            std::cout << " -- " << em_fn->get_demangled_name();
        }

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

