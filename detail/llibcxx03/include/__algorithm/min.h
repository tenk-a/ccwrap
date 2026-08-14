//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_MIN_H
#define _CCW_LIBCPP___ALGORITHM_MIN_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI const _Tp& min(const _Tp& __a, const _Tp& __b) { return __b < __a ? __b : __a; }
template <class _Tp, class _Compare> _CCW_LIBCPP_HIDE_FROM_ABI const _Tp& min(const _Tp& __a, const _Tp& __b, _Compare __c) { return __c(__b, __a) ? __b : __a; }
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
