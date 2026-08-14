//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_MAKE_HEAP_H
#define _CCW_LIBCPP___ALGORITHM_MAKE_HEAP_H
#include "../__config"
#include "../__algorithm/pop_heap.h"   // __sift_down
#include "../__iterator/iterator_traits.h"
#include "../__functional/operations.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _RandIt, class _Comp>
_CCW_LIBCPP_HIDE_FROM_ABI void make_heap(_RandIt __first, _RandIt __last, _Comp __comp) {
    typedef typename iterator_traits<_RandIt>::difference_type _Df;
    _Df __len = __last - __first;
    for (_Df __start = __len / 2 - 1; __start >= 0; --__start) _CCW_STD::__sift_down(__first, __start, __len, __comp);
}
template <class _RandIt>
_CCW_LIBCPP_HIDE_FROM_ABI void make_heap(_RandIt __first, _RandIt __last) {
    _CCW_STD::make_heap(__first, __last, less<typename iterator_traits<_RandIt>::value_type>());
}

_CCW_LIBCPP_END_NAMESPACE_STD
#endif
