/**
 *  @file   detail/cxx/complex.hpp
 *  @brief  <complex> gap-fills: the C++11 additional overloads for arithmetic
 *          types, proj, the inverse trigonometric / hyperbolic functions, and
 *          the mixed-type pow overloads.
 *  @license Boost Software License Version 1.0
 *  @note   Compiler-agnostic bodies; the forwarder (vc/std/complex,
 *          gcc/std/complex) includes native <complex> first, then this file.
 *          Self-guarded, so including it unconditionally is safe.
 */
#ifndef _CCW_DETAIL_COMPLEX_HPP
#define _CCW_DETAIL_COMPLEX_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>

#if _CCW_STD_LIB_LT(1920, 201103L)
namespace std {

#define _CCW_CX_OVER(_CxT, _CxR)                                                                  \
    inline complex<_CxR> conj(_CxT __x) { return complex<_CxR>(static_cast<_CxR>(__x), static_cast<_CxR>(0)); }\
    inline complex<_CxR> proj(_CxT __x) { return complex<_CxR>(static_cast<_CxR>(__x), static_cast<_CxR>(0)); }\
    inline _CxR norm(_CxT __x) { _CxR __v = static_cast<_CxR>(__x); return __v * __v; }           \
    inline _CxR real(_CxT __x) { return static_cast<_CxR>(__x); }                                 \
    inline _CxR imag(_CxT)     { return static_cast<_CxR>(0); }                                                \
    inline _CxR arg(_CxT __x)  { return static_cast<_CxR>(__x) < static_cast<_CxR>(0)                          \
                                    ? static_cast<_CxR>(3.14159265358979323846264338327950288L) : static_cast<_CxR>(0); }

_CCW_CX_OVER(int,                double)
_CCW_CX_OVER(unsigned int,       double)
_CCW_CX_OVER(long,               double)
_CCW_CX_OVER(unsigned long,      double)
_CCW_CX_OVER(long long,          double)
_CCW_CX_OVER(unsigned long long, double)
_CCW_CX_OVER(short,              double)
_CCW_CX_OVER(unsigned short,     double)
_CCW_CX_OVER(float,              float)
_CCW_CX_OVER(double,             double)
_CCW_CX_OVER(long double,        long double)

#undef _CCW_CX_OVER

}   // namespace std
#endif

#if _CCW_STD_LIB_LT(1600, 201103L)
#include <cmath>
#include <limits>
namespace std {

template <class _Tp>
inline complex<_Tp> proj(const complex<_Tp>& __z) {
    if (std::isinf(__z.real()) || std::isinf(__z.imag()))
        return complex<_Tp>(std::numeric_limits<_Tp>::infinity(),
                            std::copysign(_Tp(0), __z.imag()));
    return __z;
}

template <class _Tp>
inline complex<_Tp> asinh(const complex<_Tp>& __z) {
    return std::log(__z + std::sqrt(__z * __z + complex<_Tp>(_Tp(1))));
}
template <class _Tp>
inline complex<_Tp> acosh(const complex<_Tp>& __z) {
    return std::log(__z + std::sqrt(__z + complex<_Tp>(_Tp(1))) * std::sqrt(__z - complex<_Tp>(_Tp(1))));
}
template <class _Tp>
inline complex<_Tp> atanh(const complex<_Tp>& __z) {
    return (std::log(complex<_Tp>(_Tp(1)) + __z) - std::log(complex<_Tp>(_Tp(1)) - __z)) / _Tp(2);
}
template <class _Tp>
inline complex<_Tp> asin(const complex<_Tp>& __z) {
    const complex<_Tp> __i(_Tp(0), _Tp(1));
    const complex<_Tp> __w = asinh(__i * __z);
    return complex<_Tp>(__w.imag(), -__w.real());        // -i * w
}
template <class _Tp>
inline complex<_Tp> acos(const complex<_Tp>& __z) {
    const _Tp __half_pi = _Tp(1.57079632679489661923132169163975144L);
    return complex<_Tp>(__half_pi) - asin(__z);
}
template <class _Tp>
inline complex<_Tp> atan(const complex<_Tp>& __z) {
    const complex<_Tp> __i(_Tp(0), _Tp(1));
    const complex<_Tp> __w = atanh(__i * __z);
    return complex<_Tp>(__w.imag(), -__w.real());        // -i * w
}

#define _CCW_CX_POW(_CxA, _CxB, _CxR)                                                          \
    inline complex<_CxR> pow(const complex<_CxA>& __x, const complex<_CxB>& __y) {             \
        return std::pow(complex<_CxR>((_CxR)__x.real(), (_CxR)__x.imag()),                     \
                        complex<_CxR>((_CxR)__y.real(), (_CxR)__y.imag()));                    \
    }                                                                                          \
    inline complex<_CxR> pow(const _CxA& __x, const complex<_CxB>& __y) {                      \
        return std::pow(complex<_CxR>((_CxR)__x, static_cast<_CxR>(0)),                                     \
                        complex<_CxR>((_CxR)__y.real(), (_CxR)__y.imag()));                    \
    }                                                                                          \
    inline complex<_CxR> pow(const complex<_CxA>& __x, const _CxB& __y) {                      \
        return std::pow(complex<_CxR>((_CxR)__x.real(), (_CxR)__x.imag()),                     \
                        complex<_CxR>((_CxR)__y, static_cast<_CxR>(0)));                                    \
    }

_CCW_CX_POW(float,       double,      double)
_CCW_CX_POW(double,      float,       double)
_CCW_CX_POW(float,       long double, long double)
_CCW_CX_POW(long double, float,       long double)
_CCW_CX_POW(double,      long double, long double)
_CCW_CX_POW(long double, double,      long double)
#undef _CCW_CX_POW

}   // namespace std
#endif

#endif  // _CCW_DETAIL_COMPLEX_HPP
