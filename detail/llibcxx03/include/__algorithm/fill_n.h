//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_FILL_N_H
#define _CCW_LIBCPP___ALGORITHM_FILL_N_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _OutIter, class _Size, class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI _OutIter fill_n(_OutIter __first, _Size __n, const _Tp& __v) {
  for (; __n > 0; --__n, ++__first) *__first = __v;
  return __first;
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
