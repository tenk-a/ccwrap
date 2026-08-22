/**
 *  @file   detail/c/utf8_impl.h
 *  @brief  UTF-8 sequence length, decode and encode, shared by <uchar.h> and the locale layer.
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_DETAIL_UTF8_IMPL_H
#define _CCW_DETAIL_UTF8_IMPL_H

static inline int __ccw_u8_seqlen(unsigned char __lead)
{
    if (__lead < 0x80u)             return 1;    /* ASCII (including 0) */
    if ((__lead & 0xE0u) == 0xC0u)  return 2;
    if ((__lead & 0xF0u) == 0xE0u)  return 3;
    if ((__lead & 0xF8u) == 0xF0u)  return 4;
    return 0;                                    /* 0x80..0xBF (continuation) / 0xF8.. (invalid) */
}


static inline int __ccw_u8_decode(const char *__s, size_t __n, unsigned long *__cp)
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

static inline int __ccw_u8_encode(unsigned long __c, char *__buf)
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

static inline int __ccw_u8_valid(unsigned long __c, unsigned __total)
{
    if (__c > 0x10FFFFuL)                       return 0;
    if (__c >= 0xD800uL && __c <= 0xDFFFuL)     return 0;
    if (__total == 2u && __c < 0x80uL)          return 0;
    if (__total == 3u && __c < 0x800uL)         return 0;
    if (__total == 4u && __c < 0x10000uL)       return 0;
    return 1;
}

#endif  /* _CCW_DETAIL_UTF8_IMPL_H */
