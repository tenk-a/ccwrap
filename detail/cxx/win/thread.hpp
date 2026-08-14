/**
 *  @file   detail/win/thread.hpp
 *  @brief  <thread> backport over the CRT _beginthreadex + Win32 (pre-C++11).
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_DETAIL_WIN_THREAD_HPP
#define _CCW_DETAIL_WIN_THREAD_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include <process.h>
#include <exception>
#include <chrono>
#include <iosfwd>   /* basic_ostream, for operator<<(thread::id) */
#include <cstddef>
#include <cstdlib>  /* getenv / atoi -- hardware_concurrency */
#include "../move03.hpp"   /* _CCW_RV_REF -- thread is movable, never copyable */

namespace __ccw {
extern "C" {
_CCW_WINAPI(unsigned long) WaitForSingleObject(void*, unsigned long);
_CCW_WINAPI(int) CloseHandle(void*);
_CCW_WINAPI(unsigned long) GetCurrentThreadId(void);
_CCW_WINAPI(int) SwitchToThread(void);
_CCW_WINAPI(void) Sleep(unsigned long);
}
struct __thr_base { virtual ~__thr_base() {} virtual void run() = 0; };
template<class F> struct __thr0 : __thr_base { F f; __thr0(F f_) : f(f_) {} void run() { f(); } };
template<class F, class A1> struct __thr1 : __thr_base { F f; A1 a1; __thr1(F f_, A1 x1) : f(f_), a1(x1) {} void run() { f(a1); } };
template<class F, class A1, class A2> struct __thr2 : __thr_base { F f; A1 a1; A2 a2; __thr2(F f_, A1 x1, A2 x2) : f(f_), a1(x1), a2(x2) {} void run() { f(a1, a2); } };
template<class F, class A1, class A2, class A3> struct __thr3 : __thr_base { F f; A1 a1; A2 a2; A3 a3; __thr3(F f_, A1 x1, A2 x2, A3 x3) : f(f_), a1(x1), a2(x2), a3(x3) {} void run() { f(a1, a2, a3); } };
inline unsigned _ccw_stdcall __thr_run(void* p) { __thr_base* b = (__thr_base*)p; b->run(); delete b; return 0; }
}

namespace std {

class thread {
    void*         h_;
    unsigned long id_;
    thread(const thread&);
    thread& operator=(const thread&);

    void _start(__ccw::__thr_base* b) {
        unsigned id = 0;
        h_ = (void*)_beginthreadex(0, 0, __ccw::__thr_run, b, 0, &id);
        if (!h_) { delete b; id_ = 0; }
        else       id_ = id;
    }
public:
    class id {
        unsigned long v_;
    public:
        id() : v_(0) {}
        explicit id(unsigned long v) : v_(v) {}
        friend bool operator==(id a, id b) { return a.v_ == b.v_; }
        friend bool operator!=(id a, id b) { return a.v_ != b.v_; }
        friend bool operator< (id a, id b) { return a.v_ <  b.v_; }
        friend bool operator> (id a, id b) { return b.v_ <  a.v_; }
        friend bool operator<=(id a, id b) { return !(b.v_ < a.v_); }
        friend bool operator>=(id a, id b) { return !(a.v_ < b.v_); }
        unsigned long __id_value() const { return v_; }   // for hash<thread::id>
    };
    typedef void* native_handle_type;

    thread() : h_(0), id_(0) {}
    template<class F> explicit thread(F f) { _start(new __ccw::__thr0<F>(f)); }
    template<class F, class A1> thread(F f, A1 a1) { _start(new __ccw::__thr1<F, A1>(f, a1)); }
    template<class F, class A1, class A2> thread(F f, A1 a1, A2 a2) { _start(new __ccw::__thr2<F, A1, A2>(f, a1, a2)); }
    template<class F, class A1, class A2, class A3> thread(F f, A1 a1, A2 a2, A3 a3) { _start(new __ccw::__thr3<F, A1, A2, A3>(f, a1, a2, a3)); }
    ~thread() { if (joinable()) std::terminate(); }

    thread(_CCW_RV_REF(thread) o) : h_(0), id_(0) {
        thread& r = _CCW_MOVED_OBJ(o);
        h_ = r.h_; id_ = r.id_; r.h_ = 0; r.id_ = 0;
    }
    thread& operator=(_CCW_RV_REF(thread) o) {
        if (joinable()) std::terminate();
        thread& r = _CCW_MOVED_OBJ(o);
        h_ = r.h_; id_ = r.id_; r.h_ = 0; r.id_ = 0;
        return *this;
    }

    bool joinable() const { return h_ != 0; }
    void join()   { if (h_) { __ccw::WaitForSingleObject(h_, 0xFFFFFFFFul); __ccw::CloseHandle(h_); h_ = 0; id_ = 0; } }
    void detach() { if (h_) { __ccw::CloseHandle(h_); h_ = 0; id_ = 0; } }
    id   get_id() const { return id(id_); }
    native_handle_type native_handle() { return h_; }
    void swap(thread& o) {
        void* th = h_; h_ = o.h_; o.h_ = th;
        unsigned long ti = id_; id_ = o.id_; o.id_ = ti;
    }
    static unsigned hardware_concurrency() {
        const char* e = ::getenv("NUMBER_OF_PROCESSORS");
        int n = e ? ::atoi(e) : 0;
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
    inline thread::id get_id() { return thread::id(__ccw::GetCurrentThreadId()); }
    inline void yield() { __ccw::SwitchToThread(); }
    template<class Rep, class Period>
    inline void sleep_for(const chrono::duration<Rep, Period>& d) {
        __ccw::Sleep((unsigned long)chrono::duration_cast<chrono::milliseconds>(d).count());
    }
    template<class Clock, class Duration>
    inline void sleep_until(const chrono::time_point<Clock, Duration>& tp) {
        typename Clock::time_point now = Clock::now();
        if (tp <= now) { __ccw::Sleep(0); return; }
        sleep_for(tp - now);
    }
}

}   // namespace std

#endif  // _CCW_DETAIL_WIN_THREAD_HPP
