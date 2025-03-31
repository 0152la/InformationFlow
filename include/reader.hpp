#ifndef _IF_READER_HPP
#define _IF_READER_HPP

#include <string>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wall"
#pragma clang diagnostic ignored "-Wunused-parameter"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"
#pragma clang diagnostic pop

// #include "reader-basicblock.hpp"
#include "entropy.hpp"
#include "randgen.hpp"

class IF_LLVM_Module
{
private:
    std::unique_ptr<llvm::LLVMContext> llvm_ctx;
    std::unique_ptr<llvm::SMDiagnostic> llvm_smd;
    std::unique_ptr<llvm::Module> llvm_module;

public:
    IF_LLVM_Module(llvm::LLVMContext* _ctx, llvm::SMDiagnostic* _smd,
        std::unique_ptr<llvm::Module> _mod) :
        llvm_ctx(std::unique_ptr<llvm::LLVMContext>(_ctx)),
        llvm_smd(std::unique_ptr<llvm::SMDiagnostic>(_smd)),
        llvm_module(std::move(_mod)) { };

    const llvm::Module* get_module(void) { return this->llvm_module.get(); };
};

class IF_Parser
{
private:
    IF_Histogram data;
    IF_Randgen in_gen;
    // IF_EntropyMap H_map; // H cause H = entropy :>

public:
    IF_Parser();
    IF_Parser(int);

    void make_entropy_map(const llvm::Module&);

    static std::unique_ptr<IF_LLVM_Module> parse_ll(const std::string&);
    static void print_instrs(const llvm::Module&);
};

#endif // _IF_READER_HPP
