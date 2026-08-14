/**
 *  @file   detail/utility.hpp
 *  @brief  <utility> gap-fills (exchange, as_const, cmp_* / in_range, to_underlying).
 *  @license Boost Software License Version 1.0
 *  @note   Compiler-agnostic bodies; the forwarder (vc/utility) includes native
 *          <utility> first, then this file.
 */
#ifndef _CCW_DETAIL_UTILITY_HPP
#define _CCW_DETAIL_UTILITY_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>

#if !defined(__GLIBCXX__) && !defined(_LIBCPP_VERSION) && \
    (!defined(_MSC_VER) || _MSC_VER < 1600) && !defined(__WATCOMC__)
#include <cstddef>
namespace std {
template<class _Tp, _CCW_STD::size_t _Np>
inline void swap(_Tp (&__a)[_Np], _Tp (&__b)[_Np]) {
    for (_CCW_STD::size_t __i = 0; __i < _Np; ++__i) {
        _Tp __t = __a[__i]; __a[__i] = __b[__i]; __b[__i] = __t;
    }
}
}   // namespace std
#endif

#if _CCW_STD_LIB_LT(1900, 201402L)
namespace _CCW_STD14 {
template<class T, class U>
inline T exchange(T& obj, const U& new_value) {
    T old = obj;
    obj = new_value;
    return old;
}
}   // _CCW_STD14
#endif

#if _CCW_STD_LIB_LT(1900, 201402L) && \
    ((defined(_MSC_VER) && _MSC_VER >= 1800) || \
     (!defined(_MSC_VER) && !defined(__WATCOMC__) && _ccw_cplusplus >= 201103L))
#include <cstddef>
namespace _CCW_STD14 {

template<class T, T... I>
struct integer_sequence {
    typedef T value_type;
    static std::size_t size() { return sizeof...(I); }
};

template<std::size_t... I> using index_sequence = integer_sequence<std::size_t, I...>;

namespace __ccw_seq {
template<class T, std::size_t N, T... I> struct gen : gen<T, N - 1, static_cast<T>(N - 1), I...> {};
template<class T, T... I>                struct gen<T, 0, I...> { typedef integer_sequence<T, I...> type; };
}

template<class T, T N> using make_integer_sequence =
    typename __ccw_seq::gen<T, static_cast<std::size_t>(N)>::type;
template<std::size_t N> using make_index_sequence  = make_integer_sequence<std::size_t, N>;
namespace __ccw_seq { template<class... T> struct count { static const std::size_t value = sizeof...(T); }; }
template<class... T>    using index_sequence_for   = typename __ccw_seq::gen<std::size_t, __ccw_seq::count<T...>::value>::type;

}   // _CCW_STD14
#endif

#if _CCW_STD_LIB_LT(1900, 201703L)
namespace _CCW_STD17 {
template<class T> inline const T& as_const(T& t) { return t; }
}   // _CCW_STD17
#endif

#if __cplusplus < 202002L
#include <type_traits>
#include <limits>
namespace _CCW_STD20 {
namespace __ccw_cmp {
    template<bool TS, bool US> struct less_impl;
    template<> struct less_impl<true, true>   { template<class T, class U> static bool f(T t, U u) { return t < u; } };
    template<> struct less_impl<false, false> { template<class T, class U> static bool f(T t, U u) { return t < u; } };
    template<> struct less_impl<true, false>  { template<class T, class U> static bool f(T t, U u) { return t < 0 ? true  : static_cast<typename std::make_unsigned<T>::type>(t) < u; } };
    template<> struct less_impl<false, true>  { template<class T, class U> static bool f(T t, U u) { return u < 0 ? false : t < static_cast<typename std::make_unsigned<U>::type>(u); } };
}
template<class T, class U> inline bool cmp_less(T t, U u) {
    return __ccw_cmp::less_impl<std::is_signed<T>::value, std::is_signed<U>::value>::f(t, u);
}
template<class T, class U> inline bool cmp_greater(T t, U u)       { return cmp_less(u, t); }
template<class T, class U> inline bool cmp_less_equal(T t, U u)    { return !cmp_less(u, t); }
template<class T, class U> inline bool cmp_greater_equal(T t, U u) { return !cmp_less(t, u); }
template<class T, class U> inline bool cmp_equal(T t, U u)         { return !cmp_less(t, u) && !cmp_less(u, t); }
template<class T, class U> inline bool cmp_not_equal(T t, U u)     { return !cmp_equal(t, u); }
template<class R, class T> inline bool in_range(T t) {
    return cmp_greater_equal(t, std::numeric_limits<R>::min())
        && cmp_less_equal(t, std::numeric_limits<R>::max());
}
}   // _CCW_STD20
#endif

#if __cplusplus < 202302L
#include <type_traits>
namespace _CCW_STD23 {
template<class E>
inline typename std::underlying_type<E>::type to_underlying(E e) {
    return static_cast<typename std::underlying_type<E>::type>(e);
}
}   // _CCW_STD23
#endif

#if defined(__cplusplus) && !defined(__cpp_lib_unreachable)
#include <cstdlib>
namespace _CCW_STD23 {
_ccw_noreturn inline void unreachable() {
#if defined(_MSC_VER)
    __assume(0);
#elif defined(__GNUC__) || defined(__clang__)
    __builtin_unreachable();
#else
    std::abort();
#endif
}
}   // _CCW_STD23
# define __cpp_lib_unreachable 202202L
#endif

#endif  // _CCW_DETAIL_UTILITY_HPP
