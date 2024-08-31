#ifndef __CCWRAP_BIT_HPP__
#define __CCWRAP_BIT_HPP__

#include <cstring>
#include <cstdint>


extern unsigned short __builtin_bswap16(unsigned short value) noexcept;
    #pragma aux __builtin_bswap16 = \
        "xchg al, ah"       \
        parm [ax]           \
	    modify [ax]         \
        value [ax]

extern unsigned long __builtin_bswap32(unsigned long value) noexcept;
    #pragma aux __builtin_bswap32 = \
        "bswap eax"         \
        parm [eax]          \
	    modify [eax]        \
        value [eax]

extern unsigned __int64 __builtin_bswap64(unsigned __int64 value) noexcept;
    #pragma aux __builtin_bswap64 = \
        "bswap eax"         \
        "bswap edx"         \
        "xchg eax, edx"     \
        parm [eax edx]      \
	    modify [eax]        \
        value [eax edx]


#if 1
extern unsigned char  __builtin_rotl8(unsigned char value, unsigned char shift) noexcept;
	#pragma aux __builtin_rotl8 = \
	    "rol al, cl"        \
	    parm [al] [cl]      \
	    modify [al]         \
	    value [al]

extern unsigned char  __builtin_rotr8(unsigned char value, unsigned char shift) noexcept;
	#pragma aux __builtin_rotr8 = \
	    "ror al, cl"        \
	    parm [al] [cl]      \
	    modify [al]         \
	    value [al]

extern unsigned short __builtin_rotl16(unsigned short value, unsigned char shift) noexcept;
	#pragma aux __builtin_rotl16 = \
	    "rol ax, cl"        \
	    parm [ax] [cl]      \
	    modify [ax]         \
	    value [ax]

extern unsigned short __builtin_rotr16(unsigned short value, unsigned char shift) noexcept;
	#pragma aux __builtin_rotr16 = \
	    "ror ax, cl"        \
	    parm [ax] [cl]      \
	    modify [ax]         \
	    value [ax]

extern unsigned long  __builtin_rotl32(unsigned long value, unsigned char shift) noexcept;
	#pragma aux __builtin_rotl32 = \
	    "rol eax, cl"       \
	    parm [eax] [cl]     \
	    modify [eax]        \
	    value [eax]

extern unsigned long  __builtin_rotr32(unsigned long value, unsigned char shift) noexcept;
	#pragma aux __builtin_rotr32 = \
	    "ror eax, cl"       \
	    parm [eax] [cl]     \
	    modify [eax]        \
	    value [eax]
#endif


extern int __builtin_countr_zero8(unsigned char value) noexcept;
	#pragma aux __builtin_countr_zero8 = \
	    "movzx eax, al"	\
	    "bsf eax, eax"  \
	    "jnz skip1"     \
	    "mov eax, 8"    \
	    "skip1:"        \
	    parm [al]       \
	    modify [eax]    \
	    value [eax]

extern int __builtin_countr_zero16(unsigned short value) noexcept;
	#pragma aux __builtin_countr_zero16 = \
	    "movzx eax, ax"	\
	    "bsf eax, eax"  \
	    "jnz skip1"     \
	    "mov eax, 16"   \
	    "skip1:"        \
	    parm [ax]       \
	    modify [eax]    \
	    value [eax]

extern int __builtin_countr_zero32(unsigned long value) noexcept;
	#pragma aux __builtin_countr_zero32 = \
	    "bsf eax, eax"  \
	    "jnz skip1"     \
	    "mov eax, 32"   \
	    "skip1:"        \
	    parm [eax]      \
	    modify [eax]    \
	    value [eax]

extern int __builtin_countr_zero64(unsigned __int64 value) noexcept;
	#pragma aux __builtin_countr_zero64 = \
	    "bsf eax, eax"  	\
	    "jnz skip1"     	\
	    "bsf eax, edx"  	\
	    "jnz skip2"     	\
	    "mov eax, 32"   	\
		"skip2:"        	\
	    "add eax, 32"   	\
	    "skip1:"        	\
	    parm [edx eax]  	\
	    modify [eax edx]	\
	    value [eax]



extern int __builtin_countl_zero8(unsigned char value) noexcept;
	#pragma aux __builtin_countl_zero8 = \
	    "movzx eax, al"	\
	    "bsr eax, eax"	\
	    "jnz skip1"     \
	    "mov eax, 15"   \
	    "skip1:"        \
	    "xor eax, 7"    \
	    parm [al]       \
	    modify [eax]    \
	    value [eax]

extern int __builtin_countl_zero16(unsigned short value) noexcept;
	#pragma aux __builtin_countl_zero16 = \
	    "movzx eax, ax"	\
	    "bsr eax, eax"  \
	    "jnz skip1"     \
	    "mov eax, 31"   \
	    "skip1:"        \
	    "xor eax, 15"   \
	    parm [ax]       \
	    modify [eax]    \
	    value [eax]

extern unsigned long __builtin_countl_zero32(unsigned long value) noexcept;
	#pragma aux __builtin_countl_zero32 = \
	    "bsr eax, eax"  \
	    "jnz skip1"     \
	    "mov eax, 63"   \
	    "skip1:"        \
	    "xor eax, 31"   \
	    parm [eax]      \
	    modify [eax]    \
	    value [eax]

extern int __builtin_countl_zero64(unsigned __int64 value) noexcept;
    #pragma aux __builtin_countl_zero64 = \
	    "bsr edx, edx"      \
	    "jz skip1"          \
	    "xor edx, 31"       \
	    "mov eax,edx"       \
	    "jmp skip3"         \
	    "skip1:"            \
	    "bsr eax, eax"      \
	    "jnz skip2"         \
	    "mov eax, 127"      \
	    "skip2:"            \
	    "xor eax, 63"       \
        "skip3:"            \
	    parm [edx eax]      \
	    modify [eax edx]    \
	    value [eax]



extern int __builtin_popcount8(unsigned char value) noexcept;
	#pragma aux __builtin_popcount8 = \
	    "mov ah, al"         \
	    "shr ah, 1"          \
	    "and ah, 55h"        \
	    "sub al, ah"         \
	    "mov ah, al"         \
	    "shr ah, 2"          \
	    "and ah, 33h"        \
	    "and al, 33h"        \
	    "add al, ah"         \
	    "mov ah, al"         \
	    "shr ah, 4"          \
	    "add al, ah"         \
	    "and eax, 0Fh"       \
	    parm [al]            \
	    modify [eax]         \
	    value [eax]

extern int __builtin_popcount16(unsigned short value) noexcept;
	#pragma aux __builtin_popcount16 = \
	    "mov dx, ax"         \
	    "shr dx, 1"          \
	    "and dx, 5555h"      \
	    "sub ax, dx"         \
	    "mov dx, ax"         \
	    "shr dx, 2"          \
	    "and dx, 3333h"      \
	    "and ax, 3333h"      \
	    "add ax, dx"         \
	    "mov dx, ax"         \
	    "shr dx, 4"          \
	    "add ax, dx"         \
	    "and ax, 0F0Fh"      \
	    "mov dx, ax"         \
	    "shr dx, 8"          \
	    "add ax, dx"         \
	    "and eax, 00FFh"     \
	    parm [ax]            \
	    modify [dx]          \
	    value [eax]

extern int __builtin_popcount(unsigned long value) noexcept;
	#pragma aux __builtin_popcount = \
	    "mov edx, eax"         \
	    "shr edx, 1"           \
	    "and edx, 55555555h"   \
	    "sub eax, edx"         \
	    "mov edx, eax"         \
	    "shr edx, 2"           \
	    "and edx, 33333333h"   \
	    "and eax, 33333333h"   \
	    "add eax, edx"         \
	    "mov edx, eax"         \
	    "shr edx, 4"           \
	    "add eax, edx"         \
	    "and eax, 0F0F0F0Fh"   \
	    "mov edx, eax"         \
	    "shr edx, 8"           \
	    "add eax, edx"         \
	    "and eax, 00FF00FFh"   \
	    "mov edx, eax"         \
	    "shr edx, 16"          \
	    "add eax, edx"         \
	    "and eax, 0000FFFFh"   \
	    parm [eax]             \
	    modify [edx]           \
	    value [eax]


namespace std {


//
struct endian {
    enum type {
        little = 0,
        big    = 1,
        native = little,
    };
    typedef int       type_t;

    endian(type_t t=type(0)) noexcept : type_(t) {}
    endian(endian const& r)  noexcept : type_(r.type_) {}
    endian& operator=(type_t r)  noexcept { type_ = r; return *this; }
    endian& operator=(endian const& r)  noexcept { type_ = r.type_; return *this; }
    operator type_t() const noexcept { return type_; }

    bool operator==(type_t r) const noexcept { return type_ == r; }
    bool operator!=(type_t r) const noexcept { return type_ != r; }
    bool operator< (type_t r) const noexcept { return type_ <  r; }
    bool operator>=(type_t r) const noexcept { return type_ >= r; }
    bool operator> (type_t r) const noexcept { return type_ >  r; }
    bool operator<=(type_t r) const noexcept { return type_ <= r; }

    bool operator==(endian const& r) const noexcept { return type_ == r.type_; }
    bool operator!=(endian const& r) const noexcept { return type_ != r.type_; }
    bool operator< (endian const& r) const noexcept { return type_ <  r.type_; }
    bool operator>=(endian const& r) const noexcept { return type_ >= r.type_; }
    bool operator> (endian const& r) const noexcept { return type_ >  r.type_; }
    bool operator<=(endian const& r) const noexcept { return type_ <= r.type_; }

    friend inline bool operator==(type_t l, endian const& r) noexcept { return l == r.type_; }
    friend inline bool operator!=(type_t l, endian const& r) noexcept { return l != r.type_; }
    friend inline bool operator< (type_t l, endian const& r) noexcept { return l <  r.type_; }
    friend inline bool operator>=(type_t l, endian const& r) noexcept { return l >= r.type_; }
    friend inline bool operator> (type_t l, endian const& r) noexcept { return l >  r.type_; }
    friend inline bool operator<=(type_t l, endian const& r) noexcept { return l <= r.type_; }
private:
    type_t type_;
};


//
template <typename To, typename From>
inline To bit_cast(const From& src) noexcept {
    static_assert(sizeof(To) == sizeof(From), "bit_cast requires source and destination to be of the same size");
 #if 1 //defined(CCWRAP_OLD_PURE_COMPILER)
    return *(To const volatile*)((void const volatile*)&src);
 #else
    To dst;
    memcpy(&dst, &src, sizeof(To));
    return dst;
 #endif
}

//
template <typename T>
__ccwrap_forceinline bool has_single_bit(T value) noexcept {
    return value != 0 && (value & (value - 1)) == 0;
}

template<typename T>
T byteswap(T value);

// byteswap
inline uint8_t byteswap(uint8_t value) noexcept {
    return value;
}

inline uint16_t byteswap(uint16_t value) noexcept {
    return __builtin_bswap16(value);
}

inline uint32_t byteswap(uint32_t value) noexcept {
    return __builtin_bswap32(value);
}

inline uint64_t byteswap(uint64_t value) noexcept {
    return __builtin_bswap64(value);
}

inline int8_t byteswap(int8_t value) noexcept {
    return value;
}
inline int16_t byteswap(int16_t value) noexcept {
    return (int16_t)__builtin_bswap16(value);
}

inline int32_t byteswap(int32_t value) noexcept {
    return (int32_t)__builtin_bswap32(value);
}

inline int64_t byteswap(int64_t value) noexcept {
    return (int64_t)__builtin_bswap64(value);
}


// Rotate left
inline uint8_t rotl(uint8_t value, int shift) noexcept {
	return __builtin_rotl8(value, shift);
}

inline uint16_t rotl(uint16_t value, int shift) noexcept {
	return __builtin_rotl16(value, shift);
}

inline uint32_t rotl(uint32_t value, int shift) noexcept {
	return __builtin_rotl32(value, shift);
}

inline uint64_t rotl(uint64_t value, int shift) noexcept {
    return (value << shift) | (value >> (64 - shift));
}


// Rotate right
inline uint8_t rotr(uint8_t value, int shift) noexcept {
	return __builtin_rotr8(value, shift);
}

inline uint16_t rotr(uint16_t value, int shift) noexcept {
	return __builtin_rotr16(value, shift);
}

inline uint32_t rotr(uint32_t value, int shift) noexcept {
	return __builtin_rotr32(value, shift);
}

inline uint64_t rotr(uint64_t value, int shift) noexcept {
    return (value >> shift) | (value << (64 - shift));
}


// Count trailing zeros
inline int countr_zero(uint8_t value) noexcept {
	return __builtin_countr_zero8(value);
}

inline int countr_zero(uint16_t value) noexcept {
	return __builtin_countr_zero16(value);
}

inline int countr_zero(uint32_t value) noexcept {
	return (int)__builtin_countr_zero32(value);
}

inline int countr_zero(uint64_t value) noexcept {
	return (int)__builtin_countr_zero64(value);
}


// Count trailing ones
inline int countr_one(uint8_t value) noexcept {
    return __builtin_countr_zero8(~value);
}

inline int countr_one(uint16_t value) noexcept {
    return __builtin_countr_zero16(~value);
}

inline int countr_one(uint32_t value) noexcept {
    return __builtin_countr_zero32(~value);
}

inline int countr_one(uint64_t value) noexcept {
    return __builtin_countr_zero64(~value);
}


// Count leading zeros
inline int countl_zero(uint8_t value) noexcept {
    return __builtin_countl_zero8(value);
}

inline int countl_zero(uint16_t value) noexcept {
    return __builtin_countl_zero16(value);
}

inline int countl_zero(uint32_t value) noexcept {
    return __builtin_countl_zero32(value);
}

inline int countl_zero(uint64_t value) noexcept {
    return __builtin_countl_zero64(value);
}


// Count leading ones
inline int countl_one(uint8_t value) noexcept {
    return __builtin_countl_zero8(~value);
}

inline int countl_one(uint16_t value) noexcept {
    return __builtin_countl_zero16(~value);
}

inline int countl_one(uint32_t value) noexcept {
    return __builtin_countl_zero32(~value);
}

inline int countl_one(uint64_t value) noexcept {
    return __builtin_countl_zero64(~value);
}


//
inline int bit_width(uint8_t value) noexcept {
    return 8 - __builtin_countl_zero8(value);
}

inline int bit_width(uint16_t value) noexcept {
    return 16 - __builtin_countl_zero16(value);
}

inline int bit_width(uint32_t value) noexcept {
    return 32 - __builtin_countl_zero32(value);
}

inline int bit_width(uint64_t value) noexcept {
    return 64 - __builtin_countl_zero64(value);
}


//
inline uint8_t bit_floor(uint8_t value) noexcept {
    if (value == 0) return 0;
    return 1 << (7 - __builtin_countl_zero8(value));
}

inline uint16_t bit_floor(uint16_t value) noexcept {
    if (value == 0) return 0;
    return 1 << (15 - __builtin_countl_zero16(value));
}

inline uint32_t bit_floor(uint32_t value) noexcept {
    if (value == 0) return 0;
    return 1 << (31 - __builtin_countl_zero32(value));
}

inline uint64_t bit_floor(uint64_t value) noexcept {
    if (value == 0) return 0;
    return 1ULL << (63 - __builtin_countl_zero64(value));
}

//
inline int popcount(uint8_t bits) noexcept {
	return __builtin_popcount8(bits);
}

inline int popcount(uint16_t bits) noexcept {
	return __builtin_popcount16(bits);
}

inline int popcount(uint32_t bits) noexcept {
	return __builtin_popcount(bits);
}

inline int popcount(uint64_t bits) noexcept {
	return __builtin_popcount(uint32_t(bits)) + __builtin_popcount(bits >> 32);
}


#ifdef __cplusplus
} // namespace ccwap
#endif

#endif // __CCWRAP_BIT_HPP__
