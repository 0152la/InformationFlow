#ifndef _IF_ENTROPYMAP_PATH_PRINTER_HPP
#define _IF_ENTROPYMAP_PATH_PRINTER_HPP

#include "entropy_map.hpp"

#include <algorithm>
#include <cmath>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <utility>

namespace IF_EM_Path_Entropy
{

class Cycle;

using path_t = std::vector<const IF_EntropyMap::Instruction*>;
using cycle_split_nodes_t = std::unordered_set<path_t::value_type>;
using cycles_t = std::unordered_set<IF_EM_Path_Entropy::Cycle*>;
using cycles_splits_t
    = std::unordered_map<const IF_EntropyMap::Instruction*, cycles_t>;

class Cycle
{
private:
    const path_t cycle;
    const double entropy;

    double compute_cycle_entropy(const path_t&) const;

public:
    /* Constructors ***********************************************************/

    Cycle(void) = delete;
    Cycle(const path_t&);
    Cycle(const path_t&, const IF_EntropyMap::Instruction*);

    /* Getters ****************************************************************/

    inline const path_t& get_cycle(void) const { return this->cycle; };

    inline size_t get_cycle_length(void) const { return this->cycle.size(); };

    inline path_t::const_reference get_last_instr(void) const
    {
        return this->cycle.back();
    };

    inline double get_entropy(void) const { return this->entropy; };

    inline double get_entropy_bounded(uint16_t bound) const
    {
        return pow(this->entropy, bound);
    };

    /* Others *****************************************************************/

    bool is_same_cycle(const Cycle&) const;

    bool has_node(path_t::value_type) const;

    /* Printers ***************************************************************/

    std::string to_str(void) const;
    std::string to_str_entropy(void) const;
};

class Path
{
private:
    double entropy = 1.0;
    const path_t path_instrs;
    const cycles_t path_cycles;

    double compute_entropy(const IF_EM_Path_Entropy::path_t&);

public:
    /* Constructors ***********************************************************/

    Path(void) = default;
    Path(double _entropy, const path_t& _path, const cycles_t& _cycles) :
        entropy(_entropy),
        path_instrs(_path),
        path_cycles(_cycles) { };
    Path(const path_t& _path, const cycles_t& _cycles) :
        Path(this->compute_entropy(_path), _path, _cycles) { };
    Path(const Path&) = default;

    /* Getters ****************************************************************/

    double get_entropy(void) const { return this->entropy; };

    inline const path_t& get_path(void) const { return this->path_instrs; };

    inline const cycles_t& get_cycles(void) const { return this->path_cycles; };

    path_t::const_reference get_last_instr(void) const
    {
        return this->path_instrs.back();
    };

    /* Printers ***************************************************************/

    std::string to_str(void) const;
    std::string to_str_cycles(void) const;
};

class Stats
{
private:
    size_t paths_count = 0;
    size_t min_path_length = -1;
    size_t max_path_length = 0;

    size_t cycles_count = 0;
    size_t min_cycle_length = -1;
    size_t max_cycle_length = 0;

    size_t cycles_on_paths_count = 0;

public:
    /* Constructors ***********************************************************/

    Stats(void) = default;

    /* Getters ****************************************************************/

    inline size_t get_paths_count(void) const { return this->paths_count; };

    inline size_t get_min_path_length(void) const
    {
        return this->min_path_length;
    }

    inline size_t get_max_path_length(void) const
    {
        return this->max_path_length;
    }

    inline size_t get_cycles_count(void) const { return this->cycles_count; };

    inline size_t get_min_cycle_length(void) const
    {
        return this->min_cycle_length;
    }

    inline size_t get_max_cycle_length(void) const
    {
        return this->max_cycle_length;
    }

    inline size_t get_cycles_on_paths(void) const
    {
        return this->cycles_on_paths_count;
    }

    /* Setters ****************************************************************/

    void add_path(const IF_EM_Path_Entropy::path_t& _path)
    {
        this->paths_count += 1;
        this->min_path_length = std::min(this->min_path_length, _path.size());
        this->max_path_length = std::max(this->max_path_length, _path.size());
    };

    void add_cycle(const IF_EM_Path_Entropy::path_t& _cycle_path)
    {
        this->cycles_count += 1;
        this->min_cycle_length
            = std::min(this->min_cycle_length, _cycle_path.size());
        this->max_cycle_length
            = std::max(this->max_cycle_length, _cycle_path.size());
    };

    void add_cycles_on_paths(size_t to_add)
    {
        this->cycles_on_paths_count += to_add;
    }
};

class Printer
{
protected:
    using paths_t = std::vector<IF_EM_Path_Entropy::Path*>;
    using paths_by_entropy_t
        = std::map<double, IF_EM_Path_Entropy::Printer::paths_t>;

private:
    const std::string out_file;
    const IF_EntropyMap::Map& em;
    paths_t paths;

    cycles_t graph_cycles;
    cycles_splits_t graph_cycles_splits;
    uint16_t bound = 2; // TODO
    bool handle_cycles = false;

    IF_EM_Path_Entropy::Stats stats;
    bool only_stats = true;

    void find_cycles(IF_EM_Path_Entropy::path_t, std::vector<bool>,
        IF_EM_Path_Entropy::cycle_split_nodes_t);
    void crawl_path(IF_EM_Path_Entropy::path_t&, IF_EM_Path_Entropy::cycles_t,
        std::vector<bool>);

    paths_by_entropy_t compute_paths_by_entropy(void) const;

    bool check_is_new_cycle(const Cycle&) const;

public:
    Printer(const IF_EntropyMap::Map& _em, std::string _out) :
        em(_em),
        out_file(_out),
        stats() { };
    Printer(const IF_EntropyMap::Map& _em) :
        Printer(_em, "") { };
    ~Printer();

    void compute_path_entropy(const IF_EntropyMap::Instruction*);

    /* Getters ****************************************************************/

    inline const std::string get_output_file(void) const
    {
        return this->out_file;
    };

    inline const IF_EntropyMap::Map& get_entropy_map(void) const
    {
        return this->em;
    };

    inline auto get_paths(void) const -> const decltype(this->paths)&
    {
        return this->paths;
    };

    inline size_t get_paths_count(void) const
    {
        return this->stats.get_paths_count();
    };

    inline size_t get_cycle_splits_count(void) const
    {
        return this->graph_cycles_splits.size();
    };

    /* Setters ****************************************************************/

    // inline void add_path(paths_t::value_type);
    inline void add_path(
        const IF_EM_Path_Entropy::path_t&, const IF_EM_Path_Entropy::cycles_t&);

    inline void add_cycle(const IF_EM_Path_Entropy::path_t&,
        const IF_EntropyMap::Instruction*,
        const IF_EM_Path_Entropy::cycle_split_nodes_t&);

    /* Printers ***************************************************************/

    void print_cycles(void) const;
    void print_cycles_with_splits(void) const;
    void print_path_entropy(void) const;
    void print_path_entropy_summary(void) const;
    void print_stats(void) const;
};

}; // namespace IF_EM_Path_Entropy

#endif // _IF_ENTROPYMAP_PATH_PRINTER_HPP
