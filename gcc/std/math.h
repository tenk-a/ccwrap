// ccwrap <math.h>
#pragma once

#include <ccwrap_common.h>

#include_next <math.h>

#if defined(__GLIBC_USE_IEC_60559_BFP_EXT_C23) && __GLIBC_USE_IEC_60559_BFP_EXT_C23
# define _CCW_MATH_C23_NATIVE 1
#elif defined(__GLIBC_USE_IEC_60559_BFP_EXT) && __GLIBC_USE_IEC_60559_BFP_EXT
# define _CCW_MATH_C23_NATIVE 1
#endif

#if !defined(__cplusplus) && !defined(_CCW_MATH_C23_NATIVE)
# include "../../detail/c/math_c23.h"
#endif

#if defined(__cplusplus) && __cplusplus < 201103L
# include "../../detail/cxx/math_c99_ovl.hpp"
#endif
