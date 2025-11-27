#include "path_flow_test.h"

/* COnstants ******************************************************************/

static const uint8_t bit_sz = 3;
static const size_t val_min = 0;
static const size_t val_max = 1 << bit_sz;
/*static const size_t val_max = UINT8_MAX;*/


/* Printing *******************************************************************/

static double
print_entropy_one(const char* name, const val_t* vals, size_t vs_count)
{
    printf("== %s", name);
    double h = compute_entropy(vals, vs_count);
    printf(" -- ENTROPY %f\n", h);
    return h;
}

static void
print_u_coef_one(const char* name, double in_ent, double out_ent)
{
    printf("== %s", name);
    printf(" -- UNCERTAINTY COEF %f\n", compute_u_coef(in_ent, out_ent));
}

void
print_logs(const struct vals_s* vs, uint64_t vs_count)
{
    for (size_t i = 0; i < vs_count; ++i)
    {
        printf("INPUT %d -- BRANCH %s -- INNER VAL %d -- RETURN %d\n",
            vs[i].input, vs[i].branch ? "true" : "false", vs[i].val_in,
            vs[i].val_return);
    }
}

/* Entropy calculations *******************************************************/

static double
compute_entropy(const val_t* vals, size_t vs_count)
{
    double sum = 0;
    double prob;
    for (size_t i = 0; i < val_max; ++i)
    {
        if (vals[i] == 0)
        {
            continue;
        }
        prob = 1.0 * vals[i] / vs_count;
        sum += prob * log2(prob);
    }
    return -sum;
}

static double
compute_u_coef(double in_ent, double out_ent)
{
    return out_ent / in_ent;

}

/* Logging ********************************************************************/

void
gather_vals(const struct vals_s* vs, size_t vs_count)
{
    val_t* ins = calloc(val_max, sizeof(val_t));
    val_t* ins_true = calloc(val_max, sizeof(val_t));
    val_t* ins_false = calloc(val_max, sizeof(val_t));

    val_t* inners_true = calloc(val_max, sizeof(val_t));
    val_t* inners_false = calloc(val_max, sizeof(val_t));

    val_t* outs = calloc(val_max, sizeof(val_t));
    val_t* outs_true = calloc(val_max, sizeof(val_t));
    val_t* outs_false = calloc(val_max, sizeof(val_t));

    val_t* branches = calloc(2, sizeof(val_t));

    size_t ts_count = 0;
    size_t fs_count = 0;

    for (size_t i = 0; i < vs_count; ++i)
    {
        const struct vals_s cvs = vs[i];
        ins[cvs.input] += 1;
        if (cvs.branch)
        {
            ins_true[cvs.input] += 1;
            outs_true[cvs.val_in] += 1;
            inners_true[cvs.val_in] += 1;
            ts_count += 1;
        }
        else
        {
            ins_false[cvs.input] += 1;
            outs_false[cvs.val_in] += 1;
            inners_false[cvs.val_in] += 1;
            fs_count += 1;
        }
        branches[cvs.branch] += 1;
        outs[cvs.val_return] += 1;
    }

    double in_h = print_entropy_one("INS", ins, vs_count);
    double in_true_h = print_entropy_one("INS TRUE", ins_true, ts_count);
    double in_false_h = print_entropy_one("INS FALSE", ins_false, fs_count);

    double out_h = print_entropy_one("OUTS", outs, vs_count);
    double out_true_h = print_entropy_one("OUTS TRUE", outs_true, ts_count);
    double out_false_h = print_entropy_one("OUTS FALSE", outs_false, fs_count);

    /*double trues_h = print_entropy_one("TRUES", inners_true, vs_count);*/
    /*double falses_h = print_entropy_one("FALSES", inners_false, vs_count);*/

    print_u_coef_one("IN > OUT", in_h, out_h);
    print_u_coef_one("IN TRUE > OUT TRUE", in_true_h, out_true_h);
    print_u_coef_one("IN FALSE > OUT FALSE", in_false_h, out_false_h);


    printf("BRANCHING - True %d [%f%%] - False %d [%f%%]\n", branches[1],
        branches[1] * 100.0 / vs_count, branches[0],
        branches[0] * 100.0 / vs_count);

    free(ins);
    free(ins_true);
    free(ins_false);

    free(outs);
    free(outs_true);
    free(outs_false);

    free(branches);
    free(inners_true);
    free(inners_false);
}

val_t
one_iter_log(val_t in, struct vals_s* vs)
{
    vs->input = in;
    vs->branch = in % 4 == 0;
    if (vs->branch)
    {
        vs->val_in = in / 4;
    }
    else
    {
        vs->val_in = 5;
        /*vs->val_in = in;*/
    }
    vs->val_return = (in + vs->val_in) % val_max;
    return vs->val_return;
}

/* Main ***********************************************************************/

static val_t
one_iter(val_t in)
{
    val_t t;
    if (in % 4 == 0)
    {
        t = in / 4;
    }
    else
    {
        t = 5;
        /*t = in;*/
    }
    return (in + t) % val_max;
}

int
main(void)
{
    assert(val_max > val_min);
    uint64_t vals_count = val_max - val_min;
    struct vals_s* vss = calloc(vals_count, sizeof(struct vals_s));
    for (uint64_t i = val_min; i < val_max; ++i)
    {
        printf("R%ld - ", i);
        assert(one_iter(i) == one_iter_log(i, &vss[i - val_min]));
    }
    printf("\n");

    print_logs(vss, vals_count);
    gather_vals(vss, vals_count);
}
