#include "reader-basicblock.hpp"

llvm::PreservedAnalyses
ReaderPass::run(llvm::Function& func, llvm::FunctionAnalysisManager& fam)
{
    llvm::errs() << func.getName() << "\n";
    return llvm::PreservedAnalyses::all();
}
