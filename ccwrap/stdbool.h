// Boost Software Lisence Version 1.0
#ifndef __CCWRAP_STDBOOL_H_INCLUDED
#define __CCWRAP_STDBOOL_H_INCLUDED

#include <ccwrap_common.h>

#if __STDC_VERSION__ >= 199901L

#if defined(__CCWRAP_HAS_INCLUDE_NEXT)
  #include_next <stdbool.h>
#elif defined(__CCWRAP_NATIVE_UC_HEADER_PATH)
  #include __CCWRAP_NATIVE_UC_HEADER_PATH(stdbool.h)
#elif defined(__CCWRAP_NATIVE_C_HEADER_PATH)
  #include __CCWRAP_NATIVE_C_HEADER_PATH(stdbool.h)
#else
  #include <../include/stdbool.h>
#endif

#else	// __STDC_VERSION__

#ifndef __cplusplus

#if defined(_Bool)
 #define bool	    	    	_Bool
 #define true	    	    	1
 #define false	    	    	0
 #define __bool_is_defined  	1
#else
 #error not impliment stdbool.h
#endif

#endif	// __cplusplus

#endif	// __STDC_VERSION__

#endif	//__CCWRAP_STDBOOL_H_INCLUDED
