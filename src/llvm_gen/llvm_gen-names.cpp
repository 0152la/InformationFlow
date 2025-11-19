#include "llvm_gen-names.hpp"

const std::string
make_llvm_snippet_name(const std::string& base, const std::string& extra)
{
    return config::llvm_snippet_prefix + config::llvm_snippet_delim + base
        + (extra.empty() ? "" : (config::llvm_snippet_delim + extra));
}

const std::string
make_llvm_snippet_name(const std::string& base)
{
    return make_llvm_snippet_name(base, "");
}

const std::string
make_llvm_snippet_name(const llvm::Instruction& inst)
{
    unsigned int opcode = inst.getOpcode();
    if (const llvm::CmpInst* cmp_inst = llvm::dyn_cast<llvm::CmpInst>(&inst))
    {
        make_llvm_snippet_name(inst.getOpcodeName(),
            llvm::CmpInst::getPredicateName(cmp_inst->getPredicate()).str());
    }
    return make_llvm_snippet_name(inst.getOpcodeName());
}
