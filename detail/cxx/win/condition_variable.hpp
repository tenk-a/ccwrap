/**
 *  @file   detail/win/condition_variable.hpp
 *  @brief  <condition_variable> backport over Win32 CONDITION_VARIABLE (Vista+).
 *  @license Boost Software License Version 1.0
 *  @note   Included by a forwarder only when native <condition_variable> is absent,
 *          so it lives directly in namespace std. Uses the std::mutex from
 *          detail/win/mutex.hpp (native_handle() is the CRITICAL_SECTION).
 */
#ifndef _CCW_DETAIL_WIN_CONDITION_VARIABLE_HPP
#define _CCW_DETAIL_WIN_CONDITION_VARIABLE_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include <mutex>
#include "../../c/win/win32_fwd.h"
#include <chrono>
#include <exception>

#if defined(_MSC_VER) && _MSC_VER < 1500
/* The 2005 Platform SDK import library predates Vista (vc bugs D5). */
# define _CCW_CONDVAR_DYNAMIC 1
#endif

namespace __ccw {
#if !defined(_CCW_CONDVAR_DYNAMIC)
extern "C" {
_CCW_WINAPI(void) InitializeConditionVariable(__ccw_pcondition_variable);
_CCW_WINAPI(int) SleepConditionVariableCS(__ccw_pcondition_variable, __ccw_pcritical_section, unsigned long);
_CCW_WINAPI(void) WakeConditionVariable(__ccw_pcondition_variable);
_CCW_WINAPI(void) WakeAllConditionVariable(__ccw_pcondition_variable);
}
#else
typedef int (_ccw_stdcall* __ccw_farproc_t)();
extern "C" {
_CCW_WINAPI(__ccw_hmodule) GetModuleHandleA(const char*);
_CCW_WINAPI(__ccw_farproc_t) GetProcAddress(__ccw_hmodule, const char*);
}

typedef void (_ccw_stdcall* __ccw_cv_void_t)(__ccw_pcondition_variable);
typedef int  (_ccw_stdcall* __ccw_cv_sleep_t)(__ccw_pcondition_variable, __ccw_pcritical_section, unsigned long);

struct __ccw_cv_fns {
    __ccw_cv_void_t  __init_, __wake_, __wake_all_;
    __ccw_cv_sleep_t __sleep_;
};

inline const __ccw_cv_fns& __ccw_cv() {
    static __ccw_cv_fns __f = { 0, 0, 0, 0 };
    static bool __done = false;
    if (!__done) {
        __ccw_hmodule __k = GetModuleHandleA("kernel32.dll");
        if (__k) {
            __f.__init_     = reinterpret_cast<__ccw_cv_void_t>(GetProcAddress(__k, "InitializeConditionVariable"));
            __f.__wake_     = reinterpret_cast<__ccw_cv_void_t>(GetProcAddress(__k, "WakeConditionVariable"));
            __f.__wake_all_ = reinterpret_cast<__ccw_cv_void_t>(GetProcAddress(__k, "WakeAllConditionVariable"));
            __f.__sleep_    = reinterpret_cast<__ccw_cv_sleep_t>(GetProcAddress(__k, "SleepConditionVariableCS"));
        }
        __done = true;
    }
    return __f;
}

inline void InitializeConditionVariable(__ccw_pcondition_variable __p) {
    const __ccw_cv_fns& __f = __ccw_cv();
    if (!__f.__init_) ::std::terminate();
    __f.__init_(__p);
}
inline void WakeConditionVariable(__ccw_pcondition_variable __p) {
    const __ccw_cv_fns& __f = __ccw_cv();
    if (!__f.__wake_) ::std::terminate();
    __f.__wake_(__p);
}
inline void WakeAllConditionVariable(__ccw_pcondition_variable __p) {
    const __ccw_cv_fns& __f = __ccw_cv();
    if (!__f.__wake_all_) ::std::terminate();
    __f.__wake_all_(__p);
}
inline int SleepConditionVariableCS(__ccw_pcondition_variable __p, __ccw_pcritical_section __m, unsigned long __ms) {
    const __ccw_cv_fns& __f = __ccw_cv();
    if (!__f.__sleep_) ::std::terminate();
    return __f.__sleep_(__p, __m, __ms);
}
#endif  // _CCW_CONDVAR_DYNAMIC
}

namespace std {

enum cv_status { no_timeout, timeout };

class condition_variable {
    void* cv_;   /* CONDITION_VARIABLE (pointer-sized) */
    condition_variable(const condition_variable&);
    condition_variable& operator=(const condition_variable&);

    template<class Rep, class Period>
    static unsigned long __to_ms(const chrono::duration<Rep, Period>& d) {
        typedef chrono::milliseconds __ms;
        __ms::rep n = chrono::duration_cast<__ms>(d).count();
        if (n <= 0)
            return d.count() > 0 ? 1ul : 0ul;
        if (n >= (__ms::rep)0xFFFFFFFEul)
            return 0xFFFFFFFEul;
        if (chrono::duration_cast<chrono::duration<Rep, Period> >(__ms(n)) < d)
            ++n;
        return (unsigned long)n;
    }
    cv_status __wait_ms(unique_lock<mutex>& lk, unsigned long ms) {
        int r = __ccw::SleepConditionVariableCS((__ccw_pcondition_variable)&cv_,
            (__ccw_pcritical_section)lk.mutex()->native_handle(), ms);
        return r ? no_timeout : timeout;
    }
public:
    condition_variable() { __ccw::InitializeConditionVariable((__ccw_pcondition_variable)&cv_); }
    ~condition_variable() {}

    void notify_one() { __ccw::WakeConditionVariable((__ccw_pcondition_variable)&cv_); }
    void notify_all() { __ccw::WakeAllConditionVariable((__ccw_pcondition_variable)&cv_); }

    void wait(unique_lock<mutex>& lk) {
        __ccw::SleepConditionVariableCS((__ccw_pcondition_variable)&cv_,
            (__ccw_pcritical_section)lk.mutex()->native_handle(), 0xFFFFFFFFul);
    }
    template<class Pred>
    void wait(unique_lock<mutex>& lk, Pred pred) { while (!pred()) wait(lk); }

    template<class Rep, class Period>
    cv_status wait_for(unique_lock<mutex>& lk, const chrono::duration<Rep, Period>& d) {
        return __wait_ms(lk, __to_ms(d));
    }
    template<class Rep, class Period, class Pred>
    bool wait_for(unique_lock<mutex>& lk, const chrono::duration<Rep, Period>& d, Pred pred) {
        return wait_until(lk, chrono::steady_clock::now() + d, pred);
    }

    template<class Clock, class Duration>
    cv_status wait_until(unique_lock<mutex>& lk, const chrono::time_point<Clock, Duration>& tp) {
        typename Clock::time_point now = Clock::now();
        if (tp <= now)
            return timeout;
        return __wait_ms(lk, __to_ms(tp - now));
    }
    template<class Clock, class Duration, class Pred>
    bool wait_until(unique_lock<mutex>& lk, const chrono::time_point<Clock, Duration>& tp, Pred pred) {
        while (!pred()) {
            if (wait_until(lk, tp) == timeout)
                return pred();
        }
        return true;
    }
    typedef void* native_handle_type;
    native_handle_type native_handle() { return &cv_; }
};

inline void __ccw_cv_wait(condition_variable& __cv, unique_lock<mutex>& __ilk) { __cv.wait(__ilk); }
template <class _Clock, class _Duration>
inline cv_status __ccw_cv_wait_until(condition_variable& __cv, unique_lock<mutex>& __ilk,
                                     const chrono::time_point<_Clock, _Duration>& __tp) {
    return __cv.wait_until(__ilk, __tp);
}

class condition_variable_any {
    mutex              m_;
    condition_variable cv_;
    condition_variable_any(const condition_variable_any&);
    condition_variable_any& operator=(const condition_variable_any&);

public:
    condition_variable_any() {}
    ~condition_variable_any() {}

    void notify_one() { { lock_guard<mutex> __g(m_); } cv_.notify_one(); }
    void notify_all() { { lock_guard<mutex> __g(m_); } cv_.notify_all(); }

    template <class _Lock>
    void wait(_Lock& __ext) {
        {
            unique_lock<mutex> __ilk(m_);
            __ext.unlock();
            __ccw_cv_wait(cv_, __ilk);
        }
        __ext.lock();
    }
    template <class _Lock, class _Pred>
    void wait(_Lock& __ext, _Pred __p) { while (!__p()) wait(__ext); }

    template <class _Lock, class _Rep, class _Period>
    cv_status wait_for(_Lock& __ext, const chrono::duration<_Rep, _Period>& __d) {
        return wait_until(__ext, chrono::steady_clock::now() + __d);
    }
    template <class _Lock, class _Rep, class _Period, class _Pred>
    bool wait_for(_Lock& __ext, const chrono::duration<_Rep, _Period>& __d, _Pred __p) {
        return wait_until(__ext, chrono::steady_clock::now() + __d, __p);
    }
    template <class _Lock, class _Clock, class _Duration>
    cv_status wait_until(_Lock& __ext, const chrono::time_point<_Clock, _Duration>& __tp) {
        cv_status __s;
        {
            unique_lock<mutex> __ilk(m_);
            __ext.unlock();
            __s = __ccw_cv_wait_until(cv_, __ilk, __tp);
        }
        __ext.lock();
        return __s;
    }
    template <class _Lock, class _Clock, class _Duration, class _Pred>
    bool wait_until(_Lock& __ext, const chrono::time_point<_Clock, _Duration>& __tp, _Pred __p) {
        while (!__p()) {
            if (wait_until(__ext, __tp) == timeout)
                return __p();
        }
        return true;
    }
};

}   // namespace std

#endif  // _CCW_DETAIL_WIN_CONDITION_VARIABLE_HPP
