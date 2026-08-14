// ccwrap <time.h>
#pragma once

#include <ccwrap_common.h>

#include_next <time.h>

#if defined(__MINGW32__) && !defined(TIME_UTC) && \
    (_CCW_TARGET_C >= 2011 || _CCW_TARGET_CXX >= 2017)
#include <../../detail/c/time_c11.h>
#endif

#if defined(__MINGW32__) && !defined(__cplusplus) && defined(_CCW_C_CONFORM)
#include <string.h>
static _ccw_forceinline char* __ccw_fix_asc(char* __s) {
    if (__s != 0 && strlen(__s) >= 9 && __s[8] == '0') __s[8] = ' ';
    return __s;
}
static _ccw_forceinline char* __ccw_asctime_c(const struct tm* __t) { return __ccw_fix_asc(asctime(__t)); }
static _ccw_forceinline char* __ccw_ctime_c(const time_t* __t)      { return __ccw_fix_asc(ctime(__t)); }
#define asctime(t) __ccw_asctime_c(t)
#define ctime(t)   __ccw_ctime_c(t)
#endif // __MINGW32__ && !__cplusplus && _CCW_C_CONFORM
