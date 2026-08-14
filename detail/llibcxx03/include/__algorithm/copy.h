//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_COPY_H
#define _CCW_LIBCPP___ALGORITHM_COPY_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _InputIter, class _OutputIter> _CCW_LIBCPP_HIDE_FROM_ABI _OutputIter copy(_InputIter __first, _InputIter __last, _OutputIter __d) {
  for (; __first != __last; ++__first, ++__d) *__d = *__first;
  return __d;
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
