#include "test_harness.hpp"

#include <cmath>

int
main()
{
    const uint64_t samples = pow(10, 7);
    constexpr uint8_t faces = 6;
    const double entropy = std::log2(faces);
    EntropyHarness harness(samples, 0.01);

    uint64_t test_in;
    uint64_t test_out;
    for (size_t i = 0; i < harness.get_sample_count(); ++i)
    {
        test_in = harness.gen<uint64_t>();
        test_out = test_in % faces;
        harness.log_results(test_in, test_out);
    }

    harness.print_measures();
    return !harness.validate_entropy_out(entropy);
}
