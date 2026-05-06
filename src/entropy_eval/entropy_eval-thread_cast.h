#ifndef ENTROPY_EVAL_THREAD_H
#define ENTROPY_EVAL_THREAD_H

#include <pthread.h>

#include "common.h"

// Maximum number of threads to use when performing threaded computation
#define THREAD_COUNT_MAX 8
// The minimum bitsize at which threaded computation is enabled
#define THREADED_BITSIZE_LIMIT 13

/* Struct representing information required to exhaustively count outputs for a
 * given operation
 *
 * - bit_sz_in - Bit size of input space
 * - bit_sz_out - Bit size of output space
 * - inner_min - Minimum value for second operator; used to filter out division
 * by zero
 * - fn - Pointer to function to execute
 * - fn_ty - Type of function to execute, used to determine signature
 */
struct count_info
{
    uint8_t bit_sz_in;
    uint8_t bit_sz_out;
    uint8_t inner_min;
    void* fn;
    enum fn_arg_ty_e fn_ty;
};

/* Struct containing information to pass to underlying threads
 *
 * - bit_sz - Bit size of space we're currently processing
 * - r_min, r_max - Minimum / maximum value for the inner loop (pass this to
 *   have a single point of computation, since for stuff like division, the
 *   minimum value is different)
 * - fn - Pointer to function to be called on

 * - id, stride - used for computation of outer loop index
 * - inner_counts - computed values by this thread
 */
struct thread_arg_base
{
    uint64_t r_max;
    const struct count_info* ci;

    uint8_t id;
    uint64_t stride;

    uint64_t* inner_counts;
};

struct threads_arr
{
    pthread_t t;
    struct thread_arg_base* t_arg;
};

static void
do_one_count(uint64_t, uint64_t, const struct count_info* ci, uint64_t*);

uint64_t*
count_outs(struct count_info*);

static void
update_counts(uint64_t*, uint64_t*, uint8_t);
void*
count_outs_threaded_stride(void*);
uint64_t*
count_outs_threaded(struct count_info*);

#endif // ENTROPY_EVAL_THREAD_H
