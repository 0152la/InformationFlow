#ifndef _IF_FUZZENGINE_HPP
#define _IF_FUZZENGINE_HPP

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wall"
#pragma clang diagnostic ignored "-Wunused-parameter"
#include "llvm/IR/Instruction.h"
#pragma clang diagnostic pop

class IF_FuzzEngine
{
private:
    const std::unique_ptr<IF_FuzzEngine> engine;

public:
    IF_FuzzEngine() :
        engine(std::make_unique<IF_FuzzEngine>()) { };
    IF_FuzzEngine(int seed) :
        engine(std::make_unique<IF_FuzzEngine>(seed)) { };

    double IF_FuzzEngine(const llvm::Instruction&);
}

#endif // _IF_FUZZENGINE_HPP
