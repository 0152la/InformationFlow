template <typename T, typename Tc>
void
IF_Emulator::populate_fn_def(const fn_def& to_pop)
{
    T fn = (Tc) dlsym(this->ll_snippet_handler, to_pop.fn_name.c_str());
    if (const char* err = dlerror())
    {
        throw std::runtime_error("Error in `dlsym` for " + to_pop.fn_name
            + "\n\t >> " + std::string { err } + "\n");
    }

    this->emulated_fnss.emplace(to_pop.fn_name, fn);
}
