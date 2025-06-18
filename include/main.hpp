#ifndef _IF_MAIN_HPP
#define _IF_MAIN_HPP

#include <cmath>
#include <dlfcn.h>
#include <functional>
#include <iostream>
#include <limits>
#include <random>
#include <stdexcept>

#include "entropy.hpp"
#include "entropy_map_graph.hpp"
#include "entropy_map_path_printer.hpp"
#include "instr_emulator.hpp"
#include "randgen.hpp"
#include "reader.hpp"

constexpr unsigned long initial_tests = 10000;
constexpr unsigned long extra_step = 1000;
constexpr unsigned long max_extra_steps = 100;
constexpr float change_error = 0.0005;
constexpr unsigned short in_scale = 32;
constexpr unsigned short out_scale = 16;
const std::string snippets_lib_path
    = "/home/andreilascu/Documents/Repos/InformationFlow/build/src/"
      "libllvm_snippets.so";

#endif // _IF_MAIN_HPP
