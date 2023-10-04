#ifndef __CCWRAP_CONFIG_INCLUDE_H_INCLUDED
#define __CCWRAP_CONFIG_INCLUDE_H_INCLUDED

//#define __CCWRAP_ID_NONE       0
//#define __CCWRAP_ID_FAKE_STD   1
//#define __CCWRAP_ID_CCWRAP_NS  2
//#define __CCWRAP_ID_BOOST2STD  3

#if defined(_MSC_VER)
  #if _MSC_VER < 1900
    #include "../vc2013_or_earlier/ccwrap_header.h"
  #else
    #include "../vc2015_or_later/ccwrap_header.h"
  #endif
#elif defined(__clang__) || defined(__GNUC__)   // msys-2
  #if __cplusplus >= 201101
    #include "../mingw_cxx11_or_later/ccwrap_header.h"
  #else
    #include "../mingw_cxx03/ccwrap_header.h"
  #endif
//#elif defined(__MINGW32__) || defined(__MINGW64__)
//  #include "mingw/ccwrap_header.h"
#else
  #error Unkown compiler.
#endif

#ifdef __cplusplus
namespace __ccwrap {}
namespace ccwrap {}
#endif

#endif  // __FAKE_STD_INCLUDE_H_INCLUDED
