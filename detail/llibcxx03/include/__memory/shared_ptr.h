/*
 * ccwrap: libc++ layout. classic C++03 body.
 */
#ifndef _CCW_LIBCPP___MEMORY_SHARED_PTR_H
#define _CCW_LIBCPP___MEMORY_SHARED_PTR_H
#include "../__config"
#include "../__utility/move.h"
#include <exception>   // _CCW_STD::exception (bad_weak_ptr)
#include "../__exception/eh_anchor.h"
#include "../__functional/hash.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Tp> class _CCW_LIBCPP_TEMPLATE_VIS shared_ptr;
template <class _Tp> class _CCW_LIBCPP_TEMPLATE_VIS weak_ptr;
template <class _Tp> class _CCW_LIBCPP_TEMPLATE_VIS enable_shared_from_this;
template <class _Up, class _Vp, class _Cp> _CCW_LIBCPP_HIDE_FROM_ABI void __enable_weak_this(const enable_shared_from_this<_Up>*, _Vp*, _Cp*);
_CCW_LIBCPP_HIDE_FROM_ABI inline void __enable_weak_this(const volatile void*, const volatile void*, const volatile void*);

class bad_weak_ptr : public _CCW_STD::exception {
public:
    _CCW_LIBCPP_HIDE_FROM_ABI bad_weak_ptr() {}
    _CCW_LIBCPP_HIDE_FROM_ABI virtual ~bad_weak_ptr() {}
    _CCW_LIBCPP_HIDE_FROM_ABI virtual const char* what() const _CCW_LIBCPP_NOEXCEPT { return "bad_weak_ptr"; }
};

typedef void (*__sp_del_id_t)();
template <class _Dp> struct __sp_del_id { _CCW_LIBCPP_HIDE_FROM_ABI static void __id() {} };

struct __sp_ctrl_base {
    long __shared_;   // number of shared_ptr owners
    long __weak_;     // number of weak_ptr owners + 1 while __shared_ > 0
    _CCW_LIBCPP_HIDE_FROM_ABI __sp_ctrl_base() : __shared_(1), __weak_(1) {}
    _CCW_LIBCPP_HIDE_FROM_ABI virtual ~__sp_ctrl_base() {}
    _CCW_LIBCPP_HIDE_FROM_ABI virtual void __on_zero_shared() {}
    _CCW_LIBCPP_HIDE_FROM_ABI virtual void* __get_deleter(__sp_del_id_t) { return 0; }
    _CCW_LIBCPP_HIDE_FROM_ABI void __add_shared() { ++__shared_; }
    _CCW_LIBCPP_HIDE_FROM_ABI void __add_weak()   { ++__weak_; }
    _CCW_LIBCPP_HIDE_FROM_ABI void __release_weak()   { if (--__weak_ == 0) delete this; }
    _CCW_LIBCPP_HIDE_FROM_ABI void __release_shared() { if (--__shared_ == 0) { __on_zero_shared(); __release_weak(); } }
};

template <class _Yp>
struct __sp_ctrl : __sp_ctrl_base {
    _Yp* __px_;
    _CCW_LIBCPP_HIDE_FROM_ABI explicit __sp_ctrl(_Yp* __p) : __px_(__p) {}
    _CCW_LIBCPP_HIDE_FROM_ABI virtual void __on_zero_shared() { delete __px_; __px_ = 0; }
};

template <class _Yp, class _Alloc>
struct __sp_ctrl_al : __sp_ctrl_base {
    _Yp*   __px_;
    _Alloc __a_;
    _CCW_LIBCPP_HIDE_FROM_ABI __sp_ctrl_al(_Yp* __p, const _Alloc& __a) : __px_(__p), __a_(__a) {}
    _CCW_LIBCPP_HIDE_FROM_ABI virtual void __on_zero_shared() {
        if (__px_) { __px_->~_Yp(); __a_.deallocate(__px_, 1); }
        __px_ = 0;
    }
};

template <class _Yp, class _Dp>
struct __sp_ctrl_d : __sp_ctrl_base {
    _Yp* __px_;
    _Dp  __d_;
    _CCW_LIBCPP_HIDE_FROM_ABI __sp_ctrl_d(_Yp* __p, _Dp __d) : __px_(__p), __d_(__d) {}
    _CCW_LIBCPP_HIDE_FROM_ABI virtual void __on_zero_shared() { if (__px_) __d_(__px_); __px_ = 0; }
    _CCW_LIBCPP_HIDE_FROM_ABI virtual void* __get_deleter(__sp_del_id_t __id) {
        return (__id == &__sp_del_id<_Dp>::__id) ? (void*)&__d_ : (void*)0;
    }
};

template <class _Tp>
class _CCW_LIBCPP_TEMPLATE_VIS shared_ptr {
    _Tp*            __ptr_;
    __sp_ctrl_base* __cb_;
    struct __nat { int __x_; };
    friend class weak_ptr<_Tp>;
    _CCW_LIBCPP_HIDE_FROM_ABI shared_ptr(_Tp* __p, __sp_ctrl_base* __cb) : __ptr_(__p), __cb_(__cb) {}
public:
    _CCW_LIBCPP_HIDE_FROM_ABI __sp_ctrl_base* __cb() const { return __cb_; }
    _CCW_LIBCPP_HIDE_FROM_ABI static shared_ptr __adopt(_Tp* __p, __sp_ctrl_base* __cb) { return shared_ptr(__p, __cb); }
    _CCW_LIBCPP_HIDE_FROM_ABI void __reset_raw() { __ptr_ = 0; __cb_ = 0; }   // steal helper (does not release)
    typedef _Tp element_type;
    _CCW_LIBCPP_HIDE_FROM_ABI shared_ptr() : __ptr_(0), __cb_(0) {}
    _CCW_LIBCPP_HIDE_FROM_ABI explicit shared_ptr(_Tp* __p) : __ptr_(__p), __cb_(__p ? new __sp_ctrl<_Tp>(__p) : 0) {
        __enable_weak_this(__p, __p, __cb_);
    }
    template <class _Yp, class _Dp>
    _CCW_LIBCPP_HIDE_FROM_ABI shared_ptr(_Yp* __p, _Dp __d)
        : __ptr_(__p), __cb_(__p ? (__sp_ctrl_base*)new __sp_ctrl_d<_Yp, _Dp>(__p, __d) : 0) {
        __enable_weak_this(__p, __p, __cb_);
    }
    _CCW_LIBCPP_HIDE_FROM_ABI explicit shared_ptr(const weak_ptr<_Tp>& __w);
    _CCW_LIBCPP_HIDE_FROM_ABI shared_ptr(const shared_ptr& __o) : __ptr_(__o.__ptr_), __cb_(__o.__cb_) { if (__cb_) __cb_->__add_shared(); }
    template <class _Yp>
    _CCW_LIBCPP_HIDE_FROM_ABI shared_ptr(const shared_ptr<_Yp>& __o) : __ptr_(__o.get()), __cb_(__o.__cb()) { if (__cb_) __cb_->__add_shared(); }
    template <class _Yp>
    _CCW_LIBCPP_HIDE_FROM_ABI shared_ptr(const shared_ptr<_Yp>& __o, _Tp* __p) : __ptr_(__p), __cb_(__o.__cb()) { if (__cb_) __cb_->__add_shared(); }
    _CCW_LIBCPP_HIDE_FROM_ABI shared_ptr& operator=(const shared_ptr& __o) {
        if (__cb_ != __o.__cb_ || __ptr_ != __o.__ptr_) {
            if (__cb_) __cb_->__release_shared();
            __ptr_ = __o.__ptr_; __cb_ = __o.__cb_; if (__cb_) __cb_->__add_shared();
        }
        return *this;
    }
    template <class _Yp>
    _CCW_LIBCPP_HIDE_FROM_ABI shared_ptr& operator=(const shared_ptr<_Yp>& __o) {
        if (__cb_ != __o.__cb()) {
            if (__cb_) __cb_->__release_shared();
            __cb_ = __o.__cb(); if (__cb_) __cb_->__add_shared();
        }
        __ptr_ = __o.get();
        return *this;
    }
#if !_CCW_HAS_REAL_RVALUE_REF
    _CCW_LIBCPP_HIDE_FROM_ABI shared_ptr(_CCW_RV_REF(shared_ptr) __r) : __ptr_(0), __cb_(0) {
        shared_ptr& __o = _CCW_MOVED_OBJ(__r);
        __ptr_ = __o.__ptr_; __cb_ = __o.__cb_;
        __o.__ptr_ = 0; __o.__cb_ = 0;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI shared_ptr& operator=(_CCW_RV_REF(shared_ptr) __r) {
        shared_ptr& __o = _CCW_MOVED_OBJ(__r);
        if (this != &__o) {
            if (__cb_) __cb_->__release_shared();
            __ptr_ = __o.__ptr_; __cb_ = __o.__cb_;
            __o.__ptr_ = 0; __o.__cb_ = 0;
        }
        return *this;
    }
    template <class _Yp>
    _CCW_LIBCPP_HIDE_FROM_ABI shared_ptr(_CCW_RV_REF_BEG shared_ptr<_Yp> _CCW_RV_REF_END __r) : __ptr_(0), __cb_(0) {
        shared_ptr<_Yp>& __o = _CCW_MOVED_OBJ(__r);
        __ptr_ = __o.get(); __cb_ = __o.__cb();
        __o.__reset_raw();
    }
    template <class _Yp>
    _CCW_LIBCPP_HIDE_FROM_ABI shared_ptr& operator=(_CCW_RV_REF_BEG shared_ptr<_Yp> _CCW_RV_REF_END __r) {
        shared_ptr<_Yp>& __o = _CCW_MOVED_OBJ(__r);
        if (__cb_) __cb_->__release_shared();
        __ptr_ = __o.get(); __cb_ = __o.__cb();
        __o.__reset_raw();
        return *this;
    }
#endif
    _CCW_LIBCPP_HIDE_FROM_ABI ~shared_ptr() { if (__cb_) __cb_->__release_shared(); }

    _CCW_LIBCPP_HIDE_FROM_ABI void reset()         { if (__cb_) __cb_->__release_shared(); __ptr_ = 0; __cb_ = 0; }
    _CCW_LIBCPP_HIDE_FROM_ABI void reset(_Tp* __p) { shared_ptr(__p).swap(*this); }
    _CCW_LIBCPP_HIDE_FROM_ABI void swap(shared_ptr& __o) {
        _Tp* __tp = __ptr_; __ptr_ = __o.__ptr_; __o.__ptr_ = __tp;
        __sp_ctrl_base* __tc = __cb_; __cb_ = __o.__cb_; __o.__cb_ = __tc;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp* get() const        { return __ptr_; }
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp& operator*() const  { return *__ptr_; }
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp* operator->() const { return __ptr_; }
    _CCW_LIBCPP_HIDE_FROM_ABI long use_count() const  { return __cb_ ? __cb_->__shared_ : 0; }
    _CCW_LIBCPP_HIDE_FROM_ABI bool unique() const     { return use_count() == 1; }
    template <class _Yp> _CCW_LIBCPP_HIDE_FROM_ABI bool owner_before(const shared_ptr<_Yp>& __o) const { return __cb_ < __o.__cb(); }
    template <class _Yp> _CCW_LIBCPP_HIDE_FROM_ABI bool owner_before(const weak_ptr<_Yp>& __o) const   { return __cb_ < __o.__cb(); }
    _CCW_LIBCPP_HIDE_FROM_ABI size_t owner_hash() const { return reinterpret_cast<size_t>(__cb_); }
    template <class _Yp> _CCW_LIBCPP_HIDE_FROM_ABI bool owner_equal(const shared_ptr<_Yp>& __o) const { return __cb_ == __o.__cb(); }
    template <class _Yp> _CCW_LIBCPP_HIDE_FROM_ABI bool owner_equal(const weak_ptr<_Yp>& __o) const   { return __cb_ == __o.__cb(); }
    _CCW_LIBCPP_HIDE_FROM_ABI operator int __nat::*() const { return __ptr_ ? &__nat::__x_ : 0; }
};

template <class _Tp, class _Up> _CCW_LIBCPP_HIDE_FROM_ABI bool operator==(const shared_ptr<_Tp>& __a, const shared_ptr<_Up>& __b) { return __a.get() == __b.get(); }
template <class _Tp, class _Up> _CCW_LIBCPP_HIDE_FROM_ABI bool operator!=(const shared_ptr<_Tp>& __a, const shared_ptr<_Up>& __b) { return __a.get() != __b.get(); }
template <class _Tp, class _Up> _CCW_LIBCPP_HIDE_FROM_ABI bool operator<(const shared_ptr<_Tp>& __a, const shared_ptr<_Up>& __b)  { return __a.get() < __b.get(); }
template <class _Tp, class _Up> _CCW_LIBCPP_HIDE_FROM_ABI bool operator>(const shared_ptr<_Tp>& __a, const shared_ptr<_Up>& __b)  { return __b < __a; }
template <class _Tp, class _Up> _CCW_LIBCPP_HIDE_FROM_ABI bool operator<=(const shared_ptr<_Tp>& __a, const shared_ptr<_Up>& __b) { return !(__b < __a); }
template <class _Tp, class _Up> _CCW_LIBCPP_HIDE_FROM_ABI bool operator>=(const shared_ptr<_Tp>& __a, const shared_ptr<_Up>& __b) { return !(__a < __b); }
#if !defined(__WATCOMC__)
template <class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI void swap(shared_ptr<_Tp>& __a, shared_ptr<_Tp>& __b) { __a.swap(__b); }
#endif

template <class _Tp>
class _CCW_LIBCPP_TEMPLATE_VIS weak_ptr {
    _Tp*            __ptr_;
    __sp_ctrl_base* __cb_;
public:
    typedef _Tp element_type;
    _CCW_LIBCPP_HIDE_FROM_ABI __sp_ctrl_base* __cb() const  { return __cb_; }
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp*            __rawp() const { return __ptr_; }
    _CCW_LIBCPP_HIDE_FROM_ABI void            __reset_raw()  { __ptr_ = 0; __cb_ = 0; }
    _CCW_LIBCPP_HIDE_FROM_ABI weak_ptr() : __ptr_(0), __cb_(0) {}
    _CCW_LIBCPP_HIDE_FROM_ABI weak_ptr(const weak_ptr& __o) : __ptr_(__o.__ptr_), __cb_(__o.__cb_) { if (__cb_) __cb_->__add_weak(); }
    _CCW_LIBCPP_HIDE_FROM_ABI weak_ptr(const shared_ptr<_Tp>& __o) : __ptr_(__o.get()), __cb_(0) { __init_from_shared(__o); }
    _CCW_LIBCPP_HIDE_FROM_ABI weak_ptr& operator=(const weak_ptr& __o) {
        if (__cb_ != __o.__cb_) { if (__cb_) __cb_->__release_weak(); __cb_ = __o.__cb_; if (__cb_) __cb_->__add_weak(); }
        __ptr_ = __o.__ptr_; return *this;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI weak_ptr& operator=(const shared_ptr<_Tp>& __o) { weak_ptr(__o).swap(*this); return *this; }
    template <class _Yp>
    _CCW_LIBCPP_HIDE_FROM_ABI weak_ptr(const weak_ptr<_Yp>& __o) : __ptr_(__o.__rawp()), __cb_(__o.__cb()) { if (__cb_) __cb_->__add_weak(); }
    template <class _Yp>
    _CCW_LIBCPP_HIDE_FROM_ABI weak_ptr(const shared_ptr<_Yp>& __o) : __ptr_(__o.get()), __cb_(__o.__cb()) { if (__cb_) __cb_->__add_weak(); }
    template <class _Yp>
    _CCW_LIBCPP_HIDE_FROM_ABI weak_ptr& operator=(const weak_ptr<_Yp>& __o) {
        if (__cb_ != __o.__cb()) { if (__cb_) __cb_->__release_weak(); __cb_ = __o.__cb(); if (__cb_) __cb_->__add_weak(); }
        __ptr_ = __o.__rawp(); return *this;
    }
#if !_CCW_HAS_REAL_RVALUE_REF
    _CCW_LIBCPP_HIDE_FROM_ABI weak_ptr(_CCW_RV_REF(weak_ptr) __r) : __ptr_(0), __cb_(0) {
        weak_ptr& __o = _CCW_MOVED_OBJ(__r);
        __ptr_ = __o.__ptr_; __cb_ = __o.__cb_;
        __o.__ptr_ = 0; __o.__cb_ = 0;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI weak_ptr& operator=(_CCW_RV_REF(weak_ptr) __r) {
        weak_ptr& __o = _CCW_MOVED_OBJ(__r);
        if (this != &__o) {
            if (__cb_) __cb_->__release_weak();
            __ptr_ = __o.__ptr_; __cb_ = __o.__cb_;
            __o.__ptr_ = 0; __o.__cb_ = 0;
        }
        return *this;
    }
    template <class _Yp>
    _CCW_LIBCPP_HIDE_FROM_ABI weak_ptr(_CCW_RV_REF_BEG weak_ptr<_Yp> _CCW_RV_REF_END __r) : __ptr_(0), __cb_(0) {
        weak_ptr<_Yp>& __o = _CCW_MOVED_OBJ(__r);
        __ptr_ = __o.__rawp(); __cb_ = __o.__cb();
        __o.__reset_raw();
    }
#endif
    _CCW_LIBCPP_HIDE_FROM_ABI ~weak_ptr() { if (__cb_) __cb_->__release_weak(); }

    _CCW_LIBCPP_HIDE_FROM_ABI void reset() { if (__cb_) __cb_->__release_weak(); __ptr_ = 0; __cb_ = 0; }
    _CCW_LIBCPP_HIDE_FROM_ABI void swap(weak_ptr& __o) {
        _Tp* __tp = __ptr_; __ptr_ = __o.__ptr_; __o.__ptr_ = __tp;
        __sp_ctrl_base* __tc = __cb_; __cb_ = __o.__cb_; __o.__cb_ = __tc;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI long use_count() const { return __cb_ ? __cb_->__shared_ : 0; }
    _CCW_LIBCPP_HIDE_FROM_ABI bool expired() const   { return !__cb_ || __cb_->__shared_ == 0; }
    template <class _Yp> _CCW_LIBCPP_HIDE_FROM_ABI bool owner_before(const weak_ptr<_Yp>& __o) const   { return __cb_ < __o.__cb(); }
    template <class _Yp> _CCW_LIBCPP_HIDE_FROM_ABI bool owner_before(const shared_ptr<_Yp>& __o) const { return __cb_ < __o.__cb(); }
    _CCW_LIBCPP_HIDE_FROM_ABI size_t owner_hash() const { return reinterpret_cast<size_t>(__cb_); }
    template <class _Yp> _CCW_LIBCPP_HIDE_FROM_ABI bool owner_equal(const weak_ptr<_Yp>& __o) const   { return __cb_ == __o.__cb(); }
    template <class _Yp> _CCW_LIBCPP_HIDE_FROM_ABI bool owner_equal(const shared_ptr<_Yp>& __o) const { return __cb_ == __o.__cb(); }
    _CCW_LIBCPP_HIDE_FROM_ABI shared_ptr<_Tp> lock() const {
        if (__cb_ && __cb_->__shared_ > 0) { __cb_->__add_shared(); return shared_ptr<_Tp>(__ptr_, __cb_); }
        return shared_ptr<_Tp>();
    }
    _CCW_LIBCPP_HIDE_FROM_ABI void __esft_init(_Tp* __p, __sp_ctrl_base* __cb) {
        if (__cb_) __cb_->__release_weak();
        __ptr_ = __p; __cb_ = __cb; if (__cb_) __cb_->__add_weak();
    }
    _CCW_LIBCPP_HIDE_FROM_ABI shared_ptr<_Tp> __lock_unchecked() const {
        if (__cb_) __cb_->__add_shared();
        return shared_ptr<_Tp>(__ptr_, __cb_);
    }
private:
    friend class shared_ptr<_Tp>;
    _CCW_LIBCPP_HIDE_FROM_ABI void __init_from_shared(const shared_ptr<_Tp>& __o) { __cb_ = __o.__cb_; if (__cb_) __cb_->__add_weak(); }
};

#if !defined(__WATCOMC__)
template <class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI void swap(weak_ptr<_Tp>& __a, weak_ptr<_Tp>& __b) { __a.swap(__b); }
#endif

template <class _Tp>
class _CCW_LIBCPP_TEMPLATE_VIS enable_shared_from_this {
    mutable weak_ptr<_Tp> __weak_this_;
protected:
    _CCW_LIBCPP_HIDE_FROM_ABI enable_shared_from_this() {}
    _CCW_LIBCPP_HIDE_FROM_ABI enable_shared_from_this(const enable_shared_from_this&) {}
    _CCW_LIBCPP_HIDE_FROM_ABI enable_shared_from_this& operator=(const enable_shared_from_this&) { return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI ~enable_shared_from_this() {}
public:
    _CCW_LIBCPP_HIDE_FROM_ABI shared_ptr<_Tp> shared_from_this()       { return __weak_this_.__lock_unchecked(); }
    _CCW_LIBCPP_HIDE_FROM_ABI shared_ptr<_Tp> shared_from_this() const { return __weak_this_.__lock_unchecked(); }
    _CCW_LIBCPP_HIDE_FROM_ABI weak_ptr<_Tp>   weak_from_this()       { return __weak_this_; }
    _CCW_LIBCPP_HIDE_FROM_ABI weak_ptr<_Tp>   weak_from_this() const { return __weak_this_; }
    template <class _Up, class _Vp, class _Cp>
    friend void __enable_weak_this(const enable_shared_from_this<_Up>*, _Vp*, _Cp*);
};

template <class _Up, class _Vp, class _Cp>
_CCW_LIBCPP_HIDE_FROM_ABI void __enable_weak_this(const enable_shared_from_this<_Up>* __e, _Vp* __p, _Cp* __cb) {
    (void)__p;
    if (__e && __cb) {
        _Up* __up = const_cast<_Up*>(static_cast<const _Up*>(__e));
        __e->__weak_this_.__esft_init(__up, __cb);
    }
}
_CCW_LIBCPP_HIDE_FROM_ABI inline void __enable_weak_this(const volatile void*, const volatile void*, const volatile void*) {}

template <class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI inline shared_ptr<_Tp>::shared_ptr(const weak_ptr<_Tp>& __w)
    : __ptr_(0), __cb_(0) {
    if (__w.expired())
        _CCW_THROW(bad_weak_ptr());
    shared_ptr<_Tp> __s = __w.lock();
    __ptr_ = __s.get(); __cb_ = __s.__cb();
    if (__cb_) __cb_->__add_shared();       // __s releases its own share on the way out
}

template <class _Dp, class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI _Dp* get_deleter(const shared_ptr<_Tp>& __s) {
    __sp_ctrl_base* __cb = __s.__cb();
    return __cb ? (_Dp*)__cb->__get_deleter(&__sp_del_id<_Dp>::__id) : (_Dp*)0;
}

template <class _Tp, class _Yp>
_CCW_LIBCPP_HIDE_FROM_ABI shared_ptr<_Tp> static_pointer_cast(const shared_ptr<_Yp>& __o) {
    return shared_ptr<_Tp>(__o, static_cast<_Tp*>(__o.get()));
}
template <class _Tp, class _Yp>
_CCW_LIBCPP_HIDE_FROM_ABI shared_ptr<_Tp> const_pointer_cast(const shared_ptr<_Yp>& __o) {
    return shared_ptr<_Tp>(__o, const_cast<_Tp*>(__o.get()));
}
template <class _Tp, class _Yp>
_CCW_LIBCPP_HIDE_FROM_ABI shared_ptr<_Tp> reinterpret_pointer_cast(const shared_ptr<_Yp>& __o) {
    return shared_ptr<_Tp>(__o, reinterpret_cast<_Tp*>(__o.get()));
}
template <class _Tp, class _Yp>
_CCW_LIBCPP_HIDE_FROM_ABI shared_ptr<_Tp> dynamic_pointer_cast(const shared_ptr<_Yp>& __o) {
    _Tp* __p = dynamic_cast<_Tp*>(__o.get());   // requires RTTI (-xr) on Open Watcom
    return __p ? shared_ptr<_Tp>(__o, __p) : shared_ptr<_Tp>();
}

template <class _Tp> class shared_ptr;
template <class _Tp = void> struct owner_less;
template <> struct owner_less<void> {
    typedef void is_transparent;
    template <class _T1, class _T2>
    _CCW_LIBCPP_HIDE_FROM_ABI bool operator()(const shared_ptr<_T1>& __x, const shared_ptr<_T2>& __y) const { return __x.owner_before(__y); }
    template <class _T1, class _T2>
    _CCW_LIBCPP_HIDE_FROM_ABI bool operator()(const weak_ptr<_T1>& __x, const weak_ptr<_T2>& __y) const { return __x.owner_before(__y); }
    template <class _T1, class _T2>
    _CCW_LIBCPP_HIDE_FROM_ABI bool operator()(const shared_ptr<_T1>& __x, const weak_ptr<_T2>& __y) const { return __x.owner_before(__y); }
    template <class _T1, class _T2>
    _CCW_LIBCPP_HIDE_FROM_ABI bool operator()(const weak_ptr<_T1>& __x, const shared_ptr<_T2>& __y) const { return __x.owner_before(__y); }
};
template <class _Tp> struct owner_less : public owner_less<void> {};

template <class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI shared_ptr<_Tp> make_shared() { return shared_ptr<_Tp>(new _Tp()); }
template <class _Tp, class _A0>
_CCW_LIBCPP_HIDE_FROM_ABI shared_ptr<_Tp> make_shared(const _A0& __a0) { return shared_ptr<_Tp>(new _Tp(__a0)); }
template <class _Tp, class _A0, class _A1>
_CCW_LIBCPP_HIDE_FROM_ABI shared_ptr<_Tp> make_shared(const _A0& __a0, const _A1& __a1) { return shared_ptr<_Tp>(new _Tp(__a0, __a1)); }
template <class _Tp, class _A0, class _A1, class _A2>
_CCW_LIBCPP_HIDE_FROM_ABI shared_ptr<_Tp> make_shared(const _A0& __a0, const _A1& __a1, const _A2& __a2) { return shared_ptr<_Tp>(new _Tp(__a0, __a1, __a2)); }
template <class _Tp, class _A0, class _A1, class _A2, class _A3>
_CCW_LIBCPP_HIDE_FROM_ABI shared_ptr<_Tp> make_shared(const _A0& __a0, const _A1& __a1, const _A2& __a2, const _A3& __a3) { return shared_ptr<_Tp>(new _Tp(__a0, __a1, __a2, __a3)); }

template <class _Tp, class _Alloc>
_CCW_LIBCPP_HIDE_FROM_ABI shared_ptr<_Tp> allocate_shared(const _Alloc& __al) {
    typedef typename _Alloc::template rebind<_Tp>::other __A2;
    __A2 __a2(__al);
    _Tp* __p = __a2.allocate(1);
    ::new (static_cast<void*>(__p)) _Tp();
    shared_ptr<_Tp> __s = shared_ptr<_Tp>::__adopt(__p, (__sp_ctrl_base*)new __sp_ctrl_al<_Tp, __A2>(__p, __a2));
    __enable_weak_this(__p, __p, __s.__cb());
    return __s;
}
template <class _Tp, class _Alloc, class _X0>
_CCW_LIBCPP_HIDE_FROM_ABI shared_ptr<_Tp> allocate_shared(const _Alloc& __al, const _X0& __x0) {
    typedef typename _Alloc::template rebind<_Tp>::other __A2;
    __A2 __a2(__al);
    _Tp* __p = __a2.allocate(1);
    ::new (static_cast<void*>(__p)) _Tp(__x0);
    shared_ptr<_Tp> __s = shared_ptr<_Tp>::__adopt(__p, (__sp_ctrl_base*)new __sp_ctrl_al<_Tp, __A2>(__p, __a2));
    __enable_weak_this(__p, __p, __s.__cb());
    return __s;
}
template <class _Tp, class _Alloc, class _X0, class _X1>
_CCW_LIBCPP_HIDE_FROM_ABI shared_ptr<_Tp> allocate_shared(const _Alloc& __al, const _X0& __x0, const _X1& __x1) {
    typedef typename _Alloc::template rebind<_Tp>::other __A2;
    __A2 __a2(__al);
    _Tp* __p = __a2.allocate(1);
    ::new (static_cast<void*>(__p)) _Tp(__x0, __x1);
    shared_ptr<_Tp> __s = shared_ptr<_Tp>::__adopt(__p, (__sp_ctrl_base*)new __sp_ctrl_al<_Tp, __A2>(__p, __a2));
    __enable_weak_this(__p, __p, __s.__cb());
    return __s;
}
template <class _Tp, class _Alloc, class _X0, class _X1, class _X2>
_CCW_LIBCPP_HIDE_FROM_ABI shared_ptr<_Tp> allocate_shared(const _Alloc& __al, const _X0& __x0, const _X1& __x1, const _X2& __x2) {
    typedef typename _Alloc::template rebind<_Tp>::other __A2;
    __A2 __a2(__al);
    _Tp* __p = __a2.allocate(1);
    ::new (static_cast<void*>(__p)) _Tp(__x0, __x1, __x2);
    shared_ptr<_Tp> __s = shared_ptr<_Tp>::__adopt(__p, (__sp_ctrl_base*)new __sp_ctrl_al<_Tp, __A2>(__p, __a2));
    __enable_weak_this(__p, __p, __s.__cb());
    return __s;
}

#if !_CCW_LIBCPP_HAS_NATIVE_CXX11_LIB
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS hash<shared_ptr<_Tp> > {
    _CCW_LIBCPP_HIDE_FROM_ABI _CCW_STD::size_t operator()(const shared_ptr<_Tp>& __p) const {
        return hash<_Tp*>()(__p.get());
    }
};
#endif

_CCW_LIBCPP_END_NAMESPACE_STD
_CCW_EH_ANCHOR(ebwp, bad_weak_ptr, bad_weak_ptr)

#endif
