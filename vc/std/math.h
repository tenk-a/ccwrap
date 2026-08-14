// ccwrap <math.h>
#pragma once

#include <ccwrap_common.h>

#include _CCW_NATIVE_UC_HEADER_PATH(math.h)

#ifndef math_errhandling
 #define MATH_ERRNO         1
 #define MATH_ERREXCEPT     2
 #define math_errhandling   MATH_ERRNO
#endif
#ifndef FP_ILOGB0
 #define FP_ILOGB0          (-2147483647 - 1)
 #define FP_ILOGBNAN        2147483647
#endif

#if _MSC_VER < 1800
#include <float.h>

#ifndef INFINITY
 #define INFINITY           ((float)(HUGE_VAL))
#endif
#ifndef HUGE_VALF
 #define HUGE_VALF          ((float)(HUGE_VAL))
#endif
#ifndef HUGE_VALL
 #define HUGE_VALL          ((long double)(HUGE_VAL))
#endif
#ifndef NAN
 #define NAN                ((float)(HUGE_VAL * 0.0))
#endif
#ifndef FP_NAN
 #define FP_NAN             0
 #define FP_INFINITE        1
 #define FP_ZERO            2
 #define FP_SUBNORMAL       3
 #define FP_NORMAL          4
#endif

static __forceinline int __ccw_fpclassify(double x) {
    switch (_fpclass(x)) {
    case _FPCLASS_SNAN: case _FPCLASS_QNAN: return FP_NAN;
    case _FPCLASS_NINF: case _FPCLASS_PINF: return FP_INFINITE;
    case _FPCLASS_NZ:   case _FPCLASS_PZ:   return FP_ZERO;
    case _FPCLASS_ND:   case _FPCLASS_PD:   return FP_SUBNORMAL;
    default:                                return FP_NORMAL;
    }
}
static __forceinline int __ccw_fpclassify_f(float x) {
    union { float __f; unsigned __u; } __v;
    unsigned __e, __m;
    __v.__f = x;
    __e = (__v.__u >> 23) & 0xFFu;
    __m = __v.__u & 0x7FFFFFu;
    if (__e == 0xFFu) return __m ? FP_NAN : FP_INFINITE;
    if (__e == 0u)    return __m ? FP_SUBNORMAL : FP_ZERO;
    return FP_NORMAL;
}
#if defined(__cplusplus)
static __forceinline int  fpclassify(float x)              { return __ccw_fpclassify_f(x); }
static __forceinline bool isnormal(float x)                { return __ccw_fpclassify_f(x) == FP_NORMAL; }
static __forceinline bool isinf(float x)                   { return __ccw_fpclassify_f(x) == FP_INFINITE; }
static __forceinline bool isfinite(float x)                { int __c = __ccw_fpclassify_f(x); return __c != FP_NAN && __c != FP_INFINITE; }
static __forceinline int  fpclassify(long double x)        { return __ccw_fpclassify((double)x); }
static __forceinline bool isnormal(long double x)          { return __ccw_fpclassify((double)x) == FP_NORMAL; }
static __forceinline bool isinf(long double x)             { return __ccw_fpclassify((double)x) == FP_INFINITE; }
static __forceinline bool isfinite(long double x)          { return _finite((double)x) != 0; }
static __forceinline int  fpclassify(double x)             { return __ccw_fpclassify(x); }
static __forceinline bool isnan(double x)                  { return _isnan(x) != 0; }
static __forceinline bool isinf(double x)                  { return __ccw_fpclassify(x) == FP_INFINITE; }
static __forceinline bool isfinite(double x)               { return _finite(x) != 0; }
static __forceinline bool isnormal(double x)               { return __ccw_fpclassify(x) == FP_NORMAL; }
static __forceinline bool signbit(double x)                { return _copysign(1.0, x) < 0.0; }
static __forceinline bool isunordered(double x, double y)  { return isnan(x) || isnan(y); }
static __forceinline bool isgreater(double x, double y)    { return !isunordered(x, y) && x >  y; }
static __forceinline bool isgreaterequal(double x,double y){ return !isunordered(x, y) && x >= y; }
static __forceinline bool isless(double x, double y)       { return !isunordered(x, y) && x <  y; }
static __forceinline bool islessequal(double x, double y)  { return !isunordered(x, y) && x <= y; }
static __forceinline bool islessgreater(double x,double y) { return !isunordered(x, y) && x != y; }
#else
#define fpclassify(x)       __ccw_fpclassify((double)(x))
#define isnan(x)            (_isnan((double)(x)) != 0)
#define isinf(x)            (__ccw_fpclassify((double)(x)) == FP_INFINITE)
#define isfinite(x)         (_finite((double)(x)) != 0)
#define isnormal(x)         (__ccw_fpclassify((double)(x)) == FP_NORMAL)
#define signbit(x)          (_copysign(1.0, (double)(x)) < 0.0)
#define isunordered(x, y)   (isnan(x) || isnan(y))
#define isgreater(x, y)     (!isunordered(x, y) && ((x) >  (y)))
#define isgreaterequal(x,y) (!isunordered(x, y) && ((x) >= (y)))
#define isless(x, y)        (!isunordered(x, y) && ((x) <  (y)))
#define islessequal(x, y)   (!isunordered(x, y) && ((x) <= (y)))
#define islessgreater(x, y) (!isunordered(x, y) && ((x) != (y)))
#endif

#define _CCW_MATH_FL1(fn) \
    static __forceinline float       fn##f(float x)       { return (float)fn((double)x); } \
    static __forceinline long double fn##l(long double x) { return       fn((double)x); }
#define _CCW_MATH_FL2(fn) \
    static __forceinline float       fn##f(float x, float y)             { return (float)fn((double)x, (double)y); } \
    static __forceinline long double fn##l(long double x, long double y) { return       fn((double)x, (double)y); }
#define _CCW_MATH_IL1(fn, rty) \
    static __forceinline rty fn##f(float x)       { return fn((double)x); } \
    static __forceinline rty fn##l(long double x) { return fn((double)x); }

_CCW_F_INL(FUNC, double, round, (double x), (x)) {
    return (x >= 0.0) ? floor(x + 0.5) : ceil(x - 0.5);
}
_CCW_F_INL(FUNC, double, trunc, (double x), (x)) {
    return (x >= 0.0) ? floor(x) : ceil(x);
}
_CCW_F_INL(FUNC, double, rint, (double x), (x)) {
    double m = 4503599627370496.0;  /* 2^52 */
    if (!_finite(x) || fabs(x) >= m)
        return x;
    return (x >= 0.0) ? (x + m) - m : (x - m) + m;  /* nearest, ties to even */
}
static __forceinline double     nearbyint(double x) { return rint(x); }
static __forceinline long       lround(double x)    { return (long)round(x); }
static __forceinline _ccw_llong llround(double x)   { return (_ccw_llong)round(x); }
static __forceinline long       lrint(double x)     { return (long)rint(x); }
static __forceinline _ccw_llong llrint(double x)    { return (_ccw_llong)rint(x); }
_CCW_MATH_FL1(round)
_CCW_MATH_FL1(trunc)
_CCW_MATH_FL1(rint)
_CCW_MATH_FL1(nearbyint)
_CCW_MATH_IL1(lround, long)
_CCW_MATH_IL1(llround, _ccw_llong)
_CCW_MATH_IL1(lrint, long)
_CCW_MATH_IL1(llrint, _ccw_llong)

static __forceinline double fdim(double x, double y) { return (x > y) ? (x - y) : 0.0; }
_CCW_F_INL(FUNC, double, fmin, (double x, double y), (x, y)) {
    if (_isnan(x)) return y;
    if (_isnan(y)) return x;
    return (x < y) ? x : y;
}
_CCW_F_INL(FUNC, double, fmax, (double x, double y), (x, y)) {
    if (_isnan(x)) return y;
    if (_isnan(y)) return x;
    return (x > y) ? x : y;
}
static __forceinline double fma(double x, double y, double z) { return x * y + z; }  /* not fused */
static __forceinline double copysign(double x, double y)      { return _copysign(x, y); }
_CCW_MATH_FL2(fdim)
_CCW_MATH_FL2(fmin)
_CCW_MATH_FL2(fmax)
_CCW_MATH_FL2(copysign)
static __forceinline float       fmaf(float x, float y, float z)                   { return (float)((double)x * (double)y + (double)z); }
static __forceinline long double fmal(long double x, long double y, long double z) { return x * y + z; }

static __forceinline double cbrt(double x)  { return (x < 0.0) ? -pow(-x, 1.0 / 3.0) : pow(x, 1.0 / 3.0); }
static __forceinline double exp2(double x)  { return pow(2.0, x); }
_CCW_F_INL(FUNC, double, log2, (double x), (x)) {
    int e;
    double m;
    if (!(x > 0.0) || !_finite(x)) return log(x) * 1.4426950408889634;
    m = frexp(x, &e);
    if (m == 0.5) return (double)(e - 1);
    return (double)e + log(m) * 1.4426950408889634;
}
static __forceinline double expm1(double x) {
    double u = exp(x);
    if (u == 1.0) return x;                  /* exp rounded to 1: x IS the answer */
    if (u - 1.0 == -1.0) return -1.0;        /* underflowed to zero */
    return (u - 1.0) * x / log(u);
}
static __forceinline double log1p(double x) {
    double u = 1.0 + x;
    if (u == 1.0) return x;                  /* 1+x rounded to 1 */
    return log(u) * x / (u - 1.0);
}
_CCW_MATH_FL1(cbrt)
_CCW_MATH_FL1(exp2)
_CCW_MATH_FL1(log2)
_CCW_MATH_FL1(expm1)
_CCW_MATH_FL1(log1p)

static __forceinline double scalbn(double x, int n)   { return _scalb(x, (long)n); }
static __forceinline double scalbln(double x, long n) { return _scalb(x, n); }
static __forceinline double logb(double x)            { return _logb(x); }
_CCW_F_INL(FUNC, int, ilogb, (double x), (x)) {
    if (x == 0.0)                 return FP_ILOGB0;
    if (_isnan(x) || !_finite(x)) return FP_ILOGBNAN;
    return (int)_logb(x);
}
static __forceinline float       scalbnf(float x, int n)         { return (float)_scalb((double)x, (long)n); }
static __forceinline long double scalbnl(long double x, int n)   { return _scalb((double)x, (long)n); }
static __forceinline float       scalblnf(float x, long n)       { return (float)_scalb((double)x, n); }
static __forceinline long double scalblnl(long double x, long n) { return _scalb((double)x, n); }
_CCW_MATH_FL1(logb)
static __forceinline int ilogbf(float x)       { return ilogb((double)x); }
static __forceinline int ilogbl(long double x) { return ilogb((double)x); }

static __forceinline double nextafter(double x, double y)       { return _nextafter(x, y); }
static __forceinline double nexttoward(double x, long double y) { return _nextafter(x, (double)y); }
_CCW_F_INL(FUNC, double, remainder, (double x, double y), (x, y)) {
    return x - rint(x / y) * y;
}
_CCW_F_INL(FUNC, double, remquo, (double x, double y, int *quo), (x, y, quo)) {
    double n = rint(x / y);
    int qi = (int)n;
    *quo = qi < 0 ? -((-qi) & 7) : (qi & 7);
    return x - n * y;
}
static __forceinline float nextafterf(float x, float y) {
    union { float __f; unsigned __u; } __v;
    if (x == y) return y;
    if (x != x || y != y) return x + y;              /* NaN in, NaN out */
    if (x == 0.0f) { __v.__u = 1; return y > 0.0f ? __v.__f : -__v.__f; }
    __v.__f = x;
    if ((y > x) == (x > 0.0f)) ++__v.__u; else --__v.__u;
    return __v.__f;
}
static __forceinline long double nextafterl(long double x, long double y) { return nextafter((double)x, (double)y); }
static __forceinline float       nexttowardf(float x, long double y)       { return nextafterf(x, (float)y); }
static __forceinline long double nexttowardl(long double x, long double y) { return _nextafter((double)x, (double)y); }
_CCW_MATH_FL2(remainder)
static __forceinline float       remquof(float x, float y, int *q)             { return (float)remquo((double)x, (double)y, q); }
static __forceinline long double remquol(long double x, long double y, int *q) { return remquo((double)x, (double)y, q); }

static __forceinline double acosh(double x) { return log(x + sqrt(x * x - 1.0)); }
static __forceinline double asinh(double x) { return log(x + sqrt(x * x + 1.0)); }
static __forceinline double atanh(double x) { return 0.5 * log((1.0 + x) / (1.0 - x)); }
_CCW_MATH_FL1(acosh)
_CCW_MATH_FL1(asinh)
_CCW_MATH_FL1(atanh)

#include <../../detail/c/math_c99_gamma.h>
_CCW_MATH_FL1(erf)
_CCW_MATH_FL1(erfc)
_CCW_MATH_FL1(tgamma)
_CCW_MATH_FL1(lgamma)

#ifndef _CCW_MATH_EVAL_TYPES
#define _CCW_MATH_EVAL_TYPES
#if defined(FLT_EVAL_METHOD) && FLT_EVAL_METHOD == 1
 typedef double      float_t;
 typedef double      double_t;
#elif defined(FLT_EVAL_METHOD) && FLT_EVAL_METHOD == 2
 typedef long double float_t;
 typedef long double double_t;
#else
 typedef float       float_t;
 typedef double      double_t;
#endif
#endif

/* VS2010/VS2012 have hypotf inline but spell hypotl as a function-like macro, which
   would expand in the definition below; VS2013 has both for real (vc bugs D3). */
#if _MSC_VER < 1600
static __forceinline float       hypotf(float x, float y)             { return (float)_hypot((double)x, (double)y); }
#endif
#if _MSC_VER < 1800
#ifdef hypotl
#undef hypotl
#endif
static __forceinline long double hypotl(long double x, long double y) { return _hypot((double)x, (double)y); }
#endif

static __forceinline double      nan(const char *tag)  { (void)tag; return (double)NAN; }
static __forceinline float       nanf(const char *tag) { (void)tag; return (float)NAN; }
static __forceinline long double nanl(const char *tag) { (void)tag; return (long double)NAN; }

#ifdef modff
#undef modff
#define _CCW_MATH_NEED_MODFF 1
#endif
#ifdef modfl
#undef modfl
#define _CCW_MATH_NEED_MODFL 1
#endif

#ifdef _CCW_MATH_NEED_MODFF
static __forceinline float modff(float x, float *iptr) {
    double __i;
    float  __r = (float)modf((double)x, &__i);
    *iptr = (float)__i;
    return __r;
}
#endif  // _CCW_MATH_NEED_MODFF

#ifdef _CCW_MATH_NEED_MODFL
static __forceinline long double modfl(long double x, long double *iptr) {
    double __i;
    double __r = modf((double)x, &__i);
    *iptr = (long double)__i;
    return (long double)__r;
}
#endif  // _CCW_MATH_NEED_MODFL

#undef _CCW_MATH_FL1
#undef _CCW_MATH_FL2
#undef _CCW_MATH_IL1
#endif

#ifndef __cplusplus
#if _CCW_TARGET_C >= 2023
#include <../../detail/c/math_c23.h>
#endif
#endif
