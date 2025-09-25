#ifndef _IF_INSTR_EMULATOR_HPP
#define _IF_INSTR_EMULATOR_HPP

#include "config.hpp"
#include "llvm_snippets.hpp"

#include <climits>
#include <cmath>
#include <dlfcn.h>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <memory>
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

using unops_d_t = std::function<double(double)>;
using binops_i64_t = std::function<int64_t(int64_t, int64_t)>;
using binops_i64_ct = int64_t (*)(int64_t, int64_t);
using emulated_fns_t = std::unordered_map<std::string, binops_i64_t>; // TODO

using binops_dbl_t = std::function<double(double, double)>;
using binops_dbl_ct = double (*)(double, double);
using emulated_fns_dbl_t = std::unordered_map<std::string, binops_dbl_t>;

using entropy_map_key_t = std::string;
using set_entropy_t = std::unordered_map<entropy_map_key_t, double>;
using estimate_entropy_t = std::unordered_map<entropy_map_key_t,
    std::function<double(const llvm::Instruction&)>>;
// extern std::map<uint16_t, std::function<double(double)>>
// emulated_fns_unary;
extern emulated_fns_t emulated_fns;

#include "instr_emulator-typemap.hpp"

// constexpr unsigned short instr_ty_count = 5;
// const std::array<

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

class IF_Emulator

{
private:
    static constexpr std::string snippet_prefix = "llvm_impl_";
    emulated_fns2_t emulated_fnss;

    void* ll_snippet_handler;

    void dllink_snippet(const std::string&) const;
    void dllink_snippet_float(const std::string&) const;
    template <typename T, typename Tc> void populate_fn_def(const fn_def&);
    void populate_ops(void);
    void populate_all_bin_ops(void) const;
    void populate_all_other_ops(void) const;

public:
    IF_Emulator(const std::string&);
    ~IF_Emulator();

    binops_i64_t get_emulated_fn(const llvm::Instruction&) const;
    binops_dbl_t get_emulated_fn_float(const llvm::Instruction&) const;

    static std::string complete_fn_name(const std::string&);
    static std::string make_fn_name_from_opcode(unsigned int llvm_ir_opcode);
    static std::string make_emulated_fn_name(const llvm::Instruction&);

    /* Estimated operations
     * These operations do not need to be fuzzed to compute their entropy, but
     * certain dynamic elements need to be inspected in order to estimate it.
     * For example, a `trunt` instruction would retain entropy relative to the
     * number of bits removed from it.
     */

    /* Terminator Operations *************************************************/
    static double estimate_switch(const llvm::Instruction&);

    /* Vector Operations *****************************************************/
    static double estimate_extract_element(const llvm::Instruction&);

    /* Aggregate Operations **************************************************/
    static double estimate_extract_value(const llvm::Instruction&);

    /* Memory Operations *****************************************************/
    static double estimate_gep(const llvm::Instruction&);

    /* Conversion Operations *************************************************/
    static double estimate_trunc(const llvm::Instruction&);
    static double estimate_ptrtoint(const llvm::Instruction&);

    /* Other Operations ******************************************************/
    static double estimate_icmp_eq(const llvm::Instruction&);
    static double estimate_icmp_ne(const llvm::Instruction&);
    static double estimate_phi(const llvm::Instruction&);
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

#include "instr_emulator.tpp"

#endif // _IF_INSTR_EMULATOR_HPP
