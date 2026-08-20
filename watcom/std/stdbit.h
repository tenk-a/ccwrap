// ccwrap watcom: <stdbit.h> (C23 / C++26) -- the bit-manipulation utilities.
//
// C and C++ both get the full set. C23 spells the type-generic names with _Generic,
// which Open Watcom has not; they dispatch on sizeof(value) through a ternary chain
// instead (plain C89, same as the other targets).
#pragma once

#include <ccwrap_common.h>

#ifdef __cplusplus

#include _CCW_NATIVE_STL_HEADER_PATH(stdbit.h)

#else   /* C */

#include "../../detail/c/stdbit_c.h"

#endif  /* __cplusplus */
