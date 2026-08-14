/**
 * @file __chrono/time_io.h
 * @brief operator<< and from_stream for durations and time_points.
 */
#ifndef _CCW_LIBCPP___CHRONO_TIME_IO_H
#define _CCW_LIBCPP___CHRONO_TIME_IO_H

#include "../__config"
#include "calendar_io.h"
#include <string>

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

namespace chrono {

template <class _CharT, class _Traits>
_CCW_LIBCPP_HIDE_FROM_ABI void __ccw_put_micro(basic_ostream<_CharT, _Traits>& __os) {
    if (sizeof(_CharT) == 1) {
        __os.put((_CharT)(unsigned char)0xC2);
        __os.put((_CharT)(unsigned char)0xB5);
    } else {
        __os.put((_CharT)0xB5);
    }
}

template <class _CharT, class _Traits>
_CCW_LIBCPP_HIDE_FROM_ABI void __ccw_put_suffix(basic_ostream<_CharT, _Traits>& __os,
                                                long long __n, long long __d) {
    const char* __s = 0;
    if (__d == 1) {
        if      (__n == 1LL)                  __s = "s";
        else if (__n == 60LL)                 __s = "min";
        else if (__n == 3600LL)               __s = "h";
        else if (__n == 86400LL)              __s = "d";
        else if (__n == 10LL)                 __s = "das";
        else if (__n == 100LL)                __s = "hs";
        else if (__n == 1000LL)               __s = "ks";
        else if (__n == 1000000LL)            __s = "Ms";
        else if (__n == 1000000000LL)         __s = "Gs";
        else if (__n == 1000000000000LL)      __s = "Ts";
        else if (__n == 1000000000000000LL)   __s = "Ps";
        else if (__n == 1000000000000000000LL) __s = "Es";
    } else if (__n == 1) {
        if      (__d == 10LL)                 __s = "ds";
        else if (__d == 100LL)                __s = "cs";
        else if (__d == 1000LL)               __s = "ms";
        else if (__d == 1000000LL)            { chrono::__ccw_put_micro(__os);
                                                chrono::__ccw_put_narrow(__os, "s"); return; }
        else if (__d == 1000000000LL)         __s = "ns";
        else if (__d == 1000000000000LL)      __s = "ps";
        else if (__d == 1000000000000000LL)   __s = "fs";
        else if (__d == 1000000000000000000LL) __s = "as";
    }
    if (__s) { chrono::__ccw_put_narrow(__os, __s); return; }

    char  __b[48];
    char* __p = __b;
    *__p++ = '[';
    if (__n < 0) { *__p++ = '-'; __p = chrono::__ccw_hms_digits(__p, (unsigned long long)(-__n), 1); }
    else         { __p = chrono::__ccw_hms_digits(__p, (unsigned long long)__n, 1); }
    if (__d != 1) { *__p++ = '/'; __p = chrono::__ccw_hms_digits(__p, (unsigned long long)__d, 1); }
    *__p++ = ']';
    *__p++ = 's';
    *__p = '\0';
    chrono::__ccw_put_narrow(__os, __b);
}

template <class _CharT, class _Traits, class _Rep, class _Period>
_CCW_LIBCPP_HIDE_FROM_ABI basic_ostream<_CharT, _Traits>&
operator<<(basic_ostream<_CharT, _Traits>& __os, const duration<_Rep, _Period>& __d) {
    __os << __d.count();
    chrono::__ccw_put_suffix(__os, (long long)_Period::num, (long long)_Period::den);
    return __os;
}

template <class _Dur>
_CCW_LIBCPP_HIDE_FROM_ABI void __ccw_time_text(char* __b, const _Dur& __tod, int __sec_override) {
    hh_mm_ss<_Dur> __hms(__tod);
    char* __p = __b;
    __p = chrono::__ccw_hms_digits(__p, (unsigned long long)__hms.hours().count(), 2);
    *__p++ = ':';
    __p = chrono::__ccw_hms_digits(__p, (unsigned long long)__hms.minutes().count(), 2);
    *__p++ = ':';
    __p = chrono::__ccw_hms_digits(__p, __sec_override >= 0 ? (unsigned long long)__sec_override
                                                            : (unsigned long long)__hms.seconds().count(), 2);
    if (hh_mm_ss<_Dur>::fractional_width > 0) {
        *__p++ = '.';
        __p = chrono::__ccw_hms_digits(__p, (unsigned long long)__hms.subseconds().count(),
                                       (int)hh_mm_ss<_Dur>::fractional_width);
    }
    *__p = '\0';
}

template <class _CharT, class _Traits, class _Dur>
_CCW_LIBCPP_HIDE_FROM_ABI void __ccw_put_datetime(basic_ostream<_CharT, _Traits>& __os,
                                                  const time_point<system_clock, _Dur>& __tp,
                                                  int __sec_override) {
    typedef typename _Dur::period __ccw_p;
    days           __dd = chrono::floor<days>(__tp.time_since_epoch());
    sys_days       __sd(__dd);
    year_month_day __ymd(__sd);
    chrono::operator<<(__os, __ymd);
    if (__ccw_p::num == 86400 && __ccw_p::den == 1) return;
    _Dur __tod = __tp.time_since_epoch() - duration_cast<_Dur>(__dd);
    char __b[40];
    chrono::__ccw_time_text(__b, __tod, __sec_override);
    chrono::__ccw_put_narrow(__os, " ");
    chrono::__ccw_put_narrow(__os, __b);
}

template <class _CharT, class _Traits, class _Dur>
_CCW_LIBCPP_HIDE_FROM_ABI basic_ostream<_CharT, _Traits>&
operator<<(basic_ostream<_CharT, _Traits>& __os, const time_point<system_clock, _Dur>& __tp) {
    chrono::__ccw_put_datetime(__os, __tp, -1);
    return __os;
}

template <class _CharT, class _Traits, class _Dur>
_CCW_LIBCPP_HIDE_FROM_ABI basic_ostream<_CharT, _Traits>&
operator<<(basic_ostream<_CharT, _Traits>& __os, const time_point<utc_clock, _Dur>& __tp) {
    leap_second_info              __i = chrono::get_leap_second_info(__tp);
    time_point<system_clock, _Dur> __s = utc_clock::to_sys(__tp);
    if (__i.is_leap_second) {
        _Dur __one = duration_cast<_Dur>(seconds(1));
        time_point<system_clock, _Dur> __p(__s.time_since_epoch() - __one);
        chrono::__ccw_put_datetime(__os, __p, 60);
    } else {
        chrono::__ccw_put_datetime(__os, __s, -1);
    }
    return __os;
}

template <class _CharT, class _Traits, class _Dur>
_CCW_LIBCPP_HIDE_FROM_ABI basic_ostream<_CharT, _Traits>&
operator<<(basic_ostream<_CharT, _Traits>& __os, const time_point<tai_clock, _Dur>& __tp) {
    _Dur __off = duration_cast<_Dur>(days(4383));
    time_point<system_clock, _Dur> __s(__tp.time_since_epoch() - __off);
    chrono::__ccw_put_datetime(__os, __s, -1);
    return __os;
}

template <class _CharT, class _Traits, class _Dur>
_CCW_LIBCPP_HIDE_FROM_ABI basic_ostream<_CharT, _Traits>&
operator<<(basic_ostream<_CharT, _Traits>& __os, const time_point<gps_clock, _Dur>& __tp) {
    _Dur __off = duration_cast<_Dur>(days(3657));
    time_point<system_clock, _Dur> __s(__tp.time_since_epoch() + __off);
    chrono::__ccw_put_datetime(__os, __s, -1);
    return __os;
}

template <class _CharT, class _Traits, class _Dur>
_CCW_LIBCPP_HIDE_FROM_ABI basic_ostream<_CharT, _Traits>&
operator<<(basic_ostream<_CharT, _Traits>& __os, const time_point<file_clock, _Dur>& __tp) {
    time_point<system_clock, _Dur> __s = file_clock::to_sys(__tp);
    chrono::__ccw_put_datetime(__os, __s, -1);
    return __os;
}

template <class _CharT, class _Traits, class _Dur>
_CCW_LIBCPP_HIDE_FROM_ABI basic_ostream<_CharT, _Traits>&
operator<<(basic_ostream<_CharT, _Traits>& __os, const time_point<local_t, _Dur>& __tp) {
    time_point<system_clock, _Dur> __s(__tp.time_since_epoch());
    chrono::__ccw_put_datetime(__os, __s, -1);
    return __os;
}

template <class _Dur>
_CCW_LIBCPP_HIDE_FROM_ABI _Dur __ccw_frac_ticks(long long __num, long long __den) {
    typedef typename _Dur::period __ccw_p;
    long long __t = __num * (long long)__ccw_p::den / (__den * (long long)__ccw_p::num);
    return _Dur((typename _Dur::rep)__t);
}

template <class _Dur>
_CCW_LIBCPP_HIDE_FROM_ABI bool __ccw_fields_to_sys(const __ccw_cal_fields& __f,
                                                   time_point<system_clock, _Dur>& __out_) {
    if (!__f.__hy_ || !__f.__hm_ || !__f.__hd_) return false;
    year_month_day __ymd(year(__f.__y_), month(__f.__m_), day(__f.__d_));
    if (!__ymd.ok()) return false;
    sys_days __sd(__ymd);

    long long __hh = __f.__hh_;
    if (__f.__hpm_) __hh = (long long)chrono::make24(hours((long long)__hh), __f.__pm_).count();
    long long __sec = __hh * 3600 + __f.__mi_ * 60 + __f.__se_ - (long long)__f.__off_ * 60;

    _Dur __r = duration_cast<_Dur>(__sd.time_since_epoch()) + duration_cast<_Dur>(seconds(__sec));
    if (__f.__subden_ > 1) __r = __r + chrono::__ccw_frac_ticks<_Dur>(__f.__sub_, __f.__subden_);
    __out_ = time_point<system_clock, _Dur>(__r);
    return true;
}

template <class _Rep, class _Period>
_CCW_LIBCPP_HIDE_FROM_ABI bool __ccw_fields_to_dur(const __ccw_cal_fields& __f,
                                                   duration<_Rep, _Period>& __out_) {
    typedef duration<_Rep, _Period> __ccw_d;
    if (__f.__hq_) {
        if (!__f.__hqu_) { __out_ = __ccw_d((_Rep)__f.__q_); return true; }
        long long __t = __f.__q_ * __f.__qnum_ * (long long)_Period::den /
                        (__f.__qden_ * (long long)_Period::num);
        __out_ = __ccw_d((_Rep)__t);
        return true;
    }
    if (!__f.__hhh_ && !__f.__hmi_ && !__f.__hse_ && !__f.__hdoy_) return false;
    long long __sec = __f.__doy_ * 86400 + __f.__hh_ * 3600 + __f.__mi_ * 60 + __f.__se_;
    __ccw_d __r = duration_cast<__ccw_d>(seconds(__sec));
    if (__f.__subden_ > 1) __r = __r + chrono::__ccw_frac_ticks<__ccw_d>(__f.__sub_, __f.__subden_);
    __out_ = __r;
    return true;
}

template <class _CharT, class _Traits, class _Alloc>
_CCW_LIBCPP_HIDE_FROM_ABI void __ccw_store_abbrev(const __ccw_cal_fields& __f,
                                                  basic_string<_CharT, _Traits, _Alloc>* __ab) {
    if (!__ab || !__f.__habb_) return;
    __ab->erase();
    for (const char* __p = __f.__abb_; *__p; ++__p) __ab->push_back((_CharT)*__p);
}

_CCW_LIBCPP_HIDE_FROM_ABI inline void __ccw_store_offset(const __ccw_cal_fields& __f, minutes* __of) {
    if (__of && __f.__hoff_) *__of = minutes((long long)__f.__off_);
}

template <class _CharT, class _Traits, class _Rep, class _Period>
_CCW_LIBCPP_HIDE_FROM_ABI basic_istream<_CharT, _Traits>&
from_stream(basic_istream<_CharT, _Traits>& __is, const _CharT* __fmt, duration<_Rep, _Period>& __d) {
    __ccw_cal_fields        __f;
    duration<_Rep, _Period> __r;
    if (!chrono::__ccw_cal_parse(__is, __fmt, __f) || !chrono::__ccw_fields_to_dur(__f, __r)) {
        __is.setstate(ios_base::failbit);
        return __is;
    }
    __d = __r;
    return __is;
}

template <class _CharT, class _Traits, class _Rep, class _Period, class _Alloc>
_CCW_LIBCPP_HIDE_FROM_ABI basic_istream<_CharT, _Traits>&
from_stream(basic_istream<_CharT, _Traits>& __is, const _CharT* __fmt, duration<_Rep, _Period>& __d,
            basic_string<_CharT, _Traits, _Alloc>* __abbrev) {
    __ccw_cal_fields        __f;
    duration<_Rep, _Period> __r;
    if (!chrono::__ccw_cal_parse(__is, __fmt, __f) || !chrono::__ccw_fields_to_dur(__f, __r)) {
        __is.setstate(ios_base::failbit);
        return __is;
    }
    chrono::__ccw_store_abbrev(__f, __abbrev);
    __d = __r;
    return __is;
}

template <class _CharT, class _Traits, class _Rep, class _Period, class _Alloc>
_CCW_LIBCPP_HIDE_FROM_ABI basic_istream<_CharT, _Traits>&
from_stream(basic_istream<_CharT, _Traits>& __is, const _CharT* __fmt, duration<_Rep, _Period>& __d,
            basic_string<_CharT, _Traits, _Alloc>* __abbrev, minutes* __offset) {
    __ccw_cal_fields        __f;
    duration<_Rep, _Period> __r;
    if (!chrono::__ccw_cal_parse(__is, __fmt, __f) || !chrono::__ccw_fields_to_dur(__f, __r)) {
        __is.setstate(ios_base::failbit);
        return __is;
    }
    chrono::__ccw_store_abbrev(__f, __abbrev);
    chrono::__ccw_store_offset(__f, __offset);
    __d = __r;
    return __is;
}

template <class _CharT, class _Traits, class _Dur>
_CCW_LIBCPP_HIDE_FROM_ABI bool __ccw_parse_sys(basic_istream<_CharT, _Traits>& __is, const _CharT* __fmt,
                                               __ccw_cal_fields& __f, time_point<system_clock, _Dur>& __s) {
    if (!chrono::__ccw_cal_parse(__is, __fmt, __f)) return false;
    return chrono::__ccw_fields_to_sys(__f, __s);
}

#define _CCW_TP_FROM_STREAM(_Clock, _Conv)                                                          \
    template <class _CharT, class _Traits, class _Dur>                                              \
    _CCW_LIBCPP_HIDE_FROM_ABI basic_istream<_CharT, _Traits>&                                       \
    from_stream(basic_istream<_CharT, _Traits>& __is, const _CharT* __fmt,                          \
                time_point<_Clock, _Dur>& __tp) {                                                   \
        __ccw_cal_fields               __f;                                                         \
        time_point<system_clock, _Dur> __s;                                                         \
        if (!chrono::__ccw_parse_sys(__is, __fmt, __f, __s)) { __is.setstate(ios_base::failbit); return __is; } \
        __tp = _Conv;                                                                               \
        return __is;                                                                                \
    }                                                                                               \
    template <class _CharT, class _Traits, class _Dur, class _Alloc>                                \
    _CCW_LIBCPP_HIDE_FROM_ABI basic_istream<_CharT, _Traits>&                                       \
    from_stream(basic_istream<_CharT, _Traits>& __is, const _CharT* __fmt,                          \
                time_point<_Clock, _Dur>& __tp, basic_string<_CharT, _Traits, _Alloc>* __abbrev) {  \
        __ccw_cal_fields               __f;                                                         \
        time_point<system_clock, _Dur> __s;                                                         \
        if (!chrono::__ccw_parse_sys(__is, __fmt, __f, __s)) { __is.setstate(ios_base::failbit); return __is; } \
        chrono::__ccw_store_abbrev(__f, __abbrev);                                                  \
        __tp = _Conv;                                                                               \
        return __is;                                                                                \
    }                                                                                               \
    template <class _CharT, class _Traits, class _Dur, class _Alloc>                                \
    _CCW_LIBCPP_HIDE_FROM_ABI basic_istream<_CharT, _Traits>&                                       \
    from_stream(basic_istream<_CharT, _Traits>& __is, const _CharT* __fmt,                          \
                time_point<_Clock, _Dur>& __tp, basic_string<_CharT, _Traits, _Alloc>* __abbrev,    \
                minutes* __offset) {                                                                \
        __ccw_cal_fields               __f;                                                         \
        time_point<system_clock, _Dur> __s;                                                         \
        if (!chrono::__ccw_parse_sys(__is, __fmt, __f, __s)) { __is.setstate(ios_base::failbit); return __is; } \
        chrono::__ccw_store_abbrev(__f, __abbrev);                                                  \
        chrono::__ccw_store_offset(__f, __offset);                                                  \
        __tp = _Conv;                                                                               \
        return __is;                                                                                \
    }

_CCW_TP_FROM_STREAM(system_clock, (__s))
_CCW_TP_FROM_STREAM(utc_clock,    (utc_clock::from_sys(__s)))
_CCW_TP_FROM_STREAM(file_clock,   (file_clock::from_sys(__s)))
_CCW_TP_FROM_STREAM(tai_clock,    (time_point<tai_clock, _Dur>(__s.time_since_epoch() + duration_cast<_Dur>(days(4383)))))
_CCW_TP_FROM_STREAM(gps_clock,    (time_point<gps_clock, _Dur>(__s.time_since_epoch() - duration_cast<_Dur>(days(3657)))))
_CCW_TP_FROM_STREAM(local_t,      (time_point<local_t, _Dur>(__s.time_since_epoch())))

#undef _CCW_TP_FROM_STREAM

}   // namespace chrono

_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_LIBCPP___CHRONO_TIME_IO_H
