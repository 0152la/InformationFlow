#include "instr_emulator.hpp"

// std::map<uint16_t, std::function<IF_Arg(const IF_ArgList&)>> emulated_fns {
//// Binary Operations
//{ llvm::Instruction::Add, IF_Emulator::emulate_add },
//{ llvm::Instruction::Sub, IF_Emulator::emulate_sub },
//{ llvm::Instruction::Mul, IF_Emulator::emulate_mul },
//{ llvm::Instruction::UDiv, IF_Emulator::emulate_udiv },
//{ llvm::Instruction::SDiv, IF_Emulator::emulate_sdiv },
//{ llvm::Instruction::URem, IF_Emulator::emulate_urem },
//{ llvm::Instruction::SRem, IF_Emulator::emulate_srem },

//// Bitwise Binary Operations
//{ llvm::Instruction::Shl, IF_Emulator::emulate_shl },
//{ llvm::Instruction::LShr, IF_Emulator::emulate_lshr },
//{ llvm::Instruction::AShr, IF_Emulator::emulate_ashr },
//{ llvm::Instruction::And, IF_Emulator::emulate_and },
//{ llvm::Instruction::Or, IF_Emulator::emulate_or },
//{ llvm::Instruction::Xor, IF_Emulator::emulate_xor },

//// Memory Operations
//{ llvm::Instruction::AtomicRMW, IF_Emulator::emulate_atomic_rmw },

//// Conversion Operations
//{ llvm::Instruction::FPToSI, IF_Emulator::emulate_fptosi },

//// Other Operations
//{ llvm::Instruction::ICmp, IF_Emulator::emulate_icmp },
//};

// Unary Operations
// std::map<uint16_t, std::function<double(double)>> emulated_fns_unary
//{
//{ llvm::Instruction::FNeg, llvm_impl_fneg },
//};
//
emulated_fns_t emulated_fns;

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
 * Initialisation functions
 ******************************************************************************/

void
IF_Emulator::dllink_snippet(const std::string& fn_name) const
{
    binops_i64_t fn
        = (binops_i64_ct) dlsym(this->ll_snippet_handler, fn_name.c_str());
    if (const char* err = dlerror())
    {
        // throw std::runtime_error("Error in `dlsym` for " + fn_name
        //+ "\n\t >> " + std::string { err } + "\n");
        std::cerr << "Ignoring unimplemented `" << fn_name << "`\n";
    }
    emulated_fns.insert(std::make_pair(fn_name, fn));
}

void
IF_Emulator::populate_all_bin_ops(void) const
{
    binops_i64_t fn;
    for (unsigned int llvm_op = llvm::Instruction::BinaryOpsBegin;
        llvm_op < llvm::Instruction::BinaryOpsEnd; ++llvm_op)
    {
        std::string fn_name = this->snippet_prefix
            + std::string(llvm::Instruction::getOpcodeName(llvm_op));
        this->dllink_snippet(fn_name);
    }
}

void
IF_Emulator::populate_all_other_ops(void) const
{
    binops_i64_t fn; // TODO
    for (unsigned int llvm_op = llvm::Instruction::OtherOpsBegin;
        llvm_op < llvm::Instruction::OtherOpsEnd; ++llvm_op)
    {
        const std::string fn_name = this->snippet_prefix
            + std::string { llvm::Instruction::getOpcodeName(llvm_op) };
        // TODO
        if (llvm_op == llvm::Instruction::ICmp)
        {
            for (unsigned int icmp_pred = llvm::CmpInst::FIRST_ICMP_PREDICATE;
                icmp_pred < llvm::CmpInst::LAST_ICMP_PREDICATE; ++icmp_pred)
            {
                this->dllink_snippet(fn_name + "_"
                    + llvm::ICmpInst::getPredicateName(
                        llvm::ICmpInst::Predicate(icmp_pred))
                        .str());
            }
        }
        else
        {
            this->dllink_snippet(fn_name);
        }
    }
}

const std::string
IF_Emulator::make_emulated_fn_name(const llvm::Instruction& instr) const
{
    std::string fn_name = this->snippet_prefix + instr.getOpcodeName();
    if (const llvm::ICmpInst* icmp_instr
        = llvm::dyn_cast<llvm::ICmpInst>(&instr))
    {
        fn_name += "_"
            + llvm::ICmpInst::getPredicateName(icmp_instr->getPredicate())
                  .str();
    }
    return fn_name;
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

    this->populate_all_bin_ops();
    this->populate_all_other_ops();
}

IF_Emulator::~IF_Emulator() { dlclose(this->ll_snippet_handler); }

binops_i64_t
IF_Emulator::get_emulated_fn(const llvm::Instruction& instr) const
{
    const std::string lookup_name = this->make_emulated_fn_name(instr);
    try
    {
        return emulated_fns.at(lookup_name);
    }
    catch (const std::out_of_range& e)
    {
        throw std::runtime_error(
            "Error looking up function for `" + lookup_name + "`!");
    }
}

/*******************************************************************************
 * Estimated operations
 */

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
