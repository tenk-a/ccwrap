// ccwrap <assert.h>
#pragma once

#include <ccwrap_common.h>

#include _CCW_NATIVE_UC_HEADER_PATH(assert.h)

#if !defined(__cplusplus) && !defined(static_assert)
 #define static_assert       _Static_assert
#endif
