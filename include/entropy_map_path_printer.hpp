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
#include <utility>

namespace IF_EM_Path_Entropy
{
using path_t = std::vector<const IF_EntropyMap::Instruction*>;

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

    path_t::const_reference get_first_instr(void) const
    {
        return this->cycle.front();
    };

    double get_entropy(void) const { return this->entropy; };

    double get_entropy_bounded(uint16_t bound) const
    {
        return pow(this->entropy, bound);
    };

    std::string to_str(void) const;
    std::string to_str_entropy(void) const;
};

class Path
{
public:
    using path_cycles_t = std::vector<Cycle*>;
    using cycle_ends_t = std::pair<IF_EntropyMap::Instruction::idx_t,
        IF_EntropyMap::Instruction::idx_t>;
    using cycle_ends_stl_t = std::map<cycle_ends_t, bool>;

private:
    double entropy;
    path_t instr_path;
    path_cycles_t cycles;
    std::set<IF_EntropyMap::Instruction::idx_t> seen_idxs;

    cycle_ends_stl_t get_cycle_ends(void) const;

public:
    Path(void) :
        entropy(1.0) { };
    Path(double _entropy) :
        entropy(_entropy) { };
    Path(double _entropy, path_t& _path) :
        entropy(_entropy),
        instr_path(_path) { };
    Path(const Path&) = default;

    double get_entropy(void) const { return this->entropy; };

    const path_t& get_path(void) const { return this->instr_path; };

    path_cycles_t::const_reference get_last_cycle(void) const
    {
        return this->cycles.back();
    };

    path_t::const_reference get_last_instr(void) const
    {
        return this->instr_path.back();
    };

    void add_instr(path_t::value_type);

    IF_EM_Path_Entropy::Path* split_path(void) const;

    std::string to_str(void) const;
};

class Printer
{
protected:
    using paths_t = std::vector<IF_EM_Path_Entropy::Path*>;
    using crawls_t = std::queue<paths_t::value_type>;
    using cycles_t = std::vector<IF_EM_Path_Entropy::Cycle*>;

private:
    const std::string out_file;
    const IF_EntropyMap::Map& em;
    paths_t paths;
    crawls_t to_crawl;
    cycles_t graph_cycles;

    void find_cycles(IF_EM_Path_Entropy::path_t, std::vector<bool>);
    // void crawl_path(paths_t::value_type);

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

    inline void add_path_to_crawl(crawls_t::value_type);

    inline void add_cycle(
        const IF_EM_Path_Entropy::path_t&, const IF_EntropyMap::Instruction*);

    // inline void add_cycle(IF_EM_Path_Entropy::Cycle* cyc)
    //{
    // this->graph_cycles.push_back(cyc);
    //};

    void compute_path_entropy(const IF_EntropyMap::Instruction*);
    void print_cycles(void) const;
    void print_path_entropy(void) const;
};

}; // namespace IF_EM_Path_Entropy

#endif // _IF_ENTROPYMAP_PATH_PRINTER_HPP
