//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___UTILITY_PIECEWISE_CONSTRUCT_H
#define _CCW_LIBCPP___UTILITY_PIECEWISE_CONSTRUCT_H
#include "../__config"
#if _CCW_LIBCPP_NATIVE_STD
#  include <utility>
#else
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

struct piecewise_construct_t { _CCW_LIBCPP_HIDE_FROM_ABI explicit piecewise_construct_t() {} };
_ccw_inline_const piecewise_construct_t piecewise_construct = piecewise_construct_t();

_CCW_LIBCPP_END_NAMESPACE_STD
#endif // _CCW_LIBCPP_NATIVE_STD
#endif // _CCW_LIBCPP___UTILITY_PIECEWISE_CONSTRUCT_H
