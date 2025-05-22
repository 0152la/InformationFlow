#include "entropy_map_path_printer.hpp"
#include <memory>
#include <sstream>
#include <stdexcept>

/*******************************************************************************
 * IF_EM_Path_Entropy
 ******************************************************************************/

void
IF_EM_Path_Entropy::add_inst(const IF_EntropyMap_Instr* instr)
{
    this->path.push_back(instr);
    this->entropy *= instr->get_retained_entropy();
}

std::unique_ptr<IF_EM_Path_Entropy>
IF_EM_Path_Entropy::split_path()
{
    return std::make_unique<IF_EM_Path_Entropy>(this->entropy, this->path);
}

std::string
IF_EM_Path_Entropy::to_str() const
{
    std::ostringstream oss;
    const std::string delim = " -> ";
    for (const auto& instr : this->get_path())
    {
        oss << "i" << instr->get_idx() << delim;
    }
    oss.seekp(-delim.length(), std::ios_base::end);
    oss << " == " << this->get_entropy();
    return oss.str();
}

/*******************************************************************************
 * IF_EM_Path_Printer
 ******************************************************************************/

void
IF_EM_Path_Printer::crawl_path(IF_EM_Path_Entropy* em_path)
{
    const IF_EntropyMap_Instr* last_inst = em_path->get_last_instr();
    size_t succs_count = last_inst->get_succs_count();
    size_t i = 0;
    for (const auto& succ : last_inst->get_succs_inst())
    {
        if (i + 1 == succs_count)
        {
            em_path->add_inst(succ);
            this->crawl_path(em_path);
        }
        else
        {
            std::unique_ptr<IF_EM_Path_Entropy> new_path = em_path->split_path();
            new_path->add_inst(succ);
            this->crawl_path(new_path.get());
            this->add_path(std::move(new_path));
        }
        i += 1;
    }
}

void
IF_EM_Path_Printer::add_path(std::unique_ptr<IF_EM_Path_Entropy> new_path)
{
    this->paths.push_back(std::move(new_path));
}

void
IF_EM_Path_Printer::compute_path_entropy(const IF_EntropyMap_Instr* start_instr)
{
    this->paths.emplace_back(std::move(std::make_unique<IF_EM_Path_Entropy>()));
    this->paths.back()->add_inst(start_instr);
    this->crawl_path(this->paths.back().get());
}

void
IF_EM_Path_Printer::print_path_entropy(void) const
{
    if (this->get_paths().empty())
    {
        throw std::runtime_error("Path printing called before compute!");
    }

    for (const auto& path : this->get_paths())
    {
        std::cout << path->to_str() << '\n';
    }
}
