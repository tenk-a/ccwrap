#pragma once

#include <ccwrap_header.h>
#include __CCWRAP_NATIVE_UC_HEADER_PATH(stddef.h)

#if defined __cplusplus
 #include <cstddef>
#else
 #ifndef __CCWRAP_MAX_ALIGN_T_DEFINED
  #define __CCWRAP_MAX_ALIGN_T_DEFINED
  typedef double  max_align_t;
 #endif
#endif
