#ifndef _IF_ENTROPYMAP_GRAPH_HPP
#define _IF_ENTROPYMAP_GRAPH_HPP

#include "entropy_map.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <utility>

class IF_EM_Graph
{
private:
    const std::string out_file;
    const IF_EntropyMap::Map& em;

    static const std::string emit_instr_node_name(
        const IF_EntropyMap::Instruction&);
    static const std::string emit_instr_node_link(
        const IF_EntropyMap::Instruction&, const IF_EntropyMap::Instruction&);
    static const std::string emit_instr_node_link(
        const IF_EntropyMap::Instruction&, IF_EntropyMap::Instruction::idx_t);
    static const std::string emit_instr_node_link(
        const IF_EntropyMap::Instruction&, std::string);

    static const std::string emit_instr_node(const IF_EntropyMap::Instruction&);
    static const std::string emit_instr_node_succs(
        const IF_EntropyMap::Instruction&);
    static const std::string emit_func_node(const IF_EntropyMap::Function&);

    static const std::string emit_external_func_nodes(
        const IF_EntropyMap::Map&);

public:
    IF_EM_Graph(const IF_EntropyMap::Map& _em, std::string _out) :
        out_file(_out),
        em(_em) { };

    const std::string get_output_name(void) const { return this->out_file; };

    const IF_EntropyMap::Map& get_entropy_map(void) const { return this->em; };

    void draw_graph(void) const;
    void draw_callgraph(void) const;
};

#endif // _IF_ENTROPYMAP_GRAPH_HPP
