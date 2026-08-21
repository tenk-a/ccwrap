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
    typedef void* native_handle_type;
    native_handle_type native_handle() { return cs_; }
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

}   // namespace std

#define _CCW_MTX_YIELD()            __ccw::Sleep(0)
#define _CCW_MTX_CAS(p, val, cmp)   _InterlockedCompareExchange((p), (val), (cmp))
#include "../mutex_generic.hpp"

#endif  // _CCW_DETAIL_WIN_MUTEX_HPP
