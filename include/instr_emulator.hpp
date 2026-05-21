#ifndef _IF_INSTR_EMULATOR_HPP
#define _IF_INSTR_EMULATOR_HPP

#include "config.hpp"

#include <climits>
#include <cmath>
#include <dlfcn.h>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <memory>
#include <numeric>
#include <ranges>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <variant>
#include <vector>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wall"
#pragma clang diagnostic ignored "-Wunused-parameter"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/raw_ostream.h"
#pragma clang diagnostic pop

#include "fmt/base.h"
#include "fmt/format.h"

#include "toml++/toml.hpp"

using entropy_map_key_t = unsigned int;
using set_entropy_t = std::unordered_map<entropy_map_key_t, double>;
using estimate_entropy_t = std::unordered_map<entropy_map_key_t,
    std::function<double(const llvm::Instruction&)>>;

using flow_set_t = set_entropy_t;
using flow_estimate_t = estimate_entropy_t;

struct fn_def
{
    unsigned int op_code;
    std::string extra_info;
    std::string fn_name;

    std::string ret_sig;
    std::string ret_ty;
    std::vector<std::string> params;

    fn_def(const std::string);
    const std::string to_str(void);
};

namespace IF_Entropy_Vals
{
using set_map_t = std::unordered_map<std::string, double>;
using emu_map_t
    = std::unordered_map<std::string, std::unordered_map<uint16_t, double>>;

namespace Parser
{
    set_map_t parse_set_entropy(std::string_view);
    emu_map_t parse_emulated_entropy(std::string_view);

    void print_set_entropy(const set_map_t&);
    void print_emu_entropy(const emu_map_t&);
};

namespace Estimator
{
    template <typename T> double estimate_control_flow(const T&);
    template <typename T> double estimate(const T&);

    // double cast_inst(const llvm::CastInst&);
    // double trunc(const llvm::Instruction&);
    // double fptrunc(const llvm::Instruction&);

    // double ptrtoint(const llvm::Instruction&);
};

struct Getter
{
    set_map_t set_entropy;
    emu_map_t emulated_entropy;

    Getter(void);

    double get_entropy_for_inst(const llvm::Instruction&);
};
};

class IF_Emulator
{
private:
    void populate_ops(void);

public:
    IF_Emulator();
    ~IF_Emulator();

    /* Estimated operations
     * These operations do not need to be fuzzed to compute their entropy, but
     * certain dynamic elements need to be inspected in order to estimate it.
     * For example, a `trunt` instruction would retain entropy relative to the
     * number of bits removed from it.
     */

    /* Vector Operations *****************************************************/
    static double estimate_extract_element(const llvm::Instruction&);

    /* Aggregate Operations **************************************************/
    static double estimate_extract_value(const llvm::Instruction&);

    /* Memory Operations *****************************************************/
    static double estimate_gep(const llvm::Instruction&);

    /* Conversion Operations *************************************************/
    static double estimate_ptrtoint(const llvm::Instruction&);

    /* Other Operations ******************************************************/
    static double estimate_icmp_eq(const llvm::Instruction&);
    static double estimate_icmp_ne(const llvm::Instruction&);
    static double estimate_phi(const llvm::Instruction&);

    /* Flow estimate functions
     */

    /* Terminator Operations *************************************************/
    static double estimate_flow_br(const llvm::Instruction&);
    static double estimate_flow_switch(const llvm::Instruction&);
    static double estimate_flow_indirectbr(const llvm::Instruction&);
};

struct struct_sz_s;
using struct_sz_width_t = uint32_t;
using struct_sz_t = std::vector<std::variant<struct_sz_width_t, struct_sz_s>>;

struct struct_sz_s
{
    std::unique_ptr<struct_sz_t> t;
};

uint32_t
get_aggregate_type_by_idx(const struct_sz_t*, const llvm::ExtractValueInst*);
uint32_t
compute_total_struct_sz(const struct_sz_t*);
std::unique_ptr<struct_sz_t>
get_llvm_struct_bitsize(const llvm::StructType*, const llvm::Module*);
uint8_t
get_operand_bit_width(const llvm::Type*, const llvm::Module*);

#endif // _IF_INSTR_EMULATOR_HPP
