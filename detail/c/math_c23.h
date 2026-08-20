/**
 *  @file   detail/math_c23.h
 *  @brief  The C23 / C++26 rounding / extrema / classification additions to <math.h> (for Open Watcom).
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_DETAIL_MATH_C23_H
#define _CCW_DETAIL_MATH_C23_H

#ifdef __cplusplus
# define _CCW_M23_FN    inline
#else
# define _CCW_M23_FN    static _ccw_forceinline
#endif

_CCW_M23_FN _ccw_ullong __ccw_m23_dbits(double __x) {
    union { double __d; _ccw_ullong __u; } __v; __v.__d = __x; return __v.__u;
}

_CCW_M23_FN double roundeven(double __x) {
    double __r, __d;
    if (isnan(__x) || isinf(__x)) return __x;
    __r = floor(__x);
    __d = __x - __r;                                    /* fractional part in [0,1) */
    if (__d < 0.5) return __r;
    if (__d > 0.5) return __r + 1.0;
    return (fmod(__r, 2.0) == 0.0) ? __r : __r + 1.0;   /* tie -> the even neighbour */
}
_CCW_M23_FN float       roundevenf(float __x)       { return (float)roundeven((double)__x); }
_CCW_M23_FN long double roundevenl(long double __x) { return roundeven((double)__x); }

_CCW_M23_FN double fmaximum(double __x, double __y) {
    if (isnan(__x)) return __x; if (isnan(__y)) return __y;
    if (__x > __y) return __x; if (__y > __x) return __y;
    return signbit(__x) ? __y : __x;              /* equal (incl +/-0): prefer +0 */
}
_CCW_M23_FN double fminimum(double __x, double __y) {
    if (isnan(__x)) return __x; if (isnan(__y)) return __y;
    if (__x < __y) return __x; if (__y < __x) return __y;
    return signbit(__x) ? __x : __y;              /* prefer -0 */
}
_CCW_M23_FN double fmaximum_num(double __x, double __y) {
    if (isnan(__x)) return isnan(__y) ? __x : __y; if (isnan(__y)) return __x;
    if (__x > __y) return __x; if (__y > __x) return __y;
    return signbit(__x) ? __y : __x;
}
_CCW_M23_FN double fminimum_num(double __x, double __y) {
    if (isnan(__x)) return isnan(__y) ? __x : __y; if (isnan(__y)) return __x;
    if (__x < __y) return __x; if (__y < __x) return __y;
    return signbit(__x) ? __x : __y;
}
_CCW_M23_FN double fmaximum_mag(double __x, double __y) {
    double __ax, __ay;
    if (isnan(__x)) return __x; if (isnan(__y)) return __y;
    __ax = fabs(__x); __ay = fabs(__y);
    if (__ax > __ay) return __x; if (__ay > __ax) return __y;
    return fmaximum(__x, __y);
}
_CCW_M23_FN double fminimum_mag(double __x, double __y) {
    double __ax, __ay;
    if (isnan(__x)) return __x; if (isnan(__y)) return __y;
    __ax = fabs(__x); __ay = fabs(__y);
    if (__ax < __ay) return __x; if (__ay < __ax) return __y;
    return fminimum(__x, __y);
}
_CCW_M23_FN double fmaximum_mag_num(double __x, double __y) {
    double __ax, __ay;
    if (isnan(__x)) return isnan(__y) ? __x : __y; if (isnan(__y)) return __x;
    __ax = fabs(__x); __ay = fabs(__y);
    if (__ax > __ay) return __x; if (__ay > __ax) return __y;
    return fmaximum_num(__x, __y);
}
_CCW_M23_FN double fminimum_mag_num(double __x, double __y) {
    double __ax, __ay;
    if (isnan(__x)) return isnan(__y) ? __x : __y; if (isnan(__y)) return __x;
    __ax = fabs(__x); __ay = fabs(__y);
    if (__ax < __ay) return __x; if (__ay < __ax) return __y;
    return fminimum_num(__x, __y);
}

#define _CCW_C23_EXTREMUM_FL(_N) \
    _CCW_M23_FN float       _N##f(float __x, float __y)             { return (float)_N((double)__x, (double)__y); } \
    _CCW_M23_FN long double _N##l(long double __x, long double __y) { return _N((double)__x, (double)__y); }
_CCW_C23_EXTREMUM_FL(fmaximum)         _CCW_C23_EXTREMUM_FL(fminimum)
_CCW_C23_EXTREMUM_FL(fmaximum_num)     _CCW_C23_EXTREMUM_FL(fminimum_num)
_CCW_C23_EXTREMUM_FL(fmaximum_mag)     _CCW_C23_EXTREMUM_FL(fminimum_mag)
_CCW_C23_EXTREMUM_FL(fmaximum_mag_num) _CCW_C23_EXTREMUM_FL(fminimum_mag_num)
#undef _CCW_C23_EXTREMUM_FL

_CCW_M23_FN _ccw_bool issignaling(double __x) {
    _ccw_ullong __u = __ccw_m23_dbits(__x);
    int __is_nan = ((__u >> 52) & 0x7FFu) == 0x7FFu && (__u & 0xFFFFFFFFFFFFFULL) != 0;
    return __is_nan && (__u & 0x8000000000000ULL) == 0;
}
_CCW_M23_FN _ccw_bool iscanonical(double __x) { (void)__x; return 1; }

#undef _CCW_M23_FN

#endif  /* _CCW_DETAIL_MATH_C23_H */
