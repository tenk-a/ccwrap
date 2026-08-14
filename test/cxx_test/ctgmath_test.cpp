#include "test_cxx.hpp"

#include <ctgmath>

TEST_CASE(ctgmath, provides_cmath_and_complex) {

    test_true( STD::sqrt(4.0) == 2.0 );
    test_true( STD::fabs(-3.0) == 3.0 );

    STD::complex<double> z(3.0, 4.0);
    test_true( STD::abs(z) == 5.0 );
    test_true( z.real() == 3.0 && z.imag() == 4.0 );
    test_pass("cxx11:<ctgmath> provides <cmath> and <complex>");
}

#if TEST_TARGET_CXX >= 2011
#include <ctgmath>

TEST_CASE(ctgmath, cmath_declarations) {

    test_true( STD::sqrt(4.0) == 2.0 );
    test_true( STD::fabs(-3.0) == 3.0 );
    test_true( STD::pow(2.0, 10.0) == 1024.0 );

    float  f = STD::sqrt(9.0f);
    double d = STD::sqrt(9.0);
    long double l = STD::sqrt(9.0L);
    test_true( f == 3.0f );
    test_true( d == 3.0 );
    test_true( l == 3.0L );
    test_true( STD::sqrt(16) == 4.0 );
    test_pass("cxx11:cmath declarations via <ctgmath>");
}

TEST_CASE(ctgmath, complex_declarations) {

    STD::complex<double> c(3.0, 4.0);
    test_true( c.real() == 3.0 );
    test_true( c.imag() == 4.0 );
    test_true( STD::abs(c) == 5.0 );

    STD::complex<double> s = STD::sqrt(STD::complex<double>(-1.0, 0.0));
    test_true( s.real() < 1e-12 );
    test_true( s.imag() > 0.999 && s.imag() < 1.001 );
    test_pass("cxx11:complex declarations via <ctgmath>");
}
#else
TEST_CASE_SKIP(ctgmath, cmath_declarations)
TEST_CASE_SKIP(ctgmath, complex_declarations)
#endif
