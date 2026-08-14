// ccwrap <stdbit.h>
#pragma once

#include <ccwrap_common.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#if defined(_MSC_VER)
 #include <intrin.h>
#endif

#define __STDC_VERSION_STDBIT_H__ 202311L

#define __STDC_ENDIAN_LITTLE__ 1234
#define __STDC_ENDIAN_BIG__    4321
#if _CCW_ENDIAN
 #define __STDC_ENDIAN_NATIVE__ __STDC_ENDIAN_BIG__
#else
 #define __STDC_ENDIAN_NATIVE__ __STDC_ENDIAN_LITTLE__
#endif

#include "../../detail/c/bit_impl.h"

#define _CCW_STDBIT_DEFINE_SUFFIX(suffix, type, utype, width, name) \
static _ccw_forceinline unsigned int stdc_leading_zeros_##suffix(type value) \
{ return _ccw_stdbit_countl_zero_##name((utype)value); } \
static _ccw_forceinline unsigned int stdc_leading_ones_##suffix(type value) \
{ return _ccw_stdbit_countl_one_##name((utype)value); } \
static _ccw_forceinline unsigned int stdc_trailing_zeros_##suffix(type value) \
{ return _ccw_stdbit_countr_zero_##name((utype)value); } \
static _ccw_forceinline unsigned int stdc_trailing_ones_##suffix(type value) \
{ return _ccw_stdbit_countr_one_##name((utype)value); } \
static _ccw_forceinline unsigned int stdc_first_leading_zero_##suffix(type value) \
{ unsigned int n = _ccw_stdbit_countl_one_##name((utype)value); return n == width ? 0 : n + 1; } \
static _ccw_forceinline unsigned int stdc_first_leading_one_##suffix(type value) \
{ unsigned int n = _ccw_stdbit_countl_zero_##name((utype)value); return n == width ? 0 : n + 1; } \
static _ccw_forceinline unsigned int stdc_first_trailing_zero_##suffix(type value) \
{ unsigned int n = _ccw_stdbit_countr_one_##name((utype)value); return n == width ? 0 : n + 1; } \
static _ccw_forceinline unsigned int stdc_first_trailing_one_##suffix(type value) \
{ unsigned int n = _ccw_stdbit_countr_zero_##name((utype)value); return n == width ? 0 : n + 1; } \
static _ccw_forceinline unsigned int stdc_count_ones_##suffix(type value) \
{ return _ccw_stdbit_popcount_##name((utype)value); } \
static _ccw_forceinline unsigned int stdc_count_zeros_##suffix(type value) \
{ return width - _ccw_stdbit_popcount_##name((utype)value); } \
static _ccw_forceinline bool stdc_has_single_bit_##suffix(type value) \
{ return _ccw_stdbit_has_single_bit_##name((utype)value); } \
static _ccw_forceinline unsigned int stdc_bit_width_##suffix(type value) \
{ return _ccw_stdbit_bit_width_##name((utype)value); } \
static _ccw_forceinline type stdc_bit_floor_##suffix(type value) \
{ return (type)_ccw_stdbit_bit_floor_##name((utype)value); } \
static _ccw_forceinline type stdc_bit_ceil_##suffix(type value) \
{ return (type)_ccw_stdbit_bit_ceil_##name((utype)value); }

_CCW_STDBIT_DEFINE_SUFFIX(uc,  unsigned char,  uint8_t,  8,  u8)
_CCW_STDBIT_DEFINE_SUFFIX(us,  unsigned short, uint16_t, 16, u16)
_CCW_STDBIT_DEFINE_SUFFIX(ui,  unsigned int,   uint32_t, 32, u32)
_CCW_STDBIT_DEFINE_SUFFIX(ul,  unsigned long,  uint32_t, 32, u32)
_CCW_STDBIT_DEFINE_SUFFIX(ull, _ccw_ullong,    uint64_t, 64, u64)

#define _CCW_STDBIT_SELECT(value, fn) \
    (sizeof(value) <= 1 ? fn##_u8( (uint8_t )(value)) : \
     sizeof(value) <= 2 ? fn##_u16((uint16_t)(value)) : \
     sizeof(value) <= 4 ? fn##_u32((uint32_t)(value)) : \
                          fn##_u64((uint64_t)(value)))

#define _CCW_STDBIT_WIDTH(value) \
    ((unsigned int)(sizeof(value) <= 1 ? 8 : sizeof(value) <= 2 ? 16 : sizeof(value) <= 4 ? 32 : 64))

#ifdef __cplusplus

#include <../../detail/cxx/stdbit.hpp>

#else

#define stdc_leading_zeros(value)       _CCW_STDBIT_SELECT( value, _ccw_stdbit_countl_zero)
#define stdc_leading_ones(value)        _CCW_STDBIT_SELECT( value, _ccw_stdbit_countl_one)
#define stdc_trailing_zeros(value)      _CCW_STDBIT_SELECT( value, _ccw_stdbit_countr_zero)
#define stdc_trailing_ones(value)       _CCW_STDBIT_SELECT( value, _ccw_stdbit_countr_one)
#define stdc_first_leading_zero(value)  (stdc_leading_ones( value) == _CCW_STDBIT_WIDTH(value) ? 0 : stdc_leading_ones(value) + 1)
#define stdc_first_leading_one(value)   (stdc_leading_zeros(value) == _CCW_STDBIT_WIDTH(value) ? 0 : stdc_leading_zeros(value) + 1)
#define stdc_first_trailing_zero(value) (stdc_trailing_ones(value) == _CCW_STDBIT_WIDTH(value) ? 0 : stdc_trailing_ones(value) + 1)
#define stdc_first_trailing_one(value)  (stdc_trailing_zeros(value)== _CCW_STDBIT_WIDTH(value) ? 0 : stdc_trailing_zeros(value) + 1)
#define stdc_count_ones(value)          _CCW_STDBIT_SELECT( value, _ccw_stdbit_popcount)
#define stdc_count_zeros(value)         (_CCW_STDBIT_WIDTH( value) - stdc_count_ones(value))
#define stdc_has_single_bit(value)      _CCW_STDBIT_SELECT( value, _ccw_stdbit_has_single_bit)
#define stdc_bit_width(value)           _CCW_STDBIT_SELECT( value, _ccw_stdbit_bit_width)
#define stdc_bit_floor(value)           _CCW_STDBIT_SELECT( value, _ccw_stdbit_bit_floor)
#define stdc_bit_ceil(value)            _CCW_STDBIT_SELECT( value, _ccw_stdbit_bit_ceil)

#endif  /* __cplusplus */
