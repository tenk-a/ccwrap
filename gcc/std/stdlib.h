// ccwrap <stdlib.h>
#pragma once

#include <ccwrap_common.h>

#include_next <stdlib.h>

#if defined(__MINGW32__) && !defined(_UCRT) && \
    (_CCW_TARGET_C >= 2011 || _CCW_TARGET_CXX >= 2011)
# include <../../detail/c/stdlib_c11.h>
#endif

#if defined(__MINGW32__)
# define _CCW_NO_ALIGNED_ALLOC_C11          1
# define _CCW_NO_FREE_SIZED_C23             1
# define _CCW_NO_FREE_ALIGNED_SIZED_C23     1
# define _CCW_NO_STRFROM_C11                1
#endif // __MINGW32__
