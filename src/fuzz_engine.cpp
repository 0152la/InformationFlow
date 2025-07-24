#include "fuzz_engine.hpp"

IF_FuzzEngine::args_t
IF_FuzzEngine::gen_args(const llvm::Instruction& instr) const
{
    const uint8_t args_to_gen = instr.getNumOperands(); // TODO check
    if (args_to_gen != 2)
    {
        // TODO fix
        throw std::runtime_error("Shouldn't lmao");
    }

    args_t new_args { this->rng.gen_signed_int(64),
        this->rng.gen_signed_int(64) };

    return new_args;
}

double
IF_FuzzEngine::fuzz_retained_entropy(const llvm::Instruction& instr)
{
    constexpr unsigned short args_count = 2;
    if (args_count != instr.getNumOperands())
    {
        std::ostringstream err;
        err << "Found instruction `" << instr.getOpcodeName() << "` with "
            << instr.getNumOperands() << " operands help";
        throw std::runtime_error(err.str());
    }

    std::array<int64_t, args_count> args;
    std::bitset<args_count> args_mask;
    for (unsigned short i = 0; i < args_count; ++i)
    {
        if (llvm::ConstantInt* ci
            = llvm::dyn_cast<llvm::ConstantInt>(instr.getOperand(i)))
        {
            args.at(i) = ci->getZExtValue();
            args_mask.set(i);
        }
    }

    binops_i64_t fn = this->emu.get_emulated_fn(instr);

    if (!instr.getType()->isIntegerTy())
    {
        // TODO remove this once we implement more argument types
        throw std::runtime_error("Invalid argument type; expected integer!");
    }

    const uint8_t bit_width = instr.getType()->getIntegerBitWidth();

    IF_Histogram fuzz_hist;

    for (uint64_t i = 0; i < this->fuzz_count; ++i)
    {
        for (uint8_t j = 0; j < args_count; ++j)
        {
            if (!args_mask.test(j))
            {
                args.at(j) = this->rng.gen_signed_int(64);
            }
        }
        int64_t out_arg = fn(args.at(0), args.at(1));
        fuzz_hist.insert(std::hash<std::array<int64_t, args_count>> {}(args),
            std::hash<int64_t> {}(out_arg));
    }

    return fuzz_hist.calculate_uncertainty_coefficient_in_given_out();
}
