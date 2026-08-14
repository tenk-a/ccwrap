//===----------------------------------------------------------------------===//
// ccwrap: libc++ layout, classic C++03 body for Open Watcom.
// [time.clock.utc] leap_second_info and the table behind it.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___CHRONO_LEAP_SECOND_H
#define _CCW_LIBCPP___CHRONO_LEAP_SECOND_H
#include "../__config"

_CCW_LIBCPP_BEGIN_NAMESPACE_STD
namespace chrono {

struct __ccw_leap_date { int __y_; unsigned __m_; unsigned __d_; };

_CCW_LIBCPP_HIDE_FROM_ABI inline const __ccw_leap_date* __ccw_leap_dates(int& __count) {
    static const __ccw_leap_date __t[] = {
        { 1972,  7,  1 }, { 1973,  1,  1 }, { 1974,  1,  1 }, { 1975,  1,  1 },
        { 1976,  1,  1 }, { 1977,  1,  1 }, { 1978,  1,  1 }, { 1979,  1,  1 },
        { 1980,  1,  1 }, { 1981,  7,  1 }, { 1982,  7,  1 }, { 1983,  7,  1 },
        { 1985,  7,  1 }, { 1988,  1,  1 }, { 1990,  1,  1 }, { 1991,  1,  1 },
        { 1992,  7,  1 }, { 1993,  7,  1 }, { 1994,  7,  1 }, { 1996,  1,  1 },
        { 1997,  7,  1 }, { 1999,  1,  1 }, { 2006,  1,  1 }, { 2009,  1,  1 },
        { 2012,  7,  1 }, { 2015,  7,  1 }, { 2017,  1,  1 }
    };
    __count = (int)(sizeof(__t) / sizeof(__t[0]));
    return __t;
}

_CCW_LIBCPP_HIDE_FROM_ABI inline long long __ccw_leap_sys(int __i) {
    int __n = 0;
    const __ccw_leap_date* __t = chrono::__ccw_leap_dates(__n);
    year_month_day __ymd(year(__t[__i].__y_), month(__t[__i].__m_), day(__t[__i].__d_));
    sys_days __d(__ymd);
    return (long long)__d.time_since_epoch().count() * 86400;
}

_CCW_LIBCPP_HIDE_FROM_ABI inline long long __ccw_leaps_at_sys(long long __sys_secs) {
    int __n = 0;
    chrono::__ccw_leap_dates(__n);
    long long __c = 0;
    for (int __i = 0; __i < __n; ++__i)
        if (chrono::__ccw_leap_sys(__i) <= __sys_secs) ++__c;
    return __c;
}

_CCW_LIBCPP_HIDE_FROM_ABI inline void __ccw_leaps_at_utc(long long __utc_secs, long long& __elapsed, bool& __is_leap) {
    int __n = 0;
    chrono::__ccw_leap_dates(__n);
    __elapsed = 0;
    __is_leap = false;
    for (int __i = 0; __i < __n; ++__i) {
        long long __start = chrono::__ccw_leap_sys(__i) + (long long)__i;   // utc of the leap second
        if (__utc_secs >= __start + 1)   ++__elapsed;
        else if (__utc_secs >= __start)  __is_leap = true;
    }
    if (__is_leap) ++__elapsed;
}

struct leap_second_info {
    bool            is_leap_second;
    chrono::seconds elapsed;
    _CCW_LIBCPP_HIDE_FROM_ABI leap_second_info() : is_leap_second(false), elapsed(seconds(0)) {}
    _CCW_LIBCPP_HIDE_FROM_ABI leap_second_info(bool __l, const chrono::seconds& __e)
        : is_leap_second(__l), elapsed(__e) {}
};

}   // namespace chrono
_CCW_LIBCPP_END_NAMESPACE_STD
#endif // _CCW_LIBCPP___CHRONO_LEAP_SECOND_H
