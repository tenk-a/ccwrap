/*
 * @file    time_c11.h
 * @brief   ccwrap detail: the C11 additions to <time.h> that Open Watcom lacks.
 * @license Boost Software License Version 1.0
 */
#ifndef CCW_DETAIL_TIME_C11_H_INCLUDED__
#define CCW_DETAIL_TIME_C11_H_INCLUDED__

#ifndef TIME_UTC
#define TIME_UTC 1
#endif

#if !defined(_NO_EXT_KEYS) && !defined(_CCW_TIMESPEC_GET_DEFINED)
#define _CCW_TIMESPEC_GET_DEFINED

#if defined(__cplusplus)
#  define _CCW_T11_FN  inline
#else
#  define _CCW_T11_FN  static _ccw_forceinline
#endif

#if defined(_WIN32) || defined(__NT__)

#include "win/systemtime.h"

_CCW_T11_FN int timespec_get(struct timespec* __ts, int __base) {
    unsigned long __ft[2];
    _ccw_ullong __t;
    if (__base != TIME_UTC || __ts == 0)
        return 0;
    __ccw_win_systemtime_100ns(__ft);
    __t  = ((_ccw_ullong)__ft[1] << 32) | (_ccw_ullong)__ft[0];
    __t -= (_ccw_ullong)116444736000000000ULL;
    __ts->tv_sec  = __t / 10000000ULL;
    __ts->tv_nsec = (long)((__t % 10000000ULL) * 100ULL);
    return __base;
}

#else

_CCW_T11_FN int timespec_get(struct timespec* __ts, int __base) {
    if (__base != TIME_UTC || __ts == 0)
        return 0;
#if defined(__cplusplus)
    __ts->tv_sec  = _CCW_STD::time(0);   /* whole seconds only -- see the subset note */
#else
    __ts->tv_sec  = time(0);
#endif
    __ts->tv_nsec = 0;
    return __base;
}

#endif  /* _WIN32 */

#undef _CCW_T11_FN

#endif  /* !_NO_EXT_KEYS && !_CCW_TIMESPEC_GET_DEFINED */

#endif /* CCW_DETAIL_TIME_C11_H_INCLUDED__ */
