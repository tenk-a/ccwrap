//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___FUNCTIONAL_MEM_FN_H
#define _CCW_LIBCPP___FUNCTIONAL_MEM_FN_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Rp, class _Cp> class __mem_fn0 {
    _Rp (_Cp::*__p_)();
public:
    _CCW_LIBCPP_HIDE_FROM_ABI __mem_fn0(_Rp (_Cp::*__p)()) : __p_(__p) {}
    _CCW_LIBCPP_HIDE_FROM_ABI _Rp operator()(_Cp& __o) const { return (__o.*__p_)(); }
    _CCW_LIBCPP_HIDE_FROM_ABI _Rp operator()(_Cp* __o) const { return (__o->*__p_)(); }
};
template <class _Rp, class _Cp> class __mem_fn0c {
    _Rp (_Cp::*__p_)() const;
public:
    _CCW_LIBCPP_HIDE_FROM_ABI __mem_fn0c(_Rp (_Cp::*__p)() const) : __p_(__p) {}
    _CCW_LIBCPP_HIDE_FROM_ABI _Rp operator()(const _Cp& __o) const { return (__o.*__p_)(); }
    _CCW_LIBCPP_HIDE_FROM_ABI _Rp operator()(const _Cp* __o) const { return (__o->*__p_)(); }
};
template <class _Rp, class _Cp> _CCW_LIBCPP_HIDE_FROM_ABI __mem_fn0<_Rp, _Cp>  mem_fn(_Rp (_Cp::*__p)())       { return __mem_fn0<_Rp, _Cp>(__p); }
template <class _Rp, class _Cp> _CCW_LIBCPP_HIDE_FROM_ABI __mem_fn0c<_Rp, _Cp> mem_fn(_Rp (_Cp::*__p)() const) { return __mem_fn0c<_Rp, _Cp>(__p); }

template <class _Rp, class _Cp, class _A0> class __mem_fn1 {
    _Rp (_Cp::*__p_)(_A0);
public:
    _CCW_LIBCPP_HIDE_FROM_ABI __mem_fn1(_Rp (_Cp::*__p)(_A0)) : __p_(__p) {}
    _CCW_LIBCPP_HIDE_FROM_ABI _Rp operator()(_Cp& __o, _A0 __a0) const { return (__o.*__p_)(__a0); }
    _CCW_LIBCPP_HIDE_FROM_ABI _Rp operator()(_Cp* __o, _A0 __a0) const { return (__o->*__p_)(__a0); }
};
template <class _Rp, class _Cp, class _A0> class __mem_fn1c {
    _Rp (_Cp::*__p_)(_A0) const;
public:
    _CCW_LIBCPP_HIDE_FROM_ABI __mem_fn1c(_Rp (_Cp::*__p)(_A0) const) : __p_(__p) {}
    _CCW_LIBCPP_HIDE_FROM_ABI _Rp operator()(const _Cp& __o, _A0 __a0) const { return (__o.*__p_)(__a0); }
    _CCW_LIBCPP_HIDE_FROM_ABI _Rp operator()(const _Cp* __o, _A0 __a0) const { return (__o->*__p_)(__a0); }
};
template <class _Rp, class _Cp, class _A0> _CCW_LIBCPP_HIDE_FROM_ABI __mem_fn1<_Rp, _Cp, _A0>  mem_fn(_Rp (_Cp::*__p)(_A0))       { return __mem_fn1<_Rp, _Cp, _A0>(__p); }
template <class _Rp, class _Cp, class _A0> _CCW_LIBCPP_HIDE_FROM_ABI __mem_fn1c<_Rp, _Cp, _A0> mem_fn(_Rp (_Cp::*__p)(_A0) const) { return __mem_fn1c<_Rp, _Cp, _A0>(__p); }

template <class _Rp, class _Cp, class _A0, class _A1> class __mem_fn2 {
    _Rp (_Cp::*__p_)(_A0, _A1);
public:
    _CCW_LIBCPP_HIDE_FROM_ABI __mem_fn2(_Rp (_Cp::*__p)(_A0, _A1)) : __p_(__p) {}
    _CCW_LIBCPP_HIDE_FROM_ABI _Rp operator()(_Cp& __o, _A0 __a0, _A1 __a1) const { return (__o.*__p_)(__a0, __a1); }
    _CCW_LIBCPP_HIDE_FROM_ABI _Rp operator()(_Cp* __o, _A0 __a0, _A1 __a1) const { return (__o->*__p_)(__a0, __a1); }
};
template <class _Rp, class _Cp, class _A0, class _A1> _CCW_LIBCPP_HIDE_FROM_ABI __mem_fn2<_Rp, _Cp, _A0, _A1> mem_fn(_Rp (_Cp::*__p)(_A0, _A1)) { return __mem_fn2<_Rp, _Cp, _A0, _A1>(__p); }

#if !defined(__WATCOMC__)
template <class _Rp, class _Cp> class __mem_fn_data {
    _Rp _Cp::*__p_;
public:
    _CCW_LIBCPP_HIDE_FROM_ABI __mem_fn_data(_Rp _Cp::*__p) : __p_(__p) {}
    _CCW_LIBCPP_HIDE_FROM_ABI _Rp&       operator()(_Cp& __o)        const { return __o.*__p_; }
    _CCW_LIBCPP_HIDE_FROM_ABI const _Rp& operator()(const _Cp& __o)  const { return __o.*__p_; }
    _CCW_LIBCPP_HIDE_FROM_ABI _Rp&       operator()(_Cp* __o)        const { return __o->*__p_; }
    _CCW_LIBCPP_HIDE_FROM_ABI const _Rp& operator()(const _Cp* __o)  const { return __o->*__p_; }
};
template <class _Rp, class _Cp> _CCW_LIBCPP_HIDE_FROM_ABI __mem_fn_data<_Rp, _Cp> mem_fn(_Rp _Cp::*__p) { return __mem_fn_data<_Rp, _Cp>(__p); }
#endif

_CCW_LIBCPP_END_NAMESPACE_STD
#endif
