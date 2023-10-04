// Boost Software Lisence Version 1.0
#ifndef __CCWRAP_ERRNO_H_INCLUDED
#define __CCWRAP_ERRNO_H_INCLUDED

#include <boost2std_common.hpp>

#if defined(__CCWRAP_HAS_INCLUDE_NEXT)
  #include_next <errno.h>
#elif defined(__CCWRAP_NATIVE_UC_HEADER_PATH)
 #include __CCWRAP_NATIVE_UC_HEADER_PATH(errno.h)
#elif defined(__CCWRAP_NATIVE_C_HEADER_PATH)
  #include __CCWRAP_NATIVE_C_HEADER_PATH(errno.h)
#else
  #include <../include/errno.h>
#endif

#ifndef errno
#define errno	errno
#endif

#endif
