/**
 *  @file ccwrap_header.h
 *  @brief  for orange c/c++
 *  @note
 */
#ifndef __CCWRAP_HEADER_H
#define __CCWRAP_HEADER_H

#if !defined(__ORANGEC__)
#error this header is for orange c/c++
#endif

#define __CCWRAP__

#ifndef __CCWRAP_M_CAT
  #define __CCWRAP_M_CAT(a,b)               __CCWRAP_M_CAT_S2(a,b)
  #define __CCWRAP_M_CAT_S2(a,b)            a##b
#endif

#ifndef __CCWRAP_NATIVE_C_HEADER_DIR
   #define __CCWRAP_NATIVE_C_HEADER_DIR     ../include
#endif

#ifndef __CCWRAP_NATIVE_C_HEADER_PATH
  #define __CCWRAP_NATIVE_C_HEADER_PATH(x)  <__CCWRAP_NATIVE_C_HEADER_DIR/##x>
#endif

#define __NO_WCTYPE

#define __CCWRAP_NO_HEADER_CFENV                1
#define __CCWRAP_NO_HEADER_ATOMIC               1
#define __CCWRAP_NO_HEADER_THREAD               1
#define __CCWRAP_NO_HEADER_MUTEX                1
#define __CCWRAP_NO_HEADER_SHARED_MUTEX         1
#define __CCWRAP_NO_HEADER_CONDITION_VARIABLE   1
#define __CCWRAP_NO_HEADER_FUTURE               1
//#define __CCWRAP_NO_HEADER_CODECVT            1

#define __CCWRAP_LONG_BIT           32
#if defined _WIN64 || defined _M_AMD64 || defined __amd64__ || defined __AMD64__ || defined __x86_64__ || defined __arm64__ || defined __ARM64__ || defined _M_ARM64
#define __CCWRAP_NATIVE_INT_BIT     64
#else
#define __CCWRAP_NATIVE_INT_BIT     32
#endif

#endif  // IMPLICIT_INCLUDE_HEADER_H
