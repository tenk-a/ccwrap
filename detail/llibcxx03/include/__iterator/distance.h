//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ITERATOR_DISTANCE_H
#define _CCW_LIBCPP___ITERATOR_DISTANCE_H
#include "../__config"
#include "../__iterator/iterator_traits.h"
#if !_CCW_LIBCPP_NATIVE_STD

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _InputIter>
_CCW_LIBCPP_HIDE_FROM_ABI typename iterator_traits<_InputIter>::difference_type
__distance(_InputIter __first, _InputIter __last, input_iterator_tag) {
    typename iterator_traits<_InputIter>::difference_type __r = 0;
    for (; __first != __last; ++__first) ++__r;
    return __r;
}

template <class _RandIter>
_CCW_LIBCPP_HIDE_FROM_ABI typename iterator_traits<_RandIter>::difference_type
__distance(_RandIter __first, _RandIter __last, random_access_iterator_tag)
{ return __last - __first; }

template <class _InputIter>
_CCW_LIBCPP_HIDE_FROM_ABI typename iterator_traits<_InputIter>::difference_type
distance(_InputIter __first, _InputIter __last)
{ return _CCW_STD::__distance(__first, __last, typename iterator_traits<_InputIter>::iterator_category()); }

_CCW_LIBCPP_END_NAMESPACE_STD

#endif // !_CCW_LIBCPP_NATIVE_STD
#endif
