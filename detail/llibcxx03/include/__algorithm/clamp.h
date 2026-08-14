//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_CLAMP_H
#define _CCW_LIBCPP___ALGORITHM_CLAMP_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI const _Tp& clamp(const _Tp& __v, const _Tp& __lo, const _Tp& __hi) {
    return __v < __lo ? __lo : (__hi < __v ? __hi : __v);
}
template <class _Tp, class _Comp>
_CCW_LIBCPP_HIDE_FROM_ABI const _Tp& clamp(const _Tp& __v, const _Tp& __lo, const _Tp& __hi, _Comp __c) {
    return __c(__v, __lo) ? __lo : (__c(__hi, __v) ? __hi : __v);
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
