// ccwrap watcom: <intrin.h> -- MSVC-/GCC-compatible compiler intrinsics for Open Watcom.
//
// Open Watcom has no <intrin.h> and no __builtin_* intrinsics; the only way to reach the
// hardware instructions the standard-library backports want (bit scan, interlocked RMW,
// byte swap, rdtsc, cpuid, ...) is inline machine code via `#pragma aux`. This header
// exposes them under the names MSVC's <intrin.h> and GCC's __builtin_* use, so portable
// code written for either compiler builds unchanged on Watcom.
#ifndef _CCW_WATCOM_INTRIN_H
#define _CCW_WATCOM_INTRIN_H

#if defined(__WATCOMC__) && defined(__386__)

#if !defined(_M_IX86)
#  define _M_IX86 600
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern unsigned char _BitScanForward(unsigned long* _Index, unsigned long _Mask);
#pragma aux _BitScanForward =   \
    "bsf eax, ecx"              \
    "mov [edx], eax"            \
    "setne al"                  \
    parm [edx] [ecx]            \
    value [al]                  \
    modify [eax];
extern unsigned char _BitScanReverse(unsigned long* _Index, unsigned long _Mask);
#pragma aux _BitScanReverse =   \
    "bsr eax, ecx"              \
    "mov [edx], eax"            \
    "setne al"                  \
    parm [edx] [ecx]            \
    value [al]                  \
    modify [eax];

extern long _InterlockedExchange(long volatile* _Target, long _Value);
#pragma aux _InterlockedExchange =  \
    "lock xchg [edx], eax"          \
    parm [edx] [eax]                \
    value [eax]                     \
    modify [eax];

extern unsigned short _byteswap_ushort(unsigned short _Val);
#pragma aux _byteswap_ushort = "rol ax, 8" parm [ax] value [ax] modify [ax];

extern void _mm_pause(void);                  // rep nop (0xF3 0x90) -- assembled as bytes
#pragma aux _mm_pause = 0xf3 0x90;            // in case the inline assembler lacks `pause`

extern void __debugbreak(void);
#pragma aux __debugbreak = "int 3";

#if _M_IX86 >= 400

extern long _InterlockedExchangeAdd(long volatile* _Addend, long _Value);
#pragma aux _InterlockedExchangeAdd =  \
    "lock xadd [edx], eax"             \
    parm [edx] [eax]                   \
    value [eax]                        \
    modify [eax];

extern long _InterlockedCompareExchange(long volatile* _Dest, long _Exchange, long _Comparand);
#pragma aux _InterlockedCompareExchange =  \
    "lock cmpxchg [edx], ecx"              \
    parm [edx] [ecx] [eax]                 \
    value [eax]                            \
    modify [eax];

extern long _InterlockedIncrement(long volatile* _Addend);   // returns the new value
#pragma aux _InterlockedIncrement =  \
    "mov eax, 1"                     \
    "lock xadd [edx], eax"           \
    "inc eax"                        \
    parm [edx]                       \
    value [eax]                      \
    modify [eax];

extern long _InterlockedDecrement(long volatile* _Addend);   // returns the new value
#pragma aux _InterlockedDecrement =  \
    "mov eax, 0FFFFFFFFh"            \
    "lock xadd [edx], eax"           \
    "dec eax"                        \
    parm [edx]                       \
    value [eax]                      \
    modify [eax];

extern unsigned long _byteswap_ulong(unsigned long _Val);
#pragma aux _byteswap_ulong = "bswap eax" parm [eax] value [eax] modify [eax];

extern unsigned __int64 _byteswap_uint64(unsigned __int64 _Val);
#pragma aux _byteswap_uint64 =  \
    "bswap eax"                 \
    "bswap edx"                 \
    "xchg eax, edx"             \
    parm [edx eax]              \
    value [edx eax]             \
    modify [eax edx];

#endif // _M_IX86 >= 400

#if _M_IX86 >= 500

extern unsigned __int64 __rdtsc(void);       // cycle counter (edx:eax)
#pragma aux __rdtsc = "rdtsc" value [edx eax] modify [edx eax];

extern void __cpuid(int _CpuInfo[4], int _FunctionId);
#pragma aux __cpuid =           \
    "cpuid"                     \
    "mov [edi], eax"            \
    "mov [edi+4], ebx"          \
    "mov [edi+8], ecx"          \
    "mov [edi+12], edx"         \
    parm [edi] [eax]            \
    modify [eax ebx ecx edx];

#endif // _M_IX86 >= 500

#ifdef __cplusplus
}  // extern "C"
#endif

#if _M_IX86 < 400

// 386: no xadd / cmpxchg / bswap. `lock xchg` is all the atomicity the part has, so the
// read-modify-write forms run under a spin lock built from it. doc/src/watcom/std/intrin.h.md
// states what that guarantees (and what it does not, in C).
#ifdef __cplusplus
inline long* __ccw_ilk_lock386() { static long __ccw_lk = 0; return &__ccw_lk; }
#else
static long __ccw_ilk_lk386_ = 0;
static __inline long* __ccw_ilk_lock386(void) { return &__ccw_ilk_lk386_; }
#endif

static __inline void __ccw_ilk_acquire386(void) {
    while (_InterlockedExchange(__ccw_ilk_lock386(), 1) != 0) _mm_pause();
}
static __inline void __ccw_ilk_release386(void) { _InterlockedExchange(__ccw_ilk_lock386(), 0); }

static __inline long _InterlockedExchangeAdd(long volatile* _Addend, long _Value) {
    long __old;
    __ccw_ilk_acquire386();
    __old = *_Addend;
    *_Addend = __old + _Value;
    __ccw_ilk_release386();
    return __old;
}
static __inline long _InterlockedCompareExchange(long volatile* _Dest, long _Exchange, long _Comparand) {
    long __old;
    __ccw_ilk_acquire386();
    __old = *_Dest;
    if (__old == _Comparand) *_Dest = _Exchange;
    __ccw_ilk_release386();
    return __old;
}
static __inline long _InterlockedIncrement(long volatile* _Addend) { return _InterlockedExchangeAdd(_Addend,  1) + 1; }
static __inline long _InterlockedDecrement(long volatile* _Addend) { return _InterlockedExchangeAdd(_Addend, -1) - 1; }

static __inline unsigned long _byteswap_ulong(unsigned long _Val) {
    return ((_Val & 0x000000FFul) << 24) | ((_Val & 0x0000FF00ul) << 8)
         | ((_Val & 0x00FF0000ul) >> 8)  | ((_Val & 0xFF000000ul) >> 24);
}
static __inline unsigned __int64 _byteswap_uint64(unsigned __int64 _Val) {
    return ((unsigned __int64)_byteswap_ulong((unsigned long)_Val) << 32)
         |  (unsigned __int64)_byteswap_ulong((unsigned long)(_Val >> 32));
}

#endif // _M_IX86 < 400

static __inline long _InterlockedOr(long volatile* _Value, long _Mask) {
    long __o, __n;
    do { __o = *_Value; __n = __o | _Mask; } while (_InterlockedCompareExchange(_Value, __n, __o) != __o);
    return __o;
}
static __inline long _InterlockedAnd(long volatile* _Value, long _Mask) {
    long __o, __n;
    do { __o = *_Value; __n = __o & _Mask; } while (_InterlockedCompareExchange(_Value, __n, __o) != __o);
    return __o;
}
static __inline long _InterlockedXor(long volatile* _Value, long _Mask) {
    long __o, __n;
    do { __o = *_Value; __n = __o ^ _Mask; } while (_InterlockedCompareExchange(_Value, __n, __o) != __o);
    return __o;
}

static __inline void _ReadWriteBarrier(void) {}
static __inline void _ReadBarrier(void) {}
static __inline void _WriteBarrier(void) {}

static __inline int __builtin_ctz(unsigned int __x) {
    unsigned long __i;
    _BitScanForward(&__i, (unsigned long)__x);
    return (int)__i;
}
static __inline int __builtin_clz(unsigned int __x) {
    unsigned long __i;
    _BitScanReverse(&__i, (unsigned long)__x);
    return 31 - (int)__i;
}
static __inline int __builtin_popcount(unsigned int __x) {
    __x = __x - ((__x >> 1) & 0x55555555u);
    __x = (__x & 0x33333333u) + ((__x >> 2) & 0x33333333u);
    __x = (__x + (__x >> 4)) & 0x0f0f0f0fu;
    return (int)((__x * 0x01010101u) >> 24);
}
static __inline unsigned short __builtin_bswap16(unsigned short __x) { return _byteswap_ushort(__x); }
static __inline unsigned int   __builtin_bswap32(unsigned int __x)   { return (unsigned int)_byteswap_ulong((unsigned long)__x); }
static __inline unsigned __int64 __builtin_bswap64(unsigned __int64 __x) { return _byteswap_uint64(__x); }

#define __builtin_expect(expr, c) (expr)

#endif // __WATCOMC__ && __386__

#endif // _CCW_WATCOM_INTRIN_H
