#include <cstdlib>
#include <iostream>
#include <memory>

#include "entropy_map.hpp"
#include "reader.hpp"

int
main()
{
    const char* ifp_env_var = "IF_LLVM_TEST_FILE";
    const char* input_file_path = std::getenv(ifp_env_var);

    if (!input_file_path)
    {
        std::cerr << "Unable to find environment variable `" << ifp_env_var
                  << "`!\n";
        return 1;
    }

    IF_Parser if_p;
    std::unique_ptr<IF_LLVM_Module> if_m = if_p.parse_ll(input_file_path);

    std::unique_ptr<IF_EntropyMap::Map> if_em
        = if_p.make_entropy_map(*if_m->get_module());
    if_em->set_verbose(true);
    if_em->print();

    return 0;
}
