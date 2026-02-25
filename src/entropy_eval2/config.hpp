#ifndef _EEVAL_CONFIG_HPP
#define _EEVAL_CONFIG_HPP

#include <chrono>
#include <cstdint>
#include <string_view>

#define DEBUG_PRINT fmt::print

namespace Config
{
using clock_ty = std::chrono::steady_clock;

constexpr uint8_t int_min_bit_sz = 2;
constexpr uint8_t int_max_bit_sz = 19;

// index for divisor operand, to prevent division by 0
constexpr uint8_t divisor_idx = 1;

// minimum number of bits to move to parallel computation
constexpr uint8_t min_par_bit_sz = 13;
// minimum threads expected to run parallel execution
constexpr uint8_t min_thread_count = 2;
// allowed free threads for other computation
constexpr uint8_t other_free_threads = 0; // 2;

// we set one specific value for `nan`s for floating point types of a given
// type, which is offset from the maximum value of an unsigned integer type of
// the same size of that type by this amount
constexpr uint64_t nan_default_offset = -2;

constexpr auto def_path
    = std::string_view { "/home/andreilascu/Documents/Repos/InformationFlow/"
                         "build/src/llvm_snippets.def" };

constexpr auto def_header_start = std::string_view { "opcode" };

constexpr auto lib_path
    = std::string_view { "/home/andreilascu/Documents/Repos/InformationFlow/"
                         "build/src/libllvm_snippets.so" };

constexpr auto out_log_path
    = std::string_view { "entropy_out-{:%Y%m%d-%H%M%S}.log" };

constexpr auto out_csv_path
    = std::string_view { "entropy_out-{:%Y%m%d-%H%M%S}.csv" };

}

#endif // _EEVAL_CONFIG_HPP
