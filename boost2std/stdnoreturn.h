// Boost Software Lisence Version 1.0
#ifndef __CCWRAP_STDNORETURN_H_INCLUDED
#define __CCWRAP_STDNORETURN_H_INCLUDED

#include <boost2std_common.hpp>

#if __STDC_VERSION__ >= 199900L

#if defined(__CCWRAP_HAS_INCLUDE_NEXT)
  #include_next <stdnoreturn.h>
#elif defined(__CCWRAP_NATIVE_C_HEADER_PATH)
  #include __CCWRAP_NATIVE_C_HEADER_PATH(stdnoreturn.h)
#else
  #include <../include/stdnoreturn.h>
#endif

#else	// __STDC_VERSION__

#ifndef __cplusplus

#if defined(_MSC_VER)
 #define noreturn   	    	__declspec(noreturn)
 #define __noreturn_is_defined	1
 #undef _Noreturn   	    	// use _Noreturn -> __declspec(noreturn)->__declspec(__declspec(noreturn))
#elif defined(_Noreturn)
#define noreturn    	    	_Noreturn
#define __noreturn_is_defined	1
#endif

#endif	// __cplusplus

#endif	// __STDC_VERSION__

#endif	//__CCWRAP_STDNORETURN_H_INCLUDED
