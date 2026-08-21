/**
 *  @file   detail/cxx/erase_if_set.hpp
 *  @brief  std::erase_if for <set> (C++20, [set.erasure] / [multiset.erasure]).
 *  @license Boost Software License Version 1.0
 *  @note   The forwarder (vc/std/set, gcc/std/set) includes native <set> first,
 *          then this file. Self-guarded, so including it unconditionally is safe.
 */
#ifndef _CCW_DETAIL_ERASE_IF_SET_HPP
#define _CCW_DETAIL_ERASE_IF_SET_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>

#if defined(__cplusplus) && !defined(__cpp_lib_erase_if)
namespace _CCW_STD20 {

template <class _Key, class _Cmp, class _Alloc, class _Pred>
inline typename std::set<_Key, _Cmp, _Alloc>::size_type erase_if(std::set<_Key, _Cmp, _Alloc>& __c, _Pred __p) {
    typename std::set<_Key, _Cmp, _Alloc>::size_type __n = 0;
    for (typename std::set<_Key, _Cmp, _Alloc>::iterator __i = __c.begin(); __i != __c.end(); ) {
        if (__p(*__i)) { __c.erase(__i++); ++__n; } else { ++__i; }
    }
    return __n;
}

template <class _Key, class _Cmp, class _Alloc, class _Pred>
inline typename std::multiset<_Key, _Cmp, _Alloc>::size_type erase_if(std::multiset<_Key, _Cmp, _Alloc>& __c, _Pred __p) {
    typename std::multiset<_Key, _Cmp, _Alloc>::size_type __n = 0;
    for (typename std::multiset<_Key, _Cmp, _Alloc>::iterator __i = __c.begin(); __i != __c.end(); ) {
        if (__p(*__i)) { __c.erase(__i++); ++__n; } else { ++__i; }
    }
    return __n;
}

}   // _CCW_STD20
#endif

#endif  // _CCW_DETAIL_ERASE_IF_SET_HPP
