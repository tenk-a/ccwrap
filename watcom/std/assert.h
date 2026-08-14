// ccwrap <assert.h>
#pragma once

#include <ccwrap_common.h>

#ifndef assert
#include _CCW_NATIVE_C_HEADER_PATH(assert.h)
#endif

#if !defined(__cplusplus) && !defined(static_assert)
 #define static_assert  _Static_assert
#endif
