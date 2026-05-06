#include "config.hpp"

/*******************************************************************************
 * Config
 ******************************************************************************/

namespace config
{

inline const std::unordered_map<std::string, std::string> extension_map {
    { "def", "def" }, { "header", "h" }, { "header_cpp", "hpp" },
    { "llvm_impl", "ll" }
};

const std::string
make_path(const std::string& target)
{
    if (extension_map.count(target) == 0)
    {
        std::ostringstream err_msg_ss;
        err_msg_ss << "Invalid target `" << target
                   << "` for path change given!";
        throw std::runtime_error(err_msg_ss.str());
    }

    std::filesystem::path new_path(snippets_lib_path);
    new_path.replace_extension(extension_map.at(target));
    new_path.replace_filename(
        new_path.filename().string().substr(strlen("lib")));
    return new_path.string();
}

const std::string
make_snippets_def_path(void)
{
    std::filesystem::path def_path(snippets_lib_path);
    def_path.replace_extension("def");
    def_path.replace_filename(
        def_path.filename().string().substr(strlen("lib")));
    return def_path.string();
}

const std::string
make_snippets_header_path(void)
{
    std::filesystem::path header_path(snippets_lib_path);
    header_path.replace_extension("h");
    header_path.replace_filename(
        header_path.filename().string().substr(strlen("lib")));
    return header_path.string();
}

const std::string
make_snippets_header_path_cpp(void)
{
    std::filesystem::path header_path(snippets_lib_path);
    header_path.replace_extension("hpp");
    header_path.replace_filename(
        header_path.filename().string().substr(strlen("lib")));
    return header_path.string();
}

const std::string
make_snippets_ll_path(void)
{
    std::filesystem::path ll_path(snippets_lib_path);
    ll_path.replace_extension("ll");
    ll_path.replace_filename(ll_path.filename().string().substr(strlen("lib")));
    return ll_path.string();
}

}

/*******************************************************************************
 * Utils
 ******************************************************************************/

namespace Utils
{

void
debug_print(std::string_view msg)
{
    fmt::print(fmt::fg(fmt::color::red) | fmt::emphasis::bold, "[DEBUG] ");
    std::cout << msg << std::endl;
}

}
