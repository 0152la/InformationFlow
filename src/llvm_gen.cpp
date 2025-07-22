#include "llvm_gen.hpp"

const std::string snippet_prefix = "llvm_impl_"; // TODO

// clang-format off
const std::vector<std::pair<std::pair<unsigned int, unsigned int>,
    std::function<void(unsigned int, llvm_pack&)>>>
    emit_fn_map {
        { { llvm::Instruction::BinaryOpsBegin,
                        llvm::Instruction::BinaryOpsEnd },
                      emit_binop_fns },
    };
// clang-format on

const std::array binops_float { llvm::Instruction::FAdd,
    llvm::Instruction::FSub, llvm::Instruction::FMul, llvm::Instruction::FDiv,
    llvm::Instruction::FRem };
const std::array ignored_other_ops { llvm::Instruction::PHI };

/* We use this for both `binary` and `bitwise_binary` instructions, as their
 * signatures are identical
 */
void
emit_binop_fns(unsigned int op, llvm_pack& lp)
{
    llvm::Type* binop_ty;
    if (std::find(binops_float.begin(), binops_float.end(), op)
        != binops_float.end())
    {
        binop_ty = llvm::Type::getDoubleTy(lp.ctx);
    }
    else
    {
        binop_ty = llvm::Type::getInt64Ty(lp.ctx);
    }

    // TODO add noundef
    std::vector<llvm::Type*> params { binop_ty, binop_ty };
    llvm::FunctionType* binop_fn_ty(
        llvm::FunctionType::get(binop_ty, params, false));
    llvm::Function* fn(
        llvm::Function::Create(binop_fn_ty, llvm::Function::ExternalLinkage,
            snippet_prefix + llvm::Instruction::getOpcodeName(op), lp.mod));

    llvm::BasicBlock* bb(llvm::BasicBlock::Create(lp.ctx, "", fn));
    lp.ir_build.SetInsertPoint(bb);
    llvm::Value* ret_val = lp.ir_build.CreateBinOp(
        llvm::Instruction::BinaryOps(op), fn->getArg(0), fn->getArg(1));
    lp.ir_build.CreateRet(ret_val);
}

void
emit_cmp_fn(const std::string& fn_name,
    const llvm::CmpInst::Predicate& cmp_pred, llvm::Type* op_ty, llvm_pack& lp)
{
    std::vector<llvm::Type*> params { op_ty, op_ty };
    llvm::FunctionType* cmp_fn_ty(
        llvm::FunctionType::get(llvm::Type::getInt1Ty(lp.ctx), params, false));
    llvm::Function* fn(llvm::Function::Create(
        cmp_fn_ty, llvm::Function::ExternalLinkage, fn_name, lp.mod));

    llvm::BasicBlock* bb(llvm::BasicBlock::Create(lp.ctx, "", fn));
    lp.ir_build.SetInsertPoint(bb);
    llvm::Value* ret_val
        = lp.ir_build.CreateCmp(cmp_pred, fn->getArg(0), fn->getArg(1));
    lp.ir_build.CreateRet(ret_val);
}

void
emit_other_fns(llvm_pack& lp)
{
    std::string cmp_fn_name;

    // Emit `icmp` snippets
    for (const auto& icmp_pred : llvm::CmpInst::ICmpPredicates())
    {
        cmp_fn_name = snippet_prefix + std::string("icmp_")
            + llvm::CmpInst::getPredicateName(icmp_pred).str();
        emit_cmp_fn(cmp_fn_name, icmp_pred, llvm::Type::getInt64Ty(lp.ctx), lp);
    }

    // Emit `fcmp` snippets
    for (const auto& fcmp_pred : llvm::CmpInst::FCmpPredicates())
    {
        cmp_fn_name = snippet_prefix + std::string("fcmp_")
            + llvm::CmpInst::getPredicateName(fcmp_pred).str();
        emit_cmp_fn(
            cmp_fn_name, fcmp_pred, llvm::Type::getDoubleTy(lp.ctx), lp);
    }
}

int
main()
{
    auto ctx = std::make_unique<llvm::LLVMContext>();
    auto bld = std::make_unique<llvm::IRBuilder<>>(*ctx);
    auto mod = std::make_unique<llvm::Module>("Snippet", *ctx);
    mod->setTargetTriple("x86_64-pc-linux-gnu"); // TODO ?
    llvm_pack lp { *ctx, *mod, *bld };

    unsigned int op_int;
    uint8_t op_counter;
    for (const auto& [opcode_lims, emit_fn] : emit_fn_map)
    {
        op_counter = 0;
        op_int = opcode_lims.first;
        while (op_int + op_counter < opcode_lims.second)
        {
            emit_fn(op_int + op_counter, lp);
            op_counter += 1;
        }
    }
    emit_other_fns(lp);

    llvm::verifyModule(*mod);
    std::error_code ec;
    std::filesystem::path snip_out_path(snippets_lib_path);
    snip_out_path.replace_extension("ll");
    snip_out_path.replace_filename(
        snip_out_path.filename().string().substr(strlen("lib")));
    llvm::raw_fd_ostream snip_out(snip_out_path.string(), ec);
    mod->print(snip_out, nullptr);

    return 0;
}
