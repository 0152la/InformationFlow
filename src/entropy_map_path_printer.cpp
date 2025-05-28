#include "entropy_map_path_printer.hpp"
#include <cassert>

/*******************************************************************************
 * IF_EM_Path_Entropy::Cycle
 ******************************************************************************/

IF_EM_Path_Entropy::Cycle::Cycle(const Cycle::cycle_t& _cycle) :
    entropy(IF_EM_Path_Entropy::Cycle::compute_cycle_entropy(_cycle)),
    cycle(_cycle) { };

double
IF_EM_Path_Entropy::Cycle::compute_cycle_entropy(const Cycle::cycle_t& cycle)
{
    return std::accumulate(cycle.begin(), cycle.end(), 1,
        [](double acc, const cycle_t::value_type& elem)
        { return acc * elem->get_retained_entropy(); });
}

bool
IF_EM_Path_Entropy::Cycle::is_between(
    cycle_t::const_reference cyc_from, cycle_t::const_reference cyc_to) const
{
    return (this->cycle.front() == cyc_from && this->cycle.back() == cyc_to)
        || (this->cycle.back() == cyc_from && this->cycle.front() == cyc_to);
}

/*******************************************************************************
 * IF_EM_Path_Entropy::Path
 ******************************************************************************/

IF_EM_Path_Entropy::Path::cycle_ends_stl_t
IF_EM_Path_Entropy::Path::get_cycle_ends() const
{
    cycle_ends_stl_t cycle_ends;
    for (path_cycle_t::const_reference cycle : this->cycles)
    {
        auto new_ends = std::make_pair(cycle->get_cycle().front()->get_idx(),
            cycle->get_cycle().back()->get_idx());
        cycle_ends.emplace(new_ends, false);
        // cycle_ends.emplace_back(cycle->get_cycle().front()->get_idx(),
        // cycle->get_cycle().back()->get_idx());
    }
    return cycle_ends;
}

IF_EM_Path_Entropy::Path::path_t::const_reverse_iterator
IF_EM_Path_Entropy::Path::rfind_inst(path_t::value_type inst)
{
    path_t::const_reverse_iterator it
        = std::find(this->instr_path.crbegin(), this->instr_path.crend(), inst);
    if (it == this->instr_path.crend())
    {
        throw std::runtime_error(
            "Couldn't find instruction idx " + std::to_string(inst->get_idx()));
    }
    return it;
}

void
IF_EM_Path_Entropy::Path::add_inst(path_t::value_type instr)
{
    this->instr_path.push_back(instr);
    this->seen_idxs.insert(instr->get_idx());
    this->entropy *= instr->get_retained_entropy();
}

void
IF_EM_Path_Entropy::Path::add_cycle_to(path_t::value_type inst)
{
    path_t cycle_path(
        this->instr_path.crbegin(), std::next(this->rfind_inst(inst)));
    this->cycles.push_back(std::make_shared<Cycle>(cycle_path));
}

bool
IF_EM_Path_Entropy::Path::has_cycle(path_t::const_reference next_inst) const
{
    for (auto&& cycle : this->cycles)
    {
        if (cycle->is_between(this->get_last_instr(), next_inst))
        {
            return true;
        }
    }
    return false;
}

bool
IF_EM_Path_Entropy::Path::has_seen(IF_EntropyMap::Instruction::idx_t idx) const
{
    return this->seen_idxs.contains(idx);
}

std::unique_ptr<IF_EM_Path_Entropy::Path>
IF_EM_Path_Entropy::Path::split_path()
{
    return std::make_unique<IF_EM_Path_Entropy::Path>(*this);
}

std::string
IF_EM_Path_Entropy::Path::to_str() const
{
    std::ostringstream oss;
    const std::string delim = " -> ";
    const std::string instr_base = "i";
    cycle_ends_stl_t cycle_ends = this->get_cycle_ends();
    IF_EntropyMap::Instruction::idx_t idx;
    std::string instr_repr;
    for (const auto& instr : this->get_path())
    {
        idx = instr->get_idx();
        instr_repr = instr_base + std::to_string(idx);
        for (auto& [c_ends, seen] : cycle_ends)
        {

            if (idx == c_ends.first || idx == c_ends.second)
            {
                if (seen)
                {
                    instr_repr = instr_repr + "]";
                }
                else
                {
                    instr_repr = "[" + instr_repr;
                    cycle_ends.at(c_ends) = true;
                }
            }
        }
        oss << instr_repr << delim;
    }
    oss.seekp(-delim.length(), std::ios_base::end);
    oss << " == " << this->get_entropy();
    return oss.str();
}

/*******************************************************************************
 * IF_EM_Path_Entropy::Printer
 ******************************************************************************/

void
IF_EM_Path_Entropy::Printer::crawl_path(printer_t::value_type::pointer em_path)
{
    const IF_EntropyMap::Instruction* last_inst = em_path->get_last_instr();
    size_t succs_count = last_inst->get_succs_count();

    if (succs_count == 0)
    {
        return;
    }

    size_t i = 0;
    for (const auto& succ : last_inst->get_succs_inst())
    {
        if (i + 1 == succs_count)
        {
            em_path->add_inst(succ);
            this->crawl_path(em_path);
        }
        else if (!em_path->has_cycle(succ))
        {
            if (em_path->has_seen(succ->get_idx()))
            {
                em_path->add_cycle_to(succ);
            }
            else
            {
                std::unique_ptr<IF_EM_Path_Entropy::Path> new_path
                    = em_path->split_path();
                new_path->add_inst(succ);
                this->crawl_path(new_path.get());
                this->add_path(std::move(new_path));
            }
        }
        i += 1;
    }
}

void
IF_EM_Path_Entropy::Printer::add_path(printer_t::value_type new_path)
{
    this->paths.push_back(std::move(new_path));
}

void
IF_EM_Path_Entropy::Printer::compute_path_entropy(
    const IF_EntropyMap::Instruction* start_instr)
{
    this->paths.emplace_back(
        std::move(std::make_unique<IF_EM_Path_Entropy::Path>()));
    this->paths.back()->add_inst(start_instr);
    this->crawl_path(this->paths.back().get());
}

void
IF_EM_Path_Entropy::Printer::print_path_entropy(void) const
{
    if (this->get_paths().empty())
    {
        throw std::runtime_error("Path printing called before compute!");
    }

    for (const auto& path : this->get_paths())
    {
        std::cout << "==========\n";
        std::cout << path->to_str() << '\n';
    }
    std::cout << "==========\n";
}
