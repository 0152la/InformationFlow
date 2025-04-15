#ifndef _IF_INSTR_EMULATOR_HPP
#define _IF_INSTR_EMULATOR_HPP

#include <limits>
#include <type_traits>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wall"
#pragma clang diagnostic ignored "-Wunused-parameter"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instruction.h"
#pragma clang diagnostic pop

class IF_Emulator
{
public:
    template <std::integral T> static T emulate_add(T op1, T op2)
    {
        T sum = op1 + op2;
        if (op1 > 0 && op2 > 0 && sum <= 0)
        {
            return std::numeric_limits<T>::max();
        }
        if (op1 < 0 && op2 < 0 && sum >= 0)
        {
            return std::numeric_limits<T>::min();
        }
        return op1 + op2;
    }
};

template <std::integral T>
T
emulate_add(T op1, T op2)

{
    return op1 + op2;
};

#endif // _IF_INSTR_EMULATOR_HPP
