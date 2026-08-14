//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___FUNCTIONAL_REFERENCE_WRAPPER_H
#define _CCW_LIBCPP___FUNCTIONAL_REFERENCE_WRAPPER_H
#include "../__config"
#include "../__functional/invoke.h"   // invoke + __bind_rc (for operator())
#if !_CCW_LIBCPP_NATIVE_STD

_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _Tp>
class reference_wrapper {
    _Tp* __p_;
public:
    typedef _Tp type;
    _CCW_LIBCPP_HIDE_FROM_ABI reference_wrapper(_Tp& __t) : __p_(&__t) {}
    _CCW_LIBCPP_HIDE_FROM_ABI operator _Tp&() const { return *__p_; }
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp& get() const { return *__p_; }

    _CCW_LIBCPP_HIDE_FROM_ABI typename __bind_rc<_Tp>::type operator()() const { return (*__p_)(); }
    template <class _A0>
    _CCW_LIBCPP_HIDE_FROM_ABI typename __bind_rc<_Tp>::type operator()(_A0 __a0) const { return (*__p_)(__a0); }
    template <class _A0, class _A1>
    _CCW_LIBCPP_HIDE_FROM_ABI typename __bind_rc<_Tp>::type operator()(_A0 __a0, _A1 __a1) const { return (*__p_)(__a0, __a1); }
    template <class _A0, class _A1, class _A2>
    _CCW_LIBCPP_HIDE_FROM_ABI typename __bind_rc<_Tp>::type operator()(_A0 __a0, _A1 __a1, _A2 __a2) const { return (*__p_)(__a0, __a1, __a2); }
};
template <class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI reference_wrapper<_Tp>       ref(_Tp& __t)  { return reference_wrapper<_Tp>(__t); }
template <class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI reference_wrapper<const _Tp> cref(const _Tp& __t) { return reference_wrapper<const _Tp>(__t); }
template <class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI reference_wrapper<_Tp>       ref(reference_wrapper<_Tp> __t)  { return __t; }
template <class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI reference_wrapper<const _Tp> cref(reference_wrapper<_Tp> __t) { return cref(__t.get()); }
_CCW_LIBCPP_END_NAMESPACE_STD

#endif // !_CCW_LIBCPP_NATIVE_STD
#endif
