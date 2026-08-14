/*
 * ccwrap: libc++ layout. classic C++03 body.
 *  shift_left / shift_right (C++20). shift_left works for forward iterators;
 *  shift_right needs random-access iterators (covers the port's vector/deque/
 *  array/string). Elements are copied (no move on C++03).
 */
#ifndef _CCW_LIBCPP___ALGORITHM_SHIFT_H
#define _CCW_LIBCPP___ALGORITHM_SHIFT_H
#include "../__config"
#include "../__iterator/iterator_traits.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _FwdIt, class _Size>
_CCW_LIBCPP_HIDE_FROM_ABI _FwdIt shift_left(_FwdIt __first, _FwdIt __last, _Size __n) {
    if (__n <= 0) return __last;
    _FwdIt __src = __first;
    for (_Size __i = 0; __i < __n; ++__i) { if (__src == __last) return __first; ++__src; }
    _FwdIt __dst = __first;
    for (; __src != __last; ++__src, ++__dst) *__dst = *__src;
    return __dst;
}
template <class _RandIt, class _Size>
_CCW_LIBCPP_HIDE_FROM_ABI _RandIt shift_right(_RandIt __first, _RandIt __last, _Size __n) {
    if (__n <= 0) return __first;
    typename iterator_traits<_RandIt>::difference_type __sz = __last - __first;
    if ((typename iterator_traits<_RandIt>::difference_type)__n >= __sz) return __last;
    for (typename iterator_traits<_RandIt>::difference_type __i = __sz - 1; __i >= (typename iterator_traits<_RandIt>::difference_type)__n; --__i)
        __first[__i] = __first[__i - __n];
    return __first + __n;
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
