//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___MEMORY_UNIQUE_PTR_H
#define _CCW_LIBCPP___MEMORY_UNIQUE_PTR_H
#include "../__config"
#include "../__utility/move.h"   // _CCW_RV_REF / _CCW_STD::move
#include "../__fnctmpl.h"
#include "../__functional/hash.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Tp> struct default_delete;   // defined in <__memory/misc.h>

#if !_CCW_HAS_REAL_RVALUE_REF
template <class _Tp, class _Dp> struct __unique_ptr_ref { _Tp* __ptr_; _Dp __del_; };
#endif

template <class _Tp, class _Dp = default_delete<_Tp> >
class _CCW_LIBCPP_TEMPLATE_VIS unique_ptr {
    _Tp* __ptr_;
    _Dp  __del_;
    typedef void (unique_ptr::*__bool_type)() const;
    _CCW_LIBCPP_HIDE_FROM_ABI void __true_bool() const {}
public:
    typedef _Tp  element_type;
    typedef _Tp* pointer;
    typedef _Dp  deleter_type;

    _CCW_LIBCPP_HIDE_FROM_ABI explicit unique_ptr(_Tp* __p = 0) : __ptr_(__p), __del_() {}
    _CCW_LIBCPP_HIDE_FROM_ABI unique_ptr(_Tp* __p, _Dp __d) : __ptr_(__p), __del_(__d) {}
    _CCW_LIBCPP_HIDE_FROM_ABI ~unique_ptr() { if (__ptr_) __del_(__ptr_); }

#if _CCW_HAS_REAL_RVALUE_REF
    unique_ptr(const unique_ptr&);            // move-only: copy is deleted
    unique_ptr& operator=(const unique_ptr&);
    _CCW_LIBCPP_HIDE_FROM_ABI unique_ptr(unique_ptr&& __o) _CCW_LIBCPP_NOEXCEPT : __ptr_(__o.release()), __del_(__o.get_deleter()) {}
    template <class _Up, class _Ep> _CCW_LIBCPP_HIDE_FROM_ABI unique_ptr(unique_ptr<_Up, _Ep>&& __o) _CCW_LIBCPP_NOEXCEPT : __ptr_(__o.release()), __del_(__o.get_deleter()) {}
    _CCW_LIBCPP_HIDE_FROM_ABI unique_ptr& operator=(unique_ptr&& __o) _CCW_LIBCPP_NOEXCEPT { reset(__o.release()); __del_ = __o.get_deleter(); return *this; }
#else
    _CCW_LIBCPP_HIDE_FROM_ABI unique_ptr(unique_ptr& __o) : __ptr_(__o.release()), __del_(__o.get_deleter()) {}
    template <class _Up, class _Ep> _CCW_LIBCPP_HIDE_FROM_ABI unique_ptr(unique_ptr<_Up, _Ep>& __o) : __ptr_(__o.release()), __del_(__o.get_deleter()) {}
    _CCW_LIBCPP_HIDE_FROM_ABI unique_ptr& operator=(unique_ptr& __o) { reset(__o.release()); __del_ = __o.get_deleter(); return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI unique_ptr& operator=(_CCW_RV_REF(unique_ptr) __r) {
        unique_ptr& __o = _CCW_MOVED_OBJ(__r);
        if (this != &__o) { reset(__o.release()); __del_ = __o.get_deleter(); }
        return *this;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI unique_ptr(__unique_ptr_ref<_Tp, _Dp> __r) : __ptr_(__r.__ptr_), __del_(__r.__del_) {}
    _CCW_LIBCPP_HIDE_FROM_ABI unique_ptr& operator=(__unique_ptr_ref<_Tp, _Dp> __r) { reset(__r.__ptr_); __del_ = __r.__del_; return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI operator __unique_ptr_ref<_Tp, _Dp>() { __unique_ptr_ref<_Tp, _Dp> __r; __r.__ptr_ = release(); __r.__del_ = __del_; return __r; }
#endif

    _CCW_LIBCPP_HIDE_FROM_ABI _Tp& operator*() const  { return *__ptr_; }
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp* operator->() const { return __ptr_; }
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp* get() const        { return __ptr_; }
    _CCW_LIBCPP_HIDE_FROM_ABI _Dp&       get_deleter()       { return __del_; }
    _CCW_LIBCPP_HIDE_FROM_ABI const _Dp& get_deleter() const { return __del_; }
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp* release()          { _Tp* __t = __ptr_; __ptr_ = 0; return __t; }
    _CCW_LIBCPP_HIDE_FROM_ABI void reset(_Tp* __p = 0) { if (__ptr_ != __p) { _Tp* __t = __ptr_; __ptr_ = __p; if (__t) __del_(__t); } }
    _CCW_LIBCPP_HIDE_FROM_ABI void swap(unique_ptr& __o) {
        _Tp* __t = __ptr_; __ptr_ = __o.__ptr_; __o.__ptr_ = __t;
        _Dp  __d = __del_; __del_ = __o.__del_; __o.__del_ = __d;
    }

    _CCW_LIBCPP_HIDE_FROM_ABI operator __bool_type() const { return __ptr_ ? &unique_ptr::__true_bool : 0; }
};

template <class _Tp, class _Dp>
class _CCW_LIBCPP_TEMPLATE_VIS unique_ptr<_Tp[], _Dp> {
    _Tp* __ptr_;
    _Dp  __del_;
    typedef void (unique_ptr::*__bool_type)() const;
    _CCW_LIBCPP_HIDE_FROM_ABI void __true_bool() const {}
public:
    typedef _Tp  element_type;
    typedef _Tp* pointer;
    typedef _Dp  deleter_type;

    _CCW_LIBCPP_HIDE_FROM_ABI explicit unique_ptr(_Tp* __p = 0) : __ptr_(__p), __del_() {}
    _CCW_LIBCPP_HIDE_FROM_ABI unique_ptr(_Tp* __p, _Dp __d) : __ptr_(__p), __del_(__d) {}
    _CCW_LIBCPP_HIDE_FROM_ABI ~unique_ptr() { if (__ptr_) __del_(__ptr_); }

#if _CCW_HAS_REAL_RVALUE_REF
    unique_ptr(const unique_ptr&);
    unique_ptr& operator=(const unique_ptr&);
    _CCW_LIBCPP_HIDE_FROM_ABI unique_ptr(unique_ptr&& __o) _CCW_LIBCPP_NOEXCEPT : __ptr_(__o.release()), __del_(__o.get_deleter()) {}
    _CCW_LIBCPP_HIDE_FROM_ABI unique_ptr& operator=(unique_ptr&& __o) _CCW_LIBCPP_NOEXCEPT { reset(__o.release()); __del_ = __o.get_deleter(); return *this; }
#else
    _CCW_LIBCPP_HIDE_FROM_ABI unique_ptr(unique_ptr& __o) : __ptr_(__o.release()), __del_(__o.get_deleter()) {}
    _CCW_LIBCPP_HIDE_FROM_ABI unique_ptr& operator=(unique_ptr& __o) { reset(__o.release()); __del_ = __o.get_deleter(); return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI unique_ptr& operator=(_CCW_RV_REF(unique_ptr) __r) {
        unique_ptr& __o = _CCW_MOVED_OBJ(__r);
        if (this != &__o) { reset(__o.release()); __del_ = __o.get_deleter(); }
        return *this;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI unique_ptr(__unique_ptr_ref<_Tp, _Dp> __r) : __ptr_(__r.__ptr_), __del_(__r.__del_) {}
    _CCW_LIBCPP_HIDE_FROM_ABI unique_ptr& operator=(__unique_ptr_ref<_Tp, _Dp> __r) { reset(__r.__ptr_); __del_ = __r.__del_; return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI operator __unique_ptr_ref<_Tp, _Dp>() { __unique_ptr_ref<_Tp, _Dp> __r; __r.__ptr_ = release(); __r.__del_ = __del_; return __r; }
#endif

    _CCW_LIBCPP_HIDE_FROM_ABI _Tp& operator[](_CCW_STD::size_t __i) const { return __ptr_[__i]; }
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp* get() const { return __ptr_; }
    _CCW_LIBCPP_HIDE_FROM_ABI _Dp&       get_deleter()       { return __del_; }
    _CCW_LIBCPP_HIDE_FROM_ABI const _Dp& get_deleter() const { return __del_; }
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp* release() { _Tp* __t = __ptr_; __ptr_ = 0; return __t; }
    _CCW_LIBCPP_HIDE_FROM_ABI void reset(_Tp* __p = 0) { if (__ptr_ != __p) { _Tp* __t = __ptr_; __ptr_ = __p; if (__t) __del_(__t); } }
    _CCW_LIBCPP_HIDE_FROM_ABI void swap(unique_ptr& __o) {
        _Tp* __t = __ptr_; __ptr_ = __o.__ptr_; __o.__ptr_ = __t;
        _Dp  __d = __del_; __del_ = __o.__del_; __o.__del_ = __d;
    }

    _CCW_LIBCPP_HIDE_FROM_ABI operator __bool_type() const { return __ptr_ ? &unique_ptr::__true_bool : 0; }
};

template <class _T1, class _D1, class _T2, class _D2> _CCW_LIBCPP_HIDE_FROM_ABI bool operator==(const unique_ptr<_T1, _D1>& __a, const unique_ptr<_T2, _D2>& __b) { return __a.get() == __b.get(); }
template <class _T1, class _D1, class _T2, class _D2> _CCW_LIBCPP_HIDE_FROM_ABI bool operator!=(const unique_ptr<_T1, _D1>& __a, const unique_ptr<_T2, _D2>& __b) { return __a.get() != __b.get(); }
template <class _T1, class _D1, class _T2, class _D2> _CCW_LIBCPP_HIDE_FROM_ABI bool operator< (const unique_ptr<_T1, _D1>& __a, const unique_ptr<_T2, _D2>& __b) { return __a.get() <  __b.get(); }
template <class _T1, class _D1, class _T2, class _D2> _CCW_LIBCPP_HIDE_FROM_ABI bool operator> (const unique_ptr<_T1, _D1>& __a, const unique_ptr<_T2, _D2>& __b) { return __b <  __a; }
template <class _T1, class _D1, class _T2, class _D2> _CCW_LIBCPP_HIDE_FROM_ABI bool operator<=(const unique_ptr<_T1, _D1>& __a, const unique_ptr<_T2, _D2>& __b) { return !(__b < __a); }
template <class _T1, class _D1, class _T2, class _D2> _CCW_LIBCPP_HIDE_FROM_ABI bool operator>=(const unique_ptr<_T1, _D1>& __a, const unique_ptr<_T2, _D2>& __b) { return !(__a < __b); }

#if !defined(__WATCOMC__)
template <class _Tp, class _Dp> _CCW_LIBCPP_HIDE_FROM_ABI void swap(unique_ptr<_Tp, _Dp>& __x, unique_ptr<_Tp, _Dp>& __y) { __x.swap(__y); }
#endif

#if !_CCW_LIBCPP_HAS_NATIVE_CXX11_LIB
template <class _Tp, class _Dp> struct _CCW_LIBCPP_TEMPLATE_VIS hash<unique_ptr<_Tp, _Dp> > {
    _CCW_LIBCPP_HIDE_FROM_ABI _CCW_STD::size_t operator()(const unique_ptr<_Tp, _Dp>& __p) const {
        return hash<_Tp*>()(__p.get());
    }
};
#endif

#if _CCW_NO_VARIADIC_TEMPLATES
template <class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI unique_ptr<_Tp> make_unique() { return unique_ptr<_Tp>(new _Tp()); }
#  define _CCW_GEN_MAKE_UNIQUE(N)                                              \
    template <class _Tp _CCW_FNCTMPL_CM_TMPLARG(N)>                            \
    _CCW_LIBCPP_HIDE_FROM_ABI unique_ptr<_Tp> make_unique(_CCW_FNCTMPL_DECLARG(N)) { return unique_ptr<_Tp>(new _Tp(_CCW_FNCTMPL_FNCARG(N))); }
_CCW_GEN_MAKE_UNIQUE(1) _CCW_GEN_MAKE_UNIQUE(2) _CCW_GEN_MAKE_UNIQUE(3) _CCW_GEN_MAKE_UNIQUE(4)
#  undef _CCW_GEN_MAKE_UNIQUE
#else
template <class _Tp, class... _Args>
_CCW_LIBCPP_HIDE_FROM_ABI unique_ptr<_Tp> make_unique(_Args&&... __a) { return unique_ptr<_Tp>(new _Tp(static_cast<_Args&&>(__a)...)); }
#endif

template <class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI unique_ptr<_Tp> make_unique_for_overwrite() { return unique_ptr<_Tp>(new _Tp); }

_CCW_LIBCPP_END_NAMESPACE_STD
#endif
