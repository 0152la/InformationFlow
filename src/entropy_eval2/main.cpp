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
    runner.eval_all();

    //const EntropyResult res = runner.run_one("llvm_impl_fadd");
    //std::cout << res.to_str() << std::endl;

    return 0;
}
