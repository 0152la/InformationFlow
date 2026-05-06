#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <dlfcn.h>
#include <filesystem>

#include <iostream>

#include "entropy_map.hpp"
#include "reader.hpp"
#include "test_harness.hpp"

std::string llvm_snippet_lib_path;

int
do_some(uint64_t);

std::string
get_env_var(const char* env_var_name)
{
    const char* env_var_val = std::getenv(env_var_name);

    if (!env_var_val)
    {
        std::cerr << "Unable to find environment variable `" << env_var_name
                  << "`!\n";
        std::exit(1);
    }

    return std::string(env_var_val);
}

void
do_if_entropy(std::string file_path)
{
    std::cout << "# LLVM-based computed entropy - " << file_path << "\n";
    IF_Parser if_p;
    std::unique_ptr<IF_LLVM_Module> if_m = if_p.parse_ll(file_path);

    std::unique_ptr<IF_EntropyMap::Map> if_em
        = if_p.make_entropy_map(*if_m->get_module());
    if_em->set_verbose(true);
    if_em->print();
}

int
main()
{
    std::cout << "# Function-based computed entropy\n";
    EntropyHarness func_harness(std::pow(10, 5), 0.01);

    uint64_t test_in;
    uint64_t test_out;
    for (size_t i = 0; i < func_harness.get_sample_count(); ++i)
    {
        test_in = func_harness.gen<uint64_t>();
        test_out = do_some(test_in);
        func_harness.log_results(test_in, test_out);
    }
    func_harness.print_measures();


    llvm_snippet_lib_path = get_env_var("IF_LLVM_SNIP_LIB");
    std::string entropy_test_ll = get_env_var("IF_ENTROPY_TEST_FILE");


    do_if_entropy(entropy_test_ll + std::string("-O0.ll"));
    do_if_entropy(entropy_test_ll + std::string("-O2.ll"));

    return 0;
}
