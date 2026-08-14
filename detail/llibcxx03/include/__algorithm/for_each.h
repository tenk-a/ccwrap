//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_FOR_EACH_H
#define _CCW_LIBCPP___ALGORITHM_FOR_EACH_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _InputIter, class _Func> _CCW_LIBCPP_HIDE_FROM_ABI _Func for_each(_InputIter __first, _InputIter __last, _Func __f) {
  for (; __first != __last; ++__first) __f(*__first);
  return __f;
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
