//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___UTILITY_REL_OPS_H
#define _CCW_LIBCPP___UTILITY_REL_OPS_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

namespace rel_ops {

template <class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI bool operator!=(const _Tp& __x, const _Tp& __y) { return !(__x == __y); }
template <class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI bool operator>(const _Tp& __x, const _Tp& __y) { return __y < __x; }
template <class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI bool operator<=(const _Tp& __x, const _Tp& __y) { return !(__y < __x); }
template <class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI bool operator>=(const _Tp& __x, const _Tp& __y) { return !(__x < __y); }

} // namespace rel_ops

_CCW_LIBCPP_END_NAMESPACE_STD
#endif // _CCW_LIBCPP___UTILITY_REL_OPS_H
