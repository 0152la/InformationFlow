#ifndef _IF_ENTROPYMAP_GRAPH_HPP
#define _IF_ENTROPYMAP_GRAPH_HPP

#include <fstream>
#include <sstream>
#include <string>
#include <utility>

#include "entropy_map.hpp"

class IF_EM_Graph
{
private:
    const std::string out_file;
    const IF_EntropyMap& em;

    static const std::string emit_instr_node_name(const IF_EntropyMap_Instr&);
    static const std::string emit_instr_node_link(
        const IF_EntropyMap_Instr&, const IF_EntropyMap_Instr&);
    static const std::string emit_instr_node_link(const IF_EntropyMap_Instr&,
        decltype(std::declval<IF_EntropyMap_Instr>().get_idx()));
    static const std::string emit_instr_node_link(
        const IF_EntropyMap_Instr&, std::string);

    static const std::string emit_instr_node(const IF_EntropyMap_Instr&);
    static const std::string emit_func_node(const IF_EntropyMap_Func&);

    static const std::string emit_external_func_nodes(const IF_EntropyMap&);

public:
    IF_EM_Graph(const IF_EntropyMap& _em, std::string _out) :
        out_file(_out),
        em(_em) { };

    const std::string get_output_name(void) const { return this->out_file; };

    const IF_EntropyMap& get_entropy_map(void) const { return this->em; };

    void draw_graph(void) const;
    void draw_callgraph(void) const;
};

#endif // _IF_ENTROPYMAP_GRAPH_HPP
