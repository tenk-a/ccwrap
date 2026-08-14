/*
 * ccwrap: libc++ layout. classic C++03 body.
 */
#ifndef _CCW_LIBCPP___ALGORITHM_POP_HEAP_H
#define _CCW_LIBCPP___ALGORITHM_POP_HEAP_H
#include "../__config"
#include "../__utility/swap.h"
#include "../__iterator/iterator_traits.h"
#include "../__functional/operations.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _RandIt, class _Diff, class _Comp>
_CCW_LIBCPP_HIDE_FROM_ABI void __sift_down(_RandIt __first, _Diff __start, _Diff __len, _Comp __comp) {
    _Diff __root = __start;
    for (;;) {
        _Diff __child = 2 * __root + 1;
        if (__child >= __len) break;
        if (__child + 1 < __len && __comp(__first[__child], __first[__child + 1])) ++__child;
        if (__comp(__first[__root], __first[__child])) { _CCW_STD::swap(__first[__root], __first[__child]); __root = __child; }
        else break;
    }
}

template <class _RandIt, class _Comp>
_CCW_LIBCPP_HIDE_FROM_ABI void pop_heap(_RandIt __first, _RandIt __last, _Comp __comp) {
    typedef typename iterator_traits<_RandIt>::difference_type _Df;
    _Df __len = __last - __first;
    if (__len > 1) { _CCW_STD::swap(__first[0], __first[__len - 1]); _CCW_STD::__sift_down(__first, _Df(0), _Df(__len - 1), __comp); }
}
template <class _RandIt>
_CCW_LIBCPP_HIDE_FROM_ABI void pop_heap(_RandIt __first, _RandIt __last) {
    _CCW_STD::pop_heap(__first, __last, less<typename iterator_traits<_RandIt>::value_type>());
}

_CCW_LIBCPP_END_NAMESPACE_STD
#endif
