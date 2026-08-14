// ccwrap <wchar.h>
#pragma once

#include <ccwrap_common.h>

#if _MSC_VER < 1600
 #ifdef WCHAR_MIN
  #undef WCHAR_MIN
 #endif
 #ifdef WCHAR_MAX
  #undef WCHAR_MAX
 #endif
#endif

#include _CCW_NATIVE_UC_HEADER_PATH(wchar.h)

#if (_CCW_TARGET_C >= 1999 || _CCW_TARGET_CXX >= 2011) && _MSC_VER < 1900
#ifndef _CCW_VC_HAVE_WCSTOK3
#define _CCW_VC_HAVE_WCSTOK3 1
_CCW_C_DEF(FUNC, wchar_t*, __ccw_wcstok3,
           (wchar_t* __s, const wchar_t* __d, wchar_t** __ctx), (__s, __d, __ctx)) {
    return wcstok_s(__s, __d, __ctx);
}
#define wcstok(s, d, c) __ccw_wcstok3((s), (d), (c))
#endif
#endif

#if defined(_MSC_VER) && _MSC_VER < 1800
#include <wctype.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

static __inline int __ccw_wspec_len(const wchar_t* __start, const wchar_t** __endp) {
    const wchar_t* __f = __start + 1;                  /* past '%' */
    static const wchar_t __conv[] = L"diouxXeEfgGaAcsp[n";
    while (*__f && !wcschr(__conv, *__f)) ++__f;
    if (*__f == 0) return -1;
    *__endp = __f + 1;
    if (*__f == L'[') {                                /* scan set: [ ... ] */
        const wchar_t* __p = __f + 1;
        if (*__p == L'^') ++__p;
        if (*__p == L']') ++__p;
        while (*__p && *__p != L']') ++__p;
        if (*__p == L']') ++__p;
        *__endp = __p;
    }
    return (int)(*__endp - __start);
}

static __inline int _ccw_vswscanf(const wchar_t* __buf, const wchar_t* __fmt, va_list __ap) {
    const wchar_t* __b = __buf;
    const wchar_t* __f = __fmt;
    wchar_t        __spec[64];
    int            __total = 0;
    while (*__f) {
        if (iswspace(*__f)) {
            while (iswspace(*__b)) ++__b;
            ++__f;
            continue;
        }
        if (*__f != L'%') {
            if (*__b != *__f) break;
            ++__b; ++__f;
            continue;
        }
        {
            const wchar_t* __start = __f;
            const wchar_t* __end;
            int __suppress = (__f[1] == L'*'), __consumed = -1, __r, __len;
            if (__f[1] == L'%') {
                if (*__b != L'%') break;
                ++__b; __f += 2;
                continue;
            }
            __len = __ccw_wspec_len(__start, &__end);
            if (__len < 0 || __len > 60) break;
            wmemcpy(__spec, __start, (size_t)__len);
            __spec[__len] = L'%'; __spec[__len + 1] = L'n'; __spec[__len + 2] = 0;
            if (__suppress) __r = swscanf(__b, __spec, &__consumed);
            else {
                void* __arg = va_arg(__ap, void*);
                __r = swscanf(__b, __spec, __arg, &__consumed);
            }
            if (__consumed < 0) break;
            __b += __consumed;
            __f = __end;
            if (!__suppress) {
                if (__r < 1) break;
                ++__total;
            }
        }
    }
    return __total;
}

static __inline int _ccw_vfwscanf(FILE* __fp, const wchar_t* __fmt, va_list __ap) {
    const wchar_t* __f = __fmt;
    wchar_t        __spec[64];
    int            __total = 0;
    while (*__f) {
        if (iswspace(*__f)) {
            wint_t __c;
            while ((__c = fgetwc(__fp)) != WEOF && iswspace(__c)) {}
            if (__c != WEOF) ungetwc(__c, __fp);
            ++__f;
            continue;
        }
        if (*__f != L'%') {
            wint_t __c = fgetwc(__fp);
            if (__c != (wint_t)*__f) { if (__c != WEOF) ungetwc(__c, __fp); break; }
            ++__f;
            continue;
        }
        {
            const wchar_t* __start = __f;
            const wchar_t* __end;
            int __suppress = (__f[1] == L'*'), __r, __len;
            if (__f[1] == L'%') {
                wint_t __c = fgetwc(__fp);
                if (__c != (wint_t)L'%') { if (__c != WEOF) ungetwc(__c, __fp); break; }
                __f += 2;
                continue;
            }
            __len = __ccw_wspec_len(__start, &__end);
            if (__len < 0 || __len > 62) break;
            wmemcpy(__spec, __start, (size_t)__len);
            __spec[__len] = 0;
            if (__suppress) __r = fwscanf(__fp, __spec);
            else {
                void* __arg = va_arg(__ap, void*);
                __r = fwscanf(__fp, __spec, __arg);
            }
            __f = __end;
            if (!__suppress) {
                if (__r < 1) break;
                ++__total;
            }
        }
    }
    return __total;
}

static __inline int vswscanf(const wchar_t* __b, const wchar_t* __f, va_list __a) {
    return _ccw_vswscanf(__b, __f, __a);
}
static __inline int vfwscanf(FILE* __fp, const wchar_t* __f, va_list __a) {
    return _ccw_vfwscanf(__fp, __f, __a);
}
static __inline int vwscanf(const wchar_t* __f, va_list __a) {
    return _ccw_vfwscanf(stdin, __f, __a);
}

#ifdef __cplusplus
}
#endif
#endif  /* _MSC_VER < 1800 */
