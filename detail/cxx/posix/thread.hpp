/**
 *  @file   detail/cxx/posix/thread.hpp
 *  @brief  <thread> backport over POSIX threads (pre-C++11 fallback).
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_DETAIL_POSIX_THREAD_HPP
#define _CCW_DETAIL_POSIX_THREAD_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include <pthread.h>
#include <sched.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <exception>
#include <chrono>
#include <iosfwd>   /* basic_ostream, for operator<<(thread::id) */
#include <cstddef>
#include "../move03.hpp"   /* _CCW_RV_REF -- thread is movable, never copyable */

namespace __ccw {
struct __thr_base { virtual ~__thr_base() {} virtual void run() = 0; };
template<class F> struct __thr0 : __thr_base { F f; __thr0(F f_) : f(f_) {} void run() { f(); } };
template<class F, class A1> struct __thr1 : __thr_base { F f; A1 a1; __thr1(F f_, A1 x1) : f(f_), a1(x1) {} void run() { f(a1); } };
template<class F, class A1, class A2> struct __thr2 : __thr_base { F f; A1 a1; A2 a2; __thr2(F f_, A1 x1, A2 x2) : f(f_), a1(x1), a2(x2) {} void run() { f(a1, a2); } };
template<class F, class A1, class A2, class A3> struct __thr3 : __thr_base { F f; A1 a1; A2 a2; A3 a3; __thr3(F f_, A1 x1, A2 x2, A3 x3) : f(f_), a1(x1), a2(x2), a3(x3) {} void run() { f(a1, a2, a3); } };
inline void* __thr_run(void* p) { __thr_base* b = (__thr_base*)p; b->run(); delete b; return 0; }
}

namespace std {

class thread {
    ::pthread_t h_;
    bool        started_;
    thread(const thread&);
    thread& operator=(const thread&);

    void _start(__ccw::__thr_base* b) {
        if (::pthread_create(&h_, 0, __ccw::__thr_run, b) != 0) { delete b; started_ = false; }
        else                                                      started_ = true;
    }
public:
    class id {
        ::pthread_t v_;
        bool        set_;
    public:
        id() : set_(false) {}
        explicit id(::pthread_t v) : v_(v), set_(true) {}
        friend bool operator==(id a, id b) {
            if (!a.set_ || !b.set_) return a.set_ == b.set_;
            return ::pthread_equal(a.v_, b.v_) != 0;
        }
        friend bool operator!=(id a, id b) { return !(a == b); }
        friend bool operator< (id a, id b) { return a.__id_value() <  b.__id_value(); }
        friend bool operator> (id a, id b) { return b < a; }
        friend bool operator<=(id a, id b) { return !(b < a); }
        friend bool operator>=(id a, id b) { return !(a < b); }
        unsigned long __id_value() const {          // for hash<thread::id> and operator<<
            if (!set_) return 0ul;
            unsigned long __r = 0;
            const unsigned char* __p = (const unsigned char*)&v_;
            for (std::size_t __i = 0; __i < sizeof(::pthread_t); ++__i) __r = __r * 131 + __p[__i];
            return __r;
        }
    };
    typedef ::pthread_t native_handle_type;

    thread() : started_(false) {}
    template<class F> explicit thread(F f) { _start(new __ccw::__thr0<F>(f)); }
    template<class F, class A1> thread(F f, A1 a1) { _start(new __ccw::__thr1<F, A1>(f, a1)); }
    template<class F, class A1, class A2> thread(F f, A1 a1, A2 a2) { _start(new __ccw::__thr2<F, A1, A2>(f, a1, a2)); }
    template<class F, class A1, class A2, class A3> thread(F f, A1 a1, A2 a2, A3 a3) { _start(new __ccw::__thr3<F, A1, A2, A3>(f, a1, a2, a3)); }
    ~thread() { if (joinable()) std::terminate(); }

    thread(_CCW_RV_REF(thread) o) : started_(false) {
        thread& r = _CCW_MOVED_OBJ(o);
        h_ = r.h_; started_ = r.started_; r.started_ = false;
    }
    thread& operator=(_CCW_RV_REF(thread) o) {
        if (joinable()) std::terminate();
        thread& r = _CCW_MOVED_OBJ(o);
        h_ = r.h_; started_ = r.started_; r.started_ = false;
        return *this;
    }

    bool joinable() const { return started_; }
    void join()   { if (started_) { ::pthread_join(h_, 0); started_ = false; } }
    void detach() { if (started_) { ::pthread_detach(h_); started_ = false; } }
    id   get_id() const { return started_ ? id(h_) : id(); }
    native_handle_type native_handle() { return h_; }
    void swap(thread& o) {
        ::pthread_t th = h_; h_ = o.h_; o.h_ = th;
        bool ts = started_; started_ = o.started_; o.started_ = ts;
    }
    static unsigned hardware_concurrency() {
        long n = ::sysconf(_SC_NPROCESSORS_ONLN);
        return n > 0 ? (unsigned)n : 1u;
    }
};

inline void swap(thread& a, thread& b) { a.swap(b); }

template <class _CharT, class _Traits>
basic_ostream<_CharT, _Traits>& operator<<(basic_ostream<_CharT, _Traits>& __os, thread::id __i) {
    return __os << __i.__id_value();
}

template <class _Tp> struct hash;
template <> struct hash<thread::id> {
    typedef thread::id  argument_type;
    typedef std::size_t result_type;
    std::size_t operator()(thread::id __i) const { return (std::size_t)__i.__id_value(); }
};

namespace this_thread {
    inline thread::id get_id() { return thread::id(::pthread_self()); }
    inline void yield() { ::sched_yield(); }
    template<class Rep, class Period>
    inline void sleep_for(const chrono::duration<Rep, Period>& d) {
        chrono::nanoseconds __n = chrono::duration_cast<chrono::nanoseconds>(d);
        if (__n.count() <= 0) { ::sched_yield(); return; }
        ::timespec __ts;
        __ts.tv_sec  = (long)(__n.count() / 1000000000LL);
        __ts.tv_nsec = (long)(__n.count() % 1000000000LL);
        while (::nanosleep(&__ts, &__ts) == -1 && errno == EINTR) {}
    }
    template<class Clock, class Duration>
    inline void sleep_until(const chrono::time_point<Clock, Duration>& tp) {
        for (;;) {
            typename Clock::time_point now = Clock::now();
            if (tp <= now) return;
            sleep_for(tp - now);
        }
    }
}

}   // namespace std

#endif  // _CCW_DETAIL_POSIX_THREAD_HPP
