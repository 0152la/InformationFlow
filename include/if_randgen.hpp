#ifndef _IF_RANDGEN_HPP
#define _IF_RANDGEN_HPP

#include <random>

class IF_Randgen
{
private:
    std::mt19937_64 random_engine;
    std::uniform_int_distribution<int64_t> int_dist;
    std::uniform_real_distribution<double> real_dist;

public:
    IF_Randgen() { this->random_engine = std::mt19937_64(); };

    IF_Randgen(int seed) { this->random_engine = std::mt19937_64(seed); };

    int64_t gen_int64_t() { return this->int_dist(this->random_engine); };

    double gen_double() { return this->real_dist(this->random_engine); };
};

#endif // _IF_RANDGEN_HPP
