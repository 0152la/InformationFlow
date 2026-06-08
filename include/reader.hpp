#ifndef _IF_READER_HPP
#define _IF_READER_HPP

#include "entropy.hpp"
#include "entropy_map.hpp"
#include "instr_emulator.hpp"

#include <algorithm>
#include <limits>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wall"
#pragma clang diagnostic ignored "-Wunused-parameter"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"
#pragma clang diagnostic pop

class IF_LLVM_Module
{
private:
    const std::unique_ptr<llvm::LLVMContext> llvm_ctx;
    const std::unique_ptr<llvm::SMDiagnostic> llvm_smd;
    std::unique_ptr<llvm::Module> llvm_module;

public:
    IF_LLVM_Module(llvm::LLVMContext* _ctx, llvm::SMDiagnostic* _smd,
        std::unique_ptr<llvm::Module> _mod) :
        llvm_ctx(std::unique_ptr<llvm::LLVMContext>(_ctx)),
        llvm_smd(std::unique_ptr<llvm::SMDiagnostic>(_smd)),
        llvm_module(std::move(_mod)) { };

    llvm::Module* get_module(void) { return this->llvm_module.get(); };
};

class IF_Parser
{

public:
    IF_Parser() = default;

    std::unique_ptr<IF_EntropyMap::Map> make_entropy_map(llvm::Module&);

    static std::unique_ptr<IF_LLVM_Module> parse_ll(const std::string&);
    static void print_instrs(const llvm::Module&);
};

#endif // _IF_READER_HPP
