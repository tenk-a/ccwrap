// ccwrap <iso646.h>
#pragma once

#include <ccwrap_common.h>

#if !defined(__cplusplus) || defined(_MSC_VER)
 #define and      &&
 #define and_eq   &=
 #define bitand   &
 #define bitor    |
 #define compl    ~
 #define not      !
 #define not_eq   !=
 #define or       ||
 #define or_eq    |=
 #define xor      ^
 #define xor_eq   ^=
#endif
