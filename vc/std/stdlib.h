// ccwrap <stdlib.h>
#pragma once

#include <ccwrap_common.h>

#include _CCW_NATIVE_UC_HEADER_PATH(stdlib.h)

#ifdef __cplusplus
extern "C" {
#endif

#if _MSC_VER && _MSC_VER < 1800

#if _MSC_VER < 1600
static __forceinline __int64            llabs(__int64 v) { return _abs64(v); }
#endif
static __forceinline __int64            atoll(char const* p) { return _atoi64(p); }
static __forceinline __int64            strtoll(char const* p, char **e, int r) { return _strtoi64(p,e,r); }
static __forceinline unsigned __int64   strtoull(char const* p, char **e, int r) { return _strtoui64(p,e,r); }
static __forceinline __int64            wtoll(wchar_t const* p) { return _wtoi64(p); }
static __forceinline __int64            wcstoll(wchar_t const* p, wchar_t **e, int r) { return _wcstoi64(p,e,r); }
static __forceinline unsigned __int64   wcstoull(wchar_t const* p, wchar_t **e, int r) { return _wcstoui64(p,e,r); }

static __forceinline float          strtof(char const* p, char **e) { return (float)strtod(p,e); }
static __forceinline float          wcstof(wchar_t const* p, wchar_t **e) { return (float)wcstod(p,e); }
static __forceinline long double    strtold(char const* p, char **e) { return strtod(p,e); }
static __forceinline long double    wcstold(wchar_t const* p, wchar_t **e) { return wcstod(p,e); }

#if _MSC_VER < 1600
typedef struct { __int64 quot; __int64 rem; } lldiv_t;
static __forceinline lldiv_t lldiv(__int64 __n, __int64 __d) {
    lldiv_t __r; __r.quot = __n / __d; __r.rem = __n % __d; return __r;
}
#endif

#endif  // _MSC_VER

#ifdef __cplusplus
}
#endif

#if _CCW_TARGET_C >= 1999 || _CCW_TARGET_CXX >= 2011
#ifndef _CCW_VC_HAVE_UNDERSCORE_EXIT
#define _CCW_VC_HAVE_UNDERSCORE_EXIT 1
#if _MSC_VER < 1900
_CCW_C_DEF(FUNC, void, _Exit, (int __status), (__status)) { _exit(__status); }

#ifndef _CCW_QUICK_EXIT_MAX
#define _CCW_QUICK_EXIT_MAX 32
#endif
static void (*__ccw_qe_fn[_CCW_QUICK_EXIT_MAX])(void);
static int   __ccw_qe_n = 0;

_CCW_C_DEF(FUNC, int, at_quick_exit, (void (*__f)(void)), (__f)) {
    if (__f == 0 || __ccw_qe_n >= _CCW_QUICK_EXIT_MAX) return -1;
    __ccw_qe_fn[__ccw_qe_n++] = __f;
    return 0;
}
_CCW_C_DEF(FUNC, void, quick_exit, (int __status), (__status)) {
    while (__ccw_qe_n > 0) __ccw_qe_fn[--__ccw_qe_n]();
    _exit(__status);
}
#endif
#endif
#endif

#if defined(__cplusplus) && _MSC_VER < 1600
#ifndef _CCW_STDLIB_LL_CXX_DEFINED
#define _CCW_STDLIB_LL_CXX_DEFINED
inline long long abs(long long __v) { return _abs64(__v); }
inline lldiv_t   div(long long __n, long long __d) { return ::lldiv(__n, __d); }
#endif  /* _CCW_STDLIB_LL_CXX_DEFINED */
#endif
