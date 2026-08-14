/**
 *  @file   detail/cxx/math_int_overloads.hpp
 *  @brief  The integral-argument overloads <cmath> is required to provide.
 *  @license Boost Software License Version 1.0
 */
#ifndef CCW_DETAIL_MATH_INT_OVERLOADS_HPP_INCLUDED__
#define CCW_DETAIL_MATH_INT_OVERLOADS_HPP_INCLUDED__

#ifdef __cplusplus

#define _CCW_MI_UNARY_ONE(fn, R, T) \
    inline R fn(T __x) { return fn((double)__x); }
#define _CCW_MI_UNARY(fn, R)                                          \
    _CCW_MI_UNARY_ONE(fn, R, int)                                     \
    _CCW_MI_UNARY_ONE(fn, R, unsigned int)                            \
    _CCW_MI_UNARY_ONE(fn, R, long)                                    \
    _CCW_MI_UNARY_ONE(fn, R, unsigned long)                           \
    _CCW_MI_UNARY_ONE(fn, R, _ccw_llong)                              \
    _CCW_MI_UNARY_ONE(fn, R, _ccw_ullong)

#define _CCW_MI_BINARY_ONE(fn, R, T) \
    inline R fn(T __x, T __y) { return fn((double)__x, (double)__y); }
#define _CCW_MI_BINARY(fn, R)                                         \
    _CCW_MI_BINARY_ONE(fn, R, int)                                    \
    _CCW_MI_BINARY_ONE(fn, R, unsigned int)                           \
    _CCW_MI_BINARY_ONE(fn, R, long)                                   \
    _CCW_MI_BINARY_ONE(fn, R, unsigned long)                          \
    _CCW_MI_BINARY_ONE(fn, R, _ccw_llong)                             \
    _CCW_MI_BINARY_ONE(fn, R, _ccw_ullong)

namespace std {

_CCW_MI_UNARY(acos,  double)
_CCW_MI_UNARY(asin,  double)
_CCW_MI_UNARY(atan,  double)
_CCW_MI_UNARY(ceil,  double)
_CCW_MI_UNARY(cos,   double)
_CCW_MI_UNARY(cosh,  double)
_CCW_MI_UNARY(exp,   double)
_CCW_MI_UNARY(fabs,  double)
_CCW_MI_UNARY(floor, double)
_CCW_MI_UNARY(log,   double)
_CCW_MI_UNARY(log10, double)
_CCW_MI_UNARY(sin,   double)
_CCW_MI_UNARY(sinh,  double)
_CCW_MI_UNARY(sqrt,  double)
_CCW_MI_UNARY(tan,   double)
_CCW_MI_UNARY(tanh,  double)

_CCW_MI_BINARY(atan2, double)
_CCW_MI_BINARY(fmod,  double)
_CCW_MI_BINARY(pow,   double)

_CCW_MI_UNARY(acosh,     double)
_CCW_MI_UNARY(asinh,     double)
_CCW_MI_UNARY(atanh,     double)
_CCW_MI_UNARY(cbrt,      double)
_CCW_MI_UNARY(erf,       double)
_CCW_MI_UNARY(erfc,      double)
_CCW_MI_UNARY(exp2,      double)
_CCW_MI_UNARY(expm1,     double)
_CCW_MI_UNARY(lgamma,    double)
_CCW_MI_UNARY(log1p,     double)
_CCW_MI_UNARY(log2,      double)
_CCW_MI_UNARY(logb,      double)
_CCW_MI_UNARY(nearbyint, double)
_CCW_MI_UNARY(rint,      double)
_CCW_MI_UNARY(round,     double)
_CCW_MI_UNARY(tgamma,    double)
_CCW_MI_UNARY(trunc,     double)

_CCW_MI_UNARY(ilogb,  int)
_CCW_MI_UNARY(lrint,  long)
_CCW_MI_UNARY(lround, long)
_CCW_MI_UNARY(llrint,  _ccw_llong)
_CCW_MI_UNARY(llround, _ccw_llong)

_CCW_MI_BINARY(copysign,  double)
_CCW_MI_BINARY(fdim,      double)
_CCW_MI_BINARY(fmax,      double)
_CCW_MI_BINARY(fmin,      double)
_CCW_MI_BINARY(hypot,     double)
_CCW_MI_BINARY(nextafter, double)
_CCW_MI_BINARY(remainder, double)

}  // namespace std

#undef _CCW_MI_UNARY
#undef _CCW_MI_UNARY_ONE
#undef _CCW_MI_BINARY
#undef _CCW_MI_BINARY_ONE

#endif  /* __cplusplus */
#endif  /* CCW_DETAIL_MATH_INT_OVERLOADS_HPP_INCLUDED__ */
