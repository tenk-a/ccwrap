/**
 *  @file   detail/numeric.hpp
 *  @brief  <numeric> gap-fills (C++11/17/20) missing from the native library.
 *  @license Boost Software License Version 1.0
 *  @note   Compiler-agnostic bodies; the forwarder (vc/numeric) includes native
 *          <numeric> first, then this file.
 */
#ifndef _CCW_DETAIL_NUMERIC_HPP
#define _CCW_DETAIL_NUMERIC_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include <iterator>

namespace _ccw {
template<class T> struct __ccw_not_bool { typedef T type; };
template<> struct __ccw_not_bool<bool> { };
}   // namespace _ccw

#if __cplusplus < 201703L

namespace _CCW_STD17 {

template<class T>
inline typename ::_ccw::__ccw_not_bool<T>::type gcd(T a, T b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) { T t = a % b; a = b; b = t; }
    return a;
}

template<class T>
inline typename ::_ccw::__ccw_not_bool<T>::type lcm(T a, T b) {
    if (a == 0 || b == 0) return 0;
    T g = gcd(a, b);   /* sibling in this namespace (std or _ccw) */
    T x = a / g;
    if (x < 0) x = -x;
    T y = b < 0 ? -b : b;
    return x * y;
}

}   // _CCW_STD17
#endif

#if _CCW_STD_LIB_LT(1600, 201103L)
namespace std {
template<class It, class T>
inline void iota(It first, It last, T value) {
    for (; first != last; ++first, ++value) *first = value;
}
}   // namespace std
#endif

#if __cplusplus < 201703L
namespace _CCW_STD17 {
template<class It, class T>
inline T reduce(It first, It last, T init) {
    for (; first != last; ++first) init = init + *first;
    return init;
}
template<class It, class T, class BinOp>
inline T reduce(It first, It last, T init, BinOp op) {
    for (; first != last; ++first) init = op(init, *first);
    return init;
}
template<class It>
inline typename std::iterator_traits<It>::value_type reduce(It first, It last) {
    typename std::iterator_traits<It>::value_type init = typename std::iterator_traits<It>::value_type();
    return reduce(first, last, init);   /* sibling overload in this namespace */
}
}   // _CCW_STD17
#endif

#if __cplusplus < 202002L
#include <limits>
#include <type_traits>   // make_unsigned (ccwrap backports it for vc8/9)
namespace _CCW_STD20 {
namespace __ccw_mp {

template<bool _IsInteger> struct __impl;

template<> struct __impl<true> {
    template<class T> static T __f(T a, T b) {
        typedef typename std::make_unsigned<T>::type U;
        return a > b ? static_cast<T>(a - static_cast<T>(static_cast<U>(static_cast<U>(a) - static_cast<U>(b)) / 2))
                     : static_cast<T>(a + static_cast<T>(static_cast<U>(static_cast<U>(b) - static_cast<U>(a)) / 2));
    }
};

template<> struct __impl<false> {
    template<class T> static T __f(T a, T b) {
        const T lo = std::numeric_limits<T>::min() * 2;
        const T hi = std::numeric_limits<T>::max() / 2;
        const T aa = a < 0 ? -a : a;
        const T ab = b < 0 ? -b : b;
        if (aa <= hi && ab <= hi) return (a + b) / 2;   // the common case, no rounding lost
        if (aa < lo)              return a + b / 2;     // a is subnormal-ish: keep it whole
        if (ab < lo)              return a / 2 + b;
        return a / 2 + b / 2;                           // both huge: halve first
    }
};

}   // namespace __ccw_mp

template<class T>
inline typename ::_ccw::__ccw_not_bool<T>::type midpoint(T a, T b) {
    return __ccw_mp::__impl<std::numeric_limits<T>::is_integer>::__f(a, b);
}

template<class T>
inline T* midpoint(T* a, T* b) { return a + (b - a) / 2; }
}   // _CCW_STD20
#endif

#if __cplusplus < 201703L
namespace _CCW_STD17 {

template<class It1, class It2, class T>
inline T transform_reduce(It1 f1, It1 l1, It2 f2, T init) {
    for (; f1 != l1; ++f1, ++f2) init = init + (*f1) * (*f2);
    return init;
}
template<class It1, class It2, class T, class BOp1, class BOp2>
inline T transform_reduce(It1 f1, It1 l1, It2 f2, T init, BOp1 reduce, BOp2 transform) {
    for (; f1 != l1; ++f1, ++f2) init = reduce(init, transform(*f1, *f2));
    return init;
}
template<class It, class T, class BOp, class UOp>
inline T transform_reduce(It first, It last, T init, BOp reduce, UOp transform) {
    for (; first != last; ++first) init = reduce(init, transform(*first));
    return init;
}

template<class In, class Out, class T>
inline Out exclusive_scan(In first, In last, Out d, T init) {
    for (; first != last; ++first) { T tmp = init; init = init + *first; *d = tmp; ++d; }
    return d;
}
template<class In, class Out, class T, class BOp>
inline Out exclusive_scan(In first, In last, Out d, T init, BOp op) {
    for (; first != last; ++first) { T tmp = init; init = op(init, *first); *d = tmp; ++d; }
    return d;
}

template<class In, class Out>
inline Out inclusive_scan(In first, In last, Out d) {
    if (first == last) return d;
    typename std::iterator_traits<In>::value_type sum = *first;
    *d = sum; ++d; ++first;
    for (; first != last; ++first) { sum = sum + *first; *d = sum; ++d; }
    return d;
}
template<class In, class Out, class BOp>
inline Out inclusive_scan(In first, In last, Out d, BOp op) {
    if (first == last) return d;
    typename std::iterator_traits<In>::value_type sum = *first;
    *d = sum; ++d; ++first;
    for (; first != last; ++first) { sum = op(sum, *first); *d = sum; ++d; }
    return d;
}

template<class In, class Out, class BOp, class UOp>
inline Out transform_inclusive_scan(In first, In last, Out d, BOp op, UOp uop) {
    if (first == last) return d;
    typename std::iterator_traits<In>::value_type sum = uop(*first);
    *d = sum; ++d; ++first;
    for (; first != last; ++first) { sum = op(sum, uop(*first)); *d = sum; ++d; }
    return d;
}
template<class In, class Out, class BOp, class UOp, class T>
inline Out transform_inclusive_scan(In first, In last, Out d, BOp op, UOp uop, T init) {
    for (; first != last; ++first) { init = op(init, uop(*first)); *d = init; ++d; }
    return d;
}
template<class In, class Out, class T, class BOp, class UOp>
inline Out transform_exclusive_scan(In first, In last, Out d, T init, BOp op, UOp uop) {
    for (; first != last; ++first) { T tmp = init; init = op(init, uop(*first)); *d = tmp; ++d; }
    return d;
}

template<class In, class Out, class BOp, class T>
inline Out inclusive_scan(In first, In last, Out d, BOp op, T sum) {
    for (; first != last; ++first) { sum = op(sum, *first); *d = sum; ++d; }
    return d;
}

}   // _CCW_STD17
#endif

#endif  // _CCW_DETAIL_NUMERIC_HPP
