#include "entropy_map.hpp"

/*******************************************************************************
 * IF_EntropyMap_Instr
 ******************************************************************************/

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

// bool
// IF_EntropyMap_Func::operator<(const IF_EntropyMap_Func& other) const
//{
// return this->get_name() < other.get_name();
//}

void
IF_EntropyMap_Func::insert_call(const IF_EntropyMap_Func* em_fn)
{
    this->callees.push_back(em_fn);
}

const std::string
IF_EntropyMap_Func::to_str(void) const
{
    std::ostringstream oss;
    oss << this->get_name() << '\n';
    for (const auto& instr : this->get_instrs())
    {
        oss << "\t" << instr->to_str();
    }
    return oss.str();
}

/*******************************************************************************
 * IF_EntropyMap
 ******************************************************************************/

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
        std::cout << "Fn " << em_fn->get_name() << '\n';
        std::cout << "Calls: [  ";
        for (const auto& em_call_fn : em_fn->get_callees())
        {
            std::cout << em_call_fn->get_name() << "  ";
        }
        std::cout << "]\n";

        for (const auto& em_fn_instr : em_fn->get_instrs())
        {
            if (this->verbose || em_fn_instr->get_is_fuzzed())
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

