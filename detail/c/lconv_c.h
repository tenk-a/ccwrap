/**
 *  @file   detail/c/lconv_c.h
 *  @brief  A conforming localeconv() for a CRT that fills struct lconv with SCHAR_MAX.
 *  @license Boost Software License Version 1.0
 *  @note
 *
 *  Include it at global scope from <locale.h> and inside namespace std from
 *  <clocale>; the caller defines localeconv to _ccw_localeconv afterwards.
 */
#ifndef _CCW_DETAIL_LCONV_C_H
#define _CCW_DETAIL_LCONV_C_H

#include <limits.h>

#ifdef __cplusplus
# define _CCW_LCONV_FN  inline
#else
# define _CCW_LCONV_FN  static _ccw_forceinline
#endif

#if CHAR_MAX != SCHAR_MAX
# define _CCW_LCONV_NA(__m) do { if ((__m) == SCHAR_MAX) (__m) = CHAR_MAX; } while (0)
#else
# define _CCW_LCONV_NA(__m) ((void)0)
#endif

_CCW_LCONV_FN struct lconv* _ccw_localeconv(void) {
    static struct lconv __ccw_lc;
    struct lconv* __p = localeconv();
    if (__p == 0) return __p;
    __ccw_lc = *__p;
    _CCW_LCONV_NA(__ccw_lc.int_frac_digits);
    _CCW_LCONV_NA(__ccw_lc.frac_digits);
    _CCW_LCONV_NA(__ccw_lc.p_cs_precedes);
    _CCW_LCONV_NA(__ccw_lc.p_sep_by_space);
    _CCW_LCONV_NA(__ccw_lc.n_cs_precedes);
    _CCW_LCONV_NA(__ccw_lc.n_sep_by_space);
    _CCW_LCONV_NA(__ccw_lc.p_sign_posn);
    _CCW_LCONV_NA(__ccw_lc.n_sign_posn);
    return &__ccw_lc;
}

#endif /* _CCW_DETAIL_LCONV_C_H */
