#include "entropy_map.hpp"

const std::string instr_simple_prefix = "i";

/*******************************************************************************
 * IF_EntropyMap::Instruction
 ******************************************************************************/

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

IF_EntropyMap::Function::~Function()
{
    for (insts_t::const_reference inst : this->instrs)
    {
        delete(inst);
    }
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

const IF_EntropyMap::Instruction*
IF_EntropyMap::Function::get_first_instr() const
{
    return this->instrs.front();
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

IF_EntropyMap::Map::~Map()
{
    for (funcs_t::const_reference fn : this->funcs)
    {
        delete(fn);
    }
}


const IF_EntropyMap::Instruction*
IF_EntropyMap::Map::get_first_instr() const
{
    for (IF_EntropyMap::Map::funcs_t::const_reference func : this->funcs)
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

void
IF_EntropyMap::Map::compress_map(void)
{
    IF_EntropyMap::Function::insts_t compressed_insts;
    IF_EntropyMap::Function::insts_t inst_chain;
    for (const auto& em_fn : this->get_funcs())
    {
        compressed_insts.clear();
        inst_chain.clear();
        for (const auto& fn_inst : em_fn->get_instrs())
        {
            if (!fn_inst->is_trivial() || fn_inst->get_succs_count() > 1)
            {
                if (!inst_chain.empty())
                {
                    IF_EntropyMap::Instruction* to_keep
                        = inst_chain.front();
                    to_keep->clear_succs();
                    to_keep->set_natural_successor(
                        inst_chain.back()->get_natural_successor());
                    to_keep->add_successor(
                        inst_chain.back()->get_natural_successor());
                    to_keep->set_compressed_count(inst_chain.size());
                    compressed_insts.push_back(std::move(inst_chain.front()));
                    inst_chain.clear();
                }
                compressed_insts.push_back(std::move(fn_inst));
            }
            inst_chain.push_back(std::move(fn_inst));
        }
        em_fn->set_insts(std::move(compressed_insts));
    }
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

const std::string
IF_EntropyMap::Map::to_str_summary(void) const
{
    std::ostringstream oss;
    oss << "Total instructions : " << this->get_instruction_count();
    oss << " [ non-trivial " << this->get_nontrivial_instruction_count();
    oss << " ]\n";
    for (const auto& em_fn : this->get_funcs())
    {
        oss << '\t' << em_fn->get_representing_name();
        oss << " -- instr " << em_fn->get_inst_count();
        oss << " [ non-trivial " << em_fn->get_nontrivial_inst_count();
        oss << " ]\n";
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
                std::cout << " -- " << em_fn_instr->get_retained_entropy()
                          << '\n';
                // TODO arguments
            }
        }
    }
}
