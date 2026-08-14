/**
 *  @file   detail/iterator.hpp
 *  @brief  <iterator> gap-fills (begin/end/next/prev, cbegin/rbegin family,
 *          size/empty/data, ssize).
 *  @license Boost Software License Version 1.0
 *  @note   Compiler-agnostic bodies; the forwarder (vc/iterator) includes native
 *          <iterator> first, then this file.
 */
#ifndef _CCW_DETAIL_ITERATOR_HPP
#define _CCW_DETAIL_ITERATOR_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include <cstddef>

#if _CCW_STD_LIB_LT(1600, 201103L)

namespace std {

template<class C> inline typename C::iterator       begin(C& c)       { return c.begin(); }
template<class C> inline typename C::const_iterator begin(const C& c) { return c.begin(); }
template<class C> inline typename C::iterator       end(C& c)         { return c.end(); }
template<class C> inline typename C::const_iterator end(const C& c)   { return c.end(); }
template<class T, std::size_t N> inline T* begin(T (&a)[N]) { return a; }
template<class T, std::size_t N> inline T* end(T (&a)[N])   { return a + N; }

template<class It>
inline It next(It it, typename std::iterator_traits<It>::difference_type n = 1) {
    std::advance(it, n);
    return it;
}
template<class It>
inline It prev(It it, typename std::iterator_traits<It>::difference_type n = 1) {
    std::advance(it, -n);
    return it;
}

}   // namespace std
#endif

#if _CCW_STD_LIB_LT(1800, 201402L)
namespace _CCW_STD14 {
template<class C> inline typename C::const_iterator cbegin(const C& c) { return c.begin(); }
template<class C> inline typename C::const_iterator cend(const C& c)   { return c.end(); }
template<class T, std::size_t N> inline const T* cbegin(const T (&a)[N]) { return a; }
template<class T, std::size_t N> inline const T* cend(const T (&a)[N])   { return a + N; }
}   // _CCW_STD14
#endif

#if _CCW_STD_LIB_LT(1800, 201402L)
namespace _CCW_STD14 {
template<class C> inline typename C::reverse_iterator       rbegin(C& c)        { return c.rbegin(); }
template<class C> inline typename C::const_reverse_iterator rbegin(const C& c)  { return c.rbegin(); }
template<class C> inline typename C::reverse_iterator       rend(C& c)          { return c.rend(); }
template<class C> inline typename C::const_reverse_iterator rend(const C& c)    { return c.rend(); }
template<class T, std::size_t N> inline std::reverse_iterator<T*> rbegin(T (&a)[N]) { return std::reverse_iterator<T*>(a + N); }
template<class T, std::size_t N> inline std::reverse_iterator<T*> rend(T (&a)[N])   { return std::reverse_iterator<T*>(a); }
template<class C> inline typename C::const_reverse_iterator crbegin(const C& c) { return c.rbegin(); }
template<class C> inline typename C::const_reverse_iterator crend(const C& c)   { return c.rend(); }
template<class T, std::size_t N> inline std::reverse_iterator<const T*> crbegin(const T (&a)[N]) { return std::reverse_iterator<const T*>(a + N); }
template<class T, std::size_t N> inline std::reverse_iterator<const T*> crend(const T (&a)[N])   { return std::reverse_iterator<const T*>(a); }
}   // _CCW_STD14
#endif

#if _CCW_STD_LIB_LT(1900, 201703L)
namespace _CCW_STD17 {
template<class C> inline typename C::size_type size(const C& c)  { return c.size(); }
template<class T, std::size_t N> inline std::size_t size(const T (&)[N]) { return N; }
template<class C> inline bool empty(const C& c)  { return c.empty(); }
template<class T, std::size_t N> inline bool empty(const T (&)[N]) { return false; }
template<class C> inline typename C::pointer       data(C& c)       { return c.empty() ? (typename C::pointer)0       : &c[0]; }
template<class C> inline typename C::const_pointer data(const C& c) { return c.empty() ? (typename C::const_pointer)0 : &c[0]; }
template<class T, std::size_t N> inline T* data(T (&a)[N]) { return a; }
}   // _CCW_STD17
#endif

#if __cplusplus < 202002L
namespace _CCW_STD20 {
template<class C> inline std::ptrdiff_t ssize(const C& c) { return static_cast<std::ptrdiff_t>(c.size()); }
template<class T, std::size_t N> inline std::ptrdiff_t ssize(const T (&)[N]) { return static_cast<std::ptrdiff_t>(N); }
}   // _CCW_STD20
#endif

#endif  // _CCW_DETAIL_ITERATOR_HPP
