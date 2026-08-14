//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_TRANSFORM_H
#define _CCW_LIBCPP___ALGORITHM_TRANSFORM_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _InIter, class _OutIter, class _Op>
_CCW_LIBCPP_HIDE_FROM_ABI _OutIter transform(_InIter __first, _InIter __last, _OutIter __d, _Op __op) {
  for (; __first != __last; ++__first, ++__d) *__d = __op(*__first);
  return __d;
}
template <class _InIter1, class _InIter2, class _OutIter, class _BinOp>
_CCW_LIBCPP_HIDE_FROM_ABI _OutIter transform(_InIter1 __f1, _InIter1 __l1, _InIter2 __f2, _OutIter __d, _BinOp __op) {
  for (; __f1 != __l1; ++__f1, ++__f2, ++__d) *__d = __op(*__f1, *__f2);
  return __d;
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
