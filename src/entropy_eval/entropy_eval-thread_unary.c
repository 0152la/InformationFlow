static void
do_one_count_cast(uint64_t x, const struct count_info* ci, uint64_t* counts)
{
    switch (ci->fn_ty)
    {
        case CAST_ITOF:
        {
            union itof_conv_u xf = { .i = x };
            union itof_conv_u val = { .f = ((FN_PTR_ITOF) ci->fn)(xf.i) };
            counts[val.i] += 1;
            /*printf("xf i %hu f %f == val i %hu f %f\n", xf.i, (double) xf.f,*/
                /*val.i, (double) val.f);*/
            break;
        }
        case CAST_FTOI:
        {
            union itof_conv_u xf = { .i = x };
            union itof_conv_u val = { .i = ((FN_PTR_FTOI) ci->fn)(xf.f) };
            counts[val.i] += 1;
            break;
        }
        default:
        {
            printf("Invalid type for cast operation!");
            exit(1);
        }
    }
}

uint64_t*
count_outs_cast(struct count_info* ci)
{
    uint64_t* counts = calloc(pow(2, ci->bit_sz_out), sizeof(uint64_t));

    const uint64_t r_min = 0;
    const uint64_t r_max = pow(2, ci->bit_sz_in);

    for (uint64_t x = r_min; x < r_max; ++x)
    {
        do_one_count_cast(x, ci, counts);
    }

    return counts;
}

static void
update_counts_cast(uint64_t* counts, uint64_t* new_counts, uint8_t bit_sz_out)
{
    for (uint64_t x = 0; x < pow(2, bit_sz_out); ++x)
    {
        counts[x] += new_counts[x];
    }
}

static void*
count_outs_cast_threaded_stride(void* t_arg_raw)
{
    struct thread_arg_base* t_arg = (struct thread_arg_base*) t_arg_raw;

    for (uint64_t x = t_arg->stride * t_arg->id;
        x < t_arg->stride * (t_arg->id + 1); ++x)
    {
        do_one_count_cast(x, t_arg->ci, t_arg->inner_counts);
    }
    pthread_exit(NULL);
}

uint64_t*
count_outs_cast_threaded(struct count_info* ci)
{
    uint64_t* counts = calloc(pow(2, ci->bit_sz_out), sizeof(uint64_t));

    const uint64_t r_max = pow(2, ci->bit_sz_in);

    uint8_t thread_count = THREAD_COUNT_MAX;
    if (r_max % thread_count != 0)
    {
        printf("Thread count %d invalid for max value %lu.\n", thread_count,
            r_max);
        exit(1);
    }
    uint64_t stride = r_max / thread_count;
    struct threads_arr* threads
        = calloc(thread_count, sizeof(struct threads_arr));

    printf("=== Running with %d threads for %d bits ...\n", thread_count,
        ci->bit_sz_in);

    for (uint64_t x = 0; x < thread_count; x += 1)
    {
        uint64_t* ic = calloc(pow(2, ci->bit_sz_out), sizeof(uint64_t));
        struct thread_arg_base* t_arg = malloc(sizeof(struct thread_arg_base));
        t_arg->r_max = r_max;
        t_arg->ci = ci;
        t_arg->id = x;
        t_arg->stride = stride;
        t_arg->inner_counts = ic;

        threads[x].t_arg = t_arg;
        pthread_create(
            &threads[x].t, NULL, count_outs_cast_threaded_stride, t_arg);
    }

    for (uint64_t x = 0; x < thread_count; x += 1)
    {
        pthread_join(threads[x].t, NULL);
        update_counts(counts, threads[x].t_arg->inner_counts, ci->bit_sz_out);
        free(threads[x].t_arg->inner_counts);
        free(threads[x].t_arg);
    }
    free(threads);
    return counts;
}
