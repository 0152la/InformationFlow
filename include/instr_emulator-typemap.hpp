/* Emulated instruction types *************************************************/
#include <unordered_map>
using fn_ty_binop_int = std::function<int64_t(int64_t, int64_t)>;
using fn_ty_binop_int_c = int64_t (*)(int64_t, int64_t);
using fn_ty_binop_flt = std::function<double(double, double)>;
using fn_ty_binop_flt_c = double (*)(double, double);
using fn_ty_cmp_int = std::function<bool(int64_t, int64_t)>;
using fn_ty_cmp_int_c = bool (*)(int64_t, int64_t);
using fn_ty_cmp_flt = std::function<bool(double, double)>;
using fn_ty_cmp_flt_c = bool (*)(double, double);
using fn_ty_conv_flt_to_int = std::function<int64_t(float)>;
using fn_ty_conv_flt_to_int_c = int64_t (*)(float);

/* Function enum types ********************************************************/
enum fn_enum
{
    BINOP_INT,
    BINOP_FLT,
    CMP_INT,
    CMP_FLT,
    CONV_FLT_TO_INT,
};

/* Signature to enum map ******************************************************/
const std::unordered_map<std::string, fn_enum> fn_sig_map {
    { std::string("(i64)[i64,i64]"), fn_enum::BINOP_INT },
    { std::string("(double)[double,double]"), fn_enum::BINOP_FLT },
    { std::string("(i1)[i64,i64]"), fn_enum::CMP_FLT },
    { std::string("(i1)[double,double]"), fn_enum::CMP_FLT },
    { std::string("(i64)[float]"), fn_enum::CONV_FLT_TO_INT },
};

using fn_ty = std::variant<fn_ty_binop_int, fn_ty_binop_flt, fn_ty_cmp_int,
    fn_ty_cmp_flt, fn_ty_conv_flt_to_int>;
using emulated_fns2_t = std::unordered_map<std::string, fn_ty>;

