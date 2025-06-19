#include "randgen.hpp"

IF_Randgen::IF_Randgen() :
    random_engine(),
    real_dist(
        std::numeric_limits<double>::min(), std::numeric_limits<double>::max()),
    int_dist(std::numeric_limits<int64_t>::min(),
        std::numeric_limits<int64_t>::max()) { };

IF_Randgen::IF_Randgen(int seed) :
    IF_Randgen()
{
    random_engine.seed(seed);
}

int64_t
IF_Randgen::gen_signed_int(uint8_t bit_width)
{
    if (bit_width > max_bit_width)
    {
        throw std::runtime_error("Requested bit width larger than max set!");
    }

    decltype(this->int_dist)::param_type p;
    if (bit_width == max_bit_width)
    {
        p = this->int_dist.param();
    }
    else
    {
        p = decltype(this->int_dist)::param_type { -(1 << bit_width),
            (1 << bit_width) - 1 };
    }
    return this->int_dist(this->random_engine, p);
}

uint64_t
IF_Randgen::gen_unsigned_int(uint8_t bit_width)
{
    return (uint64_t) this->gen_signed_int(bit_width);
}

template <>
uint64_t
IF_Randgen::gen<uint64_t>(void)
{
    return this->gen_unsigned_int(64);
}

template <>
int64_t
IF_Randgen::gen<int64_t>(void)
{
    return this->gen_signed_int(64);
}

template <>
uint8_t
IF_Randgen::gen<uint8_t>(void)
{
    return this->gen_unsigned_int(8);
}
