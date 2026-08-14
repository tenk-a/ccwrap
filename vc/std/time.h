// ccwrap <time.h>
#pragma once

#include <ccwrap_common.h>

#include _CCW_NATIVE_UC_HEADER_PATH(time.h)

#include <../../detail/c/timespec_vc.h>

#if _MSC_VER < 1900 && !defined(__cplusplus) && defined(_CCW_C_CONFORM)
#include <string.h>
static __forceinline char* __ccw_fix_asc(char* __s) {
    if (__s != 0 && strlen(__s) >= 9 && __s[8] == '0') __s[8] = ' ';
    return __s;
}
static __forceinline char* __ccw_asctime_c(const struct tm* __t) { return __ccw_fix_asc(asctime(__t)); }
static __forceinline char* __ccw_ctime_c(const time_t* __t)      { return __ccw_fix_asc(ctime(__t)); }
#define asctime(t) __ccw_asctime_c(t)
#define ctime(t)   __ccw_ctime_c(t)
#endif
