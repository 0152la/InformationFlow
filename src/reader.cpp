#include "reader.hpp"
#include "llvm/IR/Instruction.h"
#include "llvm/Support/Casting.h"
#include <memory>
#include <sstream>
#include <stdexcept>

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
            auto em_instr = std::make_unique<IF_EntropyMap_Instr>(fn_inst);
            if (fn_inst.isUnaryOp())
            {
                if (fn_inst.getOpcode() != llvm::Instruction::FNeg)
                {
                    throw std::invalid_argument(
                        "Unhandled unary instruction that is not `fneg`");
                }

                // Negating a value is a one-to-one computation, so no entropy
                // is lost
                // TODO check two's complement extreme value - we should lose
                // one value here
                // TODO check for constants?
                em_instr->set_lost_entropy(1.0);
            }
            else if (fn_inst.isBinaryOp())
            {

                if (fn_inst.getOpcode() == llvm::Instruction::Add)
                {
                    IF_FuzzEngine if_fe;
                    double entropy = if_fe.get_fuzzed_entropy(fn_inst);
                    std::cout << "IM ENTROPY " << entropy << '\n';
                }
                else
                {
                    // TODO add other cases, turns this into an exception
                    em_instr->set_lost_entropy(1.0);
                    // throw std::runtime_error(
                    //"Unhandled binary instruction opcode "
                    //+ fn_inst.getOpcode());
                }
            }
            // TODO do these have some entropy loss?
            // TODO probably wanna handle CallBase
            // else if (llvm::isa<llvm::CallInst>(fn_inst))
            else if (llvm::isa<llvm::CallBase>(&fn_inst))
            {
                llvm::StringRef fn_call_name
                    = llvm::dyn_cast<llvm::CallBase>(&fn_inst)
                          ->getCalledFunction()
                          ->getName();
                llvm::errs() << "\t Call " << fn_inst.getOpcodeName() << " ";
                llvm::errs() << fn_call_name << '\n';
                names_call_map.at(em_fn.get()).push_back(fn_call_name.str());
            }
            em_fn->insert(std::move(em_instr));
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

/*******************************************************************************
 * IF_EntropyMap_Instr
 ******************************************************************************/

const std::string
IF_EntropyMap_Instr::to_str(void) const
{
    std::ostringstream oss;
    oss << llvm::Instruction::getOpcodeName(this->get_opcode());
    oss << " -- Entropy " << this->get_lost_entropy() << '\n';
    return oss.str();
}

/*******************************************************************************
 * IF_EntropyMap_Func
 ******************************************************************************/

// bool
// IF_EntropyMap_Func::operator<(const IF_EntropyMap_Func& other) const
//{
// return this->get_name() < other.get_name();
//}

void
IF_EntropyMap_Func::insert_call(const IF_EntropyMap_Func* em_fn)
{
    this->callees.push_back(em_fn);
}

const std::string
IF_EntropyMap_Func::to_str(void) const
{
    std::ostringstream oss;
    oss << this->get_name() << '\n';
    for (const auto& instr : this->get_instrs())
    {
        oss << "\t" << instr->to_str();
    }
    return oss.str();
}

/*******************************************************************************
 * IF_EntropyMap
 ******************************************************************************/

const std::string
IF_EntropyMap::to_str(void) const
{
    std::ostringstream oss;
    for (const auto& em_fn : this->get_funcs())
    {
        oss << em_fn->to_str();
    }
    return oss.str();
}
