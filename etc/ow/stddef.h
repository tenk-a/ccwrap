#pragma once

#include <ccwrap_header.h>
#include <_comdef.h>

#include __CCWRAP_NATIVE_C_HEADER_PATH(stddef.h)

#ifndef __CCWRAP_MAX_ALIGN_T
#define __CCWRAP_MAX_ALIGN_T
typedef union __ccwrap_max_align_t {
    void*       _m_vp;
    long long   _m_ll;
    double      _m_d;
    long double _m_ld;
} max_align_t;
#endif
