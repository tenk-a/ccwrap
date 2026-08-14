/*
 * ccwrap: libc++ layout. classic C++03 body.
 */
#ifndef _CCW_LIBCPP___ALGORITHM_INPLACE_MERGE_H
#define _CCW_LIBCPP___ALGORITHM_INPLACE_MERGE_H
#include "../__config"
#include "../__iterator/iterator_traits.h"
#include "../__functional/operations.h"
#include <vector>
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _BidIt, class _Comp>
_CCW_LIBCPP_HIDE_FROM_ABI void inplace_merge(_BidIt __first, _BidIt __mid, _BidIt __last, _Comp __comp) {
    typedef typename iterator_traits<_BidIt>::value_type _Vp;
    _CCW_STD::vector<_Vp> __tmp;
    _BidIt __i = __first, __j = __mid;
    while (__i != __mid && __j != __last) {
        if (__comp(*__j, *__i)) { __tmp.push_back(*__j); ++__j; }
        else                    { __tmp.push_back(*__i); ++__i; }
    }
    for (; __i != __mid;  ++__i) __tmp.push_back(*__i);
    for (; __j != __last; ++__j) __tmp.push_back(*__j);
    _BidIt __o = __first;
    for (_CCW_STD::size_t __k = 0; __k < __tmp.size(); ++__k, ++__o) *__o = __tmp[__k];
}
template <class _BidIt>
_CCW_LIBCPP_HIDE_FROM_ABI void inplace_merge(_BidIt __first, _BidIt __mid, _BidIt __last) {
    _CCW_STD::inplace_merge(__first, __mid, __last, _CCW_STD::less<typename iterator_traits<_BidIt>::value_type>());
}

_CCW_LIBCPP_END_NAMESPACE_STD
#endif
