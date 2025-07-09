#include "entropy.hpp"

/*******************************************************************************
 * IF_Histogram
 ******************************************************************************/

/* Private ********************************************************************/

inline IF_Histogram::input_hash_t
IF_Histogram::extract_input_hash(IF_Histogram::data_t::const_iterator it)
{
    return (*it).first;
}

inline IF_Histogram::input_hash_t
IF_Histogram::extract_input_hash(IF_Histogram::data_t::const_reference rf)
{
    return rf.first;
}

inline IF_Histogram::outputs_t
IF_Histogram::extract_outputs_t(IF_Histogram::data_t::const_iterator it)
{
    return (*it).second;
}

inline IF_Histogram::outputs_t
IF_Histogram::extract_outputs_t(IF_Histogram::data_t::const_reference rf)
{
    return rf.second;
}

inline IF_Histogram::output_hash_t
IF_Histogram::extract_output_hash(IF_Histogram::outputs_t::const_iterator it)
{
    return (*it).first;
}

inline IF_Histogram::output_hash_t
IF_Histogram::extract_output_hash(IF_Histogram::outputs_t::const_reference rf)
{
    return rf.first;
}

inline IF_Histogram::count_t
IF_Histogram::extract_count(IF_Histogram::outputs_t::const_iterator it)
{
    return (*it).second;
}

inline IF_Histogram::count_t
IF_Histogram::extract_count(IF_Histogram::outputs_t::const_reference out)
{
    return out.second;
}

IF_Histogram::count_t
IF_Histogram::compute_input_count(
    IF_Histogram::data_t::const_reference out) const
{
    count_t input_count = 0;
    for (IF_Histogram::outputs_t::const_reference one_out : out.second)
    {
        input_count += extract_count(one_out);
    }
    return input_count;
}

IF_Histogram::obs_t
IF_Histogram::get_input_observations(void) const
{
    obs_t input_obs(this->data.size());

    for (size_t i = 0; data_t::const_reference entry : this->data)
    {
        input_obs.at(i) = compute_input_count(entry);
        if (input_obs.at(i) == 0)
        {
            throw std::runtime_error("Found input observation with count 0!");
        }
        i += 1;
    }

    return input_obs;
}

IF_Histogram::obs_t
IF_Histogram::get_output_observations(void) const
{
    using out_count_t = std::unordered_map<output_hash_t, count_t>;
    out_count_t out_counts;
    output_hash_t curr_out;
    count_t curr_out_count;

    for (data_t::const_reference entry : this->data)
    {
        for (outputs_t::const_reference entry_out : entry.second)
        {
            curr_out = entry_out.first;
            curr_out_count = extract_count(entry_out);
            out_count_t::iterator out_counts_it = out_counts.find(curr_out);
            if (out_counts_it == out_counts.end())
            {
                out_counts.emplace(std::make_pair(curr_out, curr_out_count));
            }
            else
            {
                (*out_counts_it).second += curr_out_count;
            }
        }
    }

    obs_t out_obs;
    for (out_count_t::const_reference out : out_counts)
    {
        if (out.second == 0)
        {
            throw std::runtime_error("Found output observation with count 0!");
        }
        out_obs.emplace_back(out.second);
    }

    return out_obs;
}

IF_Histogram::data_t
IF_Histogram::invert_obs(void) const
{
    data_t inverted_obs;
    data_t::iterator inverted_obs_it;

    for (data_t::const_reference entry : this->data)
    {
        for (outputs_t::const_reference curr_out : extract_outputs_t(entry))
        {
            inverted_obs_it = inverted_obs.find(extract_output_hash(curr_out));
            if (inverted_obs_it == inverted_obs.end())
            {
                bool entered;
                std::tie(inverted_obs_it, entered)
                    = inverted_obs.emplace(extract_output_hash(curr_out),
                        std::unordered_map<input_hash_t, count_t> {});
                if (!entered)
                {
                    std::ostringstream err;
                    err << "Error inserting inverted in_hash <"
                        << extract_input_hash(entry) << "> for out_hash <"
                        << extract_output_hash(curr_out) << ">!\n";
                    throw std::runtime_error(err.str());
                }
            }
            (*inverted_obs_it)
                .second.emplace(
                    extract_input_hash(entry), extract_count(curr_out));
        }
    }
    return inverted_obs;
}

double
IF_Histogram::compute_entropy(const obs_t& observations, const size_t obs_count)
{
    double entropy = 0.0;
    double prob;

    // Compute `sum(p(X) * log p(x))`, by computing it for each observed value
    for (obs_t::const_reference entry : observations)
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
double
IF_Histogram::compute_conditional_entropy(
    const data_t& observations, const size_t obs_count)
{
    double entropy = 0.0;
    double prob;
    double given_prob;
    double obs_count_d = static_cast<double>(obs_count);
    size_t given_obs_count;

    for (data_t::const_reference given_obs : observations)
    {
        // Compute the probability of a specific `given` observation
        //
        // p(x) = no of a specific `x` / total no of observations
        given_obs_count = 0;

        // First, we add up all the `for` observation counts for a `given`
        for (outputs_t::const_reference given_obs_one :
            extract_outputs_t(given_obs))
        {
            given_obs_count += extract_count(given_obs_one);
        }
        // Compute the probability for one `given`
        given_prob = given_obs_count / obs_count_d;

        // Then compute individual sum members for all `for`s
        for (outputs_t::const_reference given_obs_one :
            extract_outputs_t(given_obs))
        {
            // Compute the probabilty of a specific pair of `(given, for)`
            // observations. Since we are iterating over observed `for`s for a
            // `given`, this is just the number of observed `for`s
            //
            // p(x, y) = no of (x, y) / total no of observations
            prob = extract_count(given_obs_one) / obs_count_d;

            // Plug in entropy formula
            entropy += prob * log2(prob / given_prob);
        }
    }

    return -entropy;
}

/* Public *********************************************************************/

void
IF_Histogram::insert(input_hash_t in_hash, output_hash_t out_hash)
{
    this->insert(in_hash, out_hash, 1);
}

void
IF_Histogram::insert(IF_Histogram::input_hash_t in_hash,
    IF_Histogram::output_hash_t out_hash, IF_Histogram::count_t ins_count)
{
    IF_Histogram::data_t::iterator entry = this->data.find(in_hash);

    if (entry == this->data.end())
    {
        // We have not seen this input before; create a fresh entry, with empty
        // observed outputs
        bool entered;
        std::tie(entry, entered)
            = this->data.emplace(std::make_pair(in_hash, outputs_t {}));
        if (!entered)
        {
            std::ostringstream err;
            err << "Error inserting value <" << in_hash << ", " << out_hash
                << ", " << ins_count << "> in histogram!\n";
            throw std::runtime_error(err.str());
        }
    }

    IF_Histogram::outputs_t::iterator entry_out
        = (*entry).second.find(out_hash);
    if (entry_out == (*entry).second.end())
    {
        // We have not seen this output for this input before; create a
        // fresh output entry
        (*entry).second.emplace(std::make_pair(out_hash, ins_count));
    }
    else
    {
        // We have seen this output before; update the observation count
        // value
        (*entry_out).second += ins_count;
    }
    this->obs_count += ins_count;
}

/* Calculators
 */

double
IF_Histogram::calculate_entropy_inputs(void) const
{
    return compute_entropy(this->get_input_observations(), this->obs_count);
}

double
IF_Histogram::calculate_entropy_outputs(void) const
{
    return compute_entropy(this->get_output_observations(), this->obs_count);
}

double
IF_Histogram::calculate_conditional_entropy_out_given_in(void) const
{
    return compute_conditional_entropy(this->data, this->obs_count);
}

double
IF_Histogram::calculate_conditional_entropy_in_given_out(void) const
{
    data_t inverted_obs = this->invert_obs();
    return compute_conditional_entropy(inverted_obs, this->obs_count);
}

double
IF_Histogram::calculate_uncertainty_coefficient_out_given_in(void) const
{
    double out_entropy = this->calculate_entropy_outputs();
    double cond_entropy = this->calculate_conditional_entropy_out_given_in();
    return 1 - cond_entropy / out_entropy;
}

double
IF_Histogram::calculate_uncertainty_coefficient_in_given_out(void) const
{
    double in_entropy = this->calculate_entropy_inputs();
    double cond_entropy = this->calculate_conditional_entropy_in_given_out();
    return 1 - cond_entropy / in_entropy;
}

/* Utility
 */

void
IF_Histogram::print_measures(void) const
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
