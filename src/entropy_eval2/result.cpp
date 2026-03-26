#include "result.hpp"
#include "config.hpp"
#include "entropy.hpp"

/*******************************************************************************
 * EvalData::Counter
 ******************************************************************************/

EvalData::Counter::Counter(bit_sz_t _bs, bit_sz_t _max_bs) :
    bs(_bs),
    max_bs(_max_bs),
    instance_count(0)
{
    Utils::debug_print(
        fmt::format("=== CONSTRUCT COUNTER bs {} - mbs {}", _bs, _max_bs));
    this->instances = static_cast<EvalData::instance_t*>(
        calloc(std::pow(2, this->max_bs), sizeof(instance_t)));
    Utils::do_check(
        instances == nullptr, "Error initializing EvalResult instances!");
}

EvalData::Counter::Counter(const Counter& other) :
    EvalData::Counter::Counter(other.bs, other.max_bs)
{
    Utils::debug_print(fmt::format(
        "=== COPY COUNTER bs {} - mbs {}", other.bs, other.max_bs));
    this->instance_count = other.instance_count;
    memcpy(this->instances, other.instances,
        std::pow(2, this->max_bs) * sizeof(instance_t));
}

EvalData::Counter::Counter(Counter&& other) noexcept :
    bs(other.bs),
    max_bs(other.max_bs),
    instance_count(other.instance_count)
{
    Utils::debug_print(fmt::format(
        "=== MOVE COUNTER bs {} - mbs {}", other.bs, other.max_bs));
    other.instance_count = 0;
    this->instances = std::exchange(other.instances, nullptr);
}

EvalData::Counter::~Counter(void)
{
    Utils::debug_print(fmt::format(
        "=== DESTROY COUNTER bs {} - mbs {}", this->bs, this->max_bs));
    if (this->instances)
    {
        free(this->instances);
    }
}

EvalData::Counter&
EvalData::Counter::operator=(const EvalData::Counter& other)
{
    Utils::debug_print(fmt::format(
        "=== ASSIGN COPY COUNTER bs {} - mbs {}", other.bs, other.max_bs));
    return *this = EvalData::Counter(other);
}

EvalData::Counter&
EvalData::Counter::operator=(EvalData::Counter&& other) noexcept
{
    Utils::debug_print(fmt::format(
        "=== ASSIGN MOVE COUNTER bs {} - mbs {}", other.bs, other.max_bs));
    std::swap(this->instances, other.instances);
    std::swap(this->bs, other.bs);
    std::swap(this->max_bs, other.max_bs);
    std::swap(this->instance_count, other.instance_count);
    return *this;
}

void
EvalData::Counter::add_result(res_t new_res)
{
    this->instances[new_res] += 1;
    this->instance_count += 1;
    Utils::debug_print(fmt::format("ADD RES {} TO BS {} -- SAW {} OF {}", new_res, this->bs,
        this->instances[new_res], this->instance_count));
}

void
EvalData::Counter::combine_results(const Counter& other)
{
    for (res_t r = 0; r < this->get_max_res_val(); ++r)
    {
        this->instances[r] += other.instances[r];
    }
    this->instance_count += other.instance_count;
}

EvalData::res_t
EvalData::Counter::get_max_res_val(void) const
{
    return std::pow(2, this->max_bs);
}

std::string
EvalData::Counter::to_str(void) const
{
    std::ostringstream oss;
    oss << fmt::format(
        "Bit size {} -- Total instances {}\n", this->bs, this->instance_count);
    for (res_t r = 0; r < this->get_max_res_val(); ++r)
    {
        oss << fmt::format("\t -- {} == {}\n", r, this->instances[r]);
    }
    return oss.str();
}

/*******************************************************************************
 * EvalData::Results
 ******************************************************************************/

EvalData::Results::Results(bit_sz_t _min_bit_sz, bit_sz_t _max_bit_sz) :
    min_bit_sz(_min_bit_sz),
    max_bit_sz(_max_bit_sz)
{
    Utils::do_debug_check(this->min_bit_sz > this->max_bit_sz,
        fmt::format("Given lower bit size bound {} larger than max {}!",
            this->min_bit_sz, this->max_bit_sz));

    this->results.reserve(this->max_bit_sz - this->min_bit_sz);
    for (bit_sz_t b = _min_bit_sz; b <= _max_bit_sz; ++b)
    {
        this->results.emplace_back(b, this->max_bit_sz);
    }
}

EvalData::Results::~Results(void) { this->results.clear(); }

void
EvalData::Results::add_result(res_t res, bit_sz_t bs)
{
    bs = std::max(bs, this->min_bit_sz);

    Utils::do_debug_check(bs > this->max_bit_sz,
        fmt::format("Given bit size {} higher than top bound {}!", bs,
            this->max_bit_sz));
    Utils::do_debug_check(res >= this->get_max_res_val(),
        fmt::format("Tried to log value {} (bit size {}) larger than expected "
                    "max value {} with max bit size {}!",
            fmt::group_digits(res), bs,
            fmt::group_digits(this->get_max_res_val()), this->max_bit_sz));

    this->results[bs - this->min_bit_sz].add_result(res);
}

void
EvalData::Results::combine_results(const EvalData::Results& other)
{
    Utils::do_check(!(this->max_bit_sz <= other.max_bit_sz
                        || this->min_bit_sz >= other.min_bit_sz),
        fmt::format(
            "Invalid `EvalData::Results` combination: attempting to combine "
            "results with bit range [{}, {}] into bit range [{}, {}]!",
            other.min_bit_sz, other.max_bit_sz, this->min_bit_sz,
            this->max_bit_sz));

    for (const auto& other_res : other.get_results())
    {
        this->results[other_res.bs].combine_results(other_res);
    }
}

auto
EvalData::Results::get_results(void) const -> const decltype(this->results)&
{
    return this->results;
}

auto
EvalData::Results::get_results_for_bitsize(EvalData::bit_sz_t b) const
    -> const EvalData::Counter&
{
    return this->results.at(b - this->min_bit_sz);
}

auto
EvalData::Results::get_results_count(void) const
    -> decltype(EvalData::Counter::instance_count)
{
    decltype(EvalData::Counter::instance_count) counts = 0;
    for (const auto& c : this->results)
    {
        counts += c.instance_count;
    }
    return counts;
}

auto
EvalData::Results::get_max_res_val(void) const -> res_t
{
    return std::pow(2, this->max_bit_sz);
}

std::string
EvalData::Results::to_str(void) const
{
    std::ostringstream oss;
    oss << "== EvalData::Results\n";

    for (const auto& c : this->results)
    {
        oss << c.to_str();
    }

    return oss.str();
}
