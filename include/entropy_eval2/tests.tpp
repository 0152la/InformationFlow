template <typename T>
void
check_result(T res, T expected, std::string_view test_src)
{
    if (res != expected)
    {
        throw std::runtime_error(fmt::format(
            "`{}`: invalid result {} - expected {}!", test_src, res, expected));
    }
}

template <typename T>
void
check_result_ge(T res, T expected, std::string_view test_src)
{
    if (res < expected)
    {
        throw std::runtime_error(
            fmt::format("`{}`: invalid result {} - expected above {}!",
                test_src, res, expected));
    }
}
