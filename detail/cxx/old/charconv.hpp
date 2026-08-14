/**
 *  @file   _ccwrap_detail/charconv.hpp
 *  @brief  std::to_chars / std::from_chars backport for pre-C++17 VC (all VC).
 *  @license Boost Software License Version 1.0
 *  @note
 *      Integer to_chars/from_chars are complete (base 2..36, range-checked).
 *      Floating point is best effort via the CRT: to_chars(no format) finds the
 *      shortest round-tripping "%.*g" string; to_chars(format[,precision]) maps
 *      to printf; from_chars uses strtod (skips leading whitespace and accepts a
 *      few extra spellings, unlike the strict standard grammar).
 *
 *      std::errc is a real type on every VC thanks to the <system_error> wrapper
 *      (native enum class on vc12+, a struct on vc8..11), so to_chars_result::ec
 *      is literally std::errc. chars_format still needs the namespace-enum trick
 *      below because enum class is unavailable before vc11 (< _MSC_VER 1700).
 */
#ifndef _CCW_DETAIL_CHARCONV_HPP
#define _CCW_DETAIL_CHARCONV_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <limits>
#include <type_traits>

// --- std::errc: the <system_error> wrapper makes it a real type on every VC ---
#include <system_error>

// --- std::chars_format ---
#if _CCW_HAS_ENUM_CLASS
namespace _CCW_STD17 {
enum class chars_format { scientific = 1, fixed = 2, hex = 4, general = 3 /* fixed|scientific */ };
inline chars_format operator|(chars_format a, chars_format b) { return chars_format(int(a) | int(b)); }
inline chars_format operator&(chars_format a, chars_format b) { return chars_format(int(a) & int(b)); }
inline chars_format operator^(chars_format a, chars_format b) { return chars_format(int(a) ^ int(b)); }
}   // _CCW_STD17
# define _CCW_CHARS_FMT  ::_CCW_STD17::chars_format
#else
namespace _CCW_STD17 { namespace chars_format {
    enum chars_format { scientific = 1, fixed = 2, hex = 4, general = 3 };
} }
# define _CCW_CHARS_FMT  ::_CCW_STD17::chars_format::chars_format
#endif

#if defined(_MSC_VER)
# pragma warning(push)
# pragma warning(disable:4127)  /* conditional expression is constant (is_signed<T>) */
#endif

namespace _CCW_STD17 {
using namespace ::std;

struct to_chars_result   { char* ptr;       ::std::errc ec; };
struct from_chars_result { const char* ptr; ::std::errc ec; };

namespace __ccw_cc {

inline char digit(unsigned d) { return d < 10 ? char('0' + d) : char('a' + (d - 10)); }
inline int  dval(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'z') return c - 'a' + 10;
    if (c >= 'A' && c <= 'Z') return c - 'A' + 10;
    return -1;
}

// magnitude of a signed/unsigned value (no signed/unsigned warnings per branch)
template<bool Signed> struct mag_h {
    template<class Int, class U> static U mag(Int v, bool& neg) { neg = (v < 0); return neg ? U(U(0) - U(v)) : U(v); }
};
template<> struct mag_h<false> {
    template<class Int, class U> static U mag(Int v, bool& neg) { neg = false; return U(v); }
};

// shortest round-tripping "%.*g" (maxprec = 9 for float, 17 for double)
inline to_chars_result shortest(char* first, char* last, double v, int maxprec, bool as_float) {
    char tmp[512];
    int n = 0;
    for (int prec = 1; prec <= maxprec; ++prec) {
        n = snprintf(tmp, sizeof(tmp), "%.*g", prec, v);
        if (n <= 0) break;
        double rt = strtod(tmp, 0);
        if (as_float ? (float(rt) == float(v)) : (rt == v)) break;
    }
    if (n <= 0) n = snprintf(tmp, sizeof(tmp), "%.*g", maxprec, v);
    to_chars_result r;
    if (n <= 0 || n > (last - first)) { r.ptr = last; r.ec = errc::value_too_large; return r; }
    std::memcpy(first, tmp, std::size_t(n));
    r.ptr = first + n; r.ec = ::std::errc();
    return r;
}

inline to_chars_result fp_fmt(char* first, char* last, double v, char conv, int prec, bool hasprec) {
    char f[16]; char tmp[512];
    if (hasprec) std::sprintf(f, "%%.%d%c", prec, conv);
    else         std::sprintf(f, "%%%c", conv);
    int n = snprintf(tmp, sizeof(tmp), f, v);
    to_chars_result r;
    if (n <= 0 || n > (last - first)) { r.ptr = last; r.ec = errc::value_too_large; return r; }
    std::memcpy(first, tmp, std::size_t(n));
    r.ptr = first + n; r.ec = ::std::errc();
    return r;
}

inline char conv_of(_CCW_CHARS_FMT fmt) {
    if (fmt == chars_format::scientific) return 'e';
    if (fmt == chars_format::fixed)      return 'f';
    if (fmt == chars_format::hex)        return 'a';
    return 'g';
}

inline from_chars_result parse_fp(const char* first, const char* last, double& out) {
    char tmp[512];
    std::size_t n = std::size_t(last - first);
    if (n >= sizeof(tmp)) n = sizeof(tmp) - 1;
    std::memcpy(tmp, first, n);
    tmp[n] = 0;
    errno = 0;
    char* end = 0;
    double v = strtod(tmp, &end);
    from_chars_result r;
    if (end == tmp) { r.ptr = first; r.ec = errc::invalid_argument; return r; }
    r.ptr = first + (end - tmp);
    if (errno == ERANGE) r.ec = errc::result_out_of_range;
    else { out = v; r.ec = ::std::errc(); }
    return r;
}

}   // namespace __ccw_cc

// --- integer to_chars --------------------------------------------------------
template<class Int>
inline to_chars_result to_chars(char* first, char* last, Int value, int base) {
    typedef typename make_unsigned<Int>::type U;
    bool neg = false;
    U mag = __ccw_cc::mag_h<is_signed<Int>::value>::template mag<Int, U>(value, neg);
    char tmp[sizeof(U) * 8 + 1];
    int n = 0;
    do { tmp[n++] = __ccw_cc::digit(unsigned(mag % U(base))); mag /= U(base); } while (mag != 0);
    int total = n + (neg ? 1 : 0);
    to_chars_result r;
    if (total > (last - first)) { r.ptr = last; r.ec = errc::value_too_large; return r; }
    char* d = first;
    if (neg) *d++ = '-';
    while (n) *d++ = tmp[--n];
    r.ptr = d; r.ec = ::std::errc();
    return r;
}
template<class Int>
inline to_chars_result to_chars(char* first, char* last, Int value) { return to_chars(first, last, value, 10); }

// --- integer from_chars ------------------------------------------------------
template<class Int>
inline from_chars_result from_chars(const char* first, const char* last, Int& value, int base) {
    typedef typename make_unsigned<Int>::type U;
    const char* p = first;
    bool neg = false;
    if (is_signed<Int>::value) { if (p != last && *p == '-') { neg = true; ++p; } }
    U acc = 0;
    U umax = (std::numeric_limits<U>::max)();
    bool any = false, over = false;
    const char* q = p;
    for (; q != last; ++q) {
        int dg = __ccw_cc::dval(*q);
        if (dg < 0 || dg >= base) break;
        any = true;
        if (acc > (umax - U(dg)) / U(base)) over = true;
        else acc = acc * U(base) + U(dg);
    }
    from_chars_result r;
    if (!any) { r.ptr = first; r.ec = errc::invalid_argument; return r; }
    if (!over && is_signed<Int>::value) {
        U maxpos = U((std::numeric_limits<Int>::max)());
        if (neg) { if (acc > maxpos + U(1)) over = true; }
        else     { if (acc > maxpos)        over = true; }
    }
    r.ptr = q;
    if (over) { r.ec = errc::result_out_of_range; }
    else {
        value = neg ? static_cast<Int>(U(0) - acc) : static_cast<Int>(acc);
        r.ec = ::std::errc();
    }
    return r;
}
template<class Int>
inline from_chars_result from_chars(const char* first, const char* last, Int& value) { return from_chars(first, last, value, 10); }

// --- floating point to_chars (best effort) -----------------------------------
inline to_chars_result to_chars(char* first, char* last, double value) { return __ccw_cc::shortest(first, last, value, 17, false); }
inline to_chars_result to_chars(char* first, char* last, float value)  { return __ccw_cc::shortest(first, last, double(value), 9, true); }
inline to_chars_result to_chars(char* first, char* last, double value, _CCW_CHARS_FMT fmt) {
    if (fmt == chars_format::general) return __ccw_cc::shortest(first, last, value, 17, false);
    return __ccw_cc::fp_fmt(first, last, value, __ccw_cc::conv_of(fmt), 0, false);
}
inline to_chars_result to_chars(char* first, char* last, float value, _CCW_CHARS_FMT fmt) {
    if (fmt == chars_format::general) return __ccw_cc::shortest(first, last, double(value), 9, true);
    return __ccw_cc::fp_fmt(first, last, double(value), __ccw_cc::conv_of(fmt), 0, false);
}
inline to_chars_result to_chars(char* first, char* last, double value, _CCW_CHARS_FMT fmt, int precision) {
    return __ccw_cc::fp_fmt(first, last, value, __ccw_cc::conv_of(fmt), precision, true);
}
inline to_chars_result to_chars(char* first, char* last, float value, _CCW_CHARS_FMT fmt, int precision) {
    return __ccw_cc::fp_fmt(first, last, double(value), __ccw_cc::conv_of(fmt), precision, true);
}

// --- floating point from_chars (best effort) ---------------------------------
inline from_chars_result from_chars(const char* first, const char* last, double& value, _CCW_CHARS_FMT = chars_format::general) {
    return __ccw_cc::parse_fp(first, last, value);
}
inline from_chars_result from_chars(const char* first, const char* last, float& value, _CCW_CHARS_FMT = chars_format::general) {
    double d = 0;
    from_chars_result r = __ccw_cc::parse_fp(first, last, d);
    if (r.ec == ::std::errc()) value = float(d);
    return r;
}

}   // _CCW_STD17

namespace _ccw {
    using _CCW_STD17::to_chars_result;  using _CCW_STD17::from_chars_result;
    using _CCW_STD17::to_chars;         using _CCW_STD17::from_chars;
#if _CCW_HAS_ENUM_CLASS
    using _CCW_STD17::chars_format;                 /* enum class */
#elif _CCW_TARGET_CXX >= 2017
    namespace chars_format = _CCW_STD17::chars_format;  /* namespace-enum, _CCW_STD17 == std */
    /* else: _CCW_STD17 == _ccw, chars_format namespace-enum already lives in _ccw */
#endif
}

#if defined(_MSC_VER)
# pragma warning(pop)
#endif

#undef _CCW_CHARS_FMT

#endif  // _CCW_DETAIL_CHARCONV_HPP
