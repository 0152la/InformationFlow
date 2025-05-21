#ifndef _IF_INSTR_EMULATOR_HPP
#define _IF_INSTR_EMULATOR_HPP

#include <cmath>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <ranges>
#include <stdexcept>
#include <type_traits>
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
#pragma clang diagnostic pop

/* A base class for an argument to be used in emulation. This allows us to pass
 * a list of `IF_Arg`s without caring about explicit types, as they will be set
 * in derived classes.
 */
class IF_Arg
{
protected:
    uint8_t sz;

    IF_Arg(uint8_t _sz) :
        sz(_sz) { };

public:
    virtual bool is_int() const { return false; };

    virtual bool is_float() const { return false; };

    virtual bool is_vec() const { return false; };

    virtual bool is_bool() const { return false; };

    auto get_sz(void) const -> decltype(this->sz) { return this->sz; };
};

/* An argument of integer type, with variable size.
 */
class IF_Arg_Int final : public IF_Arg
{
public:
    using val_t = int64_t;

private:
    val_t val;
    bool sgn;

public:
    IF_Arg_Int(val_t, uint8_t, bool);
    IF_Arg_Int(val_t, uint8_t);

    val_t get_val(void) const { return this->val; };

    bool is_int(void) const { return true; };

    bool is_sgn(void) const { return this->sgn; };
};

class IF_Arg_Float : public IF_Arg
{
private:
    double val;

    static uint8_t get_float_sz(const llvm::Type*);

public:
    IF_Arg_Float(double _val, uint8_t _sz) :
        IF_Arg(_sz),
        val(_val) { };
    IF_Arg_Float(double, const llvm::Type*);

    auto get_val(void) const -> decltype(val) { return this->val; };

    bool is_float() const { return true; };
};

/* An argument of `boolean` type. Essentially a logical wrapper around an `i1`
 * type variable.
 */
class IF_Arg_Bool final : public IF_Arg
{
private:
    bool val;

public:
    IF_Arg_Bool(bool _val) :
        IF_Arg(1),
        val(_val) { };

    bool get_val(void) const { return this->val; };

    bool is_bool(void) const { return true; };
};

/* A special class wrapping around the predicate for a comparison instruction
 * (such as `ICmpInst`)
 */
class IF_Cmp_Pred final : public IF_Arg
{
private:
    const llvm::CmpInst::Predicate llvm_pred;

public:
    IF_Cmp_Pred(const llvm::CmpInst::Predicate _llvm_pred) :
        IF_Arg(4),
        llvm_pred(_llvm_pred) { };

    const llvm::CmpInst::Predicate& get_pred(void) const
    {
        return this->llvm_pred;
    };
};

/* A collection of `IF_Arg`, to be used when passing to an emulated operation.
 * An emulated invocation should be passed an `IF_ArgList` containing all
 * required input arguments for that operation, as described in the LLVM
 * Language Reference [1]. The types of the arguments will be determined by
 * derived types inheriting from `IF_Arg`, which can then be cast back for
 * parsing as required during emulation.
 *
 * [1] https://llvm.org/docs/LangRef.html
 */
class IF_ArgList
{
private:
    std::vector<std::unique_ptr<IF_Arg>> args;

public:
    IF_ArgList(uint8_t _count) :
        args(std::vector<std::unique_ptr<IF_Arg>>(_count)) { };
    IF_ArgList(std::vector<std::unique_ptr<IF_Arg>> _args) :
        args(std::move(_args)) { };

    /* Getters ***************************************************************/

    const IF_Arg& get_arg(size_t) const;
    const IF_Arg* get_arg_ptr(size_t) const;

    auto get_args(void) const -> const decltype(this->args)&
    {
        return this->args;
    };

    size_t get_args_count(void) const { return this->args.size(); };

    std::vector<const IF_Arg_Int*> extract_arg_int(void) const;

    /* Checkers **************************************************************/

    static void check_args(bool);
    bool check_all_ints(void) const;
    bool check_all_floats(void) const;
    bool check_all_same_sz(void) const;
    bool check_count(uint8_t) const;
};

class IF_Emulator
{
private:
    static std::vector<IF_Arg_Int::val_t> extract_binop_ops(const IF_ArgList&);

public:
    /* Emulated operations
     * The entropy of these operations are computed by fuzzing their
     * non-constant inputs
     */

    /* Binary Operations ******************************************************
     * If these operations over-/under-flow, we clamp them to the
     * respective max/min value */
    static IF_Arg emulate_add(const IF_ArgList&);
    static IF_Arg emulate_sub(const IF_ArgList&);
    static IF_Arg emulate_mul(const IF_ArgList&);
    static IF_Arg emulate_udiv(const IF_ArgList&);
    static IF_Arg emulate_sdiv(const IF_ArgList&);
    static IF_Arg emulate_urem(const IF_ArgList&);
    static IF_Arg emulate_srem(const IF_ArgList&);

    /* Bitwise Binary Operations *********************************************/
    static IF_Arg emulate_shl(const IF_ArgList&);
    static IF_Arg emulate_lshr(const IF_ArgList&);
    static IF_Arg emulate_ashr(const IF_ArgList&);
    static IF_Arg emulate_and(const IF_ArgList&);
    static IF_Arg emulate_or(const IF_ArgList&);
    static IF_Arg emulate_xor(const IF_ArgList&);

    /* Memory Operations *****************************************************/
    static IF_Arg emulate_atomic_rmw(const IF_ArgList&);

    /* Conversion Operations *************************************************/
    static IF_Arg emulate_fptosi(const IF_ArgList&);

    /* Other Operations ******************************************************/
    static IF_Arg emulate_icmp(const IF_ArgList&);

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
    static double estimate_trunc(const llvm::Instruction&);
    static double estimate_ptrtoint(const llvm::Instruction&);

    /* Other Operations ******************************************************/
    static double estimate_phi(const llvm::Instruction&);
};

struct struct_sz_s;
using struct_sz_t = std::vector<std::variant<uint32_t, struct_sz_s>>;

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

#endif // _IF_INSTR_EMULATOR_HPP
