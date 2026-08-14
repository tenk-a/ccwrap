//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_FIND_IF_H
#define _CCW_LIBCPP___ALGORITHM_FIND_IF_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _InputIter, class _Pred> _CCW_LIBCPP_HIDE_FROM_ABI _InputIter find_if(_InputIter __first, _InputIter __last, _Pred __p) {
  for (; __first != __last; ++__first) if (__p(*__first)) break;
  return __first;
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
