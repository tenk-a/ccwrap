/**
 *  @file   detail/win/mutex.hpp
 *  @brief  <mutex> backport over Win32 CRITICAL_SECTION (pre-C++11 fallback).
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_DETAIL_WIN_MUTEX_HPP
#define _CCW_DETAIL_WIN_MUTEX_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include <intrin.h>
#include <chrono>   /* the timed mutexes take durations and time points */
#include "../fnctmpl.hpp"
#include "../../c/win/win32_fwd.h"

namespace __ccw {
extern "C" {
_CCW_WINAPI(void) InitializeCriticalSection(__ccw_pcritical_section);
_CCW_WINAPI(void) EnterCriticalSection(__ccw_pcritical_section);
_CCW_WINAPI(int) TryEnterCriticalSection(__ccw_pcritical_section);
_CCW_WINAPI(void) LeaveCriticalSection(__ccw_pcritical_section);
_CCW_WINAPI(void) DeleteCriticalSection(__ccw_pcritical_section);
_CCW_WINAPI(void) Sleep(unsigned long);
_CCW_WINAPI(void*) CreateSemaphoreA(__ccw_psecurity_attributes, long, long, const char*);
_CCW_WINAPI(int) ReleaseSemaphore(void*, long, long*);
_CCW_WINAPI(void*) CreateMutexA(__ccw_psecurity_attributes, int, const char*);
_CCW_WINAPI(int) ReleaseMutex(void*);
_CCW_WINAPI(unsigned long) WaitForSingleObject(void*, unsigned long);
_CCW_WINAPI(int) CloseHandle(void*);
}
const unsigned long __ccw_wait_object_0 = 0x00000000ul;
const unsigned long __ccw_infinite      = 0xFFFFFFFFul;
}

namespace std {

class mutex {
    void* cs_[8];   /* holds a CRITICAL_SECTION */
    mutex(const mutex&);
    mutex& operator=(const mutex&);
public:
    typedef void* native_handle_type;
    mutex()  { __ccw::InitializeCriticalSection((__ccw_pcritical_section)cs_); }
    ~mutex() { __ccw::DeleteCriticalSection((__ccw_pcritical_section)cs_); }
    void lock()     { __ccw::EnterCriticalSection((__ccw_pcritical_section)cs_); }
    bool try_lock() { return __ccw::TryEnterCriticalSection((__ccw_pcritical_section)cs_) != 0; }
    void unlock()   { __ccw::LeaveCriticalSection((__ccw_pcritical_section)cs_); }
    native_handle_type native_handle() { return cs_; }
};

class recursive_mutex {   /* CRITICAL_SECTION is already recursive */
    void* cs_[8];
    recursive_mutex(const recursive_mutex&);
    recursive_mutex& operator=(const recursive_mutex&);
public:
    recursive_mutex()  { __ccw::InitializeCriticalSection((__ccw_pcritical_section)cs_); }
    ~recursive_mutex() { __ccw::DeleteCriticalSection((__ccw_pcritical_section)cs_); }
    void lock()     { __ccw::EnterCriticalSection((__ccw_pcritical_section)cs_); }
    bool try_lock() { return __ccw::TryEnterCriticalSection((__ccw_pcritical_section)cs_) != 0; }
    void unlock()   { __ccw::LeaveCriticalSection((__ccw_pcritical_section)cs_); }
};

template <class _Rep, class _Period>
inline unsigned long __ccw_lock_timeout_ms(const chrono::duration<_Rep, _Period>& __d) {
    typedef chrono::milliseconds __ms;
    __ms::rep __n = chrono::duration_cast<__ms>(__d).count();
    if (__n <= 0)
        return __d.count() > 0 ? 1ul : 0ul;
    if (__n >= (__ms::rep)0xFFFFFFFEul)
        return 0xFFFFFFFEul;
    if (chrono::duration_cast<chrono::duration<_Rep, _Period> >(__ms(__n)) < __d)
        ++__n;
    return (unsigned long)__n;
}

class timed_mutex {
    void* h_;   /* a semaphore of maximum count 1 -- unlike a Win32 mutex it is NOT
                 * recursive, which is what timed_mutex requires. */
    timed_mutex(const timed_mutex&);
    timed_mutex& operator=(const timed_mutex&);
    bool __wait(unsigned long __ms) { return __ccw::WaitForSingleObject(h_, __ms) == __ccw::__ccw_wait_object_0; }
public:
    typedef void* native_handle_type;
    timed_mutex()  { h_ = __ccw::CreateSemaphoreA(0, 1, 1, 0); }
    ~timed_mutex() { __ccw::CloseHandle(h_); }
    void lock()     { __wait(__ccw::__ccw_infinite); }
    bool try_lock() { return __wait(0); }
    void unlock()   { __ccw::ReleaseSemaphore(h_, 1, 0); }
    template <class _Rep, class _Period>
    bool try_lock_for(const chrono::duration<_Rep, _Period>& __d) { return __wait(__ccw_lock_timeout_ms(__d)); }
    template <class _Clock, class _Duration>
    bool try_lock_until(const chrono::time_point<_Clock, _Duration>& __tp) {
        typename _Clock::time_point __now = _Clock::now();
        if (__tp <= __now) return try_lock();
        return try_lock_for(__tp - __now);
    }
    native_handle_type native_handle() { return h_; }
};

class recursive_timed_mutex {
    void* h_;   /* a Win32 mutex IS recursive, and ReleaseMutex decrements the count */
    recursive_timed_mutex(const recursive_timed_mutex&);
    recursive_timed_mutex& operator=(const recursive_timed_mutex&);
    bool __wait(unsigned long __ms) { return __ccw::WaitForSingleObject(h_, __ms) == __ccw::__ccw_wait_object_0; }
public:
    typedef void* native_handle_type;
    recursive_timed_mutex()  { h_ = __ccw::CreateMutexA(0, 0, 0); }
    ~recursive_timed_mutex() { __ccw::CloseHandle(h_); }
    void lock()     { __wait(__ccw::__ccw_infinite); }
    bool try_lock() { return __wait(0); }
    void unlock()   { __ccw::ReleaseMutex(h_); }
    template <class _Rep, class _Period>
    bool try_lock_for(const chrono::duration<_Rep, _Period>& __d) { return __wait(__ccw_lock_timeout_ms(__d)); }
    template <class _Clock, class _Duration>
    bool try_lock_until(const chrono::time_point<_Clock, _Duration>& __tp) {
        typename _Clock::time_point __now = _Clock::now();
        if (__tp <= __now) return try_lock();
        return try_lock_for(__tp - __now);
    }
    native_handle_type native_handle() { return h_; }
};

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
        __ccw::Sleep(0);
        {
            unique_lock<_M1> __l1(__m1);
            if (__m0.try_lock()) { __l1.release(); return; }
        }
        __ccw::Sleep(0);
    }
}
template <class _M0, class _M1, class _M2>
inline void lock(_M0& __m0, _M1& __m1, _M2& __m2) {
    for (;;) {
        unique_lock<_M0> __l0(__m0);
        if (try_lock(__m1, __m2) == -1) { __l0.release(); return; }
        __l0.unlock();
        __ccw::Sleep(0);
    }
}
template <class _M0, class _M1, class _M2, class _M3>
inline void lock(_M0& __m0, _M1& __m1, _M2& __m2, _M3& __m3) {
    for (;;) {
        unique_lock<_M0> __l0(__m0);
        if (try_lock(__m1, __m2, __m3) == -1) { __l0.release(); return; }
        __l0.unlock();
        __ccw::Sleep(0);
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
        if (flag._state() == 0 && _InterlockedCompareExchange(&flag._state(), 1, 0) == 0) { f(); flag._state() = 2; return; }
        __ccw::Sleep(0);
    }
}
#define _CCW_GEN_CALL_ONCE(N)                                                        \
    template<class F _CCW_FNCTMPL_CM_TMPLARG(N)>                                     \
    inline void call_once(once_flag& flag, F f _CCW_FNCTMPL_CM_DECLARG(N)) {         \
        for (;;) {                                                                   \
            if (flag._state() == 2) return;                                          \
            if (flag._state() == 0 && _InterlockedCompareExchange(&flag._state(), 1, 0) == 0) { f(_CCW_FNCTMPL_FNCARG(N)); flag._state() = 2; return; } \
            __ccw::Sleep(0);                                                         \
        }                                                                            \
    }
_CCW_GEN_CALL_ONCE(1) _CCW_GEN_CALL_ONCE(2) _CCW_GEN_CALL_ONCE(3)
#undef _CCW_GEN_CALL_ONCE
#else
template<class F, class... A>
inline void call_once(once_flag& flag, F f, A&&... a) {
    for (;;) {
        if (flag._state() == 2) return;
        if (flag._state() == 0 && _InterlockedCompareExchange(&flag._state(), 1, 0) == 0) { f(static_cast<A&&>(a)...); flag._state() = 2; return; }
        __ccw::Sleep(0);
    }
}
#endif

}   // namespace std

#endif  // _CCW_DETAIL_WIN_MUTEX_HPP
