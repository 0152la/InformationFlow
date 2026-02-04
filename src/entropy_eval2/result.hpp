#ifndef _EEVAL_RESULT_HPP
#define _EEVAL_RESULT_HPP

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "fmt/format.h"

#include "config.hpp"

class EvalResult
{
public:
    // For values of type `res_t`, holds counts of type `instance_t`
    using res_t = uint64_t;
    using instance_t = uint64_t;

private:
    // `instances` is where the instance counts are held. The indices are of
    // type `res_t`, up to a max of 2^n bitsize
    instance_t* instances;
    uint64_t instance_count = 0;
    uint8_t bit_sz;

public:
    EvalResult(uint8_t _bs) :
        bit_sz(_bs)
    {
        instances = static_cast<instance_t*>(
            calloc(pow(2, bit_sz), sizeof(instance_t)));
        if (instances == nullptr)
        {
            throw std::runtime_error(
                "Error initializing EvalResult instances!\n");
        }
    }

    ~EvalResult(void) { free(this->instances); }

    void add_result(res_t);

    void combine_results(const EvalResult&);

    auto get_instances(void) const -> const decltype(this->instances)&
    {
        return this->instances;
    }

    instance_t get_instance(size_t idx) const { return this->instances[idx]; };

    auto get_instance_count(void) const -> const decltype(this->instance_count)&
    {
        return this->instance_count;
    }

    auto get_max_instances(void) const -> decltype(instance_count)
    {
        return std::pow(2, sizeof(instance_t));
    }

    res_t get_max_res_val(void) const { return std::pow(2, this->bit_sz); }

    auto get_bit_sz(void) const -> decltype(bit_sz) { return this->bit_sz; }

    void print(void) const;
};

#endif // _EEVAL_RESULT_HPP
