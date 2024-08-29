#pragma once

#if __CCWRAP_CXX <= 202002L

#include <cstdint>
#include <intrin.h>

namespace std {

	// byteswap
	__ccwrap_forceinline uint8_t byteswap(uint8_t value) noexcept {
	    return value;
	}

	__ccwrap_forceinline uint16_t byteswap(uint16_t value) noexcept {
	    return _byteswap_ushort(value);
	}

	__ccwrap_forceinline uint32_t byteswap(uint32_t value) noexcept {
	    return _byteswap_ulong(value);
	}

	__ccwrap_forceinline uint64_t byteswap(uint64_t value) noexcept {
	    return _byteswap_uint64(value);
	}

	__ccwrap_forceinline uint8_t byteswap(int8_t value) noexcept {
	    return value;
	}
	__ccwrap_forceinline uint16_t byteswap(int16_t value) noexcept {
	    return _byteswap_ushort(value);
	}

	__ccwrap_forceinline uint32_t byteswap(int32_t value) noexcept {
	    return _byteswap_ulong(value);
	}

	__ccwrap_forceinline uint64_t byteswap(int64_t value) noexcept {
	    return _byteswap_uint64(value);
	}

}	// std


#if __CCWRAP_CXX < 202002L

#include <cstring>

namespace std {

#if __CCWRAP_CXX >= 201103L
//
enum class endian {
    little = 0,
    big    = 1,
    native = __CCWRAP_ENDIAN,
};
#else
//
struct endian {
    enum type {
        little = 0,
        big    = 1,
        native = __CCWRAP_ENDIAN,
    };

    __ccwrap_forceinline endian(type t=type(0)) noexcept : type_(t) {}
    __ccwrap_forceinline endian(endian const& r)  noexcept : type_(r.type_) {}
    __ccwrap_forceinline endian& operator=(type r)  noexcept { type_ = r; return *this; }
    __ccwrap_forceinline endian& operator=(endian const& r)  noexcept { type_ = r.type_; return *this; }
    __ccwrap_forceinline operator type() const noexcept { return type_; }

    __ccwrap_forceinline bool operator==(type r) const noexcept { return type_ == r; }
    __ccwrap_forceinline bool operator!=(type r) const noexcept { return type_ != r; }
    __ccwrap_forceinline bool operator< (type r) const noexcept { return type_ <  r; }
    __ccwrap_forceinline bool operator>=(type r) const noexcept { return type_ >= r; }
    __ccwrap_forceinline bool operator> (type r) const noexcept { return type_ >  r; }
    __ccwrap_forceinline bool operator<=(type r) const noexcept { return type_ <= r; }

    __ccwrap_forceinline bool operator==(endian const& r) const noexcept { return type_ == r.type_; }
    __ccwrap_forceinline bool operator!=(endian const& r) const noexcept { return type_ != r.type_; }
    __ccwrap_forceinline bool operator< (endian const& r) const noexcept { return type_ <  r.type_; }
    __ccwrap_forceinline bool operator>=(endian const& r) const noexcept { return type_ >= r.type_; }
    __ccwrap_forceinline bool operator> (endian const& r) const noexcept { return type_ >  r.type_; }
    __ccwrap_forceinline bool operator<=(endian const& r) const noexcept { return type_ <= r.type_; }

    friend __ccwrap_forceinline bool operator==(type l, endian const& r) noexcept { return l == r.type_; }
    friend __ccwrap_forceinline bool operator!=(type l, endian const& r) noexcept { return l != r.type_; }
    friend __ccwrap_forceinline bool operator< (type l, endian const& r) noexcept { return l <  r.type_; }
    friend __ccwrap_forceinline bool operator>=(type l, endian const& r) noexcept { return l >= r.type_; }
    friend __ccwrap_forceinline bool operator> (type l, endian const& r) noexcept { return l >  r.type_; }
    friend __ccwrap_forceinline bool operator<=(type l, endian const& r) noexcept { return l <= r.type_; }
private:
    type type_;
};
#endif


// bit_cast using memcpy to avoid strict aliasing issues
template <typename To, typename From>
__ccwrap_forceinline To bit_cast(const From& src) noexcept {
    static_assert(sizeof(To) == sizeof(From), "bit_cast requires source and destination to be of the same size");
 #if defined(__CCWRAP_MEM_ACCESS_SAFE_REINTERPRET)
    return *(To const volatile*)((void const volatile*)&src);
 #else
    To dst;
    memcpy(&dst, &src, sizeof(To));
    return dst;
 #endif
}

// has_single_bit
template <typename T>
__ccwrap_forceinline bool has_single_bit(T value) noexcept {
    return value != 0 && (value & (value - 1)) == 0;
}

// count trailing zeros
__ccwrap_forceinline int countr_zero(uint8_t value) noexcept {
    unsigned long index;
    return _BitScanForward(&index, value) ? (int)(index) : 8;
}

__ccwrap_forceinline int countr_zero(uint16_t value) noexcept {
    unsigned long index;
    return _BitScanForward(&index, value) ? (int)(index) : 16;
}

__ccwrap_forceinline int countr_zero(uint32_t value) noexcept {
    unsigned long index;
    return _BitScanForward(&index, value) ? (int)(index) : 32;
}

#if defined(_WIN64) || defined(_M_X64)
__ccwrap_forceinline int countr_zero(uint64_t value) noexcept {
    unsigned long index;
    return _BitScanForward64(&index, value) ? (int)(index) : 64;
}
#else
__ccwrap_forceinline int countr_zero(uint64_t value) noexcept {
    unsigned long index;
    if (_BitScanForward(&index, (uint32_t)(value)))
        return (int)(index);
    if (_BitScanForward(&index, (uint32_t)(value >> 32)))
        return (int)(index) + 32;
    return 64;
}
#endif

// count trailing ones
__ccwrap_forceinline int countr_one(uint8_t value) noexcept {
    return countr_zero((uint8_t)~value);
}

__ccwrap_forceinline int countr_one(uint16_t value) noexcept {
    return countr_zero((uint16_t)~value);
}

__ccwrap_forceinline int countr_one(uint32_t value) noexcept {
    return countr_zero((uint32_t)~value);
}

__ccwrap_forceinline int countr_one(uint64_t value) noexcept {
    return countr_zero((uint64_t)~value);
}


// count leading zeros
__ccwrap_forceinline int countl_zero(uint8_t value) noexcept {
    unsigned long index;
    return _BitScanReverse(&index, value) ? 7 - (int)(index) : 8;
}

__ccwrap_forceinline int countl_zero(uint16_t value) noexcept {
    unsigned long index;
    return _BitScanReverse(&index, value) ? 15 - (int)(index) : 16;
}

__ccwrap_forceinline int countl_zero(uint32_t value) noexcept {
    unsigned long index;
    return _BitScanReverse(&index, value) ? 31 - (int)(index) : 32;
}

#if defined(__CCWRAP_CPUT54)
__ccwrap_forceinline int countl_zero(uint64_t value) noexcept {
    unsigned long index;
    return _BitScanReverse64(&index, value) ? 63 - (int)(index) : 64;
}
#else
__ccwrap_forceinline int countl_zero(uint64_t value) noexcept {
    unsigned long index;
    if (_BitScanReverse(&index, (uint32_t)(value >> 32)))
        return 31 - (int)(index);
    if (_BitScanReverse(&index, (uint32_t)(value)))
        return 63 - (int)(index);
    return 64;
}
#endif

// count leading ones
__ccwrap_forceinline int countl_one(uint8_t value) noexcept {
    return countl_zero((uint8_t)~value);
}

__ccwrap_forceinline int countl_one(uint16_t value) noexcept {
    return countl_zero((uint16_t)~value);
}

__ccwrap_forceinline int countl_one(uint32_t value) noexcept {
    return countl_zero((uint32_t)~value);
}

__ccwrap_forceinline int countl_one(uint64_t value) noexcept {
    return countl_zero((uint64_t)~value);
}

// bit_floor
template <typename T>
__ccwrap_forceinline T bit_floor(T value) noexcept {
    if (value == 0)
        return 0;
    unsigned long index;
    _BitScanReverse(&index, value);
    return T(1) << index;
}

__ccwrap_forceinline uint64_t bit_floor(uint64_t value) noexcept {
    if (value == 0)
        return 0;
    unsigned long index;
    _BitScanReverse64(&index, value);
    return static_cast<uint64_t>(1) << index;
}


// bit_width
template <typename T>
__ccwrap_forceinline int bit_width(T value) noexcept {
    if (value == 0)
        return 0;
    unsigned long index;
    _BitScanReverse(&index, value);
    return static_cast<int>(index + 1);
}

__ccwrap_forceinline int bit_width(uint64_t value) noexcept {
    if (value == 0)
        return 0;
    unsigned long index;
    _BitScanReverse64(&index, value);
    return static_cast<int>(index + 1);
}


// popcount
__ccwrap_forceinline int popcount(uint8_t value) noexcept {
    return __popcnt(value);
}

__ccwrap_forceinline int popcount(uint16_t value) noexcept {
    return __popcnt16(value);
}

__ccwrap_forceinline int popcount(uint32_t value) noexcept {
    return __popcnt(value);
}

#if defined(_WIN64) || defined(_M_X64)
__ccwrap_forceinline int popcount(uint64_t value) noexcept {
    return (int)(__popcnt64(value));
}
#else
__ccwrap_forceinline int popcount(uint64_t value) noexcept {
    return __popcnt((uint32_t)(value))
         + __popcnt((uint32_t)(value >> 32));
}
#endif

// Rotate left
__ccwrap_forceinline uint8_t rotl(uint8_t value, int shift) noexcept {
	return _rotl8(value, shift);
}

__ccwrap_forceinline uint16_t rotl(uint16_t value, int shift) noexcept {
	return _rotl16(value, shift);
}

__ccwrap_forceinline uint32_t rotl(uint32_t value, int shift) noexcept {
    return _rotl(value, shift);
}

__ccwrap_forceinline uint64_t rotl(uint64_t value, int shift) noexcept {
 #if defined(_WIN64) || defined(_M_X64)
    return _rotl64(value, shift);
 #else
    return (value << shift) | (value >> (64 - shift));
 #endif
}

// Rotate right
__ccwrap_forceinline uint8_t rotr(uint8_t value, int shift) noexcept {
	return _rotr8(value, shift);
}

__ccwrap_forceinline uint16_t rotr(uint16_t value, int shift) noexcept {
	return _rotr16(value, shift);
}

__ccwrap_forceinline uint32_t rotr(uint32_t value, int shift) noexcept {
    return _rotr(value, shift);
}

__ccwrap_forceinline uint64_t rotr(uint64_t value, int shift) noexcept {
 #if defined(_WIN64) || defined(_M_X64)
    return _rotr64(value, shift);
 #else
    return (value >> shift) | (value << (64 - shift));
 #endif
}

}	// namespace std

#endif	// __CCWRAP_CXX < 202002L

#endif	// __CCWRAP_CXX <= 202002L
