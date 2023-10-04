// Boost Software Lisence Version 1.0
#ifndef __BOOST2STD_HPP_INCLUDED
#define __BOOST2STD_HPP_INCLUDED

#if defined(_MSC_VER)
  #include "../boost2std/detail/vc/ccwrap_header.h"
#elif defined(__clang__) || defined(__GNUC__)   // msys-2
  #include "../boost2std/detail/mingw/ccwrap_header.h"
#else
  #error Unkown compiler.
#endif

#ifdef __cplusplus
namespace __ccwrap {}
namespace ccwrap {}
#endif

#endif  // __FAKE_STD_INCLUDE_HPP_INCLUDED
