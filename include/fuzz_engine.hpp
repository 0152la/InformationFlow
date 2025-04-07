#ifndef _IF_FUZZENGINE_HPP
#define _IF_FUZZENGINE_HPP

#include <vector>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wall"
#pragma clang diagnostic ignored "-Wunused-parameter"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instruction.h"
#pragma clang diagnostic pop

#include "entropy.hpp"
#include "randgen.hpp"

class IF_FuzzEngine
{
private:
    const std::unique_ptr<IF_Randgen> rng;
    uint64_t fuzz_count = 10 * 1000;

    int64_t rng_int64_t(void) { return this->rng->gen_int64_t(); };

public:
    IF_FuzzEngine() :
        rng(std::make_unique<IF_Randgen>()) { };
    IF_FuzzEngine(int seed) :
        rng(std::make_unique<IF_Randgen>(seed)) { };

    double get_fuzzed_entropy(const llvm::Instruction&);
};

#endif // _IF_FUZZENGINE_HPP
