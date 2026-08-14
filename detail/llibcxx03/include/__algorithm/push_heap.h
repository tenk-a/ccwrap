//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom. Binary max-heap over random-access iterators
// (Comp defaults to less, i.e. a max-heap).
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_PUSH_HEAP_H
#define _CCW_LIBCPP___ALGORITHM_PUSH_HEAP_H
#include "../__config"
#include "../__utility/swap.h"
#include "../__iterator/iterator_traits.h"
#include "../__functional/operations.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _RandIt, class _Diff, class _Comp>
_CCW_LIBCPP_HIDE_FROM_ABI void __sift_up(_RandIt __first, _Diff __len, _Comp __comp) {
    _Diff __child = __len - 1;
    while (__child > 0) {
        _Diff __parent = (__child - 1) / 2;
        if (__comp(__first[__parent], __first[__child])) { _CCW_STD::swap(__first[__parent], __first[__child]); __child = __parent; }
        else break;
    }
}

template <class _RandIt, class _Comp>
_CCW_LIBCPP_HIDE_FROM_ABI void push_heap(_RandIt __first, _RandIt __last, _Comp __comp) {
    _CCW_STD::__sift_up(__first, __last - __first, __comp);
}
template <class _RandIt>
_CCW_LIBCPP_HIDE_FROM_ABI void push_heap(_RandIt __first, _RandIt __last) {
    _CCW_STD::__sift_up(__first, __last - __first, less<typename iterator_traits<_RandIt>::value_type>());
}

_CCW_LIBCPP_END_NAMESPACE_STD
#endif
