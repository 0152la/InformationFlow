#include "entropy.hpp"

double
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
double
compute_conditional_entropy(
    const in_out_obs_t& observations, uint64_t obs_count)
{
    double entropy = 0.0;
    double prob;
    double in_prob;
    double obs_count_d = static_cast<double>(obs_count);

    for (const auto& obs : observations)
    {
        // Compute the probability of a specific `given` observation
        //
        // p(x) = no of a specific `x` / total no of observations
        in_prob = obs.first / obs_count_d;

        for (const auto& no_obs_outs : obs.second)
        {
            // Compute the probabilty of a specific pair of `(given, for)`
            // observations. Since we are iterating over observed `for`s for a
            // `given`, this is just the number of observed `for`s
            //
            // p(x, y) = no of (x, y) / total no of observations
            prob = no_obs_outs / obs_count_d;

            // Plug in entropy formula
            entropy += prob * log2(prob / in_prob);
        }
    }

    return -entropy;
}
