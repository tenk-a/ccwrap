/*
 * ccwrap: libc++ layout. classic C++03 body.
 */
#ifndef _CCW_LIBCPP___FUNCTIONAL_NOT_FN_H
#define _CCW_LIBCPP___FUNCTIONAL_NOT_FN_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _Fn> struct __not_fn_t {
    _Fn __f_;
    _CCW_LIBCPP_HIDE_FROM_ABI explicit __not_fn_t(_Fn __f) : __f_(__f) {}
    _CCW_LIBCPP_HIDE_FROM_ABI bool operator()() const { return !__f_(); }
    template <class _A0> _CCW_LIBCPP_HIDE_FROM_ABI bool operator()(_A0 __a0) const { return !__f_(__a0); }
    template <class _A0, class _A1> _CCW_LIBCPP_HIDE_FROM_ABI bool operator()(_A0 __a0, _A1 __a1) const { return !__f_(__a0, __a1); }
    template <class _A0, class _A1, class _A2> _CCW_LIBCPP_HIDE_FROM_ABI bool operator()(_A0 __a0, _A1 __a1, _A2 __a2) const { return !__f_(__a0, __a1, __a2); }
};
template <class _Fn> _CCW_LIBCPP_HIDE_FROM_ABI __not_fn_t<_Fn> not_fn(_Fn __f) { return __not_fn_t<_Fn>(__f); }
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
