#include <algorithm>
#include <iostream>
#include <sstream>

#include "entropy.hpp"
#include "randgen.hpp"

class EntropyHarness
{
private:
    const unsigned long samples;
    const float error;

    IF_Randgen generator;
    IF_Histogram results;

public:
    EntropyHarness(unsigned long _samples, float _error) :
        samples(_samples),
        error(_error)
    {
        this->generator = IF_Randgen();
        this->results = IF_Histogram();
    };

    /* Getters ****************************************************************/
    inline unsigned long get_sample_count(void) const { return this->samples; };

    inline double get_expected_low(double ref_val) const
    {
        return ref_val * (1 - this->error);
    };

    inline double get_expected_high(double ref_val) const
    {
        return ref_val * (1 + this->error);
    };

    /* Utility ***************************************************************/
    template <typename T> inline T gen(void)
    {
        return this->generator.gen<T>();
    };

    inline void log_results(uint64_t in_result_h, uint64_t out_result_h)
    {
        this->results.insert(in_result_h, out_result_h);
    };

    /* Computations **********************************************************/
    inline double compute_entropy_in(void) const
    {
        return this->results.calculate_entropy_inputs();
    };

    inline double compute_entropy_out(void) const
    {
        return this->results.calculate_entropy_outputs();
    };

    /* Checkers **************************************************************/

    /* Checks whether the given `to_check` value is within `error` rate of the
     * given reference value `ref_val`. Returns true if it is, false otherwise.
     */
    inline bool check_entropy(const float to_check, const float ref_val) const
    {
        return (
            std::clamp(to_check, ref_val * (1 - error), ref_val * (1 + error))
            == to_check);
    }

    /* Validators ************************************************************/
    bool validate_entropy_in(const double ref_val) const
    {
        const double to_check = this->compute_entropy_in();
        return validate_entropy(to_check, ref_val);
    }

    bool validate_entropy_out(const double ref_val) const
    {
        const double to_check = this->compute_entropy_out();
        return validate_entropy(to_check, ref_val);
    }

    bool validate_entropy(const double to_check, const double ref_val) const
    {
        std::ostringstream msg;
        msg << "Expected entropy :: " << ref_val << "\n";
        msg << "Allowed error :: " << this->error * 100 << "%\n";
        msg << "\tmin :: " << this->get_expected_low(ref_val) << "\n";
        msg << "\tmax :: " << this->get_expected_high(ref_val) << "\n";
        msg << "Computed entropy :: " << to_check << "\n";
        double ref_rate = to_check * 100.0 / ref_val;
        msg << "\tactual proportion :: " << ref_rate << "\n";
        msg << "\tactual error " << std::abs(100.0 - ref_rate) << "%\n";
        std::cout << msg.str();
        return check_entropy(to_check, ref_val);
    }

    /* Printing ***************************************************************/
    void print_measures(void) const { this->results.print_measures(); }
};
