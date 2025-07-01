#ifndef _IF_ENTROPY_HPP
#define _IF_ENTROPY_HPP

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <deque>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>
#include <utility>

/* Class representing an evaluation of some underlying function, gathering all
 * observed inputs and outputs, and allowing us to perform some computations
 * over it. Most interestingly, we can compute the entropies, and uncertainty
 * coefficient.
 */
class IF_Histogram
{
private:
    /* This type represents the number of observed instances for a single value,
     * derived from actual experimental data. We abstract away the actual
     * observed values, since we don't need them to perform entropy computations
     */
    using obs_t = std::deque<uint64_t>;

public:
    using input_hash_t = size_t;
    using output_hash_t = size_t;
    using count_t = size_t;
    using outputs_t = std::unordered_map<output_hash_t, count_t>;
    using data_t = std::unordered_map<input_hash_t, outputs_t>;

private:
    data_t data;
    uint64_t obs_count = 0;

    static inline input_hash_t extract_input_hash(data_t::const_iterator);
    static inline input_hash_t extract_input_hash(data_t::const_reference);
    static inline outputs_t extract_outputs_t(data_t::const_iterator);
    static inline outputs_t extract_outputs_t(data_t::const_reference);
    static inline output_hash_t extract_output_hash(outputs_t::const_iterator);
    static inline output_hash_t extract_output_hash(outputs_t::const_reference);
    static inline count_t extract_count(outputs_t::const_iterator);
    static inline count_t extract_count(outputs_t::const_reference);

    count_t compute_input_count(data_t::const_reference) const;

    obs_t get_input_observations(void) const;
    obs_t get_output_observations(void) const;
    data_t invert_obs(void) const;

    static double compute_entropy(const obs_t&, const size_t);
    static double compute_conditional_entropy(const data_t&, const size_t);

public:
    IF_Histogram() = default;

    void insert(input_hash_t, output_hash_t);
    void insert(input_hash_t, output_hash_t, count_t);

    /* Calculators  **********************************************************/

    double calculate_entropy_inputs(void) const;
    double calculate_entropy_outputs(void) const;
    double calculate_conditional_entropy_out_given_in(void) const;
    double calculate_conditional_entropy_in_given_out(void) const;
    double calculate_uncertainty_coefficient_out_given_in(void) const;
    double calculate_uncertainty_coefficient_in_given_out(void) const;

    /* Utility ***************************************************************/

    void print_measures(void) const;
};

#endif // _IF_ENTROPY_HPP
