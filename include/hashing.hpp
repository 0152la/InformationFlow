#ifndef _IF_HASHING_HPP
#define _IF_HASHING_HPP

#include <cstdlib>
#include <functional>
#include <utility>

template <typename T>
inline void
hash_combine(size_t&, T const&);

namespace std
{
template <typename T1, typename T2> struct hash<pair<T1, T2>>
{
    size_t operator()(pair<T1, T2> const& _p) const
    {
        size_t seed = 0;
        ::hash_combine(seed, _p.first);
        ::hash_combine(seed, _p.second);
        return seed;
    }
};
}

#include "hashing.tpp"

#endif //_IF_HASHING_HPP
