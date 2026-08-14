/**
 *  @file   detail/timespec_vc.h
 *  @brief  C11 struct timespec / TIME_UTC / timespec_get for pre-UCRT MSVC (vc8..vc12).
 *  @license Boost Software License Version 1.0
 */
#if _MSC_VER < 1900 && !defined(_CCW_IN_XTIMEC) && !defined(_CCW_VC_TIMESPEC_DEFINED)
#define _CCW_VC_TIMESPEC_DEFINED

#include <ccwrap_common.h>
#include <time.h>
#include <../../detail/c/win/systemtime.h>

#ifndef TIME_UTC
 #define TIME_UTC           1
#endif

#ifndef _TIMESPEC_DEFINED
#define _TIMESPEC_DEFINED
struct timespec {
    time_t  tv_sec;
    long    tv_nsec;
};
#endif

_CCW_F_DEF(FUNC, int, timespec_get, (struct timespec *ts, int base), (ts, base)) {
    unsigned long ft[2];
    _ccw_ullong t;
    if (base != TIME_UTC || ts == 0)
        return 0;
    __ccw_win_systemtime_100ns(ft);
    t  = ((_ccw_ullong)ft[1] << 32) | (_ccw_ullong)ft[0];
    t -= (_ccw_ullong)116444736000000000ULL;
    ts->tv_sec  = (time_t)(t / 10000000ULL);
    ts->tv_nsec = (long)((t % 10000000ULL) * 100ULL);
    return base;
}
#endif
