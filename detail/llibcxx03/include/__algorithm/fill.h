//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_FILL_H
#define _CCW_LIBCPP___ALGORITHM_FILL_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _FwdIter, class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI void fill(_FwdIter __first, _FwdIter __last, const _Tp& __v) {
  for (; __first != __last; ++__first) *__first = __v;
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
