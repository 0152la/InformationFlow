#include "entropy_map_path_printer.hpp"

/*******************************************************************************
 * IF_EM_Path_Entropy::Cycle
 ******************************************************************************/

IF_EM_Path_Entropy::Cycle::Cycle(const path_t& _cycle) :
    entropy(IF_EM_Path_Entropy::Cycle::compute_cycle_entropy(_cycle)),
    cycle(_cycle) { };

IF_EM_Path_Entropy::Cycle::Cycle(
    const path_t& full_path, const IF_EntropyMap::Instruction* target) :
    cycle(
        std::find(full_path.begin(), full_path.end(), target), full_path.end()),
    entropy(IF_EM_Path_Entropy::Cycle::compute_cycle_entropy(this->cycle)) { };

double
IF_EM_Path_Entropy::Cycle::compute_cycle_entropy(const path_t& cycle) const
{
    return std::accumulate(cycle.begin(), cycle.end(), 1.0,
        [](double acc, const path_t::value_type& elem)
        { return acc * elem->get_retained_entropy(); });
}

std::string
IF_EM_Path_Entropy::Cycle::to_str(void) const
{
    auto get_node_str = [](path_t::value_type i)
    { return "i" + std::to_string(i->get_idx()); };
    std::string cycle_str = std::accumulate(std::next(this->cycle.begin()),
        this->cycle.end(), get_node_str(this->cycle.front()),
        [&get_node_str](std::string s, path_t::value_type i)
        { return std::move(s) + " -> " + get_node_str(i); });
    return "[" + cycle_str + "]";
}

std::string
IF_EM_Path_Entropy::Cycle::to_str_entropy(void) const
{
    std::ostringstream ss;
    ss << "== Cycle entropy\n";
    for (IF_EM_Path_Entropy::path_t::const_reference instr : this->cycle)
    {
        ss << '\t' << instr->to_str();
    }
    ss << "== Total Entropy " << this->compute_cycle_entropy(this->cycle);
    ss << " (Member " << this->entropy << ")";
    return ss.str();
}

/*******************************************************************************
 * IF_EM_Path_Entropy::Path
 ******************************************************************************/

IF_EM_Path_Entropy::Path::cycle_ends_stl_t
IF_EM_Path_Entropy::Path::get_cycle_ends() const
{
    cycle_ends_stl_t cycle_ends;
    for (path_cycles_t::const_reference cycle : this->cycles)
    {
        auto new_ends = std::make_pair(cycle->get_cycle().front()->get_idx(),
            cycle->get_cycle().back()->get_idx());
        cycle_ends.emplace(new_ends, false);
    }
    return cycle_ends;
}

void
IF_EM_Path_Entropy::Path::add_instr(path_t::value_type instr)
{
    this->instr_path.push_back(instr);
    this->seen_idxs.insert(instr->get_idx());
    this->entropy *= instr->get_retained_entropy();
}

IF_EM_Path_Entropy::Path*
IF_EM_Path_Entropy::Path::split_path(void) const
{
    return new IF_EM_Path_Entropy::Path(*this);
}

std::string
IF_EM_Path_Entropy::Path::to_str(void) const
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
IF_EM_Path_Entropy::Printer::find_cycles(
    IF_EM_Path_Entropy::path_t curr_path, std::vector<bool> seen_instrs)
{
    // As long as we're on a straight path in the graph, we keep on doing the
    // cycle checks, without recursing
    while (curr_path.back()->get_succs_count() == 1)
    {
        const IF_EntropyMap::Instruction* succ
            = *(curr_path.back()->get_succs_inst().begin());
        if (seen_instrs.at(succ->get_idx()))
        {
            this->add_cycle(curr_path, succ);
            // this->graph_cycles.emplace_back(
            // new IF_EM_Path_Entropy::Cycle(curr_path, succ));
            return;
        }
        else
        {
            curr_path.push_back(succ);
            seen_instrs.at(succ->get_idx()) = true;
        }
    }

    // Once we hit a split, we first check each child for a cycle, then recurse
    // over them
    for (IF_EntropyMap::Instruction::succs_t::const_reference succ :
        curr_path.back()->get_succs_inst())
    {
        // TODO collapse
        if (seen_instrs.at(succ->get_idx()))
        {
            this->add_cycle(curr_path, succ);
            // this->graph_cycles.emplace_back(
            // new IF_EM_Path_Entropy::Cycle(curr_path, succ));
            return;
        }
        else
        {
            decltype(seen_instrs) split_seen(seen_instrs);
            split_seen.at(succ->get_idx()) = true;
            decltype(curr_path) split_path(curr_path);
            split_path.push_back(succ);
            this->find_cycles(split_path, split_seen);
        }
    }
}

/*
void
IF_EM_Path_Entropy::Printer::crawl_path(paths_t::value_type curr_path)
{
    const IF_EntropyMap::Instruction* curr_instr = curr_path->get_last_instr();
    size_t succs_count = curr_instr->get_succs_count();

    // Check if we're adding a cycle
    if (curr_path->has_seen(curr_instr->get_idx()))
    {
        // curr_path->add_cycle_to(curr_instr); // TODO
        curr_instr = curr_path->get_last_cycle()->get_first_instr();
    }
    else
    {
        curr_path->add_instr(curr_instr);
    }

    // Iterate over successors and add new paths to crawl
    IF_EntropyMap::Instruction::succs_t::const_iterator succ_instr;
    while (succs_count != 0)
    {
        // Add first successor to this path
        succ_instr = curr_instr->get_succs_inst().begin();

        if (curr_path->has_seen((*succ_instr)->get_idx()))
        {
            // TODO handle
        }
        else
        {
            curr_path->add_instr(*succ_instr);
        }

        std::advance(succ_instr, 1);
        while (succ_instr != curr_instr->get_succs_inst().end())
        {
            if (curr_path->has_seen((*succ_instr)->get_idx()))
            {
                // TODO handle
            }
            else
            {
                // this->add_path_to_crawl(curr_path->split_path(*succ_instr));
            }
        }

        // curr_instr = curr_instr->get_succs_inst().front();
        succs_count = curr_instr->get_succs_count();
    }

    // size_t i = 0;
    // for (const auto& succ : last_instr->get_all_successors())
    //{
    //// This is the last successor for the current instruction
    // if (i + 1 == succs_count)
    //{
    // em_path->add_inst(succ);
    // this->crawl_path(em_path);
    //}
    //// We still have successors to crawl -- create a new path to be
    //// crawled, if it is not a cycle we have previously crawled
    // else if (!em_path->has_cycle(succ))
    //{
    //// If we find a cycle, add it to the list of cycles of the current
    //// path
    // if (em_path->has_seen(succ->get_idx()))
    //{
    // em_path->add_cycle_to(succ);
    //}
    //// Otherwise create a split path
    // else
    //{
    // std::unique_ptr<IF_EM_Path_Entropy::Path> new_path
    //= em_path->split_path();
    // new_path->add_inst(succ);
    // this->crawl_path(new_path.get());
    // this->add_path(std::move(new_path));
    //}
    //}
    // i += 1;
    //}
}
*/

IF_EM_Path_Entropy::Printer::~Printer()
{
    for (Printer::paths_t::reference em_path : this->paths)
    {
        delete (em_path);
    }

    for (Printer::cycles_t::reference em_cyc : this->graph_cycles)
    {
        delete (em_cyc);
    }
}

inline void
IF_EM_Path_Entropy::Printer::add_path(crawls_t::value_type new_path)
{
    this->paths.push_back(new_path);
}

inline void
IF_EM_Path_Entropy::Printer::add_cycle(
    const IF_EM_Path_Entropy::path_t& cyc_path,
    const IF_EntropyMap::Instruction* cyc_target)
{
    this->graph_cycles.emplace_back(
        new IF_EM_Path_Entropy::Cycle(cyc_path, cyc_target));
}

void
IF_EM_Path_Entropy::Printer::compute_path_entropy(
    const IF_EntropyMap::Instruction* start_instr)
{
    // First, we find all the cycles from the given start node
    IF_EM_Path_Entropy::path_t start_path { start_instr };
    std::vector<bool> seen_instrs(
        this->get_entropy_map().get_instruction_count(), false);
    seen_instrs.at(start_instr->get_idx()) = true;
    this->find_cycles(start_path, seen_instrs);
    this->print_cycles();
    std::exit(1);

    // Create the first path, and give it `start_instrs` as the first node
    IF_EM_Path_Entropy::Path* first_path = new IF_EM_Path_Entropy::Path();
    first_path->add_instr(start_instr);

    // Add the first path to the list of paths needed to be crawled
    this->to_crawl.push(first_path);

    // Crawl all remaining paths
    while (!this->to_crawl.empty())
    {
        // this->crawl_path(this->to_crawl.front());
        this->to_crawl.pop();
    }
}

void
IF_EM_Path_Entropy::Printer::print_cycles(void) const
{
    for (cycles_t::const_reference cyc : this->graph_cycles)
    {
        std::cout << cyc->to_str() << '\n';
        std::cout << cyc->to_str_entropy() << '\n';
    }
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
