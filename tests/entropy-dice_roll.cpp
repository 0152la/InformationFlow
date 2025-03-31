#include "test_harness.hpp"

#include <cmath>

int
main()
{
    EntropyHarness harness(1000, 0.01, std::log2(6));

    int64_t test_in;
    int64_t test_out;
    for (size_t i = 0; i < harness.get_sample_count(); ++i)
    {
        test_in = harness.gen_int64_t();
        test_out = test_in % 6;
        harness.log_results(test_in, test_out);
    }

    double test_out_entropy = harness.compute_entropy_out();
    if (!harness.check_entropy(test_out_entropy))
    {
        std::cerr << "Got entropy " << test_out_entropy
                  << "; expected entropy within [" << harness.get_expected_low()
                  << ", " << harness.get_expected_high() << "]! " << std::endl;
        return 1;
    }

    return 0;
}
