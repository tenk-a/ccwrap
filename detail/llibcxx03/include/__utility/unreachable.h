//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___UTILITY_UNREACHABLE_H
#define _CCW_LIBCPP___UTILITY_UNREACHABLE_H
#include "../__config"
#include <cstdlib>
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

_CCW_LIBCPP_HIDE_FROM_ABI inline void unreachable() { _CCW_STD::abort(); }

_CCW_LIBCPP_END_NAMESPACE_STD
#endif // _CCW_LIBCPP___UTILITY_UNREACHABLE_H
