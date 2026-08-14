/*
// ccwrap: libc++ layout, classic C++03 body.
//
// _CCW_STD::function<Signature>: a type-erased callable. Open Watcom has no variadic
// templates, so the signature is matched by ARITY via partial specializations
// (0..3 arguments). Type erasure uses a virtual base with call/clone; callables
// are stored by value (copy). Works with function pointers and functor objects
// (Open Watcom has no lambdas).
*/
#ifndef _CCW_LIBCPP___FUNCTIONAL_FUNCTION_H
#define _CCW_LIBCPP___FUNCTIONAL_FUNCTION_H
#include "../__config"
#if defined(_CPPRTTI) || defined(__GXX_RTTI) || defined(__cpp_rtti)
#  define _CCW_LIBCPP_HAS_RTTI 1
#  include <typeinfo>
#else
#  define _CCW_LIBCPP_HAS_RTTI 0
#endif
#include "../__utility/move.h"
#include "../__functional/bad_function_call.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Sig> class function;   // primary: only the arity specializations exist

template <class _Rp>
class function<_Rp()> {
    struct __base { virtual ~__base() {} virtual _Rp __call() = 0; virtual __base* __clone() const = 0;
#if _CCW_LIBCPP_HAS_RTTI
        virtual const _CCW_STD::type_info& __type() const = 0;
        virtual void* __addr() = 0;
#endif
    };
    template <class _Fp> struct __impl : __base {
        _Fp __f_;
        _CCW_LIBCPP_HIDE_FROM_ABI __impl(const _Fp& __f) : __f_(__f) {}
        _CCW_LIBCPP_HIDE_FROM_ABI _Rp __call() { return __f_(); }
        _CCW_LIBCPP_HIDE_FROM_ABI __base* __clone() const { return new __impl(__f_); }
#if _CCW_LIBCPP_HAS_RTTI
        _CCW_LIBCPP_HIDE_FROM_ABI const _CCW_STD::type_info& __type() const { return typeid(_Fp); }
        _CCW_LIBCPP_HIDE_FROM_ABI void* __addr() { return &__f_; }
#endif
    };
    __base* __p_;
    typedef void (function::*__bool_type)() const;
    _CCW_LIBCPP_HIDE_FROM_ABI void __true_bool() const {}
public:
    typedef _Rp result_type;
    _CCW_LIBCPP_HIDE_FROM_ABI function() : __p_(0) {}
    _CCW_LIBCPP_HIDE_FROM_ABI function(int) : __p_(0) {}
    template <class _Fp> _CCW_LIBCPP_HIDE_FROM_ABI function(_Fp __f) : __p_(new __impl<_Fp>(__f)) {}
    _CCW_LIBCPP_HIDE_FROM_ABI function(const function& __o) : __p_(__o.__p_ ? __o.__p_->__clone() : 0) {}
    _CCW_LIBCPP_HIDE_FROM_ABI function& operator=(const function& __o) { if (this != &__o) { delete __p_; __p_ = __o.__p_ ? __o.__p_->__clone() : 0; } return *this; }
#if !_CCW_HAS_REAL_RVALUE_REF
    _CCW_LIBCPP_HIDE_FROM_ABI function(_CCW_RV_REF(function) __r) : __p_(0) {
        function& __o = _CCW_MOVED_OBJ(__r);
        __p_ = __o.__p_; __o.__p_ = 0;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI function& operator=(_CCW_RV_REF(function) __r) {
        function& __o = _CCW_MOVED_OBJ(__r);
        if (this != &__o) { delete __p_; __p_ = __o.__p_; __o.__p_ = 0; }
        return *this;
    }
#endif
    _CCW_LIBCPP_HIDE_FROM_ABI ~function() { delete __p_; }
    _CCW_LIBCPP_HIDE_FROM_ABI _Rp operator()() const { if (!__p_) _CCW_THROW(bad_function_call()); return __p_->__call(); }
    _CCW_LIBCPP_HIDE_FROM_ABI void swap(function& __o) { __base* __t = __p_; __p_ = __o.__p_; __o.__p_ = __t; }
    _CCW_LIBCPP_HIDE_FROM_ABI operator __bool_type() const { return __p_ ? &function::__true_bool : 0; }
#if _CCW_LIBCPP_HAS_RTTI
    _CCW_LIBCPP_HIDE_FROM_ABI const _CCW_STD::type_info& target_type() const
        { return __p_ ? __p_->__type() : typeid(void); }
    template <class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI _Tp* target()
        { return (__p_ && __p_->__type() == typeid(_Tp)) ? (_Tp*)__p_->__addr() : 0; }
    template <class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI const _Tp* target() const
        { return (__p_ && __p_->__type() == typeid(_Tp)) ? (const _Tp*)const_cast<__base*>(__p_)->__addr() : 0; }
#endif
};

template <class _Rp, class _A0>
class function<_Rp(_A0)> {
    struct __base { virtual ~__base() {} virtual _Rp __call(_A0) = 0; virtual __base* __clone() const = 0;
#if _CCW_LIBCPP_HAS_RTTI
        virtual const _CCW_STD::type_info& __type() const = 0;
        virtual void* __addr() = 0;
#endif
    };
    template <class _Fp> struct __impl : __base {
        _Fp __f_;
        _CCW_LIBCPP_HIDE_FROM_ABI __impl(const _Fp& __f) : __f_(__f) {}
        _CCW_LIBCPP_HIDE_FROM_ABI _Rp __call(_A0 __a0) { return __f_(__a0); }
        _CCW_LIBCPP_HIDE_FROM_ABI __base* __clone() const { return new __impl(__f_); }
#if _CCW_LIBCPP_HAS_RTTI
        _CCW_LIBCPP_HIDE_FROM_ABI const _CCW_STD::type_info& __type() const { return typeid(_Fp); }
        _CCW_LIBCPP_HIDE_FROM_ABI void* __addr() { return &__f_; }
#endif
    };
    __base* __p_;
    typedef void (function::*__bool_type)() const;
    _CCW_LIBCPP_HIDE_FROM_ABI void __true_bool() const {}
public:
    typedef _Rp result_type;
    typedef _A0 argument_type;
    _CCW_LIBCPP_HIDE_FROM_ABI function() : __p_(0) {}
    _CCW_LIBCPP_HIDE_FROM_ABI function(int) : __p_(0) {}
    template <class _Fp> _CCW_LIBCPP_HIDE_FROM_ABI function(_Fp __f) : __p_(new __impl<_Fp>(__f)) {}
    _CCW_LIBCPP_HIDE_FROM_ABI function(const function& __o) : __p_(__o.__p_ ? __o.__p_->__clone() : 0) {}
    _CCW_LIBCPP_HIDE_FROM_ABI function& operator=(const function& __o) { if (this != &__o) { delete __p_; __p_ = __o.__p_ ? __o.__p_->__clone() : 0; } return *this; }
#if !_CCW_HAS_REAL_RVALUE_REF
    _CCW_LIBCPP_HIDE_FROM_ABI function(_CCW_RV_REF(function) __r) : __p_(0) {
        function& __o = _CCW_MOVED_OBJ(__r);
        __p_ = __o.__p_; __o.__p_ = 0;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI function& operator=(_CCW_RV_REF(function) __r) {
        function& __o = _CCW_MOVED_OBJ(__r);
        if (this != &__o) { delete __p_; __p_ = __o.__p_; __o.__p_ = 0; }
        return *this;
    }
#endif
    _CCW_LIBCPP_HIDE_FROM_ABI ~function() { delete __p_; }
    _CCW_LIBCPP_HIDE_FROM_ABI _Rp operator()(_A0 __a0) const { if (!__p_) _CCW_THROW(bad_function_call()); return __p_->__call(__a0); }
    _CCW_LIBCPP_HIDE_FROM_ABI void swap(function& __o) { __base* __t = __p_; __p_ = __o.__p_; __o.__p_ = __t; }
    _CCW_LIBCPP_HIDE_FROM_ABI operator __bool_type() const { return __p_ ? &function::__true_bool : 0; }
#if _CCW_LIBCPP_HAS_RTTI
    _CCW_LIBCPP_HIDE_FROM_ABI const _CCW_STD::type_info& target_type() const
        { return __p_ ? __p_->__type() : typeid(void); }
    template <class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI _Tp* target()
        { return (__p_ && __p_->__type() == typeid(_Tp)) ? (_Tp*)__p_->__addr() : 0; }
    template <class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI const _Tp* target() const
        { return (__p_ && __p_->__type() == typeid(_Tp)) ? (const _Tp*)const_cast<__base*>(__p_)->__addr() : 0; }
#endif
};

template <class _Rp, class _A0, class _A1>
class function<_Rp(_A0, _A1)> {
    struct __base { virtual ~__base() {} virtual _Rp __call(_A0, _A1) = 0; virtual __base* __clone() const = 0;
#if _CCW_LIBCPP_HAS_RTTI
        virtual const _CCW_STD::type_info& __type() const = 0;
        virtual void* __addr() = 0;
#endif
    };
    template <class _Fp> struct __impl : __base {
        _Fp __f_;
        _CCW_LIBCPP_HIDE_FROM_ABI __impl(const _Fp& __f) : __f_(__f) {}
        _CCW_LIBCPP_HIDE_FROM_ABI _Rp __call(_A0 __a0, _A1 __a1) { return __f_(__a0, __a1); }
        _CCW_LIBCPP_HIDE_FROM_ABI __base* __clone() const { return new __impl(__f_); }
#if _CCW_LIBCPP_HAS_RTTI
        _CCW_LIBCPP_HIDE_FROM_ABI const _CCW_STD::type_info& __type() const { return typeid(_Fp); }
        _CCW_LIBCPP_HIDE_FROM_ABI void* __addr() { return &__f_; }
#endif
    };
    __base* __p_;
    typedef void (function::*__bool_type)() const;
    _CCW_LIBCPP_HIDE_FROM_ABI void __true_bool() const {}
public:
    typedef _Rp result_type;
    _CCW_LIBCPP_HIDE_FROM_ABI function() : __p_(0) {}
    _CCW_LIBCPP_HIDE_FROM_ABI function(int) : __p_(0) {}
    template <class _Fp> _CCW_LIBCPP_HIDE_FROM_ABI function(_Fp __f) : __p_(new __impl<_Fp>(__f)) {}
    _CCW_LIBCPP_HIDE_FROM_ABI function(const function& __o) : __p_(__o.__p_ ? __o.__p_->__clone() : 0) {}
    _CCW_LIBCPP_HIDE_FROM_ABI function& operator=(const function& __o) { if (this != &__o) { delete __p_; __p_ = __o.__p_ ? __o.__p_->__clone() : 0; } return *this; }
#if !_CCW_HAS_REAL_RVALUE_REF
    _CCW_LIBCPP_HIDE_FROM_ABI function(_CCW_RV_REF(function) __r) : __p_(0) {
        function& __o = _CCW_MOVED_OBJ(__r);
        __p_ = __o.__p_; __o.__p_ = 0;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI function& operator=(_CCW_RV_REF(function) __r) {
        function& __o = _CCW_MOVED_OBJ(__r);
        if (this != &__o) { delete __p_; __p_ = __o.__p_; __o.__p_ = 0; }
        return *this;
    }
#endif
    _CCW_LIBCPP_HIDE_FROM_ABI ~function() { delete __p_; }
    _CCW_LIBCPP_HIDE_FROM_ABI _Rp operator()(_A0 __a0, _A1 __a1) const { if (!__p_) _CCW_THROW(bad_function_call()); return __p_->__call(__a0, __a1); }
    _CCW_LIBCPP_HIDE_FROM_ABI void swap(function& __o) { __base* __t = __p_; __p_ = __o.__p_; __o.__p_ = __t; }
    _CCW_LIBCPP_HIDE_FROM_ABI operator __bool_type() const { return __p_ ? &function::__true_bool : 0; }
#if _CCW_LIBCPP_HAS_RTTI
    _CCW_LIBCPP_HIDE_FROM_ABI const _CCW_STD::type_info& target_type() const
        { return __p_ ? __p_->__type() : typeid(void); }
    template <class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI _Tp* target()
        { return (__p_ && __p_->__type() == typeid(_Tp)) ? (_Tp*)__p_->__addr() : 0; }
    template <class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI const _Tp* target() const
        { return (__p_ && __p_->__type() == typeid(_Tp)) ? (const _Tp*)const_cast<__base*>(__p_)->__addr() : 0; }
#endif
};

template <class _Rp, class _A0, class _A1, class _A2>
class function<_Rp(_A0, _A1, _A2)> {
    struct __base { virtual ~__base() {} virtual _Rp __call(_A0, _A1, _A2) = 0; virtual __base* __clone() const = 0;
#if _CCW_LIBCPP_HAS_RTTI
        virtual const _CCW_STD::type_info& __type() const = 0;
        virtual void* __addr() = 0;
#endif
    };
    template <class _Fp> struct __impl : __base {
        _Fp __f_;
        _CCW_LIBCPP_HIDE_FROM_ABI __impl(const _Fp& __f) : __f_(__f) {}
        _CCW_LIBCPP_HIDE_FROM_ABI _Rp __call(_A0 __a0, _A1 __a1, _A2 __a2) { return __f_(__a0, __a1, __a2); }
        _CCW_LIBCPP_HIDE_FROM_ABI __base* __clone() const { return new __impl(__f_); }
#if _CCW_LIBCPP_HAS_RTTI
        _CCW_LIBCPP_HIDE_FROM_ABI const _CCW_STD::type_info& __type() const { return typeid(_Fp); }
        _CCW_LIBCPP_HIDE_FROM_ABI void* __addr() { return &__f_; }
#endif
    };
    __base* __p_;
    typedef void (function::*__bool_type)() const;
    _CCW_LIBCPP_HIDE_FROM_ABI void __true_bool() const {}
public:
    typedef _Rp result_type;
    _CCW_LIBCPP_HIDE_FROM_ABI function() : __p_(0) {}
    _CCW_LIBCPP_HIDE_FROM_ABI function(int) : __p_(0) {}
    template <class _Fp> _CCW_LIBCPP_HIDE_FROM_ABI function(_Fp __f) : __p_(new __impl<_Fp>(__f)) {}
    _CCW_LIBCPP_HIDE_FROM_ABI function(const function& __o) : __p_(__o.__p_ ? __o.__p_->__clone() : 0) {}
    _CCW_LIBCPP_HIDE_FROM_ABI function& operator=(const function& __o) { if (this != &__o) { delete __p_; __p_ = __o.__p_ ? __o.__p_->__clone() : 0; } return *this; }
#if !_CCW_HAS_REAL_RVALUE_REF
    _CCW_LIBCPP_HIDE_FROM_ABI function(_CCW_RV_REF(function) __r) : __p_(0) {
        function& __o = _CCW_MOVED_OBJ(__r);
        __p_ = __o.__p_; __o.__p_ = 0;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI function& operator=(_CCW_RV_REF(function) __r) {
        function& __o = _CCW_MOVED_OBJ(__r);
        if (this != &__o) { delete __p_; __p_ = __o.__p_; __o.__p_ = 0; }
        return *this;
    }
#endif
    _CCW_LIBCPP_HIDE_FROM_ABI ~function() { delete __p_; }
    _CCW_LIBCPP_HIDE_FROM_ABI _Rp operator()(_A0 __a0, _A1 __a1, _A2 __a2) const { if (!__p_) _CCW_THROW(bad_function_call()); return __p_->__call(__a0, __a1, __a2); }
    _CCW_LIBCPP_HIDE_FROM_ABI void swap(function& __o) { __base* __t = __p_; __p_ = __o.__p_; __o.__p_ = __t; }
    _CCW_LIBCPP_HIDE_FROM_ABI operator __bool_type() const { return __p_ ? &function::__true_bool : 0; }
#if _CCW_LIBCPP_HAS_RTTI
    _CCW_LIBCPP_HIDE_FROM_ABI const _CCW_STD::type_info& target_type() const
        { return __p_ ? __p_->__type() : typeid(void); }
    template <class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI _Tp* target()
        { return (__p_ && __p_->__type() == typeid(_Tp)) ? (_Tp*)__p_->__addr() : 0; }
    template <class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI const _Tp* target() const
        { return (__p_ && __p_->__type() == typeid(_Tp)) ? (const _Tp*)const_cast<__base*>(__p_)->__addr() : 0; }
#endif
};

#if !defined(__WATCOMC__)
template <class _Sig>
_CCW_LIBCPP_HIDE_FROM_ABI inline void swap(function<_Sig>& __x, function<_Sig>& __y) { __x.swap(__y); }
#endif

_CCW_LIBCPP_END_NAMESPACE_STD
#endif
