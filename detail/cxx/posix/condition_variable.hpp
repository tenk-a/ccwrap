/**
 *  @file   detail/cxx/posix/condition_variable.hpp
 *  @brief  <condition_variable> backport over POSIX threads (pre-C++11 fallback).
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_DETAIL_POSIX_CONDITION_VARIABLE_HPP
#define _CCW_DETAIL_POSIX_CONDITION_VARIABLE_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include <mutex>
#include "../cv_status.hpp"
#include <pthread.h>
#include <time.h>
#include <errno.h>
#include <chrono>
#include <exception>

namespace std {

class condition_variable {
    ::pthread_cond_t cv_;
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
        ::timespec __ts;
        ::__ccw::__px_abstime(ms, &__ts);
        int r = ::pthread_cond_timedwait(&cv_, lk.mutex()->native_handle(), &__ts);
        return (r == ETIMEDOUT) ? cv_status::timeout : cv_status::no_timeout;
    }
public:
    condition_variable() { ::pthread_cond_init(&cv_, 0); }
    ~condition_variable() { ::pthread_cond_destroy(&cv_); }

    void notify_one() { ::pthread_cond_signal(&cv_); }
    void notify_all() { ::pthread_cond_broadcast(&cv_); }

    void wait(unique_lock<mutex>& lk) {
        ::pthread_cond_wait(&cv_, lk.mutex()->native_handle());
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
            return cv_status::timeout;
        return __wait_ms(lk, __to_ms(tp - now));
    }
    template<class Clock, class Duration, class Pred>
    bool wait_until(unique_lock<mutex>& lk, const chrono::time_point<Clock, Duration>& tp, Pred pred) {
        while (!pred()) {
            if (wait_until(lk, tp) == cv_status::timeout)
                return pred();
        }
        return true;
    }
    typedef ::pthread_cond_t* native_handle_type;
    native_handle_type native_handle() { return &cv_; }
};

}   // namespace std

#include "../condition_variable_any.hpp"

#endif  // _CCW_DETAIL_POSIX_CONDITION_VARIABLE_HPP
