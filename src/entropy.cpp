#include "entropy.hpp"

float
compute_entropy(IF_Histogram& vals)
{
    vals.print();
    return 0.0;
}

/*******************************************************************************
 * IF_Histogram_Entry
 ******************************************************************************/

IF_Histogram_Entry::IF_Histogram_Entry(if_in_t _input) : input(_input)
{
    this->outputs = std::vector<if_out_t>();
}

void
IF_Histogram_Entry::insert(if_out_t _output)
{
    this->outputs.push_back(_output);
}

void
IF_Histogram_Entry::print(void)
{
    std::cout << "== INPUT " << this->input << " -- OUTPUTS [";
    std::ostringstream vals(this->outputs.empty() ? "" : std::to_string(this->outputs.at(0)), std::ios_base::ate);
    for (auto outs : this->outputs | std::views::drop(1))
    {
        vals << ", " << outs;
    }
    std::cout << vals.str() << "]" << std::endl;
}

/*******************************************************************************
 * IF_Histogram
 ******************************************************************************/

void
IF_Histogram::insert(if_in_t key, if_out_t val)
{
    IF_Histogram_Entry* entry = this->find(key);
    if (!entry)
    {
        entry = this->data.emplace_back(std::make_unique<IF_Histogram_Entry>(key)).get();
    }
    entry->insert(val);
}

IF_Histogram_Entry*
IF_Histogram::find(if_in_t key)
{
    for (auto&& entry : this->data)
    {
        if (entry->get_in() == key)
        {
            return entry.get();
        }
    }
    return nullptr;
}

void
IF_Histogram::print(void)
{
    std::cout << "=== Dumping Histogram" << std::endl;
    for (auto&& data : this->data)
    {
        data.get()->print();
    }
    std::cout << "=== END" << std::endl;
}
