#include "result.hpp"
#include "config.hpp"
#include "entropy.hpp"

/*******************************************************************************
 * EvalData::Counter
 ******************************************************************************/

EvalData::Counter::Counter(EvalData::bit_sz_t _in_bs,
    EvalData::bit_sz_t _out_bs, EvalData::bit_sz_t _in_bs_max) :
    bit_sz_in(_in_bs),
    bit_sz_out(_out_bs),
    bit_sz_in_max(_in_bs_max),
    instance_count(0)
{
    //Utils::debug_print(
        //fmt::format("CONSTRUCT COUNTER bs {} - - obs {} - mbs {}",
            //this->bit_sz_in, this->bit_sz_out, this->bit_sz_in_max));
    this->instances = static_cast<EvalData::instance_t*>(
        calloc(std::pow(2, this->bit_sz_out), sizeof(instance_t)));
    Utils::do_check(
        instances == nullptr, "Error initializing EvalResult instances!");
};

EvalData::Counter::Counter(const Counter& other) :
    EvalData::Counter::Counter(
        other.bit_sz_in, other.bit_sz_out, other.bit_sz_in_max)
{
    //Utils::debug_print(fmt::format("COPY COUNTER ibs {} - obs {} - mbs {}",
        //this->bit_sz_in, this->bit_sz_out, this->bit_sz_in_max));
    this->instance_count = other.instance_count;
    memcpy(this->instances, other.instances,
        std::pow(2, this->bit_sz_in_max) * sizeof(instance_t));
}

EvalData::Counter::Counter(Counter&& other) noexcept :
    EvalData::Counter::Counter(
        other.bit_sz_in, other.bit_sz_out, other.bit_sz_in_max)
{
    //Utils::debug_print(fmt::format("MOVE COUNTER ibs {} - obs {} - mbs {}",
        //other.bit_sz_in, other.bit_sz_out, other.bit_sz_in_max));
    this->instance_count = other.instance_count;
    this->instances = std::exchange(other.instances, nullptr);
}

EvalData::Counter::~Counter(void)
{
    //Utils::debug_print(fmt::format("DESTROY COUNTER ibs {} - obs {} - mbs {}",
        //this->bit_sz_in, this->bit_sz_out, this->bit_sz_in_max));
    if (this->instances)
    {
        free(this->instances);
    }
}

EvalData::Counter&
EvalData::Counter::operator=(const EvalData::Counter& other)
{
    //Utils::debug_print(
        //fmt::format("ASSIGN COPY COUNTER ibs {} - obs {} - mbs {}",
            //other.bit_sz_in, this->bit_sz_out, other.bit_sz_in_max));
    return *this = EvalData::Counter(other);
}

EvalData::Counter&
EvalData::Counter::operator=(EvalData::Counter&& other) noexcept
{
    //Utils::debug_print(
        //fmt::format("ASSIGN MOVE COUNTER ibs {} - obs {} - mbs {}",
            //other.bit_sz_in, other.bit_sz_out, other.bit_sz_in_max));
    std::swap(this->instances, other.instances);
    std::swap(this->bit_sz_in, other.bit_sz_in);
    std::swap(this->bit_sz_out, other.bit_sz_out);
    std::swap(this->bit_sz_in_max, other.bit_sz_in_max);
    std::swap(this->instance_count, other.instance_count);
    return *this;
}

void
EvalData::Counter::add_result(res_t new_res)
{
    this->instances[new_res] += 1;
    this->instance_count += 1;
    //Utils::debug_print(
        //fmt::format("ADD RES {} TO BS {} -- SAW {} OF {}", new_res,
            //this->bit_sz_in, this->instances[new_res], this->instance_count));
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
    return std::pow(2, this->bit_sz_out);
}

std::string
EvalData::Counter::to_str(bool filtered) const
{
    std::ostringstream oss;
    oss << fmt::format("Bit size {} -- Total instances {}\n", this->bit_sz_in,
        this->instance_count);
    for (res_t r = 0; r < this->get_max_res_val(); ++r)
    {
        if (!filtered || this->instances[r] != 0)
        {
            oss << fmt::format("\t -- {} == {}\n", r, this->instances[r]);
        }
    }
    return oss.str();
}

/*******************************************************************************
 * EvalData::Results
 ******************************************************************************/

EvalData::Results::Results(EvalData::bit_sz_t _bit_sz_in_min,
    EvalData::bit_sz_t _bit_sz_in_max, EvalData::bit_sz_t _bit_sz_out) :
    bit_sz_in_min(_bit_sz_in_min),
    bit_sz_in_max(_bit_sz_in_max),
    bit_sz_out(_bit_sz_out)
{
    Utils::do_debug_check(this->bit_sz_in_min > this->bit_sz_in_max,
        fmt::format("Given lower bit size bound {} larger than max {}!",
            this->bit_sz_in_min, this->bit_sz_in_max));

    this->results.reserve(this->bit_sz_in_max - this->bit_sz_in_min + 1);
    for (bit_sz_t b = this->bit_sz_in_min; b <= this->bit_sz_in_max; ++b)
    {
        this->results.emplace_back(b, this->bit_sz_out, this->bit_sz_in_max);
    }
}

EvalData::Results::~Results(void) { this->results.clear(); }

void
EvalData::Results::add_result(res_t res, bit_sz_t bs)
{
    bs = std::max(bs, this->bit_sz_in_min);

    Utils::do_debug_check(bs > this->bit_sz_in_max,
        fmt::format("Given bit size {} higher than top bound {}!", bs,
            this->bit_sz_in_max));
    Utils::do_debug_check(res >= this->get_max_res_val(),
        fmt::format("Tried to log value {} (bit size {}) larger than expected "
                    "max value {} with max bit size {}!",
            fmt::group_digits(res), bs,
            fmt::group_digits(this->get_max_res_val()), this->bit_sz_out));

    this->results[bs - this->bit_sz_in_min].add_result(res);
}

void
EvalData::Results::combine_results(const EvalData::Results& other)
{
    Utils::do_check(!(this->bit_sz_in_max <= other.bit_sz_in_max
                        || this->bit_sz_in_min >= other.bit_sz_in_min),
        fmt::format(
            "Invalid `EvalData::Results` combination: attempting to combine "
            "results with bit range [{}, {}] into bit range [{}, {}]!",
            other.bit_sz_in_min, other.bit_sz_in_max, this->bit_sz_in_min,
            this->bit_sz_in_max));

    for (const auto& other_res : other.get_results())
    {
        this->results[other_res.bit_sz_in - this->bit_sz_in_min]
            .combine_results(other_res);
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
    return this->results.at(b - this->bit_sz_in_min);
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
    return std::pow(2, this->bit_sz_out);
}

std::string
EvalData::Results::to_str(bool filtered) const
{
    std::ostringstream oss;
    oss << "== EvalData::Results\n";

    for (const auto& c : this->results)
    {
        oss << c.to_str(filtered);
    }

    return oss.str();
}
