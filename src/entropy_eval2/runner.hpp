#ifndef _EEVAL_RUNNER_HPP
#define _EEVAL_RUNNER_HPP

#include <chrono>
#include <cstddef>
#include <cstdio>
#include <dlfcn.h>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <type_traits>
#include <unordered_map>

#include "fmt/base.h"
#include "fmt/format.h"

#include "entropy.hpp"
#include "result.hpp"

template <typename From, typename To> union conv_u
{
    From f;
    To t;

    conv_u(From _f) :
        f(_f)
    {
        static_assert(sizeof(From) == sizeof(To));
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
        F16,
        F32
    };

private:
    inline static const std::unordered_map<std::string_view, def_ty>
        def_ty_names {
            { "i64", def_ty::I64 },
            { "i32", def_ty::I32 },
            { "i16", def_ty::I16 },

            { "float", def_ty::F32 },
            { "half", def_ty::F16 },
        };

public:
    inline static const std::unordered_map<def_ty, uint8_t> def_ty_fl_bitsizes {
        { def_ty::F16, 16 },
        { def_ty::F32, 32 },
    };

    static constexpr uint8_t int_min_bit_sz = 2;
    static constexpr uint8_t int_max_bit_sz = 16;

    static const std::string_view to_str(const def_ty& dt)
    {
        const auto it = std::find_if(def_ty_names.begin(), def_ty_names.end(),
            [&dt](const std::pair<std::string_view, def_ty>& name)
            { return name.second == dt; });
        if (it == def_ty_names.end())
        {
            throw std::runtime_error("Error converting `def_ty`!");
        }
        return it->first;
    }

    static def_ty from_str(const std::string_view& s)
    {
        return def_ty_names.at(s);
    }
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

    DefInfo(const std::string&);
    std::string get_fn_name(void) const;
    bool check_div(void) const;
    bool check_fop(void) const;
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
    bool is_div;

    RunInfo(const DefInfo&, void*);
};

class Runner
{
private:
    void* dl_hdl;
    FILE* out_fd;

    static constexpr unsigned int min_thread_count = 2;
    static constexpr unsigned int other_free_threads = 2;

    static constexpr auto def_path = std::string_view {
        "/home/andreilascu/Documents/Repos/InformationFlow/"
        "build/src/llvm_snippets.def"
    };

    static constexpr auto lib_path = std::string_view {
        "/home/andreilascu/Documents/Repos/InformationFlow/"
        "build/src/libllvm_snippets.so"
    };

    static constexpr auto out_path = std::string_view { "entropy_out.csv" };

    template <typename T, typename R>
    void log_result(EvalResult&, R, size_t) const;

    template <size_t I, typename T, typename R, typename... As>
    void do_eval(EvalResult&, const std::function<R(As...)>&, uint8_t, bool,
        std::tuple<As...>&) const;

    template <typename T, typename R, typename... Args>
    const EntropyResult exhaust_eval(const RunInfo&) const;

    template <typename T, typename R, typename A>
    const EntropyResult eval_args_forward(const RunInfo&) const;
    template <typename R> const EntropyResult eval_args(const RunInfo&) const;
    const EntropyResult eval_ret(const RunInfo&) const;

public:
    Runner(void);
    ~Runner(void);

    const EntropyResult run_one(const DefInfo&) const;
    void eval_all(void) const;
};

#include "runner.tpp"

#endif // _EEVAL_RUNNER_HPP

