#include "fuzz_engine.hpp"
#include <random>

double
IF_FuzzEngine::fuzz_retained_entropy(const llvm::Instruction& instr)
{
    if (instr.getOpcode() != llvm::Instruction::Add)
    {
        std::stringstream err_str;
        err_str << "Unhandled instruction opcode " << instr.getOpcode() << '\n';
        throw std::runtime_error(err_str.str());
    }

    // std::vector<bool> arg_fuzz_mask(instr.getNumOperands());
    // for (unsigned short i = 0; i < arg_fuzz_mask.size(); ++i)
    //{
    // arg_fuzz_mask.at(i)
    //= !llvm::isa<llvm::ConstantInt>(instr.getOperand(i));
    //}

    IF_Histogram<std::pair<int64_t, int64_t>, int64_t> fuzz_hist;
    std::uniform_int_distribution<int64_t> d;

    for (uint64_t i = 0; i < this->fuzz_count; ++i)
    {

        auto ins = std::make_pair(
            this->rng->gen_int<int64_t>(d), this->rng->gen_int<int64_t>(d));

        int64_t out = IF_Emulator::emulate_add<int64_t>(ins.first, ins.second);
        fuzz_hist.insert(ins, out);
    }

    return fuzz_hist.calculate_uncertainty_coefficient_in_given_out();
}
