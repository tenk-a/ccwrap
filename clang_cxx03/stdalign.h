/**
 *  @file   stdalign.h
 *  @brief  Alignment
 */
#ifndef __CCWRAP_STDALIGN_H_INCLUDED
#define __CCWRAP_STDALIGN_H_INCLUDED

#include <ccwrap_header.h>

#if 0

#include_next <stdalign.h>

#else

#if !defined(alignas) // || (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112)
#define alignas(a)              _Alignas(a)
#define __alignas_is_defined    1
#endif

#if !defined(alignof) // || (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112)
#define alignof(t)              _Alignof(t)
#define __alignof_is_defined    1

#endif

#endif

#endif
