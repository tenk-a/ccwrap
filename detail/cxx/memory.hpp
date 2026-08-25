/**
 *  @file   detail/memory.hpp
 *  @brief  <memory> gap-fills: pre-C++11 unique_ptr/shared_ptr/weak_ptr, plus
 *          make_shared/make_unique/addressof (C++11/14) and the destroy_at /
 *          construct_at families (C++17/20).
 *  @license Boost Software License Version 1.0
 *  @note   No move semantics on C++03; do not copy a unique_ptr by value. Each
 */
#ifndef _CCW_DETAIL_MEMORY_HPP
#define _CCW_DETAIL_MEMORY_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include "eh_fail.hpp"
#include <cstddef>
#include <cassert>
#include <exception>   /* bad_weak_ptr's base */
#include <typeinfo>    /* get_deleter compares type_info */

#if defined(_MSC_VER) && _MSC_VER >= 1400
# pragma warning(push)
# pragma warning(disable:4100)
#endif

#if _CCW_STD_LIB_LT(1600, 201103L)
#include "fnctmpl.hpp"
#include "move03.hpp"   // _CCW_RV_REF -- unique_ptr's ownership transfer, below

namespace std {

#ifndef _CCW_DEFAULT_DELETE_DEFINED
#define _CCW_DEFAULT_DELETE_DEFINED
template<class T>
struct default_delete {
    void operator()(T* p) const { delete p; }
};
template<class T>
struct default_delete<T[]> {
    void operator()(T* p) const { delete[] p; }
};
#endif

template <class T, class D>
struct __ccw_uptr_ref { T* ptr_; D del_; };

template <class T, class D = default_delete<T> >
class unique_ptr {
    struct PtDe : D { T* ptr_; };
    PtDe    m;

    typedef T const* const* const* const* const* const* _bool_type;

public:
    typedef T*  pointer;
    typedef T   element_type;
    typedef D   deleter_type;

    unique_ptr(T* p = NULL) noexcept { m.ptr_ = p; }
    unique_ptr(T* p, D const& d) noexcept { get_deleter() = d; m.ptr_ = p; }
    template<class U> unique_ptr(U p) noexcept { m.ptr_ = (T*)p; }
    template<class U> unique_ptr(U p, D const& d) noexcept { get_deleter() = d; m.ptr_ = (T*)p; }
    ~unique_ptr() noexcept { get_deleter()(m.ptr_); }

    unique_ptr(unique_ptr& o) noexcept { get_deleter() = o.get_deleter(); m.ptr_ = o.release(); }
    unique_ptr& operator=(unique_ptr& o) noexcept { reset(o.release()); get_deleter() = o.get_deleter(); return *this; }
    unique_ptr(_CCW_RV_REF(unique_ptr) o) noexcept {
        unique_ptr& r = _CCW_MOVED_OBJ(o); get_deleter() = r.get_deleter(); m.ptr_ = r.release();
    }
    unique_ptr& operator=(_CCW_RV_REF(unique_ptr) o) noexcept {
        unique_ptr& r = _CCW_MOVED_OBJ(o); reset(r.release()); get_deleter() = r.get_deleter(); return *this;
    }
    unique_ptr& operator=(T* p) noexcept { reset(p); return *this; }
    unique_ptr(__ccw_uptr_ref<T, D> r) noexcept { get_deleter() = r.del_; m.ptr_ = r.ptr_; }
    unique_ptr& operator=(__ccw_uptr_ref<T, D> r) noexcept { reset(r.ptr_); get_deleter() = r.del_; return *this; }
    operator __ccw_uptr_ref<T, D>() noexcept {
        __ccw_uptr_ref<T, D> r; r.del_ = get_deleter(); r.ptr_ = release(); return r;
    }

    T* get() const noexcept { return m.ptr_; }
    deleter_type&       get_deleter()       noexcept { return *(D*)&m; }
    deleter_type const& get_deleter() const noexcept { return *(D*)&m; }

    T* release() noexcept { T* p = m.ptr_; m.ptr_ = NULL; return p; }
    void reset(T* t = 0) noexcept { T* p = m.ptr_; if (p != t) get_deleter()(p); m.ptr_ = t; }

    T&       operator*()        noexcept { return *m.ptr_; }
    T const& operator*()  const noexcept { return *m.ptr_; }
    T*       operator->()       noexcept { return m.ptr_; }
    T const* operator->() const noexcept { return m.ptr_; }

    bool operator!() const { return !m.ptr_; }
    operator _bool_type() const { return reinterpret_cast<_bool_type>(std::size_t(m.ptr_ != NULL)); }

    void swap(unique_ptr& r) noexcept { T* t = m.ptr_; m.ptr_ = r.m.ptr_; r.m.ptr_ = t; }
};

template <class T, class D>
class unique_ptr<T[], D> {
    struct PtDe : D { T* ptr_; };
    PtDe m;
    typedef void (unique_ptr::*_bool_type)() const;
    void __ccw_true() const {}
public:
    typedef T* pointer;
    typedef T  element_type;
    typedef D  deleter_type;

    unique_ptr(T* p = NULL) noexcept { m.ptr_ = p; }
    unique_ptr(T* p, D const& d) noexcept { get_deleter() = d; m.ptr_ = p; }
    ~unique_ptr() noexcept { get_deleter()(m.ptr_); }

    unique_ptr(unique_ptr& o) noexcept { get_deleter() = o.get_deleter(); m.ptr_ = o.release(); }
    unique_ptr& operator=(unique_ptr& o) noexcept { reset(o.release()); get_deleter() = o.get_deleter(); return *this; }
    unique_ptr(_CCW_RV_REF(unique_ptr) o) noexcept {
        unique_ptr& r = _CCW_MOVED_OBJ(o); get_deleter() = r.get_deleter(); m.ptr_ = r.release();
    }
    unique_ptr& operator=(_CCW_RV_REF(unique_ptr) o) noexcept {
        unique_ptr& r = _CCW_MOVED_OBJ(o); reset(r.release()); get_deleter() = r.get_deleter(); return *this;
    }
    unique_ptr& operator=(T* p) noexcept { reset(p); return *this; }
    unique_ptr(__ccw_uptr_ref<T, D> r) noexcept { get_deleter() = r.del_; m.ptr_ = r.ptr_; }
    unique_ptr& operator=(__ccw_uptr_ref<T, D> r) noexcept { reset(r.ptr_); get_deleter() = r.del_; return *this; }
    operator __ccw_uptr_ref<T, D>() noexcept {
        __ccw_uptr_ref<T, D> r; r.del_ = get_deleter(); r.ptr_ = release(); return r;
    }

    T* get() const noexcept { return m.ptr_; }
    deleter_type&       get_deleter()       noexcept { return *(D*)&m; }
    deleter_type const& get_deleter() const noexcept { return *(D*)&m; }

    T* release() noexcept { T* p = m.ptr_; m.ptr_ = NULL; return p; }
    void reset(T* t = 0) noexcept { T* p = m.ptr_; if (p != t) get_deleter()(p); m.ptr_ = t; }

    T&       operator[](std::size_t i)       noexcept { return m.ptr_[i]; }
    T const& operator[](std::size_t i) const noexcept { return m.ptr_[i]; }

    bool operator!() const { return !m.ptr_; }
    operator _bool_type() const { return m.ptr_ ? &unique_ptr::__ccw_true : (_bool_type)0; }

    void swap(unique_ptr& r) noexcept { T* t = m.ptr_; m.ptr_ = r.m.ptr_; r.m.ptr_ = t; }
};

template<typename T, class D>
inline void swap(unique_ptr<T,D>& l, unique_ptr<T,D>& r) { l.swap(r); }

#define _CCW_UNIQUE_PTR_OP(_op)                                             \
template <class T1, class D1, class T2, class D2>                           \
inline bool operator _op(unique_ptr<T1,D1> const& l, unique_ptr<T2,D2> const& r) { return l.get() _op r.get(); } \
template<typename T, class D>                                               \
inline bool operator _op(unique_ptr<T,D> const& l, std::ptrdiff_t r) { assert(r == 0); return l.get() _op NULL; } \
template<typename T, class D>                                               \
inline bool operator _op(std::ptrdiff_t l, unique_ptr<T,D> const& r) { assert(l == 0); return NULL _op r.get(); }

_CCW_UNIQUE_PTR_OP(==)
_CCW_UNIQUE_PTR_OP(!=)
_CCW_UNIQUE_PTR_OP(<)
_CCW_UNIQUE_PTR_OP(>)
_CCW_UNIQUE_PTR_OP(<=)
_CCW_UNIQUE_PTR_OP(>=)
#undef _CCW_UNIQUE_PTR_OP

template<class T> class weak_ptr;
template<class T> class shared_ptr;
template<class T> class enable_shared_from_this;

class __ccw_sp_counter {
public:
    long use_, weak_;
    __ccw_sp_counter() : use_(1), weak_(1) {}
    virtual ~__ccw_sp_counter() {}
    virtual void dispose() = 0;
    virtual void* get_deleter_ptr(const std::type_info&) { return 0; }
    void incr_use()  { ++use_; }
    void incr_weak() { ++weak_; }
    void decr_use()  { if (--use_ == 0) { dispose(); decr_weak(); } }
    void decr_weak() { if (--weak_ == 0) delete this; }
    bool try_incr_use() { if (use_ == 0) return false; ++use_; return true; }
};
template<class Y>
class __ccw_sp_counter_p : public __ccw_sp_counter {
    Y* p_;
public:
    __ccw_sp_counter_p(Y* p) : p_(p) {}
    virtual void dispose() { delete p_; }
};
template<class Y, class D>
class __ccw_sp_counter_pd : public __ccw_sp_counter {
    Y* p_; D d_;
public:
    __ccw_sp_counter_pd(Y* p, D d) : p_(p), d_(d) {}
    virtual void dispose() { d_(p_); }
    virtual void* get_deleter_ptr(const std::type_info& __ti) {
        return __ti == typeid(D) ? (void*)&d_ : (void*)0;
    }
};

class bad_weak_ptr : public std::exception {
public:
    bad_weak_ptr() throw() {}
    virtual ~bad_weak_ptr() throw() {}
    virtual const char* what() const throw() { return "bad_weak_ptr"; }
};

template<class T, class Y> void __ccw_sp_enable(const shared_ptr<T>* sp, const enable_shared_from_this<Y>* base);
inline void __ccw_sp_enable(const volatile void*, const volatile void*) {}

template<class T>
class shared_ptr {
    T*               ptr_;
    __ccw_sp_counter* cnt_;
    template<class Y> friend class shared_ptr;
    template<class Y> friend class weak_ptr;
    typedef T* shared_ptr::* _bool_type;
public:
    typedef T element_type;

    shared_ptr() : ptr_(NULL), cnt_(NULL) {}
    template<class Y> explicit shared_ptr(Y* p) : ptr_(p), cnt_(new __ccw_sp_counter_p<Y>(p)) { __ccw_sp_enable(this, p); }
    template<class Y, class D> shared_ptr(Y* p, D d) : ptr_(p), cnt_(new __ccw_sp_counter_pd<Y, D>(p, d)) { __ccw_sp_enable(this, p); }
    shared_ptr(const shared_ptr& r) : ptr_(r.ptr_), cnt_(r.cnt_) { if (cnt_) cnt_->incr_use(); }
    template<class Y> shared_ptr(const shared_ptr<Y>& r) : ptr_(r.ptr_), cnt_(r.cnt_) { if (cnt_) cnt_->incr_use(); }
    template<class Y> shared_ptr(const shared_ptr<Y>& r, T* p) : ptr_(p), cnt_(r.cnt_) { if (cnt_) cnt_->incr_use(); }  // aliasing
    template<class Y> explicit shared_ptr(const weak_ptr<Y>& w);
    ~shared_ptr() { if (cnt_) cnt_->decr_use(); }

    shared_ptr& operator=(const shared_ptr& r) { shared_ptr(r).swap(*this); return *this; }
    template<class Y> shared_ptr& operator=(const shared_ptr<Y>& r) { shared_ptr(r).swap(*this); return *this; }

    shared_ptr(_CCW_RV_REF(shared_ptr) r) : ptr_(NULL), cnt_(NULL) {
        shared_ptr& o = _CCW_MOVED_OBJ(r);
        ptr_ = o.ptr_; cnt_ = o.cnt_; o.ptr_ = NULL; o.cnt_ = NULL;
    }
    shared_ptr& operator=(_CCW_RV_REF(shared_ptr) r) {
        shared_ptr& o = _CCW_MOVED_OBJ(r);
        if (this != &o) { shared_ptr().swap(*this); ptr_ = o.ptr_; cnt_ = o.cnt_; o.ptr_ = NULL; o.cnt_ = NULL; }
        return *this;
    }
#if !_CCW_HAS_REAL_RVALUE_REF
    template<class Y> shared_ptr(::_ccw::rv<shared_ptr<Y> >& r) : ptr_(NULL), cnt_(NULL) {
        shared_ptr<Y>& o = _CCW_MOVED_OBJ(r);
        ptr_ = o.ptr_; cnt_ = o.cnt_; o.ptr_ = NULL; o.cnt_ = NULL;
    }
    template<class Y> shared_ptr& operator=(::_ccw::rv<shared_ptr<Y> >& r) {
        shared_ptr<Y>& o = _CCW_MOVED_OBJ(r);
        shared_ptr().swap(*this);
        ptr_ = o.ptr_; cnt_ = o.cnt_; o.ptr_ = NULL; o.cnt_ = NULL;
        return *this;
    }
#endif

    void reset() { shared_ptr().swap(*this); }
    template<class Y> void reset(Y* p) { shared_ptr(p).swap(*this); }
    void swap(shared_ptr& r) {
        T* tp = ptr_; ptr_ = r.ptr_; r.ptr_ = tp;
        __ccw_sp_counter* tc = cnt_; cnt_ = r.cnt_; r.cnt_ = tc;
    }

    T*   get()   const { return ptr_; }
    T&   operator*()  const { return *ptr_; }
    T*   operator->() const { return ptr_; }
    long use_count() const { return cnt_ ? cnt_->use_ : 0; }
    bool unique()    const { return use_count() == 1; }
    bool operator!() const { return !ptr_; }
    operator _bool_type() const { return ptr_ ? &shared_ptr::ptr_ : NULL; }

    template<class Y> bool owner_before(const shared_ptr<Y>& r) const { return cnt_ < r.__ccw_owner(); }
    template<class Y> bool owner_before(const weak_ptr<Y>& r)   const { return cnt_ < r.__ccw_owner(); }
    __ccw_sp_counter* __ccw_owner() const { return cnt_; }
};

template<class T>
class weak_ptr {
    T*               ptr_;
    __ccw_sp_counter* cnt_;
    template<class Y> friend class weak_ptr;
    template<class Y> friend class shared_ptr;
public:
    typedef T element_type;
    weak_ptr() : ptr_(NULL), cnt_(NULL) {}
    weak_ptr(const weak_ptr& r) : ptr_(r.ptr_), cnt_(r.cnt_) { if (cnt_) cnt_->incr_weak(); }
    template<class Y> weak_ptr(const shared_ptr<Y>& r) : ptr_(r.get()), cnt_(r.cnt_) { if (cnt_) cnt_->incr_weak(); }
    template<class Y> weak_ptr(const weak_ptr<Y>& r) : ptr_(r.ptr_), cnt_(r.cnt_) { if (cnt_) cnt_->incr_weak(); }
    ~weak_ptr() { if (cnt_) cnt_->decr_weak(); }
    weak_ptr& operator=(const weak_ptr& r) { weak_ptr(r).swap(*this); return *this; }
    template<class Y> weak_ptr& operator=(const shared_ptr<Y>& r) { weak_ptr(r).swap(*this); return *this; }

    weak_ptr(_CCW_RV_REF(weak_ptr) r) : ptr_(NULL), cnt_(NULL) {
        weak_ptr& o = _CCW_MOVED_OBJ(r);
        ptr_ = o.ptr_; cnt_ = o.cnt_; o.ptr_ = NULL; o.cnt_ = NULL;
    }
    weak_ptr& operator=(_CCW_RV_REF(weak_ptr) r) {
        weak_ptr& o = _CCW_MOVED_OBJ(r);
        if (this != &o) { weak_ptr().swap(*this); ptr_ = o.ptr_; cnt_ = o.cnt_; o.ptr_ = NULL; o.cnt_ = NULL; }
        return *this;
    }
    void swap(weak_ptr& r) {
        T* tp = ptr_; ptr_ = r.ptr_; r.ptr_ = tp;
        __ccw_sp_counter* tc = cnt_; cnt_ = r.cnt_; r.cnt_ = tc;
    }
    void reset() { weak_ptr().swap(*this); }
    long use_count() const { return cnt_ ? cnt_->use_ : 0; }
    bool expired()   const { return use_count() == 0; }
    shared_ptr<T> lock() const {
        shared_ptr<T> __r;
        if (cnt_ && cnt_->try_incr_use()) { __r.ptr_ = ptr_; __r.cnt_ = cnt_; }
        return __r;
    }
    template<class Y> bool owner_before(const shared_ptr<Y>& r) const { return cnt_ < r.__ccw_owner(); }
    template<class Y> bool owner_before(const weak_ptr<Y>& r)   const { return cnt_ < r.__ccw_owner(); }
    __ccw_sp_counter* __ccw_owner() const { return cnt_; }
};

template<class T> template<class Y>
inline shared_ptr<T>::shared_ptr(const weak_ptr<Y>& w) : ptr_(NULL), cnt_(NULL) {
    if (w.cnt_ && w.cnt_->try_incr_use()) { ptr_ = w.ptr_; cnt_ = w.cnt_; }
    else _CCW_THROW(bad_weak_ptr());
}

template<class T> struct owner_less;
template<class T> struct owner_less< shared_ptr<T> > {
    bool operator()(const shared_ptr<T>& a, const shared_ptr<T>& b) const { return a.owner_before(b); }
    bool operator()(const shared_ptr<T>& a, const weak_ptr<T>& b)   const { return a.owner_before(b); }
    bool operator()(const weak_ptr<T>& a,   const shared_ptr<T>& b) const { return a.owner_before(b); }
};
template<class T> struct owner_less< weak_ptr<T> > {
    bool operator()(const weak_ptr<T>& a,   const weak_ptr<T>& b)   const { return a.owner_before(b); }
    bool operator()(const shared_ptr<T>& a, const weak_ptr<T>& b)   const { return a.owner_before(b); }
    bool operator()(const weak_ptr<T>& a,   const shared_ptr<T>& b) const { return a.owner_before(b); }
};
template<> struct owner_less<void> {
    typedef void is_transparent;
    template<class T, class U> bool operator()(const shared_ptr<T>& a, const shared_ptr<U>& b) const { return a.owner_before(b); }
    template<class T, class U> bool operator()(const shared_ptr<T>& a, const weak_ptr<U>& b)   const { return a.owner_before(b); }
    template<class T, class U> bool operator()(const weak_ptr<T>& a,   const shared_ptr<U>& b) const { return a.owner_before(b); }
    template<class T, class U> bool operator()(const weak_ptr<T>& a,   const weak_ptr<U>& b)   const { return a.owner_before(b); }
};

template<class D, class T>
inline D* get_deleter(const shared_ptr<T>& p) {
    __ccw_sp_counter* c = p.__ccw_owner();
    return c ? (D*)c->get_deleter_ptr(typeid(D)) : (D*)0;
}

template<class T>
class enable_shared_from_this {
protected:
    enable_shared_from_this() {}
    enable_shared_from_this(const enable_shared_from_this&) {}
    enable_shared_from_this& operator=(const enable_shared_from_this&) { return *this; }
    ~enable_shared_from_this() {}
public:
    shared_ptr<T>       shared_from_this()       { return shared_ptr<T>(weak_this_); }
    shared_ptr<const T> shared_from_this() const { return shared_ptr<const T>(weak_this_); }
    weak_ptr<T>         weak_from_this()          { return weak_this_; }
    weak_ptr<const T>   weak_from_this()    const { return weak_this_; }
private:
    mutable weak_ptr<T> weak_this_;
    template<class U, class Y> friend void __ccw_sp_enable(const shared_ptr<U>*, const enable_shared_from_this<Y>*);
};

template<class T, class Y>
inline void __ccw_sp_enable(const shared_ptr<T>* sp, const enable_shared_from_this<Y>* base) {
    if (base) {
        enable_shared_from_this<Y>* b = const_cast<enable_shared_from_this<Y>*>(base);
        b->weak_this_ = shared_ptr<Y>(*sp, static_cast<Y*>(b));
    }
}

template<class T, class U> inline bool operator==(const shared_ptr<T>& a, const shared_ptr<U>& b) { return a.get() == b.get(); }
template<class T, class U> inline bool operator!=(const shared_ptr<T>& a, const shared_ptr<U>& b) { return a.get() != b.get(); }
template<class T, class U> inline bool operator< (const shared_ptr<T>& a, const shared_ptr<U>& b) { return a.get() <  b.get(); }
template<class T, class U> inline bool operator> (const shared_ptr<T>& a, const shared_ptr<U>& b) { return a.get() >  b.get(); }
template<class T, class U> inline bool operator<=(const shared_ptr<T>& a, const shared_ptr<U>& b) { return a.get() <= b.get(); }
template<class T, class U> inline bool operator>=(const shared_ptr<T>& a, const shared_ptr<U>& b) { return a.get() >= b.get(); }
template<class T> inline void swap(shared_ptr<T>& a, shared_ptr<T>& b) { a.swap(b); }

template<class T, class U> inline shared_ptr<T> static_pointer_cast(const shared_ptr<U>& r)  { return shared_ptr<T>(r, static_cast<T*>(r.get())); }
template<class T, class U> inline shared_ptr<T> const_pointer_cast(const shared_ptr<U>& r)   { return shared_ptr<T>(r, const_cast<T*>(r.get())); }
template<class T, class U> inline shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U>& r) {
    T* p = dynamic_cast<T*>(r.get());
    return p ? shared_ptr<T>(r, p) : shared_ptr<T>();
}
template<class T> inline void swap(weak_ptr<T>& a, weak_ptr<T>& b) { a.swap(b); }

#define _CCW_GEN_MAKE_SHARED(N)                                              \
    template<class T _CCW_FNCTMPL_CM_TMPLARG(N)>                             \
    inline shared_ptr<T> make_shared(_CCW_FNCTMPL_DECLARG(N)) {              \
        return shared_ptr<T>(new T(_CCW_FNCTMPL_FNCARG(N)));                 \
    }
_CCW_FNCTMPL_GENERATE(_CCW_GEN_MAKE_SHARED)
#undef _CCW_GEN_MAKE_SHARED

#define _CCW_GEN_ALLOCATE_SHARED(N)                                          \
    template<class T, class A _CCW_FNCTMPL_CM_TMPLARG(N)>                    \
    inline shared_ptr<T> allocate_shared(const A& __a                        \
                                         _CCW_FNCTMPL_CM_DECLARG(N)) {       \
        typename A::template rebind<T>::other __ra(__a);                     \
        T* __p = __ra.allocate(1);                                           \
        _CCW_TRY { ::new ((void*)__p) T(_CCW_FNCTMPL_FNCARG(N)); }               \
        _CCW_CATCH_ALL { __ra.deallocate(__p, 1); _CCW_RETHROW; }                    \
        return shared_ptr<T>(__p, __ccw_alloc_deleter<T, A>(__a));           \
    }

template<class T, class A>
struct __ccw_alloc_deleter {
    A __a_;
    __ccw_alloc_deleter(const A& __a) : __a_(__a) {}
    void operator()(T* __p) const {
        typename A::template rebind<T>::other __ra(__a_);
        __p->~T();
        __ra.deallocate(__p, 1);
    }
};

_CCW_FNCTMPL_GENERATE(_CCW_GEN_ALLOCATE_SHARED)
#undef _CCW_GEN_ALLOCATE_SHARED

template<class T> inline T* addressof(T& r) {
    return reinterpret_cast<T*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(r)));
}

template<class T> struct hash;
template<class T> struct hash< shared_ptr<T> > {
    _CCW_STD::size_t operator()(const shared_ptr<T>& p) const { return hash<T*>()(p.get()); }
};
template<class T, class D> struct hash< unique_ptr<T, D> > {
    _CCW_STD::size_t operator()(const unique_ptr<T, D>& p) const { return hash<T*>()(p.get()); }
};

}   // namespace std
#endif  // _CCW_STD_LIB_LT(1600, 201103L) (pre-C++11 smart pointers)

#include "move03.hpp"   // _CCW_HAS_REAL_RVALUE_REF
#if _CCW_STD_LIB_LT(1800, 201402L) && _CCW_HAS_REAL_RVALUE_REF
#include "fnctmpl.hpp"
namespace _CCW_STD14 {
template<class T>           struct __ccw_mkuniq       { typedef unique_ptr<T>   __single_t; };
template<class T>           struct __ccw_mkuniq<T[]>  { typedef unique_ptr<T[]> __array;  };
template<class T, size_t N> struct __ccw_mkuniq<T[N]> { };   // make_unique<T[N]> is ill-formed

#define _CCW_GEN_MAKE_UNIQUE(N)                                              \
    template<class T _CCW_FNCTMPL_CM_TMPLARG(N)>                             \
    inline typename __ccw_mkuniq<T>::__single_t                              \
    make_unique(_CCW_FNCTMPL_DECLARG(N)) {                                   \
        return unique_ptr<T>(new T(_CCW_FNCTMPL_FNCARG(N)));                 \
    }
_CCW_FNCTMPL_GENERATE(_CCW_GEN_MAKE_UNIQUE)
#undef _CCW_GEN_MAKE_UNIQUE

template<class T>
inline typename __ccw_mkuniq<T>::__array make_unique(size_t __n) {
    return unique_ptr<T>(new typename std::remove_extent<T>::type[__n]());
}
}   // _CCW_STD14
#endif

#if _CCW_STD_LIB_LT(1700, 201103L)
namespace std {
inline void* align(std::size_t __alignment, std::size_t __size, void*& __ptr, std::size_t& __space) {
    if (__space < __size)
        return 0;
    std::size_t __cur = reinterpret_cast<std::size_t>(__ptr);
    std::size_t __aln = (__cur + (__alignment - 1)) & ~(__alignment - 1);
    std::size_t __pad = __aln - __cur;
    if (__space - __size < __pad)
        return 0;
    __ptr = reinterpret_cast<void*>(__aln);
    __space -= __pad;
    return __ptr;
}
}   // namespace std
#endif

#if _CCW_STD_LIB_LT(1700, 201103L)
#include <../../detail/llibcxx03/include/__memory/allocator_traits.h>
#include <../../detail/llibcxx03/include/__memory/pointer_traits.h>
#endif

#if _CCW_STD_LIB_LT(1700, 201103L)
#include "has_allocator_type.hpp"
namespace std {
template<class T, class Alloc>
struct uses_allocator { static const bool value = __ccw::__has_allocator_type<T>::value; };
}   // namespace std
#endif

#if _CCW_STD_LIB_LT(1600, 201103L)
#include <new>
#include <iterator>
namespace std {
template<class InIt, class Size, class FwdIt>
inline FwdIt uninitialized_copy_n(InIt first, Size n, FwdIt dest) {
    typedef typename std::iterator_traits<FwdIt>::value_type _Vt;
    FwdIt cur = dest;
    _CCW_TRY {
        for (; n > 0; --n, ++first, ++cur)
            ::new ((void*)&*cur) _Vt(*first);
    }
    _CCW_CATCH_ALL {
        for (; dest != cur; ++dest) (&*dest)->~_Vt();
        _CCW_RETHROW;
    }
    return cur;
}
}   // namespace std
#endif

#if __cplusplus < 201703L
#include <new>
#include <iterator>
#include "move03.hpp"   // _ccw::move (real move on C++11, emulation otherwise)
namespace _CCW_STD17 {
template<class T> inline void destroy_at(T* p) { p->~T(); }
template<class It> inline void destroy(It first, It last) { for (; first != last; ++first) destroy_at(std::addressof(*first)); }
template<class It, class Size> inline It destroy_n(It first, Size n) { for (; n > 0; --n, ++first) destroy_at(std::addressof(*first)); return first; }
template<class It> inline void uninitialized_default_construct(It first, It last) {
    typedef typename std::iterator_traits<It>::value_type _T;
    for (; first != last; ++first) ::new(static_cast<void*>(std::addressof(*first))) _T;
}
template<class It, class Size> inline It uninitialized_default_construct_n(It first, Size n) {
    typedef typename std::iterator_traits<It>::value_type _T;
    for (; n > 0; --n, ++first) ::new(static_cast<void*>(std::addressof(*first))) _T;
    return first;
}
template<class It> inline void uninitialized_value_construct(It first, It last) {
    typedef typename std::iterator_traits<It>::value_type _T;
    for (; first != last; ++first) ::new(static_cast<void*>(std::addressof(*first))) _T();
}
template<class It, class Size> inline It uninitialized_value_construct_n(It first, Size n) {
    typedef typename std::iterator_traits<It>::value_type _T;
    for (; n > 0; --n, ++first) ::new(static_cast<void*>(std::addressof(*first))) _T();
    return first;
}
#if !defined(_MSC_VER) || _MSC_VER < 1910
template<class T, class U> inline std::shared_ptr<T> reinterpret_pointer_cast(const std::shared_ptr<U>& r) {
    typedef typename std::shared_ptr<T>::element_type _E;
    return std::shared_ptr<T>(r, reinterpret_cast<_E*>(r.get()));
}
template<class InIt, class FwdIt> inline FwdIt uninitialized_move(InIt first, InIt last, FwdIt dest) {
    typedef typename std::iterator_traits<FwdIt>::value_type _T;
    for (; first != last; ++first, ++dest)
        ::new(static_cast<void*>(std::addressof(*dest))) _T(::_ccw::move(*first));
    return dest;
}
template<class InIt, class Size, class FwdIt> inline std::pair<InIt, FwdIt>
uninitialized_move_n(InIt first, Size n, FwdIt dest) {
    typedef typename std::iterator_traits<FwdIt>::value_type _T;
    for (; n > 0; --n, ++first, ++dest)
        ::new(static_cast<void*>(std::addressof(*dest))) _T(::_ccw::move(*first));
    return std::pair<InIt, FwdIt>(first, dest);
}
#endif  // MSVC has these from 1910 whatever /std: says
}   // _CCW_STD17
#endif

#if __cplusplus < 202002L
#include <new>
#include "fnctmpl.hpp"
namespace _CCW_STD20 {
#define _CCW_GEN_CONSTRUCT_AT(N)                                                     \
    template<class T _CCW_FNCTMPL_CM_TMPLARG(N)>                                     \
    inline T* construct_at(T* p _CCW_FNCTMPL_CM_DECLARG(N)) {                        \
        return ::new(static_cast<void*>(p)) T(_CCW_FNCTMPL_FNCARG(N));               \
    }
_CCW_FNCTMPL_GENERATE(_CCW_GEN_CONSTRUCT_AT)
#undef _CCW_GEN_CONSTRUCT_AT

template<class T> inline T* to_address(T* p) { return p; }
#if !_CCW_STD_LIB_LT(1600, 201103L)
template<class P> inline typename std::pointer_traits<P>::element_type* to_address(const P& p) {
    return to_address(p.operator->());
}
#endif

template<std::size_t N, class T> inline T* assume_aligned(T* p) { return p; }
}   // _CCW_STD20
#endif

#if !_CCW_STD_LIB_LT(1600, 201103L) && __cplusplus < 201703L && \
    !defined(__cpp_lib_transparent_operators) && !defined(__GLIBCXX__) && \
    (!defined(_MSC_VER) || _MSC_VER < 1900)
namespace std {
template<> struct owner_less<void> {
    typedef void is_transparent;
    template<class T, class U> bool operator()(const shared_ptr<T>& a, const shared_ptr<U>& b) const { return a.owner_before(b); }
    template<class T, class U> bool operator()(const shared_ptr<T>& a, const weak_ptr<U>&   b) const { return a.owner_before(b); }
    template<class T, class U> bool operator()(const weak_ptr<T>&   a, const shared_ptr<U>& b) const { return a.owner_before(b); }
    template<class T, class U> bool operator()(const weak_ptr<T>&   a, const weak_ptr<U>&   b) const { return a.owner_before(b); }
};
}   // namespace std
#endif

#if __cplusplus < 202302L
#include <cstddef>
namespace _CCW_STD23 {
template<class Pointer, class SizeType = std::size_t>
struct allocation_result {
    Pointer  ptr;
    SizeType count;
};
}   // _CCW_STD23
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1400
# pragma warning(pop)
#endif

#endif  // _CCW_DETAIL_MEMORY_HPP
