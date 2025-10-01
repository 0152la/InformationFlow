#include "fmt/format.h"

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
const std::string int_ty_str = "i64";
const std::string flt_ty_str = "double";
const std::string bool_ty_str = "i1";

const std::unordered_map<std::string, fn_enum> fn_sig_map {
    { fmt::format("({0})[{0},{0}]", int_ty_str), fn_enum::BINOP_INT },
    { fmt::format("({0})[{0},{0}]", flt_ty_str), fn_enum::BINOP_FLT },
    { fmt::format("({0})[{1},{1}]", bool_ty_str, int_ty_str),
        fn_enum::CMP_INT },
    { fmt::format("({0})[{1},{1}]", bool_ty_str, flt_ty_str),
        fn_enum::CMP_FLT },
    { fmt::format("({0})[{1}]", int_ty_str, flt_ty_str),
        fn_enum::CONV_FLT_TO_INT },
};

using fn_ty = std::variant<fn_ty_binop_int, fn_ty_binop_flt, fn_ty_cmp_int,
    fn_ty_cmp_flt, fn_ty_conv_flt_to_int>;
using emulated_fns2_t = std::unordered_map<std::string, fn_ty>;

