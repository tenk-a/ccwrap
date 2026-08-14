#include "test_cxx.hpp"

#include <numeric>
#include <functional>
#include <vector>

struct Sq { int operator()(int x) const { return x * x; } };

namespace {
struct Vc {
    int v;
    int from;
    Vc() : v(0), from(0) {}
    explicit Vc(int x) : v(x), from(0) {}
    Vc(const Vc& o) : v(o.v), from(1) {}
    Vc& operator=(const Vc& o) { v = o.v; from = 1; return *this; }
#if _TST_HAS_RVALUE_REF
    Vc(Vc&& o) : v(o.v), from(2) {}
    Vc& operator=(Vc&& o) { v = o.v; from = 2; return *this; }
#endif
};
int g_vc_first = -1;
int g_vc_second = -1;

struct VcAddFirst {
    Vc operator()(Vc a, const Vc& b) const {
        if (g_vc_first < 0) g_vc_first = a.from;
        Vc r(a.v + b.v);
        return r;
    }
};
struct VcSubSecond {
    Vc operator()(const Vc& a, Vc b) const {
        if (g_vc_second < 0) g_vc_second = b.from;
        Vc r(a.v - b.v);
        return r;
    }
};
struct VcMulPlain {
    Vc operator()(const Vc& a, const Vc& b) const { return Vc(a.v * b.v); }
};

}

#define _TST_VC_REPORT(from, copy_row, move_row)                         do {                                                                     if ((from) == 2) { test_pass(move_row); test_skip(copy_row); }         else             { test_pass(copy_row); test_skip(move_row); }     } while (0)

TEST_CASE(numeric, operand_value_category) {
    Vc data[3];
    data[0] = Vc(1); data[1] = Vc(2); data[2] = Vc(3);

    g_vc_first = -1;
    Vc acc = STD::accumulate(data, data + 3, Vc(0), VcAddFirst());
    test_eq( acc.v, 6 );
    test_true( g_vc_first == 1 || g_vc_first == 2 );
    _TST_VC_REPORT(g_vc_first, "cxx03:accumulate (copy accumulator)",
                               "cxx20:accumulate (move accumulator)");

    g_vc_first = -1;
    Vc ip = STD::inner_product(data, data + 3, data, Vc(0), VcAddFirst(), VcMulPlain());
    test_eq( ip.v, 14 );
    test_true( g_vc_first == 1 || g_vc_first == 2 );
    _TST_VC_REPORT(g_vc_first, "cxx03:inner_product (copy accumulator)",
                               "cxx20:inner_product (move accumulator)");

    g_vc_first = -1;
    Vc out[3];
    STD::partial_sum(data, data + 3, out, VcAddFirst());
    test_eq( out[2].v, 6 );
    test_true( g_vc_first == 1 || g_vc_first == 2 );
    _TST_VC_REPORT(g_vc_first, "cxx03:partial_sum (copy operand)",
                               "cxx20:partial_sum (move operand)");

    g_vc_second = -1;
    Vc diff[3];
    STD::adjacent_difference(data, data + 3, diff, VcSubSecond());
    test_eq( diff[0].v, 1 );
    test_eq( diff[1].v, 1 );
    test_true( g_vc_second == 1 || g_vc_second == 2 );
    _TST_VC_REPORT(g_vc_second, "cxx03:adjacent_difference (copy operand)",
                                "cxx20:adjacent_difference (move operand)");
}

TEST_CASE(numeric, cxx03) {
    int a[] = { 1, 2, 3, 4, 5 };
    test_eq( STD::accumulate(a, a + 5, 0), 15 );
    test_eq( STD::accumulate(a, a + 5, 1, STD::multiplies<int>()), 120 );
    test_pass("cxx03:accumulate");

    int b[] = { 1, 2, 3 }, c[] = { 4, 5, 6 };
    test_eq( STD::inner_product(b, b + 3, c, 0), 32 );
    test_eq( STD::inner_product(b, b + 3, c, 0, STD::plus<int>(), STD::multiplies<int>()), 32 );
    test_pass("cxx03:inner_product");

    int part[3];
    STD::partial_sum(b, b + 3, part);
    test_eq( part[2], 6 );
    test_pass("cxx03:partial_sum");
    int diff[3];
    int d[] = { 2, 5, 9 };
    STD::adjacent_difference(d, d + 3, diff);
    test_eq( diff[1], 3 ); test_eq( diff[2], 4 );
    test_pass("cxx03:adjacent_difference");
}

TEST_CASE(numeric, iota) {
    int a[5];
    STD::iota(a, a + 5, 10);
    test_eq( a[0], 10 ); test_eq( a[4], 14 );
    STD::vector<int> v(3);
    STD::iota(v.begin(), v.end(), 1);
    test_eq( v[2], 3 );
    test_pass("cxx11:iota");
}

#if TEST_TARGET_CXX >= 2017
TEST_CASE(numeric, gcd_lcm) {
    test_eq( STD::gcd(12, 18), 6 );
    test_eq( STD::gcd(17, 5), 1 );
    test_eq( STD::gcd(0, 9), 9 );
    test_eq( STD::gcd(-12, 8), 4 );
    test_pass("cxx17:gcd");
    test_eq( STD::lcm(4, 6), 12 );
    test_eq( STD::lcm(0, 5), 0 );
    test_pass("cxx17:lcm");
}

TEST_CASE(numeric, reduce_scan) {
    int a[] = { 1, 2, 3, 4 };
    test_eq( STD::reduce(a, a + 4), 10 );
    test_eq( STD::reduce(a, a + 4, 0), 10 );
    test_eq( STD::reduce(a, a + 4, 1, STD::multiplies<int>()), 24 );
    test_pass("cxx17:reduce");

    int inc[4];
    STD::inclusive_scan(a, a + 4, inc);
    test_eq( inc[0], 1 ); test_eq( inc[3], 10 );
    STD::inclusive_scan(a, a + 4, inc, STD::plus<int>());
    test_eq( inc[3], 10 );
    STD::inclusive_scan(a, a + 4, inc, STD::plus<int>(), 100);
    test_eq( inc[3], 110 );
    test_pass("cxx17:inclusive_scan");

    int exc[4];
    STD::exclusive_scan(a, a + 4, exc, 0);
    test_eq( exc[0], 0 ); test_eq( exc[3], 6 );
    STD::exclusive_scan(a, a + 4, exc, 0, STD::plus<int>());
    test_eq( exc[3], 6 );
    test_pass("cxx17:exclusive_scan");

    int b[] = { 1, 2, 3 }, c[] = { 4, 5, 6 };
    test_eq( STD::transform_reduce(b, b + 3, c, 0), 32 );
    test_eq( STD::transform_reduce(b, b + 3, c, 0, STD::plus<int>(), STD::multiplies<int>()), 32 );
    test_eq( STD::transform_reduce(b, b + 3, 0, STD::plus<int>(), Sq()), 14 );
#if _tst_cplusplus >= 201103L
    struct SqLocal { int operator()(int x) const { return x * x; } };
    test_eq( STD::transform_reduce(b, b + 3, 0, STD::plus<int>(), SqLocal()), 14 );
    test_pass("cxx17:transform_reduce");
#else
    TEST_SKIP1();
    test_skip("cxx17:transform_reduce");
#endif
}

TEST_CASE(numeric, transform_inclusive_scan_serial) {
    int a[] = { 1, 2, 3, 4 };
    int out[4];

    STD::transform_inclusive_scan(a, a + 4, out, STD::plus<int>(), Sq());
    test_eq( out[0], 1 );
    test_eq( out[1], 5 );
    test_eq( out[3], 30 );

    STD::transform_inclusive_scan(a, a + 4, out, STD::plus<int>(), Sq(), 100);
    test_eq( out[0], 101 );
    test_eq( out[3], 130 );
    test_pass("cxx17:transform_inclusive_scan");
}
#endif

#if !defined(__GNUC__) && !defined(__WATCOMC__) && defined(__cpp_lib_parallel_algorithm)
#include <execution>
TEST_CASE(numeric, parallel_overloads) {
    namespace ex = STD::execution;
    int a[] = { 1, 2, 3, 4 };

    test_eq( STD::reduce(ex::par, a, a + 4, 0), 10 );
    test_pass("cxx17:reduce (par)");

    int inc[4];
    STD::inclusive_scan(ex::par, a, a + 4, inc);
    test_eq( inc[3], 10 );
    test_pass("cxx17:inclusive_scan (par)");

    int exc[4];
    STD::exclusive_scan(ex::par, a, a + 4, exc, 0);
    test_eq( exc[3], 6 );
    test_pass("cxx17:exclusive_scan (par)");

    int b[] = { 1, 2, 3 }, c[] = { 4, 5, 6 };
    test_eq( STD::transform_reduce(ex::par, b, b + 3, c, 0), 32 );
    test_pass("cxx17:transform_reduce (par)");

    int tin[4];
    STD::transform_inclusive_scan(ex::par, a, a + 4, tin, STD::plus<int>(), Sq());
    test_eq( tin[3], 30 );
    test_pass("cxx17:transform_inclusive_scan (par)");

    int tex[4];
    STD::transform_exclusive_scan(ex::par, a, a + 4, tex, 0, STD::plus<int>(), Sq());
    test_eq( tex[3], 14 );
    test_pass("cxx17:transform_exclusive_scan (par)");
}
#else
TEST_CASE(numeric, parallel_overloads) {
    test_skip("cxx17:reduce (par)");
    test_skip("cxx17:inclusive_scan (par)");
    test_skip("cxx17:exclusive_scan (par)");
    test_skip("cxx17:transform_reduce (par)");
    test_skip("cxx17:transform_inclusive_scan (par)");
    test_skip("cxx17:transform_exclusive_scan (par)");
}
#endif

#include <array>
TEST_CASE(numeric, ranges_iota) {
#if defined(__cpp_lib_ranges_iota)
    STD::array<int, 5> a;

    auto r = STD::ranges::iota(a.begin(), a.end(), 10);
    test_eq( a[0], 10 );
    test_eq( a[4], 14 );
    test_true( r.out == a.end() );
    test_eq( r.value, 15 );
    test_pass("cxx23:ranges::iota");

    STD::ranges::iota_result<int*, int> named;
    named.out = 0; named.value = 7;
    test_eq( named.value, 7 );
    test_pass("cxx23:ranges::iota_result");
#else
    test_skip("cxx23:ranges::iota");
    test_skip("cxx23:ranges::iota_result");
#endif
}

#if TEST_TARGET_CXX >= 2020
TEST_CASE(numeric, midpoint) {
    test_eq( STD::midpoint(10, 20), 15 );
    test_eq( STD::midpoint(0, 9), 4 );
    test_eq( STD::midpoint(-10, 10), 0 );
    test_pass("cxx20:midpoint");
}
#endif

#if TEST_TARGET_CXX >= 2026
TEST_CASE(numeric, saturate_arith) {
    const int imax = 2147483647;
    const int imin = -2147483647 - 1;

    test_eq( STD::add_sat<int>(1, 2), 3 );
    test_pass("cxx26:saturating_add");
    test_eq( STD::add_sat<int>(imax, 1), imax );
    test_eq( STD::add_sat<unsigned>(4000000000u, 1000000000u), 4294967295u );
    test_pass("cxx26:saturating_add (positive overflow)");
    test_eq( STD::add_sat<int>(imin, -1), imin );
    test_eq( STD::add_sat<int>(imin, imin), imin );
    test_pass("cxx26:saturating_add (negative overflow)");

    test_eq( STD::sub_sat<int>(5, 3), 2 );
    test_pass("cxx26:saturating_sub");
    test_eq( STD::sub_sat<int>(imax, -1), imax );
    test_eq( STD::sub_sat<int>(imax, imin), imax );
    test_pass("cxx26:saturating_sub (positive overflow)");
    test_eq( STD::sub_sat<int>(imin, 1), imin );
    test_eq( STD::sub_sat<unsigned>(3u, 5u), 0u );
    test_pass("cxx26:saturating_sub (negative overflow)");

    test_eq( STD::mul_sat<int>(3, 4), 12 );
    test_pass("cxx26:saturating_mul");
    test_eq( STD::mul_sat<int>(100000, 100000), imax );
    test_eq( STD::mul_sat<int>(-100000, 100000), imin );
    test_eq( STD::mul_sat<int>(-100000, -100000), imax );
    test_eq( STD::mul_sat<unsigned>(0x10000u, 0x10000u), 4294967295u );
    test_pass("cxx26:saturating_mul (overflow)");

    test_eq( STD::div_sat<int>(7, 2), 3 );
    test_eq( STD::div_sat<unsigned>(10u, 3u), 3u );
    test_pass("cxx26:saturating_div");
    test_eq( STD::div_sat<int>(imin, -1), imax );
    test_pass("cxx26:saturating_div (min/-1)");

    test_eq( STD::saturate_cast<int>(5u), 5 );
    test_eq( (int)STD::saturate_cast<signed char>(100), 100 );
    test_pass("cxx26:saturating_cast");
    test_eq( (int)STD::saturate_cast<unsigned char>(300), 255 );
    test_eq( (int)STD::saturate_cast<unsigned char>(-5), 0 );
    test_eq( (int)STD::saturate_cast<signed char>(1000), 127 );
    test_eq( (int)STD::saturate_cast<signed char>(-1000), -128 );
    test_eq( STD::saturate_cast<unsigned>(-1), 0u );
    test_pass("cxx26:saturating_cast (out-of-range)");
}
#endif

#if TEST_TARGET_CXX >= 2026 && _TST_HAS_STD_C23_LIB

TEST_CASE(numeric, ckd_arith) {
    const int imax = 2147483647;
    const int imin = -2147483647 - 1;

    int r = -1;
    test_true( !STD::ckd_add(&r, 2, 3) );   test_eq( r, 5 );
    test_true( !STD::ckd_sub(&r, 2, 3) );   test_eq( r, -1 );
    test_true( !STD::ckd_mul(&r, -4, 3) );  test_eq( r, -12 );

    test_true( STD::ckd_add(&r, imax, 1) );   test_eq( r, imin );
    test_true( STD::ckd_sub(&r, imin, 1) );   test_eq( r, imax );
    test_true( STD::ckd_mul(&r, imax, 2) );   test_eq( r, -2 );
    test_true( !STD::ckd_add(&r, imax, -1) ); test_eq( r, imax - 1 );

    test_true( !STD::ckd_add(&r, -1, 1u) );      test_eq( r, 0 );

    test_true( !STD::ckd_sub(&r, 5u, 10u) );     test_eq( r, -5 );

    unsigned u = 0;
    test_true( STD::ckd_sub(&u, 5u, 10u) );      test_eq( u, (unsigned)-5 );
    test_true( STD::ckd_sub(&u, 3, 4) );         test_eq( u, (unsigned)-1 );
    test_true( !STD::ckd_add(&u, -1, 2) );       test_eq( u, 1u );

    test_true( STD::ckd_add(&r, 4294967295u, 1u) );  test_eq( r, 0 );
    test_true( STD::ckd_add(&u, 4294967295u, 1u) );  test_eq( u, 0u );
    long long ll = 0;
    test_true( !STD::ckd_add(&ll, 4294967295u, 1u) );
    test_eq( ll, 4294967296LL );
    test_pass("cxx26:ckd_add(type1* result, type2 a, type3 b)");

    signed char sc = 0;
    test_true( STD::ckd_add(&sc, 100, 100) );    test_eq( (int)sc, -56 );
    test_true( !STD::ckd_add(&sc, 100, 27) );    test_eq( (int)sc, 127 );
    test_true( STD::ckd_add(&sc, 100, 28) );     test_eq( (int)sc, -128 );
    test_true( STD::ckd_mul(&sc, 16, 8) );       test_eq( (int)sc, -128 );
    unsigned char uc = 0;
    test_true( !STD::ckd_sub(&uc, 200, 100) );   test_eq( (int)uc, 100 );
    test_true( STD::ckd_sub(&uc, 100, 200) );    test_eq( (int)uc, 156 );
    test_pass("cxx26:ckd_sub(type1* result, type2 a, type3 b)");

    const long long llmax = 9223372036854775807LL;
    const long long llmin = -llmax - 1;
    test_true( !STD::ckd_add(&ll, llmax, 0) );   test_eq( ll, llmax );
    test_true( STD::ckd_add(&ll, llmax, 1) );    test_eq( ll, llmin );
    test_true( STD::ckd_sub(&ll, llmin, 1) );    test_eq( ll, llmax );
    test_true( STD::ckd_add(&ll, llmin, -1) );   test_eq( ll, llmax );
    test_true( STD::ckd_mul(&ll, llmax, 2) );    test_eq( ll, -2LL );
    test_true( STD::ckd_mul(&ll, llmin, -1) );   test_eq( ll, llmin );
    test_true( !STD::ckd_mul(&ll, 4294967296LL, 2147483647LL) );
    test_eq( ll, 9223372032559808512LL );
    test_true( STD::ckd_mul(&ll, 4294967296LL, 4294967296LL) );
    test_eq( ll, 0LL );

    unsigned long long ull = 0;
    const unsigned long long ullmax = 18446744073709551615ULL;
    test_true( !STD::ckd_add(&ull, ullmax, 0) ); test_eq( ull, ullmax );
    test_true( STD::ckd_add(&ull, ullmax, 1) );  test_eq( ull, 0ULL );
    test_true( STD::ckd_mul(&ull, ullmax, 2) );  test_eq( ull, ullmax - 1 );
    test_true( STD::ckd_sub(&ull, 0, 1) );       test_eq( ull, ullmax );

    test_true( STD::ckd_add(&ll, ullmax, 0) );
    test_true( !STD::ckd_add(&ull, ullmax, 0) );

    test_true( STD::ckd_mul(&ull, -1, 1) );      test_eq( ull, ullmax );
    test_true( !STD::ckd_mul(&ull, -1, -1) );    test_eq( ull, 1ULL );

    test_true( !STD::ckd_add(&u, -5, 5) );       test_eq( u, 0u );
    test_true( !STD::ckd_mul(&u, -5, 0) );       test_eq( u, 0u );
    test_true( !STD::ckd_sub(&u, -5, -5) );      test_eq( u, 0u );
    test_pass("cxx26:ckd_mul(type1* result, type2 a, type3 b)");
}
#else
TEST_CASE_SKIP(numeric, ckd_arith)
#endif

#if TEST_TARGET_CXX >= 2020

TEST_CASE(numeric, midpoint_props) {
    const int imax = 2147483647;
    const int imin = -imax - 1;
    const unsigned umax = 4294967295u;

    test_eq( STD::midpoint(1, 4), 2 );
    test_eq( STD::midpoint(4, 1), 3 );
    test_eq( STD::midpoint(-4, -1), -3 );
    test_eq( STD::midpoint(-1, -4), -2 );
    test_eq( STD::midpoint(3, 4), 3 );
    test_eq( STD::midpoint(4, 3), 4 );
    test_eq( STD::midpoint(5, 5), 5 );

    test_eq( STD::midpoint(imin, imax), -1 );
    test_eq( STD::midpoint(imax, imin), 0 );
    test_eq( STD::midpoint(imax, imax), imax );
    test_eq( STD::midpoint(imin, imin), imin );
    test_eq( STD::midpoint(imax - 1, imax), imax - 1 );

    test_eq( STD::midpoint(0u, umax), 2147483647u );
    test_eq( STD::midpoint(umax, 0u), 2147483648u );
    test_eq( STD::midpoint(umax, umax), umax );
    test_eq( STD::midpoint(2u, 5u), 3u );
    test_eq( STD::midpoint(5u, 2u), 4u );

    test_eq( (int)STD::midpoint((signed char)-128, (signed char)127), -1 );
    test_eq( (int)STD::midpoint((unsigned char)0, (unsigned char)255), 127 );
    test_eq( (int)STD::midpoint((unsigned char)255, (unsigned char)0), 128 );
    const long long llmax = 9223372036854775807LL;
    const long long llmin = -llmax - 1;
    test_eq( STD::midpoint(llmin, llmax), -1LL );
    test_eq( STD::midpoint(llmax, llmin), 0LL );
    const unsigned long long ullmax = 18446744073709551615ULL;
    test_eq( STD::midpoint(0ULL, ullmax), 9223372036854775807ULL );
    test_eq( STD::midpoint(ullmax, 0ULL), 9223372036854775808ULL );

    test_true( STD::midpoint(1.0, 2.0) == 1.5 );
    test_true( STD::midpoint(2.0, 1.0) == 1.5 );
    test_true( STD::midpoint(-3.0, 3.0) == 0.0 );
    const double dmax = STD::numeric_limits<double>::max();
    test_true( STD::midpoint(dmax, dmax) == dmax );
    test_true( STD::midpoint(-dmax, dmax) == 0.0 );
    test_true( STD::midpoint(0.0, dmax) == dmax / 2 );
    const double dmin = STD::numeric_limits<double>::min();
    test_true( STD::midpoint(dmin, dmin) == dmin );
    test_true( STD::midpoint(1.0f, 2.0f) == 1.5f );

    int arr[5] = { 0, 1, 2, 3, 4 };
    test_eq( *STD::midpoint(arr, arr + 4), 2 );
    test_eq( *STD::midpoint(arr, arr + 5), 2 );
    test_eq( STD::midpoint(arr, arr), arr );
    test_pass("cxx20:midpoint (props)");
}
#endif

struct Minus3 { int operator()(int a, int b) const { return a - b; } };
struct Cat {
    STD::string operator()(const STD::string& a, const STD::string& b) const { return a + b; }
};
struct Tick {
    int v;
    Tick() : v(0) {}
    explicit Tick(int x) : v(x) {}
    Tick& operator++() { ++v; return *this; }
};

TEST_CASE(numeric, fold_properties) {
    int a[] = { 1, 2, 3 };

    test_eq( STD::accumulate(a, a + 3, 0, Minus3()), -6 );
    test_eq( STD::accumulate(a, a + 3, 100, Minus3()), 94 );
    STD::string s[] = { "a", "b", "c" };
    test_eq( STD::accumulate(s, s + 3, STD::string("_"), Cat()), STD::string("_abc") );

    double d[] = { 1.5, 2.5 };
    test_eq( STD::accumulate(d, d + 2, 0), 3 );
    test_true( STD::accumulate(d, d + 2, 0.0) == 4.0 );

    test_eq( STD::accumulate(a, a, 42), 42 );
    test_eq( STD::accumulate(a, a, 42, Minus3()), 42 );
    test_pass("cxx03:accumulate (fold)");

    int b2[] = { 10, 20, 30 };
    test_eq( STD::inner_product(a, a + 3, b2, 0), 140 );
    test_eq( STD::inner_product(a, a + 3, b2, 0, Minus3(), STD::multiplies<int>()), -140 );
    int shortr[] = { 10, 20 };
    test_eq( STD::inner_product(a, a + 2, shortr, 0), 50 );
    test_eq( STD::inner_product(a, a, b2, 7), 7 );
    test_pass("cxx03:inner_product (fold)");
}

TEST_CASE(numeric, in_place_scans) {

    int a[] = { 1, 2, 3, 4 };
    STD::partial_sum(a, a + 4, a);
    test_eq( a[0], 1 ); test_eq( a[1], 3 ); test_eq( a[2], 6 ); test_eq( a[3], 10 );
    test_pass("cxx03:partial_sum (in-place)");

    int b[] = { 1, 3, 6, 10 };
    STD::adjacent_difference(b, b + 4, b);
    test_eq( b[0], 1 ); test_eq( b[1], 2 ); test_eq( b[2], 3 ); test_eq( b[3], 4 );

    int c[] = { 5, -2, 7, 0, 3 };
    int sum[5], back[5];
    STD::partial_sum(c, c + 5, sum);
    STD::adjacent_difference(sum, sum + 5, back);
    for (int i = 0; i < 5; ++i) test_eq( back[i], c[i] );

    int one[] = { 9 };
    STD::partial_sum(one, one + 1, one);
    test_eq( one[0], 9 );
    STD::adjacent_difference(one, one + 1, one);
    test_eq( one[0], 9 );
    int* e = 0;
    test_eq( STD::partial_sum(e, e, e), (int*)0 );
    test_eq( STD::adjacent_difference(e, e, e), (int*)0 );
    test_pass("cxx03:adjacent_difference (in-place)");
}

TEST_CASE(numeric, iota_requirements) {

    Tick t[4];
    STD::iota(t, t + 4, Tick(7));
    test_eq( t[0].v, 7 ); test_eq( t[1].v, 8 ); test_eq( t[3].v, 10 );

    int store[3] = { 1, 2, 3 };
    int* ptrs[3];
    STD::iota(ptrs, ptrs + 3, store);
    test_eq( *ptrs[0], 1 ); test_eq( *ptrs[2], 3 );

    int empty[1] = { 55 };
    STD::iota(empty, empty, 0);
    test_eq( empty[0], 55 );
    test_pass("cxx11:iota (requirements)");
}

#if TEST_TARGET_CXX >= 2017
TEST_CASE(numeric, gcd_lcm_properties) {

    test_eq( STD::gcd(-12, 18), 6 );
    test_eq( STD::gcd(12, -18), 6 );
    test_eq( STD::gcd(-12, -18), 6 );
    test_eq( STD::gcd(0, 0), 0 );
    test_eq( STD::gcd(0, -7), 7 );
    test_eq( STD::gcd(-7, 0), 7 );
    test_eq( STD::gcd(13, 13), 13 );
    test_pass("cxx17:gcd (properties)");

    test_eq( STD::lcm(0, 0), 0 );
    test_eq( STD::lcm(0, 5), 0 );
    test_eq( STD::lcm(5, 0), 0 );
    test_eq( STD::lcm(-4, 6), 12 );
    test_eq( STD::lcm(4, -6), 12 );
    test_eq( STD::lcm(-4, -6), 12 );
    test_eq( STD::lcm(6, 4), 12 );
    test_eq( STD::lcm(7, 13), 91 );
    test_eq( STD::lcm(21, 6), 42 );

    for (int m = 1; m <= 12; ++m)
        for (int n = 1; n <= 12; ++n)
            test_eq( STD::gcd(m, n) * STD::lcm(m, n), m * n );

    test_eq( STD::gcd(1000000007LL * 2, 1000000007LL * 3), 1000000007LL );
    test_eq( STD::lcm(1LL << 20, 3LL << 20), 3LL << 20 );
    test_eq( (int)STD::gcd((unsigned)12, (unsigned)18), 6 );
    test_pass("cxx17:lcm (properties)");
}

TEST_CASE(numeric, reduce_scan_properties) {
    int a[] = { 1, 2, 3, 4 };

    test_eq( STD::reduce(a, a + 4, 0), STD::accumulate(a, a + 4, 0) );
    test_eq( STD::reduce(a, a, 99), 99 );
    test_eq( STD::reduce(a, a + 4), 10 );
    test_pass("cxx17:reduce (properties)");

    int b[] = { 10, 20, 30, 40 };
    test_eq( STD::transform_reduce(a, a + 4, b, 0), STD::inner_product(a, a + 4, b, 0) );
    test_eq( STD::transform_reduce(a, a, b, 5), 5 );
    test_pass("cxx17:transform_reduce (properties)");

    int ex[4], in[4];
    STD::exclusive_scan(a, a + 4, ex, 0);
    STD::inclusive_scan(a, a + 4, in);
    test_eq( ex[0], 0 ); test_eq( ex[1], 1 ); test_eq( ex[2], 3 ); test_eq( ex[3], 6 );
    test_eq( in[0], 1 ); test_eq( in[1], 3 ); test_eq( in[2], 6 ); test_eq( in[3], 10 );
    test_pass("cxx17:inclusive_scan (properties)");
    for (int i = 1; i < 4; ++i) test_eq( ex[i], in[i - 1] );

    STD::exclusive_scan(a, a + 4, ex, 100);
    test_eq( ex[0], 100 ); test_eq( ex[3], 106 );
    test_pass("cxx17:exclusive_scan (properties)");

    int ts[4];
    STD::transform_exclusive_scan(a, a + 4, ts, 0, STD::plus<int>(), Sq());
    test_eq( ts[0], 0 ); test_eq( ts[1], 1 ); test_eq( ts[2], 5 ); test_eq( ts[3], 14 );
    test_pass("cxx17:transform_exclusive_scan");

    double d[] = { 1e16, 1.0, -1e16 };
    test_true( STD::reduce(d, d + 3, 0.0) == STD::accumulate(d, d + 3, 0.0) );
    test_pass("cxx17:reduce (float)");
}
#endif

TEST_CASE(numeric, feature_test_macros) {
#if defined(__cpp_lib_gcd_lcm) && (__cpp_lib_gcd_lcm) >= 201606L
    test_true( (__cpp_lib_gcd_lcm) >= 201606L );
    test_pass("cxx17:__cpp_lib_gcd_lcm=201606L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx17:__cpp_lib_gcd_lcm=201606L");
#endif
#if defined(__cpp_lib_parallel_algorithm) && (__cpp_lib_parallel_algorithm) >= 201603L
    test_true( (__cpp_lib_parallel_algorithm) >= 201603L );
    test_pass("cxx17:__cpp_lib_parallel_algorithm=201603L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx17:__cpp_lib_parallel_algorithm=201603L");
#endif
#if defined(__cpp_lib_constexpr_numeric) && (__cpp_lib_constexpr_numeric) >= 201911L
    test_true( (__cpp_lib_constexpr_numeric) >= 201911L );
    test_pass("cxx20:__cpp_lib_constexpr_numeric=201911L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx20:__cpp_lib_constexpr_numeric=201911L");
#endif
#if defined(__cpp_lib_interpolate) && (__cpp_lib_interpolate) >= 201902L
    test_true( (__cpp_lib_interpolate) >= 201902L );
    test_pass("cxx20:__cpp_lib_interpolate=201902L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx20:__cpp_lib_interpolate=201902L");
#endif
#if defined(__cpp_lib_ranges_iota) && (__cpp_lib_ranges_iota) >= 202202L
    test_true( (__cpp_lib_ranges_iota) >= 202202L );
    test_pass("cxx23:__cpp_lib_ranges_iota=202202L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_ranges_iota=202202L");
#endif
#if defined(__cpp_lib_saturation_arithmetic) && (__cpp_lib_saturation_arithmetic) >= 202603L
    test_true( (__cpp_lib_saturation_arithmetic) >= 202603L );
    test_pass("cxx26:__cpp_lib_saturation_arithmetic=202603L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx26:__cpp_lib_saturation_arithmetic=202603L");
#endif
}

static int g_num_live = 0;

struct NumLive {
    int   v_;
    char* p_;
    NumLive() : v_(0), p_(new char[16]) { p_[0] = 0; ++g_num_live; }
    NumLive(int v) : v_(v), p_(new char[16]) { p_[0] = 0; ++g_num_live; }
    NumLive(const NumLive& o) : v_(o.v_), p_(new char[16]) { p_[0] = o.p_[0]; ++g_num_live; }
    NumLive& operator=(const NumLive& o) { v_ = o.v_; p_[0] = o.p_[0]; return *this; }
    ~NumLive() { delete[] p_; --g_num_live; }
    NumLive operator+(const NumLive& o) const { return NumLive(v_ + o.v_); }
    NumLive operator-(const NumLive& o) const { return NumLive(v_ - o.v_); }
    NumLive operator*(const NumLive& o) const { return NumLive(v_ * o.v_); }
    bool operator==(const NumLive& o) const { return v_ == o.v_; }
};

TEST_CASE(numeric, class_elements_lifetime) {
    g_num_live = 0;
    {
        STD::vector<NumLive> v;
        for (int i = 1; i <= 5; ++i) v.push_back(NumLive(i));
        test_eq( (int)v.size(), 5 );
        test_eq( g_num_live, 5 );

        NumLive sum = STD::accumulate(v.begin(), v.end(), NumLive(0));
        test_eq( sum.v_, 15 );
        test_eq( g_num_live, 6 );

        NumLive dot = STD::inner_product(v.begin(), v.end(), v.begin(), NumLive(0));
        test_eq( dot.v_, 55 );
        test_eq( g_num_live, 7 );

        STD::vector<NumLive> ps(5);
        test_eq( g_num_live, 12 );
        STD::partial_sum(v.begin(), v.end(), ps.begin());
        test_eq( ps[0].v_, 1 );
        test_eq( ps[4].v_, 15 );
        test_eq( g_num_live, 12 );

        STD::vector<NumLive> ad(5);
        STD::adjacent_difference(v.begin(), v.end(), ad.begin());
        test_eq( ad[0].v_, 1 );
        test_eq( ad[4].v_, 1 );
        test_eq( g_num_live, 17 );

        v.clear();
        test_eq( g_num_live, 12 );
    }
    test_eq( g_num_live, 0 );
}
