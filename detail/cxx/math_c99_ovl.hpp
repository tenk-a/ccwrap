/**
 *  @file   detail/cxx/math_c99_ovl.hpp
 *  @brief  The C++ overloads for the C99 math functions, emitted into the enclosing namespace.
 *  @license Boost Software License Version 1.0
 */
#ifndef CCW_DETAIL_MATH_C99_OVL_HPP_INCLUDED__
#define CCW_DETAIL_MATH_C99_OVL_HPP_INCLUDED__

#ifdef __cplusplus

#include "../c/math_c99_fl.h"

#define _CCW_MO_FL1(_N)                                                              \
    inline float       _N(float __x)       { return (float)_N((double)__x); }        \
    inline long double _N(long double __x) { return (long double)_N((double)__x); }
#define _CCW_MO_FL2(_N)                                                              \
    inline float _N(float __x, float __y)                                            \
        { return (float)_N((double)__x, (double)__y); }                              \
    inline long double _N(long double __x, long double __y)                          \
        { return (long double)_N((double)__x, (double)__y); }
#define _CCW_MO_RET1(_N, _R)                                                         \
    inline _R _N(float __x)       { return _N((double)__x); }                        \
    inline _R _N(long double __x) { return _N((double)__x); }

#define _CCW_MO_INT1_ONE(_N, _R, _T) inline _R _N(_T __x) { return _N((double)__x); }
#define _CCW_MO_INT1(_N, _R)                                                         \
    _CCW_MO_INT1_ONE(_N, _R, int)          _CCW_MO_INT1_ONE(_N, _R, unsigned int)    \
    _CCW_MO_INT1_ONE(_N, _R, long)         _CCW_MO_INT1_ONE(_N, _R, unsigned long)   \
    _CCW_MO_INT1_ONE(_N, _R, _ccw_llong)   _CCW_MO_INT1_ONE(_N, _R, _ccw_ullong)
#define _CCW_MO_INT2_ONE(_N, _R, _T)                                                 \
    inline _R _N(_T __x, _T __y) { return _N((double)__x, (double)__y); }
#define _CCW_MO_INT2(_N, _R)                                                         \
    _CCW_MO_INT2_ONE(_N, _R, int)          _CCW_MO_INT2_ONE(_N, _R, unsigned int)    \
    _CCW_MO_INT2_ONE(_N, _R, long)         _CCW_MO_INT2_ONE(_N, _R, unsigned long)   \
    _CCW_MO_INT2_ONE(_N, _R, _ccw_llong)   _CCW_MO_INT2_ONE(_N, _R, _ccw_ullong)

_CCW_MATH_C99ONLY_FL1_LIST(_CCW_MO_FL1)
_CCW_MATH_C99ONLY_FL2_LIST(_CCW_MO_FL2)

_CCW_MO_RET1(ilogb,   int)
_CCW_MO_RET1(lrint,   long)
_CCW_MO_RET1(lround,  long)
_CCW_MO_RET1(llrint,  _ccw_llong)
_CCW_MO_RET1(llround, _ccw_llong)

inline float       scalbn(float __x, int __e)         { return (float)scalbn((double)__x, __e); }
inline long double scalbn(long double __x, int __e)   { return (long double)scalbn((double)__x, __e); }
inline float       scalbln(float __x, long __e)       { return (float)scalbln((double)__x, __e); }
inline long double scalbln(long double __x, long __e) { return (long double)scalbln((double)__x, __e); }
inline float       nextafter(float __x, float __y)             { return (float)nextafter((double)__x, (double)__y); }
inline long double nextafter(long double __x, long double __y) { return (long double)nextafter((double)__x, (double)__y); }
inline float       nexttoward(float __x, long double __y)       { return (float)nextafter((double)__x, (double)__y); }
inline long double nexttoward(long double __x, long double __y) { return (long double)nextafter((double)__x, (double)__y); }
inline float       fma(float __x, float __y, float __z) { return (float)fma((double)__x, (double)__y, (double)__z); }
inline long double fma(long double __x, long double __y, long double __z)
    { return (long double)fma((double)__x, (double)__y, (double)__z); }

#define _CCW_MO_INT1_D(_N) _CCW_MO_INT1(_N, double)
#define _CCW_MO_INT2_D(_N) _CCW_MO_INT2(_N, double)
_CCW_MATH_C99ONLY_FL1_LIST(_CCW_MO_INT1_D)
_CCW_MATH_C99ONLY_FL2_LIST(_CCW_MO_INT2_D)
#undef _CCW_MO_INT1_D
#undef _CCW_MO_INT2_D

_CCW_MO_INT1(ilogb,   int)
_CCW_MO_INT1(lrint,   long)
_CCW_MO_INT1(lround,  long)
_CCW_MO_INT1(llrint,  _ccw_llong)
_CCW_MO_INT1(llround, _ccw_llong)
_CCW_MO_INT2(nextafter, double)

#undef _CCW_MO_FL1
#undef _CCW_MO_FL2
#undef _CCW_MO_RET1
#undef _CCW_MO_INT1
#undef _CCW_MO_INT1_ONE
#undef _CCW_MO_INT2
#undef _CCW_MO_INT2_ONE

#endif  /* __cplusplus */
#endif  /* CCW_DETAIL_MATH_C99_OVL_HPP_INCLUDED__ */
