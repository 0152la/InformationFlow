#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint32_t val_t;

struct vals_s
{
    val_t input;
    val_t val_in;
    val_t val_return;
    bool branch;
};

static double
print_entropy_one(const char*, const val_t*, size_t);
static double
compute_entropy(const val_t*, size_t);
static double
compute_u_coef(double, double);
static void
gather_vals(const struct vals_s*, size_t);

void
print_logs(const struct vals_s*, uint64_t);

val_t
one_iter_log(val_t, struct vals_s*);
static val_t one_iter(val_t);
