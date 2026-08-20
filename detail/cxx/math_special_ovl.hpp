/**
 *  @file   detail/cxx/math_special_ovl.hpp
 *  @brief  The same-named float / long double overloads required by [sf.cmath]/2.
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_DETAIL_MATH_SPECIAL_OVL_HPP
#define _CCW_DETAIL_MATH_SPECIAL_OVL_HPP

#include <ccwrap_common.h>

#if defined(__cplusplus) && _CCW_TARGET_CXX >= 2017

namespace _CCW_STD {

#define _CCW_SF_OVL_T(_Nm) \
    inline float _Nm(float __x) { return _Nm##f(__x); } \
    inline long double _Nm(long double __x) { return _Nm##l(__x); }
#define _CCW_SF_OVL_TT(_Nm) \
    inline float _Nm(float __a, float __b) { return _Nm##f(__a, __b); } \
    inline long double _Nm(long double __a, long double __b) { return _Nm##l(__a, __b); }
#define _CCW_SF_OVL_TTT(_Nm) \
    inline float _Nm(float __a, float __b, float __c) { return _Nm##f(__a, __b, __c); } \
    inline long double _Nm(long double __a, long double __b, long double __c) { return _Nm##l(__a, __b, __c); }
#define _CCW_SF_OVL_UT(_Nm) \
    inline float _Nm(unsigned __n, float __x) { return _Nm##f(__n, __x); } \
    inline long double _Nm(unsigned __n, long double __x) { return _Nm##l(__n, __x); }
#define _CCW_SF_OVL_UUT(_Nm) \
    inline float _Nm(unsigned __n, unsigned __m, float __x) { return _Nm##f(__n, __m, __x); } \
    inline long double _Nm(unsigned __n, unsigned __m, long double __x) { return _Nm##l(__n, __m, __x); }

_CCW_SF_OVL_T(comp_ellint_1)  _CCW_SF_OVL_T(comp_ellint_2)
_CCW_SF_OVL_T(expint)         _CCW_SF_OVL_T(riemann_zeta)
_CCW_SF_OVL_TT(beta)          _CCW_SF_OVL_TT(comp_ellint_3)
_CCW_SF_OVL_TT(ellint_1)      _CCW_SF_OVL_TT(ellint_2)
_CCW_SF_OVL_TT(cyl_bessel_i)  _CCW_SF_OVL_TT(cyl_bessel_j)
_CCW_SF_OVL_TT(cyl_bessel_k)  _CCW_SF_OVL_TT(cyl_neumann)
_CCW_SF_OVL_TTT(ellint_3)
#ifndef _CCW_SF_NATIVE_HERMITE
_CCW_SF_OVL_UT(hermite)
#endif
_CCW_SF_OVL_UT(laguerre)      _CCW_SF_OVL_UT(legendre)
_CCW_SF_OVL_UT(sph_bessel)    _CCW_SF_OVL_UT(sph_neumann)
_CCW_SF_OVL_UUT(assoc_laguerre) _CCW_SF_OVL_UUT(assoc_legendre) _CCW_SF_OVL_UUT(sph_legendre)

#undef _CCW_SF_OVL_T
#undef _CCW_SF_OVL_TT
#undef _CCW_SF_OVL_TTT
#undef _CCW_SF_OVL_UT
#undef _CCW_SF_OVL_UUT

}   // namespace _CCW_STD

#endif  /* _CCW_TARGET_CXX >= 2017 */
#endif  /* _CCW_DETAIL_MATH_SPECIAL_OVL_HPP */
