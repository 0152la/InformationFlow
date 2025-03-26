#include "main.hpp"

static void
gen_one(std::mt19937_64& gen, IF_Histogram& h)
{
    std::mt19937_64::result_type prog_in = gen() % in_scale;
    std::mt19937_64::result_type prog_out = prog_in % out_scale;

    h.insert(prog_in, prog_out);
}

int
main()
{
    std::mt19937_64 gen(42);
    IF_Histogram h;

    std::mt19937_64::result_type prog_in;
    std::mt19937_64::result_type prog_out;

    std::cout << "Initial computations - ";
    for (size_t i = 0; i < initial_tests; ++i)
    {
        prog_in = gen() % in_scale;
        prog_out = prog_in % out_scale;

        h.insert(prog_in, prog_out);
    }

    double entropy_in = h.calculate_entropy_inputs();
    double entropy_out = h.calculate_entropy_outputs();
    std::cout << " H(I) = " << entropy_in << "; H(O) = " << entropy_out
              << std::endl;

    size_t fixed_point_it = 0;
    float diff;
    double e_in_old = entropy_in;
    double e_out_old = entropy_out;
    double change_ratio;
    while (true)
    {
        std::cout << "Iteration " << fixed_point_it << " - ";
        for (size_t j = 0; j < extra_step; ++j)
        {
            gen_one(gen, h);
        }

        entropy_in = h.calculate_entropy_inputs();
        entropy_out = h.calculate_entropy_outputs();
        change_ratio = entropy_in / e_in_old + entropy_out / e_out_old - 2;
        std::cout << " H(I) = " << entropy_in << "; H(O) = " << entropy_out
                  << "; Ratio = " << change_ratio << std::endl;

        if (std::abs(change_ratio) < 2 * change_error
            || fixed_point_it >= max_extra_steps)
        {
            break;
        }

        e_in_old = entropy_in;
        e_out_old = entropy_out;
        fixed_point_it += 1;
    }

    std::cout << "CONDITIONAL ENTROPY == "
              << h.calculate_conditional_entropy_out_given_in() << std::endl;
    return 0;
}
