/**
 *  @file ccwrap_header.h
 *  @brief for mingw c/c++
 *  @license Boost Software Lisence Version 1.0
 */
#ifndef __CCWRAP_HEADER_H
#define __CCWRAP_HEADER_H

#pragma once
#undef __STRICT_ANSI__
#undef NO_OLDNAMES
#undef _NO_OLDNAMES

#if !defined(__CCWRAP__)
#define __CCWRAP__  1
#endif

#define __CCWRAP_COMPILER    "GNU C/C++ Compiler (" __CCWRAP_M_STR(__GNUC__) "." __CCWRAP_M_STR(__GNUC_MINOR__) ")"

#define __CCWRAP_HAS_INCLUDE_NEXT

#define __ccwrap_restrict                    __restrict

#if __cplusplus < 201101L
 #define __ccwrap_noreturn                   __attribute__((noreturn))
#endif
#define __ccwrap_force_inline                __inline__ __attribute__((always_inline))
#define __ccwrap_noinline                    __attribute__((noinline))
#define __ccwrap_selectany                   __attribute__((weak))

#ifdef __MINGW32__
 #define __ccwrap_cdecl                      __cdecl
 #define __ccwrap_stdcall                    __stdcall
 #define __ccwrap_fastcall                   __fastcall
 #define __ccwrap_dllimport                  __attribute__((dllimport))
 #define __ccwrap_dllexport                  __attribute__((dllexport))
 #include <_mingw.h>
#else
 #define __ccwrap_cdecl
 #define __ccwrap_stdcall
 #define __ccwrap_fastcall
 #define __ccwrap_dllimport                  __attribute__((dllimport))
 #define __ccwrap_dllexport                  __attribute__((dllexport))
#endif

#define __CCWRAP_HAS_INCLUDE_NEXT

#ifndef __CCWRAP_LDOUBLE_BIT
#if defined _WIN64 || defined _M_AMD64 || defined __amd64__ || defined __AMD64__ || defined __x86_64__ || defined __arm64__ || defined __ARM64__ || defined _M_ARM64 || defined __MINGW64__
 #define __CCWRAP_LDOUBLE_BIT    128
#else
 #define __CCWRAP_LDOUBLE_BIT    96
#endif
#endif

#include "../ccwrap/ccwrap_header_sub.hh"

#endif
