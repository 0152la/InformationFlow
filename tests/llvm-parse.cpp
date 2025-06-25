#include <iostream>
#include <cstdlib>

#include "reader.hpp"

int
main()
{
    const char* ifp_env_var = "IF_LLVM_TEST_FILE";
    const char* input_file_path = std::getenv(ifp_env_var);

    if (!input_file_path)
    {
        std::cerr << "Unable to find environment variable `" << ifp_env_var << "`!\n";
        return 1;
    }

    IF_Parser if_p;
    std::unique_ptr<IF_LLVM_Module> if_m = if_p.parse_ll(input_file_path);
    IF_Parser::print_instrs(*(if_m->get_module()));

    return 0;
}
