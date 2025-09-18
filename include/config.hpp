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
make_snippets_def_path(void)
{
    std::filesystem::path def_path(snippets_lib_path);
    def_path.replace_extension("def");
    def_path.replace_filename(
        def_path.filename().string().substr(strlen("lib")));
    return def_path.string();
}

const std::string
make_snippets_ll_path(void)
{
    std::filesystem::path ll_path(snippets_lib_path);
    ll_path.replace_extension("ll");
    ll_path.replace_filename(ll_path.filename().string().substr(strlen("lib")));
    return ll_path.string();
}

#endif // _IF_CONFIG_HPP
