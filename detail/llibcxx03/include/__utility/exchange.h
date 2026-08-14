//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___UTILITY_EXCHANGE_H
#define _CCW_LIBCPP___UTILITY_EXCHANGE_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _Tp, class _Up>
_CCW_LIBCPP_HIDE_FROM_ABI _Tp exchange(_Tp& __obj, const _Up& __new_value) {
    _Tp __old = __obj; __obj = __new_value; return __old;
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
