#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2020
#include <bit>
#include <cstdint>

TEST_CASE(bit, endian) {
    test_true( STD::endian::little != STD::endian::big );
    test_true( STD::endian::native == STD::endian::little
            || STD::endian::native == STD::endian::big );
    test_pass("cxx20:endian");
}

TEST_CASE(bit, ops_u8) {
    typedef STD::uint8_t U;
    const int N = 8;
    const U zero = 0, one = 1, top = 0x80, all = 0xFF, mix = 0xF0;

    test_eq( STD::popcount(zero), 0 );
    test_eq( STD::popcount(one), 1 );
    test_eq( STD::popcount(top), 1 );
    test_eq( STD::popcount(all), N );
    test_eq( STD::popcount(mix), 4 );
    test_pass("cxx20:popcount");

    test_eq( STD::countl_zero(zero), N );
    test_eq( STD::countl_zero(one), N - 1 );
    test_eq( STD::countl_zero(top), 0 );
    test_eq( STD::countl_zero(all), 0 );
    test_eq( STD::countl_zero(mix), 0 );
    test_pass("cxx20:countl_zero");

    test_eq( STD::countr_zero(zero), N );
    test_eq( STD::countr_zero(one), 0 );
    test_eq( STD::countr_zero(top), N - 1 );
    test_eq( STD::countr_zero(all), 0 );
    test_eq( STD::countr_zero(mix), 4 );
    test_pass("cxx20:countr_zero");

    test_eq( STD::countl_one(zero), 0 );
    test_eq( STD::countl_one(all), N );
    test_eq( STD::countl_one(top), 1 );
    test_eq( STD::countl_one(mix), 4 );
    test_pass("cxx20:countl_one");

    test_eq( STD::countr_one(zero), 0 );
    test_eq( STD::countr_one(all), N );
    test_eq( STD::countr_one(one), 1 );
    test_eq( STD::countr_one(U(0x0F)), 4 );
    test_pass("cxx20:countr_one");

    test_eq( STD::bit_width(zero), 0 );
    test_eq( STD::bit_width(one), 1 );
    test_eq( STD::bit_width(top), N );
    test_eq( STD::bit_width(all), N );
    test_eq( STD::bit_width(U(0x0F)), 4 );
    test_pass("cxx20:bit_width");

    test_true(  STD::has_single_bit(one) );
    test_true(  STD::has_single_bit(top) );
    test_true( !STD::has_single_bit(zero) );
    test_true( !STD::has_single_bit(all) );
    test_true( !STD::has_single_bit(U(6)) );
    test_pass("cxx20:has_single_bit");

    test_eq( (int)STD::bit_floor(zero), 0 );
    test_eq( (int)STD::bit_floor(one), 1 );
    test_eq( (int)STD::bit_floor(all), (int)top );
    test_eq( (int)STD::bit_floor(top), (int)top );
    test_eq( (int)STD::bit_floor(U(100)), 64 );
    test_pass("cxx20:bit_floor");

    test_eq( (int)STD::bit_ceil(zero), 1 );
    test_eq( (int)STD::bit_ceil(one), 1 );
    test_eq( (int)STD::bit_ceil(U(100)), 128 );
    test_eq( (int)STD::bit_ceil(top), (int)top );
    test_pass("cxx20:bit_ceil");

    test_eq( (int)STD::rotl(U(0x81), 4), 0x18 );
    test_eq( (int)STD::rotr(U(0x81), 4), 0x18 );
    test_eq( (int)STD::rotl(top, 1), (int)one );
    test_eq( (int)STD::rotr(one, 1), (int)top );
    test_pass("cxx20:rotr");
    test_eq( (int)STD::rotl(mix, 0), (int)mix );
    test_eq( (int)STD::rotl(mix, N), (int)mix );
    test_eq( (int)STD::rotl(mix, N + 1), (int)STD::rotl(mix, 1) );
    test_eq( (int)STD::rotl(mix, -1), (int)STD::rotr(mix, 1) );
    test_eq( (int)STD::rotr(mix, -1), (int)STD::rotl(mix, 1) );
    test_eq( (int)STD::rotl(all, 3), (int)all );
    test_pass("cxx20:rotl");

#if defined(__cpp_lib_byteswap)
    test_eq( (int)STD::byteswap(U(0xAB)), 0xAB );
    test_pass("cxx23:byteswap");
#else
    TEST_NOTE("byteswap is C++23; this library does not have it at this level");
    test_skip("cxx23:byteswap");
#endif
}

TEST_CASE(bit, ops_u16) {
    typedef STD::uint16_t U;
    const int N = 16;
    const U zero = 0, one = 1, top = 0x8000, all = 0xFFFF, mix = 0xFF00;

    test_eq( STD::popcount(zero), 0 );
    test_eq( STD::popcount(one), 1 );
    test_eq( STD::popcount(top), 1 );
    test_eq( STD::popcount(all), N );
    test_eq( STD::popcount(mix), 8 );
    test_pass("cxx20:popcount#u16");

    test_eq( STD::countl_zero(zero), N );
    test_eq( STD::countl_zero(one), N - 1 );
    test_eq( STD::countl_zero(top), 0 );
    test_eq( STD::countl_zero(all), 0 );
    test_eq( STD::countl_zero(U(0x00FF)), 8 );
    test_pass("cxx20:countl_zero#u16");

    test_eq( STD::countr_zero(zero), N );
    test_eq( STD::countr_zero(one), 0 );
    test_eq( STD::countr_zero(top), N - 1 );
    test_eq( STD::countr_zero(all), 0 );
    test_eq( STD::countr_zero(mix), 8 );
    test_pass("cxx20:countr_zero#u16");

    test_eq( STD::countl_one(zero), 0 );
    test_eq( STD::countl_one(all), N );
    test_eq( STD::countl_one(top), 1 );
    test_eq( STD::countl_one(mix), 8 );
    test_pass("cxx20:countl_one#u16");

    test_eq( STD::countr_one(zero), 0 );
    test_eq( STD::countr_one(all), N );
    test_eq( STD::countr_one(one), 1 );
    test_eq( STD::countr_one(U(0x00FF)), 8 );
    test_pass("cxx20:countr_one#u16");

    test_eq( STD::bit_width(zero), 0 );
    test_eq( STD::bit_width(one), 1 );
    test_eq( STD::bit_width(top), N );
    test_eq( STD::bit_width(all), N );
    test_eq( STD::bit_width(U(0x00FF)), 8 );
    test_pass("cxx20:bit_width#u16");

    test_true(  STD::has_single_bit(one) );
    test_true(  STD::has_single_bit(top) );
    test_true( !STD::has_single_bit(zero) );
    test_true( !STD::has_single_bit(all) );
    test_pass("cxx20:has_single_bit#u16");

    test_eq( (long)STD::bit_floor(zero), 0L );
    test_eq( (long)STD::bit_floor(one), 1L );
    test_eq( (long)STD::bit_floor(all), (long)top );
    test_eq( (long)STD::bit_floor(top), (long)top );
    test_eq( (long)STD::bit_floor(U(1000)), 512L );
    test_pass("cxx20:bit_floor#u16");

    test_eq( (long)STD::bit_ceil(zero), 1L );
    test_eq( (long)STD::bit_ceil(one), 1L );
    test_eq( (long)STD::bit_ceil(U(1000)), 1024L );
    test_eq( (long)STD::bit_ceil(top), (long)top );
    test_pass("cxx20:bit_ceil#u16");

    test_eq( (long)STD::rotl(U(0x0081), 8), 0x8100L );
    test_eq( (long)STD::rotr(U(0x8100), 8), 0x0081L );
    test_eq( (long)STD::rotl(top, 1), (long)one );
    test_eq( (long)STD::rotr(one, 1), (long)top );
    test_pass("cxx20:rotr#u16");
    test_eq( (long)STD::rotl(mix, 0), (long)mix );
    test_eq( (long)STD::rotl(mix, N), (long)mix );
    test_eq( (long)STD::rotl(mix, N + 4), (long)STD::rotl(mix, 4) );
    test_eq( (long)STD::rotl(mix, -4), (long)STD::rotr(mix, 4) );
    test_eq( (long)STD::rotr(mix, -4), (long)STD::rotl(mix, 4) );
    test_eq( (long)STD::rotl(all, 5), (long)all );
    test_pass("cxx20:rotl#u16");

#if defined(__cpp_lib_byteswap)
    test_eq( (long)STD::byteswap(U(0x1234)), 0x3412L );
    test_eq( (long)STD::byteswap(STD::byteswap(mix)), (long)mix );
    test_pass("cxx23:byteswap#u16");
#else
    TEST_NOTE("byteswap is C++23; this library does not have it at this level");
    test_skip("cxx23:byteswap#u16");
#endif
}

TEST_CASE(bit, ops_u32) {
    typedef STD::uint32_t U;
    const int N = 32;
    const U zero = 0u, one = 1u, top = 0x80000000u, all = 0xFFFFFFFFu, mix = 0xF0F0F0F0u;

    test_eq( STD::popcount(zero), 0 );
    test_eq( STD::popcount(one), 1 );
    test_eq( STD::popcount(top), 1 );
    test_eq( STD::popcount(all), N );
    test_eq( STD::popcount(mix), 16 );
    test_pass("cxx20:popcount#u32");

    test_eq( STD::countl_zero(zero), N );
    test_eq( STD::countl_zero(one), N - 1 );
    test_eq( STD::countl_zero(top), 0 );
    test_eq( STD::countl_zero(all), 0 );
    test_eq( STD::countl_zero(U(0x0000FFFFu)), 16 );
    test_pass("cxx20:countl_zero#u32");

    test_eq( STD::countr_zero(zero), N );
    test_eq( STD::countr_zero(one), 0 );
    test_eq( STD::countr_zero(top), N - 1 );
    test_eq( STD::countr_zero(all), 0 );
    test_eq( STD::countr_zero(mix), 4 );
    test_pass("cxx20:countr_zero#u32");

    test_eq( STD::countl_one(zero), 0 );
    test_eq( STD::countl_one(all), N );
    test_eq( STD::countl_one(top), 1 );
    test_eq( STD::countl_one(mix), 4 );
    test_pass("cxx20:countl_one#u32");

    test_eq( STD::countr_one(zero), 0 );
    test_eq( STD::countr_one(all), N );
    test_eq( STD::countr_one(one), 1 );
    test_eq( STD::countr_one(U(0x7u)), 3 );
    test_pass("cxx20:countr_one#u32");

    test_eq( STD::bit_width(zero), 0 );
    test_eq( STD::bit_width(one), 1 );
    test_eq( STD::bit_width(top), N );
    test_eq( STD::bit_width(all), N );
    test_eq( STD::bit_width(U(255u)), 8 );
    test_pass("cxx20:bit_width#u32");

    test_true(  STD::has_single_bit(one) );
    test_true(  STD::has_single_bit(top) );
    test_true(  STD::has_single_bit(U(8u)) );
    test_true( !STD::has_single_bit(zero) );
    test_true( !STD::has_single_bit(all) );
    test_true( !STD::has_single_bit(U(6u)) );
    test_pass("cxx20:has_single_bit#u32");

    test_eq( STD::bit_floor(zero), 0u );
    test_eq( STD::bit_floor(one), 1u );
    test_eq( STD::bit_floor(all), top );
    test_eq( STD::bit_floor(top), top );
    test_eq( STD::bit_floor(U(100u)), 64u );
    test_pass("cxx20:bit_floor#u32");

    test_eq( STD::bit_ceil(zero), 1u );
    test_eq( STD::bit_ceil(one), 1u );
    test_eq( STD::bit_ceil(U(100u)), 128u );
    test_eq( STD::bit_ceil(top), top );
    test_pass("cxx20:bit_ceil#u32");

    test_eq( STD::rotl(U(0x00000081u), 28), 0x10000008u );
    test_eq( STD::rotr(U(0x00000081u), 4), 0x10000008u );
    test_eq( STD::rotl(top, 1), one );
    test_eq( STD::rotr(one, 1), top );
    test_pass("cxx20:rotr#u32");
    test_eq( STD::rotl(mix, 0), mix );
    test_eq( STD::rotl(mix, N), mix );
    test_eq( STD::rotr(mix, N), mix );
    test_eq( STD::rotl(mix, N + 4), STD::rotl(mix, 4) );
    test_eq( STD::rotl(mix, -4), STD::rotr(mix, 4) );
    test_eq( STD::rotr(mix, -4), STD::rotl(mix, 4) );
    test_eq( STD::rotl(all, 7), all );
    test_pass("cxx20:rotl#u32");

#if defined(__cpp_lib_byteswap)
    test_eq( STD::byteswap(U(0x11223344u)), 0x44332211u );
    test_eq( STD::byteswap(STD::byteswap(mix)), mix );
    test_pass("cxx23:byteswap#u32");
#else
    TEST_NOTE("byteswap is C++23; this library does not have it at this level");
    test_skip("cxx23:byteswap#u32");
#endif
}

TEST_CASE(bit, ops_u64) {
    typedef STD::uint64_t U;
    const int N = 64;
    const U zero = 0ull, one = 1ull;
    const U top  = 0x8000000000000000ull;
    const U all  = 0xFFFFFFFFFFFFFFFFull;
    const U mix  = 0xF0F0F0F0F0F0F0F0ull;

    test_eq( STD::popcount(zero), 0 );
    test_eq( STD::popcount(one), 1 );
    test_eq( STD::popcount(top), 1 );
    test_eq( STD::popcount(all), N );
    test_eq( STD::popcount(mix), 32 );
    test_pass("cxx20:popcount#u64");

    test_eq( STD::countl_zero(zero), N );
    test_eq( STD::countl_zero(one), N - 1 );
    test_eq( STD::countl_zero(top), 0 );
    test_eq( STD::countl_zero(all), 0 );
    test_eq( STD::countl_zero(U(0xFFFFFFFFull)), 32 );
    test_pass("cxx20:countl_zero#u64");

    test_eq( STD::countr_zero(zero), N );
    test_eq( STD::countr_zero(one), 0 );
    test_eq( STD::countr_zero(top), N - 1 );
    test_eq( STD::countr_zero(all), 0 );
    test_eq( STD::countr_zero(mix), 4 );
    test_pass("cxx20:countr_zero#u64");

    test_eq( STD::countl_one(zero), 0 );
    test_eq( STD::countl_one(all), N );
    test_eq( STD::countl_one(top), 1 );
    test_eq( STD::countl_one(mix), 4 );
    test_pass("cxx20:countl_one#u64");

    test_eq( STD::countr_one(zero), 0 );
    test_eq( STD::countr_one(all), N );
    test_eq( STD::countr_one(one), 1 );
    test_eq( STD::countr_one(U(0xFFull)), 8 );
    test_pass("cxx20:countr_one#u64");

    test_eq( STD::bit_width(zero), 0 );
    test_eq( STD::bit_width(one), 1 );
    test_eq( STD::bit_width(top), N );
    test_eq( STD::bit_width(all), N );
    test_eq( STD::bit_width(U(0xFFFFFFFFull)), 32 );
    test_pass("cxx20:bit_width#u64");

    test_true(  STD::has_single_bit(one) );
    test_true(  STD::has_single_bit(top) );
    test_true( !STD::has_single_bit(zero) );
    test_true( !STD::has_single_bit(all) );
    test_pass("cxx20:has_single_bit#u64");

    test_true( STD::bit_floor(zero) == zero );
    test_true( STD::bit_floor(one) == one );
    test_true( STD::bit_floor(all) == top );
    test_true( STD::bit_floor(top) == top );
    test_true( STD::bit_floor(U(0x1FFFFFFFFull)) == 0x100000000ull );
    test_pass("cxx20:bit_floor#u64");

    test_true( STD::bit_ceil(zero) == one );
    test_true( STD::bit_ceil(one) == one );
    test_true( STD::bit_ceil(U(0x100000001ull)) == 0x200000000ull );
    test_true( STD::bit_ceil(top) == top );
    test_pass("cxx20:bit_ceil#u64");

    test_true( STD::rotl(one, 60) == 0x1000000000000000ull );
    test_true( STD::rotl(top, 1) == one );
    test_true( STD::rotr(one, 1) == top );
    test_pass("cxx20:rotr#u64");
    test_true( STD::rotl(mix, 0) == mix );
    test_true( STD::rotl(mix, N) == mix );
    test_true( STD::rotr(mix, N) == mix );
    test_true( STD::rotl(mix, N + 4) == STD::rotl(mix, 4) );
    test_true( STD::rotl(mix, -4) == STD::rotr(mix, 4) );
    test_true( STD::rotr(mix, -4) == STD::rotl(mix, 4) );
    test_true( STD::rotl(all, 9) == all );
    test_true( STD::rotl(U(0x0000000000000081ull), 60) == 0x1000000000000008ull );
    test_pass("cxx20:rotl#u64");

#if defined(__cpp_lib_byteswap)
    test_true( STD::byteswap(U(0x0102030405060708ull)) == 0x0807060504030201ull );
    test_true( STD::byteswap(STD::byteswap(mix)) == mix );
    test_pass("cxx23:byteswap#u64");
#else
    TEST_NOTE("byteswap is C++23; this library does not have it at this level");
    test_skip("cxx23:byteswap#u64");
#endif
}

TEST_CASE(bit, bit_cast) {
    STD::uint32_t bits = 0x40490FDBu;
    float f = STD::bit_cast<float>(bits);
    test_true( f > 3.14f && f < 3.15f );
    test_eq( STD::bit_cast<STD::uint32_t>(f), bits );

    STD::uint64_t dbits = 0x400921FB54442D18ull;
    double d = STD::bit_cast<double>(dbits);
    test_true( d > 3.14159 && d < 3.14160 );
    test_true( STD::bit_cast<STD::uint64_t>(d) == dbits );

    test_eq( STD::bit_cast<STD::uint32_t>(STD::int32_t(-1)), 0xFFFFFFFFu );
    test_eq( (int)STD::bit_cast<STD::int8_t>(STD::uint8_t(0xFF)), -1 );
    test_true( STD::bit_cast<STD::uint64_t>(STD::int64_t(-1)) == 0xFFFFFFFFFFFFFFFFull );
    test_pass("cxx20:bit_cast");
}
#endif

#include <bit>
#include <cstdint>
#include <cstring>

#if TEST_TARGET_CXX >= 2020
TEST_CASE(bit, endian_chk) {

    test_true( STD::endian::little != STD::endian::big );
    test_true( STD::endian::native == STD::endian::little ||
               STD::endian::native == STD::endian::big );

    STD::uint32_t v = 0x01020304u;
    unsigned char b[4];
    STD::memcpy(b, &v, sizeof v);
    if (STD::endian::native == STD::endian::little)
        test_eq( (int)b[0], 0x04 );
    else
        test_eq( (int)b[0], 0x01 );
    test_pass("cxx20:endian#chk");
}
#else
TEST_CASE(bit, endian_chk) {
    TEST_NOTE("std::endian is C++20");
    test_skip("cxx20:endian#chk");
}
#endif

#if TEST_TARGET_CXX >= 2020
TEST_CASE(bit, bit_cast_chk) {
    STD::uint32_t bits = 0x40490FDBu;
    float f = STD::bit_cast<float>(bits);
    test_true( f > 3.14f && f < 3.15f );
    test_eq( STD::bit_cast<STD::uint32_t>(f), bits );

    struct Pair { STD::uint16_t a, b; };
    Pair p = STD::bit_cast<Pair>(STD::uint32_t(0x00020001u));
    test_eq( (int)(STD::endian::native == STD::endian::little ? p.a : p.b), 1 );
    test_pass("cxx20:bit_cast#chk");
}
#else
TEST_CASE(bit, bit_cast_chk) {
    TEST_NOTE("bit_cast is C++20");
    test_skip("cxx20:bit_cast#chk");
}
#endif

#if TEST_TARGET_CXX >= 2020
TEST_CASE(bit, byteswap) {
#if defined(__cpp_lib_byteswap)
    test_eq( (int)STD::byteswap(STD::uint8_t(0x12)), 0x12 );
    test_eq( (int)STD::byteswap(STD::uint16_t(0x1234)), 0x3412 );
    test_eq( STD::byteswap(STD::uint32_t(0x11223344u)), 0x44332211u );
    test_eq( STD::byteswap(STD::uint64_t(0x0102030405060708ull)), 0x0807060504030201ull );
    test_eq( (int)STD::byteswap(STD::int16_t(0x0102)), 0x0201 );
    test_pass("cxx23:byteswap#chk");
#else
    TEST_NOTE("byteswap is C++23; this library does not have it at this level");
    test_skip("cxx23:byteswap#chk");
#endif
}

TEST_CASE(bit, single_bit_ceil_floor_width) {
    test_true(  STD::has_single_bit(1u) );
    test_true(  STD::has_single_bit(STD::uint8_t(0x80)) );
    test_true( !STD::has_single_bit(0u) );
    test_true( !STD::has_single_bit(3u) );
    test_true(  STD::has_single_bit(1ull << 40) );
    test_pass("cxx20:has_single_bit#chk");

    test_eq( STD::bit_ceil(0u), 1u );
    test_eq( STD::bit_ceil(1u), 1u );
    test_eq( STD::bit_ceil(5u), 8u );
    test_eq( (int)STD::bit_ceil(STD::uint8_t(17)), 32 );
    test_eq( STD::bit_ceil((1ull << 40) + 1), 1ull << 41 );
    test_pass("cxx20:bit_ceil#chk");

    test_eq( STD::bit_floor(0u), 0u );
    test_eq( STD::bit_floor(1u), 1u );
    test_eq( STD::bit_floor(20u), 16u );
    test_eq( (int)STD::bit_floor(STD::uint8_t(200)), 128 );
    test_eq( STD::bit_floor((1ull << 40) + 5), 1ull << 40 );
    test_pass("cxx20:bit_floor#chk");

    test_eq( STD::bit_width(0u), 0 );
    test_eq( STD::bit_width(1u), 1 );
    test_eq( STD::bit_width(0x0Fu), 4 );
    test_eq( STD::bit_width(STD::uint8_t(0xFF)), 8 );
    test_eq( STD::bit_width(STD::uint64_t(1) << 40), 41 );
    test_pass("cxx20:bit_width#chk");
}

TEST_CASE(bit, rotate) {
    test_eq( (int)STD::rotl(STD::uint8_t(0x81), 1), 0x03 );
    test_eq( (int)STD::rotr(STD::uint8_t(0x81), 1), 0xC0 );
    test_pass("cxx20:rotr#chk");

    test_eq( STD::rotl(0x80000001u, 1), 0x00000003u );
    test_eq( STD::rotr(0x80000001u, 1), 0xC0000000u );

    test_eq( STD::rotl(1u, 0), 1u );
    test_eq( STD::rotl(1u, 32), 1u );
    test_eq( STD::rotl(1u, -1), 1u << 31 );
    test_eq( STD::rotr(1u, -1), 2u );

    test_eq( STD::rotl(STD::uint64_t(1) << 63, 1), STD::uint64_t(1) );
    test_eq( STD::rotr(STD::uint64_t(1), 1), STD::uint64_t(1) << 63 );
    test_pass("cxx20:rotl#chk");
}

TEST_CASE(bit, counting) {
    test_eq( STD::countl_zero(0u), 32 );
    test_eq( STD::countl_zero(1u), 31 );
    test_eq( STD::countl_zero(STD::uint8_t(0)), 8 );
    test_eq( STD::countl_zero(STD::uint8_t(0x0F)), 4 );
    test_eq( STD::countl_zero(STD::uint64_t(1) << 63), 0 );
    test_pass("cxx20:countl_zero#chk");

    test_eq( STD::countl_one(0u), 0 );
    test_eq( STD::countl_one(0xFFFFFFFFu), 32 );
    test_eq( STD::countl_one(STD::uint8_t(0xF0)), 4 );
    test_pass("cxx20:countl_one#chk");

    test_eq( STD::countr_zero(0u), 32 );
    test_eq( STD::countr_zero(8u), 3 );
    test_eq( STD::countr_zero(STD::uint8_t(0x10)), 4 );
    test_eq( STD::countr_zero(STD::uint64_t(1) << 40), 40 );
    test_pass("cxx20:countr_zero#chk");

    test_eq( STD::countr_one(0u), 0 );
    test_eq( STD::countr_one(0x0Fu), 4 );
    test_eq( STD::countr_one(STD::uint8_t(0xFF)), 8 );
    test_pass("cxx20:countr_one#chk");

    test_eq( STD::popcount(0u), 0 );
    test_eq( STD::popcount(0x0Fu), 4 );
    test_eq( STD::popcount(STD::uint8_t(0xFF)), 8 );
    test_eq( STD::popcount(0xFFFFFFFFu), 32 );
    test_eq( STD::popcount(~STD::uint64_t(0)), 64 );
    test_pass("cxx20:popcount#chk");
}

#else
TEST_CASE(bit, byteswap) {
    TEST_NOTE("<bit> is C++20");
    test_skip("cxx23:byteswap#chk");
}
TEST_CASE(bit, single_bit_ceil_floor_width) {
    TEST_NOTE("<bit> is C++20");
    test_skip("cxx20:has_single_bit#chk");
    test_skip("cxx20:bit_ceil#chk");
    test_skip("cxx20:bit_floor#chk");
    test_skip("cxx20:bit_width#chk");
}
TEST_CASE(bit, rotate) {
    TEST_NOTE("<bit> is C++20");
    test_skip("cxx20:rotr#chk");
    test_skip("cxx20:rotl#chk");
}
TEST_CASE(bit, counting) {
    TEST_NOTE("<bit> is C++20");
    test_skip("cxx20:countl_zero#chk");
    test_skip("cxx20:countl_one#chk");
    test_skip("cxx20:countr_zero#chk");
    test_skip("cxx20:countr_one#chk");
    test_skip("cxx20:popcount#chk");
}
#endif

TEST_CASE(bit, bit_permutations_cxx26) {
    TEST_NOTE("C++26 <bit> additions (P3104); no library here defines them");
    test_skip("cxx26:bit_compress");
    test_skip("cxx26:bit_expand");
    test_skip("cxx26:bit_repeat");
    test_skip("cxx26:bit_reverse");
    test_skip("cxx26:shl");
    test_skip("cxx26:shr");
}


#if TEST_TARGET_CXX >= 2020 && defined(__cpp_lib_bitops)
TEST_CASE(bit, bit_ceil_edges_cxx20) {
    test_eq( (int)STD::bit_ceil(0u), 1 );
    test_eq( (int)STD::bit_ceil(1u), 1 );
    test_eq( (int)STD::bit_ceil((STD::uint8_t)0), 1 );
    test_pass("cxx20:bit_ceil#zero-and-one");

    test_eq( (int)STD::bit_ceil(3u), 4 );
    test_eq( (int)STD::bit_ceil(5u), 8 );
    test_eq( (int)STD::bit_ceil(9u), 16 );
    test_eq( (int)STD::bit_ceil(4u), 4 );
    test_true( STD::bit_ceil(0x7FFFFFFFu) == 0x80000000u );
    test_pass("cxx20:bit_ceil#non-power");
}

TEST_CASE(bit, rotate_edges_cxx20) {
    STD::uint8_t v = 0x81;
    test_eq( (int)STD::rotl(v, -1), (int)STD::rotr(v, 1) );
    test_eq( (int)STD::rotl(v, -1), 0xC0 );
    test_pass("cxx20:rotl#negative");

    test_eq( (int)STD::rotr(v, -1), (int)STD::rotl(v, 1) );
    test_eq( (int)STD::rotr(v, -1), 0x03 );
    test_pass("cxx20:rotr#negative");

    test_eq( (int)STD::rotl(v, 8), (int)v );
    test_eq( (int)STD::rotl(v, 9), (int)STD::rotl(v, 1) );
    test_eq( (int)STD::rotl(v, 0), (int)v );
    test_true( STD::rotl(0x12345678u, 32) == 0x12345678u );
    test_true( STD::rotl(0x12345678u, 36) == STD::rotl(0x12345678u, 4) );
    test_pass("cxx20:rotl#modulo-width");

    test_eq( (int)STD::rotr(v, 8), (int)v );
    test_eq( (int)STD::rotr(v, 9), (int)STD::rotr(v, 1) );
    test_true( STD::rotr(0x12345678u, 32) == 0x12345678u );
    test_true( STD::rotr(0x12345678u, 36) == STD::rotr(0x12345678u, 4) );
    test_pass("cxx20:rotr#modulo-width");
}
#else
TEST_CASE(bit, bit_ceil_edges_cxx20) {
    test_skip("cxx20:bit_ceil#zero-and-one");
    test_skip("cxx20:bit_ceil#non-power");
}
TEST_CASE(bit, rotate_edges_cxx20) {
    test_skip("cxx20:rotl#negative");
    test_skip("cxx20:rotr#negative");
    test_skip("cxx20:rotl#modulo-width");
    test_skip("cxx20:rotr#modulo-width");
}
#endif
