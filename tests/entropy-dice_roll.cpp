#include "test_harness.hpp"

#include <cmath>

int
main()
{
    constexpr uint8_t faces = 6;
    const double entropy = std::log2(faces);
    EntropyHarness harness(1000, 0.01);

    uint64_t test_in;
    uint64_t test_out;
    for (size_t i = 0; i < harness.get_sample_count(); ++i)
    {
        test_in = harness.gen<uint64_t>();
        test_out = test_in % faces;
        harness.log_results(test_in, test_out);
    }

    return !harness.validate_entropy_out(entropy);
}
