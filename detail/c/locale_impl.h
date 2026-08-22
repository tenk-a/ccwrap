/**
 *  @file   detail/c/locale_impl.h
 *  @brief  ccwrap's own C locale layer: struct lconv, setlocale and localeconv.
 *  @license Boost Software License Version 1.0
 *  @note
 *
 *  Include it at global scope from <locale.h> and from <clocale>; the caller
 *  pre-empts the native header's include guard and defines setlocale and
 *  localeconv to the _ccw_ entries afterwards.
 */
#ifndef _CCW_DETAIL_LOCALE_IMPL_H
#define _CCW_DETAIL_LOCALE_IMPL_H

#include <ccwrap_common.h>
#include <limits.h>

#define _CCW_CP_UTF8            65001u
#define _CCW_CP_U8FLAG          0x10000u
#define _CCW_LCONV_HAS_INT_MEMBERS  1

#ifndef LC_ALL
#define LC_CTYPE                0
#define LC_NUMERIC              1
#define LC_TIME                 2
#define LC_COLLATE              3
#define LC_MONETARY             4
#define LC_MESSAGES             5
#define LC_ALL                  6
#define LC_MIN                  LC_CTYPE
#define LC_MAX                  LC_ALL
#endif

#ifndef _CCW_LCONV_DEFINED
#define _CCW_LCONV_DEFINED
#if defined(__WATCOMC__) && defined(_M_IX86)
#pragma pack(__push, 1)
#endif
struct lconv {
    char *decimal_point;
    char *thousands_sep;
    char *int_curr_symbol;
    char *currency_symbol;
    char *mon_decimal_point;
    char *mon_thousands_sep;
    char *mon_grouping;
    char *grouping;
    char *positive_sign;
    char *negative_sign;
    char int_frac_digits;
    char frac_digits;
    char p_cs_precedes;
    char p_sep_by_space;
    char n_cs_precedes;
    char n_sep_by_space;
    char p_sign_posn;
    char n_sign_posn;
    char int_p_cs_precedes;
    char int_p_sep_by_space;
    char int_n_cs_precedes;
    char int_n_sep_by_space;
    char int_p_sign_posn;
    char int_n_sign_posn;
};
#if defined(__WATCOMC__) && defined(_M_IX86)
#pragma pack(__pop)
#endif
#endif  /* _CCW_LCONV_DEFINED */

#if defined(_CCW_OS_WIN)
_CCW_EXTERN_C _CCW_WINAPI(unsigned) GetACP(void);
_CCW_EXTERN_C _CCW_WINAPI(int) IsValidCodePage(unsigned);
#endif
_CCW_EXTERN_C unsigned __MBCodePage;

static inline int _ccw_locale_is_utf8(void)
{
#if defined(_CCW_OS_WIN)
    return __MBCodePage == _CCW_CP_UTF8;
#else
    return (__MBCodePage & _CCW_CP_U8FLAG) != 0u;
#endif
}

static inline unsigned _ccw_locale_default_codepage(void)
{
#if defined(_CCW_OS_WIN)
    return (unsigned)GetACP();
#else
    return __MBCodePage & 0xFFFFu;
#endif
}

static inline unsigned _ccw_locale_codepage(void)
{
    return _ccw_locale_is_utf8() ? _CCW_CP_UTF8 : _ccw_locale_default_codepage();
}

static inline int __ccw_lc_set_cp(unsigned __cp)
{
    if (__cp == 0u) __cp = _ccw_locale_default_codepage();
#if defined(_CCW_OS_WIN)
    if (__cp == __MBCodePage) return 1;
    if (!IsValidCodePage(__cp)) return 0;
    __MBCodePage = __cp;
    return 1;
#else
    if (__cp == _CCW_CP_UTF8)                     { __MBCodePage |=  _CCW_CP_U8FLAG; return 1; }
    if (__cp == _ccw_locale_default_codepage())   { __MBCodePage &= ~_CCW_CP_U8FLAG; return 1; }
    return 0;
#endif
}

static inline char __ccw_lc_lower(char __c)
{
    return (__c >= 'A' && __c <= 'Z') ? (char)(__c - 'A' + 'a') : __c;
}

static inline int __ccw_lc_tag_eq(const char *__s, const char *__end, const char *__tag)
{
    while (__s != __end && *__s != 0) {
        char __c = __ccw_lc_lower(*__s++);
        if (__c == '-' || __c == '_') continue;
        if (*__tag == 0 || __c != *__tag) return 0;
        ++__tag;
    }
    return *__tag == 0;
}

static inline int __ccw_lc_number(const char *__s, const char *__end, unsigned *__v)
{
    unsigned __n = 0u;
    if (__s == __end || *__s == 0) return 0;
    while (__s != __end && *__s != 0) {
        if (*__s < '0' || *__s > '9') return 0;
        __n = __n * 10u + (unsigned)(*__s++ - '0');
    }
    *__v = __n;
    return 1;
}

static inline const char *__ccw_lc_find(const char *__s, char __c)
{
    const char *__r = 0;
    for (; *__s != 0; ++__s) { if (*__s == __c) __r = __s; }
    return __r;
}

static inline long __ccw_lc_codeset(const char *__s, const char *__end)
{
    unsigned __cp;
    if (__s == __end || *__s == 0)              return 0L;
    if (__ccw_lc_tag_eq(__s, __end, "utf8"))    return (long)_CCW_CP_UTF8;
    if (__ccw_lc_number(__s, __end, &__cp))     return (long)__cp;
    return -1L;
}

static inline int __ccw_lc_lang_ok(const char *__s, const char *__end)
{
    if (__s == __end || *__s == 0)              return 1;
    if (__ccw_lc_tag_eq(__s, __end, "c"))       return 1;
    if (__ccw_lc_tag_eq(__s, __end, "posix"))   return 1;
    return 0;
}

static inline char *_ccw_locale_name(void)
{
    static char __buf[24];
    unsigned    __cp = _ccw_locale_codepage();
    char       *__d  = __buf;

    *__d++ = 'C';
    if (__cp != _ccw_locale_default_codepage()) {
        *__d++ = '.';
        if (__cp == _CCW_CP_UTF8) {
            const char *__s = "UTF-8";
            while (*__s != 0) *__d++ = *__s++;
        } else {
            char __tmp[12];
            int  __i = 0;
            do { __tmp[__i++] = (char)('0' + (int)(__cp % 10u)); __cp /= 10u; } while (__cp != 0u);
            while (__i > 0) *__d++ = __tmp[--__i];
        }
    }
    *__d = 0;
    return __buf;
}

static inline char *_ccw_setlocale(int __cat, const char *__loc)
{
    const char *__dot;
    const char *__at;
    const char *__end;
    long        __cp = 0L;

    if (__cat < LC_MIN || __cat > LC_MAX) return 0;
    if (__loc == 0) return _ccw_locale_name();

    __at  = __ccw_lc_find(__loc, '@');
    __dot = __ccw_lc_find(__loc, '.');
    if (__at != 0 && __dot != 0 && __dot > __at) __dot = 0;
    __end = __at;

    if (__dot != 0) {
        __cp = __ccw_lc_codeset(__dot + 1, __end);
        if (__cp < 0L) return 0;
        __end = __dot;
    }
    if (__cp == 0L) {
        if (!__ccw_lc_lang_ok(__loc, __end)) return 0;
    } else if (__cat != LC_ALL && __cat != LC_CTYPE) {
        return 0;
    }
    if (__cat == LC_ALL || __cat == LC_CTYPE) {
        if (!__ccw_lc_set_cp((unsigned)__cp)) return 0;
    }
    return _ccw_locale_name();
}

static inline struct lconv *_ccw_localeconv(void)
{
    static struct lconv __lc;
    static int          __init = 0;
    if (!__init) {
        char *__e = (char *)"";
        __lc.decimal_point     = (char *)".";
        __lc.thousands_sep     = __e;
        __lc.int_curr_symbol   = __e;
        __lc.currency_symbol   = __e;
        __lc.mon_decimal_point = __e;
        __lc.mon_thousands_sep = __e;
        __lc.mon_grouping      = __e;
        __lc.grouping          = __e;
        __lc.positive_sign     = __e;
        __lc.negative_sign     = __e;
        __lc.int_frac_digits   = CHAR_MAX;
        __lc.frac_digits       = CHAR_MAX;
        __lc.p_cs_precedes     = CHAR_MAX;
        __lc.p_sep_by_space    = CHAR_MAX;
        __lc.n_cs_precedes     = CHAR_MAX;
        __lc.n_sep_by_space    = CHAR_MAX;
        __lc.p_sign_posn       = CHAR_MAX;
        __lc.n_sign_posn       = CHAR_MAX;
        __lc.int_p_cs_precedes  = CHAR_MAX;
        __lc.int_p_sep_by_space = CHAR_MAX;
        __lc.int_n_cs_precedes  = CHAR_MAX;
        __lc.int_n_sep_by_space = CHAR_MAX;
        __lc.int_p_sign_posn    = CHAR_MAX;
        __lc.int_n_sign_posn    = CHAR_MAX;
        __init = 1;
    }
    return &__lc;
}

#endif  /* _CCW_DETAIL_LOCALE_IMPL_H */
