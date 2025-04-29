#ifndef _IF_INSTR_EMULATOR_HPP
#define _IF_INSTR_EMULATOR_HPP

#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <ranges>
#include <stdexcept>
#include <type_traits>
#include <vector>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wall"
#pragma clang diagnostic ignored "-Wunused-parameter"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#pragma clang diagnostic pop

class IF_Arg
{
protected:
    uint8_t sz;

public:
    IF_Arg(uint8_t _sz) :
        sz(_sz) { };

    virtual bool is_int() const { return false; };

    virtual bool is_float() const { return false; };

    virtual bool is_vec() const { return false; };

    auto get_sz(void) const -> decltype(this->sz) { return this->sz; };
};

class IF_Arg_Int final : public IF_Arg
{
public:
    using val_t = int64_t;

private:
    val_t val;
    bool sgn;

public:
    IF_Arg_Int(val_t _val, uint8_t _sz, bool _sgn);
    IF_Arg_Int(val_t _val, uint8_t _sz);

    val_t get_val(void) const { return this->val; };

    bool is_int(void) const { return true; };

    bool is_sgn(void) const { return this->sgn; };
};

class IF_ArgList
{
private:
    std::vector<std::unique_ptr<IF_Arg>> args;

public:
    IF_ArgList(uint8_t _count) :
        args(std::vector<std::unique_ptr<IF_Arg>>(_count)) { };
    IF_ArgList(std::vector<std::unique_ptr<IF_Arg>> _args) :
        args(std::move(_args)) { };

    const IF_Arg& get_arg(size_t) const;

    auto get_args(void) const -> const decltype(this->args)&
    {
        return this->args;
    };

    size_t get_args_count(void) const { return this->args.size(); };

    std::vector<const IF_Arg_Int*> extract_arg_int(void) const;

    static void check_args(bool);
    bool check_all_ints(void) const;
    bool check_all_same_sz(void) const;
    bool check_count(uint8_t) const;
};

class IF_Emulator
{
private:
    static std::vector<IF_Arg_Int::val_t> extract_binop_ops(const IF_ArgList&);

public:
    // Binary Operations
    // If these operations over-/under-flow, we clamp them to the
    // respective max/min value
    static IF_Arg emulate_add(const IF_ArgList&);
    static IF_Arg emulate_sub(const IF_ArgList&);
    static IF_Arg emulate_mul(const IF_ArgList&);
    static IF_Arg emulate_udiv(const IF_ArgList&);
    static IF_Arg emulate_sdiv(const IF_ArgList&);
    static IF_Arg emulate_urem(const IF_ArgList&);
    static IF_Arg emulate_srem(const IF_ArgList&);

    // Memory Operations
    static IF_Arg emulate_atomic_rmw(const IF_ArgList&);

    // TODO
    static IF_Arg emulate_icmp(const IF_ArgList&);

    static double estimate_extract_element(const llvm::Instruction&);
    static double estimate_extract_value(const llvm::Instruction&);
    static double estimate_trunc(const llvm::Instruction&);
};

#endif // _IF_INSTR_EMULATOR_HPP
