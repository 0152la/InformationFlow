#include "main.hpp"

int
main(int argc, char** argv)
{
    if (argc == 2 && !strcmp(argv[1], "test"))
    {
        Testing::do_all_tests();
        return 0;
    }

    auto runner = Runner {};
    runner.init_all();
    runner.run_one("llvm_impl_sitofp_f16");

    return 0;
}
