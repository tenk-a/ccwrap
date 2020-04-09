#ifndef __CCWRAP_STDDEF_H_INCLUDED
#define __CCWRAP_STDDEF_H_INCLUDED

#include <ccwrap_header.h>

#include_next <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#if __cplusplus < 201100
#ifndef __CCWRAP_MAX_ALIGN_T
#define __CCWRAP_MAX_ALIGN_T
typedef union max_align_t {
    void*       _m_vp;
    long long   _m_ll;
    double      _m_d;
    long double _m_ld;
} max_align_t;
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif
