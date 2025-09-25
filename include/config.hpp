#ifndef _IF_CONFIG_HPP
#define _IF_CONFIG_HPP

#include <cstring>
#include <filesystem>
#include <string>

constexpr unsigned long initial_tests = 10000;
constexpr unsigned long extra_step = 1000;
constexpr unsigned long max_extra_steps = 100;
constexpr float change_error = 0.0005;
constexpr unsigned short in_scale = 32;
constexpr unsigned short out_scale = 16;
const std::string snippets_lib_path
    = "/home/andreilascu/Documents/Repos/InformationFlow/build/src/"
      "libllvm_snippets.so";

const std::string
make_snippets_def_path(void);
const std::string
make_snippets_header_path(void);
const std::string
make_snippets_ll_path(void);

#endif // _IF_CONFIG_HPP
