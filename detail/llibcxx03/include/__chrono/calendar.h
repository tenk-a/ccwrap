// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
//  __chrono/calendar.h -- the C++20 civil calendar ([time.cal]) for C++03.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___CHRONO_CALENDAR_H
#define _CCW_LIBCPP___CHRONO_CALENDAR_H

#include "../__config"

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

namespace chrono {

typedef duration<long long, ratio<86400> >   days;
typedef duration<long long, ratio<604800> >  weeks;
typedef duration<long long, ratio<2629746> > months;
typedef duration<long long, ratio<31556952> > years;

struct local_t {};

typedef time_point<system_clock, days> sys_days;
typedef time_point<local_t, days>      local_days;
typedef time_point<system_clock, seconds> sys_seconds;
typedef time_point<local_t, seconds>      local_seconds;
#if _CCW_HAS_ALIAS_TEMPLATE
template <class _Dur> using sys_time   = time_point<system_clock, _Dur>;
template <class _Dur> using local_time = time_point<local_t, _Dur>;
#endif

struct last_spec {
    _CCW_LIBCPP_HIDE_FROM_ABI explicit last_spec() {}
};
static const last_spec last = last_spec();

_CCW_LIBCPP_HIDE_FROM_ABI inline long long __ccw_days_from_civil(int __y, unsigned __m, unsigned __d) {
    __y -= __m <= 2;
    const long long __era = (__y >= 0 ? __y : __y - 399) / 400;
    const unsigned  __yoe = (unsigned)(__y - __era * 400);                     // [0, 399]
    const unsigned  __doy = (153u * (__m + (__m > 2 ? -3 : 9)) + 2u) / 5u + __d - 1u;  // [0, 365]
    const unsigned  __doe = __yoe * 365u + __yoe / 4u - __yoe / 100u + __doy;  // [0, 146096]
    return __era * 146097LL + (long long)__doe - 719468LL;
}

_CCW_LIBCPP_HIDE_FROM_ABI inline void __ccw_civil_from_days(long long __z, int& __y, unsigned& __m, unsigned& __d) {
    __z += 719468LL;
    const long long __era = (__z >= 0 ? __z : __z - 146096LL) / 146097LL;
    const unsigned  __doe = (unsigned)(__z - __era * 146097LL);                // [0, 146096]
    const unsigned  __yoe = (__doe - __doe / 1460u + __doe / 36524u - __doe / 146096u) / 365u;
    const long long __yy  = (long long)__yoe + __era * 400LL;
    const unsigned  __doy = __doe - (365u * __yoe + __yoe / 4u - __yoe / 100u);
    const unsigned  __mp  = (5u * __doy + 2u) / 153u;
    __d = __doy - (153u * __mp + 2u) / 5u + 1u;                                // [1, 31]
    __m = __mp + (__mp < 10u ? 3u : -9u);                                      // [1, 12]
    __y = (int)(__yy + (__m <= 2u));
}

_CCW_LIBCPP_HIDE_FROM_ABI inline bool __ccw_is_leap(int __y) {
    return (__y % 4 == 0 && __y % 100 != 0) || __y % 400 == 0;
}

_CCW_LIBCPP_HIDE_FROM_ABI inline unsigned __ccw_last_day(int __y, unsigned __m) {
    static const unsigned __t[13] = { 0u, 31u, 28u, 31u, 30u, 31u, 30u, 31u, 31u, 30u, 31u, 30u, 31u };
    if (__m < 1u || __m > 12u) return 0u;
    if (__m == 2u && __ccw_is_leap(__y)) return 29u;
    return __t[__m];
}

_CCW_LIBCPP_HIDE_FROM_ABI inline long long __ccw_emod(long long __a, long long __n) {
    long long __r = __a % __n;
    return __r < 0 ? __r + __n : __r;
}

class day {
    unsigned char __d_;
public:
    _CCW_LIBCPP_HIDE_FROM_ABI day() : __d_(0) {}
    _CCW_LIBCPP_HIDE_FROM_ABI explicit day(unsigned __d) : __d_((unsigned char)__d) {}

    _CCW_LIBCPP_HIDE_FROM_ABI day& operator++()    { ++__d_; return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI day  operator++(int) { day __t(*this); ++__d_; return __t; }
    _CCW_LIBCPP_HIDE_FROM_ABI day& operator--()    { --__d_; return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI day  operator--(int) { day __t(*this); --__d_; return __t; }
    _CCW_LIBCPP_HIDE_FROM_ABI day& operator+=(const days& __d) { __d_ = (unsigned char)(__d_ + __d.count()); return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI day& operator-=(const days& __d) { __d_ = (unsigned char)(__d_ - __d.count()); return *this; }

    _CCW_LIBCPP_HIDE_FROM_ABI operator unsigned() const { return __d_; }
    _CCW_LIBCPP_HIDE_FROM_ABI bool ok() const { return __d_ >= 1u && __d_ <= 31u; }
};

_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator==(const day& __x, const day& __y) { return (unsigned)__x == (unsigned)__y; }
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator!=(const day& __x, const day& __y) { return !(__x == __y); }
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator< (const day& __x, const day& __y) { return (unsigned)__x <  (unsigned)__y; }
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator> (const day& __x, const day& __y) { return __y < __x; }
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator<=(const day& __x, const day& __y) { return !(__y < __x); }
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator>=(const day& __x, const day& __y) { return !(__x < __y); }
_CCW_LIBCPP_HIDE_FROM_ABI inline day  operator+(const day& __x, const days& __y) { return day((unsigned)((unsigned)__x + __y.count())); }
_CCW_LIBCPP_HIDE_FROM_ABI inline day  operator+(const days& __x, const day& __y) { return __y + __x; }
_CCW_LIBCPP_HIDE_FROM_ABI inline day  operator-(const day& __x, const days& __y) { return day((unsigned)((unsigned)__x - __y.count())); }
_CCW_LIBCPP_HIDE_FROM_ABI inline days operator-(const day& __x, const day& __y)  { return days((long long)(unsigned)__x - (long long)(unsigned)__y); }

class month {
    unsigned char __m_;
public:
    _CCW_LIBCPP_HIDE_FROM_ABI month() : __m_(0) {}
    _CCW_LIBCPP_HIDE_FROM_ABI explicit month(unsigned __m) : __m_((unsigned char)__m) {}

    _CCW_LIBCPP_HIDE_FROM_ABI month& operator++()    { *this += months(1); return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI month  operator++(int) { month __t(*this); ++(*this); return __t; }
    _CCW_LIBCPP_HIDE_FROM_ABI month& operator--()    { *this -= months(1); return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI month  operator--(int) { month __t(*this); --(*this); return __t; }
    _CCW_LIBCPP_HIDE_FROM_ABI month& operator+=(const months& __m) {
        __m_ = (unsigned char)(__ccw_emod((long long)__m_ - 1 + __m.count(), 12) + 1);
        return *this;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI month& operator-=(const months& __m) { return *this += months(-__m.count()); }

    _CCW_LIBCPP_HIDE_FROM_ABI operator unsigned() const { return __m_; }
    _CCW_LIBCPP_HIDE_FROM_ABI bool ok() const { return __m_ >= 1u && __m_ <= 12u; }
};

_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator==(const month& __x, const month& __y) { return (unsigned)__x == (unsigned)__y; }
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator!=(const month& __x, const month& __y) { return !(__x == __y); }
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator< (const month& __x, const month& __y) { return (unsigned)__x <  (unsigned)__y; }
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator> (const month& __x, const month& __y) { return __y < __x; }
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator<=(const month& __x, const month& __y) { return !(__y < __x); }
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator>=(const month& __x, const month& __y) { return !(__x < __y); }
_CCW_LIBCPP_HIDE_FROM_ABI inline month  operator+(const month& __x, const months& __y) { month __t(__x); __t += __y; return __t; }
_CCW_LIBCPP_HIDE_FROM_ABI inline month  operator+(const months& __x, const month& __y) { return __y + __x; }
_CCW_LIBCPP_HIDE_FROM_ABI inline month  operator-(const month& __x, const months& __y) { return __x + months(-__y.count()); }
_CCW_LIBCPP_HIDE_FROM_ABI inline months operator-(const month& __x, const month& __y)  {
    return months(__ccw_emod((long long)(unsigned)__x - (long long)(unsigned)__y, 12));
}

class year {
    short __y_;
public:
    _CCW_LIBCPP_HIDE_FROM_ABI year() : __y_(0) {}
    _CCW_LIBCPP_HIDE_FROM_ABI explicit year(int __y) : __y_((short)__y) {}

    _CCW_LIBCPP_HIDE_FROM_ABI year& operator++()    { ++__y_; return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI year  operator++(int) { year __t(*this); ++__y_; return __t; }
    _CCW_LIBCPP_HIDE_FROM_ABI year& operator--()    { --__y_; return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI year  operator--(int) { year __t(*this); --__y_; return __t; }
    _CCW_LIBCPP_HIDE_FROM_ABI year& operator+=(const years& __y) { __y_ = (short)(__y_ + __y.count()); return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI year& operator-=(const years& __y) { __y_ = (short)(__y_ - __y.count()); return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI year  operator+() const { return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI year  operator-() const { return year(-__y_); }

    _CCW_LIBCPP_HIDE_FROM_ABI bool is_leap() const { return __ccw_is_leap(__y_); }
    _CCW_LIBCPP_HIDE_FROM_ABI operator int() const { return __y_; }
    _CCW_LIBCPP_HIDE_FROM_ABI bool ok() const { return __y_ >= -32767; }
    _CCW_LIBCPP_HIDE_FROM_ABI static year min() { return year(-32767); }
    _CCW_LIBCPP_HIDE_FROM_ABI static year max() { return year( 32767); }
};

_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator==(const year& __x, const year& __y) { return (int)__x == (int)__y; }
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator!=(const year& __x, const year& __y) { return !(__x == __y); }
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator< (const year& __x, const year& __y) { return (int)__x <  (int)__y; }
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator> (const year& __x, const year& __y) { return __y < __x; }
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator<=(const year& __x, const year& __y) { return !(__y < __x); }
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator>=(const year& __x, const year& __y) { return !(__x < __y); }
_CCW_LIBCPP_HIDE_FROM_ABI inline year  operator+(const year& __x, const years& __y) { return year((int)((int)__x + __y.count())); }
_CCW_LIBCPP_HIDE_FROM_ABI inline year  operator+(const years& __x, const year& __y) { return __y + __x; }
_CCW_LIBCPP_HIDE_FROM_ABI inline year  operator-(const year& __x, const years& __y) { return year((int)((int)__x - __y.count())); }
_CCW_LIBCPP_HIDE_FROM_ABI inline years operator-(const year& __x, const year& __y)  { return years((long long)(int)__x - (long long)(int)__y); }

class weekday_indexed;
class weekday_last;

class weekday {
    unsigned char __w_;
    _CCW_LIBCPP_HIDE_FROM_ABI static unsigned char __from_days(long long __d) {
        return (unsigned char)__ccw_emod(__d + 4, 7);   // 1970-01-01 was a Thursday
    }
public:
    _CCW_LIBCPP_HIDE_FROM_ABI weekday() : __w_(7) {}
    _CCW_LIBCPP_HIDE_FROM_ABI explicit weekday(unsigned __w) : __w_((unsigned char)(__w == 7u ? 0u : __w)) {}
    _CCW_LIBCPP_HIDE_FROM_ABI weekday(const sys_days& __t) : __w_(__from_days(__t.time_since_epoch().count())) {}
    _CCW_LIBCPP_HIDE_FROM_ABI explicit weekday(const local_days& __t) : __w_(__from_days(__t.time_since_epoch().count())) {}

    _CCW_LIBCPP_HIDE_FROM_ABI weekday& operator++()    { *this += days(1); return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI weekday  operator++(int) { weekday __t(*this); ++(*this); return __t; }
    _CCW_LIBCPP_HIDE_FROM_ABI weekday& operator--()    { *this -= days(1); return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI weekday  operator--(int) { weekday __t(*this); --(*this); return __t; }
    _CCW_LIBCPP_HIDE_FROM_ABI weekday& operator+=(const days& __d) { __w_ = (unsigned char)__ccw_emod((long long)__w_ + __d.count(), 7); return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI weekday& operator-=(const days& __d) { return *this += days(-__d.count()); }

    _CCW_LIBCPP_HIDE_FROM_ABI unsigned c_encoding() const   { return __w_; }
    _CCW_LIBCPP_HIDE_FROM_ABI unsigned iso_encoding() const { return __w_ == 0u ? 7u : __w_; }
    _CCW_LIBCPP_HIDE_FROM_ABI bool ok() const { return __w_ <= 6u; }

    _CCW_LIBCPP_HIDE_FROM_ABI weekday_indexed operator[](unsigned __i) const;
    _CCW_LIBCPP_HIDE_FROM_ABI weekday_last    operator[](last_spec) const;
};

_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator==(const weekday& __x, const weekday& __y) { return __x.c_encoding() == __y.c_encoding(); }
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator!=(const weekday& __x, const weekday& __y) { return !(__x == __y); }
_CCW_LIBCPP_HIDE_FROM_ABI inline weekday operator+(const weekday& __x, const days& __y) { weekday __t(__x); __t += __y; return __t; }
_CCW_LIBCPP_HIDE_FROM_ABI inline weekday operator+(const days& __x, const weekday& __y) { return __y + __x; }
_CCW_LIBCPP_HIDE_FROM_ABI inline weekday operator-(const weekday& __x, const days& __y) { return __x + days(-__y.count()); }
_CCW_LIBCPP_HIDE_FROM_ABI inline days    operator-(const weekday& __x, const weekday& __y) {
    return days(__ccw_emod((long long)__x.c_encoding() - (long long)__y.c_encoding(), 7));
}

class weekday_indexed {
    chrono::weekday __wd_;
    unsigned char   __i_;
public:
    _CCW_LIBCPP_HIDE_FROM_ABI weekday_indexed() : __i_(0) {}
    _CCW_LIBCPP_HIDE_FROM_ABI weekday_indexed(const chrono::weekday& __w, unsigned __i)
        : __wd_(__w), __i_((unsigned char)__i) {}
    _CCW_LIBCPP_HIDE_FROM_ABI chrono::weekday weekday() const { return __wd_; }
    _CCW_LIBCPP_HIDE_FROM_ABI unsigned index() const { return __i_; }
    _CCW_LIBCPP_HIDE_FROM_ABI bool ok() const { return __wd_.ok() && __i_ >= 1u && __i_ <= 5u; }
};

_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator==(const weekday_indexed& __x, const weekday_indexed& __y) {
    return __x.weekday() == __y.weekday() && __x.index() == __y.index();
}
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator!=(const weekday_indexed& __x, const weekday_indexed& __y) { return !(__x == __y); }

class weekday_last {
    chrono::weekday __wd_;
public:
    _CCW_LIBCPP_HIDE_FROM_ABI weekday_last() {}
    _CCW_LIBCPP_HIDE_FROM_ABI explicit weekday_last(const chrono::weekday& __w) : __wd_(__w) {}
    _CCW_LIBCPP_HIDE_FROM_ABI chrono::weekday weekday() const { return __wd_; }
    _CCW_LIBCPP_HIDE_FROM_ABI bool ok() const { return __wd_.ok(); }
};

_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator==(const weekday_last& __x, const weekday_last& __y) { return __x.weekday() == __y.weekday(); }
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator!=(const weekday_last& __x, const weekday_last& __y) { return !(__x == __y); }

_CCW_LIBCPP_HIDE_FROM_ABI inline weekday_indexed weekday::operator[](unsigned __i) const { return weekday_indexed(*this, __i); }
_CCW_LIBCPP_HIDE_FROM_ABI inline weekday_last    weekday::operator[](last_spec) const    { return weekday_last(*this); }

class month_day {
    chrono::month __m_;
    chrono::day   __d_;
public:
    _CCW_LIBCPP_HIDE_FROM_ABI month_day() {}
    _CCW_LIBCPP_HIDE_FROM_ABI month_day(const chrono::month& __m, const chrono::day& __d) : __m_(__m), __d_(__d) {}
    _CCW_LIBCPP_HIDE_FROM_ABI chrono::month month() const { return __m_; }
    _CCW_LIBCPP_HIDE_FROM_ABI chrono::day   day() const   { return __d_; }
    _CCW_LIBCPP_HIDE_FROM_ABI bool ok() const {
        if (!__m_.ok() || (unsigned)__d_ < 1u) return false;
        unsigned __m = (unsigned)__m_;
        return (unsigned)__d_ <= (__m == 2u ? 29u : __ccw_last_day(2001, __m));
    }
};

_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator==(const month_day& __x, const month_day& __y) {
    return __x.month() == __y.month() && __x.day() == __y.day();
}
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator!=(const month_day& __x, const month_day& __y) { return !(__x == __y); }
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator< (const month_day& __x, const month_day& __y) {
    if (__x.month() != __y.month()) return __x.month() < __y.month();
    return __x.day() < __y.day();
}
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator> (const month_day& __x, const month_day& __y) { return __y < __x; }
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator<=(const month_day& __x, const month_day& __y) { return !(__y < __x); }
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator>=(const month_day& __x, const month_day& __y) { return !(__x < __y); }

class month_day_last {
    chrono::month __m_;
public:
    _CCW_LIBCPP_HIDE_FROM_ABI explicit month_day_last(const chrono::month& __m) : __m_(__m) {}
    _CCW_LIBCPP_HIDE_FROM_ABI chrono::month month() const { return __m_; }
    _CCW_LIBCPP_HIDE_FROM_ABI bool ok() const { return __m_.ok(); }
};

_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator==(const month_day_last& __x, const month_day_last& __y) { return __x.month() == __y.month(); }
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator!=(const month_day_last& __x, const month_day_last& __y) { return !(__x == __y); }
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator< (const month_day_last& __x, const month_day_last& __y) { return __x.month() < __y.month(); }
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator> (const month_day_last& __x, const month_day_last& __y) { return __y < __x; }
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator<=(const month_day_last& __x, const month_day_last& __y) { return !(__y < __x); }
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator>=(const month_day_last& __x, const month_day_last& __y) { return !(__x < __y); }

class month_weekday {
    chrono::month           __m_;
    chrono::weekday_indexed __wdi_;
public:
    _CCW_LIBCPP_HIDE_FROM_ABI month_weekday(const chrono::month& __m, const chrono::weekday_indexed& __w)
        : __m_(__m), __wdi_(__w) {}
    _CCW_LIBCPP_HIDE_FROM_ABI chrono::month month() const { return __m_; }
    _CCW_LIBCPP_HIDE_FROM_ABI chrono::weekday_indexed weekday_indexed() const { return __wdi_; }
    _CCW_LIBCPP_HIDE_FROM_ABI bool ok() const { return __m_.ok() && __wdi_.ok(); }
};

_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator==(const month_weekday& __x, const month_weekday& __y) {
    return __x.month() == __y.month() && __x.weekday_indexed() == __y.weekday_indexed();
}
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator!=(const month_weekday& __x, const month_weekday& __y) { return !(__x == __y); }

class month_weekday_last {
    chrono::month        __m_;
    chrono::weekday_last __wdl_;
public:
    _CCW_LIBCPP_HIDE_FROM_ABI month_weekday_last(const chrono::month& __m, const chrono::weekday_last& __w)
        : __m_(__m), __wdl_(__w) {}
    _CCW_LIBCPP_HIDE_FROM_ABI chrono::month month() const { return __m_; }
    _CCW_LIBCPP_HIDE_FROM_ABI chrono::weekday_last weekday_last() const { return __wdl_; }
    _CCW_LIBCPP_HIDE_FROM_ABI bool ok() const { return __m_.ok() && __wdl_.ok(); }
};

_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator==(const month_weekday_last& __x, const month_weekday_last& __y) {
    return __x.month() == __y.month() && __x.weekday_last() == __y.weekday_last();
}
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator!=(const month_weekday_last& __x, const month_weekday_last& __y) { return !(__x == __y); }

class year_month {
    chrono::year  __y_;
    chrono::month __m_;
public:
    _CCW_LIBCPP_HIDE_FROM_ABI year_month() {}
    _CCW_LIBCPP_HIDE_FROM_ABI year_month(const chrono::year& __y, const chrono::month& __m) : __y_(__y), __m_(__m) {}
    _CCW_LIBCPP_HIDE_FROM_ABI chrono::year  year() const  { return __y_; }
    _CCW_LIBCPP_HIDE_FROM_ABI chrono::month month() const { return __m_; }
    _CCW_LIBCPP_HIDE_FROM_ABI bool ok() const { return __y_.ok() && __m_.ok(); }

    _CCW_LIBCPP_HIDE_FROM_ABI year_month& operator+=(const months& __dm) {
        long long __t = (long long)(int)__y_ * 12 + ((long long)(unsigned)__m_ - 1) + __dm.count();
        __y_ = chrono::year((int)(__t >= 0 ? __t / 12 : -((-__t + 11) / 12)));
        __m_ = chrono::month((unsigned)(__ccw_emod(__t, 12) + 1));
        return *this;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI year_month& operator-=(const months& __dm) { return *this += months(-__dm.count()); }
    _CCW_LIBCPP_HIDE_FROM_ABI year_month& operator+=(const years& __dy)  { __y_ += __dy; return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI year_month& operator-=(const years& __dy)  { __y_ -= __dy; return *this; }
};

_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator==(const year_month& __x, const year_month& __y) {
    return __x.year() == __y.year() && __x.month() == __y.month();
}
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator!=(const year_month& __x, const year_month& __y) { return !(__x == __y); }
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator< (const year_month& __x, const year_month& __y) {
    if (__x.year() != __y.year()) return __x.year() < __y.year();
    return __x.month() < __y.month();
}
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator> (const year_month& __x, const year_month& __y) { return __y < __x; }
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator<=(const year_month& __x, const year_month& __y) { return !(__y < __x); }
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator>=(const year_month& __x, const year_month& __y) { return !(__x < __y); }
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month operator+(const year_month& __x, const months& __y) { year_month __t(__x); __t += __y; return __t; }
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month operator+(const months& __x, const year_month& __y) { return __y + __x; }
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month operator-(const year_month& __x, const months& __y) { return __x + months(-__y.count()); }
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month operator+(const year_month& __x, const years& __y)  { year_month __t(__x); __t += __y; return __t; }
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month operator+(const years& __x, const year_month& __y)  { return __y + __x; }
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month operator-(const year_month& __x, const years& __y)  { return __x + years(-__y.count()); }
_CCW_LIBCPP_HIDE_FROM_ABI inline months     operator-(const year_month& __x, const year_month& __y) {
    return months(((long long)(int)__x.year() - (long long)(int)__y.year()) * 12
                  + ((long long)(unsigned)__x.month() - (long long)(unsigned)__y.month()));
}

class year_month_day_last;

class year_month_day {
    chrono::year  __y_;
    chrono::month __m_;
    chrono::day   __d_;
public:
    _CCW_LIBCPP_HIDE_FROM_ABI year_month_day() {}
    _CCW_LIBCPP_HIDE_FROM_ABI year_month_day(const chrono::year& __y, const chrono::month& __m, const chrono::day& __d)
        : __y_(__y), __m_(__m), __d_(__d) {}
    _CCW_LIBCPP_HIDE_FROM_ABI year_month_day(const year_month_day_last& __ymdl);
    _CCW_LIBCPP_HIDE_FROM_ABI year_month_day(const sys_days& __t)   { __from_days(__t.time_since_epoch().count()); }
    _CCW_LIBCPP_HIDE_FROM_ABI explicit year_month_day(const local_days& __t) { __from_days(__t.time_since_epoch().count()); }

    _CCW_LIBCPP_HIDE_FROM_ABI chrono::year  year() const  { return __y_; }
    _CCW_LIBCPP_HIDE_FROM_ABI chrono::month month() const { return __m_; }
    _CCW_LIBCPP_HIDE_FROM_ABI chrono::day   day() const   { return __d_; }

    _CCW_LIBCPP_HIDE_FROM_ABI year_month_day& operator+=(const months& __dm) {
        year_month __ym(__y_, __m_); __ym += __dm;
        __y_ = __ym.year(); __m_ = __ym.month();
        return *this;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI year_month_day& operator-=(const months& __dm) { return *this += months(-__dm.count()); }
    _CCW_LIBCPP_HIDE_FROM_ABI year_month_day& operator+=(const years& __dy)  { __y_ += __dy; return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI year_month_day& operator-=(const years& __dy)  { __y_ -= __dy; return *this; }

    _CCW_LIBCPP_HIDE_FROM_ABI long long __count() const {
        return __ccw_days_from_civil((int)__y_, (unsigned)__m_, (unsigned)__d_);
    }
    _CCW_LIBCPP_HIDE_FROM_ABI operator sys_days() const   { return sys_days(days(__count())); }
    _CCW_LIBCPP_HIDE_FROM_ABI operator local_days() const { return local_days(days(__count())); }

    _CCW_LIBCPP_HIDE_FROM_ABI bool ok() const {
        if (!__y_.ok() || !__m_.ok()) return false;
        unsigned __d = (unsigned)__d_;
        return __d >= 1u && __d <= __ccw_last_day((int)__y_, (unsigned)__m_);
    }
private:
    _CCW_LIBCPP_HIDE_FROM_ABI void __from_days(long long __z) {
        int __y; unsigned __m, __d;
        __ccw_civil_from_days(__z, __y, __m, __d);
        __y_ = chrono::year(__y); __m_ = chrono::month(__m); __d_ = chrono::day(__d);
    }
};

_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator==(const year_month_day& __x, const year_month_day& __y) {
    return __x.year() == __y.year() && __x.month() == __y.month() && __x.day() == __y.day();
}
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator!=(const year_month_day& __x, const year_month_day& __y) { return !(__x == __y); }
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator< (const year_month_day& __x, const year_month_day& __y) {
    if (__x.year()  != __y.year())  return __x.year()  < __y.year();
    if (__x.month() != __y.month()) return __x.month() < __y.month();
    return __x.day() < __y.day();
}
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator> (const year_month_day& __x, const year_month_day& __y) { return __y < __x; }
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator<=(const year_month_day& __x, const year_month_day& __y) { return !(__y < __x); }
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator>=(const year_month_day& __x, const year_month_day& __y) { return !(__x < __y); }
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_day operator+(const year_month_day& __x, const months& __y) { year_month_day __t(__x); __t += __y; return __t; }
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_day operator+(const months& __x, const year_month_day& __y) { return __y + __x; }
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_day operator-(const year_month_day& __x, const months& __y) { return __x + months(-__y.count()); }
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_day operator+(const year_month_day& __x, const years& __y)  { year_month_day __t(__x); __t += __y; return __t; }
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_day operator+(const years& __x, const year_month_day& __y)  { return __y + __x; }
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_day operator-(const year_month_day& __x, const years& __y)  { return __x + years(-__y.count()); }

class year_month_day_last {
    chrono::year           __y_;
    chrono::month_day_last __mdl_;
public:
    _CCW_LIBCPP_HIDE_FROM_ABI year_month_day_last(const chrono::year& __y, const chrono::month_day_last& __mdl)
        : __y_(__y), __mdl_(__mdl) {}

    _CCW_LIBCPP_HIDE_FROM_ABI chrono::year  year() const  { return __y_; }
    _CCW_LIBCPP_HIDE_FROM_ABI chrono::month month() const { return __mdl_.month(); }
    _CCW_LIBCPP_HIDE_FROM_ABI chrono::month_day_last month_day_last() const { return __mdl_; }
    _CCW_LIBCPP_HIDE_FROM_ABI chrono::day day() const {
        return chrono::day(__ccw_last_day((int)__y_, (unsigned)__mdl_.month()));
    }

    _CCW_LIBCPP_HIDE_FROM_ABI year_month_day_last& operator+=(const months& __dm) {
        year_month __ym(__y_, __mdl_.month()); __ym += __dm;
        __y_ = __ym.year(); __mdl_ = chrono::month_day_last(__ym.month());
        return *this;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI year_month_day_last& operator-=(const months& __dm) { return *this += months(-__dm.count()); }
    _CCW_LIBCPP_HIDE_FROM_ABI year_month_day_last& operator+=(const years& __dy)  { __y_ += __dy; return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI year_month_day_last& operator-=(const years& __dy)  { __y_ -= __dy; return *this; }

    _CCW_LIBCPP_HIDE_FROM_ABI long long __count() const {
        return __ccw_days_from_civil((int)__y_, (unsigned)__mdl_.month(), (unsigned)day());
    }
    _CCW_LIBCPP_HIDE_FROM_ABI operator sys_days() const   { return sys_days(days(__count())); }
    _CCW_LIBCPP_HIDE_FROM_ABI operator local_days() const { return local_days(days(__count())); }
    _CCW_LIBCPP_HIDE_FROM_ABI bool ok() const { return __y_.ok() && __mdl_.ok(); }
};

_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_day::year_month_day(const year_month_day_last& __ymdl)
    : __y_(__ymdl.year()), __m_(__ymdl.month()), __d_(__ymdl.day()) {}

_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator==(const year_month_day_last& __x, const year_month_day_last& __y) {
    return __x.year() == __y.year() && __x.month_day_last() == __y.month_day_last();
}
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator!=(const year_month_day_last& __x, const year_month_day_last& __y) { return !(__x == __y); }
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator< (const year_month_day_last& __x, const year_month_day_last& __y) {
    if (__x.year() != __y.year()) return __x.year() < __y.year();
    return __x.month_day_last() < __y.month_day_last();
}
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator> (const year_month_day_last& __x, const year_month_day_last& __y) { return __y < __x; }
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator<=(const year_month_day_last& __x, const year_month_day_last& __y) { return !(__y < __x); }
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator>=(const year_month_day_last& __x, const year_month_day_last& __y) { return !(__x < __y); }
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_day_last operator+(const year_month_day_last& __x, const months& __y) { year_month_day_last __t(__x); __t += __y; return __t; }
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_day_last operator+(const months& __x, const year_month_day_last& __y) { return __y + __x; }
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_day_last operator-(const year_month_day_last& __x, const months& __y) { return __x + months(-__y.count()); }
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_day_last operator+(const year_month_day_last& __x, const years& __y)  { year_month_day_last __t(__x); __t += __y; return __t; }
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_day_last operator+(const years& __x, const year_month_day_last& __y)  { return __y + __x; }
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_day_last operator-(const year_month_day_last& __x, const years& __y)  { return __x + years(-__y.count()); }

class year_month_weekday {
    chrono::year            __y_;
    chrono::month           __m_;
    chrono::weekday_indexed __wdi_;
public:
    _CCW_LIBCPP_HIDE_FROM_ABI year_month_weekday() {}
    _CCW_LIBCPP_HIDE_FROM_ABI year_month_weekday(const chrono::year& __y, const chrono::month& __m,
                                                 const chrono::weekday_indexed& __wdi)
        : __y_(__y), __m_(__m), __wdi_(__wdi) {}
    _CCW_LIBCPP_HIDE_FROM_ABI year_month_weekday(const sys_days& __t)   { __from_days(__t.time_since_epoch().count()); }
    _CCW_LIBCPP_HIDE_FROM_ABI explicit year_month_weekday(const local_days& __t) { __from_days(__t.time_since_epoch().count()); }

    _CCW_LIBCPP_HIDE_FROM_ABI chrono::year    year() const    { return __y_; }
    _CCW_LIBCPP_HIDE_FROM_ABI chrono::month   month() const   { return __m_; }
    _CCW_LIBCPP_HIDE_FROM_ABI chrono::weekday weekday() const { return __wdi_.weekday(); }
    _CCW_LIBCPP_HIDE_FROM_ABI unsigned        index() const   { return __wdi_.index(); }
    _CCW_LIBCPP_HIDE_FROM_ABI chrono::weekday_indexed weekday_indexed() const { return __wdi_; }

    _CCW_LIBCPP_HIDE_FROM_ABI year_month_weekday& operator+=(const months& __dm) {
        year_month __ym(__y_, __m_); __ym += __dm;
        __y_ = __ym.year(); __m_ = __ym.month();
        return *this;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI year_month_weekday& operator-=(const months& __dm) { return *this += months(-__dm.count()); }
    _CCW_LIBCPP_HIDE_FROM_ABI year_month_weekday& operator+=(const years& __dy)  { __y_ += __dy; return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI year_month_weekday& operator-=(const years& __dy)  { __y_ -= __dy; return *this; }

    _CCW_LIBCPP_HIDE_FROM_ABI long long __count() const {
        long long __first = __ccw_days_from_civil((int)__y_, (unsigned)__m_, 1u);
        unsigned __wd1 = (unsigned)__ccw_emod(__first + 4, 7);
        long long __fwd = __ccw_emod((long long)__wdi_.weekday().c_encoding() - (long long)__wd1, 7);
        return __first + __fwd + 7LL * ((long long)__wdi_.index() - 1);
    }
    _CCW_LIBCPP_HIDE_FROM_ABI operator sys_days() const   { return sys_days(days(__count())); }
    _CCW_LIBCPP_HIDE_FROM_ABI operator local_days() const { return local_days(days(__count())); }

    _CCW_LIBCPP_HIDE_FROM_ABI bool ok() const {
        if (!__y_.ok() || !__m_.ok() || !__wdi_.ok()) return false;
        long long __first = __ccw_days_from_civil((int)__y_, (unsigned)__m_, 1u);
        unsigned __wd1 = (unsigned)__ccw_emod(__first + 4, 7);
        long long __fwd = __ccw_emod((long long)__wdi_.weekday().c_encoding() - (long long)__wd1, 7);
        long long __dom = __fwd + 7LL * ((long long)__wdi_.index() - 1) + 1;
        return __dom <= (long long)__ccw_last_day((int)__y_, (unsigned)__m_);
    }
private:
    _CCW_LIBCPP_HIDE_FROM_ABI void __from_days(long long __z) {
        int __y; unsigned __m, __d;
        __ccw_civil_from_days(__z, __y, __m, __d);
        __y_ = chrono::year(__y); __m_ = chrono::month(__m);
        __wdi_ = chrono::weekday_indexed(chrono::weekday((unsigned)__ccw_emod(__z + 4, 7)), (__d - 1u) / 7u + 1u);
    }
};

_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator==(const year_month_weekday& __x, const year_month_weekday& __y) {
    return __x.year() == __y.year() && __x.month() == __y.month() && __x.weekday_indexed() == __y.weekday_indexed();
}
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator!=(const year_month_weekday& __x, const year_month_weekday& __y) { return !(__x == __y); }
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_weekday operator+(const year_month_weekday& __x, const months& __y) { year_month_weekday __t(__x); __t += __y; return __t; }
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_weekday operator+(const months& __x, const year_month_weekday& __y) { return __y + __x; }
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_weekday operator-(const year_month_weekday& __x, const months& __y) { return __x + months(-__y.count()); }
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_weekday operator+(const year_month_weekday& __x, const years& __y)  { year_month_weekday __t(__x); __t += __y; return __t; }
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_weekday operator+(const years& __x, const year_month_weekday& __y)  { return __y + __x; }
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_weekday operator-(const year_month_weekday& __x, const years& __y)  { return __x + years(-__y.count()); }

class year_month_weekday_last {
    chrono::year         __y_;
    chrono::month        __m_;
    chrono::weekday_last __wdl_;
public:
    _CCW_LIBCPP_HIDE_FROM_ABI year_month_weekday_last(const chrono::year& __y, const chrono::month& __m,
                                                      const chrono::weekday_last& __wdl)
        : __y_(__y), __m_(__m), __wdl_(__wdl) {}

    _CCW_LIBCPP_HIDE_FROM_ABI chrono::year    year() const    { return __y_; }
    _CCW_LIBCPP_HIDE_FROM_ABI chrono::month   month() const   { return __m_; }
    _CCW_LIBCPP_HIDE_FROM_ABI chrono::weekday weekday() const { return __wdl_.weekday(); }
    _CCW_LIBCPP_HIDE_FROM_ABI chrono::weekday_last weekday_last() const { return __wdl_; }

    _CCW_LIBCPP_HIDE_FROM_ABI year_month_weekday_last& operator+=(const months& __dm) {
        year_month __ym(__y_, __m_); __ym += __dm;
        __y_ = __ym.year(); __m_ = __ym.month();
        return *this;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI year_month_weekday_last& operator-=(const months& __dm) { return *this += months(-__dm.count()); }
    _CCW_LIBCPP_HIDE_FROM_ABI year_month_weekday_last& operator+=(const years& __dy)  { __y_ += __dy; return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI year_month_weekday_last& operator-=(const years& __dy)  { __y_ -= __dy; return *this; }

    _CCW_LIBCPP_HIDE_FROM_ABI long long __count() const {
        unsigned __ld = __ccw_last_day((int)__y_, (unsigned)__m_);
        long long __lastd = __ccw_days_from_civil((int)__y_, (unsigned)__m_, __ld ? __ld : 1u);
        unsigned __wdl = (unsigned)__ccw_emod(__lastd + 4, 7);
        long long __back = __ccw_emod((long long)__wdl - (long long)__wdl_.weekday().c_encoding(), 7);
        return __lastd - __back;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI operator sys_days() const   { return sys_days(days(__count())); }
    _CCW_LIBCPP_HIDE_FROM_ABI operator local_days() const { return local_days(days(__count())); }
    _CCW_LIBCPP_HIDE_FROM_ABI bool ok() const { return __y_.ok() && __m_.ok() && __wdl_.ok(); }
};

_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator==(const year_month_weekday_last& __x, const year_month_weekday_last& __y) {
    return __x.year() == __y.year() && __x.month() == __y.month() && __x.weekday_last() == __y.weekday_last();
}
_CCW_LIBCPP_HIDE_FROM_ABI inline bool operator!=(const year_month_weekday_last& __x, const year_month_weekday_last& __y) { return !(__x == __y); }
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_weekday_last operator+(const year_month_weekday_last& __x, const months& __y) { year_month_weekday_last __t(__x); __t += __y; return __t; }
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_weekday_last operator+(const months& __x, const year_month_weekday_last& __y) { return __y + __x; }
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_weekday_last operator-(const year_month_weekday_last& __x, const months& __y) { return __x + months(-__y.count()); }
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_weekday_last operator+(const year_month_weekday_last& __x, const years& __y)  { year_month_weekday_last __t(__x); __t += __y; return __t; }
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_weekday_last operator+(const years& __x, const year_month_weekday_last& __y)  { return __y + __x; }
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_weekday_last operator-(const year_month_weekday_last& __x, const years& __y)  { return __x + years(-__y.count()); }

_CCW_LIBCPP_HIDE_FROM_ABI inline year_month operator/(const year& __y, const month& __m) { return year_month(__y, __m); }
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month operator/(const year& __y, int __m)          { return year_month(__y, month((unsigned)__m)); }

_CCW_LIBCPP_HIDE_FROM_ABI inline month_day operator/(const month& __m, const day& __d) { return month_day(__m, __d); }
_CCW_LIBCPP_HIDE_FROM_ABI inline month_day operator/(const month& __m, int __d)        { return month_day(__m, day((unsigned)__d)); }
_CCW_LIBCPP_HIDE_FROM_ABI inline month_day operator/(int __m, const day& __d)          { return month_day(month((unsigned)__m), __d); }
_CCW_LIBCPP_HIDE_FROM_ABI inline month_day operator/(const day& __d, const month& __m) { return month_day(__m, __d); }
_CCW_LIBCPP_HIDE_FROM_ABI inline month_day operator/(const day& __d, int __m)          { return month_day(month((unsigned)__m), __d); }

_CCW_LIBCPP_HIDE_FROM_ABI inline month_day_last operator/(const month& __m, last_spec) { return month_day_last(__m); }
_CCW_LIBCPP_HIDE_FROM_ABI inline month_day_last operator/(last_spec, const month& __m) { return month_day_last(__m); }
_CCW_LIBCPP_HIDE_FROM_ABI inline month_day_last operator/(last_spec, int __m)          { return month_day_last(month((unsigned)__m)); }

_CCW_LIBCPP_HIDE_FROM_ABI inline month_weekday operator/(const month& __m, const weekday_indexed& __w) { return month_weekday(__m, __w); }
_CCW_LIBCPP_HIDE_FROM_ABI inline month_weekday operator/(const weekday_indexed& __w, const month& __m) { return month_weekday(__m, __w); }
_CCW_LIBCPP_HIDE_FROM_ABI inline month_weekday operator/(const weekday_indexed& __w, int __m)          { return month_weekday(month((unsigned)__m), __w); }

_CCW_LIBCPP_HIDE_FROM_ABI inline month_weekday_last operator/(const month& __m, const weekday_last& __w) { return month_weekday_last(__m, __w); }
_CCW_LIBCPP_HIDE_FROM_ABI inline month_weekday_last operator/(const weekday_last& __w, const month& __m) { return month_weekday_last(__m, __w); }
_CCW_LIBCPP_HIDE_FROM_ABI inline month_weekday_last operator/(const weekday_last& __w, int __m)          { return month_weekday_last(month((unsigned)__m), __w); }

_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_day operator/(const year_month& __ym, const day& __d) {
    return year_month_day(__ym.year(), __ym.month(), __d);
}
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_day operator/(const year_month& __ym, int __d) {
    return year_month_day(__ym.year(), __ym.month(), day((unsigned)__d));
}
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_day operator/(const year& __y, const month_day& __md) {
    return year_month_day(__y, __md.month(), __md.day());
}
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_day operator/(const month_day& __md, const year& __y) {
    return year_month_day(__y, __md.month(), __md.day());
}
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_day operator/(const month_day& __md, int __y) {
    return year_month_day(year(__y), __md.month(), __md.day());
}

_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_day_last operator/(const year_month& __ym, last_spec) {
    return year_month_day_last(__ym.year(), month_day_last(__ym.month()));
}
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_day_last operator/(const year& __y, const month_day_last& __mdl) {
    return year_month_day_last(__y, __mdl);
}
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_day_last operator/(const month_day_last& __mdl, const year& __y) {
    return year_month_day_last(__y, __mdl);
}
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_day_last operator/(const month_day_last& __mdl, int __y) {
    return year_month_day_last(year(__y), __mdl);
}

_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_weekday operator/(const year_month& __ym, const weekday_indexed& __w) {
    return year_month_weekday(__ym.year(), __ym.month(), __w);
}
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_weekday operator/(const year& __y, const month_weekday& __mw) {
    return year_month_weekday(__y, __mw.month(), __mw.weekday_indexed());
}
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_weekday operator/(const month_weekday& __mw, const year& __y) {
    return year_month_weekday(__y, __mw.month(), __mw.weekday_indexed());
}
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_weekday operator/(const month_weekday& __mw, int __y) {
    return year_month_weekday(year(__y), __mw.month(), __mw.weekday_indexed());
}

_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_weekday_last operator/(const year_month& __ym, const weekday_last& __w) {
    return year_month_weekday_last(__ym.year(), __ym.month(), __w);
}
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_weekday_last operator/(const year& __y, const month_weekday_last& __mwl) {
    return year_month_weekday_last(__y, __mwl.month(), __mwl.weekday_last());
}
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_weekday_last operator/(const month_weekday_last& __mwl, const year& __y) {
    return year_month_weekday_last(__y, __mwl.month(), __mwl.weekday_last());
}
_CCW_LIBCPP_HIDE_FROM_ABI inline year_month_weekday_last operator/(const month_weekday_last& __mwl, int __y) {
    return year_month_weekday_last(year(__y), __mwl.month(), __mwl.weekday_last());
}

static const month January(1), February(2), March(3), April(4), May(5), June(6),
                  July(7), August(8), September(9), October(10), November(11), December(12);
static const weekday Sunday(0), Monday(1), Tuesday(2), Wednesday(3), Thursday(4),
                     Friday(5), Saturday(6);

} // namespace chrono

_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_LIBCPP___CHRONO_CALENDAR_H
