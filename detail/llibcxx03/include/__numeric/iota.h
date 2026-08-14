//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___NUMERIC_IOTA_H
#define _CCW_LIBCPP___NUMERIC_IOTA_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _FwdIter, class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI void iota(_FwdIter __first, _FwdIter __last, _Tp __val_) {
  for (; __first != __last; ++__first, ++__val_) *__first = __val_;
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
