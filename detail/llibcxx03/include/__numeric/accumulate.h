//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___NUMERIC_ACCUMULATE_H
#define _CCW_LIBCPP___NUMERIC_ACCUMULATE_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _InputIter, class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI _Tp accumulate(_InputIter __first, _InputIter __last, _Tp __init) {
  for (; __first != __last; ++__first) __init = __init + *__first;
  return __init;
}
template <class _InputIter, class _Tp, class _BinOp> _CCW_LIBCPP_HIDE_FROM_ABI _Tp accumulate(_InputIter __first, _InputIter __last, _Tp __init, _BinOp __op) {
  for (; __first != __last; ++__first) __init = __op(__init, *__first);
  return __init;
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
