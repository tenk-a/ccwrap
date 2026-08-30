/**
 *  @file   detail/c/win/wide_crt_vc.h
 *  @brief  MSVC-compatible wide CRT names that Open Watcom's win32 CRT lacks.
 *          Sectioned: the includer defines _CCW_WCRT_WANT_INT64 (needs wcstoll,
 *          so after native <wchar.h> or <stdlib.h>) and/or _CCW_WCRT_WANT_WCSTOK
 *          (needs wcstok, so after native <wchar.h> or <string.h>), then includes
 *          this file; it may be included several times, once per section.
 *  @license Boost Software License Version 1.0
 */

#include <ccwrap_common.h>

#if defined(__WATCOMC__) && defined(__NT__)

#ifdef __cplusplus
# define _CCW_WCRT_FN  inline
#else
# define _CCW_WCRT_FN  static _ccw_forceinline
#endif

#if defined(_CCW_WCRT_WANT_INT64) && !defined(_CCW_WCRT_INT64_DEFINED)
#define _CCW_WCRT_INT64_DEFINED

_CCW_WCRT_FN long long _wtoi64(const wchar_t* __s) {
    return wcstoll(__s, (wchar_t**)0, 10);
}
_CCW_WCRT_FN long long _wcstoi64(const wchar_t* __s, wchar_t** __end, int __radix) {
    return wcstoll(__s, __end, __radix);
}
_CCW_WCRT_FN unsigned long long _wcstoui64(const wchar_t* __s, wchar_t** __end, int __radix) {
    return wcstoull(__s, __end, __radix);
}

_CCW_WCRT_FN wchar_t* _ui64tow(unsigned long long __v, wchar_t* __buf, int __radix) {
    wchar_t  __tmp[66];
    int      __i = 0;
    wchar_t* __o = __buf;
    if (__radix < 2 || __radix > 36) { __buf[0] = 0; return __buf; }
    do {
        int __d = (int)(__v % (unsigned)__radix);
        __tmp[__i++] = (wchar_t)(__d < 10 ? (L'0' + __d) : (L'a' + (__d - 10)));
        __v /= (unsigned)__radix;
    } while (__v != 0);
    while (__i > 0) *__o++ = __tmp[--__i];
    *__o = 0;
    return __buf;
}
_CCW_WCRT_FN wchar_t* _i64tow(long long __v, wchar_t* __buf, int __radix) {
    if (__radix == 10 && __v < 0) {
        volatile long long __nv = __v;                 // A34: no const-folded 64bit negate in inline code
        unsigned long long __u = (unsigned long long)__nv;
        __buf[0] = L'-';
        _ui64tow(~__u + 1u, __buf + 1, __radix);
        return __buf;
    }
    return _ui64tow((unsigned long long)__v, __buf, __radix);
}

#endif  /* _CCW_WCRT_WANT_INT64 */

#if defined(_CCW_WCRT_WANT_WCSTOK) && !defined(_CCW_WCRT_WCSTOK_DEFINED)
#define _CCW_WCRT_WCSTOK_DEFINED

_CCW_WCRT_FN wchar_t* _wcstok(wchar_t* __s, const wchar_t* __delim) {
    return wcstok(__s, __delim, (wchar_t**)0);         // NULL context = CRT-internal state
}

#endif  /* _CCW_WCRT_WANT_WCSTOK */

#undef _CCW_WCRT_FN

#endif  /* __WATCOMC__ && __NT__ */

#ifdef _CCW_WCRT_WANT_INT64
#undef _CCW_WCRT_WANT_INT64
#endif
#ifdef _CCW_WCRT_WANT_WCSTOK
#undef _CCW_WCRT_WANT_WCSTOK
#endif
