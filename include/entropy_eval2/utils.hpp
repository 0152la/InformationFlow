#include <stdexcept>
#include <string_view>
#include <iostream>

#include "fmt/base.h"
#include "fmt/printf.h"
#include "fmt/color.h"

namespace Utils
{
void debug_print(std::string_view);
void test_print(std::string_view);

void
do_check(bool, std::string_view);
void
do_debug_check(bool, std::string_view);
};
