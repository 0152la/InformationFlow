#include "entropy_eval.h"

char*
get_curr_time_str(void)
{
    time_t curr_time = time(NULL);
    char* curr_time_buf = malloc(BUF_SZ);
    strftime(curr_time_buf, BUF_SZ, "%Y-%m-%d %H:%M:%S", localtime(&curr_time));
    return curr_time_buf;
}

double
compute_cmp_proportion(uint64_t* counts, uint8_t bitsize)
{
    if (bitsize == 64)
    {
        printf("Need to check 64 bitsize samples are correct.\n");
        exit(1);
    }
    uint64_t expected = pow(2, 2 * bitsize);
    if (counts[0] + counts[1] != expected)
    {
        printf("Number of cmp samples don't match - %lu 0s + %lu 1s != %lu "
               "expected\n",
            counts[0], counts[1], expected);
        exit(1);
    }
    return (counts[0] * 1.0) / expected;
}

double
compute_entropy_out(uint64_t* counts, uint8_t bitsize)
{
    double h_o = 0;
    double prob;
    for (uint64_t x = 0; x < pow(2, bitsize); ++x)
    {
        if (counts[x] == 0)
        {
            continue;
        }

        prob = counts[x] / pow(2, 2 * bitsize);
        h_o += prob * log2(prob);
    }
    return -h_o;
}

double
compute_uncertainty_coef(uint64_t* counts, uint8_t bitsize)
{
    return compute_entropy_out(counts, bitsize) / (2 * bitsize);
}

double
compute_uncertainty_coef_div(uint64_t* counts, uint8_t bitsize)
{
    double h_o = compute_entropy_out(counts, bitsize);
    double h_i = bitsize + log2(pow(2, bitsize) - 1);
    double r = h_o / h_i;
    return r;
    /*return compute_entropy_out(counts, bitsize)*/
    /*/ (bitsize + log2(pow(2, bitsize) + 1));*/
}

static void
compute_fn_ucs(struct fn_uc_data_s* fn_data, void* dl_hdl)
{
    struct timespec t_st;
    struct timespec t_en;
    int clock_res;
    uint64_t* counts;

    for (uint8_t i = 0; i < fn_data->uc_data_count; ++i)
    {
        struct uc_data_s* curr_uc_data = &fn_data->uc_data[i];
        uint8_t b = curr_uc_data->bit_sz;

        char* time_buf = get_curr_time_str();
        printf("[%s] START FN %s -- BITS %hhu (%hhu of %hhu)\n", time_buf,
            fn_data->fn_name, b, i + 1, fn_data->uc_data_count);
        free(time_buf);

        if (clock_gettime(CLOCK_MONOTONIC, &t_st) != 0)
        {
            perror("Error getting start clock!");
            exit(1);
        }

        dlerror();
        void* fn = dlsym(dl_hdl, fn_data->fn_name);
        char* fn_dl_err = dlerror();
        if (fn_dl_err)
        {
            printf("Error linking `%s` :: %s\n", fn_data->fn_name, fn_dl_err);
            exit(1);
        }

        struct count_info* ci = malloc(sizeof(struct count_info));
        ci->bit_sz_in = b;
        switch (fn_data->fn_ty)
        {
            case INT:
            case FLOAT:
                ci->bit_sz_out = b;
                ci->inner_min = 0;
                break;
            case INT_CMP:
            case FLOAT_CMP:
                ci->bit_sz_out = 1;
                ci->inner_min = 0;
                break;
            case INT_DIV:
                ci->bit_sz_out = b;
                ci->inner_min = 1;
                break;
        }
        ci->fn = fn;
        ci->fn_ty = fn_data->fn_ty;

        if (b > THREADED_BITSIZE_LIMIT)
        {
            counts = count_outs_threaded(ci);
        }
        else
        {
            counts = count_outs(ci);
        }

        free(ci);

        switch (fn_data->fn_ty)
        {
            case INT_DIV:
                curr_uc_data->u_coef = compute_uncertainty_coef_div(counts, b);
                break;
            case FLOAT_CMP:
            case INT_CMP:
                curr_uc_data->u_coef = compute_cmp_proportion(counts, b);
                break;
            default:
                curr_uc_data->u_coef = compute_uncertainty_coef(counts, b);
        }

        if (clock_gettime(CLOCK_MONOTONIC, &t_en) != 0)
        {
            perror("Error getting end clock!");
            exit(1);
        }

        curr_uc_data->eval_time
            = (t_en.tv_sec - t_st.tv_sec) + (t_en.tv_nsec - t_st.tv_nsec) / 1e9;
        free(counts);
    }
}

static void
log_fn_ucs(const struct fn_uc_data_s* fn_data, FILE* out_fd)
{
    printf("--- FN %s", fn_data->fn_name);
    int ms;
    printf("\n");
    struct uc_data_s curr_data;

    for (uint8_t i = 0; i < fn_data->uc_data_count; ++i)
    {
        struct uc_data_s* curr_data = &fn_data->uc_data[i];
        printf("\tUC[%hhu] = %f = idx %hhu", curr_data->bit_sz,
            curr_data->u_coef, i);
        printf(" --- TIME %fs", curr_data->eval_time);
        printf("\n");
        fprintf(out_fd, "%s,%d,%f\n", fn_data->fn_name, curr_data->bit_sz,
            curr_data->u_coef);
    }
}

static struct fn_uc_data_s*
make_fn_data(const char* fn_name_raw)
{
    char* fn_name = strstr(fn_name_raw, "llvm_impl_");
    char* fn_name_end = strchr(fn_name, '(');
    *fn_name_end = '\0';

    struct uc_data_s* fn_uc_data;
    uint8_t uc_data_count;
    enum fn_arg_ty_e fn_ty;

    // Check if this is a float operation
    if (strstr(fn_name, "_f") != NULL)
    {
        if (strstr(fn_name, "32") != NULL)
        {
            return NULL;
        }
        uc_data_count = 1;
        fn_uc_data = calloc(uc_data_count, sizeof(struct uc_data_s));
        fn_uc_data[0].bit_sz = FLOAT_BITS;
        if (strstr(fn_name, "fcmp") != NULL)
        {
            fn_ty = FLOAT_CMP;
        }
        else
        {
            fn_ty = FLOAT;
        }
    }
    else
    {
        uc_data_count = INT_BITS_MAX - INT_BITS_MIN + 1;
        fn_uc_data = calloc(uc_data_count, sizeof(struct uc_data_s));
        for (uint8_t b = INT_BITS_MIN; b <= INT_BITS_MAX; ++b)
        {
            fn_uc_data[b - INT_BITS_MIN].bit_sz = b;
        }
        if (strstr(fn_name, "div") != NULL || strstr(fn_name, "rem") != NULL)
        {
            fn_ty = INT_DIV;
        }
        else if (strstr(fn_name, "cmp") != NULL)
        {
            fn_ty = INT_CMP;
        }
        else
        {
            fn_ty = INT;
        }
    }

    struct fn_uc_data_s* fn_data = malloc(sizeof(struct fn_uc_data_s));
    fn_data->fn_name = fn_name;
    fn_data->fn_ty = fn_ty;
    fn_data->uc_data = fn_uc_data;
    fn_data->uc_data_count = uc_data_count;
    return fn_data;
}

const char**
eval_coefs_for_def_fns(const char* def_file_path, void* dl_hdl, FILE* out_fd)
{
    char buf[BUF_SZ];
    FILE* fn_def_fd = fopen(def_file_path, "r");

    while (fgets(buf, sizeof(buf), fn_def_fd))
    {
        struct fn_uc_data_s* fn_data = make_fn_data(buf);
        if (!fn_data)
        {
            continue;
        }

        compute_fn_ucs(fn_data, dl_hdl);
        log_fn_ucs(fn_data, out_fd);

        free(fn_data->uc_data);
        free(fn_data);
    }
    fclose(fn_def_fd);

    return NULL;
}

int
main(void)
{
    const char* lib_path = "/home/andreilascu/Documents/Repos/InformationFlow/"
                           "build/src/libllvm_snippets.so";
    void* dl_hdl = dlopen(lib_path, RTLD_NOW);
    if (!dl_hdl)
    {
        printf("Error opening library :: %s\n", dlerror());
        exit(1);
    }

    const char* def_path = "/home/andreilascu/Documents/Repos/InformationFlow/"
                           "build/src/llvm_snippets.def";

    FILE* out_fd = fopen("entropy_out.csv", "w");
    eval_coefs_for_def_fns(def_path, dl_hdl, out_fd);

    dlclose(dl_hdl);
    fclose(out_fd);

    const char* time_buf = get_curr_time_str();
    printf("[%s] END\n", time_buf);
    free((char*) time_buf);

    return 0;
}
