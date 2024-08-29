/**
 *	@file bit.hpp
 *	@brief	<bit> for c++03
 *  @license Boost Software License Version 1.0
 */
#ifndef __CCWRAP_BIT_HPP_INCLUDED__
#define __CCWRAP_BIT_HPP_INCLUDED__

#include <ccwrap_common.h>

#include <cstring>
#include <cstdint>

#ifndef __CCWRAP_NS
#define __CCWRAP_NS	std
#endif

namespace __CCWRAP_NS {

//
struct endian {
    enum type {
        little = 0,
        big    = 1,
	 #if defined(CCWRAP_ENDIAN)
	 	native = CCWRAP_ENDIAN,
	 #elif defined(CCWRAP_BIG_ENDIAN)
        native = big,
     #else
        native = little,
     #endif
    };

 #if defined(__WATCOMC__)
    typedef int       type_t;
 #else
    typedef enum type type_t;
 #endif

    __ccwrap_forceinline endian(type_t t=type(0)) noexcept : type_(t) {}
    __ccwrap_forceinline endian(endian const& r)  noexcept : type_(r.type_) {}
    __ccwrap_forceinline endian& operator=(type_t r)  noexcept { type_ = r; return *this; }
    __ccwrap_forceinline endian& operator=(endian const& r)  noexcept { type_ = r.type_; return *this; }
    __ccwrap_forceinline operator type_t() const noexcept { return type_; }

    __ccwrap_forceinline bool operator==(type_t r) const noexcept { return type_ == r; }
    __ccwrap_forceinline bool operator!=(type_t r) const noexcept { return type_ != r; }
    __ccwrap_forceinline bool operator< (type_t r) const noexcept { return type_ <  r; }
    __ccwrap_forceinline bool operator>=(type_t r) const noexcept { return type_ >= r; }
    __ccwrap_forceinline bool operator> (type_t r) const noexcept { return type_ >  r; }
    __ccwrap_forceinline bool operator<=(type_t r) const noexcept { return type_ <= r; }

    __ccwrap_forceinline bool operator==(endian const& r) const noexcept { return type_ == r.type_; }
    __ccwrap_forceinline bool operator!=(endian const& r) const noexcept { return type_ != r.type_; }
    __ccwrap_forceinline bool operator< (endian const& r) const noexcept { return type_ <  r.type_; }
    __ccwrap_forceinline bool operator>=(endian const& r) const noexcept { return type_ >= r.type_; }
    __ccwrap_forceinline bool operator> (endian const& r) const noexcept { return type_ >  r.type_; }
    __ccwrap_forceinline bool operator<=(endian const& r) const noexcept { return type_ <= r.type_; }

    friend __ccwrap_forceinline bool operator==(type_t l, endian const& r) noexcept { return l == r.type_; }
    friend __ccwrap_forceinline bool operator!=(type_t l, endian const& r) noexcept { return l != r.type_; }
    friend __ccwrap_forceinline bool operator< (type_t l, endian const& r) noexcept { return l <  r.type_; }
    friend __ccwrap_forceinline bool operator>=(type_t l, endian const& r) noexcept { return l >= r.type_; }
    friend __ccwrap_forceinline bool operator> (type_t l, endian const& r) noexcept { return l >  r.type_; }
    friend __ccwrap_forceinline bool operator<=(type_t l, endian const& r) noexcept { return l <= r.type_; }

private:
    type_t type_;
};


//
template <typename To, typename From>
__ccwrap_forceinline To bit_cast(const From& src) noexcept {
    static_assert(sizeof(To) == sizeof(From), "bit_cast requires source and destination to be of the same size");
 #if defined(__CCWRAP_MEM_ACCESS_SAFE_REINTERPRET)
    return *(To const*)((void const*)&src);
 #else
    To dst;
    memcpy(&dst, &src, sizeof(To));
    return dst;
 #endif
}


// Rotate left
__ccwrap_forceinline uint8_t rotl(uint8_t value, int shift) noexcept {
    shift &= 7;
    return (value << shift) | (value >> (8 - shift));
}

__ccwrap_forceinline uint16_t rotl(uint16_t value, int shift) noexcept {
    shift &= 15;
    return (value << shift) | (value >> (16 - shift));
}

__ccwrap_forceinline uint32_t rotl(uint32_t value, int shift) noexcept {
    shift &= 31;
    return (value << shift) | (value >> (32 - shift));
}

__ccwrap_forceinline uint64_t rotl(uint64_t value, int shift) noexcept {
    shift &= 63;
    return (value << shift) | (value >> (64 - shift));
}


// Rotate right
__ccwrap_forceinline uint8_t rotr(uint8_t value, int shift) noexcept {
    shift &= 7;
    return (value >> shift) | (value << (8 - shift));
}

__ccwrap_forceinline uint16_t rotr(uint16_t value, int shift) noexcept {
    shift &= 15;
    return (value >> shift) | (value << (16 - shift));
}

__ccwrap_forceinline uint32_t rotr(uint32_t value, int shift) noexcept {
    shift &= 31;
    return (value >> shift) | (value << (32 - shift));
}

__ccwrap_forceinline uint64_t rotr(uint64_t value, int shift) noexcept {
    shift &= 63;
    return (value >> shift) | (value << (64 - shift));
}


//
__ccwrap_forceinline uint8_t byteswap(uint8_t value) noexcept {
    return value;
}

__ccwrap_forceinline uint16_t byteswap(uint16_t value) noexcept {
    return (value >> 8) | (value << 8);
}

__ccwrap_forceinline uint32_t byteswap(uint32_t value) noexcept {
    value = ((value & 0xFF00FF00) >> 8) | ((value & 0x00FF00FF) << 8);
    return (value >> 16) | (value << 16);
}

__ccwrap_forceinline uint64_t byteswap(uint64_t value) noexcept {
    value = ((value & 0xFF00FF00FF00FF00ULL) >>  8) | ((value & 0x00FF00FF00FF00FFULL) <<  8);
    value = ((value & 0xFFFF0000FFFF0000ULL) >> 16) | ((value & 0x0000FFFF0000FFFFULL) << 16);
    return (value >> 32) | (value << 32);
}

__ccwrap_forceinline int8_t byteswap(int8_t value) noexcept {
    return value;
}

__ccwrap_forceinline int16_t byteswap(int16_t value) noexcept {
    return (int16_t)(byteswap((uint16_t)(value)));
}

__ccwrap_forceinline int32_t byteswap(int32_t value) noexcept {
    return (int32_t)(byteswap((uint32_t)(value)));
}

__ccwrap_forceinline int64_t byteswap(int64_t value) noexcept {
    return (int64_t)(byteswap((uint64_t)(value)));
}


//
#if 1
template <typename T>
__ccwrap_forceinline bool has_single_bit(T value) noexcept {
    return value != 0 && (value & (value - 1)) == 0;
}
#else
__ccwrap_forceinline bool has_single_bit(uint8_t value) noexcept {
    return value != 0 && (value & (value - 1)) == 0;
}

__ccwrap_forceinline bool has_single_bit(uint16_t value) noexcept {
    return value != 0 && (value & (value - 1)) == 0;
}

__ccwrap_forceinline bool has_single_bit(uint32_t value) noexcept {
    return value != 0 && (value & (value - 1)) == 0;
}

__ccwrap_forceinline bool has_single_bit(uint64_t value) noexcept {
    return value != 0 && (value & (value - 1)) == 0;
}
#endif


// Count trailing zeros
__ccwrap_forceinline int countr_zero(uint8_t value) noexcept {
    if (value == 0) return 8;
    int count = 0;
    if ((value & 0x0F) == 0) { count += 4; value >>= 4; }
    if ((value & 0x03) == 0) { count += 2; value >>= 2; }
    count += (value & 0x01) ^ 1;
    return count;
}

__ccwrap_forceinline int countr_zero(uint16_t value) noexcept {
    if (value == 0) return 16;
    int count = 0;
    if ((value & 0x00FF) == 0) { count += 8; value >>= 8; }
    if ((value & 0x000F) == 0) { count += 4; value >>= 4; }
    if ((value & 0x0003) == 0) { count += 2; value >>= 2; }
    count += (value & 0x0001) ^ 1;
    return count;
}

__ccwrap_forceinline int countr_zero(uint32_t value) noexcept {
    if (value == 0) return 32;
    int count = 0;
    if ((value & 0x0000FFFF) == 0) { count += 16; value >>= 16; }
    if ((value & 0x000000FF) == 0) { count += 8; value >>= 8; }
    if ((value & 0x0000000F) == 0) { count += 4; value >>= 4; }
    if ((value & 0x00000003) == 0) { count += 2; value >>= 2; }
    count += (value & 0x00000001) ^ 1;
    return count;
}

__ccwrap_forceinline int countr_zero(uint64_t value) noexcept {
    if (value == 0) return 64;
    int count = 0;
    if ((value & 0x00000000FFFFFFFFULL) == 0) { count += 32; value >>= 32; }
    if ((value & 0x000000000000FFFFULL) == 0) { count += 16; value >>= 16; }
    if ((value & 0x00000000000000FFULL) == 0) { count += 8; value >>= 8; }
    if ((value & 0x000000000000000FULL) == 0) { count += 4; value >>= 4; }
    if ((value & 0x0000000000000003ULL) == 0) { count += 2; value >>= 2; }
    count += (value & 0x0000000000000001ULL) ^ 1;
    return count;
}


// Count trailing ones
__ccwrap_forceinline int countr_one(uint8_t value) noexcept {
    return countr_zero(uint8_t(~value));
}

__ccwrap_forceinline int countr_one(uint16_t value) noexcept {
    return countr_zero(uint16_t(~value));
}

__ccwrap_forceinline int countr_one(uint32_t value) noexcept {
    return countr_zero(uint32_t(~value));
}

__ccwrap_forceinline int countr_one(uint64_t value) noexcept {
    return countr_zero(uint64_t(~value));
}


// Count leading zeros
__ccwrap_forceinline int countl_zero(uint8_t value) noexcept {
    if (value == 0) return 8;
    int count = 0;
    if ((value & 0xF0) == 0) { count += 4; value <<= 4; }
    if ((value & 0xC0) == 0) { count += 2; value <<= 2; }
    count += (value >> 7) ^ 1;
    return count;
}

__ccwrap_forceinline int countl_zero(uint16_t value) noexcept {
    if (value == 0) return 16;
    int count = 0;
    if ((value & 0xFF00) == 0) { count += 8; value <<= 8; }
    if ((value & 0xF000) == 0) { count += 4; value <<= 4; }
    if ((value & 0xC000) == 0) { count += 2; value <<= 2; }
    count += (value >> 15) ^ 1;
    return count;
}

__ccwrap_forceinline int countl_zero(uint32_t value) noexcept {
    if (value == 0) return 32;
    int count = 0;
    if ((value & 0xFFFF0000) == 0) { count += 16; value <<= 16; }
    if ((value & 0xFF000000) == 0) { count += 8; value <<= 8; }
    if ((value & 0xF0000000) == 0) { count += 4; value <<= 4; }
    if ((value & 0xC0000000) == 0) { count += 2; value <<= 2; }
    count += (value >> 31) ^ 1;
    return count;
}

__ccwrap_forceinline int countl_zero(uint64_t value) noexcept {
    if (value == 0) return 64;
    int count = 0;
    if ((value & 0xFFFFFFFF00000000ULL) == 0) { count += 32; value <<= 32; }
    if ((value & 0xFFFF000000000000ULL) == 0) { count += 16; value <<= 16; }
    if ((value & 0xFF00000000000000ULL) == 0) { count += 8; value <<= 8; }
    if ((value & 0xF000000000000000ULL) == 0) { count += 4; value <<= 4; }
    if ((value & 0xC000000000000000ULL) == 0) { count += 2; value <<= 2; }
    count += (value >> 63) ^ 1;
    return count;
}


// Count leading ones
__ccwrap_forceinline int countl_one(uint8_t value) noexcept {
    return countl_zero((uint8_t)(~value));
}

__ccwrap_forceinline int countl_one(uint16_t value) noexcept {
    return countl_zero((uint16_t)(~value));
}

__ccwrap_forceinline int countl_one(uint32_t value) noexcept {
    return countl_zero((uint32_t)(~value));
}

__ccwrap_forceinline int countl_one(uint64_t value) noexcept {
    return countl_zero((uint64_t)(~value));
}


//
__ccwrap_forceinline uint8_t bit_floor(uint8_t value) noexcept {
    if (value == 0) return 0;
    return 1 << (7 - countl_zero(value));
}

__ccwrap_forceinline uint16_t bit_floor(uint16_t value) noexcept {
    if (value == 0) return 0;
    return 1 << (15 - countl_zero(value));
}

__ccwrap_forceinline uint32_t bit_floor(uint32_t value) noexcept {
    if (value == 0) return 0;
    return 1 << (31 - countl_zero(value));
}

__ccwrap_forceinline uint64_t bit_floor(uint64_t value) noexcept {
    if (value == 0) return 0;
    return 1ULL << (63 - countl_zero(value));
}


//
__ccwrap_forceinline int bit_width(uint8_t value) noexcept {
    return 8 - countl_zero(value);
}

__ccwrap_forceinline int bit_width(uint16_t value) noexcept {
    return 16 - countl_zero(value);
}

__ccwrap_forceinline int bit_width(uint32_t value) noexcept {
    return 32 - countl_zero(value);
}

__ccwrap_forceinline int bit_width(uint64_t value) noexcept {
    return 64 - countl_zero(value);
}


//
__ccwrap_forceinline int popcount(uint8_t bits) noexcept {
    bits = (bits & 0x55) + ((bits >> 1) & 0x55);
    bits = (bits & 0x33) + ((bits >> 2) & 0x33);
    return (bits & 0x0f) + ((bits >> 4) & 0x0f);
}

__ccwrap_forceinline int popcount(uint16_t bits) noexcept {
    bits = (bits & 0x5555) + ((bits >> 1) & 0x5555);
    bits = (bits & 0x3333) + ((bits >> 2) & 0x3333);
    bits = (bits & 0x0f0f) + ((bits >> 4) & 0x0f0f);
    return (uint8_t)bits + (uint8_t)(bits >> 8);
    //return (bits & 0x00ff) + ((bits >> 8) & 0x00ff);
}

__ccwrap_forceinline int popcount(uint32_t bits) noexcept {
    bits = (bits & 0x55555555) + ((bits >> 1) & 0x55555555);
    bits = (bits & 0x33333333) + ((bits >> 2) & 0x33333333);
    bits = (bits & 0x0f0f0f0f) + ((bits >> 4) & 0x0f0f0f0f);
    bits = (bits & 0x00ff00ff) + ((bits >> 8) & 0x00ff00ff);
    return (uint16_t)bits + (uint16_t)(bits >>16);
    //return (bits & 0x0000ffff) + ((bits >>16) & 0x0000ffff);
}

__ccwrap_forceinline int popcount(uint64_t bits) noexcept {
    bits = (bits & 0x5555555555555555UL) + ((bits >> 1) & 0x5555555555555555UL);
    bits = (bits & 0x3333333333333333UL) + ((bits >> 2) & 0x3333333333333333UL);
    bits = (bits & 0x0f0f0f0f0f0f0f0fUL) + ((bits >> 4) & 0x0f0f0f0f0f0f0f0fUL);
    bits = (bits & 0x00ff00ff00ff00ffUL) + ((bits >> 8) & 0x00ff00ff00ff00ffUL);
    bits = (bits & 0x0000ffff0000ffffUL) + ((bits >>16) & 0x0000ffff0000ffffUL);
    return (uint32_t)(bits) + (uint32_t)(bits >>32);
    //bits = (bits & 0x00000000ffffffffUL) + ((bits >>32) & 0x00000000ffffffffUL);
}


} // namespace __CCWRAP_NS

#endif // __CCWRAP_BIT_HPP__
