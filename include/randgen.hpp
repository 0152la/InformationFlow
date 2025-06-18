#ifndef _IF_RANDGEN_HPP
#define _IF_RANDGEN_HPP

#include <memory>
#include <random>

constexpr uint8_t max_bit_width = 64;

class IF_Randgen
{
private:
    std::mt19937_64 random_engine;
    std::vector<std::unique_ptr<std::uniform_int_distribution<int64_t>>>
        s_int_dists;
    std::vector<std::unique_ptr<std::uniform_int_distribution<uint64_t>>>
        u_int_dists;
    std::uniform_real_distribution<double> real_dist;

public:
    IF_Randgen();
    IF_Randgen(int);

    int64_t gen_signed_int(uint16_t = max_bit_width);
    uint64_t gen_unsigned_int(uint16_t = max_bit_width);

    template <typename T> T gen(void);

    double gen_double() { return this->real_dist(this->random_engine); };
};

#endif // _IF_RANDGEN_HPP
