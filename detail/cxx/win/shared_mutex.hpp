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

}   // namespace std

#include "../shared_mutex_generic.hpp"


#endif  // _CCW_DETAIL_WIN_SHARED_MUTEX_HPP
