#include "instr_emulator.hpp"

/* For certain instructions, we can directly calculate the entropy, as they
 * either affect only control flow, or move data around.
 */
set_entropy_t set_entropy {
    /* This seems like a `Call` instruction in most cases, but consider
       edge cases */
    { llvm::Instruction::Invoke, 1.0 },
    { llvm::Instruction::Resume, 1.0 },
    { llvm::Instruction::Unreachable, 0.0 },
    { llvm::Instruction::CleanupRet, 1.0 },
    { llvm::Instruction::CatchRet, 1.0 },
    { llvm::Instruction::CallBr, 1.0 },

    // Unary Instructions
    /* The entropy here is actually off by one sample, due to two's
       complement and negating the maximum representable value; however, we
       estimate it to 1.0 */
    { llvm::Instruction::FNeg, 1.0 },

    // Binary Instructions
    { llvm::Instruction::Add, 0.5 },
    { llvm::Instruction::Sub, 0.5 },
    { llvm::Instruction::And, 0.405639 },
    { llvm::Instruction::Or, 0.405639 },
    { llvm::Instruction::Xor, 0.5 },

    // Vector Operations
    { llvm::Instruction::InsertElement, 1.0 },
    { llvm::Instruction::ShuffleVector, 0.5 },

    // Aggregate Operations
    { llvm::Instruction::InsertValue, 1.0 },

    // Memory Operations
    { llvm::Instruction::Alloca, 1.0 },
    { llvm::Instruction::Load, 1.0 },
    { llvm::Instruction::Store, 1.0 },
    { llvm::Instruction::Fence, 1.0 },
    { llvm::Instruction::AtomicCmpXchg, 1.0 },
    /* Only computes an address, doesn't dereference any memory */
    { llvm::Instruction::GetElementPtr, 1.0 },

    // Conversion Operations
    { llvm::Instruction::ZExt, 1.0 },
    { llvm::Instruction::SExt, 1.0 },
    { llvm::Instruction::FPExt, 1.0 },
    /* TODO might be fine to approximate
       to 1, but I think there is quite a bit of lost entropy from rounding
       */
    // { llvm::Instruction::fptoui, 1.0
    // },
    { llvm::Instruction::UIToFP, 1.0 },
    { llvm::Instruction::SIToFP, 1.0 },
    { llvm::Instruction::IntToPtr, 1.0 },
    { llvm::Instruction::BitCast, 1.0 },
    { llvm::Instruction::AddrSpaceCast, 1.0 },

    // Other Operations
    // TODO handle constant case?
    { llvm::Instruction::Select, 1.0 },
    { llvm::Instruction::LandingPad, 1.0 },
    /* TODO */
    { llvm::Instruction::Freeze, 1.0 },
};

/* For certain instructions, we can estimate the amount of retained entropy by
 * looking at just the data contained within the instructions themselves, rather
 * than having to perform fuzzing
 */
estimate_entropy_t estimate_entropy {

    // Vector Operations
    { llvm::Instruction::ExtractElement,
        IF_Emulator::estimate_extract_element },

    // Aggregate Operations
    { llvm::Instruction::ExtractValue, IF_Emulator::estimate_extract_value },

    // Converstion Operations
    { llvm::Instruction::Trunc, IF_Emulator::estimate_trunc },
    { llvm::Instruction::PtrToInt, IF_Emulator::estimate_ptrtoint },

    // Other Operations
    //{ IF_Emulator::complete_fn_name("icmp_eq"), IF_Emulator::estimate_icmp_eq
    //},
    //{ IF_Emulator::complete_fn_name("icmp_ne"), IF_Emulator::estimate_icmp_ne
    //},
    { llvm::Instruction::PHI, IF_Emulator::estimate_phi },
};

flow_set_t flow_set_vals {
    // Terminator Instructions
    { llvm::Instruction::Ret, 1.0 },

    // Other Operations
    { llvm::Instruction::Call, 1.0 },
};

flow_estimate_t flow_estimate_fns {
    // Terminator Operations
    { llvm::Instruction::Br, IF_Emulator::estimate_flow_br },
    { llvm::Instruction::Switch, IF_Emulator::estimate_flow_switch },
    { llvm::Instruction::IndirectBr, IF_Emulator::estimate_flow_indirectbr },
};

/*******************************************************************************
 * `fn_def` functions
 ******************************************************************************/

fn_def::fn_def(const std::string def_info)
{
    /* Regex voodoo
     * (\\d+) - opcode
     * , - literal comma
     * (\\w*) - extra info
     * , - literal comma
     * (\\w+) - function name
     * \\((\\w+)\\) - return type
     * \\[([\\w,]*)\\] - parameter types
     */
    const std::regex def_re(
        "(\\d+),(\\w*),(\\w+)(\\((\\w+)\\)\\[([\\w,]*)\\])");
    std::smatch def_re_m;

    std::regex_match(def_info, def_re_m, def_re);

    this->op_code = std::stoi(def_re_m[1]);
    this->extra_info = def_re_m[2];
    this->fn_name = def_re_m[3];
    this->ret_sig = def_re_m[4];
    this->ret_ty = def_re_m[5];

    size_t from_pos = 0;
    size_t to_pos = 0;
    const std::string param_base = def_re_m[6].str();
    do
    {
        to_pos = param_base.find(',', from_pos);
        this->params.push_back(param_base.substr(from_pos, to_pos - from_pos));
        from_pos = to_pos + 1;
    } while (to_pos != std::string::npos);
}

const std::string
fn_def::to_str(void)
{
    std::ostringstream ss;
    ss << this->op_code << " - ";
    ss << this->extra_info << " - ";
    ss << this->fn_name;
    ss << " (" << this->ret_ty << ") ";
    ss << " [";
    if (!params.empty())
    {
        ss << std::accumulate(std::next(this->params.begin()),
            this->params.end(), this->params.front(),
            [](std::string s, const std::string p) { return s + ", " + p; });
    }
    ss << "] ";
    ss << " -- " << this->ret_sig;
    return ss.str();
}

/*******************************************************************************
 * Initialisation functions
 ******************************************************************************/

void
IF_Emulator::populate_ops(void)
{
    // TODO get the entropy values
}

IF_Emulator::IF_Emulator(const std::string& lib_path)
{
    this->ll_snippet_handler = dlopen(lib_path.c_str(), RTLD_NOW);
    if (!this->ll_snippet_handler)
    {
        throw std::runtime_error(
            "Error `dlopen` :: " + std::string { dlerror() });
    }

    // Clear `dlerror`
    dlerror();

    this->populate_ops();
}

IF_Emulator::~IF_Emulator() { dlclose(this->ll_snippet_handler); }

std::string
IF_Emulator::complete_fn_name(const std::string& base)
{
    return IF_Emulator::snippet_prefix + base;
}

std::string
IF_Emulator::make_emulated_fn_name(const llvm::Instruction& inst)
{
    std::string fn_name = IF_Emulator::snippet_prefix + inst.getOpcodeName();
    if (const llvm::CmpInst* cmp_inst = llvm::dyn_cast<llvm::CmpInst>(&inst))
    {
        fn_name += "_"
            + llvm::CmpInst::getPredicateName(cmp_inst->getPredicate()).str();
    }
    return fn_name;
}

/*******************************************************************************
 * Entropy estimation functions
 */

/* Terminator Operations ******************************************************/

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
    //return 1.0 / (1 << (from_sz - to_sz));
    return to_sz / from_sz;
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

double
IF_Emulator::estimate_icmp_eq(const llvm::Instruction& instr)
{
    uint8_t bit_width = get_operand_bit_width(
        instr.getOperand(0)->getType(), instr.getModule());
    if (bit_width < 1 || bit_width > 64)
    {
        std::ostringstream err;
        err << "Invalid `icmp` bit width of " << bit_width << "!";
        throw std::runtime_error(err.str());
    }
    return 1.0 / std::pow(2, bit_width);
}

double
IF_Emulator::estimate_icmp_ne(const llvm::Instruction& instr)
{
    // Both of these comparisons have a single value which will be different
    // than the rest, so we can use the same entropy computation process
    return estimate_icmp_eq(instr);
}

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
 * Flow estimation functions
 *
 * These functions estimate how much "flow", as a proportion of entropy, is to
 * be allocated to all possible successor instructions of a given instruction.
 * These functions require a bit of further analysis in order to compute the
 * flow proportion
 * ******************************************************************************/

/* There are two cases: whether this is a conditional, or unconditional branch.
 * If it is the latter, then the flow is simply 1.0, as it's essentially a
 * `jmp` instruction. If it is the former, the flow s equivalent to the
 * computed flow value of the variable conditioned on.
 */
double
IF_Emulator::estimate_flow_br(const llvm::Instruction& inst)
{
    const llvm::BranchInst* bi = llvm::dyn_cast<llvm::BranchInst>(&inst);
    if (bi->isUnconditional())
    {
        return 1.0;
    }
    // TODO return ConditionalsMap::getValue(bi->getCondition());
    std::exit(1);
}

/* The entropy loss is simply 1 / (<number_of_choices> + 1 default choice)
 *
 * TODO double check
 */
double
IF_Emulator::estimate_flow_switch(const llvm::Instruction& instr)
{
    return 1.0 / (llvm::dyn_cast<llvm::SwitchInst>(&instr)->getNumCases() + 1);
}

/* The flow is determined by the proportion of possible valid address values in
 * the `<address>` field. Since this requires heavy dynamic analysis, we
 * estimate it to be uniformly distributed, i.e. the flow is 1 /
 * <destination_count>
 */
double
IF_Emulator::estimate_flow_indirectbr(const llvm::Instruction& inst)
{
    return 1.0
        / llvm::dyn_cast<llvm::IndirectBrInst>(&inst)->getNumDestinations();
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
    {
        struct_sz = std::get<struct_sz_s>(struct_sz->at(*idx_it)).t.get();
        idx_it += 1;
    }
    if (std::holds_alternative<struct_sz_width_t>(struct_sz->at(*idx_it)))
    {
        return std::get<struct_sz_width_t>(struct_sz->at(*idx_it));
    }
    return compute_total_struct_sz(
        std::get<struct_sz_s>(struct_sz->at(*idx_it)).t.get());
}

uint32_t
compute_total_struct_sz(const struct_sz_t* struct_sz)
{
    uint32_t sz = 0;
    for (const auto& one_struct_sz : *struct_sz)
    {
        if (std::holds_alternative<struct_sz_width_t>(one_struct_sz))
        {
            sz += std::get<struct_sz_width_t>(one_struct_sz);
        }
        else
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
            struct_sz->at(i)
                = llvm_module->getDataLayout().getPointerSize() * CHAR_BIT;
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

uint8_t
get_operand_bit_width(const llvm::Type* ty, const llvm::Module* llvm_module)
{
    if (ty->isPointerTy())
    {
        return llvm_module->getDataLayout().getPointerSize() * CHAR_BIT;
    }

    if (ty->isIntegerTy())
    {
        return ty->getIntegerBitWidth();
    }

    std::string err;
    llvm::raw_string_ostream err_os(err);
    err_os << "Unhandled type for bit_width computation : ";
    ty->print(err_os);
    throw std::runtime_error(err);
}
