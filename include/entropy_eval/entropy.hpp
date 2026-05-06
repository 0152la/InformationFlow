#ifndef _EEVAL_ENTROPY_HPP
#define _EEVAL_ENTROPY_HPP

#include <chrono>
#include <iostream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <type_traits>

#include "fmt/base.h"
#include "fmt/chrono.h"
#include "fmt/format.h"

#include "result.hpp"
#include "utils.hpp"

class EntropyCalcs
{
public:
    static double compute_entropy(const EvalData::Counter&, bool);
    static double compute_uncertainty_coef_given_entropy(
        double, const EvalData::Counter&);
};

struct EntropyResultEntry
{
    EvalData::bit_sz_t bit_sz;
    double entropy;
    double uncertainty_coef;
    bool overflow;

    EntropyResultEntry(const EvalData::Counter&, bool);

    const std::string to_str(void) const;
    const std::string to_str_csv(std::string_view, uint8_t) const;
};

struct EntropyResultEntry_cmp
{
    bool operator()(
        const EntropyResultEntry& left, const EntropyResultEntry& right) const
    {
        return left.bit_sz < right.bit_sz;
    }
};

class EntropyResult
{
private:
    std::set<EntropyResultEntry, EntropyResultEntry_cmp> data;

    // TODO move comparison to this?
    static bool cmp_data(
        const EntropyResultEntry& fst, const EntropyResultEntry& snd)
    {
        return fst.bit_sz < snd.bit_sz;
    }

public:
    auto get_data(void) const -> const decltype(this->data)&
    {
        return this->data;
    };

    void parse_evalresults(const EvalData::Results&, bool);
    std::string to_str(void) const;
    std::string to_str_csv(std::string_view, uint8_t) const;
};

#endif // _EEVAL_ENTROPY_HPP
