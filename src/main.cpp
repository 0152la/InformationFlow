#include "main.hpp"

int
main()
{
    std::mt19937_64 gen(42);
    IF_Histogram h;

    std::mt19937_64::result_type prog_in;
    std::mt19937_64::result_type prog_out;
    for (size_t i = 0; i < count; ++i)
    {
        prog_in = gen() % in_scale;
        prog_out = prog_in;

        std::cout << "GEN " << prog_in << " COMP " << prog_out << std::endl;
        h.insert(prog_in, prog_out);
    }
    compute_entropy(h);
    return 0;
}
