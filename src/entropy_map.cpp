#include "entropy_map.hpp"
#include <llvm/IR/Constant.h>
#include <llvm/IR/Instruction.h>
#include <numeric>
#include <sstream>
#include <stdexcept>

const std::string instr_simple_prefix = "i";

/*******************************************************************************
 * IF_EntropyMap::Instruction
 ******************************************************************************/

IF_EntropyMap::Instruction::Instruction(
    uint32_t _idx, const llvm::Instruction& _instr) :
    idx(_idx),
    opcode(_instr.getOpcode()),
    ret_ty_bit_sz(_instr.getType()->getPrimitiveSizeInBits().getFixedValue())
{
    for (const llvm::Use& u : _instr.operands())
    {
        llvm::Value* v = u.get();
        if (llvm::isa<llvm::Instruction>(v))
        {
            this->insts_in.insert(v);
        }
    }
};

void
IF_EntropyMap::Instruction::add_successor(
    const IF_EntropyMap::Instruction* to_add)
{
    this->succs_instr.insert(to_add);
}

void
IF_EntropyMap::Instruction::add_external_succ(std::string ex_fn)
{
    this->succs_extern.insert(ex_fn);
}

const std::string
IF_EntropyMap::Instruction::to_str(void) const
{
    std::ostringstream oss;
    oss << llvm::Instruction::getOpcodeName(this->get_opcode());
    oss << " -- Entropy " << this->get_retained_entropy() << '\n';
    return oss.str();
}

const std::string
IF_EntropyMap::Instruction::to_str_simple(void) const
{
    return instr_simple_prefix + std::to_string(this->get_idx());
}

/*******************************************************************************
 * IF_EntropyMap::Function
 ******************************************************************************/

const IF_EntropyMap::Instruction*
IF_EntropyMap::Function::get_first_instr() const
{
    return this->instrs.front().get();
}

const std::string
IF_EntropyMap::Function::set_demangled_name(const llvm::Function& _fn)
{
    const llvm::DISubprogram* dis = _fn.getSubprogram();
    if (!dis)
    {
        return std::string();
    }
    return dis->getName().str();
}

const std::string
IF_EntropyMap::Function::get_representing_name(void) const
{
    if (this->get_demangled_name().empty())
    {
        return this->get_name();
    }
    return this->get_demangled_name();
}

const std::string
IF_EntropyMap::Function::to_str(void) const
{
    std::ostringstream oss;
    oss << this->get_name();
    if (this->get_demangled_name().empty())
    {
        oss << "[ " << this->get_demangled_name() << " ]";
    }
    oss << '\n';
    for (const auto& instr : this->get_instrs())
    {
        oss << "\t" << instr->to_str();
    }
    return oss.str();
}

/*******************************************************************************
 * IF_EntropyMap::Map
 ******************************************************************************/

const IF_EntropyMap::Instruction*
IF_EntropyMap::Map::get_first_instr() const
{
    for (const auto& func : this->funcs)
    {
        if (!func->get_demangled_name().compare("main")
            || !func->get_name().compare("main"))
        {
            return func->get_first_instr();
        }
    }
    throw std::runtime_error("Couldn't find `main`!");
}

auto
IF_EntropyMap::Map::get_nontrivial_instruction_count(void) const
    -> decltype(this->instr_count)
{
    size_t i_count = 0;
    for (const auto& fn : this->funcs)
    {
        i_count
            += std::count_if(fn->get_instrs().begin(), fn->get_instrs().end(),
                [](const auto& inst) { return !inst->is_trivial(); });
    }
    return i_count;
}

void
IF_EntropyMap::Map::insert_external_func(std::string ex_fn)
{
    this->external_funcs.insert(ex_fn);
}

std::tuple<size_t, size_t, size_t>
IF_EntropyMap::Map::compute_cyclomatic_complexity(void) const
{
    std::vector<bool> seen_idxs(this->instr_count, false);
    seen_idxs.at(this->get_first_instr()->get_idx()) = true;

    std::queue<const Instruction*> to_follow;
    to_follow.push(this->get_first_instr());

    const Instruction* curr_inst;
    size_t edges = 0;
    size_t nodes = 0;

    while (!to_follow.empty())
    {
        curr_inst = to_follow.front();
        to_follow.pop();
        seen_idxs.at(curr_inst->get_idx()) = true;
        nodes += 1;

        for (IF_EntropyMap::Instruction::succs_t::const_reference succ :
            curr_inst->get_succs_inst())
        {
            edges += 1;
            if (!seen_idxs.at(succ->get_idx()))
            {
                to_follow.push(succ);
            }
        }
    }

    return std::tuple(edges, nodes, edges - nodes + 2);
}

const std::string
IF_EntropyMap::Map::to_str(void) const
{
    std::ostringstream oss;
    for (const auto& em_fn : this->get_funcs())
    {
        oss << em_fn->to_str();
    }
    return oss.str();
}

void
IF_EntropyMap::Map::print(void) const
{
    for (const auto& em_fn : this->get_funcs())
    {
        std::cout << "Fn " << em_fn->get_name();
        if (!em_fn->get_demangled_name().empty())
        {
            std::cout << " -- " << em_fn->get_demangled_name();
        }
        std::cout << '\n';

        for (const auto& em_fn_instr : em_fn->get_instrs())
        {
            if (this->verbose || !em_fn_instr->is_trivial())
            {
                std::cout << "\t"
                          << llvm::Instruction::getOpcodeName(
                                 em_fn_instr->get_opcode());
                std::cout << " -- BS " << em_fn_instr->ret_ty_bit_sz;
                std::cout << " -- UC " << em_fn_instr->get_retained_entropy()
                          << '\n';
                // TODO arguments
            }
        }
    }
}

/*******************************************************************************
 * IF_EntropyMap::UseMap
 ******************************************************************************/

/* Node ***********************************************************************/

IF_EntropyMap::UseMap::Node::Node(const Instruction* _inst) :
    inst(_inst) { };

void
IF_EntropyMap::UseMap::Node::add_pred(
    const IF_EntropyMap::Instruction* _new_pred)
{
    this->preds.push_back(_new_pred);
}

/* Path ***********************************************************************/

IF_EntropyMap::UseMap::Path::Path(const IF_EntropyMap::UseMap::Node* _node)
{
    this->start_node = _node;
}

//void
//IF_EntropyMap::UseMap::Path::combine(IF_EntropyMap::UseMap::Path* _other,
    //const IF_EntropyMap::Instruction* _inst)
//{
    //auto insert_node = std::find_if(this->sequence.begin(),
        //this->sequence.end(), [&_inst](decltype(this->sequence)::value_type n)
        //{ return n.inst == _inst; });
    //if (insert_node == this->sequence.end())
    //{
        //throw std::runtime_error(fmt::format(
            //"Couldn't find node for combination for `{}`!", _inst->to_str()));
    //}

    //this->sequence.insert(
        //insert_node, _other->sequence.begin(), _other->sequence.end());
//}

//void
//IF_EntropyMap::UseMap::Path::add_pred(const IF_EntropyMap::Instruction* _inst,
    //const IF_EntropyMap::Instruction* _new_pred)
//{
    //for (auto n : this->sequence)
    //{
        //if (n.inst == _inst)
        //{
            //n.add_pred(_new_pred);
            //return;
        //}
    //}
    //throw std::runtime_error(fmt::format(
        //"Couldn't find node for instruction `{}`!", _inst->to_str()));
//}

IF_EntropyMap::UseMap::Path::path_ucs_t
IF_EntropyMap::UseMap::Path::compute_unc_coef(void)
{
    auto ucs = IF_EntropyMap::UseMap::Path::path_ucs_t { };
    auto curr_uc_path = IF_EntropyMap::UseMap::Path::path_unc_coef_t { "", 1.0 };

    auto curr_node = this->sequence.front();
    while (true)
    {
        curr_uc_path.first += std::to_string(curr_node.inst->get_idx()) + '-';
        curr_uc_path.second *= curr_node.inst->get_retained_entropy();

        if (curr_node.preds.empty())
        {
            break;
        }
        curr_node = curr_node.preds.front(); // TODO
    }

    ucs.push_back(curr_uc_path);
    return ucs;
}

std::string
IF_EntropyMap::UseMap::Path::to_str(const IF_EntropyMap::UseMap::Path::path_ucs_t& ucs)
{
    std::ostringstream res;
    for (const auto& p : ucs)
    {
        res << fmt::format("PATH {} -- UC {}\n", p.first, p.second);
    }
    return res.str();
}

/* UseMap *********************************************************************/

IF_EntropyMap::UseMap::UseMap(const IF_EntropyMap::insts_t& _insts,
    const IF_EntropyMap::insts_llvm_mapping_t& _llvm_mapping)
{
    auto seen_insts = std::vector<bool>(_insts.size(), false);
    auto inst_to_path_map
        = std::unordered_map<const IF_EntropyMap::Instruction*,
            IF_EntropyMap::UseMap::Path*> { };
    auto use_path = IF_EntropyMap::UseMap::Path(
    for (const auto& inst : _insts)
    {
        if (seen_insts[inst->get_idx()])
        {
            continue;
        }

        seen_insts.at(inst->get_idx()) = true;
        auto curr_path = [&]() -> IF_EntropyMap::UseMap::Path*
        {
            if (auto path_it = inst_to_path_map.find(inst);
                path_it != inst_to_path_map.end())
            {
                return path_it->second;
            }
            else
            {
                return new IF_EntropyMap::UseMap::Path(inst);
            }
        }();

        inst_to_path_map.insert(std::make_pair(inst, curr_path));
        paths.insert(curr_path);

        for (const auto& pred_inst : inst->get_reg_uses())
        {
            auto pred = _llvm_mapping.at(
                llvm::dyn_cast<llvm::Instruction>(pred_inst));
            if (seen_insts[pred->get_idx()])
            {
                auto other_path = inst_to_path_map.at(pred);
                curr_path->combine(other_path, inst);
                paths.erase(other_path);
                delete other_path;
            }
            curr_path->add_pred(inst, pred);
            inst_to_path_map[pred] = curr_path;
            seen_insts.at(pred->get_idx()) = true;
        }
    }

    // TODO
    for (const auto& p : paths)
    {
        auto p_ucs = p->compute_unc_coef();
        p->to_str(p_ucs);
    }

    this->use_path = use_path;
}
