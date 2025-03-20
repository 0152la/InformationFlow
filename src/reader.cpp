#include "reader.hpp"

int main(int argc, char* argv[])
{
    const char* ll_file = "/home/andreilascu/Documents/Repos/InformationFlow/build/tests/sample.ll";

    llvm::LLVMContext llvm_ctx;
    llvm::SMDiagnostic llvm_smd;
    std::unique_ptr<llvm::Module> ir_module(llvm::parseIRFile(ll_file, llvm_smd, llvm_ctx));

    if (!ir_module)
    {
        llvm_smd.print(argv[0], llvm::errs());
        return 1;
    }



    return 0;
}
