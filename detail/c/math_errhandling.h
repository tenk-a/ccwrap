/**
 *  @file   detail/math_errhandling.h
 *  @brief  C99 math_errhandling for Open Watcom -- a value a conforming program can read.
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_MATH_ERRHANDLING_H_INCLUDED
#define _CCW_MATH_ERRHANDLING_H_INCLUDED

#include <ccwrap_common.h>

#if defined(__WATCOMC__) && defined(math_errhandling)
#undef math_errhandling
#define math_errhandling 0
#endif

#endif  /* _CCW_MATH_ERRHANDLING_H_INCLUDED */
