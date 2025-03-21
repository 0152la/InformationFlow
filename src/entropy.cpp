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

IF_Histogram_Entry::IF_Histogram_Entry(if_in_t _input)
    : input(_input)
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
    std::ostringstream vals(
        this->outputs.empty() ? "" : std::to_string(this->outputs.at(0)),
        std::ios_base::ate);

    auto outs = this->outputs.begin();
    std::advance(outs, 1);
    for (; outs != this->outputs.end(); ++outs)
    {
        vals << ", " << *outs;
    }
    std::cout << vals.str() << "]" << std::endl;
}

/*******************************************************************************
 * IF_Histogram
 ******************************************************************************/

in_out_obs
IF_Histogram::get_in_out_obs(void)
{
    in_out_obs new_obs;
    for (const auto& entry : this->data)
    {
        for (const if_out_t entry_out : entry->get_outs())
        {
            in_out_obs::key_type entry_obs { entry->get_in(), entry_out };
            if (auto entry = new_obs.find(entry_obs); entry != new_obs.end())
            {
                entry->second += 1;
            }
            else
            {
                new_obs.insert({ entry_obs, 1 });
            }
        }
    }
    return new_obs;
}

void
IF_Histogram::insert(if_in_t key, if_out_t val)
{
    IF_Histogram_Entry* entry = this->find(key);
    if (!entry)
    {
        entry
            = this->data.emplace_back(std::make_unique<IF_Histogram_Entry>(key))
                  .get();
    }
    entry->insert(val);
    this->observations += 1;
}

IF_Histogram_Entry*
IF_Histogram::find(if_in_t key)
{
    for (const auto& entry : this->data)
    {
        if (entry->get_in() == key)
        {
            return entry.get();
        }
    }
    return nullptr;
}

double
IF_Histogram::calculate_entropy_inputs(void)
{
    double entropy = 0.0;
    double prob;

    // Compute `sum(p(X) * log p(x))`, by computing it for each observed value
    for (const auto& entry : this->data)
    {
        prob = entry->get_out_count() / static_cast<double>(this->observations);
        entropy += prob * log2(prob);
    }

    // Return the negation of the sum
    return -entropy;
}

double
IF_Histogram::calculate_entropy_outputs(void)
{
    double entropy = 0.0;
    double prob;

    std::map<if_out_t, size_t> outs;
    // Gather outputs
    for (const auto& entry : this->data)
    {
        for (if_out_t entry_out : entry->get_outs())
        {
            if (auto entry = outs.find(entry_out); entry != outs.end())
            {
                entry->second += 1;
            }
            else
            {
                outs.insert({ entry_out, 1 });
            }
        }
    }

    // Compute `sum(p(X) * log p(x))`, by computing it for each observed value
    for (const auto out : outs)
    {
        prob = out.second / static_cast<double>(this->observations);
        entropy += prob * log2(prob);
    }

    // Return the negation of the sum
    return -entropy;
}

double
IF_Histogram::calculate_conditional_entropy_in_over_out(void)
{
    double entropy = 0.0;

    in_out_obs obs = this->get_in_out_obs();

    return -entropy;
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
