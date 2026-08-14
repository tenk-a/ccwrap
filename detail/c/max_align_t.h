/**
 *  @file   detail/max_align_t.h
 *  @brief  max_align_t (C11 <stddef.h> / C++11 <cstddef>)
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_DETAIL_MAX_ALIGN_T_H
#define _CCW_DETAIL_MAX_ALIGN_T_H

#ifndef _CCW_MAX_ALIGN_T_DEFINED
#define _CCW_MAX_ALIGN_T_DEFINED
typedef union { long double __ld; _ccw_llong __ll; void* __p; } max_align_t;
#endif

#endif  /* _CCW_DETAIL_MAX_ALIGN_T_H */
