#include "entropy_map.hpp"
#include "reader.hpp"
#include "entropy_map_graph.hpp"

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
    const char* dest_dir = get_env_var("IF_BUILD_DIR");
    const char* out_file = "ll-out.dot";

    IF_Parser if_p;
    std::unique_ptr<IF_LLVM_Module> if_m = if_p.parse_ll(input_file_path);

    IF_Randgen if_rng(42);
    IF_Emulator if_emu(llvm_snippet_lib_path);
    IF_FuzzEngine if_fe(if_rng, if_emu);

    std::unique_ptr<IF_EntropyMap::Map> if_em
        = if_p.make_entropy_map(*if_m->get_module(), if_fe);

    char* dest_file = (char*) malloc(strlen(dest_dir) + strlen(out_file) + 1);
    strcpy(dest_file, dest_dir);
    strcat(dest_file, out_file);
    IF_EM_Graph if_g(*if_em, dest_file);
    if_g.draw_graph();
    std::cout << "Written `dot` graph to " << dest_file << '\n';
    free(dest_file);

    return 0;
}
