//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: _CCW_STD::hash
// for basic_string.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___FUNCTIONAL_HASH_STRING_H
#define _CCW_LIBCPP___FUNCTIONAL_HASH_STRING_H
#include "../__config"
#include "hash.h"
#include <cstddef>

#if !_CCW_LIBCPP_HAS_NATIVE_CXX11_LIB

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _CharT, class _Traits, class _Alloc>
struct _CCW_LIBCPP_TEMPLATE_VIS hash<basic_string<_CharT, _Traits, _Alloc> > {
    _CCW_LIBCPP_HIDE_FROM_ABI _CCW_STD::size_t operator()(const basic_string<_CharT, _Traits, _Alloc>& __s) const {
        _CCW_STD::size_t __h = 2166136261u;   // FNV-1a
        for (_CCW_STD::size_t __i = 0; __i < __s.size(); ++__i) {
            __h ^= (_CCW_STD::size_t)__s[__i];   // whole character: wide strings hash on all bits
            __h *= 16777619u;
        }
        return __h;
    }
};

_CCW_LIBCPP_END_NAMESPACE_STD

#endif // !_CCW_LIBCPP_HAS_NATIVE_CXX11_LIB
#endif // _CCW_LIBCPP___FUNCTIONAL_HASH_STRING_H
