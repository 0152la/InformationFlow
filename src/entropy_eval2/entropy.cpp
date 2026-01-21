#include "entropy.hpp"
#include <chrono>
#include <fmt/base.h>
#include <sstream>

/*******************************************************************************
 * EntropyCalcs
 ******************************************************************************/

double
EntropyCalcs::compute_entropy(const EvalResult& res)
{
    double h_o = 0.0;
    double prob;
    EvalResult::instance_t seen_instances = 0;
    for (EvalResult::instance_t i = 0; i < res.get_max_res_val()
        && seen_instances < res.get_instance_count_distinct();
        ++i)
    {
        if (res.get_instance(i) == 0)
        {
            continue;
        }

        prob = 1.0 * res.get_instance(i) / res.get_instance_count();
        h_o += prob * std::log2(prob);

        seen_instances += 1;
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
EntropyResult::add_result(EntropyResultEntry* erd)
{
    const auto ins = this->data.insert(erd);
    if (!ins.second)
    {
        throw std::runtime_error(fmt::format(
            "Already computed entropy for bit size {0}!", erd->bit_sz));
    }
}

void
EntropyResult::parse_evalresult(
    const EvalResult& er, std::chrono::microseconds eval_duration)
{
    auto new_erd = new EntropyResultEntry { er.get_bit_sz(), eval_duration,
        EntropyCalcs::compute_entropy(er),
        EntropyCalcs::compute_uncertainty_coef(er) };
    this->add_result(new_erd);
}

void
EntropyResult::print(void) const
{
    std::cout << "-- Entropy Result\n";
    std::ostringstream oss;
    for (const auto erd : this->data)
    {
        oss << erd->to_str() << '\n';
    }
    std::cout << oss.str();
}
