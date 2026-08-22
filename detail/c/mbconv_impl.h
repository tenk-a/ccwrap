/**
 *  @file   detail/c/mbconv_impl.h
 *  @brief  The multibyte <-> wide conversions ccwrap supplies in place of the CRT's,
 *          driven by the code page the ccwrap locale layer holds.
 *  @license Boost Software License Version 1.0
 *  @note
 *
 *  Include it from <stdlib.h> and <wchar.h> once wchar_t exists. mbstate_t is int
 *  on Open Watcom, so the restartable entries take int* and callers pass their
 *  mbstate_t* unchanged.
 */
#ifndef _CCW_DETAIL_MBCONV_IMPL_H
#define _CCW_DETAIL_MBCONV_IMPL_H

#include <ccwrap_common.h>
#include <stddef.h>
#include <errno.h>
#include "locale_impl.h"
#include "utf8_impl.h"

#if defined(_CCW_OS_WIN) || defined(_CCW_OS_DOS)

#define _CCW_MB_HAS_CONV        1
#define _CCW_MB_MAX_BYTES       4

#if defined(_CCW_OS_WIN)
#include "win/win32_fwd.h"
_CCW_EXTERN_C _CCW_WINAPI(int) MultiByteToWideChar(unsigned, unsigned long, const char *, int, wchar_t *, int);
_CCW_EXTERN_C _CCW_WINAPI(int) WideCharToMultiByte(unsigned, unsigned long, const wchar_t *, int, char *, int, const char *, int *);
_CCW_EXTERN_C _CCW_WINAPI(int) IsDBCSLeadByteEx(unsigned, unsigned char);
_CCW_EXTERN_C _CCW_WINAPI(int) GetCPInfo(unsigned, __ccw_pcpinfo);
#else
#include _CCW_NATIVE_C_HEADER_PATH(mbctype.h)
#endif

#define __CCW_MB_ERRFLAG        8u
#define __CCW_MB_SURRF          (1u << 27)

static inline int _ccw_mb_cur_max(void)
{
    if (_ccw_locale_is_utf8()) return 4;
#if defined(_CCW_OS_WIN)
    {
        unsigned __info[8];
        __info[0] = 0u;
        if (GetCPInfo(_ccw_locale_codepage(), (__ccw_pcpinfo)__info)
                && __info[0] >= 1u && __info[0] <= (unsigned)_CCW_MB_MAX_BYTES)
            return (int)__info[0];
    }
    return 2;
#else
    return __IsDBCS ? 2 : 1;
#endif
}

static inline int __ccw_mb_lead(unsigned __page, unsigned char __b)
{
#if defined(_CCW_OS_WIN)
    return IsDBCSLeadByteEx(__page, __b);
#else
    (void)__page;
    return _ismbblead(__b) != 0;
#endif
}

static inline int __ccw_mb_wide_only(unsigned __cp)
{
    return __cp == 65000u || __cp == 65001u;
}

static inline size_t __ccw_mb_fail(int *__ps)
{
    *__ps = 0;
    errno = EILSEQ;
    return (size_t)-1;
}

static inline size_t __ccw_mb_next(unsigned long *__pcp, const char *__s, size_t __n, int *__ps)
{
    unsigned      __st   = (unsigned)*__ps;
    unsigned long __acc  = (unsigned long)(__st & 0x1FFFFFu);
    unsigned      __need = (__st >> 21) & 7u;
    unsigned      __tot  = (__st >> 24) & 7u;
    unsigned      __page = _ccw_locale_codepage();
    size_t        __k    = 0;
    unsigned char __b;
    wchar_t       __w;
    char          __raw[2];

#if !defined(_CCW_OS_WIN)
    (void)__w; (void)__raw;
#endif
    if (_ccw_locale_is_utf8()) {
        while (__k < __n) {
            __b = (unsigned char)__s[__k];
            if (__need == 0u) {
                int __len = __ccw_u8_seqlen(__b);
                if (__len == 0) return __ccw_mb_fail(__ps);
                ++__k;
                if (__len == 1) { *__ps = 0; *__pcp = (unsigned long)__b; return __k; }
                __acc  = (unsigned long)(__b & (unsigned char)(0x7Fu >> __len));
                __need = (unsigned)(__len - 1);
                __tot  = (unsigned)__len;
            } else {
                if ((__b & 0xC0u) != 0x80u) return __ccw_mb_fail(__ps);
                __acc = (__acc << 6) | (unsigned long)(__b & 0x3Fu);
                ++__k;
                if (--__need == 0u) {
                    if (!__ccw_u8_valid(__acc, __tot)) return __ccw_mb_fail(__ps);
                    *__ps = 0; *__pcp = __acc; return __k;
                }
            }
        }
    } else {
        while (__k < __n) {
            __b = (unsigned char)__s[__k];
            if (__need == 0u) {
                ++__k;
                if (__b != 0u && __ccw_mb_lead(__page, __b)) {
                    __acc = (unsigned long)__b;
                    __need = 1u;
                    __tot  = 2u;
                    continue;
                }
#if defined(_CCW_OS_WIN)
                __raw[0] = (char)__b;
                if (MultiByteToWideChar(__page, __CCW_MB_ERRFLAG, __raw, 1, &__w, 1) != 1)
                    return __ccw_mb_fail(__ps);
                *__ps = 0; *__pcp = (unsigned long)__w; return __k;
#else
                *__ps = 0; *__pcp = (unsigned long)__b; return __k;
#endif
            }
            ++__k;
#if defined(_CCW_OS_WIN)
            __raw[0] = (char)(unsigned char)__acc;
            __raw[1] = (char)__b;
            if (MultiByteToWideChar(__page, __CCW_MB_ERRFLAG, __raw, 2, &__w, 1) != 1)
                return __ccw_mb_fail(__ps);
            *__ps = 0; *__pcp = (unsigned long)__w; return __k;
#else
            *__ps = 0; *__pcp = (__acc << 8) | (unsigned long)__b; return __k;
#endif
        }
    }
    *__ps = (int)((unsigned)__acc | (__need << 21) | (__tot << 24));
    return (size_t)-2;
}

static inline int __ccw_mb_put(char *__buf, unsigned long __cp)
{
    unsigned __page = _ccw_locale_codepage();
    wchar_t  __w[2];
    int      __wn, __r, __used = 0;

    if (_ccw_locale_is_utf8()) {
        if (__cp >= 0xD800uL && __cp <= 0xDFFFuL) return 0;
        return __ccw_u8_encode(__cp, __buf);
    }
    if (__cp > 0x10FFFFuL) return 0;
#if defined(_CCW_OS_WIN)
    if (__cp >= 0x10000uL) {
        unsigned long __v = __cp - 0x10000uL;
        __w[0] = (wchar_t)(0xD800uL + (__v >> 10));
        __w[1] = (wchar_t)(0xDC00uL + (__v & 0x3FFuL));
        __wn = 2;
    } else {
        __w[0] = (wchar_t)__cp;
        __wn = 1;
    }
    if (__ccw_mb_wide_only(__page)) {
        __r = WideCharToMultiByte(__page, 0, __w, __wn, __buf, _CCW_MB_MAX_BYTES, 0, 0);
        return __r > 0 ? __r : 0;
    }
    __r = WideCharToMultiByte(__page, 0, __w, __wn, __buf, _CCW_MB_MAX_BYTES, 0, &__used);
    if (__r <= 0 || __used) return 0;
    return __r;
#else
    (void)__w; (void)__wn; (void)__r; (void)__used;
    if (__cp <= 0xFFuL) { __buf[0] = (char)(unsigned char)__cp; return 1; }
    if (__cp <= 0xFFFFuL) {
        unsigned char __lead = (unsigned char)(__cp >> 8);
        if (!__ccw_mb_lead(__page, __lead)) return 0;
        __buf[0] = (char)__lead;
        __buf[1] = (char)(unsigned char)__cp;
        return 2;
    }
    return 0;
#endif
}

static inline int _ccw_mbsinit(const int *__ps)
{
    return __ps == 0 || *__ps == 0;
}

static inline size_t _ccw_mbrtowc(wchar_t *__pwc, const char *__s, size_t __n, int *__ps)
{
    static int    __internal = 0;
    unsigned long __cp = 0uL;
    size_t        __r;

    if (__ps == 0) __ps = &__internal;
    if (__s == 0) { __s = ""; __n = 1; __pwc = 0; }
    __r = __ccw_mb_next(&__cp, __s, __n, __ps);
    if (__r == (size_t)-1 || __r == (size_t)-2) return __r;
    if (__cp >= 0x10000uL) { *__ps = 0; errno = EILSEQ; return (size_t)-1; }
    if (__pwc != 0) *__pwc = (wchar_t)__cp;
    return __cp == 0uL ? (size_t)0 : __r;
}

static inline size_t _ccw_mbrlen(const char *__s, size_t __n, int *__ps)
{
    static int __internal = 0;
    return _ccw_mbrtowc(0, __s, __n, __ps != 0 ? __ps : &__internal);
}

static inline size_t _ccw_wcrtomb(char *__s, wchar_t __wc, int *__ps)
{
    static int    __internal = 0;
    char          __buf[_CCW_MB_MAX_BYTES];
    unsigned long __c = (unsigned long)__wc;
    unsigned      __st;
    int           __n;

    if (__ps == 0) __ps = &__internal;
    if (__s == 0) { __s = __buf; __c = 0uL; }
    __st = (unsigned)*__ps;
    if (__st & __CCW_MB_SURRF) {
        unsigned long __hi = (unsigned long)(__st & 0xFFFFu);
        if (__c < 0xDC00uL || __c > 0xDFFFuL) { *__ps = 0; errno = EILSEQ; return (size_t)-1; }
        __c = 0x10000uL + ((__hi - 0xD800uL) << 10) + (__c - 0xDC00uL);
        *__ps = 0;
    } else if (__c >= 0xD800uL && __c <= 0xDBFFuL) {
        *__ps = (int)((unsigned)__c | __CCW_MB_SURRF);
        return 0;
    } else if (__c >= 0xDC00uL && __c <= 0xDFFFuL) {
        *__ps = 0; errno = EILSEQ; return (size_t)-1;
    }
    __n = __ccw_mb_put(__s, __c);
    if (__n <= 0) { *__ps = 0; errno = EILSEQ; return (size_t)-1; }
    return (size_t)__n;
}

static inline int _ccw_mbtowc(wchar_t *__pwc, const char *__s, size_t __n)
{
    int           __st = 0;
    unsigned long __cp = 0uL;
    size_t        __r;

    if (__s == 0) return 0;
    if (__n == 0) return -1;
    __r = __ccw_mb_next(&__cp, __s, __n, &__st);
    if (__r == (size_t)-1 || __r == (size_t)-2) { errno = EILSEQ; return -1; }
    if (__cp >= 0x10000uL) { errno = EILSEQ; return -1; }
    if (__pwc != 0) *__pwc = (wchar_t)__cp;
    return __cp == 0uL ? 0 : (int)__r;
}

static inline int _ccw_mblen(const char *__s, size_t __n)
{
    return _ccw_mbtowc(0, __s, __n);
}

static inline int _ccw_wctomb(char *__s, wchar_t __wc)
{
    int __n;
    if (__s == 0) return 0;
    if ((unsigned long)__wc >= 0xD800uL && (unsigned long)__wc <= 0xDFFFuL) { errno = EILSEQ; return -1; }
    __n = __ccw_mb_put(__s, (unsigned long)__wc);
    if (__n <= 0) { errno = EILSEQ; return -1; }
    return __n;
}

static inline size_t _ccw_mbsrtowcs(wchar_t *__dst, const char **__src, size_t __n, int *__ps)
{
    static int    __internal = 0;
    const char   *__p;
    unsigned long __cp;
    size_t        __r, __out = 0;
    int           __wide;

    if (__ps == 0) __ps = &__internal;
    __p = *__src;
    for (;;) {
        __cp = 0uL;
        __r  = __ccw_mb_next(&__cp, __p, (size_t)_CCW_MB_MAX_BYTES, __ps);
        if (__r == (size_t)-1) return (size_t)-1;
        if (__r == (size_t)-2) { *__ps = 0; errno = EILSEQ; return (size_t)-1; }
        if (__cp == 0uL) {
            if (__dst != 0) {
                if (__out >= __n) break;
                __dst[__out] = 0;
                *__src = 0;
            }
            return __out;
        }
        __wide = __cp >= 0x10000uL ? 2 : 1;
        if (__dst != 0 && __out + (size_t)__wide > __n) break;
        if (__dst != 0) {
            if (__wide == 2) {
                unsigned long __v = __cp - 0x10000uL;
                __dst[__out]     = (wchar_t)(0xD800uL + (__v >> 10));
                __dst[__out + 1] = (wchar_t)(0xDC00uL + (__v & 0x3FFuL));
            } else {
                __dst[__out] = (wchar_t)__cp;
            }
        }
        __out += (size_t)__wide;
        __p   += __r;
    }
    if (__dst != 0) *__src = __p;
    return __out;
}

static inline size_t _ccw_wcsrtombs(char *__dst, const wchar_t **__src, size_t __n, int *__ps)
{
    static int     __internal = 0;
    const wchar_t *__p;
    char           __buf[_CCW_MB_MAX_BYTES];
    unsigned long  __cp;
    size_t         __out = 0;
    int            __k, __step, __i;

    if (__ps == 0) __ps = &__internal;
    __p = *__src;
    for (;;) {
        __cp   = (unsigned long)*__p;
        __step = 1;
        if (__cp >= 0xD800uL && __cp <= 0xDBFFuL) {
            unsigned long __lo = (unsigned long)__p[1];
            if (__lo < 0xDC00uL || __lo > 0xDFFFuL) { errno = EILSEQ; return (size_t)-1; }
            __cp   = 0x10000uL + ((__cp - 0xD800uL) << 10) + (__lo - 0xDC00uL);
            __step = 2;
        } else if (__cp >= 0xDC00uL && __cp <= 0xDFFFuL) {
            errno = EILSEQ;
            return (size_t)-1;
        }
        if (__cp == 0uL) {
            if (__dst != 0) {
                if (__out >= __n) break;
                __dst[__out] = 0;
                *__src = 0;
            }
            return __out;
        }
        __k = __ccw_mb_put(__buf, __cp);
        if (__k <= 0) { errno = EILSEQ; return (size_t)-1; }
        if (__dst != 0 && __out + (size_t)__k > __n) break;
        if (__dst != 0) {
            for (__i = 0; __i < __k; ++__i) __dst[__out + (size_t)__i] = __buf[__i];
        }
        __out += (size_t)__k;
        __p   += __step;
    }
    if (__dst != 0) *__src = __p;
    return __out;
}

static inline size_t _ccw_mbstowcs(wchar_t *__dst, const char *__src, size_t __n)
{
    const char *__p  = __src;
    int         __st = 0;
    return _ccw_mbsrtowcs(__dst, &__p, __n, &__st);
}

static inline size_t _ccw_wcstombs(char *__dst, const wchar_t *__src, size_t __n)
{
    const wchar_t *__p  = __src;
    int            __st = 0;
    return _ccw_wcsrtombs(__dst, &__p, __n, &__st);
}

#endif  /* _CCW_OS_WIN || _CCW_OS_DOS */

#endif  /* _CCW_DETAIL_MBCONV_IMPL_H */
