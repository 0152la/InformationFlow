#include "config.hpp"

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
