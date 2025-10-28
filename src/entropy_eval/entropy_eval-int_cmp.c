#include "entropy_eval-int_cmp.h"

static void
update_counts_int_cmp(uint64_t* counts, uint64_t* new_counts)
{
    for (uint64_t x = 0; x < 2; ++x)
    {
        counts[x] += new_counts[x];
    }
}

static void*
count_outs_threaded_stride_int_cmp(void* t_arg_raw)
{
    struct thread_arg_int_cmp* t_arg = (struct thread_arg_int_cmp*) t_arg_raw;

    bool val;
    for (uint64_t x = t_arg->base.stride * t_arg->base.id;
        x < t_arg->base.stride * (t_arg->base.id + 1); ++x)
    {
        for (uint64_t y = t_arg->base.r_min; y < t_arg->base.r_max; ++y)
        {
            val = (t_arg->fn)(x, y);
            t_arg->base.inner_counts[val] += 1;
        }
    }
    pthread_exit(NULL);
}

static uint64_t*
count_outs_threaded_int_cmp(uint8_t bitsize, FN_PTR_CMP fn)
{
    uint64_t* counts = calloc(2, sizeof(uint64_t));

    const uint64_t r_min = 0;
    const uint64_t r_max = pow(2, bitsize);

    if ((r_max - r_min) % THREAD_COUNT_MAX != 0)
    {
        exit(1 && "Invalid threading - leftover elements.");
    }

    uint8_t thread_count = THREAD_COUNT_MAX;
    uint64_t stride = (r_max - r_min) / thread_count;
    struct threads_arr_int_cmp* threads
        = calloc(thread_count, sizeof(struct threads_arr_int_cmp));

    printf(
        "=== Running with %d threads for %d bits ...\n", thread_count, bitsize);

    for (uint64_t x = 0; x < thread_count; x += 1)
    {

        uint64_t* ic = calloc(2, sizeof(uint64_t));
        struct thread_arg_int_cmp* t_arg = malloc(sizeof(struct thread_arg_int_cmp));
        t_arg->base.bit_sz = bitsize;
        t_arg->base.r_min = r_min;
        t_arg->base.r_max = r_max;
        t_arg->fn = fn;
        t_arg->base.id = x;
        t_arg->base.stride = stride;
        t_arg->base.inner_counts = ic;

        threads[x].t_arg = t_arg;
        pthread_create(&threads[x].t, NULL, count_outs_threaded_stride_int_cmp, t_arg);
    }

    for (uint64_t x = 0; x < thread_count; x += 1)
    {
        pthread_join(threads[x].t, NULL);
        update_counts_int_cmp(counts, threads[x].t_arg->base.inner_counts);
        free(threads[x].t_arg->base.inner_counts);
        free(threads[x].t_arg);
    }
    free(threads);
    return counts;
}

uint64_t*
count_outs_int_cmp(uint8_t bitsize, FN_PTR_CMP fn)
{
    if (bitsize > THREADED_BITSIZE_LIMIT)
    {
        return count_outs_threaded_int_cmp(bitsize, fn);
    }

    uint64_t* counts = calloc(2, sizeof(uint64_t));

    const uint64_t r_min = 0;
    const uint64_t r_max = pow(2, bitsize);
    bool val;

    for (uint64_t x = r_min; x < r_max; ++x)
    {
        for (uint64_t y = r_min; y < r_max; ++y)
        {
            val = fn(x, y);
            counts[val] += 1;
        }
    }

    return counts;
}
