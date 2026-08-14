/**
 *  @file   detail/ratio.hpp
 *  @brief  std::ratio backport for pre-C++11 compilers (vc8/9, or gcc/clang at
 *          -std=c++03). Compile-time rational arithmetic; all C++03-expressible.
 *  @license Boost Software License Version 1.0
 *  @note   The forwarder (vc/ratio, gcc/ratio) includes this only where the native
 *          library lacks <ratio>, so the whole file lives in namespace std.
 */
#ifndef _CCW_DETAIL_RATIO_HPP
#define _CCW_DETAIL_RATIO_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include <cstdint>

namespace std {

namespace __ccw_r {
    template<intmax_t A> struct iabs { static const intmax_t value = A < 0 ? -A : A; };
    template<intmax_t A, intmax_t B> struct gcd { static const intmax_t value = gcd<B, A % B>::value; };
    template<intmax_t A> struct gcd<A, 0> { static const intmax_t value = iabs<A>::value; };
}

template<intmax_t N, intmax_t D = 1>
struct ratio {
private:
    static const intmax_t _g = __ccw_r::gcd<__ccw_r::iabs<N>::value, __ccw_r::iabs<D>::value>::value;
    static const intmax_t _s = (D < 0) ? -1 : 1;
public:
    static const intmax_t num = _s * N / _g;
    static const intmax_t den = _s * D / _g;
    typedef ratio<num, den> type;
};

template<class R1, class R2> struct ratio_multiply { typedef typename ratio<R1::num * R2::num, R1::den * R2::den>::type type; };
template<class R1, class R2> struct ratio_divide   { typedef typename ratio<R1::num * R2::den, R1::den * R2::num>::type type; };
template<class R1, class R2> struct ratio_add      { typedef typename ratio<R1::num * R2::den + R2::num * R1::den, R1::den * R2::den>::type type; };
template<class R1, class R2> struct ratio_subtract { typedef typename ratio<R1::num * R2::den - R2::num * R1::den, R1::den * R2::den>::type type; };

template<class R1, class R2> struct ratio_equal     { static const bool value = (R1::num == R2::num && R1::den == R2::den); };
template<class R1, class R2> struct ratio_not_equal { static const bool value = !ratio_equal<R1, R2>::value; };
template<class R1, class R2> struct ratio_less      { static const bool value = (R1::num * R2::den < R2::num * R1::den); };
template<class R1, class R2> struct ratio_less_equal    { static const bool value = !(R2::num * R1::den < R1::num * R2::den); };
template<class R1, class R2> struct ratio_greater       { static const bool value = (R2::num * R1::den < R1::num * R2::den); };
template<class R1, class R2> struct ratio_greater_equal { static const bool value = !(R1::num * R2::den < R2::num * R1::den); };

typedef ratio<1, 1000000000000000000LL> atto;
typedef ratio<1, 1000000000000000LL>    femto;
typedef ratio<1, 1000000000000LL>       pico;
typedef ratio<1, 1000000000LL>          nano;
typedef ratio<1, 1000000LL>             micro;
typedef ratio<1, 1000LL>                milli;
typedef ratio<1, 100LL>                 centi;
typedef ratio<1, 10LL>                  deci;
typedef ratio<10LL, 1>                  deca;
typedef ratio<100LL, 1>                 hecto;
typedef ratio<1000LL, 1>                kilo;
typedef ratio<1000000LL, 1>             mega;
typedef ratio<1000000000LL, 1>          giga;
typedef ratio<1000000000000LL, 1>       tera;
typedef ratio<1000000000000000LL, 1>    peta;
typedef ratio<1000000000000000000LL, 1> exa;

}   // namespace std

#endif  // _CCW_DETAIL_RATIO_HPP
