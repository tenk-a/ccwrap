// Boost Software Lisence Version 1.0
#ifndef __CCWRAP_MATH_H_INCLUDED
#define __CCWRAP_MATH_H_INCLUDED

#include <boost2std_common.hpp>

#if defined(__CCWRAP_HAS_INCLUDE_NEXT)
  #include_next <math.h>
  #include_next <float.h>
#elif defined(__CCWRAP_NATIVE_UC_HEADER_PATH)
 #include __CCWRAP_NATIVE_UC_HEADER_PATH(math.h)
 #include __CCWRAP_NATIVE_UC_HEADER_PATH(float.h)
#elif defined(__CCWRAP_NATIVE_C_HEADER_PATH)
  #include __CCWRAP_NATIVE_C_HEADER_PATH(math.h)
  #include __CCWRAP_NATIVE_C_HEADER_PATH(float.h)
#else
  #include <../include/math.h>
  #include <../include/float.h>
#endif


#if _MSC_VER && _MSC_VER < 1800

#define FP_ZERO         _FPCLASS_PZ
#define FP_NAN          _FPCLASS_QNAN
#define FP_INFINITE     _FPCLASS_PINF
#define FP_NORMAL       _FPCLASS_PN
#define FP_SUBNORMAL    _FPCLASS_PD

static inline int __ccwrap_fpclassify_conv(int n) {
    if (n > 0) {
        if (n <= _FPCLASS_QNAN) return FP_NAN;
        if (n == _FPCLASS_PZ || n == _FPCLASS_NZ) return FP_ZERO;
        if (n == _FPCLASS_PINF || n == _FPCLASS_NINF) return FP_INFINITE;
        if (n == _FPCLASS_PN || n == _FPCLASS_NN) return FP_NORMAL;
        if (n == _FPCLASS_PD || n == _FPCLASS_ND) return FP_SUBNORMAL;
    }
    return 0;
}

#ifdef __cplusplus
extern "C" {
__ccwrap_forceinline int  fpclassify(double v) { return __ccwrap_fpclassify_conv(_fpclass(v)); }
__ccwrap_forceinline bool isnan(double v) { return _isnan(v) != 0; }
__ccwrap_forceinline bool isfinite(double x) { return _finite(x) != 0; }
__ccwrap_forceinline bool isinf(double v) { int c = _fpclass(v); return c == _FPCLASS_PINF || c == _FPCLASS_NINF; }
__ccwrap_forceinline bool isnormal(double v) { int c = _fpclass(v); return c == _FPCLASS_PN || c == _FPCLASS_NN; }
__ccwrap_forceinline bool signbit(double v) { int c = _fpclass(v); return _FPCLASS_NINF <= c && c <= _FPCLASS_NZ; }
__ccwrap_forceinline bool isunordered(double x, double y) { return isnan(x) || isnan(y); }
__ccwrap_forceinline bool isgreater(double x, double y) { return isunordered(x,y) ? false : x > y; }
__ccwrap_forceinline bool isgreaterequal(double x, double y) { return isunordered(x,y) ? false : x >= y; }
__ccwrap_forceinline bool isless(double x, double y) { return isunordered(x,y) ? false : x < y; }
__ccwrap_forceinline bool islessequal(double x, double y) { return isunordered(x,y) ? false : x <= y; }
}
#endif

#ifdef __cplusplus

__ccwrap_forceinline int  fpclassify(long double v) { return fpclassify((double)v); }
__ccwrap_forceinline bool isnan(long double v) { return _isnan((double)v) != 0; }
__ccwrap_forceinline bool isfinite(long double x) { return _finite((double)x) != 0; }
__ccwrap_forceinline bool isinf(long double v) { return isinf((double)v); }
__ccwrap_forceinline bool isnormal(long double v) { return isnormal((double)v); }
__ccwrap_forceinline bool signbit(long double v) { return signbit((double)v); }
__ccwrap_forceinline bool isunordered(long double x, long double y) { return isnan(x) || isnan(y); }
__ccwrap_forceinline bool isgreater(long double x, long double y) { return isunordered(x,y) ? false : x > y; }
__ccwrap_forceinline bool isgreaterequal(long double x, long double y) { return isunordered(x,y) ? false : x >= y; }
__ccwrap_forceinline bool isless(long double x, long double y) { return isunordered(x,y) ? false : x < y; }
__ccwrap_forceinline bool islessequal(long double x, long double y) { return isunordered(x,y) ? false : x <= y; }

#ifdef _M_X64
__ccwrap_forceinline int  fpclassify(float v) { return __ccwrap_fpclassify_conv(_fpclassf(v)); }
__ccwrap_forceinline bool isnan(float v) { return _isnanf(v) != 0; }
__ccwrap_forceinline bool isfinite(float x) { return _finitef(x) != 0; }
#else
__ccwrap_forceinline int  _fpclassf(float v) { return _fpclass(v); }
__ccwrap_forceinline int  fpclassify(float v) { return __ccwrap_fpclassify_conv(_fpclassf(v)); }
__ccwrap_forceinline bool isnan(float v) { int c = _fpclassf(v); return c == _FPCLASS_QNAN || c == _FPCLASS_SNAN; }
__ccwrap_forceinline bool isfinite(float v) { int c = _fpclassf(v); return _FPCLASS_NN <= c &&  c <= _FPCLASS_PN; }
#endif
__ccwrap_forceinline bool isinf(float v) { int c = _fpclassf(v); return c == _FPCLASS_PINF || c == _FPCLASS_NINF; }
__ccwrap_forceinline bool isnormal(float v) { int c = _fpclassf(v); return c == _FPCLASS_PN || c == _FPCLASS_NN; }
__ccwrap_forceinline bool signbit(float v) { int c = _fpclassf(v); return _FPCLASS_NINF <= c && c <= _FPCLASS_NZ; }
__ccwrap_forceinline bool isunordered(float x, float y) { return isnan(x) || isnan(y); }
__ccwrap_forceinline bool isgreater(float x, float y) { return isunordered(x,y) ? false : x > y; }
__ccwrap_forceinline bool isgreaterequal(float x, float y) { return isunordered(x,y) ? false : x >= y; }
__ccwrap_forceinline bool isless(float x, float y) { return isunordered(x,y) ? false : x < y; }
__ccwrap_forceinline bool islessequal(float x, float y) { return isunordered(x,y) ? false : x <= y; }

#if 0
inline double __ccwrap_double_compare(double x, double y) {
    if (isnan(x)) {
        if (isnan(y))
            return 0;
        return -1;
    } else if (isnan(y)) {
        return 1;
    } else if (isinf(x)) {
        if (isinf(y))
            return 0;
        return 1;
    } else {
        return x - y;
    }
}
#endif

#include <limits>
#define HUGE_VALL   (std::numeric_limits<double>::infinity())
#define HUGE_VALF   (std::numeric_limits<float>::infinity())
#define INFINITY    (std::numeric_limits<float>::infinity())
#define NAN         (std::numeric_limits<float>::quiet_NaN())

#else   //TODO:
static __ccwrap_forceinline int fpclassify(double v) { return __ccwrap_fpclassify_conv(_fpclass(v)); }
static __ccwrap_forceinline int isnan(double v) { return _isnan(v) != 0; }
static __ccwrap_forceinline int isinf(double v) { int c = _fpclass(v); return c == _FPCLASS_PINF || c == _FPCLASS_NINF; }
static __ccwrap_forceinline int isfinite(double x) { return _finite(x) != 0; }
static __ccwrap_forceinline int isnormal(double v) { int c = _fpclass(v); return c == _FPCLASS_PN || c == _FPCLASS_NN; }
static __ccwrap_forceinline int signbit(double v) { int c = _fpclass(v); return _FPCLASS_NINF <= c && c <= _FPCLASS_NZ; }

static __ccwrap_forceinline int isunordered(double x, double y) { return isnan(x) || isnan(y); }
static __ccwrap_forceinline int isgreater(double x, double y) { return isunordered(x,y) ? 0 : x > y; }
static __ccwrap_forceinline int isgreaterequal(double x, double y) { return isunordered(x,y) ? 0 : x >= y; }
static __ccwrap_forceinline int isless(double x, double y) { return isunordered(x,y) ? 0 : x < y; }
static __ccwrap_forceinline int islessequal(double x, double y) { return isunordered(x,y) ? 0 : x <= y; }
//#define HUGE_VALL     (long double)(HUGE_VAL)
//#define HUGE_VALF     (float)(HUGE_VAL)
//#define INFINITY      HUGE_VALF
//#define NAN           (float)(NAN)

#endif	// __cplusplus

#endif	// _MSC_VER

#endif
