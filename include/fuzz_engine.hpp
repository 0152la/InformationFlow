#ifndef _IF_FUZZENGINE_HPP
#define _IF_FUZZENGINE_HPP

#include "entropy.hpp"
#include "instr_emulator.hpp"
#include "randgen.hpp"

#include <functional>
#include <map>
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
private:
    const std::unique_ptr<IF_Randgen> rng;
    uint64_t fuzz_count = 10 * 1000;

    IF_ArgList gen_args(const llvm::Instruction&) const;

public:
    IF_FuzzEngine() :
        rng(std::make_unique<IF_Randgen>()) { };
    IF_FuzzEngine(int seed) :
        rng(std::make_unique<IF_Randgen>(seed)) { };

    double fuzz_retained_entropy(const llvm::Instruction&);
};

#endif // _IF_FUZZENGINE_HPP
