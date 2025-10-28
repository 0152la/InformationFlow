#ifndef ENTROPY_EVAL_INT_CMP_H
#define ENTROPY_EVAL_INT_CMP_H

#include "entropy_eval-thread.h"
#include "entropy_eval-int.h"

typedef bool (*FN_PTR_CMP)(uint64_t, uint64_t);

struct thread_arg_int_cmp
{
    struct thread_arg_base base;
    FN_PTR_CMP fn;
};

struct threads_arr_int_cmp
{
    pthread_t t;
    struct thread_arg_int_cmp* t_arg;
};

static void
update_counts_int_cmp(uint64_t*, uint64_t*);
static void*
count_outs_threaded_stride_int_cmp(void*);
static uint64_t*
count_outs_threaded_int_cmp(uint8_t, FN_PTR_CMP);
uint64_t*
count_outs_int_cmp(uint8_t, FN_PTR_CMP);

#endif // ENTROPY_EVAL_INT_CMP_H
