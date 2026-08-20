// ccwrap <float.h>
#pragma once

#include <ccwrap_common.h>

#include_next <float.h>

#if !defined(FLT_DECIMAL_DIG) && defined(__FLT_DECIMAL_DIG__)
# define FLT_DECIMAL_DIG   __FLT_DECIMAL_DIG__
#endif
#if !defined(DBL_DECIMAL_DIG) && defined(__DBL_DECIMAL_DIG__)
# define DBL_DECIMAL_DIG   __DBL_DECIMAL_DIG__
#endif
#if !defined(LDBL_DECIMAL_DIG) && defined(__LDBL_DECIMAL_DIG__)
# define LDBL_DECIMAL_DIG  __LDBL_DECIMAL_DIG__
#endif
#if !defined(DECIMAL_DIG) && defined(__DECIMAL_DIG__)
# define DECIMAL_DIG       __DECIMAL_DIG__
#endif

#if !defined(FLT_HAS_SUBNORM) && defined(__FLT_HAS_DENORM__)
# define FLT_HAS_SUBNORM   __FLT_HAS_DENORM__
#endif
#if !defined(DBL_HAS_SUBNORM) && defined(__DBL_HAS_DENORM__)
# define DBL_HAS_SUBNORM   __DBL_HAS_DENORM__
#endif
#if !defined(LDBL_HAS_SUBNORM) && defined(__LDBL_HAS_DENORM__)
# define LDBL_HAS_SUBNORM  __LDBL_HAS_DENORM__
#endif

#if !defined(FLT_TRUE_MIN) && defined(__FLT_DENORM_MIN__)
# define FLT_TRUE_MIN      __FLT_DENORM_MIN__
#endif
#if !defined(DBL_TRUE_MIN) && defined(__DBL_DENORM_MIN__)
# define DBL_TRUE_MIN      __DBL_DENORM_MIN__
#endif
#if !defined(LDBL_TRUE_MIN) && defined(__LDBL_DENORM_MIN__)
# define LDBL_TRUE_MIN     __LDBL_DENORM_MIN__
#endif

#if !defined(FLT_EVAL_METHOD) && defined(__FLT_EVAL_METHOD__)
# define FLT_EVAL_METHOD   __FLT_EVAL_METHOD__
#endif

#if !defined(FLT_NORM_MAX) && defined(__FLT_NORM_MAX__)
# define FLT_NORM_MAX      __FLT_NORM_MAX__
#endif
#if !defined(DBL_NORM_MAX) && defined(__DBL_NORM_MAX__)
# define DBL_NORM_MAX      __DBL_NORM_MAX__
#endif
#if !defined(LDBL_NORM_MAX) && defined(__LDBL_NORM_MAX__)
# define LDBL_NORM_MAX     __LDBL_NORM_MAX__
#endif
