#include "instr_emulator.hpp"

std::map<uint16_t, std::function<IF_Arg(const IF_ArgList&)>> emulated_fns {
    // Binary Operations
    { llvm::Instruction::Add, IF_Emulator::emulate_add },
    { llvm::Instruction::Sub, IF_Emulator::emulate_sub },
    { llvm::Instruction::Mul, IF_Emulator::emulate_mul },
    { llvm::Instruction::UDiv, IF_Emulator::emulate_udiv },
    { llvm::Instruction::SDiv, IF_Emulator::emulate_sdiv },
    { llvm::Instruction::URem, IF_Emulator::emulate_urem },
    { llvm::Instruction::SRem, IF_Emulator::emulate_srem },

    // Bitwise Binary Operations
    { llvm::Instruction::Shl, IF_Emulator::emulate_shl },
    { llvm::Instruction::LShr, IF_Emulator::emulate_lshr },
    { llvm::Instruction::AShr, IF_Emulator::emulate_ashr },
    { llvm::Instruction::And, IF_Emulator::emulate_and },
    { llvm::Instruction::Or, IF_Emulator::emulate_or },
    { llvm::Instruction::Xor, IF_Emulator::emulate_xor },

    // Memory Operations
    { llvm::Instruction::AtomicRMW, IF_Emulator::emulate_atomic_rmw },

    // Conversion Operations
    { llvm::Instruction::FPToSI, IF_Emulator::emulate_fptosi },

    // Other Operations
    { llvm::Instruction::ICmp, IF_Emulator::emulate_icmp },
};

std::map<uint16_t, std::function<double(const llvm::Instruction&)>>
    estimate_entropy {
        // Vector Operations
        { llvm::Instruction::ExtractElement,
            IF_Emulator::estimate_extract_element },

        // Aggregate Operations
        { llvm::Instruction::ExtractValue,
            IF_Emulator::estimate_extract_value },

        // Converstion Operations
        { llvm::Instruction::Trunc, IF_Emulator::estimate_trunc },
        { llvm::Instruction::PtrToInt, IF_Emulator::estimate_ptrtoint },

        // Other Operations
        { llvm::Instruction::PHI, IF_Emulator::estimate_phi },
    };

/*******************************************************************************
 * IF_ArgInt
 ******************************************************************************/

IF_Arg_Int::IF_Arg_Int(val_t _val, uint8_t _sz, bool _sgn) :
    IF_Arg(_sz),
    val(_val),
    sgn(_sgn)
{
    if (sgn && _val < 0)
    {
        throw std::invalid_argument("Given negative value `"
            + std::to_string(val) + "` for unsigned argument!");
    }
}

IF_Arg_Int::IF_Arg_Int(val_t _val, uint8_t _sz) :
    IF_Arg_Int(_val, _sz, false)
{
    if (_val > (1 << _sz))
    {
        throw std::invalid_argument("Invalid value `" + std::to_string(val)
            + "` given for size `" + std::to_string(sz) + "`!");
    }
}

/*******************************************************************************
 * IF_ArgFloat
 ******************************************************************************/

IF_Arg_Float::IF_Arg_Float(double _val, const llvm::Type* ty) :
    IF_Arg(IF_Arg_Float::get_float_sz(ty)),
    val(_val) { };

uint8_t
IF_Arg_Float::get_float_sz(const llvm::Type* ty)
{
    switch (ty->getTypeID())
    {
        case llvm::Type::TypeID::HalfTyID:
            return 16;
        case llvm::Type::TypeID::BFloatTyID:
            return 16;
        case llvm::Type::TypeID::FloatTyID:
            return 32;
        case llvm::Type::TypeID::DoubleTyID:
            return 64;
        case llvm::Type::TypeID::X86_FP80TyID:
            // return 80;
            throw std::runtime_error("I wanna see what this type is.");
        case llvm::Type::TypeID::FP128TyID:
            return 128;
        case llvm::Type::TypeID::PPC_FP128TyID:
            // return 128;
            throw std::runtime_error("I wanna see what this type is 2.");
        default:
            throw std::runtime_error("Invalid float type!");
    }
}

/*******************************************************************************
 * IF_ArgList
 ******************************************************************************/

const IF_Arg&
IF_ArgList::get_arg(size_t idx) const
{
    if (idx > this->get_args_count())
    {
        throw std::logic_error("Attempt to retrieve arg `" + std::to_string(idx)
            + " of " + std::to_string(this->get_args_count()) + " total!");
    }
    return *this->get_args().at(idx);
}

const IF_Arg*
IF_ArgList::get_arg_ptr(size_t idx) const
{
    if (idx > this->get_args_count())
    {
        throw std::logic_error("Attempt to retrieve arg `" + std::to_string(idx)
            + " of " + std::to_string(this->get_args_count()) + " total!");
    }
    return this->get_args().at(idx).get();
}

std::vector<const IF_Arg_Int*>
IF_ArgList::extract_arg_int(void) const
{
    std::vector<const IF_Arg_Int*> args_int;
    for (const auto& one_arg : this->get_args())
    {
        args_int.push_back(dynamic_cast<const IF_Arg_Int*>(one_arg.get()));
    }
    return args_int;
}

void
IF_ArgList::check_args(bool cond)
{
    if (!cond)
    {
        throw std::runtime_error("Unmet argument conditions!");
    }
}

bool
IF_ArgList::check_all_ints(void) const
{
    for (const auto& check : this->args)
    {
        if (!check->is_int())
        {
            return false;
        }
    }
    return true;
};

bool
IF_ArgList::check_all_floats(void) const
{
    for (const auto& check : this->args)
    {
        if (!check->is_float())
        {
            return false;
        }
    }
    return true;
};

bool
IF_ArgList::check_all_same_sz(void) const
{
    if (this->args.size() == 1)
    {
        return true;
    }

    uint8_t sz_check = this->args.at(0)->get_sz();
    for (const auto& check : this->args | std::views::drop(1))
    {
        if (check->get_sz() != sz_check)
        {
            return false;
        }
    }
    return true;
};

bool
IF_ArgList::check_count(uint8_t to_check) const
{
    return this->args.size() == to_check;
}

/*******************************************************************************
 * IF_Emulator
 ******************************************************************************/

std::vector<IF_Arg_Int::val_t>
IF_Emulator::extract_binop_ops(const IF_ArgList& args)
{
    args.check_args(args.check_all_ints() && args.check_all_same_sz()
        && args.check_count(2));

    std::vector<const IF_Arg_Int*> args_int = args.extract_arg_int();
    IF_Arg_Int::val_t op1 = args_int.at(0)->get_val();
    IF_Arg_Int::val_t op2 = args_int.at(1)->get_val();

    return std::vector<IF_Arg_Int::val_t> { op1, op2 };
}

/*******************************************************************************
 * Emulated instructions */

/* Binary Operations **********************************************************/
// TODO there are `nsw` and `nuw` flags for wrapping, to emit poison. Clamping
// at `max` / `min` always is probably a good proxy, but consider if we want to
// further ensure we only clamp when the respective flag is given
IF_Arg
IF_Emulator::emulate_add(const IF_ArgList& args)
{
    std::vector<IF_Arg_Int::val_t> args_val = extract_binop_ops(args);
    IF_Arg_Int::val_t op1 = args_val.at(0);
    IF_Arg_Int::val_t op2 = args_val.at(1);
    IF_Arg_Int::val_t sum = op1 + op2;

    if (op1 > 0 && op2 > 0 && sum <= 0)
    {
        sum = std::numeric_limits<IF_Arg_Int::val_t>::max();
    }
    else if (op1 < 0 && op2 < 0 && sum >= 0)
    {
        sum = std::numeric_limits<IF_Arg_Int::val_t>::min();
    }
    // return IF_Arg_Int(sum, sizeof(IF_Arg_Int::val_t), op1_arg.is_sgn());
    return IF_Arg_Int(sum, sizeof(IF_Arg_Int::val_t), false);
}

IF_Arg
IF_Emulator::emulate_sub(const IF_ArgList& args)
{
    std::vector<IF_Arg_Int::val_t> args_val = extract_binop_ops(args);
    IF_Arg_Int::val_t op1 = args_val.at(0);
    IF_Arg_Int::val_t op2 = args_val.at(1);
    IF_Arg_Int::val_t diff = op1 - op2;

    if (op1 > 0 && op2 < 0 && diff <= 0)
    {
        diff = std::numeric_limits<IF_Arg_Int::val_t>::max();
    }
    else if (op1 < 0 && op2 > 0 && diff >= 0)
    {
        diff = std::numeric_limits<IF_Arg_Int::val_t>::min();
    }
    // return IF_Arg_Int(diff, sizeof(IF_Arg_Int::val_t), op1_arg.is_sgn());
    return IF_Arg_Int(diff, sizeof(IF_Arg_Int::val_t), false);
}

IF_Arg
IF_Emulator::emulate_mul(const IF_ArgList& args)
{
    std::vector<IF_Arg_Int::val_t> args_val = extract_binop_ops(args);
    IF_Arg_Int::val_t op1 = args_val.at(0);
    IF_Arg_Int::val_t op2 = args_val.at(1);
    IF_Arg_Int::val_t prod = op1 * op2;

    if (op1 != 0 && prod / op1 != op2)
    {
        prod = std::numeric_limits<IF_Arg_Int::val_t>::max();
    }
    // return IF_Arg_Int(prod, sizeof(IF_Arg_Int::val_t), op1_arg.is_sgn());
    return IF_Arg_Int(prod, sizeof(IF_Arg_Int::val_t), false);
}

// TODO check `exact`
IF_Arg
IF_Emulator::emulate_udiv(const IF_ArgList& args)
{
    std::vector<IF_Arg_Int::val_t> args_val = extract_binop_ops(args);
    IF_Arg_Int::val_t op1 = args_val.at(0);
    IF_Arg_Int::val_t op2 = args_val.at(1);

    IF_Arg_Int::val_t uquot;
    if (op2 == 0)
    {
        uquot = 0;
    }
    else
    {
        uquot = op1 / op2;
    }

    return IF_Arg_Int(uquot, sizeof(IF_Arg_Int::val_t), false);
}

// TODO check `exact`
// TODO check overflow (dividing `min` by `-1`) - probably can ignore?
IF_Arg
IF_Emulator::emulate_sdiv(const IF_ArgList& args)
{
    std::vector<IF_Arg_Int::val_t> args_val = extract_binop_ops(args);
    IF_Arg_Int::val_t op1 = args_val.at(0);
    IF_Arg_Int::val_t op2 = args_val.at(1);

    IF_Arg_Int::val_t squot;
    if (op2 == 0)
    {
        squot = 0;
    }
    else
    {
        squot = op1 / op2;
    }

    return IF_Arg_Int(squot, sizeof(IF_Arg_Int::val_t), true);
}

IF_Arg
IF_Emulator::emulate_urem(const IF_ArgList& args)
{
    std::vector<IF_Arg_Int::val_t> args_val = extract_binop_ops(args);
    IF_Arg_Int::val_t op1 = args_val.at(0);
    IF_Arg_Int::val_t op2 = args_val.at(1);

    IF_Arg_Int::val_t urem;
    if (op2 == 0)
    {
        urem = 0;
    }
    else
    {
        urem = op1 % op2;
    }

    return IF_Arg_Int(urem, sizeof(IF_Arg_Int::val_t), false);
}

IF_Arg
IF_Emulator::emulate_srem(const IF_ArgList& args)
{
    std::vector<IF_Arg_Int::val_t> args_val = extract_binop_ops(args);
    IF_Arg_Int::val_t op1 = args_val.at(0);
    IF_Arg_Int::val_t op2 = args_val.at(1);

    IF_Arg_Int::val_t srem;
    if (op2 == 0)
    {
        srem = 0;
    }
    else
    {
        srem = op1 % op2;
    }

    return IF_Arg_Int(srem, sizeof(IF_Arg_Int::val_t), true);
}

/* Bitwitse Binary Operations *************************************************/

IF_Arg
IF_Emulator::emulate_shl(const IF_ArgList& args)
{
    std::vector<IF_Arg_Int::val_t> args_val = extract_binop_ops(args);
    IF_Arg_Int::val_t op1 = args_val.at(0);
    IF_Arg_Int::val_t op2 = args_val.at(1);
    IF_Arg_Int::val_t shl;

    if (op2 >= args.get_arg(0).get_sz())
    {
        shl = std::numeric_limits<IF_Arg_Int::val_t>::max();
    }
    else
    {
        shl = op1 << op2;
    }

    return IF_Arg_Int(shl, sizeof(IF_Arg_Int::val_t), true);
}

IF_Arg
IF_Emulator::emulate_lshr(const IF_ArgList& args)
{
    std::vector<IF_Arg_Int::val_t> args_val = extract_binop_ops(args);
    IF_Arg_Int::val_t op1 = args_val.at(0);
    IF_Arg_Int::val_t op2 = args_val.at(1);
    IF_Arg_Int::val_t lshr;

    if (op2 >= args.get_arg(0).get_sz())
    {
        lshr = 0;
    }
    else
    {
        // TODO days in tears: 1
        IF_Arg_Int::val_t mask = 0 << (args.get_arg(0).get_sz() - 1);
        lshr = (op1 >> op2) & mask;
    }

    throw std::runtime_error("unimplemented lshr");
    return IF_Arg_Int(lshr, sizeof(IF_Arg_Int::val_t), true);
}

IF_Arg
IF_Emulator::emulate_ashr(const IF_ArgList& args)
{
    std::vector<IF_Arg_Int::val_t> args_val = extract_binop_ops(args);
    IF_Arg_Int::val_t op1 = args_val.at(0);
    IF_Arg_Int::val_t op2 = args_val.at(1);
    IF_Arg_Int::val_t ashr;

    if (op2 >= args.get_arg(0).get_sz())
    {
        ashr = 0;
    }
    else
    {
        // TODO ensure this is an arithmetic shift
        ashr = op1 >> op2;
    }

    return IF_Arg_Int(ashr, sizeof(IF_Arg_Int::val_t), true);
}

IF_Arg
IF_Emulator::emulate_and(const IF_ArgList& args)
{
    std::vector<IF_Arg_Int::val_t> args_val = extract_binop_ops(args);
    IF_Arg_Int::val_t op1 = args_val.at(0);
    IF_Arg_Int::val_t op2 = args_val.at(1);
    IF_Arg_Int::val_t and_res;

    return IF_Arg_Int(op1 & op2, sizeof(IF_Arg_Int::val_t), true);
}

IF_Arg
IF_Emulator::emulate_or(const IF_ArgList& args)
{
    std::vector<IF_Arg_Int::val_t> args_val = extract_binop_ops(args);
    IF_Arg_Int::val_t op1 = args_val.at(0);
    IF_Arg_Int::val_t op2 = args_val.at(1);
    IF_Arg_Int::val_t or_res;

    return IF_Arg_Int(op1 | op2, sizeof(IF_Arg_Int::val_t), true);
}

IF_Arg
IF_Emulator::emulate_xor(const IF_ArgList& args)
{
    std::vector<IF_Arg_Int::val_t> args_val = extract_binop_ops(args);
    IF_Arg_Int::val_t op1 = args_val.at(0);
    IF_Arg_Int::val_t op2 = args_val.at(1);
    IF_Arg_Int::val_t xor_res;

    return IF_Arg_Int(op1 ^ op2, sizeof(IF_Arg_Int::val_t), true);
}

/* Memory Operations **********************************************************/

IF_Arg
IF_Emulator::emulate_atomic_rmw(const IF_ArgList& args)
{
    // TODO
    const IF_ArgList& args2 = args;
    throw std::runtime_error("unimplemented rmw");
    return IF_Arg_Int(1, sizeof(IF_Arg_Int::val_t), false);
}

/* Conversion Operations ******************************************************/

IF_Arg
IF_Emulator::emulate_fptosi(const IF_ArgList& args)
{
    args.check_args(args.check_count(1) && args.check_all_floats());
    const decltype(std::declval<IF_Arg_Float>().get_val()) fp
        = dynamic_cast<const IF_Arg_Float*>(args.get_arg_ptr(0))->get_val();
    return IF_Arg_Int(std::trunc(fp), 32, false); // TODO
}

/* Other Operations ***********************************************************/

IF_Arg
IF_Emulator::emulate_icmp(const IF_ArgList& args)
{
    // TODO check for ints at least
    args.check_args(args.check_count(3));

    IF_Arg_Int::val_t op1
        = dynamic_cast<const IF_Arg_Int*>(args.get_arg_ptr(0))->get_val();
    IF_Arg_Int::val_t op2
        = dynamic_cast<const IF_Arg_Int*>(args.get_arg_ptr(1))->get_val();
    llvm::CmpInst::Predicate pred
        = dynamic_cast<const IF_Cmp_Pred*>(args.get_arg_ptr(2))->get_pred();
    bool icmp_res;

    switch (pred)
    {
        case llvm::CmpInst::Predicate::ICMP_EQ:
            return IF_Arg_Bool(op1 == op2);
        case llvm::CmpInst::Predicate::ICMP_NE:
            return IF_Arg_Bool(op1 != op2);
        // TODO what to do with signed and unsigned versions
        case llvm::CmpInst::Predicate::ICMP_UGT:
            return IF_Arg_Bool(op1 > op2);
        case llvm::CmpInst::Predicate::ICMP_UGE:
            return IF_Arg_Bool(op1 >= op2);
        case llvm::CmpInst::Predicate::ICMP_ULT:
            return IF_Arg_Bool(op1 < op2);
        case llvm::CmpInst::Predicate::ICMP_ULE:
            return IF_Arg_Bool(op1 <= op2);
        case llvm::CmpInst::Predicate::ICMP_SGT:
            return IF_Arg_Bool(op1 > op2);
        case llvm::CmpInst::Predicate::ICMP_SGE:
            return IF_Arg_Bool(op1 >= op2);
        case llvm::CmpInst::Predicate::ICMP_SLT:
            return IF_Arg_Bool(op1 < op2);
        case llvm::CmpInst::Predicate::ICMP_SLE:
            return IF_Arg_Bool(op1 <= op2);
        default:
            throw std::runtime_error("Invalid `ICmp_Inst` predicate!");
    }
}

/*******************************************************************************
 * Estimated operations */

/* Vector Operations **********************************************************/

/* If the width of the index type is equal or smaller than the number of
 * elements in the vector, then we do not lose any entropy, as each result
 * yielded will be unique. Any index outside those bounds will yield `poison`,
 * therefore losing entropy. Thus, the total entropy retained is the ratio
 * between the number of elements in the vector over the bit width of the index
 * type, if the index type is greater than the element count, or 1 (i.e. no
 * entropy lost) otherwise.
 *
 * TODO double check
 */
double
IF_Emulator::estimate_extract_element(const llvm::Instruction& instr)
{
    const llvm::ExtractElementInst* eei
        = llvm::dyn_cast<llvm::ExtractElementInst>(&instr);
    if (!eei)
    {
        throw std::runtime_error(
            "Could not cast instruction to `ExtractElementInst`!");
    }

    uint64_t elem_count;
    if (llvm::isa<llvm::FixedVectorType>(eei->getVectorOperandType()))
    {
        elem_count = (llvm::dyn_cast<llvm::FixedVectorType>(
                          eei->getVectorOperandType()))
                         ->getNumElements();
    }
    else if (llvm::isa<llvm::ScalableVectorType>(eei->getVectorOperandType()))
    {
        // TODO is there a better way?
        elem_count = (llvm::dyn_cast<llvm::ScalableVectorType>(
                          eei->getVectorOperandType()))
                         ->getMinNumElements();
    }
    else
    {
        throw std::runtime_error("Unknown vector type for `extractelement`!");
    }

    uint8_t index_width
        = eei->getIndexOperand()->getType()->getIntegerBitWidth();

    return (1 << index_width) <= elem_count
        ? 1
        : elem_count * 1.0 / (1 << index_width);
}

/* Aggregate Operations *******************************************************/

/* This operation is different than `extractelement`, in three main ways:
 * - the indices *must* be in bounds, therefore we do not care about poison
 * - the types to be indexed are either an array, or a struct
 * - multiple indices are allowed, which is similar to GEP indexing - we
 * recursively index within indexable elements
 *
 * The way we compute entropy is by calculating the number of bits in the
 * indexed data element, then the number of bits in the entire aggregate data
 * value, and computing the ratio. This is guaranteed to be less than one, as
 * the whole is greater than the parts.
 */
double
IF_Emulator::estimate_extract_value(const llvm::Instruction& instr)
{
    const llvm::ExtractValueInst* evi
        = llvm::dyn_cast<llvm::ExtractValueInst>(&instr);

    if (llvm::isa<llvm::ArrayType>(evi->getAggregateOperand()->getType()))
    {
        return 1.0
            / llvm::dyn_cast<llvm::ArrayType>(
                evi->getAggregateOperand()->getType())
                  ->getNumElements();
    }
    else if (llvm::isa<llvm::StructType>(evi->getAggregateOperand()->getType()))
    {
        // TODO more checks
        const llvm::StructType* str_ty = llvm::dyn_cast<llvm::StructType>(
            evi->getAggregateOperand()->getType());
        std::unique_ptr<struct_sz_t> str_sz
            = get_llvm_struct_bitsize(str_ty, instr.getModule());

        uint32_t str_sz_sum = compute_total_struct_sz(str_sz.get());
        uint32_t val_sz = get_aggregate_type_by_idx(str_sz.get(), evi);

        return val_sz * 1.0 / str_sz_sum;
    }
    else
    {
        throw std::runtime_error("Unknown aggregate type for `extractvalue`!");
    }
    throw std::logic_error("Unrechable - extractvalue");
}

/* Conversion Operations ******************************************************/

double
IF_Emulator::estimate_trunc(const llvm::Instruction& instr)
{
    const llvm::TruncInst* ti = llvm::dyn_cast<llvm::TruncInst>(&instr);
    if (!ti)
    {
        throw std::runtime_error(
            "Could not cast instruction to expected `TruncInst`!");
    }

    uint8_t from_sz = ti->getSrcTy()->getIntegerBitWidth();
    uint8_t to_sz = ti->getDestTy()->getIntegerBitWidth();
    if (from_sz <= to_sz)
    {
        throw std::logic_error("Found invalid `trunc` type bit-widths!");
    }

    // TODO double check
    return 1.0 / (1 << (from_sz - to_sz));
}

double
IF_Emulator::estimate_ptrtoint(const llvm::Instruction& instr)
{
    const llvm::PtrToIntInst* ptii = llvm::dyn_cast<llvm::PtrToIntInst>(&instr);
    if (!ptii)
    {
        throw std::runtime_error(
            "Could not cast instruction to expected `PtrToIntInst`!");
    }

    uint8_t from_sz = instr.getModule()->getDataLayout().getPointerSize();
    uint8_t to_sz = ptii->getDestTy()->getIntegerBitWidth();

    // If we need to truncate, then we potentially lose some entropy
    if (from_sz >= to_sz)
    {
        return 1.0 / (1 << (from_sz - to_sz));
    }
    // Otherwise, everything is retained (we do not care about the
    // zero-extension case)
    return 1.0;
}

/* Other Operations ***********************************************************/

// TODO double check this
double
IF_Emulator::estimate_phi(const llvm::Instruction& instr)
{
    const llvm::PHINode* phi = llvm::dyn_cast<llvm::PHINode>(&instr);
    if (!phi)
    {
        throw std::runtime_error(
            "Could not cast instruction to expected `PHINode`!");
    }

    return 1.0 / phi->getNumIncomingValues();
}

/*******************************************************************************
 * Helper functions
 ******************************************************************************/

uint32_t
get_aggregate_type_by_idx(
    const struct_sz_t* struct_sz, const llvm::ExtractValueInst* evi)
{
    auto idx_it = evi->idx_begin();
    while (idx_it + 1 != evi->idx_end())
    // for (const auto& idx : evi->getIndices())
    {
        struct_sz = std::get<struct_sz_s>(struct_sz->at(*idx_it)).t.get();
        idx_it += 1;
    }
    return std::get<uint32_t>(struct_sz->at(*(idx_it + 1)));
}

uint32_t
compute_total_struct_sz(const struct_sz_t* struct_sz)
{
    uint32_t sz = 0;
    for (const auto& one_struct_sz : *struct_sz)
    {
        try
        {
            sz += std::get<uint32_t>(one_struct_sz);
        }
        catch (const std::bad_variant_access& e)
        {
            sz += compute_total_struct_sz(
                std::get<struct_sz_s>(one_struct_sz).t.get());
        }
    }
    return sz;
}

std::unique_ptr<struct_sz_t>
get_llvm_struct_bitsize(
    const llvm::StructType* str_ty, const llvm::Module* llvm_module)
{
    auto struct_sz = std::make_unique<struct_sz_t>(str_ty->getNumElements());
    const llvm::Type* str_elem_ty;
    for (size_t i = 0; i < str_ty->getNumElements(); ++i)
    {
        str_elem_ty = str_ty->getElementType(i);
        if (str_elem_ty->isIntegerTy())
        {
            struct_sz->at(i) = llvm::dyn_cast<llvm::IntegerType>(str_elem_ty)
                                   ->getIntegerBitWidth();
        }
        else if (str_elem_ty->isPointerTy())
        {
            struct_sz->at(i) = llvm_module->getDataLayout().getPointerSize();
        }
        else if (str_elem_ty->isStructTy())
        {
            struct_sz->at(i) = struct_sz_s(get_llvm_struct_bitsize(
                llvm::dyn_cast<llvm::StructType>(str_elem_ty), llvm_module));
        }
        else
        {
            str_elem_ty->print(llvm::errs());
            throw std::runtime_error("Unhandled struct member type!");
        }
    }
    return struct_sz;
}
