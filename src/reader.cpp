#include "reader.hpp"

int
main(int argc, char* argv[])
{
    const char* ll_file = "/home/andreilascu/Documents/Repos/InformationFlow/"
                          "build/tests/sample.ll";

    llvm::LLVMContext llvm_ctx;
    llvm::SMDiagnostic llvm_smd;
    std::unique_ptr<llvm::Module> ir_module(
        llvm::parseIRFile(ll_file, llvm_smd, llvm_ctx));

    if (!ir_module)
    {
        llvm_smd.print(argv[0], llvm::errs());
        return 1;
    }

    for (const auto& fn : ir_module->getFunctionList())
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

    return 0;
}
