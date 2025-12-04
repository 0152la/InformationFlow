#ifndef _IF_CONST_INST_EMULATOR_HPP
#define _IF_CONST_INST_EMULATOR_HPP

#include <cmath>
#include <stdexcept>
#include <unordered_map>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wall"
#pragma clang diagnostic ignored "-Wunused-parameter"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instruction.h"
#include "llvm/Support/Casting.h"
#include <llvm/IR/InstrTypes.h>
#pragma clang diagnostic pop

class IF_Const_Emulator
{
public:
    static double estimate_mod(const llvm::Instruction&);
};

#endif // _IF_CONST_INST_EMULATOR_HPP
