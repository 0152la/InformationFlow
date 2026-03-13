#include "utils.hpp"

namespace Utils
{
void
do_check(bool cond, std::string_view msg)
{
    if (cond)
    {
        throw std::runtime_error(msg.data());
    }
}

void
do_debug_check([[maybe_unused]] bool cond, [[maybe_unused]] std::string_view msg)
{
#ifndef NDEBUG
    if (cond)
    {
        throw std::runtime_error(msg.data());
    }
#endif
}
};
