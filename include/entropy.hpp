#ifndef _IF_ENTROPY_HPP
#define _IF_ENTROPY_HPP

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

typedef std::map<std::pair<if_in_t, if_out_t>, size_t> in_out_obs;

class IF_Histogram_Entry
{
private:
    const if_in_t input;
    std::vector<if_out_t> outputs;

public:
    IF_Histogram_Entry(if_in_t);

    void insert(if_out_t);

    if_in_t get_in(void) { return this->input; };

    std::vector<if_out_t> get_outs(void) { return this->outputs; };

    size_t get_out_count(void) { return this->outputs.size(); };

    void print(void);
};

class IF_Histogram
{
private:
    std::vector<std::unique_ptr<IF_Histogram_Entry>> data;
    uint64_t observations = 0;

    in_out_obs get_in_out_obs(void);

public:
    void insert(if_in_t, if_out_t);
    IF_Histogram_Entry* find(if_in_t);

    double calculate_entropy_inputs(void);
    double calculate_entropy_outputs(void);
    double calculate_conditional_entropy_in_over_out(void);

    void print(void);
};

float
compute_entropy(IF_Histogram&);

#endif // _IF_ENTROPY_HPP
