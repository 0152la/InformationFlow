#ifndef _EEVAL_RUNNER_HPP
#define _EEVAL_RUNNER_HPP

#include <array>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <dlfcn.h>
#include <fstream>
#include <functional>
#include <future>
#include <iostream>
#include <numeric>
#include <optional>
#include <regex>
#include <span>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <tuple>
#include <type_traits>
#include <unordered_map>

#include "fmt/base.h"
#include "fmt/color.h"
#include "fmt/format.h"
#include "fmt/ostream.h"

#include "config.hpp"
#include "entropy.hpp"
#include "result.hpp"
#include "utils.hpp"

template <typename From, typename To> union conv_u
{
    From f;
    To t;

    conv_u(From _f) :
        f(_f)
    {
        Utils::do_check(sizeof(From) != sizeof(To),
            fmt::format(
                "Incompatible type conversion: from size {} to size {}.",
                sizeof(From), sizeof(To)));
    };

    To get(void) const { return t; };
};

class def_ty_enum
{
public:
    enum def_ty
    {
        I64,
        I32,
        I16,
        I1,
        F16,
        F32,
    };

private:
    inline static const std::unordered_map<std::string_view, def_ty>
        def_ty_names {
            { "i64", def_ty::I64 },
            { "i32", def_ty::I32 },
            { "i16", def_ty::I16 },
            { "i1", def_ty::I1 },

            { "float", def_ty::F32 },
            { "half", def_ty::F16 },
        };

    enum def_ty_type
    {
        INT,
        FLOAT,
    };

    inline static std::unordered_map<def_ty_type, std::vector<def_ty>>
        def_ty_types { { def_ty_type::INT,
                           {
                               def_ty::I64,
                               def_ty::I32,
                               def_ty::I16,
                               def_ty::I1,
                           } },
            { def_ty_type::FLOAT,
                {
                    def_ty::F32,
                    def_ty::F16,
                } } };

public:
    inline static const std::unordered_map<def_ty, uint8_t> def_ty_fl_bitsizes {
        { def_ty::F16, 16 },
        { def_ty::F32, 32 },
    };

    static const std::string_view to_str(const def_ty& dt)
    {
        const auto it = std::find_if(def_ty_names.begin(), def_ty_names.end(),
            [&dt](const std::pair<std::string_view, def_ty>& name)
            { return name.second == dt; });
        Utils::do_check(it == def_ty_names.end(), "Error converting `def_ty`!");
        return it->first;
    }

    static def_ty from_str(const std::string_view& s)
    {
        return def_ty_names.at(s);
    }

    static bool is_def_ty_int(const def_ty& dt)
    {
        auto ty_vec = def_ty_types.at(def_ty_type::INT);
        return std::find(ty_vec.begin(), ty_vec.end(), dt) != ty_vec.end();
    }

    static bool is_def_ty_float(const def_ty& dt)
    {
        auto ty_vec = def_ty_types.at(def_ty_type::FLOAT);
        return std::find(ty_vec.begin(), ty_vec.end(), dt) != ty_vec.end();
    }
};

struct DefInfoFlags
{
private:
    using sv = std::string_view;
    inline static constexpr std::array div_insts { sv("udiv"), sv("sdiv"),
        sv("urem"), sv("srem") };
    inline static constexpr std::array fop_insts { sv("fadd"), sv("fmul"),
        sv("fsub"), sv("fdiv"), sv("frem"), sv("fcmp"), sv("fpto") };
    inline static constexpr std::array overflow_insts { sv("add"), sv("sub"),
        sv("mul") };
    inline static constexpr std::array shift_insts { sv("shl"), sv("ashr"),
        sv("lshr") };

    template <typename T, size_t N>
    bool check_name_within(const std::array<T, N>&, std::string_view) const;

public:
    bool is_div;
    bool is_fop;
    bool is_overflow;
    bool is_shift;

    DefInfoFlags(std::string_view);
    DefInfoFlags(const DefInfoFlags&);
};

struct DefInfo

{
    unsigned int llvm_opcode;
    unsigned int cmp_opcode;
    std::string extra_fn_name;

    std::string llvm_fn_name;
    def_ty_enum::def_ty ret_ty;
    std::string ret_str;
    std::vector<def_ty_enum::def_ty> params_ty;
    std::string params_str;

    DefInfoFlags* di_flags;

    DefInfo(const std::string&);
    ~DefInfo(void);
    DefInfo(const DefInfo&);
    DefInfo(DefInfo&&) noexcept;
    DefInfo& operator=(const DefInfo&);
    DefInfo& operator=(DefInfo&&) noexcept;

    std::string get_fn_name(void) const;
    std::string get_extra(void) const;
    std::string get_full_name(void) const;
    uint8_t get_param_count(void) const;

    std::string to_str(void) const;

private:
    static constexpr char extra_delim = '_';
};

struct RunInfo
{
    const DefInfo* di;
    void* fn_ptr;
    uint8_t bit_sz_min;
    uint8_t bit_sz_max;

    RunInfo(const DefInfo&, void*);
};

struct InputDataRange
{
    uint64_t start;
    uint64_t end;
    uint64_t step;

    InputDataRange(uint64_t _st, uint64_t _ed) :
        start(_st),
        end(_ed),
        step(1)
    {
        this->check_range();
    };

    void set_start(uint64_t new_st)
    {
        this->start = new_st;
        this->check_range();
    };

    void set_end(uint64_t new_ed)
    {
        this->end = new_ed;
        this->check_range();
    };

    uint64_t get_count(void) const { return this->end - this->start + 1; };

private:
    void check_range(void) const
    {
        Utils::do_check(start > end,
            fmt::format(
                "Invalid range [{}, {}] given!", this->start, this->end));
    }
};

struct InputData
{
    uint8_t parameter_count;
    std::vector<InputDataRange> parameter_ranges;

    InputData(uint8_t, uint8_t, const DefInfoFlags*);
    uint64_t get_input_count(void);
    auto get_input_range(uint8_t) const
        -> decltype(this->parameter_ranges)::const_reference;
};

struct EvalRunInfo
{
    const uint8_t bit_sz_in_min;
    const uint8_t bit_sz_in_max;
    const uint8_t bit_sz_out;
    const DefInfoFlags* di_flags;

    EvalRunInfo(const RunInfo&);

private:
    EvalData::bit_sz_t get_out_bit_sz(const RunInfo&);
};

struct ThreadRunInfo
{
    const EvalRunInfo* eri;
    EvalData::Results local_results;
    std::thread thr;

    ThreadRunInfo(const EvalRunInfo&);
};

class Runner
{
private:
    void* dl_hdl;
    std::string log_fs_path;
    std::string csv_fs_path;
    std::ofstream log_fs;
    std::ofstream csv_fs;

    std::vector<DefInfo> defs;

    const DefInfo& get_def_info(std::string_view) const;

    void logs_os_init(void);
    void logs_os_close(void);
    void log_one_run(const EntropyResult&, const DefInfo&);
    template <typename T, typename R>
    void log_result(EvalData::Results&, R&, EvalData::bit_sz_t) const;

    template <typename T, typename R, typename... As>
    std::span<ThreadRunInfo> eval_threads_start(
        const EvalRunInfo&, const std::function<R(As...)>&) const;
    void eval_threads_join(
        EvalData::Results&, const std::span<ThreadRunInfo>&) const;
    template <typename T, typename R, typename... As>
    void do_eval_thread_init(
        ThreadRunInfo&, InputData, const std::function<R(As...)>&) const;

    template <size_t I, typename T, typename R, typename... As>
    void do_eval(EvalData::Results&, const InputData&,
        const std::function<R(As...)>&, std::tuple<As...>&,
        const DefInfoFlags*) const;

    template <typename T, typename R, typename... As>
    const EvalData::Results dispatch_eval(
        const EvalRunInfo&, const std::function<R(As...)>&) const;
    template <typename T, typename R, typename... Args>
    const EntropyResult exhaust_eval(const RunInfo&) const;

    template <typename T, typename R, typename A>
    const EntropyResult eval_args_forward(const RunInfo&) const;
    template <typename R> const EntropyResult eval_args(const RunInfo&) const;
    const EntropyResult eval_ret(const RunInfo&) const;

public:
    Runner(void);
    ~Runner(void);

    void init_all(void);
    void eval_all(void);

    const EntropyResult run_one(std::string_view) const;
    const EntropyResult run_one(const DefInfo&) const;
};

#include "runner.tpp"

#endif // _EEVAL_RUNNER_HPP
