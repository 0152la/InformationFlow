#ifndef _IF_LLVM_GEN_HPP
#define _IF_LLVM_GEN_HPP

#include "llvm_gen-names.hpp"

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

const std::string make_path(const std::string&);

llvm::Function*
make_llvm_fn(const std::string&, llvm::FunctionType*, unsigned int,
    const std::string&, llvm::Module&);
llvm::Function*
make_llvm_fn_cmp(const std::string&, llvm::FunctionType*, unsigned int,
    unsigned int, const std::string&, llvm::Module&);

using cast_op_create_fn_ty = std::function<llvm::Value*(
    llvm::IRBuilderBase&, llvm::Value*, llvm::Type*, const llvm::Twine&)>;
using cast_op_get_ty_ty = std::function<llvm::Type*(llvm::LLVMContext&)>;

struct cast_op_data
{
    unsigned int opcode;
    const cast_op_get_ty_ty ret_ty_fn;
    const cast_op_get_ty_ty arg_ty_fn;
    const std::string name_extra;

    cast_op_data(unsigned int _oc, const cast_op_get_ty_ty _r_ty,
        const cast_op_get_ty_ty _a_ty, const std::string _extra) :
        opcode(_oc),
        ret_ty_fn(_r_ty),
        arg_ty_fn(_a_ty),
        name_extra(_extra) { };
};

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
emit_cmp_fn_flt(unsigned int, llvm_pack&);
void
emit_cmp_fn_int(unsigned int, llvm_pack&);

void
emit_conversion_fns(llvm_pack&);

/* Definition logging *********************************************************/

struct llvm_impl_def
{
    const std::string name;
    const std::string ret_ty;
    const std::vector<std::string> params_ty;

    unsigned int opcode;
    unsigned int cmp_pred = -1;
    const std::string extra;

    llvm_impl_def(const std::string& _name, const std::string& _ret,
        std::vector<std::string>& _params, unsigned int _opcode,
        const std::string& _extra) :
        name(_name),
        ret_ty(_ret),
        params_ty(_params),
        opcode(_opcode),
        extra(_extra) { };

    llvm_impl_def(const std::string& _name, const std::string& _ret,
        std::vector<std::string>& _params, unsigned int _opcode) :
        llvm_impl_def(_name, _ret, _params, _opcode, "") { };

    void set_cmp_pred(unsigned int pred) { this->cmp_pred = pred; };
};

void
emit_impl_def(const std::string&);
void
record_impl_def(const llvm::Function*, unsigned int, const std::string&);
void
emit_impl_header(const std::string&);

#endif // _IF_LLVM_GEN_HPP
