/**
 *  @file   _ccwrap_detail/bit.hpp
 *  @brief  <bit> backport for pre-C++20 compilers (base: old ccwrap).
 *  @author tenka@6809.net (Masashi Kitamura)
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_DETAIL_BIT_HPP
#define _CCW_DETAIL_BIT_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>

#include <cstring>
#include <cstdint>

namespace _CCW_STD20 {

struct endian {
    enum type {
        little = 0,
        big    = 1,
    #if _CCW_ENDIAN
        native = big
    #else
        native = little
    #endif
    };

 #if defined(__WATCOMC__)
    typedef int       type_t;
 #else
    typedef enum type type_t;
 #endif

    _ccw_forceinline endian() noexcept /*: type_(t)*/ {}
    _ccw_forceinline endian(type_t t) noexcept : type_(t) {}
    _ccw_forceinline endian(endian const& r)    noexcept : type_(r.type_) {}
    _ccw_forceinline endian& operator=(type_t r)         noexcept { type_ = r; return *this; }
    _ccw_forceinline endian& operator=(endian const& r)  noexcept { type_ = r.type_; return *this; }
    _ccw_forceinline operator type_t() const noexcept { return type_; }

private:
    type_t type_;
};

template <typename To, typename From>
_ccw_forceinline To bit_cast(const From& src) noexcept {
    static_assert(sizeof(To) == sizeof(From), "bit_cast requires equal sizes");
 #if defined(_CCW_MEM_ACCESS_SAFE_REINTERPRET)
    return *(To const*)((void const*)&src);
 #else
    To dst;
    memcpy(&dst, &src, sizeof(To));
    return dst;
 #endif
}

// rotate left
_ccw_forceinline uint8_t  rotl(uint8_t  v, int s) noexcept { s &= 7;  return (uint8_t )((v << s) | (v >> (8  - s))); }
_ccw_forceinline uint16_t rotl(uint16_t v, int s) noexcept { s &= 15; return (uint16_t)((v << s) | (v >> (16 - s))); }
_ccw_forceinline uint32_t rotl(uint32_t v, int s) noexcept { s &= 31; return (v << s) | (v >> (32 - s)); }
_ccw_forceinline uint64_t rotl(uint64_t v, int s) noexcept { s &= 63; return (v << s) | (v >> (64 - s)); }

// rotate right
_ccw_forceinline uint8_t  rotr(uint8_t  v, int s) noexcept { s &= 7;  return (uint8_t )((v >> s) | (v << (8  - s))); }
_ccw_forceinline uint16_t rotr(uint16_t v, int s) noexcept { s &= 15; return (uint16_t)((v >> s) | (v << (16 - s))); }
_ccw_forceinline uint32_t rotr(uint32_t v, int s) noexcept { s &= 31; return (v >> s) | (v << (32 - s)); }
_ccw_forceinline uint64_t rotr(uint64_t v, int s) noexcept { s &= 63; return (v >> s) | (v << (64 - s)); }

// byteswap
_ccw_forceinline uint8_t  byteswap(uint8_t  v) noexcept { return v; }
_ccw_forceinline uint16_t byteswap(uint16_t v) noexcept { return (uint16_t)((v >> 8) | (v << 8)); }
_ccw_forceinline uint32_t byteswap(uint32_t v) noexcept {
    v = ((v & 0xFF00FF00u) >> 8) | ((v & 0x00FF00FFu) << 8);
    return (v >> 16) | (v << 16);
}
_ccw_forceinline uint64_t byteswap(uint64_t v) noexcept {
    v = ((v & 0xFF00FF00FF00FF00ULL) >>  8) | ((v & 0x00FF00FF00FF00FFULL) <<  8);
    v = ((v & 0xFFFF0000FFFF0000ULL) >> 16) | ((v & 0x0000FFFF0000FFFFULL) << 16);
    return (v >> 32) | (v << 32);
}
_ccw_forceinline int8_t  byteswap(int8_t  v) noexcept { return v; }
_ccw_forceinline int16_t byteswap(int16_t v) noexcept { return (int16_t)byteswap((uint16_t)v); }
_ccw_forceinline int32_t byteswap(int32_t v) noexcept { return (int32_t)byteswap((uint32_t)v); }
_ccw_forceinline int64_t byteswap(int64_t v) noexcept { return (int64_t)byteswap((uint64_t)v); }

template <typename T>
_ccw_forceinline bool has_single_bit(T v) noexcept { return v != 0 && (v & (v - 1)) == 0; }

// count trailing zeros
_ccw_forceinline int countr_zero(uint8_t v) noexcept {
    if (v == 0) return 8;
    int c = 0;
    if ((v & 0x0F) == 0) { c += 4; v >>= 4; }
    if ((v & 0x03) == 0) { c += 2; v >>= 2; }
    return c + ((v & 1) ^ 1);
}
_ccw_forceinline int countr_zero(uint16_t v) noexcept {
    if (v == 0) return 16;
    int c = 0;
    if ((v & 0x00FF) == 0) { c += 8; v >>= 8; }
    if ((v & 0x000F) == 0) { c += 4; v >>= 4; }
    if ((v & 0x0003) == 0) { c += 2; v >>= 2; }
    return c + ((v & 1) ^ 1);
}
_ccw_forceinline int countr_zero(uint32_t v) noexcept {
    if (v == 0) return 32;
    int c = 0;
    if ((v & 0x0000FFFFu) == 0) { c += 16; v >>= 16; }
    if ((v & 0x000000FFu) == 0) { c += 8;  v >>= 8; }
    if ((v & 0x0000000Fu) == 0) { c += 4;  v >>= 4; }
    if ((v & 0x00000003u) == 0) { c += 2;  v >>= 2; }
    return c + ((v & 1) ^ 1);
}
_ccw_forceinline int countr_zero(uint64_t v) noexcept {
    if (v == 0) return 64;
    int c = 0;
    if ((v & 0x00000000FFFFFFFFULL) == 0) { c += 32; v >>= 32; }
    if ((v & 0x000000000000FFFFULL) == 0) { c += 16; v >>= 16; }
    if ((v & 0x00000000000000FFULL) == 0) { c += 8;  v >>= 8; }
    if ((v & 0x000000000000000FULL) == 0) { c += 4;  v >>= 4; }
    if ((v & 0x0000000000000003ULL) == 0) { c += 2;  v >>= 2; }
    return c + (int)((v & 1) ^ 1);
}

_ccw_forceinline int countr_one(uint8_t  v) noexcept { return countr_zero((uint8_t )~v); }
_ccw_forceinline int countr_one(uint16_t v) noexcept { return countr_zero((uint16_t)~v); }
_ccw_forceinline int countr_one(uint32_t v) noexcept { return countr_zero((uint32_t)~v); }
_ccw_forceinline int countr_one(uint64_t v) noexcept { return countr_zero((uint64_t)~v); }

// count leading zeros
_ccw_forceinline int countl_zero(uint8_t v) noexcept {
    if (v == 0) return 8;
    int c = 0;
    if ((v & 0xF0) == 0) { c += 4; v <<= 4; }
    if ((v & 0xC0) == 0) { c += 2; v <<= 2; }
    return c + ((v >> 7) ^ 1);
}
_ccw_forceinline int countl_zero(uint16_t v) noexcept {
    if (v == 0) return 16;
    int c = 0;
    if ((v & 0xFF00) == 0) { c += 8; v <<= 8; }
    if ((v & 0xF000) == 0) { c += 4; v <<= 4; }
    if ((v & 0xC000) == 0) { c += 2; v <<= 2; }
    return c + ((v >> 15) ^ 1);
}
_ccw_forceinline int countl_zero(uint32_t v) noexcept {
    if (v == 0) return 32;
    int c = 0;
    if ((v & 0xFFFF0000u) == 0) { c += 16; v <<= 16; }
    if ((v & 0xFF000000u) == 0) { c += 8;  v <<= 8; }
    if ((v & 0xF0000000u) == 0) { c += 4;  v <<= 4; }
    if ((v & 0xC0000000u) == 0) { c += 2;  v <<= 2; }
    return c + ((v >> 31) ^ 1);
}
_ccw_forceinline int countl_zero(uint64_t v) noexcept {
    if (v == 0) return 64;
    int c = 0;
    if ((v & 0xFFFFFFFF00000000ULL) == 0) { c += 32; v <<= 32; }
    if ((v & 0xFFFF000000000000ULL) == 0) { c += 16; v <<= 16; }
    if ((v & 0xFF00000000000000ULL) == 0) { c += 8;  v <<= 8; }
    if ((v & 0xF000000000000000ULL) == 0) { c += 4;  v <<= 4; }
    if ((v & 0xC000000000000000ULL) == 0) { c += 2;  v <<= 2; }
    return c + (int)((v >> 63) ^ 1);
}

_ccw_forceinline int countl_one(uint8_t  v) noexcept { return countl_zero((uint8_t )~v); }
_ccw_forceinline int countl_one(uint16_t v) noexcept { return countl_zero((uint16_t)~v); }
_ccw_forceinline int countl_one(uint32_t v) noexcept { return countl_zero((uint32_t)~v); }
_ccw_forceinline int countl_one(uint64_t v) noexcept { return countl_zero((uint64_t)~v); }

_ccw_forceinline int bit_width(uint8_t  v) noexcept { return 8  - countl_zero(v); }
_ccw_forceinline int bit_width(uint16_t v) noexcept { return 16 - countl_zero(v); }
_ccw_forceinline int bit_width(uint32_t v) noexcept { return 32 - countl_zero(v); }
_ccw_forceinline int bit_width(uint64_t v) noexcept { return 64 - countl_zero(v); }

_ccw_forceinline uint8_t  bit_floor(uint8_t  v) noexcept { return v == 0 ? 0 : (uint8_t )(1u << (7  - countl_zero(v))); }
_ccw_forceinline uint16_t bit_floor(uint16_t v) noexcept { return v == 0 ? 0 : (uint16_t)(1u << (15 - countl_zero(v))); }
_ccw_forceinline uint32_t bit_floor(uint32_t v) noexcept { return v == 0 ? 0 : (uint32_t(1) << (31 - countl_zero(v))); }
_ccw_forceinline uint64_t bit_floor(uint64_t v) noexcept { return v == 0 ? 0 : (uint64_t(1) << (63 - countl_zero(v))); }

_ccw_forceinline uint8_t  bit_ceil(uint8_t  v) noexcept { return v <= 1 ? 1 : (uint8_t )(1u << bit_width((uint8_t )(v - 1))); }
_ccw_forceinline uint16_t bit_ceil(uint16_t v) noexcept { return v <= 1 ? 1 : (uint16_t)(1u << bit_width((uint16_t)(v - 1))); }
_ccw_forceinline uint32_t bit_ceil(uint32_t v) noexcept { return v <= 1 ? 1 : (uint32_t(1) << bit_width((uint32_t)(v - 1))); }
_ccw_forceinline uint64_t bit_ceil(uint64_t v) noexcept { return v <= 1 ? 1 : (uint64_t(1) << bit_width((uint64_t)(v - 1))); }

_ccw_forceinline int popcount(uint8_t b) noexcept {
    b = (uint8_t)((b & 0x55) + ((b >> 1) & 0x55));
    b = (uint8_t)((b & 0x33) + ((b >> 2) & 0x33));
    return (b & 0x0f) + ((b >> 4) & 0x0f);
}
_ccw_forceinline int popcount(uint16_t b) noexcept {
    b = (uint16_t)((b & 0x5555) + ((b >> 1) & 0x5555));
    b = (uint16_t)((b & 0x3333) + ((b >> 2) & 0x3333));
    b = (uint16_t)((b & 0x0f0f) + ((b >> 4) & 0x0f0f));
    return (uint8_t)b + (uint8_t)(b >> 8);
}
_ccw_forceinline int popcount(uint32_t b) noexcept {
    b = (b & 0x55555555u) + ((b >> 1) & 0x55555555u);
    b = (b & 0x33333333u) + ((b >> 2) & 0x33333333u);
    b = (b & 0x0f0f0f0fu) + ((b >> 4) & 0x0f0f0f0fu);
    b = (b & 0x00ff00ffu) + ((b >> 8) & 0x00ff00ffu);
    return (uint16_t)b + (uint16_t)(b >> 16);
}
_ccw_forceinline int popcount(uint64_t b) noexcept {
    b = (b & 0x5555555555555555ULL) + ((b >> 1) & 0x5555555555555555ULL);
    b = (b & 0x3333333333333333ULL) + ((b >> 2) & 0x3333333333333333ULL);
    b = (b & 0x0f0f0f0f0f0f0f0fULL) + ((b >> 4) & 0x0f0f0f0f0f0f0f0fULL);
    b = (b & 0x00ff00ff00ff00ffULL) + ((b >> 8) & 0x00ff00ff00ff00ffULL);
    b = (b & 0x0000ffff0000ffffULL) + ((b >> 16) & 0x0000ffff0000ffffULL);
    return (uint32_t)b + (uint32_t)(b >> 32);
}

}   // _CCW_STD20

namespace _ccw {
    using _CCW_STD20::endian;
    using _CCW_STD20::bit_cast;
    using _CCW_STD20::rotl;
    using _CCW_STD20::rotr;
    using _CCW_STD20::byteswap;
    using _CCW_STD20::has_single_bit;
    using _CCW_STD20::countl_zero;
    using _CCW_STD20::countl_one;
    using _CCW_STD20::countr_zero;
    using _CCW_STD20::countr_one;
    using _CCW_STD20::popcount;
    using _CCW_STD20::bit_width;
    using _CCW_STD20::bit_ceil;
    using _CCW_STD20::bit_floor;
}

#endif  // _CCW_DETAIL_BIT_HPP
