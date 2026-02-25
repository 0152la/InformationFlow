#include "entropy.hpp"

/*******************************************************************************
 * EntropyCalcs
 ******************************************************************************/

double
EntropyCalcs::compute_entropy(const EvalResult& res)
{
    double h_o = 0.0;
    double prob;
    EvalResult::instance_t seen_instances = 0;
    EvalResult::instance_t curr_instances;
    const EvalResult::instance_t total_instances = res.get_instance_count();
    for (EvalResult::instance_t i = 0;
        i < res.get_max_res_val() || seen_instances < total_instances; ++i)
    {
        if (res.get_instance(i) == 0)
        {
            continue;
        }
        curr_instances = res.get_instance(i);

        prob = 1.0 * curr_instances / total_instances;
        h_o += prob * std::log2(prob);

        seen_instances += curr_instances;
    }
    return -h_o;
}

double
EntropyCalcs::compute_uncertainty_coef(const EvalResult& res)
{
    return EntropyCalcs::compute_entropy(res)
        / std::log2(res.get_instance_count());
}

/*******************************************************************************
 * EntropyResultEntry
 ******************************************************************************/

const std::string
EntropyResultEntry::to_str(void) const
{
    return fmt::format(
        "Bits {} -- Entropy Outs {} -- UC {} -- Duration {} - {}", this->bit_sz,
        this->entropy, this->uncertainty_coef, this->dur_ms,
        std::chrono::duration_cast<std::chrono::seconds>(this->dur_ms));
}

const std::string
EntropyResultEntry::to_str_csv(
    std::string_view llvm_fn_name) const
{
    return fmt::format("{},{},{}", llvm_fn_name,
        this->bit_sz, this->uncertainty_coef);
}

/*******************************************************************************
 * EntropyResult
 ******************************************************************************/

EntropyResult::~EntropyResult(void)
{
    for (const auto ere : this->data)
    {
        delete ere;
    }
}

void
EntropyResult::add_result(EntropyResultEntry* ere)
{
    const auto ins = this->data.insert(ere);
    if (!ins.second)
    {
        throw std::runtime_error(fmt::format(
            "Already computed entropy for bit size {0}!", ere->bit_sz));
    }
}

void
EntropyResult::parse_evalresult(
    const EvalResult& er, std::chrono::microseconds eval_duration)
{
    auto new_erd = new EntropyResultEntry { er.get_bit_sz(), eval_duration,
        EntropyCalcs::compute_entropy(er),
        EntropyCalcs::compute_uncertainty_coef(er) };

    if (new_erd->uncertainty_coef < 0.0 || new_erd->uncertainty_coef > 1.0)
    {
        throw std::runtime_error(
            fmt::format("Invalid value `{}` for uncertainty coefficient!",
                new_erd->uncertainty_coef));
    }

    this->add_result(new_erd);
}

std::string
EntropyResult::to_str(void) const
{
    auto oss = std::ostringstream {};
    oss << "-- Entropy Result\n";
    for (const auto erd : this->data)
    {
        oss << erd->to_str() << '\n';
    }
    return oss.str();
}

std::string
EntropyResult::to_str_csv(
    std::string_view llvm_fn_name) const
{
    auto oss = std::ostringstream {};
    for (const auto erd : this->data)
    {
        oss << erd->to_str_csv(llvm_fn_name) << '\n';
    }
    return oss.str();
}
