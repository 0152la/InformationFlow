#include "result.hpp"

EvalResult::EvalResult(uint8_t _output_bit_sz) :
    res_bit_sz(_output_bit_sz)
{
    // TODO we just allocate space of 64-bit integer of instances (since we
    // generally won't go over that), but ideally we pack things tightly, while
    // ensuring the amount of memory allocated is well aligned
    instances = static_cast<instance_t*>(
        calloc(pow(2, res_bit_sz), sizeof(uint64_t)));
    if (instances == nullptr)
    {
        throw std::runtime_error("Error initializing EvalResult instances!\n");
    }
}

EvalResult::EvalResult(uint8_t _output_bit_sz, EvalResultCache& _cache) :
    EvalResult(_output_bit_sz)
{
    if (_cache.active)
    {
        this->instances = _cache.move_instances();
        size_t prev_bs_max_count = std::pow(2, this->res_bit_sz - 1);
        size_t prev_bs_size = prev_bs_max_count * sizeof(instance_t);
        this->instances = static_cast<instance_t*>(
            realloc(this->instances, 2 * prev_bs_size));
        std::memset(this->instances + prev_bs_max_count, 0, prev_bs_size);
        this->used_cache = true;
    }
}

EvalResult::~EvalResult(void)
{
    if (this->instances)
    {
        free(this->instances);
    }
}

void
EvalResult::add_result(res_t _res)
{
    this->instances[_res] += 1;
    instance_count += 1;
}

void
EvalResult::combine_results(const EvalResult& other)
{
    assert(this->res_bit_sz >= other.get_bit_sz());
    for (res_t r = 0; r < other.get_max_res_val(); ++r)
    {
        this->instances[r] += other.get_instance(r);
    }
    this->instance_count += other.get_instance_count();
}

auto
EvalResult::move_instances(void) -> decltype(this->instances)
{
    auto inst_ptr = this->instances;
    this->instances = nullptr;
    return inst_ptr;
}

void
EvalResult::print(void) const
{
    std::cout << "== EvalResult\n";
    // std::cout << fmt::format(
    //"  - Total distinct results :: {0}\n", this->instance_count_distinct);

    std::ostringstream raw_instances_oss;
    size_t total_insts = 0;
    for (res_t i = 0; i < this->get_max_res_val(); ++i)
    {
        if (this->instances[i] != 0)
        {
            raw_instances_oss
                << fmt::format("\t{0} -- {1}\n", i, this->instances[i]);
            total_insts += this->instances[i];
        }
    }

    std::cout << fmt::format("  - Total instances :: {0}", total_insts);
    std::cout << "\n" << raw_instances_oss.str();
}

EvalResultCache::~EvalResultCache(void)
{
    if (this->active)
    {
        free(this->res);
    }
}

void
EvalResultCache::set_results(EvalResult& res)
{
    this->res = res.move_instances();
    this->active = true;
}

auto
EvalResultCache::move_instances(void) -> decltype(this->res)
{
    auto ptr = this->res;
    this->res = nullptr;
    this->active = false;
    return ptr;
}
