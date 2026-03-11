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

#include "fmt/format.h"

#include "config.hpp"

struct EvalResultCache;

class EvalResult
{
public:
    // For values of type `res_t`, holds counts of type `instance_t`
    // `res_t` - the domain of the output (= 2 ^ output_bit_sz)
    // `instance_t` - total input counts
    // - for binary operations = 2 ^ (input_bit_sz + 1)
    // - for unary operations  = 2 ^ (input_bit_sz)
    using res_t = uint64_t;
    using instance_t = uint64_t;

private:
    // `instances` is where the instance counts are held. The indices are of
    // type `res_t`, up to a max of 2^n bitsize
    instance_t* instances;
    uint64_t instance_count = 0;
    uint8_t res_bit_sz;

    bool used_cache = false; // XXX for simplicity, assume this means the full
                             // results for previous bit_size were cached;
                             // perhaps TODO later for flexibility

public:
    EvalResult(uint8_t);
    EvalResult(uint8_t, EvalResultCache&);
    ~EvalResult(void);

    void add_result(res_t);
    void combine_results(const EvalResult&);

    auto get_instances(void) const -> const decltype(this->instances)&
    {
        return this->instances;
    }

    auto get_instance(size_t idx) const -> instance_t
    {
        return this->instances[idx];
    };

    auto get_instance_count(void) const -> const decltype(this->instance_count)&
    {
        return this->instance_count;
    }

    auto get_max_res_val(void) const -> res_t
    {
        return std::pow(2, this->res_bit_sz);
    }

    auto get_bit_sz(void) const -> const decltype(res_bit_sz)&
    {
        return this->res_bit_sz;
    }

    auto check_used_cache(void) const -> const decltype(this->used_cache)&
    {
        return this->used_cache;
    }

    auto move_instances(void) -> decltype(this->instances);

    void print(void) const;
};

struct EvalResultCache
{
    EvalResult::instance_t* res;
    bool active;

    EvalResultCache(void) :
        active(false) { };
    ~EvalResultCache(void);

    void set_results(EvalResult&);
    auto move_instances(void) -> decltype(this->res);
};

#endif // _EEVAL_RESULT_HPP
