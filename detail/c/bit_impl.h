/**
 * @file bit_impl.h
 * @brief
 * @license Boost Software License Version 1.0
 */
#ifndef _CCW_BIT_IMPL_H___
#define _CCW_BIT_IMPL_H___

#include <ccwrap_common.h>
#if defined(_MSC_VER)
 #include <intrin.h>
 #define _CCW_BITIMPL_HAS_VC_BITSCAN_INTRINSIC 1
 #if _MSC_VER < 1500
  #pragma intrinsic(_BitScanForward)
  #pragma intrinsic(_BitScanReverse)
  #if defined(_M_X64) || defined(_M_AMD64)
   #pragma intrinsic(_BitScanForward64)
   #pragma intrinsic(_BitScanReverse64)
  #endif
 #endif
 #if _MSC_VER >= 1500
  #if defined(_M_IX86) || defined(_M_X64) || defined(_M_AMD64) || defined(_M_ARM64) || defined(_M_ARM64EC)
   #define _CCW_BITIMPL_HAS_VC_POPCNT_INTRINSIC 1
  #endif
 #endif
#elif defined(__WATCOMC__) && defined(__386__)
 #include <intrin.h>
 #define _CCW_BITIMPL_HAS_VC_BITSCAN_INTRINSIC 1
#endif

static _ccw_forceinline bool _ccw_stdbit_has_single_bit_u8(_ccw_uint8 value) {
    return value != 0 && (value & (_ccw_uint8)(value - 1)) == 0;
}

static _ccw_forceinline bool _ccw_stdbit_has_single_bit_u16(_ccw_uint16 value) {
    return value != 0 && (value & (_ccw_uint16)(value - 1)) == 0;
}

static _ccw_forceinline bool _ccw_stdbit_has_single_bit_u32(_ccw_uint32 value) {
    return value != 0 && (value & (_ccw_uint32)(value - 1)) == 0;
}

static _ccw_forceinline bool _ccw_stdbit_has_single_bit_u64(_ccw_uint64 value) {
    _ccw_uint64 t = value & (_ccw_uint64)(value - 1);
    return value != 0 && t == 0;
}

#if defined(_CCW_BITIMPL_HAS_VC_BITSCAN_INTRINSIC)
static _ccw_forceinline unsigned int _ccw_stdbit_countr_zero_u8(_ccw_uint8 value) {
    unsigned long index;
    return _BitScanForward(&index, (unsigned long)value) ? (unsigned int)index : 8;
}
#else
static inline unsigned int _ccw_stdbit_countr_zero_u8(_ccw_uint8 value) {
    unsigned int count = 0;
    if (value == 0)
        return 8;
    if ((value & 0x0f) == 0) { count += 4; value >>= 4; }
    if ((value & 0x03) == 0) { count += 2; value >>= 2; }
    return count + ((value & 0x01) ^ 1);
}
#endif

#if defined(_CCW_BITIMPL_HAS_VC_BITSCAN_INTRINSIC)
static _ccw_forceinline unsigned int _ccw_stdbit_countr_zero_u16(_ccw_uint16 value) {
    unsigned long index;
    return _BitScanForward(&index, (unsigned long)value) ? (unsigned int)index : 16;
}
#else
static inline unsigned int _ccw_stdbit_countr_zero_u16(_ccw_uint16 value) {
    unsigned int count = 0;
    if (value == 0)
        return 16;
    if ((value & 0x00ff) == 0) { count += 8; value >>= 8; }
    if ((value & 0x000f) == 0) { count += 4; value >>= 4; }
    if ((value & 0x0003) == 0) { count += 2; value >>= 2; }
    return count + ((value & 0x0001) ^ 1);
}
#endif

#if defined(_CCW_BITIMPL_HAS_VC_BITSCAN_INTRINSIC)
static _ccw_forceinline unsigned int _ccw_stdbit_countr_zero_u32(_ccw_uint32 value) {
    unsigned long index;
    return _BitScanForward(&index, (unsigned long)value) ? (unsigned int)index : 32;
}
#else
static _ccw_forceinline unsigned int _ccw_stdbit_countr_zero_u32(_ccw_uint32 value) {
    unsigned int count = 0;
    if (value == 0)
        return 32;
    if ((value & 0x0000ffffU) == 0) { count += 16; value >>= 16; }
    if ((value & 0x000000ffU) == 0) { count += 8; value >>= 8; }
    if ((value & 0x0000000fU) == 0) { count += 4; value >>= 4; }
    if ((value & 0x00000003U) == 0) { count += 2; value >>= 2; }
    return count + ((value & 0x00000001U) ^ 1);
}
#endif

#if defined(_CCW_BITIMPL_HAS_VC_BITSCAN_INTRINSIC)
#if defined(_M_X64) || defined(_M_AMD64) || defined(_M_ARM64) || defined(_M_ARM64EC)
static _ccw_forceinline unsigned int _ccw_stdbit_countr_zero_u64(_ccw_uint64 value) {
    unsigned long index;
    if (value == 0)
        return 64;
    _BitScanForward64(&index, (unsigned __int64)value);
    return (unsigned int)index;
}
#else
static inline unsigned int _ccw_stdbit_countr_zero_u64(_ccw_uint64 value) {
    unsigned long index;
    _ccw_uint32 lo;
    if (value == 0)
        return 64;
    lo = (_ccw_uint32)value;
    if (_BitScanForward(&index, (unsigned long)lo))
        return (unsigned int)index;
    _BitScanForward(&index, (unsigned long)(value >> 32));
    return (unsigned int)index + 32;
}
#endif
#else
static _ccw_forceinline unsigned int _ccw_stdbit_countr_zero_u64(_ccw_uint64 value) {
    unsigned int count = 0;
    _ccw_uint64 t;
    if (value == 0)
        return 64;
    t = value & 0x00000000ffffffffULL; if (t == 0) { count += 32; value >>= 32; }
    t = value & 0x000000000000ffffULL; if (t == 0) { count += 16; value >>= 16; }
    t = value & 0x00000000000000ffULL; if (t == 0) { count += 8; value >>= 8; }
    t = value & 0x000000000000000fULL; if (t == 0) { count += 4; value >>= 4; }
    t = value & 0x0000000000000003ULL; if (t == 0) { count += 2; value >>= 2; }
    t = value & 0x0000000000000001ULL;
    return count + (unsigned int)(t ^ 1);
}
#endif

static _ccw_forceinline unsigned int _ccw_stdbit_countr_one_u8(_ccw_uint8 value) {
    return _ccw_stdbit_countr_zero_u8((_ccw_uint8)~value);
}

static _ccw_forceinline unsigned int _ccw_stdbit_countr_one_u16(_ccw_uint16 value) {
    return _ccw_stdbit_countr_zero_u16((_ccw_uint16)~value);
}

static _ccw_forceinline unsigned int _ccw_stdbit_countr_one_u32(_ccw_uint32 value) {
    return _ccw_stdbit_countr_zero_u32((_ccw_uint32)~value);
}

static _ccw_forceinline unsigned int _ccw_stdbit_countr_one_u64(_ccw_uint64 value) {
    return _ccw_stdbit_countr_zero_u64((_ccw_uint64)~value);
}

static _ccw_forceinline unsigned int _ccw_stdbit_countl_zero_u8(_ccw_uint8 value) {
 #if defined(_CCW_BITIMPL_HAS_VC_BITSCAN_INTRINSIC)
    unsigned long index;

    return _BitScanReverse(&index, (unsigned long)value) ? 7 - (unsigned int)index : 8;
 #else
    unsigned int count = 0;

    if (value == 0)
        return 8;
    if ((value & 0xf0) == 0) { count += 4; value <<= 4; }
    if ((value & 0xc0) == 0) { count += 2; value <<= 2; }
    return count + ((value >> 7) ^ 1);
 #endif
}

static _ccw_forceinline unsigned int _ccw_stdbit_countl_zero_u16(_ccw_uint16 value) {
 #if defined(_CCW_BITIMPL_HAS_VC_BITSCAN_INTRINSIC)
    unsigned long index;

    return _BitScanReverse(&index, (unsigned long)value) ? 15 - (unsigned int)index : 16;
 #else
    unsigned int count = 0;

    if (value == 0)
        return 16;
    if ((value & 0xff00) == 0) { count += 8; value <<= 8; }
    if ((value & 0xf000) == 0) { count += 4; value <<= 4; }
    if ((value & 0xc000) == 0) { count += 2; value <<= 2; }
    return count + ((value >> 15) ^ 1);
 #endif
}

static _ccw_forceinline unsigned int _ccw_stdbit_countl_zero_u32(_ccw_uint32 value) {
 #if defined(_CCW_BITIMPL_HAS_VC_BITSCAN_INTRINSIC)
    unsigned long index;

    return _BitScanReverse(&index, (unsigned long)value) ? 31 - (unsigned int)index : 32;
 #else
    unsigned int count = 0;

    if (value == 0)
        return 32;
    if ((value & 0xffff0000U) == 0) { count += 16; value <<= 16; }
    if ((value & 0xff000000U) == 0) { count += 8; value <<= 8; }
    if ((value & 0xf0000000U) == 0) { count += 4; value <<= 4; }
    if ((value & 0xc0000000U) == 0) { count += 2; value <<= 2; }
    return count + ((value >> 31) ^ 1);
 #endif
}

static _ccw_forceinline unsigned int _ccw_stdbit_countl_zero_u64(_ccw_uint64 value) {
 #if defined(_CCW_BITIMPL_HAS_VC_BITSCAN_INTRINSIC)
    unsigned long index;
  #if !(defined(_M_X64) || defined(_M_AMD64) || defined(_M_ARM64) || defined(_M_ARM64EC))
    _ccw_uint32 hi;
  #endif

    if (value == 0)
        return 64;
  #if defined(_M_X64) || defined(_M_AMD64) || defined(_M_ARM64) || defined(_M_ARM64EC)
    _BitScanReverse64(&index, (unsigned __int64)value);
    return 63 - (unsigned int)index;
  #else
    hi = (_ccw_uint32)(value >> 32);
    if (_BitScanReverse(&index, (unsigned long)hi))
        return 31 - (unsigned int)index;
    _BitScanReverse(&index, (unsigned long)value);
    return 63 - (unsigned int)index;
  #endif
 #else
    unsigned int count = 0;
    _ccw_uint64 t;

    if (value == 0)
        return 64;
    t = value & 0xffffffff00000000ULL; if (t == 0) { count += 32; value <<= 32; }
    t = value & 0xffff000000000000ULL; if (t == 0) { count += 16; value <<= 16; }
    t = value & 0xff00000000000000ULL; if (t == 0) { count +=  8; value <<=  8; }
    t = value & 0xf000000000000000ULL; if (t == 0) { count +=  4; value <<=  4; }
    t = value & 0xc000000000000000ULL; if (t == 0) { count +=  2; value <<=  2; }
    return count + (unsigned int)((value >> 63) ^ 1);
 #endif
}

static _ccw_forceinline unsigned int _ccw_stdbit_countl_one_u8(_ccw_uint8 value) {
    return _ccw_stdbit_countl_zero_u8((_ccw_uint8)~value);
}

static _ccw_forceinline unsigned int _ccw_stdbit_countl_one_u16(_ccw_uint16 value) {
    return _ccw_stdbit_countl_zero_u16((_ccw_uint16)~value);
}

static _ccw_forceinline unsigned int _ccw_stdbit_countl_one_u32(_ccw_uint32 value) {
    return _ccw_stdbit_countl_zero_u32((_ccw_uint32)~value);
}

static _ccw_forceinline unsigned int _ccw_stdbit_countl_one_u64(_ccw_uint64 value) {
    return _ccw_stdbit_countl_zero_u64((_ccw_uint64)~value);
}

static _ccw_forceinline _ccw_uint8 _ccw_stdbit_bit_floor_u8(_ccw_uint8 value) {
    return value == 0 ? 0 : (_ccw_uint8)(1U << (7 - _ccw_stdbit_countl_zero_u8(value)));
}

static _ccw_forceinline _ccw_uint16 _ccw_stdbit_bit_floor_u16(_ccw_uint16 value) {
    return value == 0 ? 0 : (_ccw_uint16)(1U << (15 - _ccw_stdbit_countl_zero_u16(value)));
}

static _ccw_forceinline _ccw_uint32 _ccw_stdbit_bit_floor_u32(_ccw_uint32 value) {
    return value == 0 ? 0 : (_ccw_uint32)(1UL << (31 - _ccw_stdbit_countl_zero_u32(value)));
}

static _ccw_forceinline _ccw_uint64 _ccw_stdbit_bit_floor_u64(_ccw_uint64 value) {
    return value == 0 ? 0 : (_ccw_uint64)(1ULL << (63 - _ccw_stdbit_countl_zero_u64(value)));
}

static _ccw_forceinline unsigned int _ccw_stdbit_bit_width_u8(_ccw_uint8 value) {
    return 8 - _ccw_stdbit_countl_zero_u8(value);
}

static _ccw_forceinline unsigned int _ccw_stdbit_bit_width_u16(_ccw_uint16 value) {
    return 16 - _ccw_stdbit_countl_zero_u16(value);
}

static _ccw_forceinline unsigned int _ccw_stdbit_bit_width_u32(_ccw_uint32 value) {
    return 32 - _ccw_stdbit_countl_zero_u32(value);
}

static _ccw_forceinline unsigned int _ccw_stdbit_bit_width_u64(_ccw_uint64 value) {
    return 64 - _ccw_stdbit_countl_zero_u64(value);
}

static _ccw_forceinline _ccw_uint8 _ccw_stdbit_bit_ceil_u8(_ccw_uint8 value) {
    if (value <= 1)
        return 1;
    return _ccw_stdbit_bit_width_u8((_ccw_uint8)(value - 1)) >= 8 ? 0 : (_ccw_uint8)(1U << _ccw_stdbit_bit_width_u8((_ccw_uint8)(value - 1)));
}

static _ccw_forceinline _ccw_uint16 _ccw_stdbit_bit_ceil_u16(_ccw_uint16 value) {
    if (value <= 1)
        return 1;
    return _ccw_stdbit_bit_width_u16((_ccw_uint16)(value - 1)) >= 16 ? 0 : (_ccw_uint16)(1U << _ccw_stdbit_bit_width_u16((_ccw_uint16)(value - 1)));
}

static _ccw_forceinline _ccw_uint32 _ccw_stdbit_bit_ceil_u32(_ccw_uint32 value) {
    if (value <= 1)
        return 1;
    return _ccw_stdbit_bit_width_u32((_ccw_uint32)(value - 1)) >= 32 ? 0 : (_ccw_uint32)(1UL << _ccw_stdbit_bit_width_u32((_ccw_uint32)(value - 1)));
}

static _ccw_forceinline _ccw_uint64 _ccw_stdbit_bit_ceil_u64(_ccw_uint64 value) {
    if (value <= 1)
        return 1;
    return _ccw_stdbit_bit_width_u64((_ccw_uint64)(value - 1)) >= 64 ? 0 : (_ccw_uint64)(1ULL << _ccw_stdbit_bit_width_u64((_ccw_uint64)(value - 1)));
}

static _ccw_forceinline unsigned int _ccw_stdbit_popcount_u8(_ccw_uint8 value) {
 #if defined(_CCW_BITIMPL_HAS_VC_POPCNT_INTRINSIC)
    return (unsigned int)__popcnt16((unsigned short)value);
 #else
    value = (_ccw_uint8)((value & 0x55) + ((value >> 1) & 0x55));
    value = (_ccw_uint8)((value & 0x33) + ((value >> 2) & 0x33));
    return (unsigned int)((value & 0x0f) + ((value >> 4) & 0x0f));
#endif
}

static _ccw_forceinline unsigned int _ccw_stdbit_popcount_u16(_ccw_uint16 value) {
 #if defined(_CCW_BITIMPL_HAS_VC_POPCNT_INTRINSIC)
    return (unsigned int)__popcnt16((unsigned short)value);
 #else
    value = (_ccw_uint16)((value & 0x5555) + ((value >> 1) & 0x5555));
    value = (_ccw_uint16)((value & 0x3333) + ((value >> 2) & 0x3333));
    value = (_ccw_uint16)((value & 0x0f0f) + ((value >> 4) & 0x0f0f));
    return (unsigned int)((_ccw_uint8)value + (_ccw_uint8)(value >> 8));
 #endif
}

static _ccw_forceinline unsigned int _ccw_stdbit_popcount_u32(_ccw_uint32 value) {
 #if defined(_CCW_BITIMPL_HAS_VC_POPCNT_INTRINSIC)
    return (unsigned int)__popcnt((unsigned int)value);
 #else
    value = (value & 0x55555555U) + ((value >> 1) & 0x55555555U);
    value = (value & 0x33333333U) + ((value >> 2) & 0x33333333U);
    value = (value & 0x0f0f0f0fU) + ((value >> 4) & 0x0f0f0f0fU);
    value = (value & 0x00ff00ffU) + ((value >> 8) & 0x00ff00ffU);
    return (unsigned int)((_ccw_uint16)value + (_ccw_uint16)(value >> 16));
 #endif
}

static _ccw_forceinline unsigned int _ccw_stdbit_popcount_u64(_ccw_uint64 value) {
#if defined(_CCW_BITIMPL_HAS_VC_POPCNT_INTRINSIC)
 #if defined(_M_X64) || defined(_M_AMD64) || defined(_M_ARM64) || defined(_M_ARM64EC)
    return (unsigned int)__popcnt64((unsigned __int64)value);
 #else
    return (unsigned int)(__popcnt((unsigned int)value) + __popcnt((unsigned int)(value >> 32)));
 #endif
#else
    value = (value & 0x5555555555555555ULL) + ((value >> 1) & 0x5555555555555555ULL);
    value = (value & 0x3333333333333333ULL) + ((value >> 2) & 0x3333333333333333ULL);
    value = (value & 0x0f0f0f0f0f0f0f0fULL) + ((value >> 4) & 0x0f0f0f0f0f0f0f0fULL);
    value = (value & 0x00ff00ff00ff00ffULL) + ((value >> 8) & 0x00ff00ff00ff00ffULL);
    value = (value & 0x0000ffff0000ffffULL) + ((value >> 16) & 0x0000ffff0000ffffULL);
    return (unsigned int)((_ccw_uint32)value + (_ccw_uint32)(value >> 32));
#endif
}

#endif
