#ifndef _IF_ENTROPYMAP_HPP
#define _IF_ENTROPYMAP_HPP

// #include <format>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <set>
#include <vector>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wall"
#pragma clang diagnostic ignored "-Wunused-parameter"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Module.h"
#pragma clang diagnostic pop

class IF_EntropyMap_Instr
{
private:
    uint32_t idx;
    unsigned int opcode;
    double retained_entropy;
    bool trivial = false;
    std::set<decltype(idx)> succs;
    std::set<std::string> succs_extern;

public:
    IF_EntropyMap_Instr(uint32_t _idx, const llvm::Instruction& _instr) :
        idx(_idx),
        opcode(_instr.getOpcode()) { };

    uint32_t get_idx(void) const { return this->idx; };

    unsigned int get_opcode(void) const { return this->opcode; };

    double get_retained_entropy(void) const { return this->retained_entropy; };

    auto get_succs(void) const -> const decltype(this->succs)&
    {
        return this->succs;
    };

    auto get_external_succs(void) const -> const decltype(this->succs_extern)&
    {
        return this->succs_extern;
    };

    bool is_trivial(void) const { return this->trivial; };

    void set_retained_entropy(double _entropy)
    {
        this->retained_entropy = _entropy;
        if (_entropy == 1.0)
        {
            this->trivial = true;
        }
    };

    void add_successor(const IF_EntropyMap_Instr*);

    void add_external_succ(std::string);

    const std::string to_str(void) const;
};

class IF_EntropyMap_Func
{
private:
    const std::string name;
    const std::string demangled_name;
    std::vector<std::unique_ptr<IF_EntropyMap_Instr>> instrs;

    const std::string set_demangled_name(const llvm::Function&);

public:
    IF_EntropyMap_Func() = delete;

    IF_EntropyMap_Func(const llvm::Function& _fn) :
        name(_fn.getName().str()),
        demangled_name(IF_EntropyMap_Func::set_demangled_name(_fn))
    {
        this->instrs.reserve(_fn.getInstructionCount());
    };

    const std::string get_name(void) const { return this->name; };

    const std::string get_demangled_name(void) const
    {
        return this->demangled_name;
    };

    const std::string get_representing_name(void) const;

    auto get_instrs(void) const -> const decltype(this->instrs)&
    {
        return this->instrs;
    };

    void insert(std::unique_ptr<IF_EntropyMap_Instr> _instr)
    {
        this->instrs.push_back(std::move(_instr));
    };

    const std::string to_str(void) const;
};

class IF_EntropyMap
{
private:
    std::vector<std::unique_ptr<IF_EntropyMap_Func>> funcs;
    std::set<std::string> external_funcs;
    bool verbose = false;

public:
    IF_EntropyMap(const llvm::Module& _module)
    {
        this->funcs.reserve(_module.size());
    };

    auto get_funcs(void) const -> const decltype(funcs)&
    {
        return this->funcs;
    };

    auto get_external_funcs(void) const -> const decltype(external_funcs)&
    {
        return this->external_funcs;
    };

    void insert(std::unique_ptr<IF_EntropyMap_Func> em_fn)
    {
        this->funcs.push_back(std::move(em_fn));
    };

    void insert_external_func(std::string);

    void set_verbose(bool _verbose) { this->verbose = _verbose; };

    const std::string to_str(void) const;
    void print(void) const;
};

#endif // _IF_ENTROPYMAP_HPP
