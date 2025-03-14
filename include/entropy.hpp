#ifndef _IF_ENTROPY_HPP
#define _IF_ENTROPY_HPP

#include <map>
#include <vector>
#include <iterator>
#include <iostream>
#include <sstream>
#include <numeric>
#include <cstdint>
#include <memory>
#include <ranges>

typedef uint64_t if_in_t;
typedef uint64_t if_out_t;

class IF_Histogram_Entry
{
    private:
        const if_in_t input;
        std::vector<if_out_t> outputs;

    public:
        IF_Histogram_Entry(if_in_t);

        void insert(if_out_t);

        if_in_t get_in() { return this->input; };

        void print(void);
};


class IF_Histogram
{
    private:
        std::vector<std::unique_ptr<IF_Histogram_Entry>> data;

    public:
        void insert(if_in_t, if_out_t);
        IF_Histogram_Entry* find(if_in_t);

        void print(void);
};

float
compute_entropy(IF_Histogram&);

#endif // _IF_ENTROPY_HPP
