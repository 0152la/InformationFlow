#ifndef _IF_CONFIG_HPP
#define _IF_CONFIG_HPP

#include <cstring>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

#include "fmt/base.h"
#include "fmt/color.h"

namespace Utils
{
void debug_print(std::string_view);

template <typename T>
void
print_type()
{
    std::puts(__PRETTY_FUNCTION__);
};

template <typename T> struct CompileTimeType;
}

constexpr unsigned long initial_tests = 10000;
constexpr unsigned long extra_step = 1000;
constexpr unsigned long max_extra_steps = 100;
constexpr float change_error = 0.0005;
constexpr unsigned short in_scale = 32;
constexpr unsigned short out_scale = 16;

#endif // _IF_CONFIG_HPP
