// ccwrap <stdbool.h>
#pragma once

#include <ccwrap_common.h>

#ifndef __cplusplus
 #ifndef bool
  #define bool     _Bool
 #endif
 #ifndef true
  #define true     1
 #endif
 #ifndef false
  #define false    0
 #endif
#endif

#define __bool_true_false_are_defined  1
