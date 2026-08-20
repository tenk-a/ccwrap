/**
 *  @file   detail/win/shared_mutex.hpp
 *  @brief  <shared_mutex> backport over Win32 SRWLOCK (Vista+).
 *  @license Boost Software License Version 1.0
 *  @note   Included by a forwarder only when native <shared_mutex> is absent, so it
 */
#ifndef _CCW_DETAIL_WIN_SHARED_MUTEX_HPP
#define _CCW_DETAIL_WIN_SHARED_MUTEX_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include "../../c/win/win32_fwd.h"
#include <mutex>   /* defer_lock_t / adopt_lock_t / try_to_lock_t */
#include <condition_variable>   /* shared_timed_mutex is built from these */
#include <exception>
#include "../move03.hpp"        /* _CCW_RV_REF -- shared_lock is move-only */

#if defined(_MSC_VER) && _MSC_VER < 1600
# define _CCW_SRWLOCK_TRY_DYNAMIC 1
#endif
#if defined(_MSC_VER) && _MSC_VER < 1500
# define _CCW_SRWLOCK_DYNAMIC 1
#endif

namespace __ccw {
extern "C" {
#if !defined(_CCW_SRWLOCK_DYNAMIC)
_CCW_WINAPI(void) InitializeSRWLock(__ccw_psrwlock);
_CCW_WINAPI(void) AcquireSRWLockExclusive(__ccw_psrwlock);
_CCW_WINAPI(void) ReleaseSRWLockExclusive(__ccw_psrwlock);
_CCW_WINAPI(void) AcquireSRWLockShared(__ccw_psrwlock);
_CCW_WINAPI(void) ReleaseSRWLockShared(__ccw_psrwlock);
#endif
#if !defined(_CCW_SRWLOCK_TRY_DYNAMIC)
_CCW_WINAPI(unsigned char) TryAcquireSRWLockExclusive(__ccw_psrwlock);
_CCW_WINAPI(unsigned char) TryAcquireSRWLockShared(__ccw_psrwlock);
#endif
}

#if defined(_CCW_SRWLOCK_TRY_DYNAMIC)
typedef unsigned char (_ccw_stdcall* __ccw_try_srw_t)(__ccw_psrwlock);
extern "C" {
_CCW_WINAPI(__ccw_hmodule) GetModuleHandleA(const char*);
_CCW_WINAPI(__ccw_farproc_t) GetProcAddress(__ccw_hmodule, const char*);
}

struct __ccw_srw_try_fns { __ccw_try_srw_t __ex_; __ccw_try_srw_t __sh_; };

inline const __ccw_srw_try_fns& __ccw_srw_try() {
    static __ccw_srw_try_fns __f = { 0, 0 };
    static bool __done = false;
    if (!__done) {
        __ccw_hmodule __k = GetModuleHandleA("kernel32.dll");
        if (__k) {
            __f.__ex_ = reinterpret_cast<__ccw_try_srw_t>(GetProcAddress(__k, "TryAcquireSRWLockExclusive"));
            __f.__sh_ = reinterpret_cast<__ccw_try_srw_t>(GetProcAddress(__k, "TryAcquireSRWLockShared"));
        }
        __done = true;
    }
    return __f;
}
inline unsigned char TryAcquireSRWLockExclusive(__ccw_psrwlock __p) {
    const __ccw_srw_try_fns& __f = __ccw_srw_try();
    return __f.__ex_ ? __f.__ex_(__p) : (unsigned char)0;
}
inline unsigned char TryAcquireSRWLockShared(__ccw_psrwlock __p) {
    const __ccw_srw_try_fns& __f = __ccw_srw_try();
    return __f.__sh_ ? __f.__sh_(__p) : (unsigned char)0;
}
#endif  // _CCW_SRWLOCK_TRY_DYNAMIC

#if defined(_CCW_SRWLOCK_DYNAMIC)
typedef void (_ccw_stdcall* __ccw_srw_t)(__ccw_psrwlock);

struct __ccw_srw_fns {
    __ccw_srw_t __init_, __acq_ex_, __rel_ex_, __acq_sh_, __rel_sh_;
};

inline const __ccw_srw_fns& __ccw_srw() {
    static __ccw_srw_fns __f = { 0, 0, 0, 0, 0 };
    static bool __done = false;
    if (!__done) {
        __ccw_hmodule __k = GetModuleHandleA("kernel32.dll");
        if (__k) {
            __f.__init_   = reinterpret_cast<__ccw_srw_t>(GetProcAddress(__k, "InitializeSRWLock"));
            __f.__acq_ex_ = reinterpret_cast<__ccw_srw_t>(GetProcAddress(__k, "AcquireSRWLockExclusive"));
            __f.__rel_ex_ = reinterpret_cast<__ccw_srw_t>(GetProcAddress(__k, "ReleaseSRWLockExclusive"));
            __f.__acq_sh_ = reinterpret_cast<__ccw_srw_t>(GetProcAddress(__k, "AcquireSRWLockShared"));
            __f.__rel_sh_ = reinterpret_cast<__ccw_srw_t>(GetProcAddress(__k, "ReleaseSRWLockShared"));
        }
        __done = true;
    }
    return __f;
}

inline void InitializeSRWLock(__ccw_psrwlock __p) {
    const __ccw_srw_fns& __f = __ccw_srw();
    if (!__f.__init_) ::std::terminate();
    __f.__init_(__p);
}
inline void AcquireSRWLockExclusive(__ccw_psrwlock __p) {
    const __ccw_srw_fns& __f = __ccw_srw();
    if (!__f.__acq_ex_) ::std::terminate();
    __f.__acq_ex_(__p);
}
inline void ReleaseSRWLockExclusive(__ccw_psrwlock __p) {
    const __ccw_srw_fns& __f = __ccw_srw();
    if (!__f.__rel_ex_) ::std::terminate();
    __f.__rel_ex_(__p);
}
inline void AcquireSRWLockShared(__ccw_psrwlock __p) {
    const __ccw_srw_fns& __f = __ccw_srw();
    if (!__f.__acq_sh_) ::std::terminate();
    __f.__acq_sh_(__p);
}
inline void ReleaseSRWLockShared(__ccw_psrwlock __p) {
    const __ccw_srw_fns& __f = __ccw_srw();
    if (!__f.__rel_sh_) ::std::terminate();
    __f.__rel_sh_(__p);
}
#endif  // _CCW_SRWLOCK_DYNAMIC
}

namespace std {

class shared_mutex {
    void* srw_;   /* SRWLOCK (pointer-sized) */
    shared_mutex(const shared_mutex&);
    shared_mutex& operator=(const shared_mutex&);
public:
    typedef void* native_handle_type;
    shared_mutex() { __ccw::InitializeSRWLock((__ccw_psrwlock)&srw_); }
    ~shared_mutex() {}
    void lock()             { __ccw::AcquireSRWLockExclusive((__ccw_psrwlock)&srw_); }
    bool try_lock()         { return __ccw::TryAcquireSRWLockExclusive((__ccw_psrwlock)&srw_) != 0; }
    void unlock()           { __ccw::ReleaseSRWLockExclusive((__ccw_psrwlock)&srw_); }
    void lock_shared()      { __ccw::AcquireSRWLockShared((__ccw_psrwlock)&srw_); }
    bool try_lock_shared()  { return __ccw::TryAcquireSRWLockShared((__ccw_psrwlock)&srw_) != 0; }
    void unlock_shared()    { __ccw::ReleaseSRWLockShared((__ccw_psrwlock)&srw_); }
    native_handle_type native_handle() { return &srw_; }
};

class shared_timed_mutex {
    mutex               m_;
    condition_variable  gate1_;
    condition_variable  gate2_;
    bool                writer_;
    unsigned            readers_;
    shared_timed_mutex(const shared_timed_mutex&);
    shared_timed_mutex& operator=(const shared_timed_mutex&);
public:
    shared_timed_mutex() : writer_(false), readers_(0) {}
    ~shared_timed_mutex() {}

    void lock() {
        unique_lock<mutex> __lk(m_);
        while (writer_) gate1_.wait(__lk);
        writer_ = true;
        while (readers_) gate2_.wait(__lk);
    }
    bool try_lock() {
        unique_lock<mutex> __lk(m_, try_to_lock);
        if (!__lk.owns_lock() || writer_ || readers_) return false;
        writer_ = true;
        return true;
    }
    void unlock() {
        { unique_lock<mutex> __lk(m_); writer_ = false; }
        gate1_.notify_all();
    }
    template <class _Clock, class _Duration>
    bool try_lock_until(const chrono::time_point<_Clock, _Duration>& __tp) {
        unique_lock<mutex> __lk(m_);
        while (writer_) {
            if (gate1_.wait_until(__lk, __tp) == cv_status::timeout && writer_) return false;
        }
        writer_ = true;
        while (readers_) {
            if (gate2_.wait_until(__lk, __tp) == cv_status::timeout && readers_) {
                writer_ = false;
                __lk.unlock();
                gate1_.notify_all();
                return false;
            }
        }
        return true;
    }
    template <class _Rep, class _Period>
    bool try_lock_for(const chrono::duration<_Rep, _Period>& __d) {
        return try_lock_until(chrono::steady_clock::now() + __d);
    }

    void lock_shared() {
        unique_lock<mutex> __lk(m_);
        while (writer_) gate1_.wait(__lk);
        ++readers_;
    }
    bool try_lock_shared() {
        unique_lock<mutex> __lk(m_, try_to_lock);
        if (!__lk.owns_lock() || writer_) return false;
        ++readers_;
        return true;
    }
    void unlock_shared() {
        unique_lock<mutex> __lk(m_);
        if (--readers_ == 0) {
            __lk.unlock();
            gate2_.notify_one();
        }
    }
    template <class _Clock, class _Duration>
    bool try_lock_shared_until(const chrono::time_point<_Clock, _Duration>& __tp) {
        unique_lock<mutex> __lk(m_);
        while (writer_) {
            if (gate1_.wait_until(__lk, __tp) == cv_status::timeout && writer_) return false;
        }
        ++readers_;
        return true;
    }
    template <class _Rep, class _Period>
    bool try_lock_shared_for(const chrono::duration<_Rep, _Period>& __d) {
        return try_lock_shared_until(chrono::steady_clock::now() + __d);
    }
};

template<class M>
class shared_lock {
    M*   m_;
    bool owns_;
    shared_lock(const shared_lock&);
    shared_lock& operator=(const shared_lock&);
    typedef bool shared_lock::* _bool_type;
public:
    typedef M mutex_type;
    shared_lock() : m_(0), owns_(false) {}
    explicit shared_lock(M& m) : m_(&m), owns_(false) { lock(); }
    shared_lock(M& m, defer_lock_t)  : m_(&m), owns_(false) {}
    shared_lock(M& m, try_to_lock_t) : m_(&m), owns_(false) { owns_ = m_->try_lock_shared(); }
    shared_lock(M& m, adopt_lock_t)  : m_(&m), owns_(true) {}
    template <class _Rep, class _Period>
    shared_lock(M& m, const chrono::duration<_Rep, _Period>& __d) : m_(&m), owns_(false) { owns_ = m_->try_lock_shared_for(__d); }
    template <class _Clock, class _Duration>
    shared_lock(M& m, const chrono::time_point<_Clock, _Duration>& __tp) : m_(&m), owns_(false) { owns_ = m_->try_lock_shared_until(__tp); }
    ~shared_lock() { if (owns_ && m_) m_->unlock_shared(); }

    void lock()     { m_->lock_shared(); owns_ = true; }
    bool try_lock() { owns_ = m_->try_lock_shared(); return owns_; }
    void unlock()   { m_->unlock_shared(); owns_ = false; }
    bool owns_lock() const { return owns_; }
    M*   mutex() const { return m_; }
    M*   release() { M* r = m_; m_ = 0; owns_ = false; return r; }
    template <class _Rep, class _Period>
    bool try_lock_for(const chrono::duration<_Rep, _Period>& __d) { owns_ = m_->try_lock_shared_for(__d); return owns_; }
    template <class _Clock, class _Duration>
    bool try_lock_until(const chrono::time_point<_Clock, _Duration>& __tp) { owns_ = m_->try_lock_shared_until(__tp); return owns_; }
    void swap(shared_lock& __o) {
        M* __m = m_; m_ = __o.m_; __o.m_ = __m;
        bool __b = owns_; owns_ = __o.owns_; __o.owns_ = __b;
    }
    shared_lock(_CCW_RV_REF(shared_lock) __r) : m_(0), owns_(false) {
        shared_lock& __o = _CCW_MOVED_OBJ(__r);
        m_ = __o.m_; owns_ = __o.owns_; __o.m_ = 0; __o.owns_ = false;
    }
    shared_lock& operator=(_CCW_RV_REF(shared_lock) __r) {
        shared_lock& __o = _CCW_MOVED_OBJ(__r);
        if (this != &__o) {
            if (owns_ && m_) m_->unlock_shared();
            m_ = __o.m_; owns_ = __o.owns_; __o.m_ = 0; __o.owns_ = false;
        }
        return *this;
    }
    operator _bool_type() const { return owns_ ? &shared_lock::owns_ : (_bool_type)0; }
};

template<class M>
inline void swap(shared_lock<M>& __a, shared_lock<M>& __b) { __a.swap(__b); }

}   // namespace std

#endif  // _CCW_DETAIL_WIN_SHARED_MUTEX_HPP
