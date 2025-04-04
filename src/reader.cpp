#include "reader.hpp"

/*******************************************************************************
 * IF_Parser
 ******************************************************************************/

IF_Parser::IF_Parser()
{
    this->data = IF_Histogram();
    this->in_gen = IF_Randgen();
}

IF_Parser::IF_Parser(int seed)
{
    this->data = IF_Histogram();
    this->in_gen = IF_Randgen(seed);
}

void
IF_Parser::make_entropy_map(const llvm::Module& llvm_module)
{
    // Iterate over functions ...
    for (const auto& fn : llvm_module.getFunctionList())
    {
        // ... and instructions
        for (const auto& fn_inst : llvm::instructions(fn))
        {
            llvm::errs() << "\t Instruction " << fn_inst.getOpcodeName() << " ";
            for (const auto& fn_inst_arg : fn_inst.operand_values())
            {
                if (fn_inst_arg->hasName())
                {
                    llvm::errs() << fn_inst_arg->getName();
                }
                else
                {
                    llvm::errs() << fn_inst_arg;
                }
                llvm::errs() << " ";
            }
            llvm::errs() << '\n';
        }
    }
}

std::unique_ptr<IF_LLVM_Module>
IF_Parser::parse_ll(const std::string& ll_path)
{
    llvm::LLVMContext* llvm_ctx = new llvm::LLVMContext();
    llvm::SMDiagnostic* llvm_smd = new llvm::SMDiagnostic();
    std::unique_ptr<llvm::Module> ir_module
        = llvm::parseIRFile(ll_path, *llvm_smd, *llvm_ctx);

    if (!ir_module)
    {
        llvm_smd->print("iflow", llvm::errs());
        throw std::runtime_error("Could not parse given path " + ll_path);
    }

    return std::make_unique<IF_LLVM_Module>(
        llvm_ctx, llvm_smd, std::move(ir_module));
}

void
IF_Parser::print_instrs(const llvm::Module& llvm_module)
{
    for (const auto& fn : llvm_module.getFunctionList())
    {
        llvm::errs() << "Function " << fn.getName() << " -- "
                     << fn.getValueName() << '\n';

        for (const auto& fn_arg : fn.args())
        {
            llvm::errs() << "\t Arg " << fn_arg.getArgNo() << " : "
                         << fn_arg.getName() << " : " << fn_arg.getType()
                         << '\n';
        }

        llvm::errs() << "\t======\n";

        for (const auto& fn_inst : llvm::instructions(fn))
        {
            llvm::errs() << "\t Instruction " << fn_inst.getOpcodeName() << " ";
            for (const auto& fn_inst_arg : fn_inst.operand_values())
            {
                if (const llvm::Value* v
                    = llvm::dyn_cast<llvm::Value>(fn_inst_arg))
                {
                    llvm::errs() << "[ ";
                    // v->print(llvm::errs(), true);
                    // llvm::errs() << v->getNameOrAsOperand() << '\n';
                    v->printAsOperand(llvm::errs());
                    llvm::errs() << " --- ";
                    v->printAsOperand(llvm::errs(), false, &llvm_module);
                    llvm::errs() << " ]";
                }
                else
                {
                    throw std::runtime_error("Got non-Value argument");
                }
                // This is a function (TODO check)
                if (fn_inst_arg->hasName())
                {
                    llvm::errs() << fn_inst_arg->getName();
                }
                // This is a constant
                else if (const llvm::ConstantData* cd
                    = llvm::dyn_cast<llvm::ConstantData>(fn_inst_arg))
                {
                    if (const llvm::ConstantInt* ci
                        = llvm::dyn_cast<llvm::ConstantInt>(fn_inst_arg))
                    {
                        ci->getValue().print(llvm::errs(), true);
                    }
                    else if (const llvm::ConstantFP* cfp
                        = llvm::dyn_cast<llvm::ConstantFP>(fn_inst_arg))
                    {
                        cfp->getValue().print(llvm::errs());
                    }
                    else
                    {
                        throw std::runtime_error("Unhandled ConstantData type");
                    }
                }
                else
                {
                    llvm::errs() << fn_inst_arg;
                }
                llvm::errs() << " ";
            }
            llvm::errs() << '\n';
        }
    }
}
