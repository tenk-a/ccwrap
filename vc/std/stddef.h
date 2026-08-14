// ccwrap <stddef.h>
#pragma once

#include <ccwrap_common.h>

#include _CCW_NATIVE_UC_HEADER_PATH(stddef.h)

#ifndef __cplusplus
#include <../../detail/c/max_align_t.h>
#endif

#if !defined(__cplusplus) && _CCW_TARGET_C >= 2023
 #ifndef unreachable
  #define unreachable()      __assume(0)
 #endif
#endif
