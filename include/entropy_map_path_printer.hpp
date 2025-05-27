#ifndef _IF_ENTROPYMAP_PATH_PRINTER_HPP
#define _IF_ENTROPYMAP_PATH_PRINTER_HPP

#include "entropy_map.hpp"

#include <algorithm>
#include <cmath>
#include <map>
#include <memory>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace IF_EM_Path_Entropy
{

class Cycle
{
public:
    using cycle_t = std::vector<const IF_EntropyMap_Instr*>;

private:
    const double entropy;
    const cycle_t cycle;

    double compute_cycle_entropy(const cycle_t&);

public:
    Cycle(void) = delete;
    Cycle(const cycle_t&);

    const cycle_t& get_cycle(void) const { return this->cycle; };

    double get_entropy(void) const { return this->entropy; };

    double get_entropy_bounded(uint16_t bound) const
    {
        return pow(this->entropy, bound);
    };

    bool is_between(cycle_t::const_reference, cycle_t::const_reference) const;
};

class Path
{
public:
    using path_t = std::vector<const IF_EntropyMap_Instr*>;
    using path_cycle_t = std::vector<std::shared_ptr<Cycle>>;
    using cycle_ends_t
        = std::pair<IF_EntropyMap_Instr::idx_t, IF_EntropyMap_Instr::idx_t>;
    using cycle_ends_stl_t = std::map<cycle_ends_t, bool>;

private:
    double entropy;
    path_t instr_path;
    path_cycle_t cycles;
    std::set<IF_EntropyMap_Instr::idx_t> seen_idxs;

    cycle_ends_stl_t get_cycle_ends(void) const;
    path_t::const_reverse_iterator rfind_inst(path_t::value_type);

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

    path_t::const_reference get_last_instr(void) const
    {
        return this->instr_path.back();
    };

    void add_inst(path_t::value_type);

    void add_cycle_to(path_t::value_type);

    bool has_cycle(path_t::const_reference) const;

    bool has_seen(
        decltype(std::declval<IF_EntropyMap_Instr>().get_idx())) const;

    std::unique_ptr<IF_EM_Path_Entropy::Path> split_path(void);

    std::string to_str(void) const;
};

class Printer
{
protected:
    using printer_t = std::vector<std::unique_ptr<IF_EM_Path_Entropy::Path>>;

private:
    const std::string out_file;
    const IF_EntropyMap& em;
    printer_t paths;

    void crawl_path(printer_t::value_type::pointer);

public:
    Printer(const IF_EntropyMap& _em, std::string _out) :
        em(_em),
        out_file(_out) { };
    Printer(const IF_EntropyMap& _em) :
        Printer(_em, "") { };

    const std::string get_output_file(void) const { return this->out_file; };

    const IF_EntropyMap& get_entropy_map(void) const { return this->em; };

    const printer_t& get_paths(void) const { return this->paths; };

    void add_path(printer_t::value_type);

    void compute_path_entropy(const IF_EntropyMap_Instr*);
    void print_path_entropy(void) const;
};

};

#endif // _IF_ENTROPYMAP_PATH_PRINTER_HPP
