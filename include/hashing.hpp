#ifndef _IF_HASHING_HPP
#define _IF_HASHING_HPP

#include <array>
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

template <typename T, size_t N> struct hash<array<T, N>>
{
    size_t operator()(array<T, N> const& _arr) const
    {
        size_t seed = 0;
        for (size_t i = 0; i < _arr.size(); ++i)
        {
            ::hash_combine(seed, _arr.at(i));
        }
        return seed;
    }
};

template <typename T> struct hash<vector<T>>
{
    size_t operator()(vector<T> const& _vec) const
    {
        size_t seed = 0;
        for (const auto& v_elem : _vec)
        {
            ::hash_combine(seed, v_elem);
        }
        return seed;
    }
};

}

#include "hashing.tpp"

#endif //_IF_HASHING_HPP
