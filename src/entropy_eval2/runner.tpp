#include <chrono>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>

// Helper templates

// Get the first type of a tuple
template <typename... As>
using first_t = std::tuple_element_t<0, std::tuple<As...>>;

// Check if all templates in a pack are the same
template <typename T, typename... Ts>
struct all_same : std::conjunction<std::is_same<T, Ts>...>
{
};

template <typename... Ts>
inline constexpr bool all_same_v = (sizeof...(Ts) == 0)
    ? true
    : all_same<std::tuple_element_t<0, std::tuple<Ts...>>, Ts...>::value;

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
Runner::log_result(EvalResult& er, R& res, const EvalRunInfo& eri) const
{
    if constexpr (std::is_integral_v<R>)
    {
        er.add_result(res % eri.max_val);
    }
    else if constexpr (std::is_floating_point_v<R>)
    {
        // TODO check further
        if (std::isnan(static_cast<double>(res)))
        {
            er.add_result(eri.float_nan_val);
            // res = eri.float_nan_val;
        }
        else
        {
            // er.add_result(static_cast<T>(res));
            er.add_result(convert_arg<R, T>(res));
        }
    }
}

template <size_t I, typename T, typename R, typename... As>
void
Runner::do_eval_thread(ThreadRunInfo& tri, const std::function<R(As...)>& fn,
    std::tuple<As...>& curr_args) const
{
    if constexpr (I == sizeof...(As))
    {
        R res = std::apply(fn, curr_args);
        this->log_result<T, R>(tri.local_results, res, *tri.eri);
    }
    else
    {
        size_t arg_val_min = (I == 0) ? tri.tid * tri.stride : 0;
        size_t arg_val_max
            = (I == 0) ? (tri.tid + 1) * tri.stride : tri.eri->max_val;
        bool handled_nan = false;
        for (size_t x = arg_val_min; x < arg_val_max; ++x)
        {
            if constexpr (I == Config::divisor_idx)
            {
                if (tri.eri->is_div && x == 0)
                {
                    continue;
                }
            }

            using x_arg_t = std::tuple_element_t<I, std::tuple<As...>>;
            std::get<I>(curr_args) = convert_arg<T, x_arg_t>(x);
            if constexpr (std::is_floating_point_v<x_arg_t>)
            {
                if (std::isnan(static_cast<double>(std::get<I>(curr_args))))
                {
                    if (handled_nan)
                    {
                        continue;
                    }
                    handled_nan = true;
                }
            }
            do_eval_thread<I + 1, T, R, As...>(tri, fn, curr_args);
        }
    }
}

template <typename T, typename R, typename... As>
void
Runner::do_eval_thread_init(
    ThreadRunInfo& tri, const std::function<R(As...)>& fn) const
{
    auto tr_args = std::tuple<As...> {};
    this->do_eval_thread<0, T, R, As...>(tri, fn, tr_args);
}

template <typename T, typename R, typename... As>
std::span<ThreadRunInfo>
Runner::eval_threads_start(
    EvalRunInfo& eri, const std::function<R(As...)>& fn) const
{
    uint8_t thread_count
        = std::thread::hardware_concurrency() - Config::other_free_threads;
    if (thread_count < Config::min_thread_count)
    {
        throw std::runtime_error(
            fmt::format("Expected {} min threads; found {}!",
                Config::min_thread_count, thread_count));
    }
    if (eri.max_val % thread_count != 0)
    {
        throw std::runtime_error(
            fmt::format("Inexact thread splitting: {} threads over {} values "
                        "leaves {} leftover!",
                thread_count, eri.max_val, eri.max_val % thread_count));
    }

    auto thrs_raw = static_cast<ThreadRunInfo*>(
        calloc(thread_count, sizeof(ThreadRunInfo)));

    fmt::println(
        "== [{:%Y-%m-%d == %H:%M:%S}] Running bit size {} with {} threads",
        std::chrono::round<std::chrono::seconds>(
            std::chrono::system_clock::now()),
        eri.bit_sz, thread_count);

    for (uint8_t t = 0; t < thread_count; ++t)
    {
        new (&thrs_raw[t]) ThreadRunInfo { eri, t, eri.max_val / thread_count };
        thrs_raw[t].thr
            = std::thread { &Runner::do_eval_thread_init<T, R, As...>, this,
                  std::ref(thrs_raw[t]), fn };
    }

    return std::span(thrs_raw, thread_count);
}

template <typename T, typename R, typename... As>
void
Runner::dispatch_eval(EvalRunInfo& eri, const std::function<R(As...)>& fn) const
{
    if (eri.bit_sz >= Config::min_par_bit_sz)
    {
        auto thrs = this->eval_threads_start<T, R, As...>(eri, fn);
        this->eval_threads_join(eri, thrs);
        free(thrs.data());
    }
    else
    {
        auto curr_args = std::tuple<As...> {};
        this->do_eval<0, T, R, As...>(eri, fn, curr_args);
    }
}

template <size_t I, typename T, typename R, typename... As>
void
Runner::do_eval(EvalRunInfo& eri, const std::function<R(As...)>& fn,
    std::tuple<As...>& curr_args) const
{
    if constexpr (I == sizeof...(As))
    {
        R res = std::apply(fn, curr_args);
        this->log_result<T, R>(eri.results, res, eri);
    }
    else
    {
        for (size_t x = 0; x < eri.max_val; ++x)
        {
            if constexpr (I == Config::divisor_idx)
            {
                if (eri.is_div && x == 0)
                {
                    continue;
                }
            }

            using x_arg_t = std::tuple_element_t<I, std::tuple<As...>>;
            std::get<I>(curr_args) = convert_arg<T, x_arg_t>(x);
            do_eval<I + 1, T, R, As...>(eri, fn, curr_args);
        }
    }
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

    EntropyResult entropy_res;
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

    for (auto b = ri.bit_sz_min; b <= ri.bit_sz_max; ++b)
    {
        auto eval_run_info = EvalRunInfo { b, ri.is_div };
        DEBUG_PRINT("DO {}", b);
        auto time_start = Config::clock_ty::now();
        this->dispatch_eval<T, R, Args...>(eval_run_info, fn);
        auto time_end = Config::clock_ty::now();
        auto time_dur = std::chrono::duration_cast<std::chrono::microseconds>(
            time_end - time_start);
        if constexpr (std::is_floating_point_v<R>)
        {
            DEBUG_PRINT(" == NaN count = {} of {} ({}%)",
                eval_run_info.results.get_instance(eval_run_info.float_nan_val),
                eval_run_info.results.get_instance_count(),
                eval_run_info.results.get_instance(eval_run_info.float_nan_val)
                    * 100.0 / eval_run_info.results.get_instance_count());
        }
        entropy_res.parse_evalresult(eval_run_info.results, time_dur);
    }
    return entropy_res;
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
