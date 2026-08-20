// ccwrap <stddef.h>
// NO #pragma once here: GCC's <stddef.h> is meant to be re-included with different
// __need_* macros (libstdc++'s <cstddef> relies on that), so a once-guard would make
// the later includes no-ops and lose max_align_t.

#include <ccwrap_common.h>

#include_next <stddef.h>

#if !defined(_CCW_MAX_ALIGN_T_DEFINED) \
 && !defined(_GCC_MAX_ALIGN_T) && !defined(__CLANG_MAX_ALIGN_T_DEFINED) \
 && (!defined(__cplusplus) || __cplusplus < 201103L)
#include <../../detail/c/max_align_t.h>
#endif
