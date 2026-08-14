#include "test_cxx.hpp"

TEST_CASE_SKIP(ccomplex, unimplemented)

#if TEST_TARGET_CXX >= 2011
#include <ccomplex>

TEST_CASE(ccomplex, includes_complex) {

    STD::complex<double> a(3.0, 4.0);
    STD::complex<double> b(1.0, -2.0);

    test_eq( a.real(), 3.0 );
    test_eq( a.imag(), 4.0 );
    test_eq( STD::real(a), 3.0 );
    test_eq( STD::imag(a), 4.0 );
    test_eq( STD::abs(a), 5.0 );
    test_eq( STD::norm(a), 25.0 );

    STD::complex<double> c = a + b;
    test_eq( c.real(), 4.0 );
    test_eq( c.imag(), 2.0 );

    STD::complex<double> d = STD::conj(a);
    test_eq( d.real(), 3.0 );
    test_eq( d.imag(), -4.0 );

    STD::complex<float>       f(1.0f, 2.0f);
    STD::complex<long double> l(1.0L, 0.0L);
    test_eq( f.real(), 1.0f );
    test_eq( (double)l.real(), 1.0 );

    STD::complex<double> m = a * STD::complex<double>(0.0, 1.0);
    test_eq( m.real(), -4.0 );
    test_eq( m.imag(), 3.0 );
    test_pass("cxx11:<ccomplex> includes <complex>");
}
#else
TEST_CASE_SKIP(ccomplex, includes_complex)
#endif
