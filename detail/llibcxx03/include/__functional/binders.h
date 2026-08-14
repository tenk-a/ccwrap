//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//  The classic (C++98) function adaptors: not1/not2, bind1st/bind2nd, ptr_fun,
//  mem_fun/mem_fun_ref. Deprecated in later standards but fully portable and widely
//  used in older code -- provided verbatim.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___FUNCTIONAL_BINDERS_H
#define _CCW_LIBCPP___FUNCTIONAL_BINDERS_H
#include "../__config"
#include "../__functional/unary_function.h"
#include "../__functional/binary_function.h"
#if _CCW_LIBCPP_NATIVE_STD
#  include <functional>
#else

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Pred>
class unary_negate : public unary_function<typename _Pred::argument_type, bool> {
    _Pred __p_;
public:
    _CCW_LIBCPP_HIDE_FROM_ABI explicit unary_negate(const _Pred& __p) : __p_(__p) {}
    _CCW_LIBCPP_HIDE_FROM_ABI bool operator()(const typename _Pred::argument_type& __x) const { return !__p_(__x); }
};
template <class _Pred> _CCW_LIBCPP_HIDE_FROM_ABI unary_negate<_Pred> not1(const _Pred& __p) { return unary_negate<_Pred>(__p); }

template <class _Pred>
class binary_negate : public binary_function<typename _Pred::first_argument_type, typename _Pred::second_argument_type, bool> {
    _Pred __p_;
public:
    _CCW_LIBCPP_HIDE_FROM_ABI explicit binary_negate(const _Pred& __p) : __p_(__p) {}
    _CCW_LIBCPP_HIDE_FROM_ABI bool operator()(const typename _Pred::first_argument_type& __x, const typename _Pred::second_argument_type& __y) const { return !__p_(__x, __y); }
};
template <class _Pred> _CCW_LIBCPP_HIDE_FROM_ABI binary_negate<_Pred> not2(const _Pred& __p) { return binary_negate<_Pred>(__p); }

template <class _Op>
class binder1st : public unary_function<typename _Op::second_argument_type, typename _Op::result_type> {
protected:
    _Op __op_;
    typename _Op::first_argument_type __v_;
public:
    _CCW_LIBCPP_HIDE_FROM_ABI binder1st(const _Op& __o, const typename _Op::first_argument_type& __v) : __op_(__o), __v_(__v) {}
    _CCW_LIBCPP_HIDE_FROM_ABI typename _Op::result_type operator()(const typename _Op::second_argument_type& __x) const { return __op_(__v_, __x); }
    _CCW_LIBCPP_HIDE_FROM_ABI typename _Op::result_type operator()(typename _Op::second_argument_type& __x) const { return __op_(__v_, __x); }
};
template <class _Op, class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI binder1st<_Op> bind1st(const _Op& __o, const _Tp& __x) { return binder1st<_Op>(__o, __x); }

template <class _Op>
class binder2nd : public unary_function<typename _Op::first_argument_type, typename _Op::result_type> {
protected:
    _Op __op_;
    typename _Op::second_argument_type __v_;
public:
    _CCW_LIBCPP_HIDE_FROM_ABI binder2nd(const _Op& __o, const typename _Op::second_argument_type& __v) : __op_(__o), __v_(__v) {}
    _CCW_LIBCPP_HIDE_FROM_ABI typename _Op::result_type operator()(const typename _Op::first_argument_type& __x) const { return __op_(__x, __v_); }
    _CCW_LIBCPP_HIDE_FROM_ABI typename _Op::result_type operator()(typename _Op::first_argument_type& __x) const { return __op_(__x, __v_); }
};
template <class _Op, class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI binder2nd<_Op> bind2nd(const _Op& __o, const _Tp& __x) { return binder2nd<_Op>(__o, __x); }

template <class _Arg, class _Result>
class pointer_to_unary_function : public unary_function<_Arg, _Result> {
    _Result (*__f_)(_Arg);
public:
    _CCW_LIBCPP_HIDE_FROM_ABI explicit pointer_to_unary_function(_Result (*__f)(_Arg)) : __f_(__f) {}
    _CCW_LIBCPP_HIDE_FROM_ABI _Result operator()(_Arg __x) const { return __f_(__x); }
};
template <class _Arg, class _Result> _CCW_LIBCPP_HIDE_FROM_ABI pointer_to_unary_function<_Arg, _Result> ptr_fun(_Result (*__f)(_Arg)) { return pointer_to_unary_function<_Arg, _Result>(__f); }

template <class _Arg1, class _Arg2, class _Result>
class pointer_to_binary_function : public binary_function<_Arg1, _Arg2, _Result> {
    _Result (*__f_)(_Arg1, _Arg2);
public:
    _CCW_LIBCPP_HIDE_FROM_ABI explicit pointer_to_binary_function(_Result (*__f)(_Arg1, _Arg2)) : __f_(__f) {}
    _CCW_LIBCPP_HIDE_FROM_ABI _Result operator()(_Arg1 __x, _Arg2 __y) const { return __f_(__x, __y); }
};
template <class _Arg1, class _Arg2, class _Result> _CCW_LIBCPP_HIDE_FROM_ABI pointer_to_binary_function<_Arg1, _Arg2, _Result> ptr_fun(_Result (*__f)(_Arg1, _Arg2)) { return pointer_to_binary_function<_Arg1, _Arg2, _Result>(__f); }

template <class _Result, class _Tp>
class mem_fun_t : public unary_function<_Tp*, _Result> {
    _Result (_Tp::*__p_)();
public:
    _CCW_LIBCPP_HIDE_FROM_ABI explicit mem_fun_t(_Result (_Tp::*__p)()) : __p_(__p) {}
    _CCW_LIBCPP_HIDE_FROM_ABI _Result operator()(_Tp* __o) const { return (__o->*__p_)(); }
};
template <class _Result, class _Tp, class _Arg>
class mem_fun1_t : public binary_function<_Tp*, _Arg, _Result> {
    _Result (_Tp::*__p_)(_Arg);
public:
    _CCW_LIBCPP_HIDE_FROM_ABI explicit mem_fun1_t(_Result (_Tp::*__p)(_Arg)) : __p_(__p) {}
    _CCW_LIBCPP_HIDE_FROM_ABI _Result operator()(_Tp* __o, _Arg __x) const { return (__o->*__p_)(__x); }
};
template <class _Result, class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI mem_fun_t<_Result, _Tp> mem_fun(_Result (_Tp::*__p)()) { return mem_fun_t<_Result, _Tp>(__p); }
template <class _Result, class _Tp, class _Arg> _CCW_LIBCPP_HIDE_FROM_ABI mem_fun1_t<_Result, _Tp, _Arg> mem_fun(_Result (_Tp::*__p)(_Arg)) { return mem_fun1_t<_Result, _Tp, _Arg>(__p); }

template <class _Result, class _Tp>
class mem_fun_ref_t : public unary_function<_Tp, _Result> {
    _Result (_Tp::*__p_)();
public:
    _CCW_LIBCPP_HIDE_FROM_ABI explicit mem_fun_ref_t(_Result (_Tp::*__p)()) : __p_(__p) {}
    _CCW_LIBCPP_HIDE_FROM_ABI _Result operator()(_Tp& __o) const { return (__o.*__p_)(); }
};
template <class _Result, class _Tp, class _Arg>
class mem_fun1_ref_t : public binary_function<_Tp, _Arg, _Result> {
    _Result (_Tp::*__p_)(_Arg);
public:
    _CCW_LIBCPP_HIDE_FROM_ABI explicit mem_fun1_ref_t(_Result (_Tp::*__p)(_Arg)) : __p_(__p) {}
    _CCW_LIBCPP_HIDE_FROM_ABI _Result operator()(_Tp& __o, _Arg __x) const { return (__o.*__p_)(__x); }
};
template <class _Result, class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI mem_fun_ref_t<_Result, _Tp> mem_fun_ref(_Result (_Tp::*__p)()) { return mem_fun_ref_t<_Result, _Tp>(__p); }
template <class _Result, class _Tp, class _Arg> _CCW_LIBCPP_HIDE_FROM_ABI mem_fun1_ref_t<_Result, _Tp, _Arg> mem_fun_ref(_Result (_Tp::*__p)(_Arg)) { return mem_fun1_ref_t<_Result, _Tp, _Arg>(__p); }

template <class _Result, class _Tp>
class const_mem_fun_t : public unary_function<const _Tp*, _Result> {
    _Result (_Tp::*__p_)() const;
public:
    _CCW_LIBCPP_HIDE_FROM_ABI explicit const_mem_fun_t(_Result (_Tp::*__p)() const) : __p_(__p) {}
    _CCW_LIBCPP_HIDE_FROM_ABI _Result operator()(const _Tp* __o) const { return (__o->*__p_)(); }
};
template <class _Result, class _Tp, class _Arg>
class const_mem_fun1_t : public binary_function<const _Tp*, _Arg, _Result> {
    _Result (_Tp::*__p_)(_Arg) const;
public:
    _CCW_LIBCPP_HIDE_FROM_ABI explicit const_mem_fun1_t(_Result (_Tp::*__p)(_Arg) const) : __p_(__p) {}
    _CCW_LIBCPP_HIDE_FROM_ABI _Result operator()(const _Tp* __o, _Arg __x) const { return (__o->*__p_)(__x); }
};
template <class _Result, class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI const_mem_fun_t<_Result, _Tp> mem_fun(_Result (_Tp::*__p)() const) { return const_mem_fun_t<_Result, _Tp>(__p); }
template <class _Result, class _Tp, class _Arg> _CCW_LIBCPP_HIDE_FROM_ABI const_mem_fun1_t<_Result, _Tp, _Arg> mem_fun(_Result (_Tp::*__p)(_Arg) const) { return const_mem_fun1_t<_Result, _Tp, _Arg>(__p); }

template <class _Result, class _Tp>
class const_mem_fun_ref_t : public unary_function<_Tp, _Result> {
    _Result (_Tp::*__p_)() const;
public:
    _CCW_LIBCPP_HIDE_FROM_ABI explicit const_mem_fun_ref_t(_Result (_Tp::*__p)() const) : __p_(__p) {}
    _CCW_LIBCPP_HIDE_FROM_ABI _Result operator()(const _Tp& __o) const { return (__o.*__p_)(); }
};
template <class _Result, class _Tp, class _Arg>
class const_mem_fun1_ref_t : public binary_function<_Tp, _Arg, _Result> {
    _Result (_Tp::*__p_)(_Arg) const;
public:
    _CCW_LIBCPP_HIDE_FROM_ABI explicit const_mem_fun1_ref_t(_Result (_Tp::*__p)(_Arg) const) : __p_(__p) {}
    _CCW_LIBCPP_HIDE_FROM_ABI _Result operator()(const _Tp& __o, _Arg __x) const { return (__o.*__p_)(__x); }
};
template <class _Result, class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI const_mem_fun_ref_t<_Result, _Tp> mem_fun_ref(_Result (_Tp::*__p)() const) { return const_mem_fun_ref_t<_Result, _Tp>(__p); }
template <class _Result, class _Tp, class _Arg> _CCW_LIBCPP_HIDE_FROM_ABI const_mem_fun1_ref_t<_Result, _Tp, _Arg> mem_fun_ref(_Result (_Tp::*__p)(_Arg) const) { return const_mem_fun1_ref_t<_Result, _Tp, _Arg>(__p); }

_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_LIBCPP_NATIVE_STD
#endif
