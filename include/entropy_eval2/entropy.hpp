#ifndef _EEVAL_ENTROPY_HPP
#define _EEVAL_ENTROPY_HPP

#include <chrono>
#include <iostream>
#include <set>
#include <sstream>
#include <stdexcept>

#include "fmt/base.h"
#include "fmt/chrono.h"
#include "fmt/format.h"

#include "utils.hpp"
#include "result.hpp"

class EntropyCalcs
{
public:
    static double compute_entropy(const EvalResult&);
    static double compute_uncertainty_coef(const EvalResult&);
};

struct EntropyResultEntry
{
    uint8_t bit_sz;
    std::chrono::microseconds dur_ms;
    double entropy;
    double uncertainty_coef;

    EntropyResultEntry(
        uint8_t _bs, std::chrono::microseconds _dur, double _h, double _uc) :
        bit_sz(_bs),
        dur_ms(_dur),
        entropy(_h),
        uncertainty_coef(_uc) { };

    const std::string to_str(void) const;
    const std::string to_str_csv(std::string_view) const;
};

struct EntropyResultEntry_cmp
{
    bool operator()(
        const EntropyResultEntry* left, const EntropyResultEntry* right) const
    {
        return left->bit_sz < right->bit_sz;
    }
};

class EntropyResult
{
private:
    std::set<EntropyResultEntry*, EntropyResultEntry_cmp> data;

    // TODO move comparison to this?
    static bool cmp_data(
        const EntropyResultEntry* fst, const EntropyResultEntry* snd)
    {
        return fst->bit_sz < snd->bit_sz;
    }

public:
    ~EntropyResult(void);

    auto get_data(void) const -> const decltype(this->data)&
    {
        return this->data;
    };

    void add_result(EntropyResultEntry*);
    void parse_evalresult(const EvalResult&, std::chrono::microseconds);
    std::string to_str(void) const;
    std::string to_str_csv(std::string_view) const;
};

#endif // _EEVAL_ENTROPY_HPP
