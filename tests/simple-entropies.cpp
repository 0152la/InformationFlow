#include "test_harness.hpp"

int
main()
{
    constexpr uint16_t in_entropy = 8;
    constexpr uint16_t out_entropy = 6;
    constexpr uint64_t samples = 10000;
    constexpr double error = 0.005;
    static_assert(in_entropy >= out_entropy);

    constexpr uint16_t in_scale = 1 << in_entropy;
    constexpr uint16_t out_scale = 1 << out_entropy;
    EntropyHarness harness(samples, error);

    uint64_t test_in;
    uint64_t test_out;

    static int64_t _counter = 0;
    auto test_fn = [](uint64_t _in) { return (_in + _counter); };

    for (size_t i = 0; i < samples; ++i)
    {
        test_in = harness.gen<uint64_t>() % in_scale;
        test_out = test_fn(test_in) % out_scale;
        harness.log_results(test_in, test_out);
    }

    return !(harness.validate_entropy_in(in_entropy)
        | harness.validate_entropy_out(out_entropy));
}
