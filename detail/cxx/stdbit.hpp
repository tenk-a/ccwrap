/**
 *  @file   detail/stdbit.hpp
 *  @brief  <stdbit.h> (C++26) gap-fill: the std::stdc_* bit-manipulation utilities.
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_DETAIL_STDBIT_HPP
#define _CCW_DETAIL_STDBIT_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include <climits>

#ifndef _CCW_STD_STDBIT_DEFINED
#define _CCW_STD_STDBIT_DEFINED

namespace std {

template <class T> inline unsigned int __ccw_bit_width_of(T) { return (unsigned int)(sizeof(T) * CHAR_BIT); }

template <class T> inline unsigned int stdc_leading_zeros(T x) {
    unsigned int w = __ccw_bit_width_of(x), n = 0;
    for (unsigned int i = 0; i < w; ++i) { if ((x >> (w - 1 - i)) & 1) break; ++n; }
    return n;
}
template <class T> inline unsigned int stdc_leading_ones(T x) {
    unsigned int w = __ccw_bit_width_of(x), n = 0;
    for (unsigned int i = 0; i < w; ++i) { if (!((x >> (w - 1 - i)) & 1)) break; ++n; }
    return n;
}
template <class T> inline unsigned int stdc_trailing_zeros(T x) {
    unsigned int w = __ccw_bit_width_of(x), n = 0;
    for (unsigned int i = 0; i < w; ++i) { if ((x >> i) & 1) break; ++n; }
    return n;
}
template <class T> inline unsigned int stdc_trailing_ones(T x) {
    unsigned int w = __ccw_bit_width_of(x), n = 0;
    for (unsigned int i = 0; i < w; ++i) { if (!((x >> i) & 1)) break; ++n; }
    return n;
}
template <class T> inline unsigned int stdc_count_ones(T x) {
    unsigned int w = __ccw_bit_width_of(x), n = 0;
    for (unsigned int i = 0; i < w; ++i) if ((x >> i) & 1) ++n;
    return n;
}
template <class T> inline unsigned int stdc_count_zeros(T x) { return __ccw_bit_width_of(x) - stdc_count_ones(x); }
template <class T> inline unsigned int stdc_first_leading_one(T x)   { return x ? stdc_leading_zeros(x) + 1 : 0; }
template <class T> inline unsigned int stdc_first_leading_zero(T x)  { unsigned int lo = stdc_leading_ones(x);  return lo == __ccw_bit_width_of(x) ? 0 : lo + 1; }
template <class T> inline unsigned int stdc_first_trailing_one(T x)  { return x ? stdc_trailing_zeros(x) + 1 : 0; }
template <class T> inline unsigned int stdc_first_trailing_zero(T x) { unsigned int to = stdc_trailing_ones(x); return to == __ccw_bit_width_of(x) ? 0 : to + 1; }
template <class T> inline bool         stdc_has_single_bit(T x) { return x != 0 && (T)(x & (T)(x - 1)) == 0; }
template <class T> inline unsigned int stdc_bit_width(T x) { return __ccw_bit_width_of(x) - stdc_leading_zeros(x); }
template <class T> inline T            stdc_bit_floor(T x) { if (!x) return 0; return (T)((T)1 << (stdc_bit_width(x) - 1)); }
template <class T> inline T            stdc_bit_ceil(T x)  { if (x <= 1) return 1; return (T)((T)1 << stdc_bit_width((T)(x - 1))); }

#define _CCW_STDBIT_TYPED(op)                                                     \
    inline unsigned int op##_uc(unsigned char x)       { return op(x); }          \
    inline unsigned int op##_us(unsigned short x)      { return op(x); }          \
    inline unsigned int op##_ui(unsigned int x)        { return op(x); }          \
    inline unsigned int op##_ul(unsigned long x)       { return op(x); }          \
    inline unsigned int op##_ull(_ccw_ullong x)        { return op(x); }
_CCW_STDBIT_TYPED(stdc_leading_zeros)
_CCW_STDBIT_TYPED(stdc_leading_ones)
_CCW_STDBIT_TYPED(stdc_trailing_zeros)
_CCW_STDBIT_TYPED(stdc_trailing_ones)
_CCW_STDBIT_TYPED(stdc_first_leading_zero)
_CCW_STDBIT_TYPED(stdc_first_leading_one)
_CCW_STDBIT_TYPED(stdc_first_trailing_zero)
_CCW_STDBIT_TYPED(stdc_first_trailing_one)
_CCW_STDBIT_TYPED(stdc_count_zeros)
_CCW_STDBIT_TYPED(stdc_count_ones)
_CCW_STDBIT_TYPED(stdc_bit_width)
#undef _CCW_STDBIT_TYPED

}   // namespace std

#endif  // _CCW_STD_STDBIT_DEFINED
#endif  // _CCW_DETAIL_STDBIT_HPP
