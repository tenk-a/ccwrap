/**
 *  @file   detail/cxx/condition_variable_any.hpp
 *  @brief  std::condition_variable_any for the pre-C++11 <condition_variable>
 *          backports. Built entirely from std::mutex and std::condition_variable,
 *          so the same body serves every OS layer.
 *  @license Boost Software License Version 1.0
 *  @note   Included by detail/cxx/{win,posix}/condition_variable.hpp after they
 *          have defined std::condition_variable.
 */
#ifndef _CCW_DETAIL_CONDITION_VARIABLE_ANY_HPP
#define _CCW_DETAIL_CONDITION_VARIABLE_ANY_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include <chrono>

namespace std {

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
            if (wait_until(__ext, __tp) == cv_status::timeout)
                return __p();
        }
        return true;
    }
};

}   // namespace std

#endif  // _CCW_DETAIL_CONDITION_VARIABLE_ANY_HPP
