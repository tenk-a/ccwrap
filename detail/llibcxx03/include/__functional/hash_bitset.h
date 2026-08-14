//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: _CCW_STD::hash
// for bitset (C++11).
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___FUNCTIONAL_HASH_BITSET_H
#define _CCW_LIBCPP___FUNCTIONAL_HASH_BITSET_H
#include "../__config"
#include "hash.h"
#include <cstddef>

#if !_CCW_LIBCPP_HAS_NATIVE_CXX11_LIB

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <_CCW_STD::size_t _Size>
struct _CCW_LIBCPP_TEMPLATE_VIS hash<bitset<_Size> > {
    _CCW_LIBCPP_HIDE_FROM_ABI _CCW_STD::size_t operator()(const bitset<_Size>& __b) const {
        _CCW_STD::size_t __h = 2166136261u;   // FNV-1a over the bits
        for (_CCW_STD::size_t __i = 0; __i < _Size; ++__i) {
            __h ^= (_CCW_STD::size_t)(__b.test(__i) ? 1u : 0u);
            __h *= 16777619u;
        }
        return __h;
    }
};

_CCW_LIBCPP_END_NAMESPACE_STD

#endif // !_CCW_LIBCPP_HAS_NATIVE_CXX11_LIB
#endif // _CCW_LIBCPP___FUNCTIONAL_HASH_BITSET_H
