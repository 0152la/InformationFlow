#include <cstdint>
#include <stdexcept>
#include <string_view>

#include "fmt/format.h"

#include "runner.hpp"

namespace Testing
{
void
check_add(const Runner&);
void
do_all_tests(void);
};

#include "tests.tpp"
