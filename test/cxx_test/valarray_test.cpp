#include "test_cxx.hpp"

#include <valarray>
#include <cstddef>

namespace {
    inline bool vnear(double a, double b) { double d = a - b; return (d < 0 ? -d : d) < 1e-9; }
    int va_sq(int x) { return x * x; }
}

TEST_CASE(valarray, construct_reduce) {
    double d[4] = { 1.0, 2.0, 3.0, 4.0 };
    STD::valarray<double> v(d, 4);
    test_eq( v.size(), 4u );
    test_pass("cxx03:valarray(const T*, size_t)");
    test_eq( v[0], 1.0 );
    test_eq( v[3], 4.0 );
    test_pass("cxx03:operator[]");
    test_eq( v.sum(), 10.0 );
    test_pass("cxx03:sum");
    test_eq( v.min(), 1.0 );
    test_pass("cxx03:min");
    test_eq( v.max(), 4.0 );
    test_pass("cxx03:max");

    STD::valarray<int> z(5u);
    test_eq( z.size(), 5u );
    test_pass("cxx03:size");
    test_eq( z[0], 0 );
    test_pass("cxx03:valarray(size_t)");

    STD::valarray<int> f(7, 3u);
    test_eq( f.size(), 3u );
    test_eq( f[2], 7 );
    test_eq( f.sum(), 21 );
    test_pass("cxx03:valarray(const T&, size_t)");

    STD::valarray<int> c(f);
    test_eq( c.sum(), 21 );
    test_pass("cxx03:valarray(const valarray&)");
    c.resize(2, 5);
    test_eq( c.size(), 2u );
    test_eq( c[1], 5 );
    test_pass("cxx03:resize");

}

#if _TST_HAS_CXX11_LIB_MEMBERS
TEST_CASE(valarray, cxx11_swap) {
    STD::valarray<int> s1(1, 2u), s2(9, 3u);
    s1.swap(s2);
    test_eq( s1.size(), 3u );
    test_eq( s1[0], 9 );
    test_eq( s2.size(), 2u );
    test_pass("cxx11:valarray::swap");
    using STD::swap;
    swap(s1, s2);
    test_eq( s1.size(), 2u );
    test_pass("cxx11:swap(valarray)");
}
#else
TEST_CASE_SKIP(valarray, cxx11_swap)
#endif

TEST_CASE(valarray, elementwise_ops) {
    double d[4] = { 1.0, 2.0, 3.0, 4.0 };
    STD::valarray<double> v(d, 4);

    STD::valarray<double> w = v * 2.0;
    test_eq( w[0], 2.0 );
    test_eq( w[3], 8.0 );
    test_pass("cxx03:operator*(valarray, scalar)");

    STD::valarray<double> s = v + w;
    test_eq( s[0], 3.0 );
    test_eq( s[3], 12.0 );
    test_pass("cxx03:operator+(valarray, valarray)");

    v *= 3.0;
    test_eq( v[1], 6.0 );
    test_pass("cxx03:operator*=(scalar)");

    STD::valarray<double> neg = -v;
    test_eq( neg[0], -3.0 );
    test_pass("cxx03:operator-() (unary)");

    STD::valarray<double> pos = +v;
    test_eq( pos[0], 3.0 );
    test_pass("cxx03:operator+() (unary)");

    STD::valarray<double> r = 12.0 / STD::valarray<double>(3.0, 4u);
    test_eq( r[0], 4.0 );
    test_pass("cxx03:operator/(scalar, valarray)");
}

TEST_CASE(valarray, integer_and_bitwise_ops) {
    int a[4] = { 0x0F, 0x10, 0x11, 0x12 };
    STD::valarray<int> v(a, 4);

    STD::valarray<int> m = v % 4;
    test_eq( m[0], 3 );
    test_eq( m[1], 0 );
    test_pass("cxx03:operator%(valarray, scalar)");

    STD::valarray<int> b = v & 0x0F;
    test_eq( b[0], 0x0F );
    test_eq( b[1], 0x00 );
    test_pass("cxx03:operator&(valarray, scalar)");

    STD::valarray<int> o = v | 0xF0;
    test_eq( o[0], 0xFF );
    test_pass("cxx03:operator|(valarray, scalar)");

    STD::valarray<int> x = v ^ v;
    test_eq( x.sum(), 0 );
    test_pass("cxx03:operator^(valarray, valarray)");

    STD::valarray<int> sl = v << 1;
    test_eq( sl[1], 0x20 );
    test_pass("cxx03:operator<<(valarray, scalar)");
    STD::valarray<int> sr = v >> 1;
    test_eq( sr[1], 0x08 );
    test_pass("cxx03:operator>>(valarray, scalar)");

    STD::valarray<int> n = ~STD::valarray<int>(0, 3u);
    test_eq( n[0], ~0 );
    test_pass("cxx03:operator~()");

    STD::valarray<int> c(a, 4);
    c &= 0x0F;  test_eq( c[1], 0 );
    test_pass("cxx03:operator&=(scalar)");
    c |= 0x01;  test_eq( c[1], 1 );
    test_pass("cxx03:operator|=(scalar)");
    c <<= 2;    test_eq( c[1], 4 );
    test_pass("cxx03:operator<<=(scalar)");
    c >>= 1;    test_eq( c[1], 2 );
    test_pass("cxx03:operator>>=(scalar)");
}

TEST_CASE(valarray, comparison_and_logical) {
    int a[4] = { 1, 2, 3, 4 };
    STD::valarray<int> v(a, 4);

    STD::valarray<bool> gt = v > 2;
    test_true( !gt[0] && !gt[1] && gt[2] && gt[3] );
    test_pass("cxx03:operator>(valarray, scalar)");

    STD::valarray<bool> eq = (v == v);
    test_true( eq[0] && eq[3] );
    test_pass("cxx03:operator==(valarray, valarray)");

    STD::valarray<bool> ne = (v != 3);
    test_true( ne[0] && !ne[2] );
    test_pass("cxx03:operator!=(valarray, scalar)");

    STD::valarray<bool> lt = (2 < v);
    test_true( !lt[1] && lt[2] );
    test_pass("cxx03:operator<(scalar, valarray)");

    STD::valarray<bool> nz = !(v > 2);
    test_true( nz[0] && !nz[3] );
    test_pass("cxx03:operator!()");

    STD::valarray<bool> both = (v > 1) && (v < 4);
    test_true( !both[0] && both[1] && both[2] && !both[3] );
    test_pass("cxx03:operator&&(valarray, valarray)");

    STD::valarray<bool> either = (v < 2) || (v > 3);
    test_true( either[0] && !either[1] && either[3] );
    test_pass("cxx03:operator||(valarray, valarray)");
}

TEST_CASE(valarray, shift_cshift_apply) {
    int a[5] = { 1, 2, 3, 4, 5 };
    STD::valarray<int> v(a, 5);

    STD::valarray<int> s = v.shift(2);
    test_eq( s[0], 3 ); test_eq( s[2], 5 ); test_eq( s[3], 0 ); test_eq( s[4], 0 );
    test_pass("cxx03:shift");

    STD::valarray<int> sn = v.shift(-2);
    test_eq( sn[0], 0 ); test_eq( sn[2], 1 ); test_eq( sn[4], 3 );
    test_pass("cxx03:shift (negative)");

    STD::valarray<int> c = v.cshift(2);
    test_eq( c[0], 3 ); test_eq( c[3], 1 ); test_eq( c[4], 2 );
    test_eq( c.sum(), v.sum() );
    test_pass("cxx03:cshift");

    STD::valarray<int> cn = v.cshift(-1);
    test_eq( cn[0], 5 ); test_eq( cn[1], 1 );
    test_eq( cn.sum(), v.sum() );
    test_pass("cxx03:cshift (negative)");

    test_eq( v.cshift(5)[0], 1 );
    test_pass("cxx03:cshift (full turn)");
}

TEST_CASE(valarray, math_functions) {
    double a[3] = { 1.0, 4.0, 9.0 };
    STD::valarray<double> v(a, 3);

    STD::valarray<double> r = STD::sqrt(v);
    test_true( vnear(r[0], 1.0) && vnear(r[1], 2.0) && vnear(r[2], 3.0) );
    test_pass("cxx03:sqrt");

    double n[3] = { -1.5, 2.5, -3.5 };
    STD::valarray<double> av = STD::abs(STD::valarray<double>(n, 3));
    test_true( vnear(av[0], 1.5) && vnear(av[2], 3.5) );
    test_pass("cxx03:abs");

    STD::valarray<double> e = STD::log(STD::exp(v));
    test_true( vnear(e[0], 1.0) && vnear(e[2], 9.0) );
    test_pass("cxx03:exp");
    test_true( 1 );
    test_pass("cxx03:log");

    STD::valarray<double> p = STD::pow(STD::valarray<double>(2.0, 3u), 3.0);
    test_true( vnear(p[0], 8.0) );
    test_pass("cxx03:pow(valarray, scalar)");
    STD::valarray<double> p2 = STD::pow(2.0, STD::valarray<double>(3.0, 2u));
    test_true( vnear(p2[0], 8.0) );
    test_pass("cxx03:pow(scalar, valarray)");
    STD::valarray<double> p3 = STD::pow(STD::valarray<double>(2.0, 2u), STD::valarray<double>(4.0, 2u));
    test_true( vnear(p3[0], 16.0) );
    test_pass("cxx03:pow(valarray, valarray)");

    STD::valarray<double> t = STD::atan2(STD::valarray<double>(0.0, 2u), STD::valarray<double>(1.0, 2u));
    test_true( vnear(t[0], 0.0) );
    test_pass("cxx03:atan2");

    STD::valarray<double> h(0.5, 2u);
    test_true( vnear(STD::sin(STD::asin(h))[0], 0.5) );
    test_pass("cxx03:sin");
    test_true( 1 );
    test_pass("cxx03:asin");
    test_true( vnear(STD::tanh(STD::valarray<double>(0.0, 1u))[0], 0.0) );
    test_pass("cxx03:tanh");
    test_true( vnear(STD::cos(STD::valarray<double>(0.0, 1u))[0], 1.0) );
    test_pass("cxx03:cos");
    test_true( vnear(STD::log10(STD::valarray<double>(100.0, 1u))[0], 2.0) );
    test_pass("cxx03:log10");
}

TEST_CASE(valarray, slice_view) {
    int a[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    STD::valarray<int> v(a, 10);

    STD::valarray<int> odd = v[STD::slice(1, 5, 2)];
    test_eq( odd.size(), (STD::size_t)5 );
    test_eq( odd[0], 1 ); test_eq( odd[1], 3 ); test_eq( odd[4], 9 );
    test_pass("cxx03:operator[](slice)");

    STD::slice s(0, 3, 3);
    test_eq( s.start(), (STD::size_t)0 );
    test_pass("cxx03:slice::start");
    test_eq( s.size(), (STD::size_t)3 );
    test_pass("cxx03:slice::size");
    test_eq( s.stride(), (STD::size_t)3 );
    test_pass("cxx03:slice::stride");
    STD::valarray<int> every3 = v[s];
    test_eq( every3[0], 0 ); test_eq( every3[1], 3 ); test_eq( every3[2], 6 );
    test_pass("cxx03:operator[](const slice&) lvalue");

    v[STD::slice(0, 5, 2)] = 0;
    test_eq( v[0], 0 ); test_eq( v[2], 0 ); test_eq( v[8], 0 );
    test_eq( v[1], 1 ); test_eq( v[9], 9 );
    TEST_NOTE("scalar rhs to slice_array is an extension");
    test_pass("cxx03:slice_array::operator=(scalar)");

    v[STD::slice(1, 5, 2)] *= STD::valarray<int>(10, 5);
    test_eq( v[1], 10 ); test_eq( v[9], 90 );
    test_pass("cxx03:slice_array::operator*=(valarray)");

    int b[3] = { 7, 8, 9 };
    STD::valarray<int> src(b, 3);
    STD::valarray<int> t(0, 6u);
    t[STD::slice(0, 3, 2)] = src;
    test_eq( t[0], 7 ); test_eq( t[2], 8 ); test_eq( t[4], 9 );
    test_eq( t[1], 0 );
    test_pass("cxx03:slice_array::operator=(valarray)");

    STD::valarray<int> gathered(t[STD::slice(0, 3, 2)]);
    test_eq( gathered.size(), (STD::size_t)3 );
    test_eq( gathered[2], 9 );
    test_pass("cxx03:valarray(const slice_array&)");
}

TEST_CASE(valarray, gslice_view) {

    int a[6] = { 1, 2, 3,
                 4, 5, 6 };
    STD::valarray<int> m(a, 6);

    STD::size_t len[2]  = { 2, 3 };
    STD::size_t str[2]  = { 3, 1 };
    STD::valarray<STD::size_t> l(len, 2), d(str, 2);
    STD::gslice g(0, l, d);

    test_eq( g.start(), (STD::size_t)0 );
    test_pass("cxx03:gslice::start");
    test_eq( g.size()[0], (STD::size_t)2 );
    test_pass("cxx03:gslice::size");
    test_eq( g.stride()[1], (STD::size_t)1 );
    test_pass("cxx03:gslice::stride");

    STD::valarray<int> all = m[g];
    test_eq( all.size(), (STD::size_t)6 );
    test_eq( all[0], 1 ); test_eq( all[3], 4 ); test_eq( all[5], 6 );
    test_pass("cxx03:operator[](gslice)");

    STD::size_t cl[1] = { 2 };
    STD::size_t cd[1] = { 3 };
    STD::valarray<STD::size_t> cl_(cl, 1), cd_(cd, 1);
    STD::valarray<int> col = m[STD::gslice(1, cl_, cd_)];
    test_eq( col.size(), (STD::size_t)2 );
    test_eq( col[0], 2 ); test_eq( col[1], 5 );
    test_pass("cxx03:operator[](gslice) column");

    m[STD::gslice(1, cl_, cd_)] = 0;
    test_eq( m[1], 0 ); test_eq( m[4], 0 );
    test_eq( m[0], 1 ); test_eq( m[2], 3 );
    test_pass("cxx03:gslice_array::operator=(scalar)");
}

TEST_CASE(valarray, mask_and_indirect_view) {
    int a[6] = { 1, 2, 3, 4, 5, 6 };
    STD::valarray<int> v(a, 6);

    STD::valarray<bool> even = (v % 2 == 0);
    STD::valarray<int> picked = v[even];
    test_eq( picked.size(), (STD::size_t)3 );
    test_eq( picked[0], 2 ); test_eq( picked[2], 6 );
    test_pass("cxx03:operator[](valarray<bool>) mask");

    v[v > 4] = 0;
    test_eq( v[4], 0 ); test_eq( v[5], 0 );
    test_eq( v[3], 4 );
    test_pass("cxx03:mask_array::operator=(scalar)");

    v[v == 0] += STD::valarray<int>(100, 2);
    test_eq( v[4], 100 );
    test_pass("cxx03:mask_array::operator+=(valarray)");

    STD::valarray<int> w(a, 6);
    STD::size_t idx[3] = { 5, 0, 3 };
    STD::valarray<STD::size_t> i(idx, 3);
    STD::valarray<int> got = w[i];
    test_eq( got.size(), (STD::size_t)3 );
    test_eq( got[0], 6 ); test_eq( got[1], 1 ); test_eq( got[2], 4 );
    test_pass("cxx03:operator[](valarray<size_t>) indirect");

    w[i] = -1;
    test_eq( w[5], -1 ); test_eq( w[0], -1 ); test_eq( w[3], -1 );
    test_eq( w[1], 2 );
    test_pass("cxx03:indirect_array::operator=(scalar)");
}

TEST_CASE(valarray, begin_end) {
    int a[4] = { 1, 2, 3, 4 };
    STD::valarray<int> v(a, 4);

    int sum = 0;
    for (int* p = STD::begin(v); p != STD::end(v); ++p) sum += *p;
    test_eq( sum, 10 );
    test_eq( (long)(STD::end(v) - STD::begin(v)), 4L );
    test_pass("cxx11:begin/end");

    const STD::valarray<int>& cv = v;
    test_eq( *STD::begin(cv), 1 );
    test_pass("cxx11:begin (const)");

    *STD::begin(v) = 9;
    test_eq( v[0], 9 );
    test_pass("cxx11:begin (writable)");
}

TEST_CASE(valarray, more_compound_and_compare) {
    int a[4] = { 20, 21, 22, 23 };
    STD::valarray<int> v(a, 4);

    STD::valarray<int> d(a, 4);
    d /= 2;    test_eq( d[0], 10 ); test_eq( d[3], 11 );
    test_pass("cxx03:operator/=(scalar)");
    STD::valarray<int> md(a, 4);
    md %= 5;   test_eq( md[0], 0 );  test_eq( md[2], 2 );
    test_pass("cxx03:operator%=(scalar)");
    STD::valarray<int> xr(a, 4);
    xr ^= xr;  test_eq( xr.sum(), 0 );
    test_pass("cxx03:operator^=(valarray)");

    STD::valarray<int> p(a, 4), q(2, 4u);
    p /= q;    test_eq( p[0], 10 ); test_eq( p[1], 10 );
    test_pass("cxx03:operator/=(valarray)");

    STD::valarray<bool> le = (v <= 21);
    test_true( le[0] && le[1] && !le[2] && !le[3] );
    test_pass("cxx03:operator<=(valarray, scalar)");
    STD::valarray<bool> ge = (v >= 22);
    test_true( !ge[0] && !ge[1] && ge[2] && ge[3] );
    test_pass("cxx03:operator>=(valarray, scalar)");
    STD::valarray<bool> ge2 = (22 >= v);
    test_true( ge2[0] && ge2[1] && ge2[2] && !ge2[3] );
    test_pass("cxx03:operator>=(scalar, valarray)");
}

TEST_CASE(valarray, ctors_assign_more) {
    STD::valarray<int> empty;
    test_eq( (long)empty.size(), 0L );
    test_pass("cxx03:valarray()");

    int raw[3] = { 1, 2, 3 };
    STD::valarray<int> src(raw, 3);
    STD::valarray<int> cp;
    cp = src;
    test_true( cp.size() == 3 && cp[2] == 3 );
    test_pass("cxx03:operator=(const valarray&)");

    STD::valarray<int> sc(0, 3);
    sc = 7;
    test_true( sc[0] == 7 && sc[1] == 7 && sc[2] == 7 );
    test_pass("cxx03:operator=(const T&)");

#if TEST_TARGET_CXX >= 2011 && _tst_cplusplus >= 201103L
    STD::valarray<int> il = { 4, 5, 6 };
    test_true( il.size() == 3 && il[1] == 5 );
    test_pass("cxx11:valarray(initializer_list)");
    STD::valarray<int> il2;
    il2 = { 8, 9 };
    test_true( il2.size() == 2 && il2[0] == 8 );
    test_pass("cxx11:operator=(initializer_list)");
#else
    test_skip("cxx11:valarray(initializer_list)");
    test_skip("cxx11:operator=(initializer_list)");
#endif

#if TEST_TARGET_CXX >= 2011
    static const int raw4[4] = { 1, 2, 3, 4 };
    STD::valarray<int> mv(raw4, 4);
    STD::valarray<int> mv2(STD::move(mv));
    test_true( mv2.size() == 4 && mv2[3] == 4 );
    test_pass("cxx11:valarray(valarray&&)");
    STD::valarray<int> mv3;
    mv3 = STD::move(mv2);
    test_true( mv3.size() == 4 && mv3[0] == 1 );
    test_pass("cxx11:operator=(valarray&&)");
#else
    test_skip("cxx11:valarray(valarray&&)");
    test_skip("cxx11:operator=(valarray&&)");
#endif
}

TEST_CASE(valarray, compound_and_binary) {
    STD::valarray<int> a(5, 3), b(2, 3);

    a += 1;   test_true( a[0] == 6 );  test_pass("cxx03:operator+=(scalar)");
    a -= 1;   test_true( a[0] == 5 );  test_pass("cxx03:operator-=(scalar)");
    a += b;   test_true( a[0] == 7 );  test_pass("cxx03:operator+=(valarray)");
    a -= b;   test_true( a[0] == 5 );  test_pass("cxx03:operator-=(valarray)");

    STD::valarray<int> d = a - b;
    test_true( d[0] == 3 && d[1] == 3 && d[2] == 3 );
    test_pass("cxx03:operator-(valarray, valarray)");
}

TEST_CASE(valarray, transcendental_and_apply) {
    STD::valarray<double> z(3);
    z[0] = 0.0; z[1] = 0.5; z[2] = 1.0;

    STD::valarray<double> ac = STD::acos(z);
    test_true( vnear(ac[0], 1.5707963267948966) );
    test_pass("cxx03:acos");
    STD::valarray<double> at = STD::atan(z);
    test_true( vnear(at[0], 0.0) );
    test_pass("cxx03:atan");
    STD::valarray<double> ta = STD::tan(z);
    test_true( vnear(ta[0], 0.0) );
    test_pass("cxx03:tan");
    STD::valarray<double> ch = STD::cosh(z);
    test_true( vnear(ch[0], 1.0) );
    test_pass("cxx03:cosh");
    STD::valarray<double> sh = STD::sinh(z);
    test_true( vnear(sh[0], 0.0) );
    test_pass("cxx03:sinh");

    int ir[3] = { 2, 3, 4 };
    STD::valarray<int> iv(ir, 3);
    STD::valarray<int> sq = iv.apply(va_sq);
    test_true( sq[0] == 4 && sq[1] == 9 && sq[2] == 16 );
    test_pass("cxx03:apply");
}

TEST_CASE(valarray, view_ctors_assign) {
    STD::valarray<int> src(10);
    for (STD::size_t i = 0; i < 10; ++i) src[i] = (int)i;

    STD::valarray<int> ds(3);
    ds = src[STD::slice(0, 3, 1)];
    test_true( ds[0] == 0 && ds[2] == 2 );
    test_pass("cxx03:operator=(const slice_array&)");

    STD::valarray<STD::size_t> lens(2), strides(2);
    lens[0] = 2; lens[1] = 2; strides[0] = 4; strides[1] = 1;
    STD::valarray<int> fg(src[STD::gslice(0, lens, strides)]);
    test_true( fg.size() == 4 && fg[0] == 0 );
    test_pass("cxx03:valarray(const gslice_array&)");

    STD::valarray<bool> mask(10);
    for (STD::size_t i = 0; i < 10; ++i) mask[i] = (i % 2 == 0);
    STD::valarray<int> fm(src[mask]);
    test_true( fm.size() == 5 && fm[0] == 0 && fm[1] == 2 );
    test_pass("cxx03:valarray(const mask_array&)");

    STD::valarray<STD::size_t> idx(3);
    idx[0] = 9; idx[1] = 0; idx[2] = 5;
    STD::valarray<int> fi(src[idx]);
    test_true( fi.size() == 3 && fi[0] == 9 && fi[2] == 5 );
    test_pass("cxx03:valarray(const indirect_array&)");
}


TEST_CASE(valarray, member_value_types_cxx11) {
    test_true(( STD::is_same<STD::valarray<int>::value_type, int>::value ));
    test_pass("cxx03:valarray::value_type");

    test_true(( STD::is_same<STD::slice_array<double>::value_type, double>::value ));
    test_pass("cxx11:slice_array::value_type");

    test_true(( STD::is_same<STD::gslice_array<double>::value_type, double>::value ));
    test_pass("cxx11:gslice_array::value_type");

    test_true(( STD::is_same<STD::mask_array<double>::value_type, double>::value ));
    test_pass("cxx11:mask_array::value_type");

    test_true(( STD::is_same<STD::indirect_array<double>::value_type, double>::value ));
    test_pass("cxx11:indirect_array::value_type");
}

TEST_CASE(valarray, iterator_types_cxx03) {
    STD::valarray<int> v(3);
    v[0] = 1; v[1] = 2; v[2] = 3;

    int* b = STD::begin(v);
    test_eq( *b, 1 );
    test_eq( (int)(STD::end(v) - STD::begin(v)), 3 );
    test_pass("cxx11:valarray::iterator");

    const STD::valarray<int>& cv = v;
    const int* cb = STD::begin(cv);
    test_eq( *cb, 1 );
    test_eq( (int)(STD::end(cv) - STD::begin(cv)), 3 );
    test_pass("cxx11:valarray::const_iterator");
}

TEST_CASE(valarray, feature_test_macros) {
#if defined(__cpp_lib_valarray) && (__cpp_lib_valarray) >= 202511L
    test_true( (__cpp_lib_valarray) >= 202511L );
    test_pass("cxx26:__cpp_lib_valarray=202511L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx26:__cpp_lib_valarray=202511L");
#endif
}
