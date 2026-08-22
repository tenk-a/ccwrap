/**
 *  @file   detail/uchar_impl.h
 *  @brief  Implementation of the <uchar.h> conversions: mbrtoc8/c8rtomb, mbrtoc16/c16rtomb, mbrtoc32/c32rtomb.
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_DETAIL_UCHAR_IMPL_H
#define _CCW_DETAIL_UCHAR_IMPL_H

#ifndef __STDC_VERSION_UCHAR_H__
#define __STDC_VERSION_UCHAR_H__ 202311L
#endif

#define __CCW_HAS_MBRTOC8       1

#include <ccwrap_common.h>
#include <stddef.h>     /* size_t */
#include <errno.h>      /* EILSEQ */
#include <wchar.h>      /* mbstate_t */

#include "utf8_impl.h"

#define __CCW_UC_SURR16         (1u << 28)
#define __CCW_UC_PEND8          (1u << 29)
#define __CCW_UC_ACC(st)        ((unsigned long)((st) & 0x1FFFFFu))
#define __CCW_UC_NEED(st)       (((st) >> 21) & 7u)
#define __CCW_UC_TOTAL(st)      (((st) >> 24) & 7u)

static inline unsigned *__ccw_uc_state(mbstate_t *__ps, mbstate_t *__fallback)
{
    return (unsigned *)(void *)(__ps ? __ps : __fallback);
}

static inline size_t __ccw_uc_next(unsigned long *__pcp, const char *__s, size_t __n, unsigned *__st)
{
#if defined(_CCW_MB_HAS_CONV)
    return __ccw_mb_next(__pcp, __s, __n, (int *)(void *)__st);
#else
    int __len;
    (void)__st;
    __len = __ccw_u8_decode(__s, __n, __pcp);
    if (__len == -1) { errno = EILSEQ; return (size_t)-1; }
    if (__len == -2) return (size_t)-2;
    return (size_t)__len;
#endif
}

static inline int __ccw_uc_put(char *__buf, unsigned long __cp)
{
#if defined(_CCW_MB_HAS_CONV)
    return __ccw_mb_put(__buf, __cp);
#else
    return __ccw_u8_encode(__cp, __buf);
#endif
}

static inline size_t mbrtoc8(_ccw_char8 *__pc8, const char *__s, size_t __n, mbstate_t *__ps)
{
    static mbstate_t __internal;
    unsigned      *__st = __ccw_uc_state(__ps, &__internal);
    unsigned long  __cp = 0uL;
    char           __buf[4];
    unsigned       __left;
    int            __len;
    size_t         __r;

    if (__s == 0) { *__st = 0u; return 0; }
    if (*__st & __CCW_UC_PEND8) {
        __cp   = __CCW_UC_ACC(*__st);
        __left = __CCW_UC_NEED(*__st);
        __len  = __ccw_u8_encode(__cp, __buf);
        if (__pc8) *__pc8 = (_ccw_char8)(unsigned char)__buf[__len - (int)__left];
        --__left;
        *__st = __left ? (__CCW_UC_PEND8 | (unsigned)__cp | (__left << 21)) : 0u;
        return (size_t)-3;
    }
    __r = __ccw_uc_next(&__cp, __s, __n, __st);
    if (__r == (size_t)-1 || __r == (size_t)-2) return __r;
    if (__cp == 0uL) { if (__pc8) *__pc8 = 0; return 0; }
    __len = __ccw_u8_encode(__cp, __buf);
    if (__len <= 0) { *__st = 0u; errno = EILSEQ; return (size_t)-1; }
    if (__pc8) *__pc8 = (_ccw_char8)(unsigned char)__buf[0];
    if (__len > 1)
        *__st = __CCW_UC_PEND8 | (unsigned)__cp | ((unsigned)(__len - 1) << 21);
    return __r;
}

static inline size_t c8rtomb(char *__s, _ccw_char8 __c8, mbstate_t *__ps)
{
    static mbstate_t __internal;
    unsigned      *__st = __ccw_uc_state(__ps, &__internal);
    unsigned char  __b  = (unsigned char)__c8;
    unsigned long  __acc;
    unsigned       __need, __total;
    int            __len, __seq;

    if (__s == 0) { *__st = 0u; return 1; }
    __acc   = __CCW_UC_ACC(*__st);
    __need  = __CCW_UC_NEED(*__st);
    __total = __CCW_UC_TOTAL(*__st);
    if (__need == 0u) {
        __seq = __ccw_u8_seqlen(__b);
        if (__seq == 0) { *__st = 0u; errno = EILSEQ; return (size_t)-1; }
        if (__seq == 1) {
            __len = __ccw_uc_put(__s, (unsigned long)__b);
            if (__len <= 0) { errno = EILSEQ; return (size_t)-1; }
            return (size_t)__len;
        }
        __acc = (unsigned long)(__b & (unsigned char)(0x7Fu >> __seq));
        *__st = (unsigned)__acc | ((unsigned)(__seq - 1) << 21) | ((unsigned)__seq << 24);
        return 0;
    }
    if ((__b & 0xC0u) != 0x80u) { *__st = 0u; errno = EILSEQ; return (size_t)-1; }
    __acc = (__acc << 6) | (unsigned long)(__b & 0x3Fu);
    if (--__need == 0u) {
        *__st = 0u;
        if (!__ccw_u8_valid(__acc, __total)) { errno = EILSEQ; return (size_t)-1; }
        __len = __ccw_uc_put(__s, __acc);
        if (__len <= 0) { errno = EILSEQ; return (size_t)-1; }
        return (size_t)__len;
    }
    *__st = (unsigned)__acc | (__need << 21) | (__total << 24);
    return 0;
}

#ifndef __CCW_UCHAR_IMPL_C8_ONLY

static inline size_t mbrtoc32(_ccw_char32 *__pc32, const char *__s, size_t __n, mbstate_t *__ps)
{
    static mbstate_t __internal;
    unsigned      *__st = __ccw_uc_state(__ps, &__internal);
    unsigned long  __cp = 0uL;
    size_t         __r;

    if (__s == 0) { *__st = 0u; return 0; }
    __r = __ccw_uc_next(&__cp, __s, __n, __st);
    if (__r == (size_t)-1 || __r == (size_t)-2) return __r;
    if (__pc32) *__pc32 = (_ccw_char32)__cp;
    return __cp == 0uL ? (size_t)0 : __r;
}

static inline size_t c32rtomb(char *__s, _ccw_char32 __c32, mbstate_t *__ps)
{
    static mbstate_t __internal;
    unsigned      *__st = __ccw_uc_state(__ps, &__internal);
    unsigned long  __cp = (unsigned long)__c32;
    int            __len;

    if (__s == 0) { *__st = 0u; return 1; }
    if (__cp >= 0xD800uL && __cp <= 0xDFFFuL) { errno = EILSEQ; return (size_t)-1; }
    __len = __ccw_uc_put(__s, __cp);
    if (__len <= 0) { errno = EILSEQ; return (size_t)-1; }
    return (size_t)__len;
}

static inline size_t mbrtoc16(_ccw_char16 *__pc16, const char *__s, size_t __n, mbstate_t *__ps)
{
    static mbstate_t __internal;
    unsigned      *__st = __ccw_uc_state(__ps, &__internal);
    unsigned long  __cp = 0uL;
    size_t         __r;

    if (__s == 0) { *__st = 0u; return 0; }
    if (*__st & __CCW_UC_SURR16) {
        unsigned __lo = (unsigned)(*__st & 0xFFFFu);
        *__st = 0u;
        if (__pc16) *__pc16 = (_ccw_char16)__lo;
        return (size_t)-3;
    }
    __r = __ccw_uc_next(&__cp, __s, __n, __st);
    if (__r == (size_t)-1 || __r == (size_t)-2) return __r;
    if (__cp == 0uL) { if (__pc16) *__pc16 = 0; return 0; }
    if (__cp <= 0xFFFFuL) { if (__pc16) *__pc16 = (_ccw_char16)__cp; return __r; }
    {
        unsigned long __v = __cp - 0x10000uL;
        if (__pc16) *__pc16 = (_ccw_char16)(0xD800uL + (__v >> 10));
        *__st = (unsigned)(0xDC00uL + (__v & 0x3FFuL)) | __CCW_UC_SURR16;
    }
    return __r;
}

static inline size_t c16rtomb(char *__s, _ccw_char16 __c16, mbstate_t *__ps)
{
    static mbstate_t __internal;
    unsigned      *__st = __ccw_uc_state(__ps, &__internal);
    unsigned       __u  = (unsigned)__c16;
    unsigned long  __cp;
    int            __len;

    if (__s == 0) { *__st = 0u; return 1; }
    if (*__st & __CCW_UC_SURR16) {
        unsigned __hi = (unsigned)(*__st & 0xFFFFu);
        *__st = 0u;
        if (__u < 0xDC00u || __u > 0xDFFFu) { errno = EILSEQ; return (size_t)-1; }
        __cp = 0x10000uL + ((unsigned long)(__hi - 0xD800u) << 10) + (unsigned long)(__u - 0xDC00u);
    } else if (__u >= 0xD800u && __u <= 0xDBFFu) {
        *__st = __u | __CCW_UC_SURR16;
        return 0;
    } else if (__u >= 0xDC00u && __u <= 0xDFFFu) {
        errno = EILSEQ;
        return (size_t)-1;
    } else {
        __cp = (unsigned long)__u;
    }
    __len = __ccw_uc_put(__s, __cp);
    if (__len <= 0) { errno = EILSEQ; return (size_t)-1; }
    return (size_t)__len;
}

#endif  /* !__CCW_UCHAR_IMPL_C8_ONLY */

#endif  /* _CCW_DETAIL_UCHAR_IMPL_H */
