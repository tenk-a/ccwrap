// Boost Software Lisence Version 1.0
#ifndef __CCWRAP_STDINT_H_INCLUDED
#define __CCWRAP_STDINT_H_INCLUDED

#include <boost2std_common.hpp>

#if !defined(__CCWRAP_NO_HEADER_CSTDINT)

 #if defined(__CCWRAP_HAS_INCLUDE_NEXT)
  #include_next <stdint.h>
 #elif defined(__CCWRAP_NATIVE_UC_HEADER_PATH)
  #include __CCWRAP_NATIVE_UC_HEADER_PATH(stdint.h)
 #elif defined(__CCWRAP_NATIVE_C_HEADER_PATH)
  #include __CCWRAP_NATIVE_C_HEADER_PATH(stdint.h)
 #else
  #include <../include/stdint.h>
 #endif

#else // defined(__CCWRAP_NO_HEADER_CSTDINT)

#include "../boost2std/cstdint"

#if 1
using boost::int8_t;
using boost::uint8_t;
using boost::int16_t;
using boost::uint16_t;
using boost::int32_t;
using boost::uint32_t;
using boost::int64_t;
using boost::uint64_t;

using boost::intmax_t;
using boost::uintmax_t;

using boost::intptr_t;
using boost::uintptr_t;

using boost::int_least8_t;
using boost::uint_least8_t;
using boost::int_least16_t;
using boost::uint_least16_t;
using boost::int_least32_t;
using boost::uint_least32_t;
using boost::int_least64_t;
using boost::uint_least64_t;

using boost::int_fast8_t;
using boost::uint_fast8_t;
using boost::int_fast16_t;
using boost::uint_fast16_t;
using boost::int_fast32_t;
using boost::uint_fast32_t;
using boost::int_fast64_t;
using boost::uint_fast64_t;
#endif

#endif  // __CCWRAP_NO_HEADER_CSTDINT

#endif  // __CCWRAP_STDINT_H_INCLUDED
