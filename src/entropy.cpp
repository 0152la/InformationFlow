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

/* This computes the conditional entropy, given a structure of observations, of
 * the form `<given, <for:count>>`, where count is the number of times a `for`
 * was observed for the respective `given`. We also take the total number of
 * observations (equal to the total number of `count`s), for performance
 * reasons.
 */
static double
compute_conditional_entropy(
    const IF_Histogram::data_t& observations, uint64_t obs_count)
{
    double entropy = 0.0;
    double prob;
    double in_prob;
    double obs_count_d = static_cast<double>(obs_count);

    for (const auto& entry : observations)
    {
        // Compute the probability of a specific `given` observation
        //
        // p(x) = no of a specific `x` / total no of observations
        in_prob = entry->get_total_out_count() / obs_count_d;

        for (const auto& out : entry->get_outs())
        {
            // Compute the probabilty of a specific pair of `(given, for)`
            // observations. Since we are iterating over observed `for`s for a
            // `given`, this is just the number of observed `for`s
            //
            // p(x, y) = no of (x, y) / total no of observations
            prob = out.second / obs_count_d;

            // Plug in entropy formula
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
    this->insert_many(_output, 1);
}

void
IF_Histogram_Entry::insert_many(if_out_t _output, uint64_t _out_count)
{
    if (auto entry = this->outputs.find(_output); entry != this->outputs.end())
    {
        entry->second += _out_count;
    }
    else
    {
        this->outputs.insert({ _output, _out_count });
    }
    this->out_count += _out_count;
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

obs_t
IF_Histogram::get_output_observations(void)
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

    return flatten_obs;
}

auto
IF_Histogram::invert_obs(void) -> const decltype(this->data)
{
    decltype(this->data) inverted_obs;

    // Iterate over all observations
    for (const auto& obs : this->data)
    {
        // Iterate over the observed outputs for a recorded input
        for (const auto& out_obs : obs->get_outs())
        {
            // Check if the output is already logged
            for (const auto& inv_obs : inverted_obs)
            {
                // If we found the logged observation, add the number of times
                // we saw this pair
                if (inv_obs->get_in() == out_obs.first)
                {
                    inv_obs->insert_many(obs->get_in(), out_obs.second);
                    goto done;
                }
            }
            // We didn't see this observation yet, so we must insert it
            {
                auto& inv_ref = inverted_obs.emplace_back(
                    std::make_unique<IF_Histogram_Entry>(out_obs.first));
                inv_ref->insert_many(obs->get_in(), out_obs.second);
            }
        done:
            (void) 0;
        }
    }

    return inverted_obs;
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
    return compute_entropy(this->get_output_observations(), this->obs_count);
}

double
IF_Histogram::calculate_conditional_entropy_out_given_in(void)
{
    return compute_conditional_entropy(this->data, this->obs_count);
}

double
IF_Histogram::calculate_conditional_entropy_in_given_out(void)
{
    const decltype(this->data) inverted_obs = this->invert_obs();
    return compute_conditional_entropy(inverted_obs, this->obs_count);
}

double
IF_Histogram::calculate_uncertainty_coefficient_out_given_in(void)
{
    double out_entropy = this->calculate_entropy_outputs();
    double cond_entropy = this->calculate_conditional_entropy_out_given_in();
    return 1 - cond_entropy / out_entropy;
}

double
IF_Histogram::calculate_uncertainty_coefficient_in_given_out(void)
{
    double in_entropy = this->calculate_entropy_inputs();
    double cond_entropy = this->calculate_conditional_entropy_in_given_out();
    return 1 - cond_entropy / in_entropy;
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
