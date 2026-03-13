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

// Check if all values in a tuple fulfill come condition
template <typename Fn, typename Tuple, size_t... Is>
bool
check_tuple_impl(Fn&& fn, Tuple&& t, std::index_sequence<Is...>)
{
    return (fn(std::get<Is>(std::forward<Tuple>(t))) && ...);
}

template <typename Fn, typename Tuple>
bool
check_tuple(Fn&& fn, Tuple&& t)
{
    using T = std::remove_reference_t<Tuple>;
    return check_tuple_impl(std::forward<Fn>(fn), std::forward<T>(t),
        std::make_index_sequence<std::tuple_size_v<T>> {});
}

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
Runner::log_result(EvalResult& er, R& res) const
{
    if constexpr (std::is_integral_v<R>)
    {
        er.add_result(res % er.get_max_res_val());
    }
    else if constexpr (std::is_floating_point_v<R>)
    {
        er.add_result(convert_arg<R, T>(res));
    }
}

template <size_t I, typename T, typename R, typename... As>
void
Runner::do_eval_thread(ThreadRunInfo& tri, const InputData& inputs,
    const std::function<R(As...)>& fn, std::tuple<As...>& curr_args) const
{
    if constexpr (I == sizeof...(As))
    {
        if (tri.used_cache
            && check_tuple([&tri](auto arg)
                { return arg < std::pow(2, tri.eri->out_bit_sz - 1); },
                curr_args))
        {
            return;
        }
        R res = std::apply(fn, curr_args);
        this->log_result<T, R>(tri.local_results, res);
    }
    else
    {
        const auto& curr_input = inputs.get_input_range(I);
        for (size_t x = curr_input.start; x <= curr_input.end;
            x += curr_input.step)
        {
            using x_arg_t = std::tuple_element_t<I, std::tuple<As...>>;
            std::get<I>(curr_args) = convert_arg<T, x_arg_t>(x);
            do_eval_thread<I + 1, T, R, As...>(tri, inputs, fn, curr_args);
        }
    }
}

template <typename T, typename R, typename... As>
void
Runner::do_eval_thread_init(ThreadRunInfo& tri, InputData inputs,
    const std::function<R(As...)>& fn) const
{
    auto tr_args = std::tuple<As...> {};
    this->do_eval_thread<0, T, R, As...>(tri, inputs, fn, tr_args);
}

template <typename T, typename R, typename... As>
std::span<ThreadRunInfo>
Runner::eval_threads_start(
    EvalRunInfo& eri, bool used_cache, const std::function<R(As...)>& fn) const
{
    uint8_t thread_count
        = std::thread::hardware_concurrency() - Config::other_free_threads;

    Utils::do_check(thread_count < Config::min_thread_count,
        fmt::format("Expected {} min threads; found {}!",
            Config::min_thread_count, thread_count));

    Utils::do_check(eri.max_val % thread_count != 0,
        fmt::format("Inexact thread splitting: {} threads over {} values "
                    "leaves {} leftover!",
            thread_count, eri.max_val, eri.max_val % thread_count));

    auto thrs_raw = static_cast<ThreadRunInfo*>(
        calloc(thread_count, sizeof(ThreadRunInfo)));

    for (uint8_t t = 0; t < thread_count; ++t)
    {
        new (&thrs_raw[t]) ThreadRunInfo { eri, used_cache };
        auto id = InputData { sizeof...(As), eri.out_bit_sz, eri.is_div };
        auto& outer_param = id.parameter_ranges.front();
        outer_param.set_start(t);
        outer_param.step = thread_count;

        thrs_raw[t].thr
            = std::thread { &Runner::do_eval_thread_init<T, R, As...>, this,
                  std::ref(thrs_raw[t]), id, fn };
    }

    return std::span(thrs_raw, thread_count);
}

template <typename T, typename R, typename... As>
const EvalResult
Runner::dispatch_eval(EvalRunInfo& eri, EvalResultCache& er_cache,
    const std::function<R(As...)>& fn) const
{
    auto id = InputData { sizeof...(As), eri.out_bit_sz, eri.is_div };
    auto results = EvalResult { eri.out_bit_sz, er_cache };
    if (eri.out_bit_sz >= Config::min_par_bit_sz)
    {
        auto thrs = this->eval_threads_start<T, R, As...>(
            eri, results.check_used_cache(), fn);
        this->eval_threads_join(results, thrs);
        free(thrs.data());
    }
    else
    {
        auto curr_args = std::tuple<As...> {};
        this->do_eval<0, T, R, As...>(results, id, fn, curr_args);
    }

    Utils::do_check(results.get_instance_count() != id.get_input_count(),
            fmt::format("Mismatch for bit size {}: expected {} -- seen {}!",
                eri.out_bit_sz, fmt::group_digits(id.get_input_count()),
                fmt::group_digits(results.get_instance_count())));

    return results;
}

template <size_t I, typename T, typename R, typename... As>
void
Runner::do_eval(EvalResult& results, const InputData& inputs,
    const std::function<R(As...)>& fn, std::tuple<As...>& curr_args) const
{
    if constexpr (I == sizeof...(As))
    {
        if (results.check_used_cache()
            && check_tuple([&results](auto arg)
                { return arg < std::pow(2, results.get_bit_sz() - 1); },
                curr_args))
        {
            return;
        }
        R res = std::apply(fn, curr_args);
        this->log_result<T, R>(results, res);
    }
    else
    {
        for (size_t x = inputs.get_input_range(I).start;
            x <= inputs.get_input_range(I).end; ++x)
        {
            using x_arg_t = std::tuple_element_t<I, std::tuple<As...>>;
            std::get<I>(curr_args) = convert_arg<T, x_arg_t>(x);
            do_eval<I + 1, T, R, As...>(results, inputs, fn, curr_args);
        }
    }
}

template <typename T, typename R, typename... Args>
const EntropyResult
Runner::exhaust_eval(const RunInfo& ri) const
{
    static_assert(std::is_unsigned_v<T> && std::is_integral_v<T>);

    Utils::do_debug_check(sizeof(T) * CHAR_BIT < ri.bit_sz_max, fmt::format(
            "Given type {} (size {}) smaller than maximum bit_sz {}!",
            typeid(T).name(), sizeof(T), ri.bit_sz_max));

    auto fn_c = reinterpret_cast<R (*)(Args...)>(ri.fn_ptr);
    auto fn = static_cast<std::function<R(Args...)>>(fn_c);

    EntropyResult entropy_res;
    if constexpr (std::is_floating_point_v<first_t<Args...>>)
    {
        bool check = true;
        check &= ri.bit_sz_min == ri.bit_sz_max;
        check &= ri.bit_sz_min == sizeof(T) * CHAR_BIT;
        Utils::do_debug_check(check, fmt::format(
                "Expected bit range of single size {}; got [{}, {}]!",
                sizeof(T) * CHAR_BIT, ri.bit_sz_min, ri.bit_sz_max));
    }

    auto cache = EvalResultCache {};
    for (auto b = ri.bit_sz_min; b <= ri.bit_sz_max; ++b)
    {
        auto eval_run_info = EvalRunInfo { b, ri.is_div };
        // std::cout << '\r';
        std::cout << fmt::format("[{}] Running {} {} bits",
            std::chrono::round<std::chrono::seconds>(
                std::chrono::system_clock::now()),
            ri.di->llvm_fn_name, b)
                  << std::flush;
        std::cout << '\n';
        // DEBUG_PRINT("\r[{}] Running {} {} bits",
        // std::chrono::round<std::chrono::seconds>(
        // std::chrono::system_clock::now()),
        // ri.di->llvm_fn_name, b);
        auto time_start = Config::clock_ty::now();
        auto results
            = this->dispatch_eval<T, R, Args...>(eval_run_info, cache, fn);
        auto time_end = Config::clock_ty::now();
        auto time_dur = std::chrono::duration_cast<std::chrono::microseconds>(
            time_end - time_start);
        entropy_res.parse_evalresult(results, time_dur);
        if (b >= Config::min_par_bit_sz - 2)
        {
             //cache.set_results(results);
        }
    }
    std::cout << '\r';
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
