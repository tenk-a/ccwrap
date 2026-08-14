// ccwrap <stdalign.h>
#pragma once

#include <ccwrap_common.h>

#ifndef __cplusplus
 #ifndef alignas
  #define alignas  _Alignas
 #endif
 #ifndef alignof
  #define alignof  _Alignof
 #endif
#endif

#define __alignas_is_defined  1
#define __alignof_is_defined  1
