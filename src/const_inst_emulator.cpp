#include "const_inst_emulator.hpp"

// using set_entropy_t = std::unordered_map<entropy_map_key_t, double>;
// using estimate_entropy_t = std::unordered_map<entropy_map_key_t,
// std::function<double(const llvm::Instruction&)>>;

// set_entropy_t const_set_entropy {
std::unordered_map<unsigned int, double> const_inst_set_entropy {
    // Binary Instructions
    { llvm::Instruction::Add, 1.0 },
    { llvm::Instruction::Sub, 1.0 },
};

// estimate_entropy_t const_estimate_entropy {
std::unordered_map<unsigned int,
    std::function<double(const llvm::Instruction&)>>
    const_inst_estimate_entropy {

        // Binary Instructions
        { llvm::Instruction::URem, IF_Const_Emulator::estimate_mod },
        { llvm::Instruction::SRem, IF_Const_Emulator::estimate_mod },
    };

/*******************************************************************************
 * Entropy estimation functions
 */

/* Binary Instructions ********************************************************/

double
IF_Const_Emulator::estimate_mod(const llvm::Instruction& inst)
{
    const llvm::BinaryOperator* bo
        = llvm::dyn_cast<llvm::BinaryOperator>(&inst);

    if (!bo)
    {
        throw std::runtime_error(
            "Could not cast instruction to `BinaryOperator` with `mod`!");
    }

    if (bo->getOpcode() != llvm::Instruction::SRem
        || bo->getOpcode() != llvm::Instruction::URem)
    {
        throw std::runtime_error(
            "Instruction does not have expected opcode `URem` or `SRem`!");
    }

    uint8_t const_op_idx = 1;
    const llvm::ConstantInt* ci
        = llvm::dyn_cast<llvm::ConstantInt>(bo->getOperand(const_op_idx));
    if (!ci)
    {
        throw std::runtime_error(
            "Instruction does not have expected operand idx "
            + std::to_string(const_op_idx) + " constant!");
    }

    return 1.0 * std::log2(ci->getZExtValue())
        / bo->getType()->getIntegerBitWidth();
}
