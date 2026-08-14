//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_IS_SORTED_H
#define _CCW_LIBCPP___ALGORITHM_IS_SORTED_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _It>
_CCW_LIBCPP_HIDE_FROM_ABI _It is_sorted_until(_It __f, _It __l) {
    if (__f == __l) return __l;
    _It __n = __f; ++__n;
    for (; __n != __l; ++__f, ++__n) if (*__n < *__f) return __n;
    return __l;
}
template <class _It>
_CCW_LIBCPP_HIDE_FROM_ABI bool is_sorted(_It __f, _It __l) { return _CCW_STD::is_sorted_until(__f, __l) == __l; }

template <class _It, class _Comp>
_CCW_LIBCPP_HIDE_FROM_ABI _It is_sorted_until(_It __f, _It __l, _Comp __comp) {
    if (__f == __l) return __l;
    _It __n = __f; ++__n;
    for (; __n != __l; ++__f, ++__n) if (__comp(*__n, *__f)) return __n;
    return __l;
}
template <class _It, class _Comp>
_CCW_LIBCPP_HIDE_FROM_ABI bool is_sorted(_It __f, _It __l, _Comp __comp) { return _CCW_STD::is_sorted_until(__f, __l, __comp) == __l; }
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
