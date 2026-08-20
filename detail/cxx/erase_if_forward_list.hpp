/**
 *  @file   detail/cxx/erase_if_forward_list.hpp
 *  @brief  std::erase / std::erase_if for <forward_list> (C++20, [forward_list.erasure]).
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_DETAIL_ERASE_IF_FORWARD_LIST_HPP
#define _CCW_DETAIL_ERASE_IF_FORWARD_LIST_HPP
#include <ccwrap_common.h>

#if defined(__cplusplus) && !defined(__cpp_lib_erase_if)
#include <algorithm>
#include <forward_list>

namespace _CCW_STD20 {

template <class _Cont>
inline typename _Cont::size_type __ccw_flist_len(const _Cont& __c) {
    typename _Cont::size_type __n = 0;
    for (typename _Cont::const_iterator __i = __c.begin(); __i != __c.end(); ++__i) ++__n;
    return __n;
}

template <class _Tp, class _Alloc, class _Up>
inline typename std::forward_list<_Tp, _Alloc>::size_type erase(std::forward_list<_Tp, _Alloc>& __c, const _Up& __v) {
    typename std::forward_list<_Tp, _Alloc>::size_type __before = __ccw_flist_len(__c);
    __c.remove(__v);
    return __before - __ccw_flist_len(__c);
}

template <class _Tp, class _Alloc, class _Pred>
inline typename std::forward_list<_Tp, _Alloc>::size_type erase_if(std::forward_list<_Tp, _Alloc>& __c, _Pred __p) {
    typename std::forward_list<_Tp, _Alloc>::size_type __before = __ccw_flist_len(__c);
    __c.remove_if(__p);
    return __before - __ccw_flist_len(__c);
}

}   // _CCW_STD20
#endif

#endif  /* _CCW_DETAIL_ERASE_IF_FORWARD_LIST_HPP */
