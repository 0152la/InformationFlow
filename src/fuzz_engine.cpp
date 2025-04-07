#include "fuzz_engine.hpp"

double
IF_FuzzEngine::get_fuzzed_entropy(const llvm::Instruction& instr)
{
    IF_Histogram fuzz_hist;

    std::vector<bool> arg_fuzz_mask(instr.getNumOperands());
    for (unsigned short i = 0; i < arg_fuzz_mask.size(); ++i)
    {
        arg_fuzz_mask.at(i)
            = !llvm::isa<llvm::ConstantInt>(instr.getOperand(i));
    }

    for (uint64_t i = 0; i < this->fuzz_count; ++i)
    {
        if (instr.getOpcode() != llvm::Instruction::Add)
        {
            std::stringstream err_str;
            err_str << "Unhandled instruction opcode " << instr.getOpcode()
                    << '\n';
            throw std::runtime_error(err_str.str());
        }

        int64_t op1 = this->rng_int64_t();
        int64_t op2 = this->rng_int64_t();
    }

    return fuzz_hist.calculate_uncertainty_coefficient_out_given_in();
}
