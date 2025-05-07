#include <randgen.hpp>

IF_Randgen::IF_Randgen() :
    random_engine(),
    int_dist(std::numeric_limits<int64_t>::min(),
        std::numeric_limits<int64_t>::max()),
    real_dist(std::numeric_limits<double>::min(),
        std::numeric_limits<double>::max()) { };

IF_Randgen::IF_Randgen(int seed) :
    random_engine(seed),
    int_dist(std::numeric_limits<int64_t>::min(),
        std::numeric_limits<int64_t>::max()),
    real_dist(std::numeric_limits<double>::min(),
        std::numeric_limits<double>::max()) { };
