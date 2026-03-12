#include "tests.hpp"

namespace Testing
{

void
check_add(const Runner& eval_runner)
{
    EntropyResult res = eval_runner.run_one("llvm_impl_add");
    for (const auto& r : res.get_data())
    {
        check_result<double>(r->uncertainty_coef, 0.5, "check_add");
    }
}

void
do_all_tests(void)
{
    auto runner = Runner {};
    runner.init_all();
    check_add(runner);
}

};
