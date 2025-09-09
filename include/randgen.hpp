#ifndef _IF_RANDGEN_HPP
#define _IF_RANDGEN_HPP

#include <limits>
#include <memory>
#include <random>

constexpr uint8_t max_bit_width_int = 64;
constexpr uint8_t max_bit_width_fl = 128;

class IF_Randgen
{
private:
    using max_int_t = int64_t;
    using max_uint_t = uint64_t;
    using max_fl_t = long double;
    
    std::mt19937_64 random_engine;
    std::uniform_int_distribution<max_int_t> int_dist;
    std::uniform_real_distribution<max_fl_t> real_dist;

public:
    IF_Randgen();
    IF_Randgen(int);

    max_int_t gen_signed_int(uint8_t = max_bit_width_int);
    max_uint_t gen_unsigned_int(uint8_t = max_bit_width_int);

    max_fl_t gen_float(uint8_t = max_bit_width_fl);

    template <typename T> T gen(void);
};

#endif // _IF_RANDGEN_HPP
