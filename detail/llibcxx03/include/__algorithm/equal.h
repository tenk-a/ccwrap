//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_EQUAL_H
#define _CCW_LIBCPP___ALGORITHM_EQUAL_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _InIter1, class _InIter2>
_CCW_LIBCPP_HIDE_FROM_ABI bool equal(_InIter1 __f1, _InIter1 __l1, _InIter2 __f2) {
  for (; __f1 != __l1; ++__f1, ++__f2) if (!(*__f1 == *__f2)) return false;
  return true;
}
template <class _InIter1, class _InIter2, class _Pred>
_CCW_LIBCPP_HIDE_FROM_ABI bool equal(_InIter1 __f1, _InIter1 __l1, _InIter2 __f2, _Pred __p) {
  for (; __f1 != __l1; ++__f1, ++__f2) if (!__p(*__f1, *__f2)) return false;
  return true;
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
