/**
 *  @file   detail/algorithm.hpp
 *  @brief  <algorithm> gap-fills (C++11/17/20) missing from the native library.
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_DETAIL_ALGORITHM_HPP
#define _CCW_DETAIL_ALGORITHM_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include <utility>    /* std::pair */
#include <iterator>
#include <functional> /* std::less (is_heap default) */
#include "move03.hpp" /* _ccw::move -- real move on C++11, emulation before it */

#if _CCW_STD_LIB_LT(1600, 201103L)

namespace std {

template<class It, class Pred>
inline bool all_of(It first, It last, Pred p) {
    for (; first != last; ++first) if (!p(*first)) return false;
    return true;
}
template<class It, class Pred>
inline bool any_of(It first, It last, Pred p) {
    for (; first != last; ++first) if (p(*first)) return true;
    return false;
}
template<class It, class Pred>
inline bool none_of(It first, It last, Pred p) {
    for (; first != last; ++first) if (p(*first)) return false;
    return true;
}
template<class In, class Size, class Out>
inline Out copy_n(In first, Size n, Out result) {
    for (; n > 0; --n, ++first, ++result) *result = *first;
    return result;
}

template<class In, class Out>
inline Out move(In first, In last, Out result) {
    for (; first != last; ++first, ++result) *result = ::_ccw::move(*first);
    return result;
}
template<class Bi1, class Bi2>
inline Bi2 move_backward(Bi1 first, Bi1 last, Bi2 result) {
    while (first != last) *--result = ::_ccw::move(*--last);
    return result;
}
template<class It, class Pred>
inline It find_if_not(It first, It last, Pred p) {
    for (; first != last; ++first) if (!p(*first)) return first;
    return last;
}
template<class It>
inline bool is_sorted(It first, It last) {
    if (first == last) return true;
    It next = first;
    for (++next; next != last; first = next, ++next) if (*next < *first) return false;
    return true;
}
template<class It, class Comp>
inline bool is_sorted(It first, It last, Comp comp) {
    if (first == last) return true;
    It next = first;
    for (++next; next != last; first = next, ++next) if (comp(*next, *first)) return false;
    return true;
}
template<class T>
inline std::pair<T, T> minmax(const T& a, const T& b) {
    return b < a ? std::pair<T, T>(b, a) : std::pair<T, T>(a, b);
}
template<class It>
inline std::pair<It, It> minmax_element(It first, It last) {
    It mn = first, mx = first;
    if (first != last) for (++first; first != last; ++first) {
        if (*first < *mn) mn = first;
        if (!(*first < *mx)) mx = first;
    }
    return std::pair<It, It>(mn, mx);
}
template<class It>
inline It is_sorted_until(It first, It last) {
    if (first == last) return last;
    It next = first;
    for (++next; next != last; first = next, ++next) if (*next < *first) return next;
    return last;
}
template<class It, class Comp>
inline It is_sorted_until(It first, It last, Comp comp) {
    if (first == last) return last;
    It next = first;
    for (++next; next != last; first = next, ++next) if (comp(*next, *first)) return next;
    return last;
}
template<class It, class Pred>
inline bool is_partitioned(It first, It last, Pred p) {
    for (; first != last; ++first) if (!p(*first)) break;
    for (; first != last; ++first) if (p(*first)) return false;
    return true;
}
template<class It, class Pred>
inline It partition_point(It first, It last, Pred p) {
    typename std::iterator_traits<It>::difference_type n = std::distance(first, last);
    while (n > 0) {
        It mid = first;
        typename std::iterator_traits<It>::difference_type half = n / 2;
        std::advance(mid, half);
        if (p(*mid)) { first = ++mid; n -= half + 1; }
        else         { n = half; }
    }
    return first;
}

template<class In, class Out, class Pred>
inline Out copy_if(In first, In last, Out out, Pred pred) {
    for (; first != last; ++first) if (pred(*first)) { *out = *first; ++out; }
    return out;
}
template<class In, class OutT, class OutF, class Pred>
inline std::pair<OutT, OutF> partition_copy(In first, In last, OutT ot, OutF of, Pred pred) {
    for (; first != last; ++first) { if (pred(*first)) { *ot = *first; ++ot; } else { *of = *first; ++of; } }
    return std::pair<OutT, OutF>(ot, of);
}

template<class RA, class Comp>
inline RA is_heap_until(RA first, RA last, Comp comp) {
    typename std::iterator_traits<RA>::difference_type len = last - first;
    for (typename std::iterator_traits<RA>::difference_type c = 1; c < len; ++c)
        if (comp(first[(c - 1) / 2], first[c])) return first + c;
    return last;
}
template<class RA>
inline RA is_heap_until(RA first, RA last) {
    return std::is_heap_until(first, last, std::less<typename std::iterator_traits<RA>::value_type>());
}
template<class RA, class Comp> inline bool is_heap(RA first, RA last, Comp comp) { return std::is_heap_until(first, last, comp) == last; }
template<class RA>             inline bool is_heap(RA first, RA last)             { return std::is_heap_until(first, last) == last; }

}   // namespace std
#endif

#if _CCW_STD_LIB_LT(1700, 201103L)
namespace std {

struct __ccw_algo_eq { template<class A, class B> bool operator()(const A& a, const B& b) const { return a == b; } };

template<class It1, class It2, class Pred>
inline bool is_permutation(It1 first1, It1 last1, It2 first2, Pred pred) {
    for (; first1 != last1; ++first1, ++first2) if (!pred(*first1, *first2)) break;
    if (first1 == last1) return true;
    It2 last2 = first2; std::advance(last2, std::distance(first1, last1));
    for (It1 i = first1; i != last1; ++i) {
        bool counted = false;
        for (It1 j = first1; j != i; ++j) if (pred(*j, *i)) { counted = true; break; }
        if (counted) continue;
        int c1 = 0; for (It1 j = i; j != last1; ++j) if (pred(*i, *j)) ++c1;
        int c2 = 0; for (It2 j = first2; j != last2; ++j) if (pred(*i, *j)) ++c2;
        if (c1 != c2) return false;
    }
    return true;
}
template<class It1, class It2>
inline bool is_permutation(It1 first1, It1 last1, It2 first2) {
    return std::is_permutation(first1, last1, first2, __ccw_algo_eq());
}

template<class _It, class _Urbg>
inline void shuffle(_It __first, _It __last, _Urbg& __g) {
    typedef typename std::iterator_traits<_It>::difference_type _Diff;
    typedef typename _Urbg::result_type _Res;
    _Diff __n = __last - __first;
    if (__n < 2) return;
    const _Res __range = __g.max() - __g.min();
    for (_Diff __i = __n - 1; __i > 0; --__i) {
        const _Res __bound = (_Res)(__i + 1);
        const _Res __limit = __range == (_Res)-1
            ? (_Res)(((_Res)-1 / __bound) * __bound)
            : (_Res)(((__range + 1) / __bound) * __bound);
        _Res __r;
        do { __r = (_Res)(__g() - __g.min()); } while (__limit != 0 && __r >= __limit);
        const _Diff __j = (_Diff)(__r % __bound);
        if (__j != __i) {
            typename std::iterator_traits<_It>::value_type __t = __first[__i];
            __first[__i] = __first[__j];
            __first[__j] = __t;
        }
    }
}

}   // namespace std
#endif

#if __cplusplus < 201703L && (!defined(_MSC_VER) || _MSC_VER < 1900)
namespace _CCW_STD17 {
template<class T>
inline const T& clamp(const T& v, const T& lo, const T& hi) {
    return v < lo ? lo : (hi < v ? hi : v);
}
template<class T, class Comp>
inline const T& clamp(const T& v, const T& lo, const T& hi, Comp comp) {
    return comp(v, lo) ? lo : (comp(hi, v) ? hi : v);
}
}   // _CCW_STD17
#endif

#if __cplusplus < 201703L
namespace _CCW_STD17 {
template<class It, class Size, class Fn>
inline It for_each_n(It first, Size n, Fn f) {
    for (; n > 0; --n, ++first) f(*first);
    return first;
}
}   // _CCW_STD17
#endif

#if __cplusplus < 202002L
namespace _CCW_STD20 {
template<class It>
inline It shift_left(It first, It last, typename std::iterator_traits<It>::difference_type n) {
    if (n <= 0) return last;
    It mid = first;
    for (typename std::iterator_traits<It>::difference_type i = 0; i < n; ++i) {
        if (mid == last) return first;
        ++mid;
    }
    It out = first;
    for (; mid != last; ++mid, ++out) *out = *mid;
    return out;
}
template<class It>
inline It shift_right(It first, It last, typename std::iterator_traits<It>::difference_type n) {
    if (n <= 0) return first;
    typename std::iterator_traits<It>::difference_type sz = std::distance(first, last);
    if (n >= sz) return last;
    It src = first; std::advance(src, sz - n);   /* one past the last source element */
    It ret = first; std::advance(ret, n);
    It d = last;                                 /* backward move (bidirectional) */
    while (src != first) { --src; --d; *d = *src; }
    return ret;
}
}   // _CCW_STD20
#endif

namespace _CCW_STD17 {

template<class It, class Out, class Size, class Gen>
inline Out sample(It first, It last, Out out, Size n, Gen& g) {
    typedef typename std::iterator_traits<It>::difference_type diff_t;
    diff_t left = std::distance(first, last);
    if (n > (Size)left) n = (Size)left;
    for (; first != last && n > 0; ++first, --left) {
        if ((Size)(g() % (unsigned long)left) < n) { *out = *first; ++out; --n; }
    }
    return out;
}

}   // _CCW_STD17

#endif  // _CCW_DETAIL_ALGORITHM_HPP
