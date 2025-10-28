#ifndef ENTROPY_EVAL_THREAD_H
#define ENTROPY_EVAL_THREAD_H

#include <stdint.h>

// Maximum number of threads to use when performing threaded computation
#define THREAD_COUNT_MAX 8
// The minimum bitsize at which threaded computation is enabled
#define THREADED_BITSIZE_LIMIT 13

/* Struct containing information to pass to underlying threads
 *
 * - bit_sz - Bit size of space we're currently processing
 * - r_min, r_max - Minimum / maximum value for the inner loop (pass this to
 *   have a single point of computation, since for stuff like division, the
 *   minimum value is different)
 * - id, stride - used for computation of outer loop index
 * - inner_counts - computed values by this thread
 */
struct thread_arg_base
{
    uint8_t bit_sz;
    uint64_t r_min;
    uint64_t r_max;

    uint8_t id;
    uint64_t stride;

    uint64_t* inner_counts;
};

#endif // ENTROPY_EVAL_THREAD_H
