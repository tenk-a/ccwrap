// ccwrap <float.h>
#pragma once

#include <ccwrap_common.h>

#include _CCW_NATIVE_UC_HEADER_PATH(float.h)

#if _MSC_VER < 1900
 #ifndef DECIMAL_DIG
  #define DECIMAL_DIG        17
 #elif defined(_CCW_C_CONFORM)
  #if DECIMAL_DIG < 17
   #undef  DECIMAL_DIG
   #define DECIMAL_DIG       17
  #endif
 #endif
 #ifndef FLT_EVAL_METHOD
  #if _CCW_CPU_BIT >= 64
   #define FLT_EVAL_METHOD   0
  #else
   #define FLT_EVAL_METHOD   2
  #endif
 #endif
 #ifndef FLT_DECIMAL_DIG
  #define FLT_DECIMAL_DIG    9
  #define DBL_DECIMAL_DIG    17
  #define LDBL_DECIMAL_DIG   17
 #endif
 #ifndef FLT_HAS_SUBNORM
  #define FLT_HAS_SUBNORM    1
  #define DBL_HAS_SUBNORM    1
  #define LDBL_HAS_SUBNORM   1
 #endif
 #ifndef FLT_TRUE_MIN
  #define FLT_TRUE_MIN       1.401298464324817e-45F
  #define DBL_TRUE_MIN       4.9406564584124654e-324
  #define LDBL_TRUE_MIN      4.9406564584124654e-324
 #endif
#endif

#if defined(DBL_DECIMAL_DIG) && !defined(LDBL_DECIMAL_DIG)
 #define LDBL_DECIMAL_DIG   DBL_DECIMAL_DIG
#endif

#ifndef FLT_NORM_MAX
 #define FLT_NORM_MAX   FLT_MAX
 #define DBL_NORM_MAX   DBL_MAX
 #define LDBL_NORM_MAX  LDBL_MAX
#endif
#ifndef FLT_IS_IEC_60559
 #define FLT_IS_IEC_60559   1
 #define DBL_IS_IEC_60559   1
 #define LDBL_IS_IEC_60559  1
#endif
#ifndef FLT_IS_IEC_60559_SUBNORMAL
 #define FLT_IS_IEC_60559_SUBNORMAL   1
 #define DBL_IS_IEC_60559_SUBNORMAL   1
 #define LDBL_IS_IEC_60559_SUBNORMAL  1
#endif
#ifndef __STDC_VERSION_FLOAT_H__
 #define __STDC_VERSION_FLOAT_H__  202311L
#endif
