#ifndef _IF_READER_BASICBLOCK_HPP
#define _IF_READER_BASICBLOCK_HPP

#include "llvm/IR/Function.h"
#include "llvm/IR/PassManager.h"

class ReaderPass : public llvm::PassInfoMixin<ReaderPass>
{
public:
    llvm::PreservedAnalyses run(
        llvm::Function&, llvm::FunctionAnalysisManager&);
};

#endif // _IF_READER_BASICBLOCK_HPP
