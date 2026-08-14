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

#include <ccwrap_common.h>
#include <stddef.h>     /* size_t */
#include <errno.h>      /* EILSEQ */
#include <wchar.h>      /* mbstate_t */

static int __ccw_u8_seqlen(unsigned char __lead)
{
    if (__lead < 0x80u)             return 1;    /* ASCII (including 0) */
    if ((__lead & 0xE0u) == 0xC0u)  return 2;
    if ((__lead & 0xF0u) == 0xE0u)  return 3;
    if ((__lead & 0xF8u) == 0xF0u)  return 4;
    return 0;                                    /* 0x80..0xBF (continuation) / 0xF8.. (invalid) */
}

static int __ccw_u8_decode(const char *__s, size_t __n, unsigned long *__cp)
{
    unsigned char __lead;
    int           __len, __i;
    unsigned long __c;

    if (__n == 0) return -2;
    __lead = (unsigned char)__s[0];
    __len  = __ccw_u8_seqlen(__lead);
    if (__len == 0)          return -1;
    if ((size_t)__len > __n) return -2;
    for (__i = 1; __i < __len; ++__i)
        if (((unsigned char)__s[__i] & 0xC0u) != 0x80u) return -1;

    if      (__len == 1) __c = __lead;
    else if (__len == 2) __c = ((unsigned long)(__lead & 0x1Fu) << 6)
                             |  ((unsigned char)__s[1] & 0x3Fu);
    else if (__len == 3) __c = ((unsigned long)(__lead & 0x0Fu) << 12)
                             | (((unsigned long)((unsigned char)__s[1] & 0x3Fu)) << 6)
                             |  ((unsigned char)__s[2] & 0x3Fu);
    else                 __c = ((unsigned long)(__lead & 0x07u) << 18)
                             | (((unsigned long)((unsigned char)__s[1] & 0x3Fu)) << 12)
                             | (((unsigned long)((unsigned char)__s[2] & 0x3Fu)) << 6)
                             |  ((unsigned char)__s[3] & 0x3Fu);
    *__cp = __c;
    return __len;
}

static int __ccw_u8_encode(unsigned long __c, char *__buf)
{
    if (__c < 0x80u) {
        __buf[0] = (char)__c; return 1;
    }
    if (__c < 0x800u) {
        __buf[0] = (char)(0xC0u | (__c >> 6));
        __buf[1] = (char)(0x80u | (__c & 0x3Fu));
        return 2;
    }
    if (__c < 0x10000u) {
        __buf[0] = (char)(0xE0u | (__c >> 12));
        __buf[1] = (char)(0x80u | ((__c >> 6) & 0x3Fu));
        __buf[2] = (char)(0x80u | (__c & 0x3Fu));
        return 3;
    }
    if (__c < 0x110000u) {
        __buf[0] = (char)(0xF0u | (__c >> 18));
        __buf[1] = (char)(0x80u | ((__c >> 12) & 0x3Fu));
        __buf[2] = (char)(0x80u | ((__c >> 6) & 0x3Fu));
        __buf[3] = (char)(0x80u | (__c & 0x3Fu));
        return 4;
    }
    return 0;
}

static size_t mbrtoc8(char8_t *__pc8, const char *__s, size_t __n, mbstate_t *__ps)
{
    static mbstate_t __internal;   /* static storage => zero-initialized (mbstate_t is a
                                      struct on glibc, so `= 0` would be an invalid
                                      initializer; scalar on Watcom -- zero-init covers both) */
    unsigned char   *__st;
    unsigned char    __lead;
    int              __len, __i, __rem;

    __st = (unsigned char *)(__ps ? (void *)__ps : (void *)&__internal);
    if (__s == 0) { __st[0] = __st[1] = __st[2] = __st[3] = 0; return 0; }
    if (__st[0] != 0) {                          /* pending output code unit */
        if (__pc8) *__pc8 = (char8_t)__st[1];
        __st[1] = __st[2]; __st[2] = __st[3]; __st[3] = 0; --__st[0];
        return (size_t)-3;
    }
    if (__n == 0) return (size_t)-2;
    __lead = (unsigned char)__s[0];
    __len  = __ccw_u8_seqlen(__lead);
    if (__len == 0)          { errno = EILSEQ; return (size_t)-1; }
    if ((size_t)__len > __n) return (size_t)-2;
    for (__i = 1; __i < __len; ++__i)
        if (((unsigned char)__s[__i] & 0xC0u) != 0x80u) { errno = EILSEQ; return (size_t)-1; }
    if (__lead == 0) { if (__pc8) *__pc8 = 0; return 0; }
    if (__pc8) *__pc8 = (char8_t)__lead;
    __rem = __len - 1;
    for (__i = 0; __i < __rem; ++__i) __st[1 + __i] = (unsigned char)__s[1 + __i];
    __st[0] = (unsigned char)__rem;
    return (size_t)__len;
}

static size_t c8rtomb(char *__s, char8_t __c8, mbstate_t *__ps)
{
    static mbstate_t __internal;   /* static storage => zero-initialized (mbstate_t is a
                                      struct on glibc, so `= 0` would be an invalid
                                      initializer; scalar on Watcom -- zero-init covers both) */
    unsigned char   *__st;
    unsigned char    __b, __lead;
    int              __len, __i, __cnt;

    __st = (unsigned char *)(__ps ? (void *)__ps : (void *)&__internal);
    if (__s == 0) { __st[0] = __st[1] = __st[2] = __st[3] = 0; return 1; }
    __b = (unsigned char)__c8;
    __cnt = (int)__st[0];
    if (__cnt == 0) {
        __len = __ccw_u8_seqlen(__b);
        if (__len == 0) { errno = EILSEQ; return (size_t)-1; }
        if (__len == 1) { __s[0] = (char)__b; return 1; }
        __st[1] = __b; __st[0] = 1;
        return 0;
    }
    if ((__b & 0xC0u) != 0x80u) { errno = EILSEQ; return (size_t)-1; }
    __lead = __st[1];
    __len  = __ccw_u8_seqlen(__lead);
    if (__cnt + 1 == __len) {
        for (__i = 0; __i < __cnt; ++__i) __s[__i] = (char)__st[1 + __i];
        __s[__cnt] = (char)__b;
        __st[0] = __st[1] = __st[2] = __st[3] = 0;
        return (size_t)__len;
    }
    __st[1 + __cnt] = __b;
    __st[0] = (unsigned char)(__cnt + 1);
    return 0;
}

static size_t mbrtoc32(char32_t *__pc32, const char *__s, size_t __n, mbstate_t *__ps)
{
    unsigned long __cp;
    int           __len;
    (void)__ps;                                  /* partial input state is not kept (simplified) */
    if (__s == 0) return 0;                       /* same as mbrtoc32(NULL,"",1,ps) */
    __len = __ccw_u8_decode(__s, __n, &__cp);
    if (__len == -1) { errno = EILSEQ; return (size_t)-1; }
    if (__len == -2) return (size_t)-2;
    if (__cp == 0) { if (__pc32) *__pc32 = 0; return 0; }
    if (__pc32) *__pc32 = (char32_t)__cp;
    return (size_t)__len;
}

static size_t c32rtomb(char *__s, char32_t __c32, mbstate_t *__ps)
{
    unsigned long __c;
    int           __len;
    (void)__ps;
    if (__s == 0) return 1;                       /* no shift state */
    __c = (unsigned long)__c32;
    if (__c >= 0xD800u && __c <= 0xDFFFu) { errno = EILSEQ; return (size_t)-1; }  /* surrogate */
    __len = __ccw_u8_encode(__c, __s);
    if (__len == 0) { errno = EILSEQ; return (size_t)-1; }
    return (size_t)__len;
}

static size_t mbrtoc16(char16_t *__pc16, const char *__s, size_t __n, mbstate_t *__ps)
{
    static mbstate_t __internal;   /* static storage => zero-initialized (mbstate_t is a
                                      struct on glibc, so `= 0` would be an invalid
                                      initializer; scalar on Watcom -- zero-init covers both) */
    unsigned char   *__st;
    unsigned long    __cp;
    int              __len;
    unsigned         __hi, __lo;

    __st = (unsigned char *)(__ps ? (void *)__ps : (void *)&__internal);
    if (__s == 0) { __st[0] = __st[1] = __st[2] = __st[3] = 0; return 0; }
    if (__st[0] != 0) {                           /* pending low surrogate */
        __lo = (unsigned)__st[1] | ((unsigned)__st[2] << 8);
        if (__pc16) *__pc16 = (char16_t)__lo;
        __st[0] = __st[1] = __st[2] = __st[3] = 0;
        return (size_t)-3;
    }
    __len = __ccw_u8_decode(__s, __n, &__cp);
    if (__len == -1) { errno = EILSEQ; return (size_t)-1; }
    if (__len == -2) return (size_t)-2;
    if (__cp == 0) { if (__pc16) *__pc16 = 0; return 0; }
    if (__cp <= 0xFFFFu) { if (__pc16) *__pc16 = (char16_t)__cp; return (size_t)__len; }
    __cp -= 0x10000u;                             /* split into a surrogate pair */
    __hi = 0xD800u | (unsigned)(__cp >> 10);
    __lo = 0xDC00u | (unsigned)(__cp & 0x3FFu);
    if (__pc16) *__pc16 = (char16_t)__hi;
    __st[1] = (unsigned char)(__lo & 0xFFu);
    __st[2] = (unsigned char)((__lo >> 8) & 0xFFu);
    __st[0] = 1;
    return (size_t)__len;
}

static size_t c16rtomb(char *__s, char16_t __c16, mbstate_t *__ps)
{
    static mbstate_t __internal;   /* static storage => zero-initialized (mbstate_t is a
                                      struct on glibc, so `= 0` would be an invalid
                                      initializer; scalar on Watcom -- zero-init covers both) */
    unsigned char   *__st;
    unsigned         __u, __hi;
    unsigned long    __cp;
    int              __len;

    __st = (unsigned char *)(__ps ? (void *)__ps : (void *)&__internal);
    if (__s == 0) { __st[0] = __st[1] = __st[2] = __st[3] = 0; return 1; }
    __u = (unsigned)__c16;
    if (__st[0] != 0) {                           /* expecting the low surrogate */
        __hi = (unsigned)__st[1] | ((unsigned)__st[2] << 8);
        __st[0] = __st[1] = __st[2] = __st[3] = 0;
        if (__u < 0xDC00u || __u > 0xDFFFu) { errno = EILSEQ; return (size_t)-1; }
        __cp = 0x10000u + (((unsigned long)(__hi - 0xD800u)) << 10) + (__u - 0xDC00u);
        __len = __ccw_u8_encode(__cp, __s);
        return (size_t)__len;
    }
    if (__u >= 0xD800u && __u <= 0xDBFFu) {        /* high surrogate: keep in the state */
        __st[1] = (unsigned char)(__u & 0xFFu);
        __st[2] = (unsigned char)((__u >> 8) & 0xFFu);
        __st[0] = 1;
        return 0;
    }
    if (__u >= 0xDC00u && __u <= 0xDFFFu) { errno = EILSEQ; return (size_t)-1; }  /* unpaired low surrogate */
    __len = __ccw_u8_encode((unsigned long)__u, __s);
    return (size_t)__len;
}

#endif  /* _CCW_DETAIL_UCHAR_IMPL_H */
