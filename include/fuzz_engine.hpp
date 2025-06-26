#ifndef _IF_FUZZENGINE_HPP
#define _IF_FUZZENGINE_HPP

#include "entropy.hpp"
#include "instr_emulator.hpp"
#include "randgen.hpp"

#include <functional>
#include <map>
#include <memory>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wall"
#pragma clang diagnostic ignored "-Wunused-parameter"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instruction.h"
#pragma clang diagnostic pop

class IF_FuzzEngine
{
public:
    using args_t = std::pair<int64_t, int64_t>;

private:
    IF_Randgen& rng;
    const IF_Emulator& emu;
    uint64_t fuzz_count = 10 * 1000;

    args_t gen_args(const llvm::Instruction&) const;

public:
    IF_FuzzEngine(IF_Randgen& _rng, const IF_Emulator& _emu) :
        rng(_rng),
        emu(_emu) { };

    double fuzz_retained_entropy(const llvm::Instruction&);
};

#endif // _IF_FUZZENGINE_HPP
