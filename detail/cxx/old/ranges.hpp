/**
 *  @file   detail/old/ranges.hpp
 *  @brief  Partial <ranges> backport: access functions + range wrappers over
 *          classic <algorithm> / <numeric> (no views / lazy adaptors / concepts).
 *  @license Boost Software License Version 1.0
 *  @note   Included by the forwarder (vc/ranges) only when native <ranges> is absent.
 */
#ifndef _CCW_DETAIL_RANGES_HPP
#define _CCW_DETAIL_RANGES_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include <cstddef>
#include <iterator>
#include <algorithm>
#include <numeric>

namespace _CCW_STD20 {
using namespace ::std;
namespace ranges {

template<class C> inline typename C::iterator       begin(C& c)       { return c.begin(); }
template<class C> inline typename C::const_iterator begin(const C& c) { return c.begin(); }
template<class C> inline typename C::iterator       end(C& c)         { return c.end(); }
template<class C> inline typename C::const_iterator end(const C& c)   { return c.end(); }
template<class T, std::size_t N> inline T* begin(T (&a)[N]) { return a; }
template<class T, std::size_t N> inline T* end(T (&a)[N])   { return a + N; }

template<class C> inline typename C::size_type size(const C& c) { return c.size(); }
template<class T, std::size_t N> inline std::size_t size(const T (&)[N]) { return N; }
template<class C> inline bool empty(const C& c) { return c.empty(); }
template<class C> inline typename C::pointer       data(C& c)       { return c.data(); }
template<class C> inline typename C::const_pointer data(const C& c) { return c.data(); }

template<class R, class T> inline typename R::iterator find(R& r, const T& v) { return std::find(r.begin(), r.end(), v); }
template<class R, class Pred> inline typename R::iterator find_if(R& r, Pred p)     { return std::find_if(r.begin(), r.end(), p); }
template<class R, class Pred> inline typename R::iterator find_if_not(R& r, Pred p) { return std::find_if_not(r.begin(), r.end(), p); }
template<class R, class T> inline std::ptrdiff_t count(R& r, const T& v) { return std::count(r.begin(), r.end(), v); }
template<class R, class Pred> inline std::ptrdiff_t count_if(R& r, Pred p) { return std::count_if(r.begin(), r.end(), p); }
template<class R, class F> inline void for_each(R& r, F f) { std::for_each(r.begin(), r.end(), f); }
template<class R, class Pred> inline bool all_of(R& r, Pred p)  { return std::all_of(r.begin(), r.end(), p); }
template<class R, class Pred> inline bool any_of(R& r, Pred p)  { return std::any_of(r.begin(), r.end(), p); }
template<class R, class Pred> inline bool none_of(R& r, Pred p) { return std::none_of(r.begin(), r.end(), p); }
template<class R> inline typename R::iterator min_element(R& r) { return std::min_element(r.begin(), r.end()); }
template<class R> inline typename R::iterator max_element(R& r) { return std::max_element(r.begin(), r.end()); }
template<class R> inline void sort(R& r)  { std::sort(r.begin(), r.end()); }
template<class R> inline void reverse(R& r) { std::reverse(r.begin(), r.end()); }
template<class R, class Out> inline Out copy(R& r, Out out) { return std::copy(r.begin(), r.end(), out); }
template<class R, class T> inline void fill(R& r, const T& v) { std::fill(r.begin(), r.end(), v); }
template<class R, class T> inline T accumulate(R& r, T init) { return std::accumulate(r.begin(), r.end(), init); }

}   // namespace ranges
}   // namespace _CCW_STD20

// When _CCW_TARGET_CXX < 2020, _CCW_STD20 IS _ccw, so ranges already lives in _ccw;
// a self-referential namespace-alias would be an error -- only alias when std.
#if _CCW_TARGET_CXX >= 2020
namespace _ccw { namespace ranges = _CCW_STD20::ranges; }
#endif

#endif  // _CCW_DETAIL_RANGES_HPP
