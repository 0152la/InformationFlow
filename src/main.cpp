#include "main.hpp"

static void
gen_one(IF_Randgen& gen, IF_Histogram& h, auto alter_fn)
{
    int64_t prog_in = gen.gen_int64_t() % in_scale;
    int64_t prog_out = alter_fn(prog_in) % out_scale;

    h.insert(prog_in, prog_out);
}

static int
entropy_dev(void)
{
    return 0;
}

static int
reader_dev(void)
{
    const std::string ll_path = "/home/andreilascu/Documents/Repos/"
                                "InformationFlow/build/tests/sample.ll";
    IF_Parser if_p;
    std::unique_ptr<IF_LLVM_Module> if_module = if_p.parse_ll(ll_path);
    if_p.make_entropy_map(*if_module->get_module());

    return 0;
}

static int
test_printer(void)
{
    const std::string ll_path = "/home/andreilascu/Documents/Repos/"
                                "InformationFlow/build/tests/sample.ll";
    IF_Parser if_p;
    std::unique_ptr<IF_LLVM_Module> if_module = if_p.parse_ll(ll_path);
    IF_Parser::print_instrs(*if_module->get_module());
    return 0;
}

static int
test_entropies(void)
{
    IF_Randgen generator(42);
    IF_Histogram h;

    int64_t prog_in;
    int64_t prog_out;

    auto fn_id = [](int64_t _in) { return _in; };

    static int _add_counter = 0;
    auto fn_add = [](int64_t _in)
    {
        _add_counter += 1;
        return _in + _add_counter;
    };

    auto fn_out = fn_add;
    for (size_t i = 0; i < initial_tests; ++i)
    {
        gen_one(generator, h, fn_out);
    }

    std::cout << "CONDITIONAL ENTROPY (O|I) == "
              << h.calculate_conditional_entropy_out_given_in() << std::endl;
    std::cout << "UNCERTAINTY COEFFICIENT (O|I) == "
              << h.calculate_uncertainty_coefficient_out_given_in()
              << std::endl;
    std::cout << "CONDITIONAL ENTROPY (I|O) == "
              << h.calculate_conditional_entropy_in_given_out() << std::endl;
    std::cout << "UNCERTAINTY COEFFICIENT (I|O) == "
              << h.calculate_uncertainty_coefficient_in_given_out()
              << std::endl;

    std::cout << "Initial computations - ";
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
            gen_one(generator, h, fn_out);
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

    return 0;
}

int
main()
{
    // return test_printer();
    //  return reader_dev();
    return test_entropies();
}
