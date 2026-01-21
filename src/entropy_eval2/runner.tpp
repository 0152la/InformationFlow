#include <stdexcept>
#include <tuple>
#include <type_traits>

// Enforce `is_floating_point` check on `_Float16` to return true
namespace std
{
template <> struct is_floating_point<_Float16> : true_type
{
};
}

template <typename From, typename To>
To
convert_arg(From init_val)
{
    if (std::is_same_v<From, To>)
    {
        return init_val;
    }
    return conv_u<From, To>(init_val).get();
}

template <typename T, typename R>
void
Runner::log_result(EvalResult& er, R res, size_t max_val) const
{
    if constexpr (std::is_integral_v<R>)
    {
        er.add_result(res % max_val);
    }
    else if constexpr (std::is_floating_point_v<R>)
    {
        er.add_result(static_cast<T>(res));
    }
}

template <typename T, typename R, typename A>
const EvalResult
Runner::do_eval(const std::function<R(A)> fn, uint8_t bit_sz, bool is_div) const
{
    bool is_div2 = is_div; // TODO fix
    EvalResult er(bit_sz);
    size_t max_val = std::pow(2, bit_sz);
    for (size_t x = 0; x < max_val; ++x)
    {
        A x_a = convert_arg<T, A>(x);
        R res = fn(x_a);
        this->log_result<T, R>(er, res, max_val);
    }
    return er;
}

template <typename T, typename R, typename A1, typename A2>
const EvalResult
Runner::do_eval_thread(const std::function<R(A1, A2)> fn, uint8_t bit_sz,
    bool is_div, size_t _tid, size_t stride) const
{
    static_assert(std::is_same_v<A1, A2>);
    EvalResult er(bit_sz);
    size_t max_val_out = (_tid + 1) * stride;
    size_t max_val_in = std::pow(2, bit_sz);
    for (T x = _tid * stride; x < max_val_out; ++x)
        for (T x = 0; x < max_val; ++x)
        {
            A1 x_a = convert_arg<T, A1>(x);
            for (T y = (is_div ? 1 : 0); y < max_val_in; ++y)
            {
                A2 y_a = convert_arg<T, A2>(y);
                R res = fn(x_a, y_a);
                this->log_result<T, R>(er, res, max_val); // TODO make local
                if (y == max_val_in - 1)
                {
                    break;
                }
            }
            if (x == max_val_out - 1)
            {
                break;
            }
        }
    return er;
}

template <typename T, typename R, typename... Args>
const EvalResult
Runner::do_eval_thread_wrapper(const std::function<R(Args...)> fn,
    uint8_t bit_sz, bool is_div, unsigned int thr_count) const
{
    EvalResult er(bit_sz);

    size_t outer_vals = std::pow(2, bit_sz);
    if (outer_vals % thr_count != 0)
    {
        throw std::runtime_error(
            "Invalid thread count {} - inexact division over {} values!",
            thr_count, outer_vals);
    }
    size_t stride = outer_vals / thr_count;
    std::thread* thrs = reinterpret_cast<std::thread*>(
        calloc(thr_count, sizeof(std::thread)));
    for (size_t t = 0; t < thr_count; ++t)
    {
        thrs[t] = std::thread(do_eval_thread<T, R, Args...>(fn, bit_sz, is_div, t,stride));
    }

    for (size_t t = 0; t < thr_count; ++t)
    {
        thrs[t].join();
        er.combine_results(
    }
}

template <typename T, typename R, typename A1, typename A2>
const EvalResult
Runner::do_eval(
    const std::function<R(A1, A2)> fn, uint8_t bit_sz, bool is_div) const
{
    static_assert(std::is_same_v<A1, A2>);

    EvalResult er(bit_sz);
    size_t max_val = std::pow(2, bit_sz);
    for (T x = 0; x < max_val; ++x)
    {
        A1 x_a = convert_arg<T, A1>(x);
        for (T y = (is_div ? 1 : 0); y < max_val; ++y)
        {
            A2 y_a = convert_arg<T, A2>(y);
            R res = fn(x_a, y_a);
            this->log_result<T, R>(er, res, max_val);
            if (y == max_val - 1)
            {
                break;
            }
        }
        if (x == max_val - 1)
        {
            break;
        }
    }
    return er;
}

template <typename... As>
using first_t = std::tuple_element_t<0, std::tuple<As...>>;

template <typename T, typename R, typename... Args>
const EntropyResult
Runner::exhaust_eval_thread(const RunInfo& ri) const
{
    static_assert(std::is_unsigned_v<T> && std::is_integral_v<T>);

    if (sizeof(T) * CHAR_BIT < ri.bit_sz_max)
    {
        const auto err = fmt::format(
            "Given type {} (size {}) smaller than maximum bit_sz {}!",
            typeid(T).name(), sizeof(T), ri.bit_sz_max);
        throw std::runtime_error(err);
    }

    auto fn_c = reinterpret_cast<R (*)(Args...)>(ri.fn_ptr);
    auto fn = static_cast<std::function<R(Args...)>>(fn_c);

    unsigned int thr_count
        = std::thread::hardware_concurrency() - this->other_free_threads;
    if (thr_count < this->min_thread_count)
    {
        throw std::runtime_error(
            fmt::format("Expected {} min threads; found {}!",
                this->min_thread_count, thr_count));
    }

    EntropyResult er;
    if (std::is_floating_point_v<first_t<Args...>>)
    {
        bool check = true;
        check &= ri.bit_sz_min == ri.bit_sz_max;
        check &= ri.bit_sz_min == sizeof(T) * CHAR_BIT;
        if (!check)
        {
            throw std::runtime_error(fmt::format(
                "Expected bit range of single size {}; got [{}, {}]!",
                sizeof(T) * CHAR_BIT, ri.bit_sz_min, ri.bit_sz_max));
        }
    }

    for (size_t b = ri.bit_sz_min; b <= ri.bit_sz_max; ++b)
    {
        fmt::println("DO {}", b);
        std::chrono::time_point<EntropyResultEntry::clock_ty> time_start
            = EntropyResultEntry::clock_ty::now();
        const EvalResult eval_res = this->do_eval_thread_wrapper<T, R, Args...>(
            fn, b, ri.is_div, thr_count);
        std::chrono::time_point<EntropyResultEntry::clock_ty> time_end
            = EntropyResultEntry::clock_ty::now();
        auto time_dur = std::chrono::duration_cast<std::chrono::microseconds>(
            time_end - time_start);
        er.parse_evalresult(eval_res, time_dur);
    }
    return er;
}

template <typename T, typename R, typename... Args>
const EntropyResult
Runner::exhaust_eval(const RunInfo& ri) const
{
    static_assert(std::is_unsigned_v<T> && std::is_integral_v<T>);

    if (sizeof(T) * CHAR_BIT < ri.bit_sz_max)
    {
        const auto err = fmt::format(
            "Given type {} (size {}) smaller than maximum bit_sz {}!",
            typeid(T).name(), sizeof(T), ri.bit_sz_max);
        throw std::runtime_error(err);
    }

    auto fn_c = reinterpret_cast<R (*)(Args...)>(ri.fn_ptr);
    auto fn = static_cast<std::function<R(Args...)>>(fn_c);

    EntropyResult er;
    if (std::is_floating_point_v<first_t<Args...>>)
    {
        bool check = true;
        check &= ri.bit_sz_min == ri.bit_sz_max;
        check &= ri.bit_sz_min == sizeof(T) * CHAR_BIT;
        if (!check)
        {
            throw std::runtime_error(fmt::format(
                "Expected bit range of single size {}; got [{}, {}]!",
                sizeof(T) * CHAR_BIT, ri.bit_sz_min, ri.bit_sz_max));
        }
    }

    for (size_t b = ri.bit_sz_min; b <= ri.bit_sz_max; ++b)
    {
        fmt::println("DO {}", b);
        std::chrono::time_point<EntropyResultEntry::clock_ty> time_start
            = EntropyResultEntry::clock_ty::now();
        const EvalResult eval_res
            = this->do_eval<T, R, Args...>(fn, b, ri.is_div);
        std::chrono::time_point<EntropyResultEntry::clock_ty> time_end
            = EntropyResultEntry::clock_ty::now();
        auto time_dur = std::chrono::duration_cast<std::chrono::microseconds>(
            time_end - time_start);
        er.parse_evalresult(eval_res, time_dur);
    }
    return er;
}

template <typename T, typename R, typename A>
const EntropyResult
Runner::eval_args_forward(const RunInfo& ri) const
{
    switch (ri.di->params_ty.size())
    {
        case 1:
            return this->exhaust_eval<T, R, A>(ri);
        case 2:
            return this->exhaust_eval<T, R, A, A>(ri);
        default:
            throw std::runtime_error(
                fmt::format("Unhandled argument type with count {}!",
                    ri.di->params_ty.size()));
    }
}

template <typename R>
const EntropyResult
Runner::eval_args(const RunInfo& ri) const
{
    switch (ri.di->params_ty.at(0))
    {
        case def_ty_enum::I64:
            return this->eval_args_forward<uint64_t, R, uint64_t>(ri);
        case def_ty_enum::I32:
            return this->eval_args_forward<uint32_t, R, uint32_t>(ri);
        case def_ty_enum::I16:
            return this->eval_args_forward<uint16_t, R, uint16_t>(ri);
        case def_ty_enum::F32:
            return this->eval_args_forward<uint32_t, R, float>(ri);
        case def_ty_enum::F16:
            return this->eval_args_forward<uint16_t, R, _Float16>(ri);
        default:
            throw std::runtime_error(fmt::format("Unhandled argument type {}!",
                def_ty_enum::to_str(ri.di->params_ty.at(0))));
    }
}
