/**
 *  @file   detail/cxx/erase_if_deque.hpp
 *  @brief  std::erase / std::erase_if for <deque> (C++20, [deque.erasure]).
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_DETAIL_ERASE_IF_DEQUE_HPP
#define _CCW_DETAIL_ERASE_IF_DEQUE_HPP
#include <ccwrap_common.h>

#if defined(__cplusplus) && !defined(__cpp_lib_erase_if)
#include <algorithm>
#include <deque>

namespace _CCW_STD20 {

template <class _Tp, class _Alloc, class _Up>
inline typename std::deque<_Tp, _Alloc>::size_type erase(std::deque<_Tp, _Alloc>& __c, const _Up& __v) {
    typename std::deque<_Tp, _Alloc>::iterator __it = std::remove(__c.begin(), __c.end(), __v);
    typename std::deque<_Tp, _Alloc>::size_type __n = 0;
    for (typename std::deque<_Tp, _Alloc>::iterator __i = __it; __i != __c.end(); ++__i) ++__n;
    __c.erase(__it, __c.end());
    return __n;
}

template <class _Tp, class _Alloc, class _Pred>
inline typename std::deque<_Tp, _Alloc>::size_type erase_if(std::deque<_Tp, _Alloc>& __c, _Pred __p) {
    typename std::deque<_Tp, _Alloc>::iterator __it = std::remove_if(__c.begin(), __c.end(), __p);
    typename std::deque<_Tp, _Alloc>::size_type __n = 0;
    for (typename std::deque<_Tp, _Alloc>::iterator __i = __it; __i != __c.end(); ++__i) ++__n;
    __c.erase(__it, __c.end());
    return __n;
}

}   // _CCW_STD20
#endif

#endif  /* _CCW_DETAIL_ERASE_IF_DEQUE_HPP */
