//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___FUNCTIONAL_INVOKE_H
#define _CCW_LIBCPP___FUNCTIONAL_INVOKE_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Fp> struct __bind_rc { typedef typename _Fp::result_type type; };
template <class _Rp> struct __bind_rc<_Rp (*)()> { typedef _Rp type; };
template <class _Rp, class _P0> struct __bind_rc<_Rp (*)(_P0)> { typedef _Rp type; };
template <class _Rp, class _P0, class _P1> struct __bind_rc<_Rp (*)(_P0, _P1)> { typedef _Rp type; };
template <class _Rp, class _P0, class _P1, class _P2> struct __bind_rc<_Rp (*)(_P0, _P1, _P2)> { typedef _Rp type; };
template <class _Rp, class _Cp> struct __bind_rc<_Rp (_Cp::*)()> { typedef _Rp type; };
template <class _Rp, class _Cp> struct __bind_rc<_Rp (_Cp::*)() const> { typedef _Rp type; };
template <class _Rp, class _Cp, class _P0> struct __bind_rc<_Rp (_Cp::*)(_P0)> { typedef _Rp type; };
template <class _Rp, class _Cp, class _P0> struct __bind_rc<_Rp (_Cp::*)(_P0) const> { typedef _Rp type; };

template <class _Rp> _CCW_LIBCPP_HIDE_FROM_ABI _Rp invoke(_Rp (*__f)()) { return __f(); }
template <class _Rp, class _A0> _CCW_LIBCPP_HIDE_FROM_ABI _Rp invoke(_Rp (*__f)(_A0), _A0 __a0) { return __f(__a0); }
template <class _Rp, class _A0, class _A1> _CCW_LIBCPP_HIDE_FROM_ABI _Rp invoke(_Rp (*__f)(_A0, _A1), _A0 __a0, _A1 __a1) { return __f(__a0, __a1); }
template <class _Rp, class _A0, class _A1, class _A2> _CCW_LIBCPP_HIDE_FROM_ABI _Rp invoke(_Rp (*__f)(_A0, _A1, _A2), _A0 __a0, _A1 __a1, _A2 __a2) { return __f(__a0, __a1, __a2); }

template <class _Rp, class _Cp> _CCW_LIBCPP_HIDE_FROM_ABI _Rp invoke(_Rp (_Cp::*__f)(), _Cp& __o) { return (__o.*__f)(); }
template <class _Rp, class _Cp> _CCW_LIBCPP_HIDE_FROM_ABI _Rp invoke(_Rp (_Cp::*__f)() const, const _Cp& __o) { return (__o.*__f)(); }
template <class _Rp, class _Cp, class _A0> _CCW_LIBCPP_HIDE_FROM_ABI _Rp invoke(_Rp (_Cp::*__f)(_A0), _Cp& __o, _A0 __a0) { return (__o.*__f)(__a0); }
template <class _Rp, class _Cp, class _A0> _CCW_LIBCPP_HIDE_FROM_ABI _Rp invoke(_Rp (_Cp::*__f)(_A0) const, const _Cp& __o, _A0 __a0) { return (__o.*__f)(__a0); }
template <class _Rp, class _Cp, class _A0, class _A1> _CCW_LIBCPP_HIDE_FROM_ABI _Rp invoke(_Rp (_Cp::*__f)(_A0, _A1), _Cp& __o, _A0 __a0, _A1 __a1) { return (__o.*__f)(__a0, __a1); }

template <class _Rp, class _Cp> _CCW_LIBCPP_HIDE_FROM_ABI _Rp& invoke(_Rp _Cp::*__m, _Cp& __o) { return __o.*__m; }
template <class _Rp, class _Cp> _CCW_LIBCPP_HIDE_FROM_ABI const _Rp& invoke(_Rp _Cp::*__m, const _Cp& __o) { return __o.*__m; }
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
