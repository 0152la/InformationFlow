#include "main.hpp"

/*******************************************************************************
 * Helper functions
 ******************************************************************************/

static void
gen_one(IF_Randgen& gen, IF_Histogram& h, auto alter_fn)
{
    uint64_t prog_in = gen.gen_unsigned_int() % in_scale;
    uint64_t prog_out = alter_fn(prog_in) % out_scale;

    h.insert(prog_in, prog_out);
}

/*******************************************************************************
 * Test functions
 ******************************************************************************/

static int
test_entropies(void)
{
    IF_Randgen generator(42);
    IF_Histogram h;

    int64_t prog_in;
    int64_t prog_out;

    static int _add_counter = 0;
    auto fn_add = [](uint64_t _in)
    {
        _add_counter += 1;
        return _in + _add_counter;
    };

    for (size_t i = 0; i < initial_tests; ++i)
    {
        gen_one(generator, h, fn_add);
    }

    // std::cout << "CONDITIONAL ENTROPY (O|I) == "
    //<< h.calculate_conditional_entropy_out_given_in() << std::endl;
    // std::cout << "UNCERTAINTY COEFFICIENT (O|I) == "
    //<< h.calculate_uncertainty_coefficient_out_given_in()
    //<< std::endl;
    // std::cout << "CONDITIONAL ENTROPY (I|O) == "
    //<< h.calculate_conditional_entropy_in_given_out() << std::endl;
    // std::cout << "UNCERTAINTY COEFFICIENT (I|O) == "
    //<< h.calculate_uncertainty_coefficient_in_given_out()
    //<< std::endl;

    std::cout << "Initial computations - ";
    double entropy_in = h.calculate_entropy_inputs();
    double entropy_out = h.calculate_entropy_outputs();
    std::cout << " H(I) = " << entropy_in << "; H(O) = " << entropy_out
              << std::endl;

    return 0;

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
            gen_one(generator, h, fn_add);
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

/*******************************************************************************
 * Dev playground
 *
 *
 ******************************************************************************/

template <typename T>
static void
do_snippet_entropy(std::function<T(T, T)> fn)
{
    IF_Randgen gen(42);
    IF_Histogram h;

    for (size_t i = 0; i < initial_tests; ++i)
    {
        T in1 = gen.gen<T>();
        T in2 = gen.gen<T>();
        T out = fn(in1, in2);

        h.insert(std::hash<std::pair<T, T>> {}(std::make_pair(in1, in2)), out);
    }

    // std::cout << "CONDITIONAL ENTROPY (O|I) == "
    //<< h.calculate_conditional_entropy_out_given_in() << std::endl;
    // std::cout << "UNCERTAINTY COEFFICIENT (O|I) == "
    //<< h.calculate_uncertainty_coefficient_out_given_in()
    //<< std::endl;
    // std::cout << "CONDITIONAL ENTROPY (I|O) == "
    //<< h.calculate_conditional_entropy_in_given_out() << std::endl;
    // std::cout << "UNCERTAINTY COEFFICIENT (I|O) == "
    //<< h.calculate_uncertainty_coefficient_in_given_out()
    //<< std::endl;

    std::cout << "Initial computations - ";
    double entropy_in = h.calculate_entropy_inputs();
    double entropy_out = h.calculate_entropy_outputs();
    std::cout << " H(I) = " << entropy_in << "; H(O) = " << entropy_out
              << std::endl;

    return;
}

// Need to test that the calculated entropy values are right
static int
entropy_dev(void)
{
    return 0;
}

// WIP mapping stuff, probably just the entropy loss values for now
static int
reader_dev(void)
{
    // const std::string ll_path = "/home/andreilascu/Documents/Repos/"
    //"InformationFlow/build/tests/simple-struct.ll";
    // const std::string ll_path = "/home/andreilascu/Documents/Repos/"
    //"InformationFlow/tests/for.ll";
    // const std::string ll_path = "/home/andreilascu/Documents/Repos/"
    //"InformationFlow/build/tests/sample.ll";
    const std::string ll_path = "/home/andreilascu/Documents/Repos/"
                                "InformationFlow/tmp/snip_add.ll";

    IF_Emulator emu { };

    IF_Parser if_p;
    std::unique_ptr<IF_LLVM_Module> if_module = if_p.parse_ll(ll_path);
    std::unique_ptr<IF_EntropyMap::Map> em
        = if_p.make_entropy_map(*if_module->get_module());
    em->set_verbose(true);
    em->print();

    // IF_EM_Graph g(*em, "ll-out.dot");
    // g.draw_callgraph();
    // g.draw_graph();

    // IF_EM_Path_Entropy::Printer p(*em);
    // p.compute_path_entropy(em->get_first_instr());
    // p.print_path_entropy();

    return 0;
}

/*******************************************************************************
 * Main function
 ******************************************************************************/

int
main()
{
    return test_entropies();
    //  return test_emulator();

    // return reader_dev();
}
