// Taken from
// * https://stackoverflow.com/a/27952689
// * https://stackoverflow.com/q/28367913
template <typename T>
inline void
hash_combine(size_t& _hash, T const& _to_combine)
{
    _hash ^= std::hash<T> {}(_to_combine) + 0x9e3779b9 + (_hash << 6)
        + (_hash >> 2);
}
