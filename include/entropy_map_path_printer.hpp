#ifndef _IF_ENTROPYMAP_PATH_PRINTER_HPP
#define _IF_ENTROPYMAP_PATH_PRINTER_HPP

#include "entropy_map.hpp"

class IF_EM_Path_Entropy
{
private:
    double entropy;
    std::vector<const IF_EntropyMap_Instr*> path;

public:
    IF_EM_Path_Entropy(void) :
        entropy(1.0) { };
    IF_EM_Path_Entropy(double _entropy) :
        entropy(_entropy) { };
    IF_EM_Path_Entropy(
        double _entropy, std::vector<const IF_EntropyMap_Instr*>& _path) :
        entropy(_entropy),
        path(_path) { };

    double get_entropy(void) const { return this->entropy; };

    auto get_path(void) const -> const decltype(this->path)&
    {
        return this->path;
    };

    const IF_EntropyMap_Instr* get_last_instr(void) const { return this->path.back(); };

    void add_inst(const IF_EntropyMap_Instr*);

    std::unique_ptr<IF_EM_Path_Entropy> split_path(void);

    std::string to_str(void) const;
};

class IF_EM_Path_Printer
{
private:
    const std::string out_file;
    const IF_EntropyMap& em;
    std::vector<std::unique_ptr<IF_EM_Path_Entropy>> paths;

    void crawl_path(IF_EM_Path_Entropy*);

public:
    IF_EM_Path_Printer(const IF_EntropyMap& _em, std::string _out) :
        em(_em),
        out_file(_out) { };
    IF_EM_Path_Printer(const IF_EntropyMap& _em) :
        IF_EM_Path_Printer(_em, "") { };

    const std::string get_output_file(void) const { return this->out_file; };

    const IF_EntropyMap& get_entropy_map(void) const { return this->em; };

    auto get_paths(void) const -> const decltype(paths)&
    {
        return this->paths;
    };

    void add_path(std::unique_ptr<IF_EM_Path_Entropy>);

    void compute_path_entropy(const IF_EntropyMap_Instr*);
    void print_path_entropy(void) const;
};

#endif // _IF_ENTROPYMAP_PATH_PRINTER_HPP
