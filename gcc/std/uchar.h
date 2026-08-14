// ccwrap <uchar.h>
#pragma once

#include <ccwrap_common.h>

#if defined(__cplusplus)

# include_next <uchar.h>

#else

# include <stddef.h>    /* size_t */
# include <stdint.h>    /* uint_least16_t / uint_least32_t */

# ifndef _CCW_CHAR8_16_32_DEFINED
#  define _CCW_CHAR8_16_32_DEFINED
   typedef unsigned char   char8_t;
   typedef uint_least16_t  char16_t;
   typedef uint_least32_t  char32_t;
#  ifndef __CCW_HAS_CHAR8_T
#   define __CCW_HAS_CHAR8_T 1
#  endif
# endif

# include "../../detail/c/uchar_impl.h"

#endif // __cplusplus
