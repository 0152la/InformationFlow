#ifndef _IF_LLVM_GEN_HPP
#define _IF_LLVM_GEN_HPP

#include "config.hpp"

#include <cstring>
#include <filesystem>
#include <functional>
#include <memory>
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

struct llvm_pack
{
    llvm::LLVMContext& ctx;
    llvm::Module& mod;
    llvm::IRBuilder<>& ir_build;
};

void
emit_binop_fns(unsigned int, llvm_pack&);
void
emit_other_fns(llvm_pack&);

void
emit_cmp_fn(const std::string&, const llvm::CmpInst::Predicate&, llvm::Type*,
    llvm_pack&);
#endif // _IF_LLVM_GEN_HPP
