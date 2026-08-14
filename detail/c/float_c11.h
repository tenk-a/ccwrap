/**
 *  @file   detail/float_c11.h
 *  @brief  The C11 / C23 additions to <float.h>.
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_DETAIL_FLOAT_C11_H___
#define _CCW_DETAIL_FLOAT_C11_H___

#ifndef __STDC_VERSION_FLOAT_H__
#define __STDC_VERSION_FLOAT_H__ 202311L
#endif

#ifndef FLT_DECIMAL_DIG
#define FLT_DECIMAL_DIG   9
#endif
#ifndef DECIMAL_DIG
#define DECIMAL_DIG       17
#endif

#ifndef DBL_DECIMAL_DIG
#define DBL_DECIMAL_DIG   17
#endif
#ifndef LDBL_DECIMAL_DIG
#define LDBL_DECIMAL_DIG  DBL_DECIMAL_DIG
#endif

#if !defined(FLT_TRUE_MIN) || !defined(DBL_TRUE_MIN) || !defined(LDBL_TRUE_MIN)

#if defined(__cplusplus)
#  define _CCW_TM_FN  inline
#else
#  define _CCW_TM_FN  static _ccw_forceinline
#endif

_CCW_TM_FN float __ccw_flt_true_min(void) {
    union { unsigned long __u; float __f; } __v;
    __v.__u = 1UL;
    return __v.__f;
}
_CCW_TM_FN double __ccw_dbl_true_min(void) {
    union { unsigned char __b[8]; double __d; } __v;
    int __i;
    for (__i = 0; __i < 8; ++__i) __v.__b[__i] = 0;
    __v.__b[0] = 1;                     /* little-endian (x86) */
    return __v.__d;
}

#ifndef FLT_TRUE_MIN
#define FLT_TRUE_MIN   (__ccw_flt_true_min())
#endif
#ifndef DBL_TRUE_MIN
#define DBL_TRUE_MIN   (__ccw_dbl_true_min())
#endif
#ifndef LDBL_TRUE_MIN
#define LDBL_TRUE_MIN  ((long double)__ccw_dbl_true_min())
#endif

#undef _CCW_TM_FN

#endif  /* *_TRUE_MIN */

#ifndef FLT_HAS_SUBNORM
#define FLT_HAS_SUBNORM   1
#endif
#ifndef DBL_HAS_SUBNORM
#define DBL_HAS_SUBNORM   1
#endif
#ifndef LDBL_HAS_SUBNORM
#define LDBL_HAS_SUBNORM  1
#endif

#ifndef FLT_NORM_MAX
#define FLT_NORM_MAX   FLT_MAX
#endif
#ifndef DBL_NORM_MAX
#define DBL_NORM_MAX   DBL_MAX
#endif
#ifndef LDBL_NORM_MAX
#define LDBL_NORM_MAX  LDBL_MAX
#endif

#ifndef FLT_EVAL_METHOD
#define FLT_EVAL_METHOD  2
#endif

#ifndef FLT_IS_IEC_60559
#define FLT_IS_IEC_60559  1
#endif
#ifndef DBL_IS_IEC_60559
#define DBL_IS_IEC_60559  1
#endif
#ifndef LDBL_IS_IEC_60559
#define LDBL_IS_IEC_60559 1
#endif
#ifndef FLT_IS_IEC_60559_SUBNORMAL
#define FLT_IS_IEC_60559_SUBNORMAL  1
#endif
#ifndef DBL_IS_IEC_60559_SUBNORMAL
#define DBL_IS_IEC_60559_SUBNORMAL  1
#endif
#ifndef LDBL_IS_IEC_60559_SUBNORMAL
#define LDBL_IS_IEC_60559_SUBNORMAL 1
#endif

#endif /* _CCW_DETAIL_FLOAT_C11_H___ */
