//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_SORT_HEAP_H
#define _CCW_LIBCPP___ALGORITHM_SORT_HEAP_H
#include "../__config"
#include "../__algorithm/pop_heap.h"
#include "../__iterator/iterator_traits.h"
#include "../__functional/operations.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _RandIt, class _Comp>
_CCW_LIBCPP_HIDE_FROM_ABI void sort_heap(_RandIt __first, _RandIt __last, _Comp __comp) {
    for (_RandIt __it = __last; __it - __first > 1; --__it) _CCW_STD::pop_heap(__first, __it, __comp);
}
template <class _RandIt>
_CCW_LIBCPP_HIDE_FROM_ABI void sort_heap(_RandIt __first, _RandIt __last) {
    _CCW_STD::sort_heap(__first, __last, less<typename iterator_traits<_RandIt>::value_type>());
}

_CCW_LIBCPP_END_NAMESPACE_STD
#endif
