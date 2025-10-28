#ifndef ENTROPY_EVAL_FLOAT_BOOL_H
#define ENTROPY_EVAL_FLOAT_BOOL_H

#include <stdbool.h>

#include "entropy_eval-thread.h"
#include "entropy_eval-float.h"

typedef bool (*FN_PTR_FLT_BOOL)(_Float16, _Float16);

struct thread_arg_float_bool
{
    struct thread_arg_base base;
    FN_PTR_FLT_BOOL fn;
};

struct threads_arr_float_bool
{
    pthread_t t;
    struct thread_arg_float_bool* t_arg;
};

static void
update_counts_float_bool(uint64_t*, uint64_t*);
static void*
count_outs_float_bool_threaded_stride(void*);
static uint64_t*
count_outs_float_bool_threaded(uint8_t, FN_PTR_FLT_BOOL);
uint64_t* count_outs_float_bool(uint8_t, FN_PTR_FLT_BOOL);

#endif // ENTROPY_EVAL_FLOAT_BOOL_H
