#if __CCWRAP_CXX <= 202002L

#include <cstdint>

namespace std {

// byteswap
__ccwrap_forceinline std::uint16_t byteswap(std::uint16_t value) noexcept {
    return __builtin_bswap16(value);
}

__ccwrap_forceinline std::uint32_t byteswap(std::uint32_t value) noexcept {
    return __builtin_bswap32(value);
}

__ccwrap_forceinline std::uint64_t byteswap(std::uint64_t value) noexcept {
    return __builtin_bswap64(value);
}

__ccwrap_forceinline std::uint8_t byteswap(std::uint8_t value) noexcept {
    return value;  // No byteswap needed for 8-bit values
}

}	// std


#if __CCWRAP_CXX < 202002L

#include <cstring>
#include <type_traits>

namespace std {

#if __CCWRAP_CXX >= 201103L
//
enum class endian {
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
#else	// __CCWRAP_CXX < 201103L
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
#endif	// __CCWRAP_CXX  201103L


// bit_cast using memcpy to avoid strict aliasing issues
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


// has_single_bit
template <typename T>
__ccwrap_forceinline bool has_single_bit(T value) noexcept {
    return value != 0 && (value & (value - 1)) == 0;
}


// Rotate left
__ccwrap_forceinline std::uint8_t rotl(std::uint8_t value, int shift) noexcept {
    return (value << shift) | (value >> (8 - shift));
}

__ccwrap_forceinline std::uint16_t rotl(std::uint16_t value, int shift) noexcept {
    return (value << shift) | (value >> (16 - shift));
}

__ccwrap_forceinline std::uint32_t rotl(std::uint32_t value, int shift) noexcept {
    return (value << shift) | (value >> (32 - shift));
}

__ccwrap_forceinline std::uint64_t rotl(std::uint64_t value, int shift) noexcept {
    return (value << shift) | (value >> (64 - shift));
}


// Rotate right
__ccwrap_forceinline std::uint8_t rotr(std::uint8_t value, int shift) noexcept {
    return (value >> shift) | (value << (8 - shift));
}

__ccwrap_forceinline std::uint16_t rotr(std::uint16_t value, int shift) noexcept {
    return (value >> shift) | (value << (16 - shift));
}

__ccwrap_forceinline std::uint32_t rotr(std::uint32_t value, int shift) noexcept {
    return (value >> shift) | (value << (32 - shift));
}

__ccwrap_forceinline std::uint64_t rotr(std::uint64_t value, int shift) noexcept {
    return (value >> shift) | (value << (64 - shift));
}


// count trailing zeros
__ccwrap_forceinline int countr_zero(std::uint8_t value) noexcept {
    return value == 0 ? 8 : __builtin_ctz(value);
}

__ccwrap_forceinline int countr_zero(std::uint16_t value) noexcept {
    return value == 0 ? 16 : __builtin_ctz(value);
}

__ccwrap_forceinline int countr_zero(std::uint32_t value) noexcept {
    return value == 0 ? 32 : __builtin_ctz(value);
}

__ccwrap_forceinline int countr_zero(std::uint64_t value) noexcept {
    return value == 0 ? 64 : __builtin_ctzll(value);
}


// count trailing ones
__ccwrap_forceinline int countr_one(std::uint8_t value) noexcept {
    return countr_zero((std::uint8_t)~value);
}

__ccwrap_forceinline int countr_one(std::uint16_t value) noexcept {
    return countr_zero((std::uint16_t)~value);
}

__ccwrap_forceinline int countr_one(std::uint32_t value) noexcept {
    return countr_zero((std::uint32_t)~value);
}

__ccwrap_forceinline int countr_one(std::uint64_t value) noexcept {
    return countr_zero((std::uint64_t)~value);
}


// count leading zeros
__ccwrap_forceinline int countl_zero(std::uint8_t value) noexcept {
    return value == 0 ? 8 : __builtin_clz(value) - 24;
}

__ccwrap_forceinline int countl_zero(std::uint16_t value) noexcept {
    return value == 0 ? 16 : __builtin_clz(value) - 16;
}

__ccwrap_forceinline int countl_zero(std::uint32_t value) noexcept {
    return value == 0 ? 32 : __builtin_clz(value);
}

__ccwrap_forceinline int countl_zero(std::uint64_t value) noexcept {
    return value == 0 ? 64 : __builtin_clzll(value);
}


// count leading ones
__ccwrap_forceinline int countl_one(std::uint8_t value) noexcept {
    return countl_zero((std::uint8_t)~value);
}

__ccwrap_forceinline int countl_one(std::uint16_t value) noexcept {
    return countl_zero((std::uint16_t)~value);
}

__ccwrap_forceinline int countl_one(std::uint32_t value) noexcept {
    return countl_zero((std::uint32_t)~value);
}

__ccwrap_forceinline int countl_one(std::uint64_t value) noexcept {
    return countl_zero((std::uint64_t)~value);
}


// bit_floor
template <typename T>
__ccwrap_forceinline T bit_floor(T value) noexcept {
    if (value == 0) return 0;
    return T(1) << (sizeof(std::uint32_t) * 8 - 1 - __builtin_clz(value));
}
__ccwrap_forceinline std::uint64_t bit_floor(std::uint64_t value) noexcept {
    if (value == 0) return 0;
    return std::uint64_t(1) << (sizeof(std::uint64_t) * 8 - 1 - __builtin_clzll(value));
}


// bit_width
__ccwrap_forceinline int bit_width(std::uint8_t value) noexcept {
    return value ? sizeof(std::uint32_t) * 8 - __builtin_clz((std::uint32_t)value) : 0;
}
__ccwrap_forceinline int bit_width(std::uint16_t value) noexcept {
    return value ? sizeof(std::uint32_t) * 8 - __builtin_clz((std::uint32_t)value) : 0;
}
__ccwrap_forceinline int bit_width(std::uint32_t value) noexcept {
    return value ? sizeof(std::uint32_t) * 8 - __builtin_clz(value) : 0;
}
__ccwrap_forceinline int bit_width(std::uint64_t value) noexcept {
    return value ? sizeof(std::uint64_t) * 8 - __builtin_clzll(value) : 0;
}


// popcount
__ccwrap_forceinline int popcount(std::uint8_t value) noexcept {
    return __builtin_popcount(value);
}

__ccwrap_forceinline int popcount(std::uint16_t value) noexcept {
    return __builtin_popcount(value);
}

__ccwrap_forceinline int popcount(std::uint32_t value) noexcept {
    return __builtin_popcount(value);
}

__ccwrap_forceinline int popcount(std::uint64_t value) noexcept {
    return __builtin_popcountll(value);
}

} // namespace std

#endif	// __CCWRAP_CXX < 202002L

#endif	// __CCWRAP_CXX <= 202002L
