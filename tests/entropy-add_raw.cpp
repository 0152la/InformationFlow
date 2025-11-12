#include "test_harness.hpp"
#include <limits>

using add_t = uint16_t;

int
main()
{
    constexpr size_t search_partition = 1;
    size_t samples = std::numeric_limits<add_t>::max() / search_partition;
    std::cout << "Using " << samples << " samples ...\n";
    EntropyHarness harness(samples, pow(10, -4));

    add_t in1;
    add_t in2;
    add_t out;

    double progress_percent = 0.05;
    size_t sample_batch_size = std::ceil(samples * progress_percent);
    size_t sample_batch_count = std::ceil(1.0 / progress_percent);

    auto test_func = [](add_t in1, add_t in2) { return in1 + in2; };

    std::cout << "Batches : " << sample_batch_count << '\n';
    std::cout << "Batch size : " << sample_batch_size << '\n';
    std::cout << "Batch expected : " << samples * progress_percent << '\n';
    std::cout << "Total real : " << sample_batch_count * sample_batch_size << '\n';
    std::cout << "Total expected : " << samples << '\n';
    std::cout << "================\n";

    for (size_t i = 0; i < sample_batch_count; ++i)
    {
        for (size_t j = 0; j < sample_batch_size; ++j)
        {
            in1 = harness.gen<add_t>();
            in2 = harness.gen<add_t>();
            out = test_func(in1, in2);
            harness.log_results(
                std::hash<std::pair<add_t, add_t>> {}(std::make_pair(in1, in2)),
                out);
        }
        std::cout << "\rProgress " << (i + 1) * progress_percent * 100 << "%";
        std::cout << " [ " << i * sample_batch_size;
        std::cout << " of " << samples << " ]" << std::flush;
    }
    std::cout << "\nDone\n";

    // for (size_t i = 0; i < harness.get_sample_count(); ++i)
    //{
    // in1 = harness.gen<add_t>();
    // in2 = harness.gen<add_t>();
    // out = test_func(in1, in2);
    // harness.log_results(std::hash<std::pair<add_t, add_t>> {}(
    // std::make_pair(in1, in2)),
    // out);
    //}

    harness.print_measures();
    return 0;
}
