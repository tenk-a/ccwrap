/**
 *  @file   detail/cxx/erase_if_map.hpp
 *  @brief  std::erase_if for <map> (C++20, [map.erasure] / [multimap.erasure]).
 *  @license Boost Software License Version 1.0
 *  @note   The forwarder (vc/std/map, gcc/std/map) includes native <map> first,
 *          then this file. Self-guarded, so including it unconditionally is safe.
 */
#ifndef _CCW_DETAIL_ERASE_IF_MAP_HPP
#define _CCW_DETAIL_ERASE_IF_MAP_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>

#if defined(__cplusplus) && !defined(__cpp_lib_erase_if)
namespace _CCW_STD20 {

template <class _Key, class _Tp, class _Cmp, class _Alloc, class _Pred>
inline typename std::map<_Key, _Tp, _Cmp, _Alloc>::size_type erase_if(std::map<_Key, _Tp, _Cmp, _Alloc>& __c, _Pred __p) {
    typename std::map<_Key, _Tp, _Cmp, _Alloc>::size_type __n = 0;
    for (typename std::map<_Key, _Tp, _Cmp, _Alloc>::iterator __i = __c.begin(); __i != __c.end(); ) {
        if (__p(*__i)) { __c.erase(__i++); ++__n; } else { ++__i; }
    }
    return __n;
}

template <class _Key, class _Tp, class _Cmp, class _Alloc, class _Pred>
inline typename std::multimap<_Key, _Tp, _Cmp, _Alloc>::size_type erase_if(std::multimap<_Key, _Tp, _Cmp, _Alloc>& __c, _Pred __p) {
    typename std::multimap<_Key, _Tp, _Cmp, _Alloc>::size_type __n = 0;
    for (typename std::multimap<_Key, _Tp, _Cmp, _Alloc>::iterator __i = __c.begin(); __i != __c.end(); ) {
        if (__p(*__i)) { __c.erase(__i++); ++__n; } else { ++__i; }
    }
    return __n;
}

}   // _CCW_STD20
#endif

#endif  // _CCW_DETAIL_ERASE_IF_MAP_HPP
