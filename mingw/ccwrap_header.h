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

#define __CCWRAP__

//#ifdef __clang__
//#define __CCWRAP_HAS_PRAGMA_ONCE      1
//#endif
#define __CCWRAP_HAS_INCLUDE_NEXT

#if defined _WIN32
#define __CCWRAP_LONG_BIT           32
#endif
#if defined _WIN64 || defined _M_AMD64 || defined __amd64__ || defined __AMD64__ || defined __x86_64__ || defined __arm64__ || defined __ARM64__ || defined _M_ARM64
#define __CCWRAP_NATIVE_INT_BIT     64
#else
#define __CCWRAP_NATIVE_INT_BIT     32
#endif

#endif
