// ccwrap <math.h>
#pragma once

#include <ccwrap_common.h>

#include _CCW_NATIVE_C_HEADER_PATH(math.h)

#include "../../detail/c/math_c99_fl.h"
#include "../../detail/c/math_nextafterf.h"  // bit-exact float nextafter (shared by C and C++)
#include "../../detail/c/math_errhandling.h" // C99-legal math_errhandling (native answers 4)

#ifdef __cplusplus

#include <cmath>
#ifndef _CCW_MATH_LROUND_USING
#define _CCW_MATH_LROUND_USING
using std::lround;  using std::lroundf;  using std::lroundl;
using std::llround; using std::llroundf; using std::llroundl;
#endif

#if __WATCOMC__ >= 1300
#ifndef _CCW_MATH_FL_USING
#define _CCW_MATH_FL_USING
#define _X(_N) using std::_N##f; using std::_N##l;
_CCW_MATH_FL1_LIST(_X)
_CCW_MATH_FL2_LIST(_X)
_CCW_MATH_FLX_FMA_LIST(_X)
_CCW_MATH_FLX_INTARG_LIST(_X)
_CCW_MATH_FLX_LONGARG_LIST(_X)
_CCW_MATH_FLX_INTPTR_LIST(_X)
_CCW_MATH_FLX_RETINT_LIST(_X)
_CCW_MATH_FLX_MODF_LIST(_X)
_CCW_MATH_FLX_REMQUO_LIST(_X)
_CCW_MATH_FLX_RETLLONG_LIST(_X)
#undef _X
using std::lrintf; using std::lrintl;
using std::scalbln;
using std::nextafterf; using std::nextafterl;
using std::nexttoward; using std::nexttowardf; using std::nexttowardl;
using std::float_t;    using std::double_t;
using std::isgreater;  using std::isgreaterequal;
using std::isless;     using std::islessequal;
using std::islessgreater; using std::isunordered;
using std::roundeven; using std::roundevenf; using std::roundevenl;
#define _X(_N) using std::_N; using std::_N##f; using std::_N##l;
_X(fmaximum) _X(fminimum) _X(fmaximum_num) _X(fminimum_num)
_X(fmaximum_mag) _X(fminimum_mag) _X(fmaximum_mag_num) _X(fminimum_mag_num)
#undef _X
using std::issignaling; using std::iscanonical;
#endif  /* _CCW_MATH_FL_USING */

#ifndef _CCW_MATH_FP_ABS_USING
#define _CCW_MATH_FP_ABS_USING
using std::abs;
#endif  /* _CCW_MATH_FP_ABS_USING */

#endif  /* __WATCOMC__ >= 1300 */
#if __WATCOMC__ < 1300
#ifndef _CCW_MATH_C99_USING
#define _CCW_MATH_C99_USING
using std::trunc; using std::round;  using std::nearbyint; using std::rint;
using std::truncf; using std::roundf; using std::fmin;      using std::fmax;
using std::copysign; using std::fma;  using std::scalbn;    using std::cbrt;
using std::exp2;  using std::exp2f;   using std::expm1;     using std::log1p;
#endif
#endif  /* __WATCOMC__ < 1300 */

#else  /* C */

#ifndef _CCW_MATH_LROUND_DEFINED
#define _CCW_MATH_LROUND_DEFINED
static _ccw_forceinline long      lround(double x)       { return (long)(x >= 0.0 ? x + 0.5 : x - 0.5); }
static _ccw_forceinline long      lroundf(float x)       { return lround((double)x); }
static _ccw_forceinline long      lroundl(long double x) { return lround((double)x); }
static _ccw_forceinline _ccw_llong llround(double x)       { return (_ccw_llong)(x >= 0.0 ? x + 0.5 : x - 0.5); }
static _ccw_forceinline _ccw_llong llroundf(float x)       { return llround((double)x); }
static _ccw_forceinline _ccw_llong llroundl(long double x) { return llround((double)x); }
#endif  /* _CCW_MATH_LROUND_DEFINED */

#if __WATCOMC__ >= 1300
#ifndef _CCW_MATH_FL_DEFINED
#define _CCW_MATH_FL_DEFINED
#define _X(_N) static _ccw_forceinline float _N##f(float x){return (float)_N((double)x);} \
               static _ccw_forceinline long double _N##l(long double x){return _N((double)x);}
_CCW_MATH_FL1_LIST(_X)
#undef _X
#define _X(_N) static _ccw_forceinline float _N##f(float x,float y){return (float)_N((double)x,(double)y);} \
               static _ccw_forceinline long double _N##l(long double x,long double y){return _N((double)x,(double)y);}
_CCW_MATH_FL2_LIST(_X)
#undef _X
#define _X(_N) static _ccw_forceinline float _N##f(float x,float y,float z){return (float)_N((double)x,(double)y,(double)z);} \
               static _ccw_forceinline long double _N##l(long double x,long double y,long double z){return _N((double)x,(double)y,(double)z);}
_CCW_MATH_FLX_FMA_LIST(_X)
#undef _X
#define _X(_N) static _ccw_forceinline float _N##f(float x,int e){return (float)_N((double)x,e);} \
               static _ccw_forceinline long double _N##l(long double x,int e){return _N((double)x,e);}
_CCW_MATH_FLX_INTARG_LIST(_X)
#undef _X
#define _X(_N) static _ccw_forceinline float _N##f(float x,long e){return (float)ldexp((double)x,(int)e);} \
               static _ccw_forceinline long double _N##l(long double x,long e){return ldexp((double)x,(int)e);}
_CCW_MATH_FLX_LONGARG_LIST(_X)
#undef _X
#define _X(_N) static _ccw_forceinline float _N##f(float x,int* e){return (float)_N((double)x,e);} \
               static _ccw_forceinline long double _N##l(long double x,int* e){return _N((double)x,e);}
_CCW_MATH_FLX_INTPTR_LIST(_X)
#undef _X
#define _X(_N) static _ccw_forceinline int _N##f(float x){return _N((double)x);} \
               static _ccw_forceinline int _N##l(long double x){return _N((double)x);}
_CCW_MATH_FLX_RETINT_LIST(_X)
#undef _X
#define _X(_N) static _ccw_forceinline float _N##f(float x,float* p){double __i; float __r=(float)_N((double)x,&__i); *p=(float)__i; return __r;} \
               static _ccw_forceinline long double _N##l(long double x,long double* p){double __i; double __r=_N((double)x,&__i); *p=__i; return __r;}
_CCW_MATH_FLX_MODF_LIST(_X)
#undef _X
#define _X(_N) static _ccw_forceinline float _N##f(float x,float y,int* q){return (float)_N((double)x,(double)y,q);} \
               static _ccw_forceinline long double _N##l(long double x,long double y,int* q){return _N((double)x,(double)y,q);}
_CCW_MATH_FLX_REMQUO_LIST(_X)
#undef _X
#define _X(_N) static _ccw_forceinline _ccw_llong _N##f(float x){return _N((double)x);} \
               static _ccw_forceinline _ccw_llong _N##l(long double x){return _N((double)x);}
_CCW_MATH_FLX_RETLLONG_LIST(_X)
#undef _X
static _ccw_forceinline long lrintf(float x)       { return (long)rint((double)x); }
static _ccw_forceinline long lrintl(long double x) { return (long)rint((double)x); }
static _ccw_forceinline double      scalbln(double x, long e)      { return ldexp(x, (int)e); }
static _ccw_forceinline float       nextafterf(float x, float y)             { return __ccw_nextafterf(x, y); }
static _ccw_forceinline long double nextafterl(long double x, long double y) { return nextafter((double)x, (double)y); }
static _ccw_forceinline double      nexttoward(double x, long double y)      { return nextafter(x, (double)y); }
static _ccw_forceinline float       nexttowardf(float x, long double y)      { return __ccw_nextafterf(x, (float)y); }
static _ccw_forceinline long double nexttowardl(long double x, long double y) { return nextafter((double)x, (double)y); }
#endif  /* _CCW_MATH_FL_DEFINED */
#endif  /* __WATCOMC__ >= 1300 */

#if __WATCOMC__ >= 1300
#include "../../detail/c/math_c23.h"
#endif

#ifndef _CCW_MATH_EVAL_TYPES_DEFINED
#define _CCW_MATH_EVAL_TYPES_DEFINED
#include <float.h>
#if FLT_EVAL_METHOD == 0
typedef float  float_t;
typedef double double_t;
#elif FLT_EVAL_METHOD == 1
typedef double float_t;
typedef double double_t;
#else   /* 2, or anything else -- evaluate in long double */
typedef long double float_t;
typedef long double double_t;
#endif
#endif  /* _CCW_MATH_EVAL_TYPES_DEFINED */

#ifndef _CCW_MATH_CMP_DEFINED
#define _CCW_MATH_CMP_DEFINED
#define isunordered(x, y)   (isnan(x) || isnan(y))
#define isgreater(x, y)     (!isunordered(x, y) && (x) >  (y))
#define isgreaterequal(x, y) (!isunordered(x, y) && (x) >= (y))
#define isless(x, y)        (!isunordered(x, y) && (x) <  (y))
#define islessequal(x, y)   (!isunordered(x, y) && (x) <= (y))
#define islessgreater(x, y) (!isunordered(x, y) && ((x) < (y) || (x) > (y)))
#endif  /* _CCW_MATH_CMP_DEFINED */

#if __WATCOMC__ < 1300
#ifndef _CCW_MATH_C99_DEFINED
#define _CCW_MATH_C99_DEFINED

static _ccw_forceinline double trunc(double x)     { return x < 0.0 ? ceil(x) : floor(x); }
static _ccw_forceinline double round(double x)     { return x < 0.0 ? ceil(x - 0.5) : floor(x + 0.5); }
static _ccw_forceinline double nearbyint(double x) { return round(x); }
static _ccw_forceinline double rint(double x)      { return round(x); }
static _ccw_forceinline float  truncf(float x)     { return (float)trunc((double)x); }
static _ccw_forceinline float  roundf(float x)     { return (float)round((double)x); }

static _ccw_forceinline double fmin(double a, double b) { return a < b ? a : b; }
static _ccw_forceinline double fmax(double a, double b) { return a > b ? a : b; }
static _ccw_forceinline double copysign(double x, double y) { return y < 0.0 ? -fabs(x) : fabs(x); }
static _ccw_forceinline double fma(double a, double b, double c) { return a * b + c; }
static _ccw_forceinline double scalbn(double x, int n) { return ldexp(x, n); }

static _ccw_forceinline double cbrt(double x)  { return x < 0.0 ? -pow(-x, 1.0 / 3.0) : pow(x, 1.0 / 3.0); }
static _ccw_forceinline double exp2(double x)  { return pow(2.0, x); }
static _ccw_forceinline float  exp2f(float x)  { return (float)pow(2.0, (double)x); }
static _ccw_forceinline double expm1(double x) { return exp(x) - 1.0; }
static _ccw_forceinline double log1p(double x) { return log(1.0 + x); }

#endif  /* _CCW_MATH_C99_DEFINED */
#endif  /* __WATCOMC__ < 1300 */

#endif  /* __cplusplus */
