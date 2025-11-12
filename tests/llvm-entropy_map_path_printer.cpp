#include <algorithm>
#include <iostream>
#include <memory>

#include "entropy_map_graph.hpp"
#include "entropy_map_path_printer.hpp"
#include "instr_emulator.hpp"
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
    const char* snippets_lib_path = get_env_var("IF_LLVM_SNIP_LIB");
    size_t threshold = 1000;

    IF_Emulator emu(snippets_lib_path);

    IF_Parser if_p;
    std::unique_ptr<IF_LLVM_Module> if_m = if_p.parse_ll(input_file_path);
    std::unique_ptr<IF_EntropyMap::Map> if_em
        = if_p.make_entropy_map(*if_m->get_module());

    IF_EM_Path_Entropy::Printer em_pr(*if_em);
    em_pr.compute_path_entropy(if_em->get_first_instr());
    if (em_pr.get_paths_count() > threshold)
    {
        em_pr.print_path_entropy_summary();
    }
    else
    {
        em_pr.print_path_entropy();
    }
    em_pr.print_stats();

    return 0;
}
