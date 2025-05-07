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
    if (_val > (2 << _sz))
    {
        throw std::invalid_argument("Invalid value `" + std::to_string(val)
            + "` given for size `" + std::to_string(sz) + "`!");
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
    return IF_Arg_Int(1, sizeof(IF_Arg_Int::val_t), false);
}

/* Other Operations ***********************************************************/

IF_Arg
IF_Emulator::emulate_icmp(const IF_ArgList& args)
{
    // TODO
    const IF_ArgList& args2 = args;
    return IF_Arg_Int(1, sizeof(IF_Arg_Int::val_t), false);
}

/*******************************************************************************
 * Estimated operations */

/* Vector Operations **********************************************************/

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

    const llvm::VectorType* vt
        = llvm::dyn_cast<llvm::VectorType>(eei->getType());
    if (!vt)
    {
        throw std::runtime_error(
            "Could not cast type to expected `VectorType`!");
    }

    // TODO finish
    vt->print(llvm::errs());
    // return vt->ElementCount()
    return 1.0;
}

/* Aggregate Operations *******************************************************/

double
IF_Emulator::estimate_extract_value(const llvm::Instruction& instr)
{
    // TODO
    auto& instr2 = instr;
    return 1.0;
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
    const llvm::Type* ty_src = ti->getSrcTy();
    const llvm::Type* ty_dst = ti->getDestTy();
    if (ty_src->getIntegerBitWidth() <= ty_dst->getIntegerBitWidth())
    {
        throw std::logic_error("Found invalid `trunc` type bit-widths!");
    }

    // TODO double check
    return 1.0
        / (2 << (ty_src->getIntegerBitWidth() - ty_dst->getIntegerBitWidth()
               - 1));
}

/* Other Operations ***********************************************************/

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
