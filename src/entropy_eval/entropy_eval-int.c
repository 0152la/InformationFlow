#include "entropy_eval-int.h"

static void
update_counts(uint64_t* counts, uint64_t* new_counts, uint8_t bitsize)
{
    for (uint64_t x = 0; x < pow(2, bitsize); ++x)
    {
        counts[x] += new_counts[x];
    }
}

static void*
count_outs_threaded_stride(void* t_arg_raw)
{
    struct thread_arg_int* t_arg = (struct thread_arg_int*) t_arg_raw;

    uint64_t val;
    for (uint64_t x = t_arg->base.stride * t_arg->base.id;
        x < t_arg->base.stride * (t_arg->base.id + 1); ++x)
    {
        for (uint64_t y = t_arg->base.r_min; y < t_arg->base.r_max; ++y)
        {
            val = (t_arg->fn)(x, y) % (uint64_t) pow(2, t_arg->base.bit_sz);
            t_arg->base.inner_counts[val] += 1;
        }
    }
    pthread_exit(NULL);
}

static uint64_t*
count_outs_threaded(uint8_t bitsize, FN_PTR fn, bool is_div)
{
    uint64_t* counts = calloc(pow(2, bitsize), sizeof(uint64_t));

    const uint64_t r_min = is_div ? 1 : 0;
    const uint64_t r_max = pow(2, bitsize);

    if ((r_max - r_min) % THREAD_COUNT_MAX != 0)
    {
        exit(1 && "Invalid threading - leftover elements.");
    }

    uint8_t thread_count = THREAD_COUNT_MAX;
    uint64_t stride = (r_max - r_min) / thread_count;
    struct threads_arr* threads
        = calloc(thread_count, sizeof(struct threads_arr));

    printf(
        "=== Running with %d threads for %d bits ...\n", thread_count, bitsize);

    for (uint64_t x = 0; x < thread_count; x += 1)
    {

        uint64_t* ic = calloc(pow(2, bitsize), sizeof(uint64_t));
        struct thread_arg_int* t_arg = malloc(sizeof(struct thread_arg_int));
        t_arg->base.bit_sz = bitsize;
        t_arg->base.r_min = r_min;
        t_arg->base.r_max = r_max;
        t_arg->fn = fn;
        t_arg->base.id = x;
        t_arg->base.stride = stride;
        t_arg->base.inner_counts = ic;

        threads[x].t_arg = t_arg;
        pthread_create(&threads[x].t, NULL, count_outs_threaded_stride, t_arg);
    }

    for (uint64_t x = 0; x < thread_count; x += 1)
    {
        pthread_join(threads[x].t, NULL);
        update_counts(counts, threads[x].t_arg->base.inner_counts, bitsize);
        free(threads[x].t_arg->base.inner_counts);
        free(threads[x].t_arg);
    }
    free(threads);
    return counts;
}

uint64_t*
count_outs(uint8_t bitsize, FN_PTR fn, bool is_div)
{
    if (bitsize > THREADED_BITSIZE_LIMIT)
    {
        return count_outs_threaded(bitsize, fn, is_div);
    }

    uint64_t* counts = calloc(pow(2, bitsize), sizeof(uint64_t));

    const uint64_t r_min = 0;
    const uint64_t r_max = pow(2, bitsize);
    const uint64_t snd_r_min = is_div ? 1 : r_min;
    uint64_t val;

    for (uint64_t x = r_min; x < r_max; ++x)
    {
        for (uint64_t y = snd_r_min; y < r_max; ++y)
        {
            val = fn(x, y) % (uint64_t) pow(2, bitsize);
            counts[val] += 1;
        }
    }

    return counts;
}
