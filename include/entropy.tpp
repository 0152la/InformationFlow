/*******************************************************************************
 * IF_Histogram_Entry
 ******************************************************************************/

template <typename I, typename O>
void
IF_Histogram_Entry<I, O>::insert(O _output)
{
    this->insert_many(_output, 1);
}

template <typename I, typename O>
void
IF_Histogram_Entry<I, O>::insert_many(O _output, uint64_t _out_count)
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

template <typename I, typename O>
void
IF_Histogram_Entry<I, O>::print(void)
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

template <typename I, typename O>
void
IF_Histogram<I, O>::insert(I key, O val)
{
    IF_Histogram_Entry<I, O>* entry = this->find(key);
    if (!entry)
    {
        entry
            = this->data
                  .emplace_back(std::make_unique<IF_Histogram_Entry<I, O>>(key))
                  .get();
    }
    entry->insert(val);
    this->obs_count += 1;
}

template <typename I, typename O>
IF_Histogram_Entry<I, O>*
IF_Histogram<I, O>::find(I key)
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

template <typename I, typename O>
obs_t
IF_Histogram<I, O>::get_input_observations(void)
{
    obs_t input_obs;

    for (const auto& entry : this->data)
    {
        input_obs.push_back(entry->get_total_out_count());
    }

    return input_obs;
}

template <typename I, typename O>
obs_t
IF_Histogram<I, O>::get_output_observations(void)
{
    std::map<I, size_t> parsed_obs;

    // Parse output observations
    for (const auto& entry : this->data)
    {
        for (const std::pair<I, uint64_t>& entry_out : entry->get_outs())
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

template <typename I, typename O>
IF_Histogram<I, O>::data_t
IF_Histogram<I, O>::invert_obs(void)
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
                    std::make_unique<IF_Histogram_Entry<I, O>>(out_obs.first));
                inv_ref->insert_many(obs->get_in(), out_obs.second);
            }
        done:
            (void) 0;
        }
    }

    return inverted_obs;
}

template <typename I, typename O>
in_out_obs_t
IF_Histogram<I, O>::data_t_to_in_out_obs_t(const data_t& in_out_data)
{
    in_out_obs_t in_out_obs;
    obs_t one_out_obs;

    for (const auto& entry : in_out_data)
    {
        one_out_obs.clear();
        for (const auto& entry_out : entry->get_outs())
        {
            one_out_obs.emplace_back(entry_out.second);
        }
        in_out_obs.emplace(entry->get_total_out_count(), one_out_obs);
    }

    return in_out_obs;
}

template <typename I, typename O>
double
IF_Histogram<I, O>::calculate_entropy_inputs(void)
{
    return compute_entropy(this->get_input_observations(), this->obs_count);
}

template <typename I, typename O>
double
IF_Histogram<I, O>::calculate_entropy_outputs(void)
{
    return compute_entropy(this->get_output_observations(), this->obs_count);
}

template <typename I, typename O>
double
IF_Histogram<I, O>::calculate_conditional_entropy_out_given_in(void)
{
    in_out_obs_t inputs_obs_count = this->data_t_to_in_out_obs_t(this->data);
    return compute_conditional_entropy(inputs_obs_count, this->obs_count);
}

template <typename I, typename O>
double
IF_Histogram<I, O>::calculate_conditional_entropy_in_given_out(void)
{
    const decltype(this->data) inverted_obs = this->invert_obs();
    return compute_conditional_entropy(
        this->data_t_to_in_out_obs_t(inverted_obs), this->obs_count);
}

template <typename I, typename O>
double
IF_Histogram<I, O>::calculate_uncertainty_coefficient_out_given_in(void)
{
    double out_entropy = this->calculate_entropy_outputs();
    double cond_entropy = this->calculate_conditional_entropy_out_given_in();
    return 1 - cond_entropy / out_entropy;
}

template <typename I, typename O>
double
IF_Histogram<I, O>::calculate_uncertainty_coefficient_in_given_out(void)
{
    double in_entropy = this->calculate_entropy_inputs();
    double cond_entropy = this->calculate_conditional_entropy_in_given_out();
    return 1 - cond_entropy / in_entropy;
}

template <typename I, typename O>
void
IF_Histogram<I, O>::print(void)
{
    std::cout << "=== Dumping Histogram" << std::endl;
    for (auto&& data : this->data)
    {
        data.get()->print();
    }
    std::cout << "=== END" << std::endl;
}
