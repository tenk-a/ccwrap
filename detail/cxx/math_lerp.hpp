/**
 *  @file   detail/cxx/math_lerp.hpp
 *  @brief  std::lerp (C++20, [c.math.lerp]) for libraries that do not have it yet.
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_DETAIL_MATH_LERP_HPP
#define _CCW_DETAIL_MATH_LERP_HPP
#include <ccwrap_common.h>

#if !defined(__cpp_lib_interpolate)

namespace _CCW_STD20 {
namespace __ccw_m {
template<class T> _ccw_constexpr11 T lerp_x(T a, T b, T t) { return a + t * (b - a); }
template<class T> _ccw_constexpr11 T lerp_impl(T a, T b, T t) {
    return ((a <= T(0) && b >= T(0)) || (a >= T(0) && b <= T(0)))
               ? t * b + (T(1) - t) * a
         : (t == T(1)) ? b
         : ((t > T(1)) == (b > a))
               ? (b < lerp_x(a, b, t) ? lerp_x(a, b, t) : b)
               : (lerp_x(a, b, t) < b ? lerp_x(a, b, t) : b);
}
}   // namespace __ccw_m
_ccw_constexpr11 inline float       lerp(float a,       float b,       float t)       { return __ccw_m::lerp_impl(a, b, t); }
_ccw_constexpr11 inline double      lerp(double a,      double b,      double t)      { return __ccw_m::lerp_impl(a, b, t); }
_ccw_constexpr11 inline long double lerp(long double a, long double b, long double t) { return __ccw_m::lerp_impl(a, b, t); }
}   // _CCW_STD20

#endif  /* __cpp_lib_interpolate */
#endif  /* _CCW_DETAIL_MATH_LERP_HPP */
