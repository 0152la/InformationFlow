#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "entropy_eval-float.h"
#include "entropy_eval-float_bool.h"
#include "entropy_eval-int.h"
#include "entropy_eval-int_cmp.h"

#define BUF_SZ 256

enum fn_arg_ty_e
{
    INT,
    INT_CMP,
    DIV,
    FLOAT,
    FLOAT_CMP,
};

/* Struct representing data for computing the uncertainty coefficient data for
 * a given bitsize
 *
 * - bit_sz - The bitsize for the current computation
 * - u_coef - The value of the computed uncertainty coefficient
 * - eval_time - Wall clock time taken for computation
 */
struct uc_data_s
{
    uint8_t bit_sz;
    double u_coef;
    double eval_time;
};

/* Struct representing the unified uncertainty coefficient data for a given
 * function.
 *
 * - fn_name - The name of the function analysed
 * - fn_ty - Used internally to determine how to evaluate the uncertainty
 * coefficient
 * - uc_data - Array of computed uncertainty coefficients, for all
 * applicable bitsizes
 * - uc_data_count - Number of members in `uc_data`
 */
struct fn_uc_data_s
{
    const char* fn_name;
    enum fn_arg_ty_e fn_ty;
    struct uc_data_s* uc_data;
    uint8_t uc_data_count;
};

const char*
print_curr_time(void);

double
compute_cmp_proportion(uint64_t*, uint8_t);
double
compute_entropy_out(uint64_t*, uint8_t);
double
compute_uncertainty_coef(uint64_t*, uint8_t);
double
compute_uncertainty_coef_div(uint64_t*, uint8_t);

static void
compute_fn_ucs(struct fn_uc_data_s*, void*);

static void
log_fn_ucs(const struct fn_uc_data_s*, FILE*);
static struct fn_uc_data_s*
make_fn_data(const char*);
const char**
eval_coefs_for_def_fns(const char*, void*, FILE*);
