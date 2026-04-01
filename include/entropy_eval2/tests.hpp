#include <cstdint>
#include <functional>
#include <sstream>
#include <stdexcept>
#include <string_view>

#include "fmt/format.h"

#include "runner.hpp"

namespace Testing
{
using test_fn_ty = std::function<void(const Runner&)>;

void
check_add(const Runner&);
void
check_sitofp_f16(const Runner&);

void
do_one_test(const Runner&, std::string_view, Testing::test_fn_ty);
void
do_all_tests(void);
};

#include "tests.tpp"
