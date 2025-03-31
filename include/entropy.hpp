#ifndef _IF_ENTROPY_HPP
#define _IF_ENTROPY_HPP

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <numeric>
#include <sstream>
#include <utility>
#include <vector>

typedef uint64_t if_in_t;
typedef uint64_t if_out_t;

typedef std::map<std::pair<if_in_t, if_out_t>, size_t> in_out_obs_t;
typedef std::vector<uint64_t> obs_t;
typedef std::map<std::pair<if_in_t, if_out_t>, size_t> pair_obs_t;

/* Class representing an entry in the histogram, recording all outputs for a
 * given input.
 */
class IF_Histogram_Entry
{
private:
    const if_in_t input;
    std::map<if_out_t, uint64_t> outputs = std::map<if_out_t, uint64_t>();
    size_t out_count = 0;

public:
    IF_Histogram_Entry(if_in_t _input) :
        input(_input) { };

    void insert(if_out_t);

    if_in_t get_in(void) { return this->input; };

    auto get_outs(void) -> decltype(this->outputs) { return this->outputs; };

    size_t get_total_out_count(void) { return this->out_count; };

    double get_out_count(if_out_t _out)
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
class IF_Histogram
{
private:
    std::vector<std::unique_ptr<IF_Histogram_Entry>> data;
    uint64_t obs_count = 0;

public:
    void insert(if_in_t, if_out_t);
    IF_Histogram_Entry* find(if_in_t);

    double calculate_entropy_inputs(void);
    double calculate_entropy_outputs(void);
    double calculate_conditional_entropy_out_given_in(void);
    double calculate_uncertainty_coefficient_out_given_in(void);

    void print(void);
};

static double
compute_entropy(const obs_t&, const size_t);
static double
compute_conditional_entropy(
    const std::vector<std::unique_ptr<IF_Histogram_Entry>>&, const uint64_t);

#endif // _IF_ENTROPY_HPP
