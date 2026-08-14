//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_COUNT_H
#define _CCW_LIBCPP___ALGORITHM_COUNT_H
#include "../__config"
#include "../__iterator/iterator_traits.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _InputIter, class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI typename iterator_traits<_InputIter>::difference_type
count(_InputIter __first, _InputIter __last, const _Tp& __v) {
  typename iterator_traits<_InputIter>::difference_type __r = 0;
  for (; __first != __last; ++__first) if (*__first == __v) ++__r;
  return __r;
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
