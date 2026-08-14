//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_FIND_H
#define _CCW_LIBCPP___ALGORITHM_FIND_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _InputIter, class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI _InputIter find(_InputIter __first, _InputIter __last, const _Tp& __v) {
  for (; __first != __last; ++__first) if (*__first == __v) break;
  return __first;
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
