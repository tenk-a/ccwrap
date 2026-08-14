/**
 * @file __chrono/calendar_io.h
 * @brief operator<< and from_stream for the civil calendar.
 */
#ifndef _CCW_LIBCPP___CHRONO_CALENDAR_IO_H
#define _CCW_LIBCPP___CHRONO_CALENDAR_IO_H

#include "../__config"
#include "hh_mm_ss.h"
#include <istream>
#include <ostream>

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

namespace chrono {

_CCW_LIBCPP_HIDE_FROM_ABI inline const char* __ccw_month_abbr(unsigned __m) {
    static const char* __t[13] = { "???", "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                                   "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
    return __t[__m >= 1u && __m <= 12u ? __m : 0u];
}
_CCW_LIBCPP_HIDE_FROM_ABI inline const char* __ccw_month_full(unsigned __m) {
    static const char* __t[13] = { "???", "January", "February", "March", "April", "May",
                                   "June", "July", "August", "September", "October",
                                   "November", "December" };
    return __t[__m >= 1u && __m <= 12u ? __m : 0u];
}
_CCW_LIBCPP_HIDE_FROM_ABI inline const char* __ccw_weekday_abbr(unsigned __w) {
    static const char* __t[8] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "???" };
    return __t[__w <= 6u ? __w : 7u];
}
_CCW_LIBCPP_HIDE_FROM_ABI inline const char* __ccw_weekday_full(unsigned __w) {
    static const char* __t[8] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday",
                                  "Friday", "Saturday", "???" };
    return __t[__w <= 6u ? __w : 7u];
}

template <class _CharT, class _Traits>
_CCW_LIBCPP_HIDE_FROM_ABI void __ccw_put_narrow(basic_ostream<_CharT, _Traits>& __os, const char* __s) {
    for (; *__s; ++__s) __os.put(__os.widen(*__s));
}

_CCW_LIBCPP_HIDE_FROM_ABI inline void __ccw_pad2(char* __b, unsigned __v) {
    __b[0] = (char)('0' + (__v / 10u) % 10u);
    __b[1] = (char)('0' + __v % 10u);
    __b[2] = '\0';
}
_CCW_LIBCPP_HIDE_FROM_ABI inline void __ccw_year4(char* __b, int __y) {
    unsigned __a = (unsigned)(__y < 0 ? -__y : __y);
    int __i = 0;
    if (__y < 0) __b[__i++] = '-';
    __b[__i++] = (char)('0' + (__a / 1000u) % 10u);
    __b[__i++] = (char)('0' + (__a / 100u) % 10u);
    __b[__i++] = (char)('0' + (__a / 10u) % 10u);
    __b[__i++] = (char)('0' + __a % 10u);
    __b[__i]   = '\0';
}

template <class _CharT, class _Traits>
_CCW_LIBCPP_HIDE_FROM_ABI basic_ostream<_CharT, _Traits>&
operator<<(basic_ostream<_CharT, _Traits>& __os, const day& __d) {
    char __b[4]; __ccw_pad2(__b, (unsigned)__d);
    __ccw_put_narrow(__os, __b);
    if (!__d.ok()) __ccw_put_narrow(__os, " is not a valid day");
    return __os;
}

template <class _CharT, class _Traits>
_CCW_LIBCPP_HIDE_FROM_ABI basic_ostream<_CharT, _Traits>&
operator<<(basic_ostream<_CharT, _Traits>& __os, const month& __m) {
    if (__m.ok()) { __ccw_put_narrow(__os, __ccw_month_abbr((unsigned)__m)); return __os; }
    char __b[4]; __ccw_pad2(__b, (unsigned)__m);
    __ccw_put_narrow(__os, __b);
    __ccw_put_narrow(__os, " is not a valid month");
    return __os;
}

template <class _CharT, class _Traits>
_CCW_LIBCPP_HIDE_FROM_ABI basic_ostream<_CharT, _Traits>&
operator<<(basic_ostream<_CharT, _Traits>& __os, const year& __y) {
    char __b[8]; __ccw_year4(__b, (int)__y);
    __ccw_put_narrow(__os, __b);
    if (!__y.ok()) __ccw_put_narrow(__os, " is not a valid year");
    return __os;
}

template <class _CharT, class _Traits>
_CCW_LIBCPP_HIDE_FROM_ABI basic_ostream<_CharT, _Traits>&
operator<<(basic_ostream<_CharT, _Traits>& __os, const weekday& __w) {
    if (__w.ok()) { __ccw_put_narrow(__os, __ccw_weekday_abbr(__w.c_encoding())); return __os; }
    char __b[4]; __ccw_pad2(__b, __w.c_encoding());
    __ccw_put_narrow(__os, __b);
    __ccw_put_narrow(__os, " is not a valid weekday");
    return __os;
}

template <class _CharT, class _Traits>
_CCW_LIBCPP_HIDE_FROM_ABI basic_ostream<_CharT, _Traits>&
operator<<(basic_ostream<_CharT, _Traits>& __os, const weekday_indexed& __wi) {
    chrono::operator<<(__os, __wi.weekday());
    __ccw_put_narrow(__os, "[");
    char __b[4]; __b[0] = (char)('0' + __wi.index() % 10u); __b[1] = '\0';
    __ccw_put_narrow(__os, __b);
    if (!(__wi.index() >= 1u && __wi.index() <= 5u)) __ccw_put_narrow(__os, " is not a valid index");
    __ccw_put_narrow(__os, "]");
    return __os;
}

template <class _CharT, class _Traits>
_CCW_LIBCPP_HIDE_FROM_ABI basic_ostream<_CharT, _Traits>&
operator<<(basic_ostream<_CharT, _Traits>& __os, const weekday_last& __wl) {
    chrono::operator<<(__os, __wl.weekday());
    __ccw_put_narrow(__os, "[last]");
    return __os;
}

template <class _CharT, class _Traits>
_CCW_LIBCPP_HIDE_FROM_ABI basic_ostream<_CharT, _Traits>&
operator<<(basic_ostream<_CharT, _Traits>& __os, const month_day& __md) {
    chrono::operator<<(__os, __md.month());
    __ccw_put_narrow(__os, "/");
    chrono::operator<<(__os, __md.day());
    return __os;
}

template <class _CharT, class _Traits>
_CCW_LIBCPP_HIDE_FROM_ABI basic_ostream<_CharT, _Traits>&
operator<<(basic_ostream<_CharT, _Traits>& __os, const month_day_last& __mdl) {
    chrono::operator<<(__os, __mdl.month());
    __ccw_put_narrow(__os, "/last");
    return __os;
}

template <class _CharT, class _Traits>
_CCW_LIBCPP_HIDE_FROM_ABI basic_ostream<_CharT, _Traits>&
operator<<(basic_ostream<_CharT, _Traits>& __os, const month_weekday& __mw) {
    chrono::operator<<(__os, __mw.month());
    __ccw_put_narrow(__os, "/");
    chrono::operator<<(__os, __mw.weekday_indexed());
    return __os;
}

template <class _CharT, class _Traits>
_CCW_LIBCPP_HIDE_FROM_ABI basic_ostream<_CharT, _Traits>&
operator<<(basic_ostream<_CharT, _Traits>& __os, const month_weekday_last& __mwl) {
    chrono::operator<<(__os, __mwl.month());
    __ccw_put_narrow(__os, "/");
    chrono::operator<<(__os, __mwl.weekday_last());
    return __os;
}

template <class _CharT, class _Traits>
_CCW_LIBCPP_HIDE_FROM_ABI basic_ostream<_CharT, _Traits>&
operator<<(basic_ostream<_CharT, _Traits>& __os, const year_month& __ym) {
    chrono::operator<<(__os, __ym.year());
    __ccw_put_narrow(__os, "/");
    chrono::operator<<(__os, __ym.month());
    return __os;
}

template <class _CharT, class _Traits>
_CCW_LIBCPP_HIDE_FROM_ABI basic_ostream<_CharT, _Traits>&
operator<<(basic_ostream<_CharT, _Traits>& __os, const year_month_day& __ymd) {
    char __b[8];
    __ccw_year4(__b, (int)__ymd.year());  __ccw_put_narrow(__os, __b);
    __ccw_put_narrow(__os, "-");
    __ccw_pad2(__b, (unsigned)__ymd.month()); __ccw_put_narrow(__os, __b);
    __ccw_put_narrow(__os, "-");
    __ccw_pad2(__b, (unsigned)__ymd.day());   __ccw_put_narrow(__os, __b);
    if (!__ymd.ok()) __ccw_put_narrow(__os, " is not a valid date");
    return __os;
}

template <class _CharT, class _Traits>
_CCW_LIBCPP_HIDE_FROM_ABI basic_ostream<_CharT, _Traits>&
operator<<(basic_ostream<_CharT, _Traits>& __os, const year_month_day_last& __ymdl) {
    chrono::operator<<(__os, __ymdl.year());
    __ccw_put_narrow(__os, "/");
    chrono::operator<<(__os, __ymdl.month_day_last());
    return __os;
}

template <class _CharT, class _Traits>
_CCW_LIBCPP_HIDE_FROM_ABI basic_ostream<_CharT, _Traits>&
operator<<(basic_ostream<_CharT, _Traits>& __os, const year_month_weekday& __ymw) {
    chrono::operator<<(__os, __ymw.year());
    __ccw_put_narrow(__os, "/");
    chrono::operator<<(__os, __ymw.month());
    __ccw_put_narrow(__os, "/");
    chrono::operator<<(__os, __ymw.weekday_indexed());
    return __os;
}

template <class _CharT, class _Traits>
_CCW_LIBCPP_HIDE_FROM_ABI basic_ostream<_CharT, _Traits>&
operator<<(basic_ostream<_CharT, _Traits>& __os, const year_month_weekday_last& __ymwl) {
    chrono::operator<<(__os, __ymwl.year());
    __ccw_put_narrow(__os, "/");
    chrono::operator<<(__os, __ymwl.month());
    __ccw_put_narrow(__os, "/");
    chrono::operator<<(__os, __ymwl.weekday_last());
    return __os;
}

struct __ccw_cal_fields {
    int      __y_;
    unsigned __m_;
    unsigned __d_;
    unsigned __wd_;
    bool     __hy_, __hm_, __hd_, __hwd_;

    long long __hh_;              // %H / %I  hours
    long long __mi_;              // %M    minutes
    long long __se_;              // %S    whole seconds
    long long __sub_, __subden_;  // the fraction after %S, as __sub_/__subden_
    long long __doy_;             // %j    day of the year (whole days for a duration)
    long long __q_;               // %Q    a duration's numeric value
    long long __qnum_, __qden_;   // %q    the unit %Q is expressed in
    long      __off_;             // %z    minutes east of UTC
    char      __abb_[24];         // %Z    time-zone abbreviation
    bool      __hhh_, __hmi_, __hse_, __hdoy_, __hq_, __hqu_, __hoff_, __habb_;
    bool      __pm_, __hpm_;      // %p

    _CCW_LIBCPP_HIDE_FROM_ABI __ccw_cal_fields()
        : __y_(0), __m_(0), __d_(0), __wd_(0), __hy_(false), __hm_(false), __hd_(false), __hwd_(false),
          __hh_(0), __mi_(0), __se_(0), __sub_(0), __subden_(1), __doy_(0), __q_(0),
          __qnum_(1), __qden_(1), __off_(0),
          __hhh_(false), __hmi_(false), __hse_(false), __hdoy_(false), __hq_(false),
          __hqu_(false), __hoff_(false), __habb_(false), __pm_(false), __hpm_(false) { __abb_[0] = '\0'; }
};

_CCW_LIBCPP_HIDE_FROM_ABI inline bool __ccw_ci_eq(char __a, char __b) {
    if (__a >= 'A' && __a <= 'Z') __a = (char)(__a - 'A' + 'a');
    if (__b >= 'A' && __b <= 'Z') __b = (char)(__b - 'A' + 'a');
    return __a == __b;
}

template <class _CharT, class _Traits>
_CCW_LIBCPP_HIDE_FROM_ABI bool __ccw_read_int(basic_istream<_CharT, _Traits>& __is, int __maxd, long long& __out_) {
    bool __neg = false;
    typename _Traits::int_type __c = __is.peek();
    if (!_Traits::eq_int_type(__c, _Traits::eof())) {
        char __n = __is.narrow(_Traits::to_char_type(__c), '\0');
        if (__n == '-' || __n == '+') { __neg = (__n == '-'); __is.get(); }
    }
    long long __v = 0;
    int __n = 0;
    for (; __n < __maxd; ++__n) {
        __c = __is.peek();
        if (_Traits::eq_int_type(__c, _Traits::eof())) break;
        char __ch = __is.narrow(_Traits::to_char_type(__c), '\0');
        if (__ch < '0' || __ch > '9') break;
        __v = __v * 10 + (__ch - '0');
        __is.get();
    }
    if (__n == 0) return false;
    __out_ = __neg ? -__v : __v;
    return true;
}

template <class _CharT, class _Traits>
_CCW_LIBCPP_HIDE_FROM_ABI bool __ccw_read_name(basic_istream<_CharT, _Traits>& __is,
                                               const char* const* __full, const char* const* __abbr,
                                               unsigned __lo, unsigned __hi, unsigned& __out_) {
    char __buf[16];
    int __n = 0;
    for (; __n < 15; ++__n) {
        typename _Traits::int_type __c = __is.peek();
        if (_Traits::eq_int_type(__c, _Traits::eof())) break;
        char __ch = __is.narrow(_Traits::to_char_type(__c), '\0');
        bool __alpha = (__ch >= 'a' && __ch <= 'z') || (__ch >= 'A' && __ch <= 'Z');
        if (!__alpha) break;
        __buf[__n] = __ch;
        __is.get();
    }
    __buf[__n] = '\0';
    if (__n == 0) return false;
    for (unsigned __i = __lo; __i <= __hi; ++__i) {
        const char* __cand[2];
        __cand[0] = __full[__i];
        __cand[1] = __abbr[__i];
        for (int __k = 0; __k < 2; ++__k) {
            const char* __s = __cand[__k];
            int __j = 0;
            for (; __s[__j] && __j < __n && __ccw_ci_eq(__s[__j], __buf[__j]); ++__j) {}
            if (__s[__j] == '\0' && __j == __n) { __out_ = __i; return true; }
        }
    }
    return false;
}

_CCW_LIBCPP_HIDE_FROM_ABI inline bool __ccw_unit_ratio(const char* __s, long long& __num_, long long& __den_) {
    struct __ent { const char* __n_; long long __num_, __den_; };
    static const __ent __t[] = {
        { "as", 1, 1000000000000000000LL }, { "fs", 1, 1000000000000000LL },
        { "ps", 1, 1000000000000LL },       { "ns", 1, 1000000000LL },
        { "us", 1, 1000000LL },             { "ms", 1, 1000LL },
        { "cs", 1, 100LL },                 { "ds", 1, 10LL },
        { "s",  1, 1 },                     { "das", 10, 1 },
        { "hs", 100, 1 },                   { "ks", 1000LL, 1 },
        { "Ms", 1000000LL, 1 },             { "Gs", 1000000000LL, 1 },
        { "Ts", 1000000000000LL, 1 },       { "Ps", 1000000000000000LL, 1 },
        { "Es", 1000000000000000000LL, 1 }, { "min", 60, 1 },
        { "h",  3600, 1 },                  { "d", 86400, 1 }
    };
    for (unsigned __i = 0; __i < sizeof(__t) / sizeof(__t[0]); ++__i) {
        const char* __a = __t[__i].__n_;
        int __j = 0;
        for (; __a[__j] && __a[__j] == __s[__j]; ++__j) {}
        if (__a[__j] == '\0' && __s[__j] == '\0') { __num_ = __t[__i].__num_; __den_ = __t[__i].__den_; return true; }
    }
    return false;
}

template <class _CharT, class _Traits>
_CCW_LIBCPP_HIDE_FROM_ABI bool __ccw_read_unit(basic_istream<_CharT, _Traits>& __is, long long& __num_, long long& __den_) {
    char __buf[8];
    int  __n = 0;
    while (__n < 7) {
        typename _Traits::int_type __c = __is.peek();
        if (_Traits::eq_int_type(__c, _Traits::eof())) break;
        _CharT __wc = _Traits::to_char_type(__c);
        if (__wc == (_CharT)0xB5) { __buf[__n++] = 'u'; __is.get(); continue; }
        if (__wc == (_CharT)(unsigned char)0xC2) {           // UTF-8 lead byte of U+00B5
            __is.get();
            __c = __is.peek();
            if (_Traits::eq_int_type(__c, _Traits::eof())) return false;
            if (_Traits::to_char_type(__c) != (_CharT)(unsigned char)0xB5) return false;
            __is.get();
            __buf[__n++] = 'u';
            continue;
        }
        char __ch = __is.narrow(__wc, '\0');
        bool __alpha = (__ch >= 'a' && __ch <= 'z') || (__ch >= 'A' && __ch <= 'Z');
        if (!__alpha) break;
        __buf[__n++] = __ch;
        __is.get();
    }
    __buf[__n] = '\0';
    if (__n == 0) return false;
    return chrono::__ccw_unit_ratio(__buf, __num_, __den_);
}

template <class _CharT, class _Traits>
_CCW_LIBCPP_HIDE_FROM_ABI bool __ccw_expect(basic_istream<_CharT, _Traits>& __is, char __want) {
    typename _Traits::int_type __c = __is.get();
    if (_Traits::eq_int_type(__c, _Traits::eof())) return false;
    return __is.narrow(_Traits::to_char_type(__c), '\0') == __want;
}

template <class _CharT, class _Traits>
_CCW_LIBCPP_HIDE_FROM_ABI void __ccw_read_frac(basic_istream<_CharT, _Traits>& __is, __ccw_cal_fields& __f) {
    typename _Traits::int_type __c = __is.peek();
    if (_Traits::eq_int_type(__c, _Traits::eof())) return;
    char __ch = __is.narrow(_Traits::to_char_type(__c), '\0');
    if (__ch != '.' && __ch != ',') return;
    __is.get();
    long long __v = 0, __d = 1;
    for (int __n = 0; __n < 9; ++__n) {
        __c = __is.peek();
        if (_Traits::eq_int_type(__c, _Traits::eof())) break;
        __ch = __is.narrow(_Traits::to_char_type(__c), '\0');
        if (__ch < '0' || __ch > '9') break;
        __v = __v * 10 + (__ch - '0');
        __d *= 10;
        __is.get();
    }
    __f.__sub_ = __v;
    __f.__subden_ = __d;
}

template <class _CharT, class _Traits>
_CCW_LIBCPP_HIDE_FROM_ABI bool __ccw_cal_parse(basic_istream<_CharT, _Traits>& __is,
                                               const _CharT* __fmt, __ccw_cal_fields& __f) {
    static const char* __mfull[13] = { "???", "January", "February", "March", "April", "May",
                                       "June", "July", "August", "September", "October",
                                       "November", "December" };
    static const char* __mabbr[13] = { "???", "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                                       "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
    static const char* __wfull[7]  = { "Sunday", "Monday", "Tuesday", "Wednesday",
                                       "Thursday", "Friday", "Saturday" };
    static const char* __wabbr[7]  = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

    for (; *__fmt; ++__fmt) {
        char __fc = __is.narrow(*__fmt, '\0');
        if (__fc != '%') {
            if (__fc == ' ' || __fc == '\t' || __fc == '\n') {
                while (true) {
                    typename _Traits::int_type __c = __is.peek();
                    if (_Traits::eq_int_type(__c, _Traits::eof())) break;
                    char __ch = __is.narrow(_Traits::to_char_type(__c), '\0');
                    if (__ch != ' ' && __ch != '\t' && __ch != '\n') break;
                    __is.get();
                }
                continue;
            }
            typename _Traits::int_type __c = __is.get();
            if (_Traits::eq_int_type(__c, _Traits::eof())) return false;
            if (__is.narrow(_Traits::to_char_type(__c), '\0') != __fc) return false;
            continue;
        }
        ++__fmt;
        if (!*__fmt) return false;
        char __sp = __is.narrow(*__fmt, '\0');
        long long __v = 0;
        unsigned __u = 0;
        switch (__sp) {
        case '%': {
            typename _Traits::int_type __c = __is.get();
            if (_Traits::eq_int_type(__c, _Traits::eof())) return false;
            if (__is.narrow(_Traits::to_char_type(__c), '\0') != '%') return false;
            break;
        }
        case 'n': case 't':
            while (true) {
                typename _Traits::int_type __c = __is.peek();
                if (_Traits::eq_int_type(__c, _Traits::eof())) break;
                char __ch = __is.narrow(_Traits::to_char_type(__c), '\0');
                if (__ch != ' ' && __ch != '\t' && __ch != '\n') break;
                __is.get();
            }
            break;
        case 'd': case 'e':
            if (!__ccw_read_int(__is, 2, __v)) return false;
            __f.__d_ = (unsigned)__v; __f.__hd_ = true;
            break;
        case 'm':
            if (!__ccw_read_int(__is, 2, __v)) return false;
            __f.__m_ = (unsigned)__v; __f.__hm_ = true;
            break;
        case 'Y':
            if (!__ccw_read_int(__is, 6, __v)) return false;
            __f.__y_ = (int)__v; __f.__hy_ = true;
            break;
        case 'y':
            if (!__ccw_read_int(__is, 2, __v)) return false;
            __f.__y_ = (int)(__v >= 69 ? 1900 + __v : 2000 + __v); __f.__hy_ = true;
            break;
        case 'b': case 'B': case 'h':
            if (!__ccw_read_name(__is, __mfull, __mabbr, 1u, 12u, __u)) return false;
            __f.__m_ = __u; __f.__hm_ = true;
            break;
        case 'a': case 'A':
            if (!__ccw_read_name(__is, __wfull, __wabbr, 0u, 6u, __u)) return false;
            __f.__wd_ = __u; __f.__hwd_ = true;
            break;
        case 'F': {
            if (!__ccw_read_int(__is, 6, __v)) return false;
            __f.__y_ = (int)__v; __f.__hy_ = true;
            if (!__ccw_expect(__is, '-')) return false;
            if (!__ccw_read_int(__is, 2, __v)) return false;
            __f.__m_ = (unsigned)__v; __f.__hm_ = true;
            if (!__ccw_expect(__is, '-')) return false;
            if (!__ccw_read_int(__is, 2, __v)) return false;
            __f.__d_ = (unsigned)__v; __f.__hd_ = true;
            break;
        }
        case 'D': {
            if (!__ccw_read_int(__is, 2, __v)) return false;
            __f.__m_ = (unsigned)__v; __f.__hm_ = true;
            if (!__ccw_expect(__is, '/')) return false;
            if (!__ccw_read_int(__is, 2, __v)) return false;
            __f.__d_ = (unsigned)__v; __f.__hd_ = true;
            if (!__ccw_expect(__is, '/')) return false;
            if (!__ccw_read_int(__is, 2, __v)) return false;
            __f.__y_ = (int)(__v >= 69 ? 1900 + __v : 2000 + __v); __f.__hy_ = true;
            break;
        }
        case 'H': case 'I':
            if (!__ccw_read_int(__is, 2, __v)) return false;
            __f.__hh_ = __v; __f.__hhh_ = true;
            break;
        case 'M':
            if (!__ccw_read_int(__is, 2, __v)) return false;
            __f.__mi_ = __v; __f.__hmi_ = true;
            break;
        case 'S':
            if (!__ccw_read_int(__is, 2, __v)) return false;
            __f.__se_ = __v; __f.__hse_ = true;
            chrono::__ccw_read_frac(__is, __f);
            break;
        case 'T': case 'X':
            if (!__ccw_read_int(__is, 2, __v)) return false;
            __f.__hh_ = __v; __f.__hhh_ = true;
            if (!__ccw_expect(__is, ':')) return false;
            if (!__ccw_read_int(__is, 2, __v)) return false;
            __f.__mi_ = __v; __f.__hmi_ = true;
            if (!__ccw_expect(__is, ':')) return false;
            if (!__ccw_read_int(__is, 2, __v)) return false;
            __f.__se_ = __v; __f.__hse_ = true;
            chrono::__ccw_read_frac(__is, __f);
            break;
        case 'R':
            if (!__ccw_read_int(__is, 2, __v)) return false;
            __f.__hh_ = __v; __f.__hhh_ = true;
            if (!__ccw_expect(__is, ':')) return false;
            if (!__ccw_read_int(__is, 2, __v)) return false;
            __f.__mi_ = __v; __f.__hmi_ = true;
            break;
        case 'j':
            if (!__ccw_read_int(__is, 3, __v)) return false;
            __f.__doy_ = __v; __f.__hdoy_ = true;
            break;
        case 'p': {
            static const char* const __pfull[2] = { "AM", "PM" };
            if (!__ccw_read_name(__is, __pfull, __pfull, 0u, 1u, __u)) return false;
            __f.__pm_ = (__u == 1u); __f.__hpm_ = true;
            break;
        }
        case 'Q':
            if (!__ccw_read_int(__is, 18, __v)) return false;
            __f.__q_ = __v; __f.__hq_ = true;
            break;
        case 'q':
            if (!chrono::__ccw_read_unit(__is, __f.__qnum_, __f.__qden_)) return false;
            __f.__hqu_ = true;
            break;
        case 'z': {
            typename _Traits::int_type __c = __is.peek();
            if (_Traits::eq_int_type(__c, _Traits::eof())) return false;
            char __sg = __is.narrow(_Traits::to_char_type(__c), '\0');
            if (__sg != '+' && __sg != '-') return false;
            __is.get();
            if (!__ccw_read_int(__is, 2, __v)) return false;
            long long __oh = __v;
            __c = __is.peek();
            if (!_Traits::eq_int_type(__c, _Traits::eof()) &&
                __is.narrow(_Traits::to_char_type(__c), '\0') == ':') __is.get();
            long long __om = 0;
            if (!__ccw_read_int(__is, 2, __om)) __om = 0;   // "+09" is a whole hour
            long __t = (long)(__oh * 60 + __om);
            __f.__off_ = (__sg == '-') ? -__t : __t;
            __f.__hoff_ = true;
            break;
        }
        case 'Z': {
            int __n = 0;
            while (__n < 23) {
                typename _Traits::int_type __c = __is.peek();
                if (_Traits::eq_int_type(__c, _Traits::eof())) break;
                char __ch = __is.narrow(_Traits::to_char_type(__c), '\0');
                bool __ok = (__ch >= 'a' && __ch <= 'z') || (__ch >= 'A' && __ch <= 'Z') ||
                            (__ch >= '0' && __ch <= '9') ||
                            __ch == '_' || __ch == '/' || __ch == '+' || __ch == '-';
                if (!__ok) break;
                __f.__abb_[__n++] = __ch;
                __is.get();
            }
            __f.__abb_[__n] = '\0';
            if (__n == 0) return false;
            __f.__habb_ = true;
            break;
        }
        default:
            return false;
        }
    }
    return true;
}

#define _CCW_CAL_FROM_STREAM(_Type, _Need, _Assign)                                            \
    template <class _CharT, class _Traits>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI basic_istream<_CharT, _Traits>&                                  \
    from_stream(basic_istream<_CharT, _Traits>& __is, const _CharT* __fmt, _Type& __tp) {      \
        __ccw_cal_fields __f;                                                                  \
        if (!__ccw_cal_parse(__is, __fmt, __f) || !(_Need)) { __is.setstate(ios_base::failbit); return __is; } \
        _Assign;                                                                               \
        return __is;                                                                           \
    }

_CCW_CAL_FROM_STREAM(day,     __f.__hd_,  __tp = day(__f.__d_))
_CCW_CAL_FROM_STREAM(month,   __f.__hm_,  __tp = month(__f.__m_))
_CCW_CAL_FROM_STREAM(year,    __f.__hy_,  __tp = year(__f.__y_))
_CCW_CAL_FROM_STREAM(weekday, __f.__hwd_, __tp = weekday(__f.__wd_))
_CCW_CAL_FROM_STREAM(month_day,  __f.__hm_ && __f.__hd_, __tp = month_day(month(__f.__m_), day(__f.__d_)))
_CCW_CAL_FROM_STREAM(year_month, __f.__hy_ && __f.__hm_, __tp = year_month(year(__f.__y_), month(__f.__m_)))
_CCW_CAL_FROM_STREAM(year_month_day, __f.__hy_ && __f.__hm_ && __f.__hd_,
                     __tp = year_month_day(year(__f.__y_), month(__f.__m_), day(__f.__d_)))

#undef _CCW_CAL_FROM_STREAM

_CCW_LIBCPP_HIDE_FROM_ABI inline char* __ccw_hms_digits(char* __p, unsigned long long __v, int __width) {
    char __tmp[24];
    int  __n = 0;
    do { __tmp[__n++] = (char)('0' + (int)(__v % 10u)); __v /= 10u; } while (__v);
    for (; __n < __width; ++__n) __tmp[__n] = '0';
    while (__n) *__p++ = __tmp[--__n];
    return __p;
}

template <class _CharT, class _Traits, class _Duration>
_CCW_LIBCPP_HIDE_FROM_ABI basic_ostream<_CharT, _Traits>&
operator<<(basic_ostream<_CharT, _Traits>& __os, const hh_mm_ss<_Duration>& __t) {
    char  __b[64];
    char* __p = __b;
    if (__t.is_negative()) *__p++ = '-';
    __p = __ccw_hms_digits(__p, (unsigned long long)__t.hours().count(), 2);
    *__p++ = ':';
    __p = __ccw_hms_digits(__p, (unsigned long long)__t.minutes().count(), 2);
    *__p++ = ':';
    __p = __ccw_hms_digits(__p, (unsigned long long)__t.seconds().count(), 2);
    if (hh_mm_ss<_Duration>::fractional_width > 0) {
        *__p++ = '.';
        __p = __ccw_hms_digits(__p, (unsigned long long)__t.subseconds().count(),
                               (int)hh_mm_ss<_Duration>::fractional_width);
    }
    *__p = '\0';
    chrono::__ccw_put_narrow(__os, __b);
    return __os;
}

} // namespace chrono

_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_LIBCPP___CHRONO_CALENDAR_IO_H
