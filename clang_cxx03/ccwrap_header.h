/**
 *  @file ccwrap_header.h
 *  @brief for msys2 clang++ /c++03
 *  @license Boost Software Lisence Version 1.0
 */
#ifndef __CCWRAP_HEADER_H
#define __CCWRAP_HEADER_H

#define __CCWRAP_HAS_INCLUDE_NEXT

#if !defined(__CCWRAP__)
#define __CCWRAP__  1
#endif

#define __CCWRAP_COMPILER                    "LLVM Clang Compiler"

#ifndef __CCWRAP_LDOUBLE_BIT
 #if defined _WIN64 || defined _M_AMD64 || defined __amd64__ || defined __AMD64__ || defined __x86_64__ || defined __arm64__ || defined __ARM64__ || defined _M_ARM64
  #define __CCWRAP_LDOUBLE_BIT       128
 #else
  #define __CCWRAP_LDOUBLE_BIT       96
 #endif
#endif

#if __cplusplus < 201101L
 #define __ccwrap_noreturn       __attribute__((noreturn))
#endif
#ifdef _WIN32
 #define __ccwrap_cdecl          __cdecl
 #define __ccwrap_stdcall        __stdcall
 #define __ccwrap_fastcall       __fastcall
#endif
#define __ccwrap_force_inline    __inline__ __attribute__((always_inline))
#define __ccwrap_noinline        __attribute__((noinline))
#define __ccwrap_selectany       __attribute__((weak))
#define __ccwrap_dllimport       __attribute__((dllimport))
#define __ccwrap_dllexport       __attribute__((dllexport))
#undef _MSC_VER
#ifdef __clang__
#undef __GUNUC__
#undef __MINGW__
#endif

#if defined(__cplusplus)
 #if __cplusplus < 201100
  typedef unsigned char             char8_t;
  typedef unsigned                  char32_t;
  typedef unsigned short            char16_t;
  #define __CCWRAP_NO_STATIC_CONST_INT_MEMBER_INITIALIZATION 1
  #ifndef nullptr
   #define nullptr                  0
  #endif
  #if !defined(alignas)
    #define alignas(a)              __attribute__((aligned(a)))
  #endif
  #if !defined(noexcept)
    #define noexcept                throw()
  #endif
  #if !defined(override)
    #define override
  #endif
  #if !defined(final)
    #define final
  #endif
  #if !defined(thread_local)
    #define thread_local
  #endif
  #if !defined(constexpr)
    #define constexpr
  #endif
  #if !defined(alignof)
    namespace __ccwrap_detail {
        template<class T> class __align_of {
            struct U { char a; T b; };
            enum { nt = sizeof(T), nu = sizeof(U), d = nu - nt };
        public:
            enum { value = (nt < d) ? nt : d };
        };
    }
    #define alignof(a)              (__ccwrap_detail::__align_of<a>::value)
  #endif
  #if !defined(static_assert)
    namespace __ccwrap { template<int x> struct static_assert_check{}; }
    template <bool x> struct __static_assert_FAILED_;
    template <> struct __static_assert_FAILED_<true> { enum { value = 1 }; };
    #define static_assert(c, ...) typedef __ccwrap::static_assert_check<sizeof(__static_assert_FAILED_<(c) != 0>)> __CCWRAP_M_CAT(__ccwrap_static_assert_L_, __LINE__)
  #endif

  #define __CCWRAP_NO_CXX11_AUTO    		1
  #define __CCWRAP_NO_DECLTYPE      		1
  #define __CCWRAP_HEADER_NO_TYPE_TRAITS	1
  #define __CCWRAP_NO_CXX11_ITERATOR		1
  #define __CCWRAP_NO_HEADER_CXX14_ITERATOR	1
 #endif
 #if !defined(nullptr)
  #define nullptr                   (0)
 #endif
#endif


#include "../ccwrap/ccwrap_header_sub.hh"
#endif
