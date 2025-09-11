#include "randgen.hpp"

IF_Randgen::IF_Randgen() :
    random_engine(),
    real_dist(std::numeric_limits<max_fl_t>::min(),
        std::numeric_limits<max_fl_t>::max()),
    int_dist(std::numeric_limits<max_int_t>::min(),
        std::numeric_limits<max_int_t>::max()) { };

IF_Randgen::IF_Randgen(int seed) :
    IF_Randgen()
{
    random_engine.seed(seed);
}

IF_Randgen::max_int_t
IF_Randgen::gen_signed_int(uint8_t bit_width)
{
    if (bit_width > max_bit_width_int)
    {
        throw std::runtime_error("Requested bit width larger than max set!");
    }

    decltype(this->int_dist)::param_type p;
    if (bit_width == max_bit_width_int)
    {
        p = this->int_dist.param();
    }
    else
    {
        p = decltype(this->int_dist)::param_type { -(1 << (bit_width - 1)),
            (1 << (bit_width - 1)) - 1 };
    }
    return this->int_dist(this->random_engine, p);
}

IF_Randgen::max_uint_t
IF_Randgen::gen_unsigned_int(uint8_t bit_width)
{
    return (uint64_t) this->gen_signed_int(bit_width);
}

IF_Randgen::max_fl_t
IF_Randgen::gen_float(uint8_t bit_width)
{
    decltype(this->real_dist)::param_type p;
    switch (bit_width)
    {
        case 32:
            p = decltype(this->real_dist)::param_type {
                std::numeric_limits<float>::min(),
                std::numeric_limits<float>::max()
            };
            break;
        case 64:
            p = decltype(this->real_dist)::param_type {
                std::numeric_limits<double>::min(),
                std::numeric_limits<double>::max()
            };
            break;
        case max_bit_width_fl:
            p = this->real_dist.param();
            // p = decltype(this->real_dist)::param_type {
            // std::numeric_limits<long double>::min(),
            // std::numeric_limits<long double>::max()
            //};
            break;
        default:
            throw std::runtime_error(
                "Unexpected float bit-size: " + std::to_string(bit_width));
    }
    return this->real_dist(this->random_engine, p);
}

/*******************************************************************************
 * Specialised `gen` functions
 ******************************************************************************/

/* 64 bits ********************************************************************/

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

/* 32 bits ********************************************************************/

template <>
uint32_t
IF_Randgen::gen<uint32_t>(void)
{
    return this->gen_unsigned_int(32);
}

template <>
int32_t
IF_Randgen::gen<int32_t>(void)
{
    return this->gen_signed_int(32);
}

/* 16 bits ********************************************************************/

template <>
uint16_t
IF_Randgen::gen<uint16_t>(void)
{
    return this->gen_unsigned_int(16);
}

template <>
int16_t
IF_Randgen::gen<int16_t>(void)
{
    return this->gen_signed_int(16);
}

/*  8 bits ********************************************************************/

template <>
uint8_t
IF_Randgen::gen<uint8_t>(void)
{
    return this->gen_unsigned_int(8);
}

/* float **********************************************************************/

template <>
float
IF_Randgen::gen<float>(void)
{
    return this->gen_float(32);
}

/* double *********************************************************************/

template <>
double
IF_Randgen::gen<double>(void)
{
    return this->gen_float(64);
}

/* long double ****************************************************************/

template <>
long double
IF_Randgen::gen<long double>(void)
{
    return this->gen_float(128); // TODO double check size here, might be 80
}
