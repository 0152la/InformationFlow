#ifndef _EEVAL_CONFIG_HPP
#define _EEVAL_CONFIG_HPP

#include <chrono>
#include <cstdint>
#include <string_view>

#define DEBUG_PRINT fmt::print

namespace Config
{
using clock_ty = std::chrono::steady_clock;

inline constexpr uint8_t int_min_bit_sz = 2;
inline constexpr uint8_t int_max_bit_sz = 18;

// index for divisor operand, to prevent division by 0
inline constexpr uint8_t divisor_idx = 1;

// minimum number of bits to move to parallel computation
inline constexpr uint8_t min_par_bit_sz = 13;
// minimum threads expected to run parallel execution
inline constexpr uint8_t min_thread_count = 2;
// allowed free threads for other computation
inline constexpr uint8_t other_free_threads = 0;

// we set one specific value for `nan`s for floating point types of a given
// type, which is offset from the maximum value of an unsigned integer type of
// the same size of that type by this amount
inline constexpr uint64_t nan_default_offset = -2;

inline constexpr auto def_header_start = std::string_view { "opcode" };

inline constexpr auto out_log_path
    = std::string_view { "entropy_out-{:%Y%m%d-%H%M%S}.log" };
inline constexpr auto out_csv_path
    = std::string_view { "entropy_out-{:%Y%m%d-%H%M%S}.csv" };
}

#endif // _EEVAL_CONFIG_HPP
