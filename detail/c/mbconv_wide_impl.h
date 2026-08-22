/**
 *  @file   detail/c/mbconv_wide_impl.h
 *  @brief  btowc and wctob for the ccwrap locale layer; they need wint_t and WEOF.
 *  @license Boost Software License Version 1.0
 *  @note   Include it from <wchar.h>, after the native header has defined wint_t.
 */
#ifndef _CCW_DETAIL_MBCONV_WIDE_IMPL_H
#define _CCW_DETAIL_MBCONV_WIDE_IMPL_H

#include "mbconv_impl.h"

#if defined(_CCW_MB_HAS_CONV)

static inline wint_t _ccw_btowc(int __c)
{
    char          __b;
    int           __st = 0;
    unsigned long __cp = 0uL;
    size_t        __r;

    if (__c < 0 || __c > 255) return WEOF;
    __b = (char)(unsigned char)__c;
    __r = __ccw_mb_next(&__cp, &__b, (size_t)1, &__st);
    if (__r != (size_t)1 || __cp > 0xFFFFuL) return WEOF;
    return (wint_t)__cp;
}

static inline int _ccw_wctob(wint_t __c)
{
    char __buf[_CCW_MB_MAX_BYTES];
    int  __n;

    if (__c == WEOF) return -1;
    __n = __ccw_mb_put(__buf, (unsigned long)__c);
    if (__n != 1) return -1;
    return (int)(unsigned char)__buf[0];
}

#endif  /* _CCW_MB_HAS_CONV */

#endif  /* _CCW_DETAIL_MBCONV_WIDE_IMPL_H */
