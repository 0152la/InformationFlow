#include "entropy_map_path_printer.hpp"

/*******************************************************************************
 * IF_EM_Path_Entropy::Cycle
 ******************************************************************************/

IF_EM_Path_Entropy::Cycle::Cycle(const path_t& _cycle) :
    entropy(IF_EM_Path_Entropy::Cycle::compute_cycle_entropy(_cycle)),
    cycle(_cycle) { };

IF_EM_Path_Entropy::Cycle::Cycle(
    const path_t& full_path, const IF_EntropyMap::Instruction* target) :
    Cycle(path_t(std::find(full_path.begin(), full_path.end(), target),
        full_path.end())) { };

double
IF_EM_Path_Entropy::Cycle::compute_cycle_entropy(const path_t& cycle) const
{
    return std::accumulate(cycle.begin(), cycle.end(), 1.0,
        [](double acc, const path_t::value_type& elem)
        { return acc * elem->get_retained_entropy(); });
}

bool
IF_EM_Path_Entropy::Cycle::is_same_cycle(const Cycle& other_cyc) const
{
    const path_t& other_path = other_cyc.get_cycle();
    if (this->cycle.size() != other_path.size())
    {
        return false;
    }

    size_t idx = 0;
    while (idx != this->cycle.size())
    {
        if (this->cycle.at(idx) != other_path.at(idx))
        {
            return false;
        }
        idx += 1;
    }

    return true;
}

bool
IF_EM_Path_Entropy::Cycle::has_node(const path_t::value_type to_check) const
{
    for (path_t::const_reference node : this->cycle)
    {
        if (node == to_check)
        {
            return true;
        }
    }
    return false;
}

std::string
IF_EM_Path_Entropy::Cycle::to_str(void) const
{
    std::string cycle_str = std::accumulate(std::next(this->cycle.begin()),
        this->cycle.end(), this->cycle.front()->to_str_simple(),
        [](std::string s, path_t::value_type i)
        { return std::move(s) + " -> " + i->to_str_simple(); });
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

double
IF_EM_Path_Entropy::Path::compute_entropy(
    const IF_EM_Path_Entropy::path_t& _path)
{
    double entropy = 1.0;
    for (IF_EM_Path_Entropy::path_t::const_reference node : _path)
    {
        entropy *= node->get_retained_entropy();
    }
    return entropy;
}

std::string
IF_EM_Path_Entropy::Path::to_str(void) const
{
    std::ostringstream oss;
    const std::string delim = " -> ";
    oss << std::accumulate(std::next(this->get_path().begin()),
        this->get_path().end(), this->get_path().front()->to_str_simple(),
        [&delim](std::string s, IF_EM_Path_Entropy::path_t::const_reference i)
        { return std::move(s) + delim + i->to_str_simple(); });
    return oss.str();
}

std::string
IF_EM_Path_Entropy::Path::to_str_cycles(void) const
{
    std::ostringstream oss;
    for (IF_EM_Path_Entropy::cycles_t::const_reference cyc : this->get_cycles())
    {
        oss << '\t' << cyc->to_str() << '\n';
    }
    return oss.str();
}

/*******************************************************************************
 * IF_EM_Path_Entropy::Printer
 ******************************************************************************/

void
IF_EM_Path_Entropy::Printer::find_cycles(IF_EM_Path_Entropy::path_t curr_path,
    std::vector<bool> seen_instrs,
    IF_EM_Path_Entropy::cycle_split_nodes_t split_nodes)
{
    auto get_curr_path_back_succs
        = [&curr_path]() -> const IF_EntropyMap::Instruction::succs_t&
    { return curr_path.back()->get_succs_inst(); };

    IF_EntropyMap::Instruction::succs_t::const_iterator succ_it
        = get_curr_path_back_succs().begin();
    IF_EntropyMap::Instruction::succs_t::value_type succ;

    while (succ_it != get_curr_path_back_succs().end())
    {
        succ = *succ_it;

        // If we've seen this node before, it means we have found a cycle; add
        // it and terminate current crawl
        if (seen_instrs.at(succ->get_idx()))
        {
            this->add_cycle(curr_path, succ, split_nodes);
            return;
        }

        // If this is the the only successor, iteratively crawl, without needing
        // to recurse
        if (curr_path.back()->get_succs_count() == 1)
        {
            curr_path.push_back(succ);
            seen_instrs.at(succ->get_idx()) = true;
            succ_it = get_curr_path_back_succs().begin();
            continue;
        }

        // Otherwise, recursively crawl over all successors
        split_nodes.insert(curr_path.back());

        decltype(seen_instrs) split_seen(seen_instrs);
        split_seen.at(succ->get_idx()) = true;
        decltype(curr_path) split_path(curr_path);
        split_path.push_back(succ);
        this->find_cycles(split_path, split_seen, split_nodes);

        std::advance(succ_it, 1);
    }
}

void
IF_EM_Path_Entropy::Printer::crawl_path(IF_EM_Path_Entropy::path_t& curr_path,
    IF_EM_Path_Entropy::cycles_t curr_cycles, std::vector<bool> seen_instrs)
{

    auto get_curr_path_back_succs
        = [&curr_path]() -> const IF_EntropyMap::Instruction::succs_t&
    { return curr_path.back()->get_succs_inst(); };

    IF_EntropyMap::Instruction::succs_t::const_iterator succ_it
        = get_curr_path_back_succs().begin();
    IF_EntropyMap::Instruction::succs_t::value_type succ;

    bool first_it = true;
    while (succ_it != get_curr_path_back_succs().end())
    {
        succ = *succ_it;

        // We followed a cycle, so we terminate crawling this path, without
        // adding it as a new path
        if (seen_instrs.at(succ->get_idx()))
        {
            std::advance(succ_it, 1);
            continue;
        }

        // We are on a straight path; iteratively continue analysis
        if (get_curr_path_back_succs().size() == 1)
        {
            seen_instrs.at(succ->get_idx()) = true;
            curr_path.push_back(succ);
            succ_it = get_curr_path_back_succs().begin();
            continue;
        }

        // We hit a split; first time we find a split node, we check for any
        // cycles to add to our path
        if (handle_cycles && first_it)
        {
            if (cycles_splits_t::const_iterator cyc_it
                = this->graph_cycles_splits.find(curr_path.back());
                cyc_it != this->graph_cycles_splits.end())
            {
                for (cycles_t::const_reference cyc : (*cyc_it).second)
                {
                    curr_cycles.insert(cyc);
                }
            }
            first_it = false;
        }

        // Recursively split the analysis for all successor nodes
        decltype(seen_instrs) split_seen(seen_instrs);
        split_seen.at(succ->get_idx()) = true;
        size_t curr_sz = curr_path.size();
        curr_path.push_back(succ);
        this->crawl_path(curr_path, curr_cycles, split_seen);
        IF_EM_Path_Entropy::path_t::const_iterator revert_it
            = curr_path.begin();
        std::advance(revert_it, curr_sz);
        curr_path.erase(revert_it, curr_path.end());

        std::advance(succ_it, 1);
    }

    // If we reach the end of a path, we add that to our collection of paths
    // if (curr_path->get_last_instr()->get_succs_count() == 0)
    if (curr_path.back()->get_succs_count() == 0)
    {
        this->add_path(curr_path, curr_cycles);
    }
}

IF_EM_Path_Entropy::Printer::paths_by_entropy_t
IF_EM_Path_Entropy::Printer::compute_paths_by_entropy(void) const
{
    paths_by_entropy_t paths_by_entropy;
    double curr_entropy;

    for (IF_EM_Path_Entropy::Printer::paths_t::const_reference pat :
        this->paths)
    {
        curr_entropy = pat->get_entropy();
        if (paths_by_entropy.find(curr_entropy) == paths_by_entropy.end())
        {
            paths_by_entropy.emplace(
                curr_entropy, IF_EM_Path_Entropy::Printer::paths_t());
        }
        paths_by_entropy.at(curr_entropy).push_back(pat);
    }

    return paths_by_entropy;
}

bool
IF_EM_Path_Entropy::Printer::check_is_new_cycle(const Cycle& to_check) const
{
    for (cycles_t::const_reference cyc : this->graph_cycles)
    {
        if (cyc->is_same_cycle(to_check))
        {
            return false;
        }
    }
    return true;
}

IF_EM_Path_Entropy::Printer::~Printer()
{
    for (Printer::paths_t::reference em_path : this->paths)
    {
        delete (em_path);
    }

    for (IF_EM_Path_Entropy::cycles_t::const_reference em_cyc :
        this->graph_cycles)
    {
        delete (em_cyc);
    }
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
    if (this->handle_cycles)
    {
        std::cout << "START find cycles";
        std::cout << std::endl;
        this->find_cycles(
            start_path, seen_instrs, IF_EM_Path_Entropy::cycle_split_nodes_t());
        std::cout << "END find cycles" << std::endl;
    }

    // Now crawl all paths form the `start_instr`
    IF_EM_Path_Entropy::cycles_t start_cycles {};
    this->crawl_path(start_path, start_cycles, seen_instrs);
    std::cout << "END crawl path\n";
}

inline void
IF_EM_Path_Entropy::Printer::add_path(
    const IF_EM_Path_Entropy::path_t& new_path,
    const IF_EM_Path_Entropy::cycles_t& new_path_cycles)
{
    this->stats.add_path(new_path);

    if (this->only_stats)
    {
        return;
    }

    this->paths.emplace_back(
        new IF_EM_Path_Entropy::Path(new_path, new_path_cycles));
    this->stats.add_cycles_on_paths(new_path_cycles.size());
}

inline void
IF_EM_Path_Entropy::Printer::add_cycle(
    const IF_EM_Path_Entropy::path_t& cyc_path,
    const IF_EntropyMap::Instruction* cyc_target,
    const IF_EM_Path_Entropy::cycle_split_nodes_t& cyc_split_nodes)
{
    this->stats.add_cycle(cyc_path);
    if (this->only_stats)
    {
        return;
    }

    IF_EM_Path_Entropy::Cycle* new_cyc
        = new IF_EM_Path_Entropy::Cycle(cyc_path, cyc_target);

    // Only add cycles we've not seen before
    if (this->check_is_new_cycle(*new_cyc))
    {
        this->graph_cycles.insert(
            new IF_EM_Path_Entropy::Cycle(cyc_path, cyc_target));
        for (IF_EM_Path_Entropy::path_t::value_type node : cyc_split_nodes)
        {
            if (!new_cyc->has_node(node))
            {
                continue;
            }

            if (!this->graph_cycles_splits.contains(node))
            {
                this->graph_cycles_splits.emplace(node, cycles_t());
            }
            this->graph_cycles_splits.at(node).insert(new_cyc);
        }
    }
}

void
IF_EM_Path_Entropy::Printer::print_cycles(void) const
{
    if (!this->handle_cycles)
    {
        std::cerr << "Cycle handling disabled - no cycle data available.\n";
        return;
    }

    for (cycles_t::const_reference cyc : this->graph_cycles)
    {
        std::cout << cyc->to_str() << '\n';
    }
}

void
IF_EM_Path_Entropy::Printer::print_cycles_with_splits(void) const
{
    if (!this->handle_cycles)
    {
        std::cerr << "Cycle handling disabled - no cycle data available.\n";
        return;
    }

    for (const auto& [split_node, cycs] : this->graph_cycles_splits)
    {
        std::cout << " == " << split_node->to_str_simple() << '\n';
        for (cycles_t::const_reference cyc : cycs)
        {
            std::cout << '\t' << cyc->to_str() << '\n';
        }
    }
}

void
IF_EM_Path_Entropy::Printer::print_path_entropy(void) const
{
    if (this->only_stats)
    {
        std::cerr
            << "Stats only mode enabled - no path entropy data available.\n";
        return;
    }

    if (this->get_paths().empty())
    {
        throw std::runtime_error("Path printing called before compute!");
    }

    std::cout << "Entropy (" << this->bound << "-bounded)\n";
    for (const auto& path : this->get_paths())
    {
        std::cout << "==========\n";
        std::cout << path->to_str() << '\n';
        std::cout << "Path entropy - " << path->get_entropy() << '\n';

        std::cout << "Path cycles\n";
        std::cout << path->to_str_cycles() << '\n';

        size_t i = 0;
        for (const auto& cyc : path->get_cycles())
        {
            double cyc_entropy = cyc->get_entropy_bounded(this->bound);
            double total_entropy = path->get_entropy() * cyc_entropy;
            std::cout << "Cycle " << i << " entropy - " << cyc->get_entropy()
                      << " -- total " << this->bound << "-bounded "
                      << cyc_entropy << " -- path total " << total_entropy
                      << '\n';
            i += 1;
        }
    }
    std::cout << "==========\n";
}

void
IF_EM_Path_Entropy::Printer::print_path_entropy_summary(void) const
{
    if (this->only_stats)
    {
        std::cerr
            << "Stats only mode enabled - no path entropy data available.\n";
        return;
    }

    std::cout << "========== Entropy paths summary\n";
    std::cout << "Total paths : " << this->stats.get_paths_count() << '\n';

    paths_by_entropy_t paths_by_entropy = this->compute_paths_by_entropy();

    std::cout << "Total entropy unique values : " << paths_by_entropy.size()
              << '\n';
    std::cout << "Path entropy -- Paths count -- % of total :\n";
    for (const auto& [ent, paths] : paths_by_entropy)
    {
        std::cout << "\t" << ent << " -- ";
        std::cout << paths.size() << " -- ";
        std::cout << 100.0 * paths.size() / this->stats.get_paths_count()
                  << '\n';
    }
}

void
IF_EM_Path_Entropy::Printer::print_stats(void) const
{
    std::cout << "========== Entropy paths stats\n";
    std::cout << "Total map instructions : " << this->em.get_instruction_count()
              << '\n';
    std::cout << "Non-trivial instructions : "
              << this->em.get_nontrivial_instruction_count() << '\n';
    const auto em_cc = this->em.compute_cyclomatic_complexity();
    std::cout << "Map cyclomatic complexity : " << std::get<2>(em_cc)
              << " (edges " << std::get<0>(em_cc) << ", nodes "
              << std::get<1>(em_cc) << ")\n";
    std::cout << "Total paths : " << this->stats.get_paths_count() << '\n';
    std::cout << "Total cycles on distinct paths : "
              << this->stats.get_cycles_on_paths() << '\n';
    std::cout << "Min path length : " << this->stats.get_min_path_length()
              << '\n';
    std::cout << "Max path length : " << this->stats.get_max_path_length()
              << '\n';
    std::cout << "Total cycles : " << this->stats.get_cycles_count() << '\n';
    std::cout << "Total cycle split nodes : " << this->get_cycle_splits_count()
              << '\n';
    std::cout << "Min path cycle count : " << this->stats.get_min_cycle_length()
              << '\n';
    std::cout << "Max path cycle count : " << this->stats.get_max_cycle_length()
              << '\n';
    std::cout << "==========\n";
}
