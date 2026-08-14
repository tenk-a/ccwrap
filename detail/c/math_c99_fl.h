/**
 *  @file   detail/math_c99_fl.h
 *  @brief  Name LISTS of the C99 float(f) / long double(l) suffixed math functions.
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_DETAIL_MATH_C99_FL_H___
#define _CCW_DETAIL_MATH_C99_FL_H___

#define _CCW_MATH_FL1_LIST(_X)                                                  \
    _X(acos)  _X(asin)  _X(atan)  _X(cos)   _X(sin)   _X(tan)                   \
    _X(acosh) _X(asinh) _X(atanh) _X(cosh)  _X(sinh)  _X(tanh)                  \
    _X(exp)   _X(exp2)  _X(expm1) _X(log)   _X(log10) _X(log1p) _X(log2)        \
    _X(logb)                                                                    \
    _X(sqrt)  _X(cbrt)  _X(fabs)  _X(ceil)  _X(floor) _X(trunc) _X(round)       \
    _X(nearbyint) _X(rint)                                                      \
    _X(erf)   _X(erfc)  _X(tgamma) _X(lgamma)

#define _CCW_MATH_FL2_LIST(_X)                                                  \
    _X(atan2) _X(pow)   _X(fmod)  _X(hypot) _X(copysign)                        \
    _X(fdim)  _X(fmax)  _X(fmin)  _X(remainder)

#define _CCW_MATH_C99ONLY_FL1_LIST(_X)                                          \
    _X(acosh) _X(asinh) _X(atanh) _X(cbrt)  _X(exp2)  _X(expm1)                 \
    _X(log1p) _X(log2)  _X(logb)  _X(trunc) _X(round) _X(nearbyint)             \
    _X(rint)  _X(erf)   _X(erfc)  _X(tgamma) _X(lgamma)

#define _CCW_MATH_C99ONLY_FL2_LIST(_X)                                          \
    _X(hypot) _X(copysign) _X(fdim) _X(fmax) _X(fmin) _X(remainder)

#define _CCW_MATH_FLX_FMA_LIST(_X)     _X(fma)
#define _CCW_MATH_FLX_INTARG_LIST(_X)  _X(ldexp) _X(scalbn)
#define _CCW_MATH_FLX_LONGARG_LIST(_X) _X(scalbln)
#define _CCW_MATH_FLX_INTPTR_LIST(_X)  _X(frexp)
#define _CCW_MATH_FLX_RETINT_LIST(_X)  _X(ilogb)

#define _CCW_MATH_FLX_MODF_LIST(_X)     _X(modf)
#define _CCW_MATH_FLX_REMQUO_LIST(_X)   _X(remquo)
#define _CCW_MATH_FLX_RETLLONG_LIST(_X) _X(llrint)
#define _CCW_MATH_FLX_NEXTTWD_LIST(_X)  _X(nexttoward)

#ifndef HUGE_VALF
#define HUGE_VALF ((float)HUGE_VAL)
#endif
#ifndef HUGE_VALL
#define HUGE_VALL ((long double)HUGE_VAL)
#endif

#endif /* _CCW_DETAIL_MATH_C99_FL_H___ */
