// ccwrap <stdbit.h>
#pragma once

#include <ccwrap_common.h>

#ifdef __cplusplus

# ifndef __STDC_VERSION_STDBIT_H__
#  define __STDC_VERSION_STDBIT_H__ 202311L
# endif
# ifndef __STDC_ENDIAN_LITTLE__
#  define __STDC_ENDIAN_LITTLE__ 1234
#  define __STDC_ENDIAN_BIG__    4321
#  if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#   define __STDC_ENDIAN_NATIVE__ __STDC_ENDIAN_BIG__
#  else
#   define __STDC_ENDIAN_NATIVE__ __STDC_ENDIAN_LITTLE__
#  endif
# endif

# include "../../detail/cxx/stdbit.hpp"

#else

# if defined(__has_include_next)
#  if __has_include_next(<stdbit.h>)
#   define _CCW_HAS_NATIVE_STDBIT_H 1
#  endif
# endif

# if defined(_CCW_HAS_NATIVE_STDBIT_H)
#  include_next <stdbit.h>
# else
#  include "../../detail/c/stdbit_c.h"
# endif

#endif // __cplusplus
