#include "test_cxx.hpp"
#include <bitset>

TEST_CASE(bitset, header_compiles) { test_true( true ); test_pass("cxx03:bitset"); }

#if TEST_TARGET_CXX >= 2011
#include <bitset>
#include <string>
#include <string_view>
#include <utility>
#include <sstream>
#include <functional>
#include <stdexcept>
#include <cstddef>

TEST_CASE(bitset, construct) {

    STD::bitset<8>   b8;
    STD::bitset<64>  b64;
    STD::bitset<100> b100;
    test_true( b8.none() );
    test_true( b64.none() );
    test_true( b100.none() );
    test_pass("cxx03:bitset()");

    STD::bitset<8>  v8(0xF0ul);
    test_eq( v8.to_ulong(), 0xF0ul );
    test_pass("cxx03:bitset(unsigned long)");

    STD::bitset<8> trunc(0x1234ul);
    test_eq( trunc.to_ulong(), 0x34ul );

    STD::bitset<100> w(0xFFul);
    test_eq( w.count(), STD::size_t(8) );
    test_pass("cxx03:bitset ctor truncation");
}

#if _TST_HAS_CXX11_LIB_MEMBERS
TEST_CASE(bitset, cxx11_ullong) {
    STD::bitset<64> v64(0x0123456789ABCDEFull);
    test_eq( v64.to_ullong(), 0x0123456789ABCDEFull );
    test_pass("cxx11:bitset(unsigned long long)");
}
#else
TEST_CASE_SKIP(bitset, cxx11_ullong)
#endif

TEST_CASE(bitset, construct_from_string) {

    STD::string s = "xx1100yy";
    test_eq( STD::bitset<8>(STD::string("11001010")).to_ulong(), 0xCAul );
    test_eq( STD::bitset<4>(s, 2, 4).to_ulong(), 12ul );
    test_eq( STD::bitset<8>(STD::string("00001111"), 4).to_ulong(), 0x0Ful );
    test_pass("cxx03:bitset(string)");

    test_throw( STD::bitset<8>(STD::string("1012")) );
    test_throw( STD::bitset<8>(STD::string("11"), 5) );
    test_pass("cxx03:bitset(string) throws");

    STD::bitset<100> big(STD::string("101"));
    test_eq( big.to_ulong(), 5ul );
    test_eq( big.count(), STD::size_t(2) );
    test_pass("cxx03:bitset(string) short");
}

#if _TST_HAS_CXX11_LIB_MEMBERS
TEST_CASE(bitset, cxx11_construct_from_string) {
    STD::string ab = "abbaabba";
    test_eq( STD::bitset<8>(ab, 0, 8, 'a', 'b').to_ulong(), 0x66ul );
    test_pass("cxx11:bitset(string, pos, n, zero, one)");

    test_eq( STD::bitset<8>("10101010").to_ulong(), 0xAAul );
    test_eq( STD::bitset<8>("1111000011", 8).to_ulong(), 0xF0ul );
    test_eq( STD::bitset<4>("bbaa", 4, 'a', 'b').to_ulong(), 12ul );
    test_throw( STD::bitset<8>("12") );
    test_pass("cxx11:bitset(const char*)");
}
#else
TEST_CASE_SKIP(bitset, cxx11_construct_from_string)
#endif

TEST_CASE(bitset, member_ops) {
    STD::bitset<8> a(0xF0ul), b(0x3Cul);

    STD::bitset<8> t = a;   t &= b;  test_eq( t.to_ulong(), 0x30ul );
    test_pass("cxx03:operator&=");
    t = a;                  t |= b;  test_eq( t.to_ulong(), 0xFCul );
    test_pass("cxx03:operator|=");
    t = a;                  t ^= b;  test_eq( t.to_ulong(), 0xCCul );
    test_pass("cxx03:operator^=");
    t = a;                  t <<= 2; test_eq( t.to_ulong(), 0xC0ul );
    test_pass("cxx03:operator<<=");
    t = a;                  t >>= 4; test_eq( t.to_ulong(), 0x0Ful );
    test_pass("cxx03:operator>>=");

    test_eq( (~a).to_ulong(), 0x0Ful );
    test_pass("cxx03:operator~");
    test_eq( (a << 1).to_ulong(), 0xE0ul );
    test_pass("cxx03:operator<<(shift)");
    test_eq( (a >> 1).to_ulong(), 0x78ul );
    test_pass("cxx03:operator>>(shift)");

    test_true( (a << 8).none() );
    test_true( (a >> 8).none() );

    STD::bitset<100> w;
    w.set(0);
    test_eq( (w << 99).count(), STD::size_t(1) );
    test_true( (w << 99).test(99) );
    test_true( (w << 100).none() );
    STD::bitset<100> hi;
    hi.set(99);
    test_true( (hi >> 99).test(0) );
    test_pass("cxx03:bitset shift wide");

    STD::bitset<64> x(12345ull), y(12345ull), z(54321ull);
    test_true( x == y );
    test_true( !(x == z) );
    test_pass("cxx03:operator==");
    test_true( x != z );
    test_true( !(x != y) );
    test_pass("cxx03:operator!=");

    STD::bitset<8> m(0x05ul);
    const STD::bitset<8>& cm = m;
    test_true(  cm[0] );
    test_true( !cm[1] );
    test_true(  cm[2] );
    m[1] = true;
    test_eq( m.to_ulong(), 0x07ul );
    test_pass("cxx03:operator[]");
}

TEST_CASE(bitset, free_ops_and_stream) {
    STD::bitset<8> a(0xF0ul), b(0x3Cul);
    test_eq( (a & b).to_ulong(), 0x30ul );
    test_pass("cxx03:operator&");
    test_eq( (a | b).to_ulong(), 0xFCul );
    test_pass("cxx03:operator|");
    test_eq( (a ^ b).to_ulong(), 0xCCul );
    test_pass("cxx03:operator^");

    STD::bitset<100> p, q;
    p.set(99); p.set(0);
    q.set(99);
    test_eq( (p & q).count(), STD::size_t(1) );
    test_eq( (p | q).count(), STD::size_t(2) );
    test_eq( (p ^ q).count(), STD::size_t(1) );
    test_pass("cxx03:bitset free bitops wide");

    STD::ostringstream os;
    os << STD::bitset<8>(0xC5ul);
    STD::string os_s = os.str();
    test_true( os_s == "11000101" );
    test_pass("cxx03:operator<<(ostream)");

    STD::istringstream is("1011 rest");
    STD::bitset<4> r;
    is >> r;
    test_eq( r.to_ulong(), 11ul );

    STD::istringstream is2("110");
    STD::bitset<8> r2;
    is2 >> r2;
    test_eq( r2.to_ulong(), 6ul );
    test_pass("cxx03:operator>>(istream)");
}

TEST_CASE(bitset, set_reset_flip) {
    STD::bitset<8> b;
    b.set();
    test_eq( b.to_ulong(), 0xFFul );
    test_pass("cxx03:set()");
    b.reset();
    test_true( b.none() );
    test_pass("cxx03:reset()");

    b.set(3);
    b.set(1, true);
    b.set(0, false);
    test_eq( b.to_ulong(), 0x0Aul );
    test_pass("cxx03:set(pos)");
    b.reset(1);
    test_eq( b.to_ulong(), 0x08ul );
    test_pass("cxx03:reset(pos)");

    b.flip();
    test_eq( b.to_ulong(), 0xF7ul );
    test_pass("cxx03:flip()");
    b.flip(3);
    test_eq( b.to_ulong(), 0xFFul );
    test_pass("cxx03:flip(pos)");

    test_throw( b.set(8) );
    test_pass("cxx03:set out_of_range");
    test_throw( b.reset(8) );
    test_pass("cxx03:reset out_of_range");
    test_throw( b.flip(8) );
    test_pass("cxx03:flip out_of_range");

    STD::bitset<100> w;
    w.flip();
    test_eq( w.count(), STD::size_t(100) );
    w.flip(99);
    test_true( w.any() );
    test_eq( w.count(), STD::size_t(99) );
    test_pass("cxx03:flip() wide");
}

#if _TST_HAS_CXX11_LIB_MEMBERS
TEST_CASE(bitset, cxx11_all) {
    STD::bitset<8> b;
    b.set();
    test_true( b.all() );
    b.reset(0);
    test_true( !b.all() );
    test_pass("cxx11:all");

    STD::bitset<100> w;
    w.flip();
    test_true( w.all() );
    w.flip(99);
    test_true( !w.all() );
    test_pass("cxx11:bitset::all() (wide)");

    STD::bitset<64> f;
    f.set();
    test_true( f.all() );
    test_eq( f.to_ullong(), ~0ull );
    test_pass("cxx11:bitset::all() (one-word)");
}
#else
TEST_CASE_SKIP(bitset, cxx11_all)
#endif

TEST_CASE(bitset, observers_and_conversions) {
    STD::bitset<8>   b8(0x81ul);
    STD::bitset<64>  b64(0x1ul);
    b64.set(63);
    STD::bitset<100> b100;
    b100.set(0);
    b100.set(63);

    test_eq( b8.size(),   STD::size_t(8) );
    test_eq( b64.size(),  STD::size_t(64) );
    test_eq( b100.size(), STD::size_t(100) );
    test_pass("cxx03:size");

    test_eq( b8.count(),   STD::size_t(2) );
    test_eq( b64.count(),  STD::size_t(2) );
    test_eq( b100.count(), STD::size_t(2) );
    test_pass("cxx03:count");

    test_true(  b8.test(0) );
    test_true( !b8.test(1) );
    test_true(  b8.test(7) );
    test_throw( (void)b8.test(8) );
    test_pass("cxx03:test");

    test_true( b8.any() );
    test_pass("cxx03:any");
    test_true( !b8.none() );
    test_true( STD::bitset<8>().none() );
    test_pass("cxx03:none");

    test_eq( b8.to_ulong(),  0x81ul );
    test_pass("cxx03:to_ulong");
#if _TST_HAS_CXX11_LIB_MEMBERS
    test_true( !b8.all() );
    test_true( STD::bitset<8>(0xFFul).all() );
    test_eq( b8.to_ullong(), 0x81ull );
    test_eq( b64.to_ullong(), 0x8000000000000001ull );
    test_eq( b100.to_ullong(), 0x8000000000000001ull );
    test_pass("cxx11:to_ullong");
#else
    test_skip("cxx11:to_ullong");
#endif

    STD::bitset<100> over;
    over.set(sizeof(unsigned long) * 8);
    test_throw( (void)over.to_ulong() );
    test_pass("cxx03:to_ulong overflow_error");

    STD::string s0 = b8.to_string();
    test_true( s0 == "10000001" );
    test_pass("cxx03:to_string");
#if _TST_HAS_CXX11_LIB_MEMBERS
    STD::string s1 = b8.to_string('.', '#');
    test_true( s1 == "#......#" );
    test_pass("cxx11:bitset::to_string(zero, one)");
#else
    test_skip("cxx11:bitset::to_string(zero, one)");
#endif
#if 1
    STD::string s  = b8.to_string<char, STD::char_traits<char>, STD::allocator<char> >();
    test_true( s  == "10000001" );
    STD::wstring ws = b8.to_string<wchar_t>();
    test_eq( ws.size(), STD::size_t(8) );
    test_true( ws[0] == L'1' );
    test_pass("cxx03:to_string<CharT>");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx03:to_string<CharT>");
#endif
}

TEST_CASE(bitset, reference_proxy) {
    STD::bitset<8> b(0x00ul);

    STD::bitset<8>::reference r0 = b[0];
    r0 = true;
    test_true( b.test(0) );
    test_pass("cxx03:bitset::reference::operator=(bool)");

    STD::bitset<8>::reference r1 = b[1];
    r1 = r0;
    test_true( b.test(1) );
    test_pass("cxx03:bitset::reference::operator=(reference)");

    bool v = r0;
    test_true( v );
    test_true( b[0] == true );
    test_pass("cxx03:bitset::reference::operator bool");

    bool nr = ~r0;
    test_true( !nr );
    test_true( b.test(0) );
    test_pass("cxx03:bitset::reference::operator~");

    r0.flip();
    test_true( !b.test(0) );
    r0.flip();
    test_true( b.test(0) );
    test_pass("cxx03:bitset::reference::flip");

    STD::bitset<100> w;
    STD::bitset<100>::reference rw = w[70];
    rw = true;
    test_true( w.test(70) );
    test_eq( w.count(), STD::size_t(1) );
    rw.flip();
    test_true( w.none() );
    test_pass("cxx03:bitset::reference wide");
}

TEST_CASE(bitset, hash_specialization) {
    STD::hash<STD::bitset<8> >   h8;
    STD::hash<STD::bitset<64> >  h64;
    STD::hash<STD::bitset<100> > h100;

    STD::bitset<8> a(0x5Aul), b(0x5Aul), c(0xA5ul);
    STD::size_t ha = h8(a), hb = h8(b), hc = h8(c);
    test_eq( ha, hb );
    test_ne( ha, hc );

    STD::bitset<64> x(1ull), y(1ull);
    test_eq( h64(x), h64(y) );
    test_pass("cxx11:hash<bitset>");

    STD::bitset<100> p, q;
    p.set(99);
    q.set(99);
    STD::size_t hp = h100(p), hq = h100(q);
    test_eq( hp, hq );
    test_pass("cxx11:hash<bitset> wide");
}
#else
TEST_CASE_SKIP(bitset, construct)
TEST_CASE_SKIP(bitset, construct_from_string)
TEST_CASE_SKIP(bitset, member_ops)
TEST_CASE_SKIP(bitset, free_ops_and_stream)
TEST_CASE_SKIP(bitset, set_reset_flip)
TEST_CASE_SKIP(bitset, observers_and_conversions)
TEST_CASE_SKIP(bitset, reference_proxy)
TEST_CASE_SKIP(bitset, hash_specialization)
#endif


TEST_CASE(bitset, constexpr_and_cxx26_additions) {
#if defined(__cpp_lib_constexpr_bitset)
    test_true( __cpp_lib_constexpr_bitset > 0L );
    test_pass("cxx23:__cpp_lib_constexpr_bitset");
    {
        STD::bitset<8> b(0x0Fu);
        b.flip();
        test_eq( (int)b.to_ulong(), 0xF0 );
        test_eq( (int)b.count(), 4 );
    }
    test_pass("cxx23:constexpr bitset operations");
#else
    TEST_NOTE("constexpr bitset is C++23; this library does not announce it");
    test_skip("cxx23:__cpp_lib_constexpr_bitset");
    test_skip("cxx23:constexpr bitset operations");
#endif

#if defined(__cpp_lib_bitset)
    test_true( __cpp_lib_bitset >= 202306L );
    test_pass("cxx26:__cpp_lib_bitset");
    {
        STD::string_view sv("1101");
        STD::bitset<4> b(sv);
        test_eq( (int)b.to_ulong(), 13 );
        STD::bitset<4> part(STD::string_view("xx1101xx").substr(2, 4));
        test_eq( (int)part.to_ulong(), 13 );
    }
    test_pass("cxx26:bitset(string_view)");
    {
        STD::bitset<4> b("110100", 4);
        test_eq( (int)b.to_ulong(), 13 );
    }
    test_pass("cxx26:bitset(const char*, n)");
#else
    TEST_NOTE("the string_view constructors are C++26 (P2697); not in this library");
    test_skip("cxx26:__cpp_lib_bitset");
    test_skip("cxx26:bitset(string_view)");
    test_skip("cxx26:bitset(const char*, n)");
#endif
}

TEST_CASE(bitset, reference_assignment_and_swap_cxx26) {
#if defined(__cpp_lib_bitset) && __cpp_lib_bitset >= 202306L
    STD::bitset<4> b(0x5u);
    {
        const STD::bitset<4>::reference r = b[1];
        r = true;
        test_true( b[1] );
    }
    test_pass("cxx26:bitset::reference::operator=(bool) const");

    b = STD::bitset<4>(0x1u);
    STD::swap(b[0], b[1]);
    test_true( !b[0] && b[1] );
    test_pass("cxx26:swap(bitset::reference, bitset::reference)");

    bool flag = false;
    STD::swap(b[1], flag);
    test_true( !b[1] && flag );
    test_pass("cxx26:swap(bitset::reference, bool&)");

    STD::swap(flag, b[1]);
    test_true( b[1] && !flag );
    test_pass("cxx26:swap(bool&, bitset::reference)");
#else
    TEST_NOTE("bitset::reference's const assignment and swaps are C++26 (P2988)");
    test_skip("cxx26:bitset::reference::operator=(bool) const");
    test_skip("cxx26:swap(bitset::reference, bitset::reference)");
    test_skip("cxx26:swap(bitset::reference, bool&)");
    test_skip("cxx26:swap(bool&, bitset::reference)");
#endif
}

TEST_CASE(bitset, constexpr_members_cxx23) {
#if defined(__cpp_lib_constexpr_bitset) && (__cpp_lib_constexpr_bitset) >= 202202L
    struct K {
        static constexpr bool all_narrow() { STD::bitset<8> b(0xFFu); return b.all(); }
        static constexpr bool all_wide() { STD::bitset<100> b; b.set(); return b.all(); }
        static constexpr bool to_str() { STD::bitset<4> b(0x5u);
                                         return b.to_string('.', '*') == ".*.*"; }
        static constexpr unsigned long from_str() {
            STD::bitset<4> b(STD::string("x**x"), 1, 2, 'x', '*'); return b.to_ulong(); }
    };
    static_assert(K::all_narrow(), "constexpr bitset::all (one word)");
    test_true( K::all_narrow() );
    test_pass("cxx23:constexpr bitset::all() (one-word)");

    static_assert(K::all_wide(), "constexpr bitset::all (multi word)");
    test_true( K::all_wide() );
    test_pass("cxx23:constexpr bitset::all() (wide)");

    static_assert(K::to_str(), "constexpr bitset::to_string(zero, one)");
    test_true( K::to_str() );
    test_pass("cxx23:constexpr bitset::to_string(zero, one)");

    static_assert(K::from_str() == 3ul, "constexpr bitset(string, pos, n, zero, one)");
    test_eq( K::from_str(), 3ul );
    test_pass("cxx23:constexpr bitset(string, pos, n, zero, one)");
#else
    TEST_NOTE("constexpr bitset is C++23 (__cpp_lib_constexpr_bitset)");
    TEST_SKIP_N(4);
    test_skip("cxx23:constexpr bitset::all() (one-word)");
    test_skip("cxx23:constexpr bitset::all() (wide)");
    test_skip("cxx23:constexpr bitset::to_string(zero, one)");
    test_skip("cxx23:constexpr bitset(string, pos, n, zero, one)");
#endif
}
