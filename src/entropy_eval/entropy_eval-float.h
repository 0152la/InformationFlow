#ifndef ENTROPY_EVAL_FLOAT_H
#define ENTROPY_EVAL_FLOAT_H

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

#include "entropy_eval-thread.h"

#define FLOAT_BITS 16

typedef _Float16 (*FN_PTR_FLT)(_Float16, _Float16);

struct thread_arg_float
{
    struct thread_arg_base base;
    FN_PTR_FLT fn;
};

struct threads_arr_float
{
    pthread_t t;
    struct thread_arg_float* t_arg;
};

union itof_conv_u
{
    uint16_t i;
    _Float16 f;
};

static void
update_counts_float(uint64_t*, uint64_t*, uint8_t);
static void*
count_outs_float_threaded_stride(void*);
static uint64_t*
count_outs_float_threaded(uint8_t, FN_PTR_FLT);
uint64_t* count_outs_float(uint8_t, FN_PTR_FLT);

#endif // ENTROPY_EVAL_FLOAT_H
