// ccwrap <stdnoreturn.h>
#pragma once

#include <ccwrap_common.h>

#if !defined(__cplusplus)
 #if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
  #ifndef noreturn
   #define noreturn  _Noreturn
  #endif
 #endif
#endif
