#include <algorithm>
#include <iostream>

#include "entropy.hpp"
#include "randgen.hpp"

class EntropyHarness
{
private:
    const unsigned long samples;
    const float error;
    const float expected_entropy;

    IF_Randgen generator;
    IF_Histogram<uint64_t, uint64_t> results;

public:
    EntropyHarness(unsigned long _samples, float _error, float _expected)
        : samples(_samples)
        , error(_error)
        , expected_entropy(_expected)
    {
        this->generator = IF_Randgen();
        this->results = IF_Histogram<uint64_t, uint64_t>();
    };

    // Getters
    unsigned long get_sample_count() { return this->samples; };

    double get_expected_low()
    {
        return this->expected_entropy * (1 - this->error);
    };

    double get_expected_high()
    {
        return this->expected_entropy * (1 + this->error);
    };

    // Generators
   int64_t gen_int64_t() { return this->generator.gen_int64_t(); };

    // Loggers
    void log_results(int64_t in_result, int64_t out_result)
    {
        this->results.insert(in_result, out_result);
    };

    // Computations
    double compute_entropy_out()
    {
        return this->results.calculate_entropy_outputs();
    };

    // Checkers
    bool check_entropy(const float to_check)
    {
        return std::clamp(to_check, expected_entropy * (1 - error),
                   expected_entropy * (1 + error))
            == to_check;
    }
};
