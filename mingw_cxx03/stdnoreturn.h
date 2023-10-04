/**
 *  @file   stdnoreturn.h
 *  @brief  _Noreturn   (B.22)
 */
#ifndef __CCWRAP_STDNORETURN_H_INCLUDED
#define __CCWRAP_STDNORETURN_H_INCLUDED

#include <ccwrap_header.h>

#if 0

#include_next <stdnoreturn.h>

#else

#ifndef noreturn
 //#define _Noreturn            __attribute__((noreturn))
 #define noreturn               __attribute__((noreturn))
 #define __noreturn_is_defined  1
#endif

#endif

#endif
