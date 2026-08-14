//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___NUMERIC_INNER_PRODUCT_H
#define _CCW_LIBCPP___NUMERIC_INNER_PRODUCT_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _It1, class _It2, class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI _Tp inner_product(_It1 __f1, _It1 __l1, _It2 __f2, _Tp __init) {
  for (; __f1 != __l1; ++__f1, ++__f2) __init = __init + (*__f1) * (*__f2);
  return __init;
}
template <class _It1, class _It2, class _Tp, class _BOp1, class _BOp2> _CCW_LIBCPP_HIDE_FROM_ABI _Tp inner_product(_It1 __f1, _It1 __l1, _It2 __f2, _Tp __init, _BOp1 __op1, _BOp2 __op2) {
  for (; __f1 != __l1; ++__f1, ++__f2) __init = __op1(__init, __op2(*__f1, *__f2));
  return __init;
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
