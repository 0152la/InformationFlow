#include "fuzz_engine.hpp"
#include <stdexcept>

IF_FuzzEngine::args_t
IF_FuzzEngine::gen_args(const llvm::Instruction& instr) const
{
     const uint8_t args_to_gen = instr.getNumOperands(); // TODO check
     if (args_to_gen != 2)
     {
         throw std::runtime_error("Shouldn't lmao");
     }

    args_t new_args { this->rng->gen_signed_int(64),
        this->rng->gen_signed_int(64) };
    // uint16_t bit_width;
    // for (size_t i = 0; i < args_to_gen; ++i)
    //{
    // assert(instr.getOperand(i)->getType()->isIntegerTy());
    // bit_width
    //= llvm::dyn_cast<llvm::IntegerType>(instr.getOperand(i)->getType())
    //->getBitWidth();
    // new_args.emplace_back(std::make_unique<IF_Arg_Int>(
    // this->rng->gen_unsigned_int(bit_width), bit_width));
    //}

    // if (llvm::isa<llvm::CmpInst>(instr))
    //{
    // new_args.emplace_back(std::make_unique<IF_Cmp_Pred>(
    // llvm::dyn_cast<llvm::CmpInst>(&instr)->getPredicate()));
    //}

    return new_args;
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

    // std::function<IF_Arg(const IF_ArgList&)> fn;
    std::function<int64_t(int64_t, int64_t)> fn;
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
        // TODO remove this once we implement more argument types
        throw std::runtime_error("Invalid argument type; expected integer!");
    }

    const uint8_t bit_width = instr.getType()->getIntegerBitWidth();
    uint8_t args_to_gen = 2; // TODO

    IF_Histogram<args_t, int64_t> fuzz_hist;
    // IF_Histogram<const IF_ArgList, const IF_Arg> fuzz_hist;
    // IF_Histogram<uint64_t, uint64_t> fuzz_hist;

    for (uint64_t i = 0; i < this->fuzz_count; ++i)
    {
        args_t instr_args = this->gen_args(instr); // args_to_gen, bit_width);
        int64_t out_arg = fn(instr_args.first, instr_args.second);

        // auto instr_args_int = instr_args.extract_arg_int();
        // int64_t out = static_cast<IF_Arg_Int*>(&out_arg)->get_val();
        // auto ins = std::make_pair(
        // instr_args_int.at(0)->get_val(), instr_args_int.at(1)->get_val());
        fuzz_hist.insert(instr_args, out_arg);

        // fuzz_hist.insert(instr_args.to_hash(), out_arg.to_hash());
    }

    return fuzz_hist.calculate_uncertainty_coefficient_in_given_out();
}
