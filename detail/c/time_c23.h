/*
 * @file    time_c23.h
 * @brief   ccwrap detail: the C23 addition to <time.h> that MSVC lacks (timegm).
 * @license Boost Software License Version 1.0
 */
#ifndef CCW_DETAIL_TIME_C23_H_INCLUDED__
#define CCW_DETAIL_TIME_C23_H_INCLUDED__

#if defined(_MSC_VER) && !defined(_CCW_TIMEGM_DEFINED)
#define _CCW_TIMEGM_DEFINED

#if defined(__cplusplus)
#  define _CCW_T23_FN  inline
#else
#  define _CCW_T23_FN  static _ccw_forceinline
#endif

_CCW_T23_FN time_t timegm(struct tm* __t) {
#if defined(_USE_32BIT_TIME_T)
    return (time_t)_mkgmtime32(__t);
#else
    return (time_t)_mkgmtime64(__t);
#endif
}

#undef _CCW_T23_FN

#endif  /* _MSC_VER && !_CCW_TIMEGM_DEFINED */

#endif /* CCW_DETAIL_TIME_C23_H_INCLUDED__ */
