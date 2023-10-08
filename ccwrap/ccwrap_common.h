// Boost Software Lisence Version 1.0
#ifndef __CCWRAP_COMMON_H_INCLUDED
#define __CCWRAP_COMMON_H_INCLUDED

#if defined(__CCWRAP__)

#elif defined(_MSC_VER)
 #include <../vc/ccwrap_header.h>
#elif defined(__GNUC__) || defined(__clang__)
 #include <../gcc/ccwrap_header.h>
#else
 #error Unkown compiler.
#endif

#if 1 // stddef.h and cstddef

#if (__CCWRAP_CXX && __CCWRAP_CXX < 201103L) || (__CCWRAP_C_VER && __CCWRAP_C_VER < 201100L)
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
 #ifdef __cplusplus
  namespace std {
		using ::max_align_t;
  }
 #endif
#endif

#if __cplusplus
 #if defined(__CCWRAP_HAS_INCLUDE_NEXT)
  #include_next <cstddef>
 #elif defined(__CCWRAP_NATIVE_C_HEADER_PATH)
  #include __CCWRAP_NATIVE_C_HEADER_PATH(cstddef)
 #else
  #include <../include/cstddef>
 #endif
#endif

#if defined(__CCWRAP_HAS_INCLUDE_NEXT)
  #include_next <stddef.h>
#elif defined(__CCWRAP_NATIVE_UC_HEADER_PATH)
  #include __CCWRAP_NATIVE_UC_HEADER_PATH(stddef.h)
#elif defined(__CCWRAP_NATIVE_C_HEADER_PATH)
  #include __CCWRAP_NATIVE_C_HEADER_PATH(stddef.h)
#else
  #include <../include/stddef.h>
#endif


#endif	//

#endif	// __CCWRAP_COMMON_H_INCLUDED
