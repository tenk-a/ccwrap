/**
 *  @file   _ccwrap_detail/chrono.hpp
 *  @brief  std::chrono for pre-C++11 VC (vc8/9).
 *  @license Boost Software License Version 1.0
 *  @note   Practical subset: duration / time_point / duration_cast and the
 *          system_clock / steady_clock / high_resolution_clock clocks (via
 *          self-declared Win32). Same-type arithmetic/comparison only (no
 *          common_type mixing). No chrono literals / calendar.
 */
#ifndef _CCW_DETAIL_CHRONO_HPP
#define _CCW_DETAIL_CHRONO_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include <cstdint>
#include <ctime>
#include <ratio>

namespace __ccw {
#if defined(_WIN32)
extern "C" {
_CCW_WINAPI(void) GetSystemTimeAsFileTime(void*);         /* FILETIME* */
_CCW_WINAPI(int) QueryPerformanceCounter(long long*);   /* LARGE_INTEGER* */
_CCW_WINAPI(int) QueryPerformanceFrequency(long long*);
}
#endif
}

namespace std {
namespace chrono {

template<class Rep, class Period = ratio<1> >
class duration {
    Rep rep_;
public:
    typedef Rep    rep;
    typedef Period period;

    duration() : rep_(Rep(0)) {}
    explicit duration(const Rep& r) : rep_(r) {}
    template<class Rep2, class Period2>
    duration(const duration<Rep2, Period2>& d) {
        typedef typename ratio_divide<Period2, Period>::type CF;
        rep_ = static_cast<Rep>(d.count() * CF::num / CF::den);
    }

    Rep count() const { return rep_; }

    duration operator+() const { return *this; }
    duration operator-() const { return duration(Rep(-rep_)); }
    duration& operator++()    { ++rep_; return *this; }
    duration& operator--()    { --rep_; return *this; }
    duration& operator+=(const duration& o) { rep_ += o.rep_; return *this; }
    duration& operator-=(const duration& o) { rep_ -= o.rep_; return *this; }
    duration& operator*=(const Rep& v)      { rep_ *= v; return *this; }
    duration& operator/=(const Rep& v)      { rep_ /= v; return *this; }

    static duration zero() { return duration(Rep(0)); }
};

template<class R, class P> inline duration<R, P> operator+(const duration<R, P>& a, const duration<R, P>& b) { return duration<R, P>(a.count() + b.count()); }
template<class R, class P> inline duration<R, P> operator-(const duration<R, P>& a, const duration<R, P>& b) { return duration<R, P>(a.count() - b.count()); }
template<class R, class P, class R2> inline duration<R, P> operator*(const duration<R, P>& a, const R2& v) { return duration<R, P>(static_cast<R>(a.count() * v)); }
template<class R, class P, class R2> inline duration<R, P> operator*(const R2& v, const duration<R, P>& a) { return duration<R, P>(static_cast<R>(a.count() * v)); }
template<class R, class P, class R2> inline duration<R, P> operator/(const duration<R, P>& a, const R2& v) { return duration<R, P>(static_cast<R>(a.count() / v)); }
template<class R, class P> inline bool operator==(const duration<R, P>& a, const duration<R, P>& b) { return a.count() == b.count(); }
template<class R, class P> inline bool operator!=(const duration<R, P>& a, const duration<R, P>& b) { return a.count() != b.count(); }
template<class R, class P> inline bool operator< (const duration<R, P>& a, const duration<R, P>& b) { return a.count() <  b.count(); }
template<class R, class P> inline bool operator> (const duration<R, P>& a, const duration<R, P>& b) { return a.count() >  b.count(); }
template<class R, class P> inline bool operator<=(const duration<R, P>& a, const duration<R, P>& b) { return a.count() <= b.count(); }
template<class R, class P> inline bool operator>=(const duration<R, P>& a, const duration<R, P>& b) { return a.count() >= b.count(); }

template<class ToDur, class Rep, class Period>
inline ToDur duration_cast(const duration<Rep, Period>& d) {
    typedef typename ratio_divide<Period, typename ToDur::period>::type CF;
    return ToDur(static_cast<typename ToDur::rep>(d.count() * CF::num / CF::den));
}

typedef duration<_ccw_llong, nano>       nanoseconds;
typedef duration<_ccw_llong, micro>      microseconds;
typedef duration<_ccw_llong, milli>      milliseconds;
typedef duration<_ccw_llong>             seconds;
typedef duration<_ccw_llong, ratio<60> > minutes;
typedef duration<_ccw_llong, ratio<3600> > hours;

template<class Clock, class Duration>
class time_point {
    Duration d_;
public:
    typedef Clock                     clock;
    typedef Duration                  duration;
    typedef typename Duration::rep    rep;
    typedef typename Duration::period period;

    time_point() {}
    explicit time_point(const Duration& d) : d_(d) {}
    Duration time_since_epoch() const { return d_; }
    time_point& operator+=(const Duration& d) { d_ += d; return *this; }
    time_point& operator-=(const Duration& d) { d_ -= d; return *this; }
};

template<class C, class D> inline time_point<C, D> operator+(const time_point<C, D>& t, const D& d) { return time_point<C, D>(t.time_since_epoch() + d); }
template<class C, class D> inline time_point<C, D> operator-(const time_point<C, D>& t, const D& d) { return time_point<C, D>(t.time_since_epoch() - d); }
template<class C, class D> inline D operator-(const time_point<C, D>& a, const time_point<C, D>& b) { return a.time_since_epoch() - b.time_since_epoch(); }
template<class C, class D> inline bool operator==(const time_point<C, D>& a, const time_point<C, D>& b) { return a.time_since_epoch() == b.time_since_epoch(); }
template<class C, class D> inline bool operator!=(const time_point<C, D>& a, const time_point<C, D>& b) { return a.time_since_epoch() != b.time_since_epoch(); }
template<class C, class D> inline bool operator< (const time_point<C, D>& a, const time_point<C, D>& b) { return a.time_since_epoch() <  b.time_since_epoch(); }
template<class C, class D> inline bool operator> (const time_point<C, D>& a, const time_point<C, D>& b) { return a.time_since_epoch() >  b.time_since_epoch(); }
template<class C, class D> inline bool operator<=(const time_point<C, D>& a, const time_point<C, D>& b) { return a.time_since_epoch() <= b.time_since_epoch(); }
template<class C, class D> inline bool operator>=(const time_point<C, D>& a, const time_point<C, D>& b) { return a.time_since_epoch() >= b.time_since_epoch(); }

struct system_clock {
    typedef _ccw_llong                            rep;
    typedef micro                                 period;   /* microseconds */
    typedef chrono::duration<rep, period>         duration;
    typedef chrono::time_point<system_clock, duration> time_point;
    static const bool is_steady = false;

    static time_point now() {
#if defined(_WIN32)
        struct { unsigned long lo, hi; } ft;
        __ccw::GetSystemTimeAsFileTime(&ft);
        _ccw_llong t = ((_ccw_llong)ft.hi << 32) | ft.lo;  /* 100ns since 1601 */
        t -= 116444736000000000LL;                          /* -> since 1970 */
        return time_point(duration(t / 10));                /* -> microseconds */
#else
        return time_point(duration((_ccw_llong)::time(0) * 1000000));
#endif
    }
    static time_t to_time_t(const time_point& t) { return (time_t)(t.time_since_epoch().count() / 1000000); }
    static time_point from_time_t(time_t t)      { return time_point(duration((_ccw_llong)t * 1000000)); }
};

struct steady_clock {
    typedef _ccw_llong                            rep;
    typedef nano                                  period;
    typedef chrono::duration<rep, period>         duration;
    typedef chrono::time_point<steady_clock, duration> time_point;
    static const bool is_steady = true;

    static time_point now() {
#if defined(_WIN32)
        _ccw_llong c = 0, f = 1;
        __ccw::QueryPerformanceCounter(&c);
        __ccw::QueryPerformanceFrequency(&f);
        _ccw_llong sec = c / f, rem = c % f;
        return time_point(duration(sec * 1000000000LL + rem * 1000000000LL / f));
#else
        return time_point(duration((_ccw_llong)::clock() * (1000000000LL / CLOCKS_PER_SEC)));
#endif
    }
};

typedef steady_clock high_resolution_clock;

}   // namespace chrono
}   // namespace std

#endif  // _CCW_DETAIL_CHRONO_HPP
