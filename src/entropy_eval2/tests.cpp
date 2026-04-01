#include "tests.hpp"

namespace Testing
{

void
check_add(const Runner& eval_runner)
{
    EntropyResult res = eval_runner.run_one("llvm_impl_add");
    for (const auto& r : res.get_data())
    {
        check_result<double>(r.uncertainty_coef, 0.5, "check_add");
    }
    Utils::test_print(res.to_str());
}

void
check_sitofp_f16(const Runner& eval_runner)
{
    EntropyResult res = eval_runner.run_one("llvm_impl_sitofp_f16");
    for (const auto& r : res.get_data())
    {
        check_result_ge<double>(r.uncertainty_coef, 0.5, "check_sitofp_f16");
    }
    Utils::test_print(res.to_str());
}

void
do_one_test(const Runner& eval_runner, std::string_view test_name,
    Testing::test_fn_ty test_fn)
{
    Utils::test_print(fmt::format("START `{}`", test_name));
    test_fn(eval_runner);
    Utils::test_print(fmt::format("DONE `{}`", test_name));
}

void
do_all_tests(void)
{
    auto runner = Runner { };
    runner.init_all();
    do_one_test(runner, "check_add", check_add);
    do_one_test(runner, "check_sitofp_f16", check_sitofp_f16);
}

};
