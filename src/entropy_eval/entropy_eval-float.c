#include "entropy_eval-float.h"

static void
update_counts_float(uint64_t* counts, uint64_t* new_counts, uint8_t bitsize)
{
    for (uint64_t x = 0; x < pow(2, bitsize); ++x)
    {
        counts[x] += new_counts[x];
    }
}

static void*
count_outs_float_threaded_stride(void* t_arg_raw)
{
    struct thread_arg_float* t_arg = (struct thread_arg_float*) t_arg_raw;

    union itof_conv_u val;
    for (uint64_t x = t_arg->base.stride * t_arg->base.id;
        x < t_arg->base.stride * (t_arg->base.id + 1); ++x)
    {
        for (uint64_t y = t_arg->base.r_min; y < t_arg->base.r_max; ++y)
        {
            union itof_conv_u xf = { .i = x };
            union itof_conv_u yf = { .i = y };

            val.f = (t_arg->fn)(xf.f, yf.f);
            t_arg->base.inner_counts[val.i] += 1;
        }
    }
    pthread_exit(NULL);
}

static uint64_t*
count_outs_float_threaded(uint8_t bitsize, FN_PTR_FLT fn)
{
    uint64_t* counts = calloc(pow(2, bitsize), sizeof(uint64_t));

    const uint64_t r_min = 0;
    const uint64_t r_max = pow(2, bitsize);

    if ((r_max - r_min) % THREAD_COUNT_MAX != 0)
    {
        exit(1 && "Invalid threading - leftover elements.");
    }

    uint8_t thread_count = THREAD_COUNT_MAX;
    uint64_t stride = (r_max - r_min) / thread_count;
    struct threads_arr_float* threads
        = calloc(thread_count, sizeof(struct threads_arr_float));

    printf(
        "=== Running with %d threads for %d bits ...\n", thread_count, bitsize);

    for (uint64_t x = 0; x < thread_count; x += 1)
    {

        uint64_t* ic = calloc(pow(2, bitsize), sizeof(uint64_t));
        struct thread_arg_float* t_arg
            = malloc(sizeof(struct thread_arg_float));
        t_arg->base.bit_sz = bitsize;
        t_arg->base.r_min = r_min;
        t_arg->base.r_max = r_max;
        t_arg->fn = fn;
        t_arg->base.id = x;
        t_arg->base.stride = stride;
        t_arg->base.inner_counts = ic;

        threads[x].t_arg = t_arg;
        pthread_create(
            &threads[x].t, NULL, count_outs_float_threaded_stride, t_arg);
    }

    for (uint64_t x = 0; x < thread_count; x += 1)
    {
        pthread_join(threads[x].t, NULL);
        update_counts_float(counts, threads[x].t_arg->base.inner_counts, bitsize);
        free(threads[x].t_arg->base.inner_counts);
        free(threads[x].t_arg);
    }
    free(threads);
    return counts;
}

uint64_t*
count_outs_float(uint8_t bitsize, FN_PTR_FLT fn)
{
    if (bitsize > THREADED_BITSIZE_LIMIT)
    {
        return count_outs_float_threaded(bitsize, fn);
    }

    uint64_t* counts = calloc(pow(2, bitsize), sizeof(uint64_t));

    const uint64_t r_min = 0;
    const uint64_t r_max = pow(2, bitsize);
    if (r_max < r_min)
    {
        exit(1 && "Maximum range computed smaller than minimum range!");
    }

    union itof_conv_u val;
    union itof_conv_u xf;
    union itof_conv_u yf;
    for (uint64_t x = r_min; x < r_max; ++x)
    {
        xf.i = x;
        for (uint64_t y = r_min; y < r_max; ++y)
        {
            yf.i = y;
            val.f = fn(xf.f, yf.f);
            counts[val.i] += 1;
        }
    }

    return counts;
}
