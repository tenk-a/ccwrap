//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_UNIQUE_H
#define _CCW_LIBCPP___ALGORITHM_UNIQUE_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _FwdIter, class _BinPred>
_CCW_LIBCPP_HIDE_FROM_ABI _FwdIter unique(_FwdIter __first, _FwdIter __last, _BinPred __pred) {
  if (__first == __last) return __last;
  _FwdIter __r = __first;
  while (++__first != __last) if (!__pred(*__r, *__first)) { ++__r; *__r = *__first; }
  ++__r; return __r;
}

template <class _FwdIter>
_CCW_LIBCPP_HIDE_FROM_ABI _FwdIter unique(_FwdIter __first, _FwdIter __last) {
  if (__first == __last) return __last;
  _FwdIter __r = __first;
  while (++__first != __last) if (!(*__r == *__first)) { ++__r; *__r = *__first; }
  ++__r; return __r;
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
