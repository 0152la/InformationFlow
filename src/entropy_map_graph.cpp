#include "entropy_map_graph.hpp"

constexpr std::string instr_node_prefix = "i";

/*******************************************************************************
 * Helper functions
 ******************************************************************************/

const std::string
IF_EM_Graph::emit_instr_node_name(const IF_EntropyMap::Instruction& em_instr)
{
    return instr_node_prefix + std::to_string(em_instr.get_idx());
}

const std::string
IF_EM_Graph::emit_instr_node_link(
    const IF_EntropyMap::Instruction& instr_from, const IF_EntropyMap::Instruction& instr_to)
{
    return emit_instr_node_name(instr_from) + " -> "
        + emit_instr_node_name(instr_to);
}

const std::string
IF_EM_Graph::emit_instr_node_link(const IF_EntropyMap::Instruction& instr_from,
    IF_EntropyMap::Instruction::idx_t instr_to_idx)
{
    return emit_instr_node_name(instr_from) + " -> " + instr_node_prefix
        + std::to_string(instr_to_idx);
}

const std::string
IF_EM_Graph::emit_instr_node_link(
    const IF_EntropyMap::Instruction& instr_from, std::string node_to)
{
    return emit_instr_node_name(instr_from) + " -> " + node_to;
}

const std::string
IF_EM_Graph::emit_external_func_nodes(const IF_EntropyMap::Map& em)
{
    const std::string ex_fn_shape = "box";
    std::ostringstream oss;
    for (const auto& ex_fn_name : em.get_external_funcs())
    {
        oss << ex_fn_name << " [shape = " << ex_fn_shape << "];\n";
    }

    return oss.str();
}

/*******************************************************************************
 * Component functions
 ******************************************************************************/

const std::string
IF_EM_Graph::emit_instr_node(const IF_EntropyMap::Instruction& em_instr)
{
    std::ostringstream oss;
    oss << emit_instr_node_name(em_instr) << " [shape = record, label=\"i"
        << em_instr.get_idx() << " | "
        << llvm::Instruction::getOpcodeName(em_instr.get_opcode()) << " | "
        << em_instr.get_retained_entropy() << "\"];\n";

    for (const auto& succ : em_instr.get_succs())
    {
        oss << emit_instr_node_link(em_instr, succ) << ";\n";
    }

    return oss.str();
}

const std::string
IF_EM_Graph::emit_func_node(const IF_EntropyMap::Function& em_fn)
{
    std::ostringstream oss;
    std::ostringstream oss_ex_fn;
    oss << "subgraph cluster_" << em_fn.get_representing_name() << " {\n";
    oss << "label=\"" << em_fn.get_representing_name() << "\";\n";
    for (const auto& instr : em_fn.get_instrs())
    {
        oss << emit_instr_node(*instr);
        for (const auto& succ_fn : instr->get_external_succs())
        {
            oss_ex_fn << emit_instr_node_link(*instr, succ_fn) << ";\n";
        }
    }

    oss << "}\n";

    // Calls to external functions (must be outside subgraph scope)
    oss << oss_ex_fn.str();

    return oss.str();
}

/*******************************************************************************
 * Graph draw functions
 ******************************************************************************/

void
IF_EM_Graph::draw_graph(void) const
{

    std::ostringstream graph_ss;
    graph_ss << "digraph G {\n";
    graph_ss << "label=\"Entropy graph for `<file>`\";\n";

    graph_ss << emit_external_func_nodes(this->get_entropy_map());

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
    const std::string fn_shape = "diamond";

    std::ostringstream graph_ss;
    graph_ss << "digraph G{\n";
    graph_ss << "node [shape = " << fn_shape << "];\n";

    graph_ss << emit_external_func_nodes(this->get_entropy_map());

    for (const auto& em_fn : this->get_entropy_map().get_funcs())
    {
        graph_ss << em_fn->get_representing_name() << "[shape = " << fn_shape
                 << "];\n";
        for (const auto& fn_instr : em_fn->get_instrs())
        {
            // TODO
        }
    }
    graph_ss << "}";

    std::ofstream out_file(this->get_output_name());
    out_file << graph_ss.str();
    out_file.close();
}
