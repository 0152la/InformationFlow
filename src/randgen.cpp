#include <randgen.hpp>
#include <random>

IF_Randgen::IF_Randgen() :
    random_engine(),
    real_dist(
        std::numeric_limits<double>::min(), std::numeric_limits<double>::max()),
    s_int_dists(
        std::vector<std::unique_ptr<std::uniform_int_distribution<int64_t>>>(
            max_bit_width)),
    u_int_dists(
        std::vector<std::unique_ptr<std::uniform_int_distribution<uint64_t>>>(
            max_bit_width))
{
    for (uint8_t i = 0; i < max_bit_width; ++i)
    {
        this->s_int_dists.at(i)
            = std::make_unique<std::uniform_int_distribution<int64_t>>(
                1 - (1 << i), 1 << i);
        this->u_int_dists.at(i)
            = std::make_unique<std::uniform_int_distribution<uint64_t>>(
                0, 1 << (i + 1));
    }
};

IF_Randgen::IF_Randgen(int seed) :
    IF_Randgen()
{
    random_engine.seed(seed);
}

int64_t
IF_Randgen::gen_signed_int(uint16_t bit_width)
{
    if (bit_width > max_bit_width)
    {
        throw std::runtime_error("Requested bit width larger than max set!");
    }
    return this->s_int_dists.at(bit_width - 1)->operator()(this->random_engine);
}

uint64_t
IF_Randgen::gen_unsigned_int(uint16_t bit_width)
{
    if (bit_width > max_bit_width)
    {
        throw std::runtime_error("Requested bit width larger than max set!");
    }
    return this->u_int_dists.at(bit_width - 1)->operator()(this->random_engine);
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
