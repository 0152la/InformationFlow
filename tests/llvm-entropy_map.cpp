#include <cstdlib>
#include <iostream>
#include <memory>

#include "entropy_map.hpp"
#include "reader.hpp"

const char*
get_env_var(const char* env_var_name)
{
    const char* env_var_val = std::getenv(env_var_name);


    if (!env_var_val)
    {
        std::cerr << "Unable to find environment variable `" << env_var_name
                  << "`!\n";
        std::exit(1);
    }

    return env_var_val;
}

int
main()
{
    const char* input_file_path = get_env_var("IF_LLVM_TEST_FILE");
    const char* llvm_snippet_lib_path = get_env_var("IF_LLVM_SNIP_LIB");

    IF_Parser if_p;
    std::unique_ptr<IF_LLVM_Module> if_m = if_p.parse_ll(input_file_path);

    IF_Emulator if_emu(llvm_snippet_lib_path);
    std::unique_ptr<IF_EntropyMap::Map> if_em
        = if_p.make_entropy_map(*if_m->get_module());
    if_em->set_verbose(true);

    //std::cout << if_em->to_str_summary();
    //if_em->compress_map();
    //std::cout << if_em->to_str_summary();
    //if_em->print();
    if_em->print_stats();

    return 0;
}
