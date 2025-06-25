#include "test_harness.hpp"

int
main()
{
    constexpr int8_t entropy = 1;
    EntropyHarness harness(1000, 0.0001);

    uint64_t test_in;
    uint64_t test_out;
    for (size_t i = 0; i < harness.get_sample_count(); ++i)
    {
        test_in = harness.gen<uint64_t>();
        test_out = test_in % (1 << 1);
        harness.log_results(test_in, test_out);
    }

    return !harness.validate_entropy_out(entropy);
}
