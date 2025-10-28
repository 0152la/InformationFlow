#include "config.hpp"

#include <string>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wall"
#pragma clang diagnostic ignored "-Wunused-parameter"
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#pragma clang diagnostic pop

const std::string
make_llvm_snippet_name(const std::string&, const std::string&);
const std::string
make_llvm_snippet_name(const std::string&);

const std::string
make_llvm_snippet_name(const llvm::Instruction&);
