#include "result.hpp"

void
EvalResult::add_result(res_t _res)
{
    this->instances[_res] += 1;
    instance_count += 1;
}

void
EvalResult::combine_results(const EvalResult& other)
{
    for (res_t r = 0; r < other.get_max_res_val(); ++r)
    {
        this->instances[r] += other.get_instance(r);
    }
    this->instance_count += other.get_instance_count();
}


void
EvalResult::print(void) const
{
    std::cout << "== EvalResult\n";
    //std::cout << fmt::format(
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
