#include "llvm_gen.hpp"

const std::string snippet_prefix = "llvm_impl_"; // TODO
std::vector<llvm_impl_def> fn_defs;

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

llvm::Function*
make_llvm_fn(const std::string& fn_name, llvm::FunctionType* fn_ty,
    unsigned int instr_opcode, const std::string& instr_extra,
    llvm::Module& mod)
{
    llvm::Function* fn(llvm::Function::Create(
        fn_ty, llvm::Function::ExternalLinkage, fn_name, mod));
    record_impl_def(fn, instr_opcode, instr_extra);
    return fn;
}

/* We use this for both `binary` and `bitwise_binary` instructions, as their
 * signatures are identical
 */
void
emit_binop_fns(unsigned int op, llvm_pack& lp)
{
    llvm::Type* binop_ty;
    std::string binop_name
        = make_llvm_snippet_name(llvm::Instruction::getOpcodeName(op));

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
    llvm::Function* fn = make_llvm_fn(binop_name, binop_fn_ty, op, "", lp.mod);

    llvm::BasicBlock* bb(llvm::BasicBlock::Create(lp.ctx, "", fn));
    lp.ir_build.SetInsertPoint(bb);
    llvm::Value* ret_val = lp.ir_build.CreateBinOp(
        llvm::Instruction::BinaryOps(op), fn->getArg(0), fn->getArg(1));
    lp.ir_build.CreateRet(ret_val);
}

void
emit_cmp_fn(
    const llvm::CmpInst::Predicate& cmp_pred, llvm::Type* op_ty, llvm_pack& lp)
{
    const std::string cmp_extra
        = llvm::CmpInst::getPredicateName(cmp_pred).str();
    unsigned int cmp_opcode = op_ty->isIntegerTy() ? llvm::Instruction::ICmp
                                                   : llvm::Instruction::FCmp;
    const std::string cmp_ty = llvm::Instruction::getOpcodeName(cmp_opcode);
    //const std::string cmp_name = snippet_prefix + cmp_ty + "_" + cmp_extra;
    const std::string cmp_name = make_llvm_snippet_name(cmp_ty, cmp_extra);

    std::vector<llvm::Type*> params { op_ty, op_ty };
    llvm::FunctionType* cmp_fn_ty(
        llvm::FunctionType::get(llvm::Type::getInt1Ty(lp.ctx), params, false));
    llvm::Function* fn
        = make_llvm_fn(cmp_name, cmp_fn_ty, cmp_opcode, cmp_extra, lp.mod);

    llvm::BasicBlock* bb(llvm::BasicBlock::Create(lp.ctx, "", fn));
    lp.ir_build.SetInsertPoint(bb);
    llvm::Value* ret_val
        = lp.ir_build.CreateCmp(cmp_pred, fn->getArg(0), fn->getArg(1));
    lp.ir_build.CreateRet(ret_val);
}

void
emit_conversion_fns(llvm_pack& lp)
{
    llvm::Type* fptosi_ty = llvm::Type::getInt64Ty(lp.ctx);
    llvm::Type* op_ty = llvm::Type::getDoubleTy(lp.ctx);
    llvm::FunctionType* conv_fn_ty(
        llvm::FunctionType::get(fptosi_ty, op_ty, false));
    const std::string conv_name = make_llvm_snippet_name("fptosi");
    llvm::Function* fn = make_llvm_fn(conv_name,
        conv_fn_ty, llvm::Instruction::FPToSI, "", lp.mod);

    llvm::BasicBlock* bb(llvm::BasicBlock::Create(lp.ctx, "", fn));
    lp.ir_build.SetInsertPoint(bb);
    llvm::Value* ret_val = lp.ir_build.CreateFPToSI(fn->getArg(0), fptosi_ty);
    lp.ir_build.CreateRet(ret_val);
}

void
emit_other_fns(llvm_pack& lp)
{

    // Emit `icmp` snippets
    for (const auto& icmp_pred : llvm::CmpInst::ICmpPredicates())
    {
        emit_cmp_fn(icmp_pred, llvm::Type::getInt64Ty(lp.ctx), lp);
    }

    // Emit `fcmp` snippets
    for (const auto& fcmp_pred : llvm::CmpInst::FCmpPredicates())
    {
        emit_cmp_fn(fcmp_pred, llvm::Type::getDoubleTy(lp.ctx), lp);
    }
}

void
emit_impl_def(const std::string& def_path)
{
    std::ostringstream defs;
    for (const llvm_impl_def& fn_def : fn_defs)
    {
        defs << fn_def.opcode << ",";
        defs << fn_def.helper << ",";
        defs << fn_def.name;
        defs << "(" << fn_def.ret_ty << ")";
        defs << "[";
        if (!fn_def.params_ty.empty())
        {
            defs << std::accumulate(std::next(fn_def.params_ty.begin()),
                fn_def.params_ty.end(), fn_def.params_ty.front(),
                [](std::string s, std::string param)
                { return std::move(s) + ',' + param; });
        }
        defs << "]\n";
    }
    std::ofstream defs_out;
    defs_out.open(def_path);
    defs_out << defs.str();
    defs_out.close();
}

void
record_impl_def(const llvm::Function* llvm_fn, unsigned int instr_opcode,
    const std::string& instr_extra)
{
    std::string ret_ty;
    llvm::raw_string_ostream ret_rso(ret_ty);
    llvm_fn->getReturnType()->print(ret_rso);

    std::vector<std::string> fn_params;
    for (const llvm::Type* p_ty : llvm_fn->getFunctionType()->params())
    {
        std::string rso_buf;
        llvm::raw_string_ostream rso(rso_buf);
        p_ty->print(rso);
        fn_params.push_back(rso_buf);
    }

    fn_defs.emplace_back(
        llvm_fn->getName().str(), ret_ty, fn_params, instr_opcode, instr_extra);
}

void
emit_impl_header(const std::string& header_path)
{
    std::unordered_map<std::string, std::string> type_map {
        { "i64", "int64_t" },
        { "i1", "bool" },
        { "double", "double" },
        { "float", "float" },
    };

    std::ostringstream hss;
    hss << "#include <cstdint>\n\n";
    hss << "extern \"C\"\n";
    hss << "{";

    for (const llvm_impl_def& fn_def : fn_defs)
    {
        hss << "\n\t" << type_map[fn_def.ret_ty];
        hss << " " << fn_def.name;
        hss << "(";
        if (!fn_def.params_ty.empty())
        {
            hss << std::accumulate(std::next(fn_def.params_ty.begin()),
                fn_def.params_ty.end(), type_map[fn_def.params_ty.front()],
                [&type_map](std::string s, std::string param)
                { return std::move(s) + ", " + type_map[param]; });
        }
        hss << ");";
    }
    hss << "\n}";

    std::ofstream header_out;
    header_out.open(header_path);
    header_out << hss.str();
    header_out.close();
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
    emit_conversion_fns(lp);
    emit_other_fns(lp);

    llvm::verifyModule(*mod);
    std::error_code ec;
    llvm::raw_fd_ostream snip_out(make_snippets_ll_path(), ec);
    mod->print(snip_out, nullptr);

    emit_impl_def(make_snippets_def_path());
    emit_impl_header(make_snippets_header_path());

    return 0;
}
