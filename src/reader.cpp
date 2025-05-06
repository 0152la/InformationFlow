#include "reader.hpp"
#include "llvm/IR/Instruction.h"
#include "llvm/Support/Casting.h"
#include <algorithm>
#include <memory>
#include <sstream>
#include <stdexcept>

std::map<uint16_t, double> set_entropy {
    // Terminator Instructions
    { llvm::Instruction::Ret, 1.0 }, { llvm::Instruction::Br, 1.0 },
    //{ llvm::Instruction::Switch, }, // TODO
    { llvm::Instruction::IndirectBr, 1.0 },
    { llvm::Instruction::Invoke, 1.0 }, /* This seems like a `Call` instruction
                                           in most cases, but consider edge
                                           cases
                                           */
    { llvm::Instruction::CallBr, 1.0 }, { llvm::Instruction::Resume, 1.0 },
    //{ llvm::Instruction::CatchSwitch, }, // TODO
    { llvm::Instruction::CatchRet, 1.0 },
    { llvm::Instruction::CleanupRet, 1.0 },
    { llvm::Instruction::Unreachable, 0.0 },

    // Unary Instructions
    { llvm::Instruction::FNeg, 1.0 }, /* The entropy here is actually off by
                                         one sample, due to two's complement
                                         and negating the maximum representable
                                         value; however, we estimate it to 1.0
                                         */
    // Vector Operations
    { llvm::Instruction::InsertElement, 1.0 },
    { llvm::Instruction::ShuffleVector, 0.5 },

    // Aggregate Operations
    //{ llvm::Instruction::ExtractValue, 1.0 },
    { llvm::Instruction::InsertValue, 1.0 },

    // Memory Operations
    { llvm::Instruction::Alloca, 1.0 },
    { llvm::Instruction::Load, 1.0 }, /* TODO does the value overwritten lose
                                         information?
                                         */
    { llvm::Instruction::Store, 1.0 }, { llvm::Instruction::Fence, 1.0 },
    { llvm::Instruction::AtomicCmpXchg, 1.0 }, // TODO same as Load
    { llvm::Instruction::GetElementPtr, 1.0 }, // TODO same as load

    // Conversion Operations
    //{ llvm::Instruction::Trunc, 1.0 },
    { llvm::Instruction::ZExt, 1.0 }, { llvm::Instruction::SExt, 1.0 },
    //{ llvm::Instruction::FPTrunc, 1.0 },
    { llvm::Instruction::FPExt, 1.0 },

    //{ llvm::Instruction::FPToUI, 1.0 }, // TODO might be fine to approximate
    // to 1, but I think there is quite a bit of lost entropy from rounding

    //{ llvm::Instruction::FPToSI, 1.0 }, // TODO same as above
    { llvm::Instruction::UIToFP, 1.0 }, // TODO any unrepresentable integer?
    { llvm::Instruction::SIToFP, 1.0 }, // TODO same as above
    //{ llvm::Instruction::PtrToInt, 1.0 }, // TODO could truncate
    //{ llvm::Instruction::IntToPtr, 1.0 }, // TODO same as above
    { llvm::Instruction::BitCast, 1.0 },
    { llvm::Instruction::AddrSpaceCast, 1.0 },

    // Other Operations
    { llvm::Instruction::Call, 1.0 },
    { llvm::Instruction::Select, 0.5 }, // TODO handle constant case (for all)
    //{ llvm::Instruction::LandingPad, 1.0 },
    //{ llvm::Instruction::Freeze, 1.0 }, // XXX

};

extern std::map<uint16_t, std::function<double(const llvm::Instruction&)>>
    estimate_entropy;

/*******************************************************************************
 * IF_Parser
 ******************************************************************************/

IF_Parser::IF_Parser()
{
    this->data = IF_Histogram<if_in_t, if_out_t>();
    this->in_gen = IF_Randgen();
}

IF_Parser::IF_Parser(int seed)
{
    this->data = IF_Histogram<if_in_t, if_out_t>();
    this->in_gen = IF_Randgen(seed);
}

std::unique_ptr<IF_EntropyMap>
IF_Parser::make_entropy_map(const llvm::Module& llvm_module)
{
    uint32_t instr_idx = 0;
    auto em = std::make_unique<IF_EntropyMap>(llvm_module);
    std::map<IF_EntropyMap_Func*, std::vector<std::string>> names_call_map;
    // Iterate over functions ...
    for (const auto& fn : llvm_module.getFunctionList())
    {
        auto em_fn = std::make_unique<IF_EntropyMap_Func>(fn);
        names_call_map.emplace(em_fn.get(), std::vector<std::string>());

        // ... and instructions
        for (const auto& fn_inst : llvm::instructions(fn))
        {
            auto em_instr
                = std::make_unique<IF_EntropyMap_Instr>(instr_idx, fn_inst);
            double retained_entropy;

            // We first search whether this instruction is one we have a
            // default entropy for. If we do, then we simply set the entropy to
            // that value.
            if (auto fn_set_entropy = set_entropy.find(fn_inst.getOpcode());
                fn_set_entropy != set_entropy.end())
            {
                retained_entropy = fn_set_entropy->second;
            }
            // Then we check whether we can estimate the entropy, without having
            // to fuzz
            else if (auto fn_est_entropy
                = estimate_entropy.find(fn_inst.getOpcode());
                fn_est_entropy != estimate_entropy.end())
            {
                retained_entropy = (fn_est_entropy->second)(fn_inst);
            }
            // Otherwise, we perform fuzzing, via emulated instructions.
            else
            {
                IF_FuzzEngine if_fe;
                retained_entropy = if_fe.fuzz_retained_entropy(fn_inst);
            }

            // Record `call` instructions in a function, so we can draw a
            // callgraph
            // TODO maybe just handle this with instruction successors?
            if (llvm::isa<llvm::CallBase>(&fn_inst))
            {
                llvm::StringRef fn_call_name
                    = llvm::dyn_cast<llvm::CallBase>(&fn_inst)
                          ->getCalledFunction()
                          ->getName();
                names_call_map.at(em_fn.get()).push_back(fn_call_name.str());
            }
            em_instr->set_retained_entropy(retained_entropy);
            em_fn->insert(std::move(em_instr));
            instr_idx += 1;
        }
        em->insert(std::move(em_fn));
    }

    // Resolve call names to `IF_EntropyMap_Func`s
    for (auto& [fn, calls] : names_call_map)
    {
        for (const std::string& call_name : calls)
        {
            auto called_fn = std::find_if(names_call_map.begin(),
                names_call_map.end(), [&call_name](const auto& fn)
                { return fn.first->get_name() == call_name; });
            if (called_fn == names_call_map.end())
            {
                throw std::logic_error(
                    "Did not find `IF_EntropyMap_Func` entry for function `"
                    + call_name + "`!");
            }
            fn->insert_call((*called_fn).first);
        }
    }

    return em;
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
