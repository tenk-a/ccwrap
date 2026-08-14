//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_PARTIAL_SORT_H
#define _CCW_LIBCPP___ALGORITHM_PARTIAL_SORT_H
#include "../__config"
#include "../__algorithm/make_heap.h"
#include "../__algorithm/sort_heap.h"
#include "../__algorithm/pop_heap.h"
#include "../__utility/swap.h"
#include "../__functional/operations.h"
#include "../__iterator/iterator_traits.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _RandIt, class _Compare>
_CCW_LIBCPP_HIDE_FROM_ABI void partial_sort(_RandIt __first, _RandIt __mid, _RandIt __last, _Compare __c) {
    if (__first == __mid) return;
    _CCW_STD::make_heap(__first, __mid, __c);
    for (_RandIt __i = __mid; __i != __last; ++__i)
        if (__c(*__i, *__first)) { _CCW_STD::swap(*__i, *__first); _CCW_STD::__sift_down(__first, (typename iterator_traits<_RandIt>::difference_type)0, __mid - __first, __c); }
    _CCW_STD::sort_heap(__first, __mid, __c);
}

template <class _RandIt>
_CCW_LIBCPP_HIDE_FROM_ABI void partial_sort(_RandIt __first, _RandIt __mid, _RandIt __last) {
    _CCW_STD::partial_sort(__first, __mid, __last, less<typename iterator_traits<_RandIt>::value_type>());
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
