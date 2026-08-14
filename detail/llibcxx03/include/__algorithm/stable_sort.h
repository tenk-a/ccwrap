/*
 * ccwrap: libc++ layout. classic C++03 body.
 */
#ifndef _CCW_LIBCPP___ALGORITHM_STABLE_SORT_H
#define _CCW_LIBCPP___ALGORITHM_STABLE_SORT_H
#include "../__config"
#include "../__iterator/iterator_traits.h"
#include "../__functional/operations.h"
#include <vector>
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _RandIt, class _Comp>
_CCW_LIBCPP_HIDE_FROM_ABI void __stable_sort_impl(_RandIt __first, _RandIt __last, _Comp __comp) {
    typedef typename iterator_traits<_RandIt>::value_type _Vp;
    typedef typename iterator_traits<_RandIt>::difference_type _Dp;
    _Dp __n = __last - __first;
    if (__n < 2) return;
    _RandIt __mid = __first + __n / 2;
    _CCW_STD::__stable_sort_impl(__first, __mid, __comp);
    _CCW_STD::__stable_sort_impl(__mid, __last, __comp);
    _CCW_STD::vector<_Vp> __tmp; __tmp.reserve((_CCW_STD::size_t)__n);
    _RandIt __i = __first, __j = __mid;
    while (__i != __mid && __j != __last) {
        if (__comp(*__j, *__i)) { __tmp.push_back(*__j); ++__j; }   // strict: keep left on ties -> stable
        else                    { __tmp.push_back(*__i); ++__i; }
    }
    for (; __i != __mid;  ++__i) __tmp.push_back(*__i);
    for (; __j != __last; ++__j) __tmp.push_back(*__j);
    _RandIt __o = __first;
    for (_CCW_STD::size_t __k = 0; __k < __tmp.size(); ++__k, ++__o) *__o = __tmp[__k];
}
template <class _RandIt, class _Comp>
_CCW_LIBCPP_HIDE_FROM_ABI void stable_sort(_RandIt __first, _RandIt __last, _Comp __comp) { _CCW_STD::__stable_sort_impl(__first, __last, __comp); }
template <class _RandIt>
_CCW_LIBCPP_HIDE_FROM_ABI void stable_sort(_RandIt __first, _RandIt __last) {
    _CCW_STD::__stable_sort_impl(__first, __last, _CCW_STD::less<typename iterator_traits<_RandIt>::value_type>());
}

_CCW_LIBCPP_END_NAMESPACE_STD
#endif
