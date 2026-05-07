#include "main.hpp"

int
main(int argc, char** argv)
{
    auto init_path = [](const std::string& env_var)
    {
        const auto ev_path = std::getenv(env_var.c_str());
        Utils::do_check(ev_path == nullptr,
            fmt::format("Empty value for `{}` given!", env_var));
        return ev_path;
    };
    const auto def_path = init_path("EE_DEF_PATH");
    const auto lib_path = init_path("EE_LIB_PATH");

    auto runner = Runner { lib_path, def_path };
    runner.init_all();

    if (argc == 2 && !strcmp(argv[1], "test"))
    {
        Testing::do_all_tests(runner);
        return 0;
    }

    runner.eval_all();

    // const EntropyResult res = runner.run_one("llvm_impl_fadd");
    // std::cout << res.to_str() << std::endl;

    return 0;
}
