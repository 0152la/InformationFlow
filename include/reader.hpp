#ifndef _IF_READER_HPP
#define _IF_READER_HPP

#include <memory>
#include <set>
#include <sstream>
#include <string>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wall"
#pragma clang diagnostic ignored "-Wunused-parameter"
#include "llvm/IR/Constants.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"
#pragma clang diagnostic pop

// #include "reader-basicblock.hpp"
#include "entropy.hpp"
#include "fuzz_engine.hpp"
#include "randgen.hpp"

class IF_LLVM_Module
{
private:
    const std::unique_ptr<llvm::LLVMContext> llvm_ctx;
    const std::unique_ptr<llvm::SMDiagnostic> llvm_smd;
    const std::unique_ptr<llvm::Module> llvm_module;

public:
    IF_LLVM_Module(llvm::LLVMContext* _ctx, llvm::SMDiagnostic* _smd,
        std::unique_ptr<llvm::Module> _mod) :
        llvm_ctx(std::unique_ptr<llvm::LLVMContext>(_ctx)),
        llvm_smd(std::unique_ptr<llvm::SMDiagnostic>(_smd)),
        llvm_module(std::move(_mod)) { };

    const llvm::Module* get_module(void) { return this->llvm_module.get(); };
};

class IF_EntropyMap_Instr
{
private:
    unsigned int opcode;
    double entropy_loss;

public:
    IF_EntropyMap_Instr(const llvm::Instruction& _instr) :
        opcode(_instr.getOpcode()) { };

    unsigned int get_opcode(void) const { return this->opcode; };

    double get_lost_entropy() const { return this->entropy_loss; };

    // TODO better name? This is retained entropy I believe
    void set_lost_entropy(double _entropy) { this->entropy_loss = _entropy; };

    const std::string to_str(void) const;
};

class IF_EntropyMap_Func
{
private:
    const std::string name;
    std::vector<std::unique_ptr<IF_EntropyMap_Instr>> instrs;
    std::vector<const IF_EntropyMap_Func*> callees;

public:
    IF_EntropyMap_Func() = delete;

    IF_EntropyMap_Func(const llvm::Function& _fn) :
        name(_fn.getName().str())
    {
        this->instrs.reserve(_fn.getInstructionCount());
    };

    const std::string get_name(void) const { return this->name; };

    auto get_instrs(void) const -> const decltype(this->instrs)&
    {
        return this->instrs;
    };

    void insert(std::unique_ptr<IF_EntropyMap_Instr> _instr)
    {
        this->instrs.push_back(std::move(_instr));
    };

    void insert_call(const IF_EntropyMap_Func*);

    const std::string to_str(void) const;
};

class IF_EntropyMap
{
private:
    std::vector<std::unique_ptr<IF_EntropyMap_Func>> funcs;

public:
    IF_EntropyMap(const llvm::Module& _module)
    {
        this->funcs.reserve(_module.size());
    };

    auto get_funcs(void) const -> const decltype(funcs)&
    {
        return this->funcs;
    };

    void insert(std::unique_ptr<IF_EntropyMap_Func> em_fn)
    {
        this->funcs.push_back(std::move(em_fn));
    };

    const std::string to_str(void) const;
};

class IF_Parser
{
private:
    IF_Histogram<if_in_t, if_out_t> data;
    IF_Randgen in_gen;
    // IF_EntropyMap H_map; // H cause H = entropy :>

public:
    IF_Parser();
    IF_Parser(int);

    std::unique_ptr<IF_EntropyMap> make_entropy_map(const llvm::Module&);

    static std::unique_ptr<IF_LLVM_Module> parse_ll(const std::string&);
    static void print_instrs(const llvm::Module&);
};

#endif // _IF_READER_HPP
