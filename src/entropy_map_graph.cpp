#include "entropy_map_graph.hpp"
#include <llvm/IR/Instruction.h>

const std::string
IF_EM_Graph::emit_instr_node(const IF_EntropyMap_Instr& em_instr) const
{
    return "i" + std::to_string(em_instr.get_idx())
        + " [shape = record, label=\""
        + llvm::Instruction::getOpcodeName(em_instr.get_opcode()) + " | "
        + std::to_string(em_instr.get_retained_entropy()) + "\"];";
    // return std::format("i{} [shape = record, label=\"{} | {}\"];",
    // this->get_idx(), llvm::Instruction::getOpcodeName(this->get_opcode()),
    // this->get_retained_entropy());
}

const std::string
IF_EM_Graph::emit_func_node(const IF_EntropyMap_Func& em_fn) const
{
    std::ostringstream oss;
    oss << "subgraph cluster_" << em_fn.get_name() << " {\n";
    // oss << std::format("subgraph cluster_{} {{\n", this->get_name());
    for (const auto& instr : em_fn.get_instrs())
    {
    }
    return oss.str();
}

void
IF_EM_Graph::draw_graph(void) const
{
    std::ostringstream graph_ss;
    graph_ss << "digraph G {\n";
    graph_ss << "\tlabel=Entropy graph for `<file>`\n";
}

void
IF_EM_Graph::draw_callgraph(void) const
{
    std::ostringstream graph_ss;
    graph_ss << "digraph G{\n";
    graph_ss << "\tnode [shape=box];\n";
    for (const auto& em_fn : this->get_entropy_map().get_funcs())
    {
        for (const auto& callee : em_fn->get_callees())
        {
            graph_ss << "\t"
                     << em_fn->get_representing_name()
                     << " -> " << callee->get_representing_name() << ";\n";
            // graph_ss << std::format(
            //"\t{} -> {};\n", em_fn->get_name(), callee->get_name());
        }
    }
    graph_ss << "}";

    std::ofstream out_file(this->get_output_name());
    out_file << graph_ss.str();
    out_file.close();
}
