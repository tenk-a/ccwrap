/**
 *  @file   detail/cxx/mutex_generic.hpp
 *  @brief  The OS-independent half of the pre-C++11 <mutex> backport: the lock
 *          tags, lock_guard / unique_lock / scoped_lock, lock / try_lock and
 *          once_flag / call_once.
 *  @license Boost Software License Version 1.0
 *  @note   Included by detail/cxx/win/mutex.hpp and detail/cxx/posix/mutex.hpp
 *          after they have defined the four mutex classes and the two hooks
 *          _CCW_MTX_YIELD() and _CCW_MTX_CAS(p, cmp, val).
 */
#ifndef _CCW_DETAIL_MUTEX_GENERIC_HPP
#define _CCW_DETAIL_MUTEX_GENERIC_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include "fnctmpl.hpp"

namespace std {

struct defer_lock_t  {};
struct try_to_lock_t {};
struct adopt_lock_t  {};
_ccw_inline_const defer_lock_t  defer_lock  = defer_lock_t();
_ccw_inline_const try_to_lock_t try_to_lock = try_to_lock_t();
_ccw_inline_const adopt_lock_t  adopt_lock  = adopt_lock_t();

template<class M>
class lock_guard {
    M& m_;
    lock_guard(const lock_guard&);
    lock_guard& operator=(const lock_guard&);
public:
    typedef M mutex_type;
    explicit lock_guard(M& m) : m_(m) { m_.lock(); }
    lock_guard(M& m, adopt_lock_t) : m_(m) {}
    ~lock_guard() { m_.unlock(); }
};

template<class M>
class unique_lock {
    M*   m_;
    bool owns_;
    unique_lock(const unique_lock&);
    unique_lock& operator=(const unique_lock&);
    typedef bool unique_lock::* _bool_type;
public:
    typedef M mutex_type;
    unique_lock() : m_(0), owns_(false) {}
    explicit unique_lock(M& m) : m_(&m), owns_(false) { lock(); }
    unique_lock(M& m, defer_lock_t)  : m_(&m), owns_(false) {}
    unique_lock(M& m, try_to_lock_t) : m_(&m), owns_(false) { owns_ = m_->try_lock(); }
    unique_lock(M& m, adopt_lock_t)  : m_(&m), owns_(true) {}
    template <class _Rep, class _Period>
    unique_lock(M& m, const chrono::duration<_Rep, _Period>& __d) : m_(&m), owns_(false) { owns_ = m_->try_lock_for(__d); }
    template <class _Clock, class _Duration>
    unique_lock(M& m, const chrono::time_point<_Clock, _Duration>& __tp) : m_(&m), owns_(false) { owns_ = m_->try_lock_until(__tp); }
    ~unique_lock() { if (owns_ && m_) m_->unlock(); }

    void lock()     { m_->lock(); owns_ = true; }
    bool try_lock() { owns_ = m_->try_lock(); return owns_; }
    void unlock()   { m_->unlock(); owns_ = false; }
    bool owns_lock() const { return owns_; }
    M*   mutex() const { return m_; }
    M*   release() { M* r = m_; m_ = 0; owns_ = false; return r; }
    template <class _Rep, class _Period>
    bool try_lock_for(const chrono::duration<_Rep, _Period>& __d) { owns_ = m_->try_lock_for(__d); return owns_; }
    template <class _Clock, class _Duration>
    bool try_lock_until(const chrono::time_point<_Clock, _Duration>& __tp) { owns_ = m_->try_lock_until(__tp); return owns_; }
    void swap(unique_lock& __o) {
        M* __m = m_; m_ = __o.m_; __o.m_ = __m;
        bool __b = owns_; owns_ = __o.owns_; __o.owns_ = __b;
    }
    operator _bool_type() const { return owns_ ? &unique_lock::owns_ : (_bool_type)0; }
};

template <class M>
inline void swap(unique_lock<M>& __a, unique_lock<M>& __b) { __a.swap(__b); }

template <class _M0, class _M1>
inline int try_lock(_M0& __m0, _M1& __m1) {
    unique_lock<_M0> __l0(__m0, try_to_lock);
    if (!__l0.owns_lock()) return 0;
    if (!__m1.try_lock())  return 1;
    __l0.release();
    return -1;
}
template <class _M0, class _M1, class _M2>
inline int try_lock(_M0& __m0, _M1& __m1, _M2& __m2) {
    unique_lock<_M0> __l0(__m0, try_to_lock);
    if (!__l0.owns_lock()) return 0;
    int __r = try_lock(__m1, __m2);
    if (__r != -1) return __r + 1;
    __l0.release();
    return -1;
}
template <class _M0, class _M1, class _M2, class _M3>
inline int try_lock(_M0& __m0, _M1& __m1, _M2& __m2, _M3& __m3) {
    unique_lock<_M0> __l0(__m0, try_to_lock);
    if (!__l0.owns_lock()) return 0;
    int __r = try_lock(__m1, __m2, __m3);
    if (__r != -1) return __r + 1;
    __l0.release();
    return -1;
}

template <class _M0, class _M1>
inline void lock(_M0& __m0, _M1& __m1) {
    for (;;) {
        {
            unique_lock<_M0> __l0(__m0);
            if (__m1.try_lock()) { __l0.release(); return; }
        }
        _CCW_MTX_YIELD();
        {
            unique_lock<_M1> __l1(__m1);
            if (__m0.try_lock()) { __l1.release(); return; }
        }
        _CCW_MTX_YIELD();
    }
}
template <class _M0, class _M1, class _M2>
inline void lock(_M0& __m0, _M1& __m1, _M2& __m2) {
    for (;;) {
        unique_lock<_M0> __l0(__m0);
        if (try_lock(__m1, __m2) == -1) { __l0.release(); return; }
        __l0.unlock();
        _CCW_MTX_YIELD();
    }
}
template <class _M0, class _M1, class _M2, class _M3>
inline void lock(_M0& __m0, _M1& __m1, _M2& __m2, _M3& __m3) {
    for (;;) {
        unique_lock<_M0> __l0(__m0);
        if (try_lock(__m1, __m2, __m3) == -1) { __l0.release(); return; }
        __l0.unlock();
        _CCW_MTX_YIELD();
    }
}

class __ccw_sl_nil {
public:
    void lock() {}
    bool try_lock() { return true; }
    void unlock() {}
};

template <class _M0, class _M1, class _M2, class _M3>
class scoped_lock;
template <class _M0 = __ccw_sl_nil, class _M1 = __ccw_sl_nil, class _M2 = __ccw_sl_nil, class _M3 = __ccw_sl_nil>
class scoped_lock {
    _M0* __p0_; _M1* __p1_; _M2* __p2_; _M3* __p3_;
    scoped_lock(const scoped_lock&);
    scoped_lock& operator=(const scoped_lock&);
public:
    typedef _M0 mutex_type;      // only meaningful when there is exactly one
    scoped_lock() : __p0_(0), __p1_(0), __p2_(0), __p3_(0) {}
    explicit scoped_lock(_M0& __m0) : __p0_(&__m0), __p1_(0), __p2_(0), __p3_(0) { __p0_->lock(); }
    scoped_lock(_M0& __m0, _M1& __m1) : __p0_(&__m0), __p1_(&__m1), __p2_(0), __p3_(0) { std::lock(__m0, __m1); }
    scoped_lock(_M0& __m0, _M1& __m1, _M2& __m2) : __p0_(&__m0), __p1_(&__m1), __p2_(&__m2), __p3_(0) { std::lock(__m0, __m1, __m2); }
    scoped_lock(_M0& __m0, _M1& __m1, _M2& __m2, _M3& __m3) : __p0_(&__m0), __p1_(&__m1), __p2_(&__m2), __p3_(&__m3) { std::lock(__m0, __m1, __m2, __m3); }
    scoped_lock(adopt_lock_t, _M0& __m0) : __p0_(&__m0), __p1_(0), __p2_(0), __p3_(0) {}
    scoped_lock(adopt_lock_t, _M0& __m0, _M1& __m1) : __p0_(&__m0), __p1_(&__m1), __p2_(0), __p3_(0) {}
    scoped_lock(adopt_lock_t, _M0& __m0, _M1& __m1, _M2& __m2) : __p0_(&__m0), __p1_(&__m1), __p2_(&__m2), __p3_(0) {}
    scoped_lock(adopt_lock_t, _M0& __m0, _M1& __m1, _M2& __m2, _M3& __m3) : __p0_(&__m0), __p1_(&__m1), __p2_(&__m2), __p3_(&__m3) {}
    ~scoped_lock() {
        if (__p3_) __p3_->unlock();
        if (__p2_) __p2_->unlock();
        if (__p1_) __p1_->unlock();
        if (__p0_) __p0_->unlock();
    }
};

class once_flag {
    long s_;
    once_flag(const once_flag&);
    once_flag& operator=(const once_flag&);
public:
    once_flag() : s_(0) {}
    long& _state() { return s_; }
};

#if _CCW_NO_VARIADIC_TEMPLATES
template<class F>
inline void call_once(once_flag& flag, F f) {
    for (;;) {
        if (flag._state() == 2) return;
        if (flag._state() == 0 && _CCW_MTX_CAS(&flag._state(), 1, 0) == 0) { f(); flag._state() = 2; return; }
        _CCW_MTX_YIELD();
    }
}
#define _CCW_GEN_CALL_ONCE(N)                                                        \
    template<class F _CCW_FNCTMPL_CM_TMPLARG(N)>                                     \
    inline void call_once(once_flag& flag, F f _CCW_FNCTMPL_CM_DECLARG(N)) {         \
        for (;;) {                                                                   \
            if (flag._state() == 2) return;                                          \
            if (flag._state() == 0 && _CCW_MTX_CAS(&flag._state(), 1, 0) == 0) { f(_CCW_FNCTMPL_FNCARG(N)); flag._state() = 2; return; } \
            _CCW_MTX_YIELD();                                                        \
        }                                                                            \
    }
_CCW_GEN_CALL_ONCE(1) _CCW_GEN_CALL_ONCE(2) _CCW_GEN_CALL_ONCE(3)
#undef _CCW_GEN_CALL_ONCE
#else
template<class F, class... A>
inline void call_once(once_flag& flag, F f, A&&... a) {
    for (;;) {
        if (flag._state() == 2) return;
        if (flag._state() == 0 && _CCW_MTX_CAS(&flag._state(), 1, 0) == 0) { f(static_cast<A&&>(a)...); flag._state() = 2; return; }
        _CCW_MTX_YIELD();
    }
}
#endif

}   // namespace std

#endif  // _CCW_DETAIL_MUTEX_GENERIC_HPP
