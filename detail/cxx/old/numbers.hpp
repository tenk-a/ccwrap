/**
 *  @file   detail/old/numbers.hpp
 *  @brief  <numbers> backport: double-precision math constants (C++20).
 *  @license Boost Software License Version 1.0
 *  @note   Included by the forwarder (vc/numbers) only when native <numbers> is
 *          absent. The per-type variable templates (pi_v<T> etc.) are not provided.
 */
#ifndef _CCW_DETAIL_NUMBERS_HPP
#define _CCW_DETAIL_NUMBERS_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>

namespace _CCW_STD20 {
using namespace ::std;
namespace numbers {
static const double e          = 2.718281828459045235360287471352662498;
static const double log2e      = 1.442695040888963407359924681001892137;
static const double log10e     = 0.434294481903251827651128918916605082;
static const double pi         = 3.141592653589793238462643383279502884;
static const double inv_pi     = 0.318309886183790671537767526745028724;
static const double inv_sqrtpi = 0.564189583547756286948079451560772586;
static const double ln2        = 0.693147180559945309417232121458176568;
static const double ln10       = 2.302585092994045684017991454684364208;
static const double sqrt2      = 1.414213562373095048801688724209698079;
static const double sqrt3      = 1.732050807568877293527446341505872367;
static const double inv_sqrt3  = 0.577350269189625764509148780501957456;
static const double egamma     = 0.577215664901532860606512090082402431;
static const double phi        = 1.618033988749894848204586834365638118;
}   // namespace numbers
}   // namespace _CCW_STD20
// When _CCW_TARGET_CXX < 2020, _CCW_STD20 IS _ccw, so numbers already lives in _ccw;
// a self-referential namespace-alias would be an error -- only alias when std.
#if _CCW_TARGET_CXX >= 2020
namespace _ccw { namespace numbers = _CCW_STD20::numbers; }
#endif

#endif  // _CCW_DETAIL_NUMBERS_HPP
