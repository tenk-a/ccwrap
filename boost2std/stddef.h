// Boost Software Lisence Version 1.0
#ifndef __CCWRAP_STDDEF_H_INCLUDED
#define __CCWRAP_STDDEF_H_INCLUDED

#include <boost2std.hpp>

#if defined(__CCWRAP_HAS_INCLUDE_NEXT)
  #include_next <stddef.h>
#elif defined(__CCWRAP_NATIVE_UC_HEADER_PATH)
 #include __CCWRAP_NATIVE_UC_HEADER_PATH(stddef.h)
#elif defined(__CCWRAP_NATIVE_C_HEADER_PATH)
  #include __CCWRAP_NATIVE_C_HEADER_PATH(stddef.h)
#else
  #include <../include/stddef.h>
#endif

#if (__STDC_VERSION__ < 199900L && !defined(__cplusplus)) || (__CCWRAP_CXX && __CCWRAP_CXX < 201103L)

 #ifndef __CCWRAP_MAX_ALIGN_T_DEFINED
  #define __CCWRAP_MAX_ALIGN_T_DEFINED
   #ifdef _MSC_VER
    typedef double  max_align_t;
   #else
	typedef union max_align_t {
	    void*       	_m_vp;
	    __ccwrap_llong	_m_ll;
	    double      	_m_d;
		//long double 	_m_ld;
	} max_align_t;
   #endif
 #endif

#endif

#endif
