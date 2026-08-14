#include "test_cxx.hpp"

#include <complex>
#include <cmath>

namespace { inline bool cnear(double a, double b) { double d = a - b; return (d < 0 ? -d : d) < 1e-9; } }

TEST_CASE(complex, basics) {
    STD::complex<double> a(3.0, 4.0);
    test_eq( a.real(), 3.0 );
    test_pass("cxx03:complex::real");
    test_eq( a.imag(), 4.0 );
    test_pass("cxx03:complex::imag");
    test_eq( STD::abs(a), 5.0 );
    test_pass("cxx03:abs");
    test_eq( STD::norm(a), 25.0 );
    test_pass("cxx03:norm");
    test_eq( STD::conj(a).imag(), -4.0 );
    test_pass("cxx03:conj");
}

TEST_CASE(complex, arithmetic) {
    STD::complex<double> c(1.0, 2.0), d(3.0, 4.0);
    test_true( (c + d) == STD::complex<double>(4.0, 6.0) );
    test_pass("cxx03:operator+");
    test_true( (c * d) == STD::complex<double>(-5.0, 10.0) );
    test_pass("cxx03:operator*");

    STD::complex<double> z(2.0, 0.0);
    z *= STD::complex<double>(0.0, 1.0);
    test_true( z == STD::complex<double>(0.0, 2.0) );
    test_pass("cxx03:complex::operator*=");
}

TEST_CASE(complex, transcendental) {
    STD::complex<double> r = STD::sqrt(STD::complex<double>(4.0, 0.0));
    test_true( cnear(r.real(), 2.0) );
    test_true( cnear(r.imag(), 0.0) );
    test_pass("cxx03:sqrt");

    STD::complex<double> e = STD::exp(STD::complex<double>(0.0, 3.14159265358979));
    test_true( STD::fabs(e.real() + 1.0) < 1e-6 );
    test_true( STD::fabs(e.imag()) < 1e-6 );
    test_pass("cxx03:exp");
}

#if TEST_TARGET_CXX >= 2011
TEST_CASE(complex, inverse_trig_roundtrip) {
#if defined(_MSC_VER) && _MSC_VER >= 1700 && _MSC_VER < 1900
    TEST_SKIP_VC("MSVC 11/12 proj() drops the imaginary part of a finite value");
#endif
    test_true( STD::proj(STD::complex<double>(1.0, 2.0)) == STD::complex<double>(1.0, 2.0) );
    test_pass("cxx11:proj");

    STD::complex<double> w(0.5, 0.3);
    STD::complex<double> rt = STD::sin(STD::asin(w));
    test_true( cnear(rt.real(), 0.5) && cnear(rt.imag(), 0.3) );
    test_pass("cxx11:asin");

    STD::complex<double> rt2 = STD::tanh(STD::atanh(w));
    test_true( cnear(rt2.real(), 0.5) && cnear(rt2.imag(), 0.3) );
    test_pass("cxx11:atanh");

    STD::complex<double> rt3 = STD::cosh(STD::acosh(STD::complex<double>(1.5, 0.0)));
    test_true( cnear(rt3.real(), 1.5) );
    test_pass("cxx11:acosh");
}
#endif

#if TEST_TARGET_CXX >= 2011
#include <complex>
#include <cmath>
#include <sstream>
#include <string>
#include <type_traits>

namespace {

template <class _Want, class _Got>
bool same_type(const _Got&) { return STD::is_same<_Want, _Got>::value; }

const double kPi = 3.14159265358979323846;

static bool near_eq(double a, double b) { return STD::fabs(a - b) < 1e-9; }

static bool near_eq(const STD::complex<double>& a, const STD::complex<double>& b) {
    return near_eq(a.real(), b.real()) && near_eq(a.imag(), b.imag());
}

static bool near_eqf(float a, float b) { return STD::fabs(a - b) < 1e-5f; }

static bool near_eql(long double a, long double b) { return STD::fabs(a - b) < 1e-9L; }

}

TEST_CASE(complex, class_template_and_specializations) {

    typedef STD::complex<float>       cf;
    typedef STD::complex<double>      cd;
    typedef STD::complex<long double> cl;

    test_true( (STD::is_same<cf::value_type, float>::value) );
    test_true( (STD::is_same<cd::value_type, double>::value) );
    test_true( (STD::is_same<cl::value_type, long double>::value) );
    test_pass("cxx03:complex::value_type");

    cf  a(1.0f, 2.0f);
    cd  b(1.0,  2.0);
    cl  c(1.0L, 2.0L);
    test_true( near_eqf(a.real(), 1.0f) );
    test_true( near_eq (b.imag(), 2.0)  );
    test_true( near_eql(c.real(), 1.0L) );
    test_pass("cxx03:complex");

    test_eq( sizeof(cd), sizeof(double) * 2 );
    test_pass("cxx03:complex#layout");
}

TEST_CASE(complex, constructors) {

    STD::complex<double> z0;
    test_true( near_eq(z0.real(), 0.0) );
    test_true( near_eq(z0.imag(), 0.0) );
    test_pass("cxx03:complex::complex#default");

    STD::complex<double> z1(3.0);
    test_true( near_eq(z1.real(), 3.0) );
    test_true( near_eq(z1.imag(), 0.0) );
    test_pass("cxx03:complex::complex#value");

    STD::complex<double> z2(3.0, 4.0);
    test_true( near_eq(z2.real(), 3.0) );
    test_true( near_eq(z2.imag(), 4.0) );
    test_pass("cxx03:complex::complex");

    STD::complex<double> z3(z2);
    test_true( z3 == z2 );
    test_pass("cxx03:complex::complex#copy");

    STD::complex<float>       f(1.5f, -2.5f);
    STD::complex<double>      d(f);
    STD::complex<long double> l(d);
    test_true( near_eq (d.real(),  1.5)  );
    test_true( near_eq (d.imag(), -2.5)  );
    test_true( near_eql(l.real(),  1.5L) );
    test_true( near_eql(l.imag(), -2.5L) );
    test_pass("cxx03:complex::complex#converting");

    STD::complex<float> back(d);
    test_true( near_eqf(back.imag(), -2.5f) );
    test_pass("cxx03:complex::complex#narrowing");
}

TEST_CASE(complex, real_imag_accessors) {
    STD::complex<double> z(3.0, 4.0);

    test_true( near_eq(z.real(), 3.0) );
    test_true( near_eq(z.imag(), 4.0) );
    test_pass("cxx03:complex#accessors_get");

    z.real(-1.0);
    test_true( near_eq(z.real(), -1.0) );
    test_true( near_eq(z.imag(),  4.0) );
    test_pass("cxx11:complex::real#set");
    z.imag(-2.0);
    test_true( near_eq(z.imag(), -2.0) );
    test_pass("cxx11:complex::imag#set");

    STD::complex<float> f;
    f.real(7.0f);
    f.imag(8.0f);
    test_true( near_eqf(f.real(), 7.0f) );
    test_true( near_eqf(f.imag(), 8.0f) );
    test_pass("cxx11:complex#set_float");

    STD::complex<long double> l;
    l.real(7.0L);
    l.imag(8.0L);
    test_true( near_eql(l.real(), 7.0L) );
    test_true( near_eql(l.imag(), 8.0L) );
    test_pass("cxx11:complex#set_longdouble");
}

TEST_CASE(complex, member_scalar_operators) {

    STD::complex<double> z(1.0, 1.0);
    z = 5.0;
    test_true( near_eq(z.real(), 5.0) );
    test_true( near_eq(z.imag(), 0.0) );
    test_pass("cxx03:complex::operator=#scalar");

    z = STD::complex<double>(3.0, 4.0);
    z += 1.0;
    test_true( z == STD::complex<double>(4.0, 4.0) );
    test_pass("cxx03:complex::operator+=#scalar");
    z -= 2.0;
    test_true( z == STD::complex<double>(2.0, 4.0) );
    test_pass("cxx03:complex::operator-=#scalar");
    z *= 2.0;
    test_true( z == STD::complex<double>(4.0, 8.0) );
    test_pass("cxx03:complex::operator*=#scalar");
    z /= 4.0;
    test_true( z == STD::complex<double>(1.0, 2.0) );
    test_pass("cxx03:complex::operator/=#scalar");
}

TEST_CASE(complex, member_complex_operators) {

    STD::complex<double> a(1.0, 2.0);
    STD::complex<double> b;
    b = a;
    test_true( b == a );
    test_pass("cxx03:complex::operator=#copy");

    STD::complex<float>  f(3.0f, 4.0f);
    STD::complex<double> d;
    d = f;
    test_true( near_eq(d.real(), 3.0) && near_eq(d.imag(), 4.0) );
    test_pass("cxx03:complex::operator=#complexX");

    STD::complex<double> z(1.0, 2.0);
    z += STD::complex<float>(1.0f, 1.0f);
    test_true( near_eq(z, STD::complex<double>(2.0, 3.0)) );
    test_pass("cxx03:complex::operator+=#complex");
    z -= STD::complex<float>(0.5f, 0.5f);
    test_true( near_eq(z, STD::complex<double>(1.5, 2.5)) );
    test_pass("cxx03:complex::operator-=#complex");

    STD::complex<double> m(1.0, 2.0);
    m *= STD::complex<float>(3.0f, 4.0f);
    test_true( near_eq(m, STD::complex<double>(-5.0, 10.0)) );
    test_pass("cxx03:complex::operator*=#mixed");

    m /= STD::complex<float>(3.0f, 4.0f);
    test_true( near_eq(m, STD::complex<double>(1.0, 2.0)) );
    test_pass("cxx03:complex::operator/=#complex");

    STD::complex<double> s(1.0, 1.0);
    s *= STD::complex<double>(0.0, 1.0);
    test_true( near_eq(s, STD::complex<double>(-1.0, 1.0)) );
    test_pass("cxx03:complex::operator*=#same");
}

TEST_CASE(complex, nonmember_arithmetic) {
    const STD::complex<double> a(1.0, 2.0);
    const STD::complex<double> b(3.0, 4.0);
    const double               k = 2.0;

    test_true( near_eq(a + b, STD::complex<double>(4.0, 6.0)) );
    test_true( near_eq(a + k, STD::complex<double>(3.0, 2.0)) );
    test_true( near_eq(k + a, STD::complex<double>(3.0, 2.0)) );
    test_pass("cxx03:operator+#forms");

    test_true( near_eq(a - b, STD::complex<double>(-2.0, -2.0)) );
    test_true( near_eq(a - k, STD::complex<double>(-1.0,  2.0)) );
    test_true( near_eq(k - a, STD::complex<double>( 1.0, -2.0)) );
    test_pass("cxx03:operator-");

    test_true( near_eq(a * b, STD::complex<double>(-5.0, 10.0)) );
    test_true( near_eq(a * k, STD::complex<double>( 2.0,  4.0)) );
    test_true( near_eq(k * a, STD::complex<double>( 2.0,  4.0)) );
    test_pass("cxx03:operator*#forms");

    test_true( near_eq(STD::complex<double>(-5.0, 10.0) / b, a) );
    test_true( near_eq(a / k, STD::complex<double>(0.5, 1.0)) );

    test_true( near_eq(k / a, STD::complex<double>(0.4, -0.8)) );
    test_pass("cxx03:operator/");

    test_true( near_eq(+a, a) );
    test_pass("cxx03:operator+#unary");
    test_true( near_eq(-a, STD::complex<double>(-1.0, -2.0)) );
    test_pass("cxx03:operator-#unary");

    STD::complex<float> fa(1.0f, 2.0f);
    STD::complex<float> fs = fa + 1.0f;
    test_true( near_eqf(fs.real(), 2.0f) );
    test_pass("cxx03:operator+#float");
    STD::complex<long double> la(1.0L, 2.0L);
    STD::complex<long double> ls = 1.0L - la;
    test_true( near_eql(ls.imag(), -2.0L) );
    test_pass("cxx03:operator-#longdouble");
}

TEST_CASE(complex, comparison_operators) {
    const STD::complex<double> a(1.0, 2.0);
    const STD::complex<double> b(1.0, 2.0);
    const STD::complex<double> c(1.0, 0.0);
    const STD::complex<double> d(3.0, 0.0);

    test_eq( a, b );
    test_true( !(a == c) );
    test_true(  c == 1.0 );
    test_true(  1.0 == c );
    test_true( !(d == 1.0) );
    test_pass("cxx03:operator==");

    test_true( !(a != b) );
    test_ne( a, c );
    test_true(  d != 1.0 );
    test_true(  1.0 != d );
    test_pass("cxx03:operator!=");
}

TEST_CASE(complex, stream_io) {

    STD::ostringstream os;
    os << STD::complex<double>(1.0, 2.0);
    STD::string s = os.str();
    test_true( s == "(1,2)" );
    test_pass("cxx03:operator<<");

    STD::complex<double> z;
    STD::istringstream is1("(3,4)");
    is1 >> z;
    test_true( !is1.fail() );
    test_true( near_eq(z, STD::complex<double>(3.0, 4.0)) );
    test_pass("cxx03:operator>>");

    STD::complex<double> z2;
    STD::istringstream is2("(5)");
    is2 >> z2;
    test_true( !is2.fail() );
    test_true( near_eq(z2, STD::complex<double>(5.0, 0.0)) );
    test_pass("cxx03:operator>>#one");

    STD::complex<double> z3;
    STD::istringstream is3("6");
    is3 >> z3;
    test_true( !is3.fail() );
    test_true( near_eq(z3, STD::complex<double>(6.0, 0.0)) );
    test_pass("cxx03:operator>>#scalar");

    STD::ostringstream os2;
    os2 << STD::complex<float>(1.5f, -2.5f);
    STD::istringstream is4(os2.str());
    STD::complex<float> f;
    is4 >> f;
    test_true( near_eqf(f.real(),  1.5f) );
    test_true( near_eqf(f.imag(), -2.5f) );
    test_pass("cxx03:operator>>#roundtrip");
}

TEST_CASE(complex, value_operations) {
    const STD::complex<double> z(3.0, 4.0);

    test_true( near_eq(STD::real(z), 3.0) );
    test_pass("cxx03:real");
    test_true( near_eq(STD::imag(z), 4.0) );
    test_pass("cxx03:imag");
    test_true( near_eq(STD::abs(z),  5.0) );
    test_pass("cxx03:abs#free");
    test_true( near_eq(STD::norm(z), 25.0) );
    test_pass("cxx03:norm#free");
    test_true( near_eq(STD::arg(z),  STD::atan2(4.0, 3.0)) );
    test_pass("cxx03:arg");
    test_true( near_eq(STD::conj(z), STD::complex<double>(3.0, -4.0)) );
    test_pass("cxx03:conj#free");

    test_true( near_eq(STD::arg(STD::complex<double>(1.0, 0.0)), 0.0) );
    test_true( near_eq(STD::arg(STD::complex<double>(0.0, 1.0)), kPi / 2) );
    test_pass("cxx03:arg#axis");

#if defined(_MSC_VER) && _MSC_VER >= 1700 && _MSC_VER < 1900
    TEST_SKIP_VC("MSVC 11/12 proj() drops the imaginary part of a finite value");
#endif
    test_true( near_eq(STD::proj(z), z) );
    test_pass("cxx11:proj#finite");

    STD::complex<double> p = STD::polar(2.0, kPi / 2);
    test_true( near_eq(p, STD::complex<double>(0.0, 2.0)) );
    test_pass("cxx03:polar");
    STD::complex<double> p0 = STD::polar(3.0);
    test_true( near_eq(p0, STD::complex<double>(3.0, 0.0)) );
    test_pass("cxx03:polar#default");

    STD::complex<double> q = STD::polar(STD::abs(z), STD::arg(z));
    test_true( near_eq(q, z) );
    test_pass("cxx03:polar#consistency");

    test_true( near_eqf(STD::abs(STD::complex<float>(3.0f, 4.0f)), 5.0f) );
    test_pass("cxx03:abs#float");
    test_true( near_eql(STD::norm(STD::complex<long double>(3.0L, 4.0L)), 25.0L) );
    test_pass("cxx03:norm#longdouble");
}

TEST_CASE(complex, arithmetic_type_overloads) {

    test_true( near_eq(STD::real(3.0), 3.0) );
    test_pass("cxx11:real#scalar");
    test_true( near_eq(STD::imag(3.0), 0.0) );
    test_pass("cxx11:imag#scalar");
    test_true( near_eq(STD::abs(-3.0), 3.0) );
    test_pass("cxx11:abs#scalar");
    test_true( near_eq(STD::norm(3.0), 9.0) );
    test_pass("cxx11:norm#scalar");
    test_true( near_eq(STD::arg(1.0),  0.0) );
    test_true( near_eq(STD::arg(-1.0), kPi) );
    test_pass("cxx11:arg#scalar");
    test_true( near_eq(STD::conj(3.0), STD::complex<double>(3.0, 0.0)) );
    test_pass("cxx11:conj#scalar");
    test_true( near_eq(STD::proj(3.0), STD::complex<double>(3.0, 0.0)) );
    test_pass("cxx11:proj#scalar");

    test_true( same_type<STD::complex<double> >(STD::conj(3)) );
    test_true( near_eq(STD::norm(3), 9.0) );
    test_pass("cxx11:conj#int");

    test_true( same_type<STD::complex<float> >(STD::conj(3.0f)) );
    test_true( near_eqf(STD::real(3.0f), 3.0f) );
    test_pass("cxx11:conj#float");

    test_true( same_type<STD::complex<long double> >(STD::proj(3.0L)) );
    test_true( near_eql(STD::norm(3.0L), 9.0L) );
    test_pass("cxx11:proj#longdouble");
}

TEST_CASE(complex, exp_log_sqrt) {

    test_true( near_eq(STD::exp(STD::complex<double>(0.0, kPi)),
                       STD::complex<double>(-1.0, 0.0)) );
    test_true( near_eq(STD::exp(STD::complex<double>(1.0, 0.0)),
                       STD::complex<double>(STD::exp(1.0), 0.0)) );
    test_pass("cxx03:exp#values");

    STD::complex<double> z(0.3, 0.7);
    test_true( near_eq(STD::log(STD::exp(z)), z) );
    test_true( near_eq(STD::log(STD::complex<double>(1.0, 0.0)),
                       STD::complex<double>(0.0, 0.0)) );

    test_true( near_eq(STD::log(STD::complex<double>(-1.0, 0.0)),
                       STD::complex<double>(0.0, kPi)) );
    test_pass("cxx03:log");

    test_true( near_eq(STD::log10(STD::complex<double>(100.0, 0.0)),
                       STD::complex<double>(2.0, 0.0)) );
    test_pass("cxx03:log10");

    test_true( near_eq(STD::sqrt(STD::complex<double>(-1.0, 0.0)),
                       STD::complex<double>(0.0, 1.0)) );
    STD::complex<double> r = STD::sqrt(z);
    test_true( near_eq(r * r, z) );
    test_pass("cxx03:sqrt#values");

    STD::complex<float> fe = STD::exp(STD::complex<float>(0.0f, 0.0f));
    test_true( near_eqf(fe.real(), 1.0f) );
    test_pass("cxx03:exp#float");
    STD::complex<long double> ls = STD::sqrt(STD::complex<long double>(4.0L, 0.0L));
    test_true( near_eql(ls.real(), 2.0L) );
    test_pass("cxx03:sqrt#longdouble");
}

TEST_CASE(complex, trigonometric) {
    const STD::complex<double> z(0.3, 0.4);

    STD::complex<double> s = STD::sin(z);
    STD::complex<double> c = STD::cos(z);
    test_true( near_eq(s * s + c * c, STD::complex<double>(1.0, 0.0)) );
    test_pass("cxx03:sin");

    test_true( near_eq(STD::tan(z), s / c) );
    test_pass("cxx03:tan");

    test_true( near_eq(STD::sin(STD::complex<double>(1.0, 0.0)),
                       STD::complex<double>(STD::sin(1.0), 0.0)) );
    test_pass("cxx03:sin#realaxis");
    test_true( near_eq(STD::cos(STD::complex<double>(0.0, 0.0)),
                       STD::complex<double>(1.0, 0.0)) );
    test_pass("cxx03:cos");

    test_true( near_eq(STD::sin(STD::complex<double>(0.0, 1.0)),
                       STD::complex<double>(0.0, STD::sinh(1.0))) );
    test_pass("cxx03:sin#imag");

    test_true( near_eq(STD::asin(STD::sin(z)), z) );
    test_pass("cxx11:asin#roundtrip");
    test_true( near_eq(STD::atan(STD::tan(z)), z) );
    test_pass("cxx11:atan");

    STD::complex<double> w(0.6, 0.2);
    test_true( near_eq(STD::cos(STD::acos(w)), w) );
    test_pass("cxx11:acos");
    test_true( near_eq(STD::asin(STD::complex<double>(0.0, 0.0)),
                       STD::complex<double>(0.0, 0.0)) );
    test_pass("cxx11:asin#zero");
    test_true( near_eq(STD::acos(STD::complex<double>(1.0, 0.0)),
                       STD::complex<double>(0.0, 0.0)) );
    test_pass("cxx11:acos#one");
    test_true( near_eq(STD::atan(STD::complex<double>(0.0, 0.0)),
                       STD::complex<double>(0.0, 0.0)) );
    test_pass("cxx11:atan#zero");

    STD::complex<float> fs = STD::sin(STD::complex<float>(0.0f, 0.0f));
    test_true( near_eqf(fs.real(), 0.0f) );
    test_pass("cxx03:sin#float");
    STD::complex<long double> lc = STD::cos(STD::complex<long double>(0.0L, 0.0L));
    test_true( near_eql(lc.real(), 1.0L) );
    test_pass("cxx03:cos#longdouble");
    STD::complex<float> ft = STD::tan(STD::complex<float>(0.0f, 0.0f));
    test_true( near_eqf(ft.real(), 0.0f) );
    test_pass("cxx03:tan#float");
}

TEST_CASE(complex, hyperbolic) {
    const STD::complex<double> z(0.3, 0.4);

    STD::complex<double> sh = STD::sinh(z);
    STD::complex<double> ch = STD::cosh(z);
    test_true( near_eq(ch * ch - sh * sh, STD::complex<double>(1.0, 0.0)) );
    test_pass("cxx03:sinh");

    test_true( near_eq(STD::tanh(z), sh / ch) );
    test_pass("cxx03:tanh");

    test_true( near_eq(STD::sinh(STD::complex<double>(1.0, 0.0)),
                       STD::complex<double>(STD::sinh(1.0), 0.0)) );
    test_pass("cxx03:sinh#realaxis");
    test_true( near_eq(STD::cosh(STD::complex<double>(0.0, 0.0)),
                       STD::complex<double>(1.0, 0.0)) );
    test_pass("cxx03:cosh");

    test_true( near_eq(STD::asinh(STD::sinh(z)), z) );
    test_pass("cxx11:asinh");
    test_true( near_eq(STD::atanh(STD::tanh(z)), z) );
    test_pass("cxx11:atanh#roundtrip");

    STD::complex<double> w(1.7, 0.3);
    test_true( near_eq(STD::cosh(STD::acosh(w)), w) );
    test_pass("cxx11:acosh#roundtrip");
    test_true( near_eq(STD::acosh(STD::complex<double>(1.0, 0.0)),
                       STD::complex<double>(0.0, 0.0)) );
    test_pass("cxx11:acosh#one");
    test_true( near_eq(STD::asinh(STD::complex<double>(0.0, 0.0)),
                       STD::complex<double>(0.0, 0.0)) );
    test_pass("cxx11:asinh#zero");
    test_true( near_eq(STD::atanh(STD::complex<double>(0.0, 0.0)),
                       STD::complex<double>(0.0, 0.0)) );
    test_pass("cxx11:atanh#zero");

    STD::complex<float> fsh = STD::sinh(STD::complex<float>(0.0f, 0.0f));
    test_true( near_eqf(fsh.real(), 0.0f) );
    test_pass("cxx03:sinh#float");
    STD::complex<long double> lth = STD::tanh(STD::complex<long double>(0.0L, 0.0L));
    test_true( near_eql(lth.real(), 0.0L) );
    test_pass("cxx03:tanh#longdouble");
}

TEST_CASE(complex, power) {
    const STD::complex<double> z(1.0, 2.0);

    test_true( near_eq(STD::pow(z, STD::complex<double>(2.0, 0.0)), z * z) );
    test_pass("cxx03:pow");

    test_true( near_eq(STD::pow(z, 2.0), z * z) );
    test_pass("cxx03:pow#complex_scalar");
    test_true( near_eq(STD::pow(STD::complex<double>(0.0, 1.0), 2.0),
                       STD::complex<double>(-1.0, 0.0)) );
    test_pass("cxx03:pow#i_squared");

    test_true( near_eq(STD::pow(2.0, STD::complex<double>(3.0, 0.0)),
                       STD::complex<double>(8.0, 0.0)) );
    test_pass("cxx03:pow#scalar_complex");

    test_true( near_eq(STD::pow(z, 0.0), STD::complex<double>(1.0, 0.0)) );
    test_pass("cxx03:pow#zero");

    test_true( near_eq(STD::pow(z, 0.5), STD::sqrt(z)) );
    test_pass("cxx03:pow#half");

    STD::complex<float> f(1.0f, 2.0f);
    test_true( same_type<STD::complex<double> >(STD::pow(f, 2.0)) );
    test_true( near_eq(STD::pow(f, 2.0), STD::complex<double>(-3.0, 4.0)) );
    test_pass("cxx11:pow#mixed_scalar");

    test_true( same_type<STD::complex<double> >(STD::pow(z, 2)) );
    test_true( near_eq(STD::pow(z, 2), z * z) );
    test_pass("cxx11:pow#int_exp");

    test_true( same_type<STD::complex<double> >(STD::pow(2.0, f)) );
    test_true( near_eq(STD::pow(2.0f, STD::complex<double>(3.0, 0.0)),
                       STD::complex<double>(8.0, 0.0)) );
    test_pass("cxx11:pow#mixed_base");

    test_true( same_type<STD::complex<double> >(STD::pow(f, z)) );
    test_true( near_eq(STD::pow(f, STD::complex<double>(2.0, 0.0)),
                       STD::complex<double>(-3.0, 4.0)) );
    test_pass("cxx11:pow#mixed_both");

    STD::complex<long double> lp = STD::pow(STD::complex<long double>(2.0L, 0.0L), 3.0L);
    test_true( near_eql(lp.real(), 8.0L) );
    test_pass("cxx11:pow#longdouble");
}

#if !_TST_HAS_UDL || TEST_TARGET_CXX < 2014
TEST_CASE_SKIP(complex, literals)
#else
TEST_CASE(complex, literals) {
    using namespace STD::complex_literals;

    STD::complex<double> a = 2.0i;
    STD::complex<double> b = 2i;
    test_true( near_eq(a, STD::complex<double>(0.0, 2.0)) );
    test_true( near_eq(b, STD::complex<double>(0.0, 2.0)) );
    test_true( same_type<STD::complex<double> >(2i) );
    test_pass("cxx14:complex_literals::i");

    STD::complex<float> c = 2.0if;
    STD::complex<float> d = 2if;
    test_true( near_eqf(c.imag(), 2.0f) );
    test_true( near_eqf(d.imag(), 2.0f) );
    test_true( same_type<STD::complex<float> >(2if) );
    test_pass("cxx14:complex_literals::if");

    STD::complex<long double> e = 2.0il;
    STD::complex<long double> g = 2il;
    test_true( near_eql(e.imag(), 2.0L) );
    test_true( near_eql(g.imag(), 2.0L) );
    test_true( same_type<STD::complex<long double> >(2il) );
    test_pass("cxx14:complex_literals::il");

    STD::complex<double> z = 1.0 + 2.0i;
    test_true( near_eq(z, STD::complex<double>(1.0, 2.0)) );
    test_true( near_eq(1.0i * 1.0i, STD::complex<double>(-1.0, 0.0)) );
    test_pass("cxx14:complex_literals#compose");
}

#endif

#if defined(__cpp_lib_complex_tuple_interface)
TEST_CASE(complex, tuple_interface_chk) {
    typedef STD::complex<double> cd;
    test_eq( (STD::tuple_size<cd>::value), STD::size_t(2) );
    test_pass("cxx26:tuple_size<complex>");
    test_true( (STD::is_same<STD::tuple_element<0, cd>::type, double>::value) );
    test_true( (STD::is_same<STD::tuple_element<1, cd>::type, double>::value) );
    test_pass("cxx26:tuple_element<complex>");

    cd z(3.0, 4.0);
    test_true( near_eq(STD::get<0>(z), 3.0) );
    test_true( near_eq(STD::get<1>(z), 4.0) );
    test_pass("cxx26:get<complex>");

    const cd cz(5.0, 6.0);
    test_true( near_eq(STD::get<0>(cz), 5.0) );
    test_pass("cxx26:get<complex>");
    test_true( near_eq(STD::get<1>(STD::move(cz)), 6.0) );
    test_true( near_eq(STD::get<0>(cd(7.0, 8.0)), 7.0) );
    test_pass("cxx26:get<complex>");

    STD::get<0>(z) = 9.0;
    test_true( near_eq(z.real(), 9.0) );
    test_pass("cxx26:get<complex>");
}
#else
TEST_CASE_SKIP(complex, tuple_interface)
#endif

TEST_CASE(complex, tuple_spec_names) {
#if defined(__cpp_lib_complex_tuple_interface) || defined(_CCW_LIB_COMPLEX_TUPLE_TRAITS)
    typedef STD::complex<double> cd;
    STD::size_t n = STD::tuple_size<cd>::value;
    test_eq( n, STD::size_t(2) );
    test_pass("cxx26:tuple_size<complex>");
    bool e0 = STD::is_same<STD::tuple_element<0, cd>::type, double>::value;
    bool e1 = STD::is_same<STD::tuple_element<1, cd>::type, double>::value;
    bool ec = STD::is_same<STD::tuple_element<0, const cd>::type, const double>::value;
    test_true( e0 );
    test_true( e1 );
    test_true( ec );
    test_pass("cxx26:tuple_element<complex>");
#else
    test_skip("cxx26:tuple_size<complex>");
    test_skip("cxx26:tuple_element<complex>");
#endif
#if defined(__cpp_lib_complex_tuple_interface) || defined(_CCW_LIB_COMPLEX_TUPLE_GET)
    cd z2(3.0, 4.0);
    test_true( near_eq(STD::get<0>(z2), 3.0) );
    test_true( near_eq(STD::get<1>(z2), 4.0) );
    const cd cz2(5.0, 6.0);
    test_true( near_eq(STD::get<1>(cz2), 6.0) );
    STD::get<0>(z2) = 9.0;
    test_true( near_eq(z2.real(), 9.0) );
    test_pass("cxx26:get<complex>");
#else
    test_skip("cxx26:get<complex>");
#endif
}
#else
TEST_CASE_SKIP(complex, tuple_spec_names)
TEST_CASE_SKIP(complex, class_template_and_specializations)
TEST_CASE_SKIP(complex, constructors)
TEST_CASE_SKIP(complex, real_imag_accessors)
TEST_CASE_SKIP(complex, member_scalar_operators)
TEST_CASE_SKIP(complex, member_complex_operators)
TEST_CASE_SKIP(complex, nonmember_arithmetic)
TEST_CASE_SKIP(complex, comparison_operators)
TEST_CASE_SKIP(complex, stream_io)
TEST_CASE_SKIP(complex, value_operations)
TEST_CASE_SKIP(complex, arithmetic_type_overloads)
TEST_CASE_SKIP(complex, exp_log_sqrt)
TEST_CASE_SKIP(complex, trigonometric)
TEST_CASE_SKIP(complex, hyperbolic)
TEST_CASE_SKIP(complex, power)
TEST_CASE_SKIP(complex, literals)
TEST_CASE_SKIP(complex, tuple_interface)
TEST_CASE_SKIP(complex, tuple_interface_chk)
#endif


TEST_CASE(complex, feature_macros_cxx20) {
#if defined(__cpp_lib_constexpr_complex)
    test_true( __cpp_lib_constexpr_complex >= 201711L );
    {
        const STD::complex<double> a(1.0, 2.0);
        const STD::complex<double> b = a + STD::complex<double>(3.0, 4.0);
        test_true( b.real() == 4.0 && b.imag() == 6.0 );
    }
    test_pass("cxx20:__cpp_lib_constexpr_complex");
    if (__cpp_lib_constexpr_complex >= 202306L) {
        test_pass("cxx26:__cpp_lib_constexpr_complex");
        test_pass("cxx26:more constexpr complex");
    } else {
        TEST_NOTE("the C++26 level of constexpr complex is not reached by this library");
        test_skip("cxx26:__cpp_lib_constexpr_complex");
        test_skip("cxx26:more constexpr complex");
    }
#else
    TEST_NOTE("__cpp_lib_constexpr_complex is not defined by this library");
    test_skip("cxx20:__cpp_lib_constexpr_complex");
    test_skip("cxx26:__cpp_lib_constexpr_complex");
    test_skip("cxx26:more constexpr complex");
#endif

#if defined(__cpp_lib_tuple_like)
    test_true( __cpp_lib_tuple_like >= 202207L );
    test_pass("cxx26:__cpp_lib_tuple_like");
#else
    TEST_NOTE("__cpp_lib_tuple_like is not defined by this library");
    test_skip("cxx26:__cpp_lib_tuple_like");
#endif
}

TEST_CASE(complex, arithmetic_overloads_cxx11) {
    test_true( STD::real(3.5) == 3.5 );
    test_true( STD::real(3.5f) == 3.5f );
    test_true( STD::real(3) == 3.0 );
    test_true( STD::real(STD::complex<double>(3.5, 1.0)) == 3.5 );
    test_pass("cxx11:real (arithmetic overload)");

    test_true( STD::imag(3.5) == 0.0 );
    test_true( STD::imag(3.5f) == 0.0f );
    test_true( STD::imag(3) == 0.0 );
    test_true( STD::imag(STD::complex<double>(3.5, 1.0)) == 1.0 );
    test_pass("cxx11:imag (arithmetic overload)");

    test_true( STD::norm(3.0) == 9.0 );
    test_true( STD::norm(-3.0) == 9.0 );
    test_true( STD::norm(0.0) == 0.0 );
    test_true( STD::norm(STD::complex<double>(3.0, 4.0)) == 25.0 );
    test_pass("cxx11:norm (arithmetic overload)");

    test_true( STD::arg(1.0) == 0.0 );
    test_true( STD::arg(-1.0) > 3.14 && STD::arg(-1.0) < 3.15 );
    test_true( STD::arg(STD::complex<double>(1.0, 0.0)) == 0.0 );
    test_pass("cxx11:arg (arithmetic overload)");
}
