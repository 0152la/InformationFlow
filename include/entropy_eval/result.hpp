#ifndef _EEVAL_RESULT_HPP
#define _EEVAL_RESULT_HPP

#include <cassert> // TODO make debug assert
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <vector>

#include "fmt/format.h"

#include "config.hpp"
#include "utils.hpp"

namespace EvalData
{
// count of individual output values
using instance_t = uint32_t;
// domain of outputs - expected to fit `2 ^ [max_bit_sz]` values
using res_t = uint32_t;
using bit_sz_t = uint8_t;

// For values of type `res_t`, holds counts of type `instance_t`
// `res_t` - the domain of the output (= 2 ^ output_bit_sz)
// `instance_t` - total input counts
// - for binary operations = 2 ^ (input_bit_sz + 1)
// - for unary operations  = 2 ^ (input_bit_sz)

struct Counter
{
    EvalData::bit_sz_t bit_sz_in;
    EvalData::bit_sz_t bit_sz_out;
    EvalData::bit_sz_t bit_sz_in_max;

    // `instances` is where the instance counts are held. The indices are of
    // type `res_t`, up to a max of 2^n bitsize
    instance_t* instances;
    uint64_t instance_count;

    Counter(EvalData::bit_sz_t _in, EvalData::bit_sz_t _out,
        EvalData::bit_sz_t _in_max);
    Counter(const Counter&);
    Counter(Counter&&) noexcept;
    ~Counter(void);

    Counter& operator=(const Counter&);
    Counter& operator=(Counter&&) noexcept;

    void add_result(EvalData::res_t);
    void combine_results(const Counter&);
    res_t get_max_res_val(void) const;

    std::string to_str(bool = false) const;
};

struct Results
{
private:
    std::vector<Counter> results;

public:
    bit_sz_t bit_sz_in_min;
    bit_sz_t bit_sz_in_max;
    bit_sz_t bit_sz_out;

    Results(EvalData::bit_sz_t _in_min, EvalData::bit_sz_t _in_max,
        EvalData::bit_sz_t _out);
    ~Results(void);

    void add_result(res_t, bit_sz_t);
    void combine_results(const EvalData::Results&);

    auto get_results(void) const -> const decltype(this->results)&;
    auto get_results_for_bitsize(bit_sz_t) const -> const Counter&;
    auto get_results_count(void) const
        -> decltype(std::declval<EvalData::Counter>().instance_count);

    auto get_max_res_val(void) const -> res_t;

    std::string to_str(bool = false) const;
};

};

#endif // _EEVAL_RESULT_HPP
