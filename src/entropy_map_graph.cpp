#include "entropy_map_graph.hpp"
#include <llvm/IR/Instruction.h>

const std::string
IF_EM_Graph::emit_instr_node_name(const IF_EntropyMap_Instr& em_instr) const
{
    return "i" + std::to_string(em_instr.get_idx());
}

const std::string
IF_EM_Graph::emit_instr_node(const IF_EntropyMap_Instr& em_instr) const
{
    std::ostringstream oss;
    oss << this->emit_instr_node_name(em_instr) << " [shape = record, label=\""
        << llvm::Instruction::getOpcodeName(em_instr.get_opcode()) << " | "
        << em_instr.get_retained_entropy() << "\"];\n";

    for (const auto& succ : em_instr.get_succs())
    {
        oss << this->emit_instr_node_name(em_instr) << " -> "
            << "i" << succ << ";\n";
    }

    return oss.str();
}

const std::string
IF_EM_Graph::emit_func_node(const IF_EntropyMap_Func& em_fn) const
{
    std::ostringstream oss;
    std::ostringstream oss_ex_fn;
    oss << "subgraph cluster_" << em_fn.get_representing_name() << " {\n";
    oss << "label=\"" << em_fn.get_representing_name() << "\";\n";
    // oss << std::format("subgraph cluster_{} {{\n", this->get_name());
    for (const auto& instr : em_fn.get_instrs())
    {
        oss << emit_instr_node(*instr);
        for (const auto& succ_fn : instr->get_external_succs())
        {
            oss_ex_fn << this->emit_instr_node_name(*instr) << " -> " << succ_fn << ";\n";
        }
    }

    oss << "}\n";

    // Calls to external functions (must be outside subgraph scope)
    oss << oss_ex_fn.str();

    return oss.str();
}

void
IF_EM_Graph::draw_graph(void) const
{
    std::ostringstream graph_ss;
    graph_ss << "digraph G {\n";
    graph_ss << "label=\"Entropy graph for `<file>`\";\n";

    // External function nodes
    for (const auto& ex_fn_name : this->get_entropy_map().get_external_funcs())
    {
        graph_ss << ex_fn_name << " [shape = box];\n";
    }

    // Function subgraphs with their instructions
    for (const auto& em_fn : this->get_entropy_map().get_funcs())
    {
        graph_ss << this->emit_func_node(*em_fn) << '\n';
    }

    graph_ss << "}\n";

    std::ofstream out_file(this->get_output_name());
    out_file << graph_ss.str();
    out_file.close();
}

void
IF_EM_Graph::draw_callgraph(void) const
{
    // std::ostringstream graph_ss;
    // graph_ss << "digraph G{\n";
    // graph_ss << "\tnode [shape=box];\n";
    // for (const auto& em_fn : this->get_entropy_map().get_funcs())
    //{
    // for (const auto& callee : em_fn->get_callees())
    //{
    // graph_ss << "\t"
    //<< em_fn->get_representing_name()
    //<< " -> " << callee->get_representing_name() << ";\n";
    //// graph_ss << std::format(
    ////"\t{} -> {};\n", em_fn->get_name(), callee->get_name());
    //}
    //}
    // graph_ss << "}";

    // std::ofstream out_file(this->get_output_name());
    // out_file << graph_ss.str();
    // out_file.close();
}
