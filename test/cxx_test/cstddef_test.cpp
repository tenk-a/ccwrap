#include "test_cxx.hpp"

#include <cstddef>

TEST_CASE(cstddef, types) {
    STD_NS size_t s = 5;
    test_eq( s, 5u );
    test_true( sizeof(STD_NS size_t) >= 4u );
    test_pass("cxx03:size_t (basic)");

    STD_NS ptrdiff_t d = -3;
    test_eq( (long long)d, -3 );
    test_true( sizeof(STD_NS ptrdiff_t) >= 4u );
    test_pass("cxx03:ptrdiff_t (basic)");

    struct S { char a; int b; double c; };
    test_eq( offsetof(S, a), 0u );
    test_true( offsetof(S, b) >= sizeof(char) );
    test_true( offsetof(S, c) >= offsetof(S, b) );
    test_pass("cxx03:offsetof (basic)");

    void* nil = NULL;
    test_true( nil == 0 );
    test_pass("cxx03:NULL (basic)");

    test_true( sizeof(STD_NS max_align_t) >= sizeof(double) );
}

#if TEST_TARGET_CXX >= 2017 && (!defined(_MSC_VER) || _MSC_VER >= 1700)
TEST_CASE(cstddef, byte) {
    STD::byte b = STD::byte(0x0f);
    test_eq( STD::to_integer<int>(b), 15 );
    b = b | STD::byte(0xf0);
    test_eq( STD::to_integer<int>(b), 255 );
    b = b & STD::byte(0x0f);
    test_eq( STD::to_integer<int>(b), 15 );
    b = b ^ STD::byte(0xff);
    test_eq( STD::to_integer<int>(b), 240 );
    b = ~STD::byte(0x00);
    test_eq( STD::to_integer<int>(b), 255 );
    b = STD::byte(1);
    b <<= 3;
    test_eq( STD::to_integer<int>(b), 8 );
    b >>= 1;
    test_eq( STD::to_integer<int>(b), 4 );
    test_pass("cxx17:byte + operators (basic)");
}
#endif

#if TEST_TARGET_CXX >= 2011
#include <cstddef>
#include <type_traits>

namespace {

struct Off {
    char        c;
    int         i;
    double      d;
    char        tail[4];
};

#if _TST_HAS_BYTE
int as_int(STD::byte b) { return STD::to_integer<int>(b); }
#endif

template <class _Want, class _Got>
bool same_type(const _Got&) { return STD::is_same<_Want, _Got>::value; }

}

TEST_CASE(cstddef, size_t_and_ptrdiff_t) {
    test_true( same_type<STD_NS size_t>(sizeof(int)) );
    test_true( STD::is_unsigned<STD_NS size_t>::value );
    test_true( STD::is_integral<STD_NS size_t>::value );
    test_true( sizeof(STD_NS size_t) >= sizeof(void*) );

    STD_NS size_t n = sizeof(Off);
    test_true( n >= sizeof(double) );
    test_pass("cxx03:size_t (traits)");

    int arr[8] = {0};
    test_true( same_type<STD_NS ptrdiff_t>(&arr[4] - &arr[0]) );
    test_true( STD::is_signed<STD_NS ptrdiff_t>::value );
    test_true( STD::is_integral<STD_NS ptrdiff_t>::value );
    test_eq( sizeof(STD_NS ptrdiff_t), sizeof(STD_NS size_t) );

    STD_NS ptrdiff_t d = &arr[7] - &arr[2];
    test_eq( d, (STD_NS ptrdiff_t)5 );
    STD_NS ptrdiff_t dn = &arr[2] - &arr[7];
    test_eq( dn, (STD_NS ptrdiff_t)-5 );
    test_pass("cxx03:ptrdiff_t (traits + pointer difference)");
}

#if !_TST_HAS_NULLPTR
TEST_CASE_SKIP(cstddef, nullptr_t)
#else
TEST_CASE(cstddef, nullptr_t) {
    test_true( (STD::is_same<STD_NS nullptr_t, decltype(nullptr)>::value) );
    test_eq( sizeof(STD_NS nullptr_t), sizeof(void*) );

    STD_NS nullptr_t np = nullptr;
    int* p = np;
    test_ptr0( p );
    test_true( p == nullptr );
    test_true( np == nullptr );
    test_true( !(np != nullptr) );

    STD_NS nullptr_t np2 = STD_NS nullptr_t();
    test_true( np2 == np );
    test_true( static_cast<int*>(np2) == static_cast<int*>(0) );
    test_pass("cxx11:nullptr_t");
}

#endif

TEST_CASE(cstddef, max_align_t) {
#if !defined(__WATCOMC__) && (!defined(_MSC_VER) || _MSC_VER >= 1600)
    test_true( STD::is_trivial<STD_NS max_align_t>::value );
#else
    TEST_SKIP1();
#endif
    test_true( alignof(STD_NS max_align_t) >= alignof(char) );
    test_true( alignof(STD_NS max_align_t) >= alignof(int) );
    test_true( alignof(STD_NS max_align_t) >= alignof(long long) );
    test_true( alignof(STD_NS max_align_t) >= alignof(double) );
    test_true( alignof(STD_NS max_align_t) >= alignof(long double) );
    test_true( alignof(STD_NS max_align_t) >= alignof(void*) );

    STD_NS size_t a = alignof(STD_NS max_align_t);
    test_true( a != 0 && (a & (a - 1)) == 0 );

    STD_NS max_align_t m;
    void* pm = &m;
    TEST_SKIP_WAT();
#if defined(_M_IX86)
    TEST_SKIP_VC("32-bit MSVC frames align locals to 4 bytes");
#endif
    test_true( ((STD_NS size_t)pm % alignof(STD_NS max_align_t)) == 0 );
    test_pass("cxx11:max_align_t (alignment >= every scalar)");
}

#if !_TST_HAS_SCOPED_ENUM
TEST_CASE_SKIP(cstddef, byte_type)
#else
TEST_CASE(cstddef, byte_type) {
#if TEST_TARGET_CXX >= 2017

    test_eq( sizeof(STD::byte), (STD_NS size_t)1 );
    test_eq( alignof(STD::byte), (STD_NS size_t)1 );
    test_true( STD::is_enum<STD::byte>::value );
#if defined(__cpp_lib_is_scoped_enum)
    test_true( STD::is_scoped_enum<STD::byte>::value );
#else
    TEST_SKIP1();
#endif
    test_true( !STD::is_integral<STD::byte>::value );
    test_true( (STD::is_same<STD::underlying_type<STD::byte>::type, unsigned char>::value) );

    STD::byte b = STD::byte(42);
    test_true( as_int(b) == 42 );
    STD::byte z = STD::byte(0);
    test_true( as_int(z) == 0 );
    STD::byte f = static_cast<STD::byte>(0xFFu);
    test_true( as_int(f) == 0xFF );

    test_true( b == STD::byte(42) );
    test_true( b != z );
    test_pass("cxx17:byte type (scoped enum; is_scoped_enum=cxx23)");
#else
    TEST_NOTE("std::byte is C++17");
    test_skip("cxx17:byte type (scoped enum; is_scoped_enum=cxx23)");
#endif
}

#endif

#if _TST_HAS_BYTE
TEST_CASE(cstddef, to_integer) {
    STD::byte b = STD::byte(0x80);
    test_true( STD::to_integer<int>(b) == 0x80 );
    test_true( STD::to_integer<unsigned>(b) == 0x80u );
    test_true( STD::to_integer<unsigned char>(b) == 0x80u );

    test_true( STD::to_integer<long long>(STD::byte(0xFF)) == 255 );
    test_true( STD::to_integer<int>(STD::byte(0)) == 0 );

    int r = STD::to_integer<int>(STD::byte(7));
    test_eq( r, 7 );
    test_pass("cxx17:to_integer<T>(byte)");
}

TEST_CASE(cstddef, byte_shift_operators) {

    STD::byte b = STD::byte(0x01);
    test_true( as_int(b << 1) == 0x02 );
    test_true( as_int(b << 7) == 0x80 );
    test_true( as_int(b << 8) == 0x00 );
    test_true( as_int(STD::byte(0xFF) << 4) == 0xF0 );
    test_true( as_int(b << 0) == 0x01 );
    test_pass("cxx17:byte operator<<");

    test_true( as_int(STD::byte(0x80) >> 7) == 0x01 );
    test_true( as_int(STD::byte(0xFF) >> 4) == 0x0F );

    test_true( as_int(STD::byte(0x08) >> 3u) == 0x01 );
    test_pass("cxx17:byte operator>>");

    STD::byte s = STD::byte(0x01);
    s <<= 3;
    test_true( as_int(s) == 0x08 );
    s = STD::byte(0x02);
    STD::byte& ref = (s <<= 1);
    test_true( &ref == &s );
    test_true( as_int(s) == 0x04 );
    test_pass("cxx17:byte operator<<=");

    s = STD::byte(0x08);
    s >>= 2;
    test_true( as_int(s) == 0x02 );
    s = STD::byte(0x04);
    s >>= 8;
    test_true( as_int(s) == 0x00 );
    test_pass("cxx17:byte operator>>=");
}

TEST_CASE(cstddef, byte_bitwise_operators) {
    STD::byte a = STD::byte(0xF0);
    STD::byte b = STD::byte(0x3C);

    test_true( as_int(a | b) == 0xFC );
    test_true( as_int(a | STD::byte(0)) == 0xF0 );
    test_pass("cxx17:byte operator|");

    test_true( as_int(a & b) == 0x30 );
    test_true( as_int(a & STD::byte(0)) == 0x00 );
    test_pass("cxx17:byte operator&");

    test_true( as_int(a ^ b) == 0xCC );
    test_true( as_int(a ^ a) == 0x00 );
    test_pass("cxx17:byte operator^");

    test_true( as_int(~a) == 0x0F );
    test_true( as_int(~STD::byte(0)) == 0xFF );
    test_pass("cxx17:byte operator~");

    STD::byte v = STD::byte(0xF0);
    STD::byte& r1 = (v |= STD::byte(0x0C));
    test_true( &r1 == &v );
    test_true( as_int(v) == 0xFC );
    test_pass("cxx17:byte operator|=");

    STD::byte& r2 = (v &= STD::byte(0x3C));
    test_true( &r2 == &v );
    test_true( as_int(v) == 0x3C );
    test_pass("cxx17:byte operator&=");

    STD::byte& r3 = (v ^= STD::byte(0xFF));
    test_true( &r3 == &v );
    test_true( as_int(v) == 0xC3 );
    test_pass("cxx17:byte operator^=");
}
#else   // no std::byte (pre-VS2012 MSVC): the byte-arithmetic cases cannot be expressed
TEST_CASE_SKIP(cstddef, to_integer)
TEST_CASE_SKIP(cstddef, byte_shift_operators)
TEST_CASE_SKIP(cstddef, byte_bitwise_operators)
#endif

TEST_CASE(cstddef, offsetof_macro) {
    test_eq( offsetof(Off, c), (STD_NS size_t)0 );
    test_true( offsetof(Off, i) >= sizeof(char) );
    test_true( offsetof(Off, i) % alignof(int) == 0 );
    test_true( offsetof(Off, d) >= offsetof(Off, i) + sizeof(int) );
    test_true( offsetof(Off, tail) >= offsetof(Off, d) + sizeof(double) );
    test_true( offsetof(Off, tail) + sizeof(((Off*)0)->tail) <= sizeof(Off) );

    Off o;
    const char* base = reinterpret_cast<const char*>(&o);
    const char* pi   = reinterpret_cast<const char*>(&o.i);
    STD_NS size_t delta = (STD_NS size_t)(pi - base);
    test_eq( delta, offsetof(Off, i) );

    char probe[offsetof(Off, d) > 0 ? 1 : -1];
    (void)probe;
    test_pass("cxx03:offsetof");
}

TEST_CASE(cstddef, null_macro) {

    int* p = NULL;
    test_ptr0( p );
    test_true( p == nullptr );
    test_true( p == 0 );

    void* vp = NULL;
    test_true( vp == nullptr );

    int i = 3;
    int* q = &i;
    test_true( q != NULL );
    q = NULL;
    test_true( q == NULL );
    test_pass("cxx03:NULL");
}
#else
TEST_CASE_SKIP(cstddef, size_t_and_ptrdiff_t)
TEST_CASE_SKIP(cstddef, nullptr_t)
TEST_CASE_SKIP(cstddef, max_align_t)
TEST_CASE_SKIP(cstddef, byte_type)
TEST_CASE_SKIP(cstddef, to_integer)
TEST_CASE_SKIP(cstddef, byte_shift_operators)
TEST_CASE_SKIP(cstddef, byte_bitwise_operators)
TEST_CASE_SKIP(cstddef, offsetof_macro)
TEST_CASE_SKIP(cstddef, null_macro)
#endif
