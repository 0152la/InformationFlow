#ifndef ENTROPY_EVAL_COMMON_H
#define ENTROPY_EVAL_COMMON_H

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* General ********************************************************************/

#define BUF_SZ 256

enum fn_arg_ty_e
{
    INT,
    INT_CMP,
    INT_DIV,
    FLOAT,
    FLOAT_CMP,
    CAST_FTOI,
    CAST_ITOF,
};

/* Integer ********************************************************************/

#define INT_BITS_MIN 2
#define INT_BITS_MAX 30
typedef uint64_t (*FN_PTR)(uint64_t, uint64_t);
typedef bool (*FN_PTR_CMP)(uint64_t, uint64_t);

/* Float **********************************************************************/

#define FLOAT_BITS 16
typedef bool (*FN_PTR_FLT_BOOL)(_Float16, _Float16);
typedef _Float16 (*FN_PTR_FLT)(_Float16, _Float16);

union itof_conv_u
{
    uint16_t i;
    _Float16 f;
};

/* Float **********************************************************************/

typedef uint16_t (*FN_PTR_FTOI)(_Float16);
typedef _Float16 (*FN_PTR_ITOF)(uint16_t);

#endif // ENTROPY_EVAL_COMMON_H
