#ifndef ENTROPY_EVAL_INT_H
#define ENTROPY_EVAL_INT_H

#include <math.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "entropy_eval-thread.h"

#define INT_BITS_MIN 1
#define INT_BITS_MAX 16

typedef uint64_t (*FN_PTR)(uint64_t, uint64_t);

struct thread_arg_int
{
    struct thread_arg_base base;
    FN_PTR fn;
};

struct threads_arr
{
    pthread_t t;
    struct thread_arg_int* t_arg;
};

static void
update_counts(uint64_t*, uint64_t*, uint8_t);
static void*
count_outs_threaded_stride(void*);
static uint64_t*
count_outs_threaded(uint8_t, FN_PTR, bool is_div);
uint64_t*
count_outs(uint8_t, FN_PTR, bool is_div);

#endif // ENTROPY_EVAL_INT_H
