#include "utils.hpp"

namespace Utils
{

void
debug_print(std::string_view msg)
{
    fmt::print(fmt::fg(fmt::color::red) | fmt::emphasis::bold, "[DEBUG] ");
    std::cout << msg << std::endl;
}

void
test_print(std::string_view msg)
{
    fmt::print(fmt::fg(fmt::color::light_sky_blue) | fmt::emphasis::bold, "[TEST] ");
    std::cout << msg << std::endl;
}

void
do_check(bool cond, std::string_view msg)
{
    if (cond)
    {
        throw std::runtime_error(msg.data());
    }
}

void
do_debug_check(
    [[maybe_unused]] bool cond, [[maybe_unused]] std::string_view msg)
{
#ifndef NDEBUG
    if (cond)
    {
        throw std::runtime_error(msg.data());
    }
#endif
}
};
