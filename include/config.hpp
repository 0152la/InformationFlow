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

namespace config
{
const std::string llvm_snippet_prefix = "llvm_impl";
constexpr char llvm_snippet_delim = '_';

const std::string make_path(const std::string&);

const std::string
make_snippets_def_path(void);
const std::string
make_snippets_header_path(void);
const std::string
make_snippets_header_path_cpp(void);
const std::string
make_snippets_ll_path(void);

};

namespace Utils
{
    void debug_print(std::string_view);
}

constexpr unsigned long initial_tests = 10000;
constexpr unsigned long extra_step = 1000;
constexpr unsigned long max_extra_steps = 100;
constexpr float change_error = 0.0005;
constexpr unsigned short in_scale = 32;
constexpr unsigned short out_scale = 16;

const std::string snippets_lib_path
    = "/home/andreilascu/Documents/Repos/InformationFlow/build/src/"
      "libllvm_snippets.so";

#endif // _IF_CONFIG_HPP
