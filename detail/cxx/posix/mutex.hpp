/**
 *  @file   detail/cxx/posix/mutex.hpp
 *  @brief  <mutex> backport over POSIX threads (pre-C++11 fallback).
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_DETAIL_POSIX_MUTEX_HPP
#define _CCW_DETAIL_POSIX_MUTEX_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include <pthread.h>
#include <sched.h>
#include <time.h>
#include <errno.h>
#include <chrono>
#include "../fnctmpl.hpp"

namespace __ccw {

inline void __px_abstime(unsigned long __ms, ::timespec* __ts) {
    ::clock_gettime(CLOCK_REALTIME, __ts);
    __ts->tv_sec  += (long)(__ms / 1000ul);
    __ts->tv_nsec += (long)(__ms % 1000ul) * 1000000L;
    if (__ts->tv_nsec >= 1000000000L) { __ts->tv_nsec -= 1000000000L; ++__ts->tv_sec; }
}

inline void __px_init(::pthread_mutex_t* __m, int __recursive) {
    if (!__recursive) { ::pthread_mutex_init(__m, 0); return; }
    ::pthread_mutexattr_t __a;
    ::pthread_mutexattr_init(&__a);
    ::pthread_mutexattr_settype(&__a, PTHREAD_MUTEX_RECURSIVE);
    ::pthread_mutex_init(__m, &__a);
    ::pthread_mutexattr_destroy(&__a);
}

}   // namespace __ccw

namespace std {

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

class mutex {
    ::pthread_mutex_t m_;
    mutex(const mutex&);
    mutex& operator=(const mutex&);
public:
    typedef ::pthread_mutex_t* native_handle_type;
    mutex()  { ::__ccw::__px_init(&m_, 0); }
    ~mutex() { ::pthread_mutex_destroy(&m_); }
    void lock()     { ::pthread_mutex_lock(&m_); }
    bool try_lock() { return ::pthread_mutex_trylock(&m_) == 0; }
    void unlock()   { ::pthread_mutex_unlock(&m_); }
    native_handle_type native_handle() { return &m_; }
};

class recursive_mutex {
    ::pthread_mutex_t m_;
    recursive_mutex(const recursive_mutex&);
    recursive_mutex& operator=(const recursive_mutex&);
public:
    typedef ::pthread_mutex_t* native_handle_type;
    recursive_mutex()  { ::__ccw::__px_init(&m_, 1); }
    ~recursive_mutex() { ::pthread_mutex_destroy(&m_); }
    void lock()     { ::pthread_mutex_lock(&m_); }
    bool try_lock() { return ::pthread_mutex_trylock(&m_) == 0; }
    void unlock()   { ::pthread_mutex_unlock(&m_); }
    native_handle_type native_handle() { return &m_; }
};

class timed_mutex {
    ::pthread_mutex_t m_;
    timed_mutex(const timed_mutex&);
    timed_mutex& operator=(const timed_mutex&);
    bool __wait(unsigned long __ms) {
        ::timespec __ts;
        ::__ccw::__px_abstime(__ms, &__ts);
        return ::pthread_mutex_timedlock(&m_, &__ts) == 0;
    }
public:
    typedef ::pthread_mutex_t* native_handle_type;
    timed_mutex()  { ::__ccw::__px_init(&m_, 0); }
    ~timed_mutex() { ::pthread_mutex_destroy(&m_); }
    void lock()     { ::pthread_mutex_lock(&m_); }
    bool try_lock() { return ::pthread_mutex_trylock(&m_) == 0; }
    void unlock()   { ::pthread_mutex_unlock(&m_); }
    template <class _Rep, class _Period>
    bool try_lock_for(const chrono::duration<_Rep, _Period>& __d) { return __wait(__ccw_lock_timeout_ms(__d)); }
    template <class _Clock, class _Duration>
    bool try_lock_until(const chrono::time_point<_Clock, _Duration>& __tp) {
        typename _Clock::time_point __now = _Clock::now();
        if (__tp <= __now) return try_lock();
        return try_lock_for(__tp - __now);
    }
    native_handle_type native_handle() { return &m_; }
};

class recursive_timed_mutex {
    ::pthread_mutex_t m_;
    recursive_timed_mutex(const recursive_timed_mutex&);
    recursive_timed_mutex& operator=(const recursive_timed_mutex&);
    bool __wait(unsigned long __ms) {
        ::timespec __ts;
        ::__ccw::__px_abstime(__ms, &__ts);
        return ::pthread_mutex_timedlock(&m_, &__ts) == 0;
    }
public:
    typedef ::pthread_mutex_t* native_handle_type;
    recursive_timed_mutex()  { ::__ccw::__px_init(&m_, 1); }
    ~recursive_timed_mutex() { ::pthread_mutex_destroy(&m_); }
    void lock()     { ::pthread_mutex_lock(&m_); }
    bool try_lock() { return ::pthread_mutex_trylock(&m_) == 0; }
    void unlock()   { ::pthread_mutex_unlock(&m_); }
    template <class _Rep, class _Period>
    bool try_lock_for(const chrono::duration<_Rep, _Period>& __d) { return __wait(__ccw_lock_timeout_ms(__d)); }
    template <class _Clock, class _Duration>
    bool try_lock_until(const chrono::time_point<_Clock, _Duration>& __tp) {
        typename _Clock::time_point __now = _Clock::now();
        if (__tp <= __now) return try_lock();
        return try_lock_for(__tp - __now);
    }
    native_handle_type native_handle() { return &m_; }
};

}   // namespace std

#define _CCW_MTX_YIELD()            ::sched_yield()
#define _CCW_MTX_CAS(p, val, cmp)   __sync_val_compare_and_swap((p), (cmp), (val))
#include "../mutex_generic.hpp"

#endif  // _CCW_DETAIL_POSIX_MUTEX_HPP
