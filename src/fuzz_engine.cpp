#include "fuzz_engine.hpp"
#include <llvm/IR/Instruction.h>
#include <stdexcept>

extern std::map<uint16_t, std::function<IF_Arg(const IF_ArgList&)>>
    emulated_fns;

// TODO current unsigned
IF_ArgList
IF_FuzzEngine::gen_args(uint8_t to_gen, uint8_t bit_width) const
{
    std::vector<std::unique_ptr<IF_Arg>> new_args;
    std::uniform_int_distribution<int64_t> d(0, 2 << bit_width);

    for (size_t i = 0; i < to_gen; ++i)
    {
        new_args.emplace_back(std::make_unique<IF_Arg_Int>(
            this->rng->gen_int<int64_t>(d), bit_width));
    }

    return IF_ArgList(std::move(new_args));
}

double
IF_FuzzEngine::fuzz_retained_entropy(const llvm::Instruction& instr)
{
    // std::vector<bool> arg_fuzz_mask(instr.getNumOperands());
    // for (unsigned short i = 0; i < arg_fuzz_mask.size(); ++i)
    //{
    // arg_fuzz_mask.at(i)
    //= !llvm::isa<llvm::ConstantInt>(instr.getOperand(i));
    //}
    //

    std::function<IF_Arg(const IF_ArgList&)> fn;
    try
    {
            fn = emulated_fns.at(instr.getOpcode());
    }
    catch (const std::out_of_range& e)
    {
        std::cerr << "Found unemulated instruction for `"
                  << llvm::Instruction::getOpcodeName(instr.getOpcode())
                  << "`!\n";
        std::exit(1);
    }

    if (!instr.getType()->isIntegerTy())
    {
        throw std::runtime_error("Invalid argument type; expected integer!");
    }

    const uint8_t bit_width = instr.getType()->getIntegerBitWidth();
    uint8_t args_to_gen = 2; // TODO

    IF_Histogram<std::pair<int64_t, int64_t>, int64_t> fuzz_hist;

    for (uint64_t i = 0; i < this->fuzz_count; ++i)
    {
        IF_ArgList instr_args = this->gen_args(args_to_gen, bit_width);
        auto instr_args_int = instr_args.extract_arg_int();

        IF_Arg out_arg = fn(instr_args);
        int64_t out = static_cast<IF_Arg_Int*>(&out_arg)->get_val();
        auto ins = std::make_pair(
            instr_args_int.at(0)->get_val(), instr_args_int.at(1)->get_val());
        fuzz_hist.insert(ins, out);
    }

    return fuzz_hist.calculate_uncertainty_coefficient_in_given_out();
}
