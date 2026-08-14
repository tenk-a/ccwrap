#include "test_cxx.hpp"

#if defined(__WATCOMC__)
#include <intrin.h>

TEST_CASE(intrin, bitscan) {
    unsigned long idx = 999;
    unsigned char nz;

    nz = _BitScanForward(&idx, 0x100ul);
    test_true( nz != 0 );
    test_eq( idx, 8ul );
    nz = _BitScanForward(&idx, 1ul);
    test_true( nz != 0 );
    test_eq( idx, 0ul );
    nz = _BitScanForward(&idx, 0ul);
    test_true( nz == 0 );

    nz = _BitScanReverse(&idx, 0x100ul);
    test_true( nz != 0 );
    test_eq( idx, 8ul );
    nz = _BitScanReverse(&idx, 0x80000000ul);
    test_true( nz != 0 );
    test_eq( idx, 31ul );
    nz = _BitScanReverse(&idx, 0ul);
    test_true( nz == 0 );
    test_pass("cxx03:bitscan");
}

TEST_CASE(intrin, interlocked) {
    long c = 3;
    long old;

    old = _InterlockedExchange(&c, 7);
    test_eq( old, 3l );  test_eq( c, 7l );

    old = _InterlockedExchangeAdd(&c, 5);
    test_eq( old, 7l );  test_eq( c, 12l );

    old = _InterlockedCompareExchange(&c, 20, 12);
    test_eq( old, 12l ); test_eq( c, 20l );
    old = _InterlockedCompareExchange(&c, 99, 12);
    test_eq( old, 20l ); test_eq( c, 20l );

    long ninc = _InterlockedIncrement(&c);
    test_eq( ninc, 21l );
    test_eq( c, 21l );
    long ndec = _InterlockedDecrement(&c);
    test_eq( ndec, 20l );
    test_eq( c, 20l );

    c = 0xF0;
    old = _InterlockedOr(&c, 0x0F);
    test_eq( old, 0xF0l ); test_eq( c, 0xFFl );
    old = _InterlockedAnd(&c, 0x3C);
    test_eq( old, 0xFFl ); test_eq( c, 0x3Cl );
    old = _InterlockedXor(&c, 0xFF);
    test_eq( old, 0x3Cl ); test_eq( c, 0xC3l );
    test_pass("cxx03:interlocked");
}

TEST_CASE(intrin, byteswap_and_misc) {
    test_eq( (unsigned)_byteswap_ushort(0x1234), 0x3412u );
    test_eq( _byteswap_ulong(0x12345678ul), 0x78563412ul );

    unsigned __int64 s64 = _byteswap_uint64(0x1122334455667788ULL);
    test_true( s64 == 0x8877665544332211ULL );

    unsigned __int64 t0 = __rdtsc();
    _mm_pause();
    unsigned __int64 t1 = __rdtsc();
    test_true( t1 >= t0 );

    int info[4];
    __cpuid(info, 0);
    test_true( info[0] >= 1 );

    _ReadWriteBarrier(); _ReadBarrier(); _WriteBarrier();
    test_true( true );

    test_pass("cxx03:byteswap and misc");
}

TEST_CASE(intrin, gcc_builtins) {
    test_eq( __builtin_ctz(0x100u), 8 );
    test_eq( __builtin_ctz(1u), 0 );
    test_eq( __builtin_clz(1u), 31 );
    test_eq( __builtin_clz(0x80000000u), 0 );

    test_eq( __builtin_popcount(0u), 0 );
    test_eq( __builtin_popcount(0xFFu), 8 );
    test_eq( __builtin_popcount(0xFFFFFFFFu), 32 );
    test_eq( __builtin_popcount(0x80000001u), 2 );

    test_eq( (unsigned)__builtin_bswap16(0x1234), 0x3412u );
    test_eq( __builtin_bswap32(0x12345678u), 0x78563412u );
    test_true( __builtin_bswap64(0x1122334455667788ULL) == 0x8877665544332211ULL );

    int x = 42;
    test_eq( __builtin_expect(x, 1), 42 );
    test_pass("cxx03:gcc builtins");
}
#else
TEST_CASE_SKIP(intrin, bitscan)
TEST_CASE_SKIP(intrin, interlocked)
TEST_CASE_SKIP(intrin, byteswap_and_misc)
TEST_CASE_SKIP(intrin, gcc_builtins)
#endif
