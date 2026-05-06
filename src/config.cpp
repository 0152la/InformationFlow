#include "config.hpp"

/*******************************************************************************
 * Utils
 ******************************************************************************/

namespace Utils
{

void
debug_print(std::string_view msg)
{
    fmt::print(fmt::fg(fmt::color::red) | fmt::emphasis::bold, "[DEBUG] ");
    std::cout << msg << std::endl;
}

}
