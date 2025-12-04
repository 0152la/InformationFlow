#include "llvm_gen.hpp"
#include <llvm/IR/IRBuilder.h>

const std::string snippet_prefix = "llvm_impl_"; // TODO
static std::vector<llvm_impl_def> fn_defs;

/* Map from ranges of llvm opcodes to functions to produce LLVM snippets
 * <<llvm_opcode_begin, llvm_opcode_end>, snippet_gen_function>
 */
// clang-format off
static const std::vector<std::pair<std::pair<unsigned int, unsigned int>,
    std::function<void(unsigned int, llvm_pack&)>>>
    emit_fn_map {
        { { llvm::Instruction::BinaryOpsBegin,
                        llvm::Instruction::BinaryOpsEnd },
                      emit_binop_fns },
        { { llvm::CmpInst::FIRST_FCMP_PREDICATE, llvm::CmpInst::LAST_FCMP_PREDICATE },
            emit_cmp_fn_flt },
        { { llvm::CmpInst::FIRST_ICMP_PREDICATE, llvm::CmpInst::LAST_ICMP_PREDICATE },
            emit_cmp_fn_int },
    };
// clang-format on

static const std::array ignored_binops { llvm::Instruction::Add,
    llvm::Instruction::Sub, llvm::Instruction::Or, llvm::Instruction::And,
    llvm::Instruction::Xor };

static const std::array binops_float { llvm::Instruction::FAdd,
    llvm::Instruction::FSub, llvm::Instruction::FMul, llvm::Instruction::FDiv,
    llvm::Instruction::FRem };
static const std::array ignored_other_ops { llvm::Instruction::PHI };

// using cast_op_create_fn_ty = std::mem_fn;
static const std::unordered_map<unsigned int, cast_op_create_fn_ty>
    cast_op_create_fn {
        { llvm::Instruction::FPToSI,
            std::mem_fn(&llvm::IRBuilderBase::CreateFPToSI) },
        { llvm::Instruction::FPToUI,
            std::mem_fn(&llvm::IRBuilderBase::CreateFPToUI) },
        { llvm::Instruction::SIToFP,
            std::mem_fn(&llvm::IRBuilderBase::CreateSIToFP) },
        { llvm::Instruction::UIToFP,
            std::bind(std::mem_fn(&llvm::IRBuilderBase::CreateUIToFP),
                std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3, std::placeholders::_4, false) },
    };

static const std::vector<cast_op_data> cast_ops {
    { llvm::Instruction::FPToSI, &llvm::Type::getInt16Ty,
        &llvm::Type::getHalfTy, "f16" },
    { llvm::Instruction::FPToUI, &llvm::Type::getHalfTy,
        &llvm::Type::getInt16Ty, "f16" },
    { llvm::Instruction::SIToFP, &llvm::Type::getInt16Ty,
        &llvm::Type::getHalfTy, "f16" },
    { llvm::Instruction::UIToFP, &llvm::Type::getHalfTy,
        &llvm::Type::getInt16Ty, "f16" },
    { llvm::Instruction::FPToSI, &llvm::Type::getInt32Ty,
        &llvm::Type::getFloatTy, "f32" },
    { llvm::Instruction::FPToUI, &llvm::Type::getFloatTy,
        &llvm::Type::getInt32Ty, "f32" },
    { llvm::Instruction::SIToFP, &llvm::Type::getInt32Ty,
        &llvm::Type::getFloatTy, "f32" },
    { llvm::Instruction::UIToFP, &llvm::Type::getFloatTy,
        &llvm::Type::getInt32Ty, "f32" },
};

/*******************************************************************************
 * Utility functions
 ******************************************************************************/

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

llvm::Function*
make_llvm_fn_cmp(const std::string& fn_name, llvm::FunctionType* fn_ty,
    unsigned int instr_opcode, unsigned int cmp_pred,
    const std::string& instr_extra, llvm::Module& mod)
{
    llvm::Function* fn
        = make_llvm_fn(fn_name, fn_ty, instr_opcode, instr_extra, mod);
    fn_defs.back().set_cmp_pred(cmp_pred);
    return fn;
}

/*******************************************************************************
 * Emitting functions
 ******************************************************************************/

/* We use this for both `binary` and `bitwise_binary` instructions, as their
 * signatures are identical
 */
void
emit_binop_fns(unsigned int op, llvm_pack& lp)
{
    // We don't want to emit certain instructions, as these have proven
    // (empirical or hand-derived) fixed uncertainty coefficient values
    if (std::find(ignored_binops.begin(), ignored_binops.end(), op)
        != ignored_binops.end())
    {
        return;
    }

    llvm::Type* binop_ty;
    std::string binop_name
        = make_llvm_snippet_name(llvm::Instruction::getOpcodeName(op));

    if (std::find(binops_float.begin(), binops_float.end(), op)
        != binops_float.end())
    {
        binop_ty = llvm::Type::getHalfTy(lp.ctx);
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
    std::string cmp_extra = "";

    unsigned int cmp_opcode;
    if (llvm::CmpInst::isIntPredicate(cmp_pred))
    {
        cmp_opcode = llvm::Instruction::ICmp;
    }
    else if (llvm::CmpInst::isFPPredicate(cmp_pred))
    {
        cmp_opcode = llvm::Instruction::FCmp;
        llvm::DataLayout dl = llvm::DataLayout(&lp.mod);
        cmp_extra = std::to_string(dl.getTypeSizeInBits(op_ty));
    }
    else
    {
        throw std::runtime_error("Unhandled CmpInst Predicate type!");
    }
    const std::string cmp_name
        = make_llvm_snippet_name_cmp(cmp_opcode, cmp_pred, cmp_extra);

    std::vector<llvm::Type*> params { op_ty, op_ty };
    llvm::FunctionType* cmp_fn_ty(
        llvm::FunctionType::get(llvm::Type::getInt1Ty(lp.ctx), params, false));
    llvm::Function* fn = make_llvm_fn_cmp(
        cmp_name, cmp_fn_ty, cmp_opcode, cmp_pred, cmp_extra, lp.mod);

    llvm::BasicBlock* bb(llvm::BasicBlock::Create(lp.ctx, "", fn));
    lp.ir_build.SetInsertPoint(bb);
    llvm::Value* ret_val
        = lp.ir_build.CreateCmp(cmp_pred, fn->getArg(0), fn->getArg(1));
    lp.ir_build.CreateRet(ret_val);
}

void
emit_cmp_fn_int(unsigned int pred, llvm_pack& lp)
{
    emit_cmp_fn(
        (llvm::CmpInst::Predicate) pred, llvm::Type::getInt64Ty(lp.ctx), lp);
}

void
emit_cmp_fn_flt(unsigned int pred, llvm_pack& lp)
{
    emit_cmp_fn(
        (llvm::CmpInst::Predicate) pred, llvm::Type::getHalfTy(lp.ctx), lp);
    emit_cmp_fn(
        (llvm::CmpInst::Predicate) pred, llvm::Type::getFloatTy(lp.ctx), lp);
}

void
emit_conversion_fns(llvm_pack& lp)
{
    // for (const auto& [opcode, create_fn] : cast_ops)
    for (const auto& co : cast_ops)
    {
        llvm::Type* ret_ty = co.ret_ty_fn(lp.ctx);
        llvm::Type* op_ty = co.arg_ty_fn(lp.ctx);

        llvm::FunctionType* conv_fn_ty(
            llvm::FunctionType::get(ret_ty, op_ty, false));
        const std::string conv_name = make_llvm_snippet_name(
            llvm::Instruction::getOpcodeName(co.opcode));
        llvm::Function* fn = make_llvm_fn(
            conv_name, conv_fn_ty, co.opcode, co.name_extra, lp.mod);

        llvm::BasicBlock* bb(llvm::BasicBlock::Create(lp.ctx, "", fn));
        lp.ir_build.SetInsertPoint(bb);
        // llvm::Value* ret_val = lp.ir_build.CreateFPToSI(fn->getArg(0),
        // ret_ty);
        llvm::Value* ret_val
            //= create_fn(lp.ir_build, fn->getArg(0), ret_ty, "");
            = (cast_op_create_fn.at(co.opcode))(
                lp.ir_build, fn->getArg(0), ret_ty, "");
        lp.ir_build.CreateRet(ret_val);
    }
}

/*******************************************************************************
 * `Def` file functions
 ******************************************************************************/

void
emit_impl_def(const std::string& def_path)
{
    std::ostringstream defs;

    const std::vector<std::string> header { "opcode", "cmp_opcode",
        "name_extra", "fn_name", "ret_ty", "params_ty" };
    defs << std::accumulate(std::next(header.begin()), header.end(),
        header.front(),
        [](std::string s, std::string h) { return std::move(s) + ',' + h; })
         << '\n';

    for (const llvm_impl_def& fn_def : fn_defs)
    {
        defs << fn_def.opcode << ",";
        defs << (fn_def.cmp_pred == (unsigned int) -1
                ? ""
                : std::to_string(fn_def.cmp_pred))
             << ",";
        defs << fn_def.extra << ",";
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

/*******************************************************************************
 * Header file functions
 ******************************************************************************/

void
emit_impl_header_cpp(const std::string& header_path)
{
    std::ostringstream hss;
    hss << "#include <cstdint>\n\n";
    hss << "extern \"C\"\n";
    hss << "{\n";
    hss << "#include \"" << config::make_snippets_header_path() << "\"\n";
    hss << "}";

    std::ofstream header_out;
    header_out.open(header_path);
    header_out << hss.str();
    header_out.close();
}

void
emit_impl_header(const std::string& header_path)
{
    std::unordered_map<std::string, std::string> type_map {
        { "i64", "int64_t" },
        { "i16", "int16_t" },
        { "i1", "bool" },
        { "double", "double" },
        { "float", "float" },
        { "half", "_Float16" },
    };

    std::ostringstream hss;
    for (const llvm_impl_def& fn_def : fn_defs)
    {
        hss << type_map[fn_def.ret_ty];
        hss << " " << fn_def.name;
        hss << "(";
        if (!fn_def.params_ty.empty())
        {
            hss << std::accumulate(std::next(fn_def.params_ty.begin()),
                fn_def.params_ty.end(), type_map[fn_def.params_ty.front()],
                [&type_map](std::string s, std::string param)
                { return std::move(s) + ", " + type_map[param]; });
        }
        hss << ");\n";
    }

    std::ofstream header_out;
    header_out.open(header_path);
    header_out << hss.str();
    header_out.close();
}

/*******************************************************************************
 * Main
 *
 * Generates three files to be used by other parts of the toolchain
 * - a `llvm_snippets.ll` file, containing the implementation in LLVM IR of
 * the operations of interest for the rest of the infrastructure; this will
 * be compiled to a library file, to be consumed by `entropy_eval`, to
 * evaluate empirically the uncertainty coefficient values
 * - a `llvm_snippets.def` file, which contains various metadata of the
 *   generated instructions above; this is used by various parts of the
 *   toolchain in order to know which functions we are looking for the
 *   empirical data for
 * - a `llvm_snippets.{h,hpp}`, header files to be used to refer directly to
 *   functions in the shared library compiled from `llvm_snippets.ll`
 ******************************************************************************/

int
main()
{
    auto ctx = std::make_unique<llvm::LLVMContext>();
    auto bld = std::make_unique<llvm::IRBuilder<>>(*ctx);
    auto mod = std::make_unique<llvm::Module>("Snippet", *ctx);
    mod->setTargetTriple("x86_64-unknown-linux-gnu"); // TODO ?
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

    llvm::verifyModule(*mod);
    std::error_code ec;
    llvm::raw_fd_ostream snip_out(config::make_snippets_ll_path(), ec);
    mod->print(snip_out, nullptr);

    emit_impl_def(config::make_snippets_def_path());
    emit_impl_header(config::make_snippets_header_path());
    emit_impl_header_cpp(config::make_snippets_header_path_cpp());

    return 0;
}
