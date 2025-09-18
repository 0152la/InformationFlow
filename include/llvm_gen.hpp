#ifndef _IF_LLVM_GEN_HPP
#define _IF_LLVM_GEN_HPP

#include "config.hpp"

#include <filesystem>
#include <fstream>
#include <functional>
#include <memory>
#include <numeric>
#include <system_error>
#include <unordered_map>
#include <utility>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wall"
#pragma clang diagnostic ignored "-Wunused-parameter"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_ostream.h"
#pragma clang diagnostic pop

/* Helper functions ***********************************************************/

llvm::Function*
make_llvm_fn(const std::string&, llvm::FunctionType*, unsigned int,
    const std::string&, llvm::Module&);

/* Emitting instruction snippets **********************************************/
struct llvm_pack
{
    llvm::LLVMContext& ctx;
    llvm::Module& mod;
    llvm::IRBuilder<>& ir_build;
};

void
emit_binop_fns(unsigned int, llvm_pack&);

void
emit_cmp_fn(const std::string&, const llvm::CmpInst::Predicate&, llvm::Type*,
    llvm_pack&);
void
emit_conversion_fns(llvm_pack&);
void
emit_other_fns(llvm_pack&);

/* Definition logging *********************************************************/

struct llvm_impl_def
{
    const std::string name;
    const std::string ret_ty;
    const std::vector<std::string> params_ty;

    unsigned int opcode;
    const std::string helper;

    llvm_impl_def(const std::string& _name, const std::string& _ret,
        std::vector<std::string>& _params, unsigned int _opcode,
        const std::string& _helper) :
        name(_name),
        ret_ty(_ret),
        params_ty(_params),
        opcode(_opcode),
        helper(_helper) { };

    llvm_impl_def(const std::string&& _name, const std::string& _ret,
        std::vector<std::string>& _params, unsigned int _opcode) :
        llvm_impl_def(_name, _ret, _params, _opcode, "") {};
};

void
emit_impl_def(const std::string&);
void
record_impl_def(const llvm::Function*, unsigned int, const std::string&);
void
emit_impl_header(const std::string&);

#endif // _IF_LLVM_GEN_HPP
