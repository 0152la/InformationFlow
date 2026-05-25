#include "entropy_map.hpp"
#include <llvm/IR/Constant.h>
#include <llvm/IR/Instruction.h>
#include <memory>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

const std::string instr_simple_prefix = "i";

/*******************************************************************************
 * IF_EntropyMap::Instruction
 ******************************************************************************/

IF_EntropyMap::Instruction::Instruction(
    uint32_t _idx, const llvm::Instruction& _inst) :
    idx(_idx),
    opcode(_inst.getOpcode()),
    ret_ty_bit_sz(_inst.getType()->getPrimitiveSizeInBits().getFixedValue()),
    llvm_no_uses(_inst.use_empty()),
    llvm_is_terminator(_inst.isTerminator()) { };

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
    em_inst(_inst) { };

double
IF_EntropyMap::UseMap::Node::get_unc_coef(void) const
{
    return this->em_inst->get_retained_entropy();
}

auto
IF_EntropyMap::UseMap::Node::get_idx(void) const
    -> IF_EntropyMap::Instruction::idx_t
{
    return this->em_inst->get_idx();
}

std::string
IF_EntropyMap::UseMap::Node::to_str_path(uint32_t _indent) const
{
    const auto indent_str = std::string(_indent, '-');
    std::ostringstream oss;
    oss << fmt::format("{}NODE {}{}\n", indent_str, this->get_idx(),
        this->em_inst->llvm_is_terminator ? " TERM" : "");
    for (const auto& user : this->users)
    {
        oss << user->to_str_path(_indent + 1);
    }
    return oss.str();
}

/* UC_Path ********************************************************************/

IF_EntropyMap::UseMap::UC_Path::UC_Path(
    const IF_EntropyMap::UseMap::Node* _node) :
    last_node(_node),
    node_path(std::to_string(_node->get_idx())),
    unc_coef(1.0) { };

std::string
IF_EntropyMap::UseMap::UC_Path::to_str(void) const
{
    return fmt::format("PATH {} -- UC {}", this->node_path, this->unc_coef);
}

/* UseMap *********************************************************************/

IF_EntropyMap::UseMap::UseMap(
    const IF_EntropyMap::UseMap::insts_pair_t& _em_insts,
    const IF_EntropyMap::UseMap::llvm_to_insts_map_t& _llvm_insts_map)
{
    auto seen_insts = std::vector<bool>(_em_insts.size(), false);
    auto inst_to_node_map
        = std::unordered_map<const IF_EntropyMap::Instruction*,
            IF_EntropyMap::UseMap::Node*>();

    auto get_or_make_node
        = [&_em_insts = std::as_const(_em_insts), &inst_to_node_map,
              &seen_insts, this](const IF_EntropyMap::Instruction* _em_inst,
              bool _start) -> IF_EntropyMap::UseMap::Node*
    {
        if (seen_insts[_em_inst->get_idx()])
        {
            return inst_to_node_map[_em_inst];
        }
        else
        {
            auto new_node = new IF_EntropyMap::UseMap::Node(_em_inst);
            inst_to_node_map.emplace(_em_inst, new_node);
            seen_insts[_em_inst->get_idx()] = true;
            if (_start)
            {
                this->start_nodes.push_back(new_node);
            }
            return new_node;
        }
    };

    for (const auto& [em_inst, llvm_inst] : _em_insts)
    {
        auto curr_node = get_or_make_node(em_inst, true);
        llvm::outs() << "INST ";
        llvm_inst->print(llvm::outs());
        llvm::outs() << '\n';

        for (const auto& user : llvm_inst->users())
        {
            llvm::outs() << "\tUSER ";
            user->print(llvm::outs());
            llvm::outs() << '\n';
            const auto user_inst = llvm::dyn_cast<llvm::Instruction>(user);
            if (!user_inst)
            {
                continue;
            }

            llvm::outs() << "\tINST!!!\n";
            if (const auto llvm_inst_map_it = _llvm_insts_map.find(user_inst);
                llvm_inst_map_it != _llvm_insts_map.end())
            {
                auto em_user_inst = llvm_inst_map_it->second;
                auto em_user_node = get_or_make_node(em_user_inst, false);
                curr_node->users.emplace_back(em_user_node);
            }
        }
    }

    std::cout << this->to_str() << std::endl;
}

std::string
IF_EntropyMap::UseMap::UseMap::to_str(void) const
{
    std::ostringstream ss;
    const std::string line_delim = "----------\n";

    ss << "USEMAP\n" << line_delim;
    for (const auto& n : this->start_nodes)
    {
        ss << n->to_str_path(0);
        ss << line_delim;
    }
    return ss.str();
}
