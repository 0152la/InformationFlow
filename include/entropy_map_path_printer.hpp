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
using cycles_t = std::vector<IF_EM_Path_Entropy::Cycle*>;
using cycles_splits_t
    = std::unordered_map<const IF_EntropyMap::Instruction*, cycles_t>;

class Cycle
{
private:
    const path_t cycle;
    const double entropy;

    double compute_cycle_entropy(const path_t&) const;

public:
    Cycle(void) = delete;
    Cycle(const path_t&);
    Cycle(const path_t&, const IF_EntropyMap::Instruction*);

    const path_t& get_cycle(void) const { return this->cycle; };

    path_t::const_reference get_last_instr(void) const
    {
        return this->cycle.back();
    };

    double get_entropy(void) const { return this->entropy; };

    double get_entropy_bounded(uint16_t bound) const
    {
        return pow(this->entropy, bound);
    };

    bool is_same_cycle(const Cycle&) const;

    bool has_node(path_t::value_type) const;

    std::string to_str(void) const;
    std::string to_str_entropy(void) const;
};

class Path
{
private:
    double entropy = 1.0;
    path_t path_instrs;
    cycles_t path_cycles;

public:
    Path(void) = default;
    Path(double _entropy) :
        entropy(_entropy) { };
    Path(double _entropy, path_t& _path) :
        entropy(_entropy),
        path_instrs(_path) { };
    Path(path_t& _path) :
        path_instrs(_path) { };
    Path(const Path&) = default;

    double get_entropy(void) const { return this->entropy; };

    inline const path_t& get_path(void) const { return this->path_instrs; };

    inline const cycles_t& get_cycles(void) const { return this->path_cycles; };

    path_t::const_reference get_last_instr(void) const
    {
        return this->path_instrs.back();
    };

    inline void add_instr(IF_EM_Path_Entropy::path_t::value_type);

    inline void add_cycle(IF_EM_Path_Entropy::cycles_t::value_type);

    std::string to_str(void) const;
    std::string to_str_cycles(void) const;
};

class Printer
{
protected:
    using paths_t = std::vector<IF_EM_Path_Entropy::Path*>;

private:
    const std::string out_file;
    const IF_EntropyMap::Map& em;
    paths_t paths;
    cycles_t graph_cycles;
    cycles_splits_t graph_cycles_splits;
    uint16_t bound = 2; // TODO

    void find_cycles(IF_EM_Path_Entropy::path_t, std::vector<bool>,
        IF_EM_Path_Entropy::cycle_split_nodes_t);
    void crawl_path(IF_EM_Path_Entropy::Path*, std::vector<bool>);

    IF_EM_Path_Entropy::cycles_t find_cycles_with(
        IF_EM_Path_Entropy::path_t::const_reference) const;
    bool check_is_new_cycle(const Cycle&) const;

public:
    Printer(const IF_EntropyMap::Map& _em, std::string _out) :
        em(_em),
        out_file(_out) { };
    Printer(const IF_EntropyMap::Map& _em) :
        Printer(_em, "") { };
    ~Printer();

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

    inline void add_path(paths_t::value_type);

    inline void add_cycle(const IF_EM_Path_Entropy::path_t&,
        const IF_EntropyMap::Instruction*,
        const IF_EM_Path_Entropy::cycle_split_nodes_t&);

    void compute_path_entropy(const IF_EntropyMap::Instruction*);
    void print_cycles(void) const;
    void print_cycles_with_splits(void) const;
    void print_path_entropy(void) const;
};

}; // namespace IF_EM_Path_Entropy

#endif // _IF_ENTROPYMAP_PATH_PRINTER_HPP
