template <std::integral T>
T
IF_Randgen::gen_int(std::uniform_int_distribution<T>& d)
{
    return d(this->random_engine);
}
