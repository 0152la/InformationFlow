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
check_shl(const Runner& eval_runner)
{
    EntropyResult res = eval_runner.run_one("llvm_impl_shl");
    const std::unordered_map<EvalData::bit_sz_t, double> shl_expected_uc {
        { 2, 0.603759374819711 }, { 3, 0.556405460373124 },
        { 4, 0.5428926125371982 }
    };
    for (const auto& r : res.get_data())
    {
        if (const auto& expected = shl_expected_uc.find(r.bit_sz);
            expected != shl_expected_uc.end())
        {
            check_result<double>(r.uncertainty_coef, expected->second,
                fmt::format("check_shl_bs{}", expected->first));
        }
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
    try
    {
        test_fn(eval_runner);
    }
    catch (std::runtime_error e)
    {
        fmt::println("Couldn't run test `{}` -- {}", test_name, e.what());
    }
    Utils::test_print(fmt::format("DONE `{}`", test_name));
}

void
do_all_tests(const Runner& runner)
{
    do_one_test(runner, "check_add", check_add);
    do_one_test(runner, "check_sitofp_f16", check_sitofp_f16);
    do_one_test(runner, "check_shl", check_shl);
}

};
