//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_MAX_ELEMENT_H
#define _CCW_LIBCPP___ALGORITHM_MAX_ELEMENT_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _FwdIter, class _Compare>
_CCW_LIBCPP_HIDE_FROM_ABI _FwdIter max_element(_FwdIter __first, _FwdIter __last, _Compare __c) {
  if (__first == __last) return __last;
  _FwdIter __m = __first;
  for (++__first; __first != __last; ++__first) if (__c(*__m, *__first)) __m = __first;
  return __m;
}

template <class _FwdIter> _CCW_LIBCPP_HIDE_FROM_ABI _FwdIter max_element(_FwdIter __first, _FwdIter __last) {
  if (__first == __last) return __last;
  _FwdIter __m = __first;
  for (++__first; __first != __last; ++__first) if (*__m < *__first) __m = __first;
  return __m;
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
