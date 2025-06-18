/*******************************************************************************
 * IF_Histogram_Entry
 ******************************************************************************/

template <typename I, typename O>
IF_Histogram_Entry<I, O>::outs_t::mapped_type
IF_Histogram_Entry<I, O>::get_out_count(
    IF_Histogram_Entry<I, O>::outs_t::key_type& _out) const
{
    typename outs_t::const_iterator found = this->outputs.find(_out);
    return found == this->outputs.end() ? 0 : found->second;
}

template <typename I, typename O>
void
IF_Histogram_Entry<I, O>::insert(const outs_t::key_type& _output)
{
    this->insert_many(_output, 1);
}

template <typename I, typename O>
void
IF_Histogram_Entry<I, O>::insert_many(
    const outs_t::key_type& _output, uint64_t _out_count)
{
    if (typename outs_t::iterator entry = this->outputs.find(_output);
        entry != this->outputs.end())
    {
        entry->second += _out_count;
    }
    else
    {
        this->outputs.insert({ _output, _out_count });
    }
    this->out_count += _out_count;
}

/*******************************************************************************
 * IF_Histogram
 ******************************************************************************/

template <typename I, typename O>
obs_t
IF_Histogram<I, O>::get_input_observations(void) const
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
IF_Histogram<I, O>::get_output_observations(void) const
{
    typename IF_Histogram_Entry<I, O>::outs_t parsed_obs;

    // Parse output observations
    for (const auto& entry : this->data)
    {
        for (typename IF_Histogram_Entry<I, O>::outs_t::const_reference
                 entry_out : entry->get_outs())
        {
            if (typename IF_Histogram_Entry<I, O>::outs_t::iterator entry
                = parsed_obs.find(entry_out.first);
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
    for (typename IF_Histogram_Entry<I, O>::outs_t::const_reference entry :
        parsed_obs)
    {
        flatten_obs.push_back(entry.second);
    }

    return flatten_obs;
}

template <typename I, typename O>
IF_Histogram<I, O>::data_inv_t
IF_Histogram<I, O>::invert_obs(void) const
{
    data_inv_t inverted_obs;

    // Iterate over all observations
    for (typename data_t::const_reference obs : this->data)
    {
        // Iterate over the observed outputs for a recorded input
        for (typename IF_Histogram_Entry<I, O>::outs_t::const_reference
                 out_obs : obs->get_outs())
        {
            // Check if the output is already logged
            for (typename data_inv_t::reference inv_obs : inverted_obs)
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
                typename data_inv_t::reference inv_ref
                    = inverted_obs.emplace_back(
                        std::make_unique<IF_Histogram_Entry<O, I>>(
                            out_obs.first));
                inv_ref->insert_many(obs->get_in(), out_obs.second);
            }
        done:
            (void) 0;
        }
    }

    return inverted_obs;
}

template <typename I, typename O>
template <typename U>
in_out_obs_t
IF_Histogram<I, O>::count_observations(const U& obs_data) const
{
    in_out_obs_t obs_count;
    obs_t one_obs;

    for (const auto& entry : obs_data)
    {
        one_obs.clear();
        for (const auto& entry_out : entry->get_outs())
        {
            one_obs.emplace_back(entry_out.second);
        }
        obs_count.emplace(entry->get_total_out_count(), one_obs);
    }

    return obs_count;
}

template <typename I, typename O>
void
IF_Histogram<I, O>::insert(const I& key, O& val)
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
IF_Histogram<I, O>::find(const I& key) const
{
    for (typename data_t::const_reference entry : this->data)
    {
        if (entry->get_in() == key)
        {
            return entry.get();
        }
    }
    return nullptr;
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
    in_out_obs_t inputs_obs_count = this->count_observations(this->data);
    return compute_conditional_entropy(inputs_obs_count, this->obs_count);
}

template <typename I, typename O>
double
IF_Histogram<I, O>::calculate_conditional_entropy_in_given_out(void)
{
    const IF_Histogram<I, O>::data_inv_t inverted_obs = this->invert_obs();
    return compute_conditional_entropy(
        this->count_observations(inverted_obs), this->obs_count);
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
IF_Histogram<I, O>::print_measures(void)
{
    std::cout << "=== Histogram measures (" << this->obs_count << " samples)\n";
    std::cout << "\t Entropy(I) = " << this->calculate_entropy_inputs() << "\n";
    std::cout << "\t Entropy(O) = " << this->calculate_entropy_outputs()
              << "\n";
    std::cout << "\t Entropy(I|O) = "
              << this->calculate_conditional_entropy_in_given_out() << "\n";
    std::cout << "\t Entropy(O|I) = "
              << this->calculate_conditional_entropy_out_given_in() << "\n";
    std::cout << "\t U(I|O) = "
              << this->calculate_uncertainty_coefficient_in_given_out() << "\n";
    std::cout << "\t U(O|I) = "
              << this->calculate_uncertainty_coefficient_out_given_in() << "\n";
    std::cout << "=== END\n";
}

