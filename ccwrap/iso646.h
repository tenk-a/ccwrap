// Boost Software Lisence Version 1.0
#ifdef	__CCWRAP_ISO646_H_INCLUDED
#define __CCWRAP_ISO646_H_INCLUDED

#include <ccwrap_common.h>

#if !defined(__CCWRAP_NO_ISO646)

#if defined(__CCWRAP_HAS_INCLUDE_NEXT)
  #include_next <iso646.h>
#elif defined(__CCWRAP_NATIVE_UC_HEADER_PATH)
  #include __CCWRAP_NATIVE_UC_HEADER_PATH(iso646.h)
#elif defined(__CCWRAP_NATIVE_C_HEADER_PATH)
  #include __CCWRAP_NATIVE_C_HEADER_PATH(iso646.h)
#else
  #include <../include/iso646.h>
#endif

#else

#if !defined(__cplusplus) || defined(__CCWRAP_NO_ISO646)

#define and 	    &&
#define and_eq	    &=
#define bitand	    &
#define bitor	    |
#define compl	    ~
#define not 	    !
#define not_eq	    !=
#define or  	    ||
#define or_eq	    |=
#define xor 	    ^
#define xor_eq	    ^=

#endif

#endif

#endif	// __CCWRAP_ISO646_H_INCLUDED
