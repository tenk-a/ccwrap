// Boost Software Lisence Version 1.0
#ifndef __CCWRAP_STDALIGN_H_INCLUDED
#define __CCWRAP_STDALIGN_H_INCLUDED

#include <ccwrap_common.h>

#ifndef __cplusplus

#if __STDC_VERSION__ >= 199901L
#if defined(__CCWRAP_HAS_INCLUDE_NEXT)
  #include_next <stdalign.h>
#elif defined(__CCWRAP_NATIVE_UC_HEADER_PATH)
  #include __CCWRAP_NATIVE_UC_HEADER_PATH(stdalign.h)
#elif defined(__CCWRAP_NATIVE_C_HEADER_PATH)
  #include __CCWRAP_NATIVE_C_HEADER_PATH(stdalign.h)
#else
  #include <../include/stdalign.h>
#endif

#else	// __STDC_VERSION__

#if defined(_Alignas) // || (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112)
#define alignas(a)              _Alignas(a)
#define __alignas_is_defined    1
#endif

#if defined(_Alignof) // || (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112)
#define alignof(t)              _Alignof(t)
#define __alignof_is_defined    1
#endif
#endif	// __STDC_VERSION__

#endif	// __cplusplus

#endif	//__CCWRAP_STDALIGN_H_INCLUDED
