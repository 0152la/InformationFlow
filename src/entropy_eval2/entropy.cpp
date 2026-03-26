#include "entropy.hpp"
#include "result.hpp"

/*******************************************************************************
 * EntropyCalcs
 ******************************************************************************/

double
EntropyCalcs::compute_entropy(
    const EvalData::Counter& counts)
{
    double h_o = 0.0;
    double prob;
    const EvalData::res_t max_val_curr = std::pow(2, counts.bs);
    const EvalData::res_t max_val = std::pow(2, counts.max_bs);

    EvalData::instance_t seen_instances = 0;
    EvalData::instance_t curr_instances;
    for (EvalData::instance_t i = 0;
        i < max_val_curr || seen_instances < counts.instance_count; ++i)
    {
        curr_instances = 0;

        for (EvalData::instance_t j = i; j < max_val; j += max_val_curr)
        {
            curr_instances += counts.instances[j];
        }

        if (curr_instances == 0)
        {
            continue;
        }

        prob = 1.0 * curr_instances / counts.instance_count;
        h_o += prob * std::log2(prob);

        seen_instances += curr_instances;
    }

    return -h_o;
}

double
EntropyCalcs::compute_uncertainty_coef(const EvalData::Counter& counts)
{
    return compute_uncertainty_coef_given_entropy(
        EntropyCalcs::compute_entropy(counts), counts);
}

double
EntropyCalcs::compute_uncertainty_coef_given_entropy(
    double entropy, const EvalData::Counter& counts)
{
    return entropy / std::log2(counts.instance_count);
}

/*******************************************************************************
 * EntropyResultEntry
 ******************************************************************************/

EntropyResultEntry::EntropyResultEntry(//const EntropyCalcs::data_t& _results,
    const EvalData::Counter& _counts) :
    bit_sz(_counts.bs)
{
    this->entropy = EntropyCalcs::compute_entropy(_counts);
    this->uncertainty_coef
        = EntropyCalcs::compute_uncertainty_coef_given_entropy(
            this->entropy, _counts);

    Utils::do_debug_check(
        this->uncertainty_coef < 0.0 || this->uncertainty_coef > 1.0,
        fmt::format("Invalid value `{}` for uncertainty coefficient for "
                    "bit size {}!",
            this->uncertainty_coef, this->bit_sz));
}

const std::string
EntropyResultEntry::to_str(void) const
{
    return fmt::format("Bits {} -- Entropy Outs {} -- UC {}", this->bit_sz,
        this->entropy, this->uncertainty_coef);
}

const std::string
EntropyResultEntry::to_str_csv(std::string_view llvm_fn_name) const
{
    return fmt::format(
        "{},{},{}", llvm_fn_name, this->bit_sz, this->uncertainty_coef);
}

/*******************************************************************************
 * EntropyResult
 ******************************************************************************/

void
EntropyResult::parse_evalresults(const EvalData::Results& er)
{
    auto scaled_res = EvalData::Counter { er.min_bit_sz, er.max_bit_sz };
    for (EvalData::bit_sz_t b = er.min_bit_sz; b <= er.max_bit_sz; ++b)
    {
        scaled_res.combine_results(er.get_results_for_bitsize(b));
        scaled_res.bs = b;
        this->data.emplace(scaled_res);
    }
}

std::string
EntropyResult::to_str(void) const
{
    auto oss = std::ostringstream { };
    oss << "-- Entropy Result\n";
    for (const auto& erd : this->data)
    {
        oss << erd.to_str() << '\n';
    }
    return oss.str();
}

std::string
EntropyResult::to_str_csv(std::string_view llvm_fn_name) const
{
    auto oss = std::ostringstream { };
    for (const auto& erd : this->data)
    {
        oss << erd.to_str_csv(llvm_fn_name) << '\n';
    }
    return oss.str();
}
