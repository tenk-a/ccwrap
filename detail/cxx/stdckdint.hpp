/**
 *  @file   detail/stdckdint.hpp
 *  @brief  <stdckdint.h> (C++26) gap-fill: std::ckd_add / ckd_sub / ckd_mul.
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_DETAIL_STDCKDINT_HPP
#define _CCW_DETAIL_STDCKDINT_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include <limits>

#ifndef _CCW_STD_CKD_DEFINED
#define _CCW_STD_CKD_DEFINED

namespace std {

template <class T>
inline bool ckd_add(T* r, T a, T b) {
    T s = (T)(a + b);
    *r = s;
    if (std::numeric_limits<T>::is_signed)
        return (a > 0 && b > 0 && s < 0) || (a < 0 && b < 0 && s >= 0);
    return s < a;   // unsigned wrap
}

template <class T>
inline bool ckd_sub(T* r, T a, T b) {
    T d = (T)(a - b);
    *r = d;
    if (std::numeric_limits<T>::is_signed)
        return (a >= 0 && b < 0 && d < 0) || (a < 0 && b > 0 && d >= 0);
    return a < b;   // unsigned wrap
}

template <class T>
inline bool ckd_mul(T* r, T a, T b) {
    T p = (T)(a * b);
    *r = p;
    if (a == 0 || b == 0) return false;
    if (std::numeric_limits<T>::is_signed &&
        ((a == std::numeric_limits<T>::min() && b == (T)-1) ||
         (b == std::numeric_limits<T>::min() && a == (T)-1)))
        return true;
    return (T)(p / a) != b;
}

}   // namespace std

#endif  // _CCW_STD_CKD_DEFINED
#endif  // _CCW_DETAIL_STDCKDINT_HPP
