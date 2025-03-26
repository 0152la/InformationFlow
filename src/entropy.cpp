#include "entropy.hpp"

static double
compute_entropy(const obs_t& observations, const size_t obs_count)
{
    double entropy = 0.0;
    double prob;

    // Compute `sum(p(X) * log p(x))`, by computing it for each observed value
    for (const auto& entry : observations)
    {
        prob = entry / static_cast<double>(obs_count);
        entropy += prob * log2(prob);
    }

    // Return the negation of the sum
    return -entropy;
}

static double
compute_conditional_entropy(
    const std::vector<std::unique_ptr<IF_Histogram_Entry>>& observations,
    uint64_t obs_count)
{
    double entropy = 0.0;
    double prob;
    double in_prob;
    double obs_count_d = static_cast<double>(obs_count);

    for (const auto& entry : observations)
    {
        in_prob = entry->get_total_out_count() / obs_count_d;
        for (const auto& out : entry->get_outs())
        {
            prob = out.second / obs_count_d;
            entropy += prob * log2(prob / in_prob);
        }
    }

    return -entropy;
}

/*******************************************************************************
 * IF_Histogram_Entry
 ******************************************************************************/

void
IF_Histogram_Entry::insert(if_out_t _output)
{
    // this->outputs.push_back(_output);

    if (auto entry = this->outputs.find(_output); entry != this->outputs.end())
    {
        entry->second += 1;
    }
    else
    {
        this->outputs.insert({ _output, 1 });
    }
    this->out_count += 1;
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
        vals << ", <" << outs->first << ", " << outs->second << ">";
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
        entry
            = this->data.emplace_back(std::make_unique<IF_Histogram_Entry>(key))
                  .get();
    }
    entry->insert(val);
    this->obs_count += 1;
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
    obs_t parsed_obs;

    // Flatten observations
    for (const auto& entry : this->data)
    {
        parsed_obs.push_back(entry->get_total_out_count());
    }

    return compute_entropy(parsed_obs, this->obs_count);
}

double
IF_Histogram::calculate_entropy_outputs(void)
{
    std::map<if_out_t, size_t> parsed_obs;

    // Parse output observations
    for (const auto& entry : this->data)
    {
        for (const std::pair<if_out_t, uint64_t>& entry_out : entry->get_outs())
        {
            if (auto entry = parsed_obs.find(entry_out.first);
                entry != parsed_obs.end())
            {
                entry->second += entry_out.second;
            }
            else
            {
                parsed_obs.insert(entry_out);
            }
        }
    }

    // Flatten observations
    obs_t flatten_obs;
    for (const auto& entry : parsed_obs)
    {
        flatten_obs.push_back(entry.second);
    }

    return compute_entropy(flatten_obs, this->obs_count);
}

double
IF_Histogram::calculate_conditional_entropy_out_given_in(void)
{
    return compute_conditional_entropy(this->data, this->obs_count);
}

double
IF_Histogram::calculate_uncertainty_coefficient_out_given_in(void)
{
    double out_entropy = this->calculate_entropy_outputs();
    double cond_entropy = this->calculate_conditional_entropy_out_given_in();
    return 1 - cond_entropy / out_entropy;
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
