#include "entropy.hpp"
#include "randgen.hpp"

#include <iostream>

int
main()
{
    const size_t samples = 3000;
    const uint16_t in_scale = 1 << 8;
    const uint16_t out_scale = 1 << 6;

    IF_Randgen gen(42);
    IF_Histogram hist;

    uint64_t test_in;
    uint64_t test_out;

    static int64_t _counter = 0;
    auto test_fn = [](uint64_t _in)
    {
        _counter += 1;
        return (_in + _counter);
    };

    for (size_t i = 0; i < samples; ++i)
    {
        test_in = gen.gen_unsigned_int(64); // % in_scale;
        test_in %= in_scale;
        test_out = test_fn(test_in) % out_scale;
        hist.insert(test_in, test_out);
    }

    hist.print_measures();

    return 0;
}
