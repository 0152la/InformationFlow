#ifndef _IF_ENTROPY_HPP
#define _IF_ENTROPY_HPP

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <deque>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <numeric>
#include <sstream>
#include <type_traits>
#include <utility>

typedef uint64_t if_in_t;
typedef uint64_t if_out_t;

using obs_t = std::deque<uint64_t>;
using in_out_obs_t = std::map<uint64_t, obs_t>;

/* Class representing an entry in the histogram, recording all outputs for a
 * given input.
 */
template <typename I, typename O> class IF_Histogram_Entry
{
private:
    const I input;
    std::map<O, uint64_t> outputs = std::map<O, uint64_t>();
    size_t out_count = 0;

public:
    IF_Histogram_Entry(I _input) :
        input(_input) { };

    void insert(O);
    void insert_many(O, uint64_t);

    I get_in(void) const { return this->input; };

    auto get_outs(void) const -> decltype(this->outputs)
    {
        return this->outputs;
    };

    size_t get_total_out_count(void) const { return this->out_count; };

    double get_out_count(if_out_t _out) const
    {
        const auto& found = this->outputs.find(_out);
        return found == this->outputs.end() ? 0 : found->second;
    };

    void print(void);
};

/* Class representing an evaluation of some underlying function, gathering all
 * observed inputs and outputs, and allowing us to perform some computations
 * over it. Most interestingly, we can compute the entropies, and uncertainty
 * coefficient.
 */
template <typename I, typename O> class IF_Histogram
{
private:
    using data_inv_t = std::deque<std::unique_ptr<IF_Histogram_Entry<O, I>>>;

public:
    using data_t = std::deque<std::unique_ptr<IF_Histogram_Entry<I, O>>>;

private:
    data_t data;
    uint64_t obs_count = 0;

    obs_t get_input_observations(void) const;
    obs_t get_output_observations(void) const;
    data_inv_t invert_obs(void) const;

    template <typename U> in_out_obs_t count_observations(const U&) const;

public:
    void insert(I, O);
    IF_Histogram_Entry<I, O>* find(I);

    double calculate_entropy_inputs(void);
    double calculate_entropy_outputs(void);
    double calculate_conditional_entropy_out_given_in(void);
    double calculate_conditional_entropy_in_given_out(void);
    double calculate_uncertainty_coefficient_out_given_in(void);
    double calculate_uncertainty_coefficient_in_given_out(void);

    void print_measures(void);
};

double
compute_entropy(const obs_t&, const size_t);
double
compute_conditional_entropy(const in_out_obs_t&, const uint64_t);

#include "entropy.tpp"

#endif // _IF_ENTROPY_HPP
