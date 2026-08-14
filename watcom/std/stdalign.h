// ccwrap <stdalign.h>
#pragma once

#include <ccwrap_common.h>

#ifndef __cplusplus
 #ifndef alignas
  #define alignas(a)   _Alignas(a)   /* no-op on Open Watcom */
 #endif
 #ifndef alignof
  #include <stddef.h>
  #define alignof(type)  ((size_t)&(((struct { char __ccw_c; type __ccw_x; }*)0)->__ccw_x))
 #endif
#endif

#define __alignas_is_defined  1
#define __alignof_is_defined  1
