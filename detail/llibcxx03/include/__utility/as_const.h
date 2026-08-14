//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___UTILITY_AS_CONST_H
#define _CCW_LIBCPP___UTILITY_AS_CONST_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI const _Tp& as_const(_Tp& __t) { return __t; }
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
