// -*- C++ -*-
//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception).
//  ccwrap llibcxx03: <stdbit.h> (C++26) for Open Watcom -- the bit-manipulation
//  utilities.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP_STDBIT_H
#define _CCW_LIBCPP_STDBIT_H
#include "__config"
#include <climits>
#include "../../c/bit_impl.h"

#ifndef __STDC_VERSION_STDBIT_H__
#define __STDC_VERSION_STDBIT_H__ 202311L
#endif

#define __STDC_ENDIAN_LITTLE__ 1234
#define __STDC_ENDIAN_BIG__    4321
#define __STDC_ENDIAN_NATIVE__ __STDC_ENDIAN_LITTLE__   /* Open Watcom targets are little-endian */

#define _CCW_STD_STDBIT_DEFINED   /* same set as detail/cxx/stdbit.hpp; keeps the two from colliding */

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <unsigned _Sz> struct __ccw_bitsel;

#define _CCW_BITSEL(_Sz, _Ut, _Nm)                                                    \
    template <> struct __ccw_bitsel<_Sz> {                                            \
        typedef _Ut _Up;                                                              \
        _CCW_LIBCPP_HIDE_FROM_ABI static unsigned int __clz(_Up __x) { return _ccw_stdbit_countl_zero_##_Nm(__x); } \
        _CCW_LIBCPP_HIDE_FROM_ABI static unsigned int __clo(_Up __x) { return _ccw_stdbit_countl_one_##_Nm(__x); }  \
        _CCW_LIBCPP_HIDE_FROM_ABI static unsigned int __ctz(_Up __x) { return _ccw_stdbit_countr_zero_##_Nm(__x); } \
        _CCW_LIBCPP_HIDE_FROM_ABI static unsigned int __cto(_Up __x) { return _ccw_stdbit_countr_one_##_Nm(__x); }  \
        _CCW_LIBCPP_HIDE_FROM_ABI static unsigned int __pop(_Up __x) { return _ccw_stdbit_popcount_##_Nm(__x); }    \
        _CCW_LIBCPP_HIDE_FROM_ABI static bool         __hsb(_Up __x) { return _ccw_stdbit_has_single_bit_##_Nm(__x) != 0; } \
        _CCW_LIBCPP_HIDE_FROM_ABI static unsigned int __bw(_Up __x)  { return _ccw_stdbit_bit_width_##_Nm(__x); }   \
        _CCW_LIBCPP_HIDE_FROM_ABI static _Up          __bf(_Up __x)  { return (_Up)_ccw_stdbit_bit_floor_##_Nm(__x); } \
        _CCW_LIBCPP_HIDE_FROM_ABI static _Up          __bc(_Up __x)  { return (_Up)_ccw_stdbit_bit_ceil_##_Nm(__x); }  \
    };
_CCW_BITSEL(1, _ccw_uint8,  u8)
_CCW_BITSEL(2, _ccw_uint16, u16)
_CCW_BITSEL(4, _ccw_uint32, u32)
_CCW_BITSEL(8, _ccw_uint64, u64)
#undef _CCW_BITSEL

#define _CCW_STDBIT_SEL(_Tp) __ccw_bitsel<sizeof(_Tp)>
#define _CCW_STDBIT_W(_Tp)   ((unsigned int)(sizeof(_Tp) * CHAR_BIT))

template <class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI unsigned int stdc_leading_zeros(_Tp __x)
{ return _CCW_STDBIT_SEL(_Tp)::__clz((_CCW_STDBIT_SEL(_Tp)::_Up)__x); }
template <class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI unsigned int stdc_leading_ones(_Tp __x)
{ return _CCW_STDBIT_SEL(_Tp)::__clo((_CCW_STDBIT_SEL(_Tp)::_Up)__x); }
template <class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI unsigned int stdc_trailing_zeros(_Tp __x)
{ return _CCW_STDBIT_SEL(_Tp)::__ctz((_CCW_STDBIT_SEL(_Tp)::_Up)__x); }
template <class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI unsigned int stdc_trailing_ones(_Tp __x)
{ return _CCW_STDBIT_SEL(_Tp)::__cto((_CCW_STDBIT_SEL(_Tp)::_Up)__x); }
template <class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI unsigned int stdc_count_ones(_Tp __x)
{ return _CCW_STDBIT_SEL(_Tp)::__pop((_CCW_STDBIT_SEL(_Tp)::_Up)__x); }
template <class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI unsigned int stdc_count_zeros(_Tp __x)
{ return _CCW_STDBIT_W(_Tp) - stdc_count_ones(__x); }
template <class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI unsigned int stdc_first_leading_one(_Tp __x)
{ unsigned int __n = stdc_leading_zeros(__x); return __n == _CCW_STDBIT_W(_Tp) ? 0 : __n + 1; }
template <class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI unsigned int stdc_first_leading_zero(_Tp __x)
{ unsigned int __n = stdc_leading_ones(__x); return __n == _CCW_STDBIT_W(_Tp) ? 0 : __n + 1; }
template <class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI unsigned int stdc_first_trailing_one(_Tp __x)
{ unsigned int __n = stdc_trailing_zeros(__x); return __n == _CCW_STDBIT_W(_Tp) ? 0 : __n + 1; }
template <class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI unsigned int stdc_first_trailing_zero(_Tp __x)
{ unsigned int __n = stdc_trailing_ones(__x); return __n == _CCW_STDBIT_W(_Tp) ? 0 : __n + 1; }
template <class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI bool stdc_has_single_bit(_Tp __x)
{ return _CCW_STDBIT_SEL(_Tp)::__hsb((_CCW_STDBIT_SEL(_Tp)::_Up)__x); }
template <class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI unsigned int stdc_bit_width(_Tp __x)
{ return _CCW_STDBIT_SEL(_Tp)::__bw((_CCW_STDBIT_SEL(_Tp)::_Up)__x); }
template <class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI _Tp stdc_bit_floor(_Tp __x)
{ return (_Tp)_CCW_STDBIT_SEL(_Tp)::__bf((_CCW_STDBIT_SEL(_Tp)::_Up)__x); }
template <class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI _Tp stdc_bit_ceil(_Tp __x)
{ return (_Tp)_CCW_STDBIT_SEL(_Tp)::__bc((_CCW_STDBIT_SEL(_Tp)::_Up)__x); }

#define _CCW_STDBIT_TYPED(op)                                                                 \
    _CCW_LIBCPP_HIDE_FROM_ABI inline unsigned int op##_uc(unsigned char __x)      { return op(__x); } \
    _CCW_LIBCPP_HIDE_FROM_ABI inline unsigned int op##_us(unsigned short __x)     { return op(__x); } \
    _CCW_LIBCPP_HIDE_FROM_ABI inline unsigned int op##_ui(unsigned int __x)       { return op(__x); } \
    _CCW_LIBCPP_HIDE_FROM_ABI inline unsigned int op##_ul(unsigned long __x)      { return op(__x); } \
    _CCW_LIBCPP_HIDE_FROM_ABI inline unsigned int op##_ull(unsigned long long __x){ return op(__x); }
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

#define _CCW_STDBIT_TYPED2(_Sfx, _Tp)                                                          \
    _CCW_LIBCPP_HIDE_FROM_ABI inline bool stdc_has_single_bit_##_Sfx(_Tp __x) { return stdc_has_single_bit(__x); } \
    _CCW_LIBCPP_HIDE_FROM_ABI inline _Tp  stdc_bit_floor_##_Sfx(_Tp __x)      { return stdc_bit_floor(__x); }      \
    _CCW_LIBCPP_HIDE_FROM_ABI inline _Tp  stdc_bit_ceil_##_Sfx(_Tp __x)       { return stdc_bit_ceil(__x); }
_CCW_STDBIT_TYPED2(uc,  unsigned char)
_CCW_STDBIT_TYPED2(us,  unsigned short)
_CCW_STDBIT_TYPED2(ui,  unsigned int)
_CCW_STDBIT_TYPED2(ul,  unsigned long)
_CCW_STDBIT_TYPED2(ull, unsigned long long)
#undef _CCW_STDBIT_TYPED2

_CCW_LIBCPP_END_NAMESPACE_STD

#ifndef _CCW_STDBIT_GLOBAL_USING
#define _CCW_STDBIT_GLOBAL_USING
using _CCW_STD::stdc_leading_zeros;       using _CCW_STD::stdc_leading_ones;
using _CCW_STD::stdc_trailing_zeros;      using _CCW_STD::stdc_trailing_ones;
using _CCW_STD::stdc_first_leading_zero;  using _CCW_STD::stdc_first_leading_one;
using _CCW_STD::stdc_first_trailing_zero; using _CCW_STD::stdc_first_trailing_one;
using _CCW_STD::stdc_count_zeros;         using _CCW_STD::stdc_count_ones;
using _CCW_STD::stdc_has_single_bit;      using _CCW_STD::stdc_bit_width;
using _CCW_STD::stdc_bit_floor;           using _CCW_STD::stdc_bit_ceil;
#define _CCW_STDBIT_USING_TYPED(op) \
    using _CCW_STD::op##_uc; using _CCW_STD::op##_us; using _CCW_STD::op##_ui; using _CCW_STD::op##_ul; using _CCW_STD::op##_ull;
_CCW_STDBIT_USING_TYPED(stdc_leading_zeros)
_CCW_STDBIT_USING_TYPED(stdc_leading_ones)
_CCW_STDBIT_USING_TYPED(stdc_trailing_zeros)
_CCW_STDBIT_USING_TYPED(stdc_trailing_ones)
_CCW_STDBIT_USING_TYPED(stdc_first_leading_zero)
_CCW_STDBIT_USING_TYPED(stdc_first_leading_one)
_CCW_STDBIT_USING_TYPED(stdc_first_trailing_zero)
_CCW_STDBIT_USING_TYPED(stdc_first_trailing_one)
_CCW_STDBIT_USING_TYPED(stdc_count_zeros)
_CCW_STDBIT_USING_TYPED(stdc_count_ones)
_CCW_STDBIT_USING_TYPED(stdc_bit_width)
_CCW_STDBIT_USING_TYPED(stdc_has_single_bit)
_CCW_STDBIT_USING_TYPED(stdc_bit_floor)
_CCW_STDBIT_USING_TYPED(stdc_bit_ceil)
#undef _CCW_STDBIT_USING_TYPED
#endif  /* _CCW_STDBIT_GLOBAL_USING */

#endif // _CCW_LIBCPP_STDBIT_H
