#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2020
#include <numbers>
#include <cmath>

namespace {
inline bool near_eq(double a, double b) { double d = a - b; return (d < 0 ? -d : d) < 1e-12; }
}

TEST_CASE(numbers, constants) {
    test_true( near_eq(STD::numbers::e,          2.718281828459045) );
    test_pass("cxx20:numbers::e");
    test_true( near_eq(STD::numbers::log2e,      1.4426950408889634) );
    test_pass("cxx20:numbers::log2e");
    test_true( near_eq(STD::numbers::log10e,     0.4342944819032518) );
    test_pass("cxx20:numbers::log10e");
    test_true( near_eq(STD::numbers::pi,         3.141592653589793) );
    test_pass("cxx20:numbers::pi");
    test_true( near_eq(STD::numbers::inv_pi,     0.3183098861837907) );
    test_pass("cxx20:numbers::inv_pi");
    test_true( near_eq(STD::numbers::inv_sqrtpi, 0.5641895835477563) );
    test_pass("cxx20:numbers::inv_sqrtpi");
    test_true( near_eq(STD::numbers::ln2,        0.6931471805599453) );
    test_pass("cxx20:numbers::ln2");
    test_true( near_eq(STD::numbers::ln10,       2.302585092994046) );
    test_pass("cxx20:numbers::ln10");
    test_true( near_eq(STD::numbers::sqrt2,      1.4142135623730951) );
    test_pass("cxx20:numbers::sqrt2");
    test_true( near_eq(STD::numbers::sqrt3,      1.7320508075688772) );
    test_pass("cxx20:numbers::sqrt3");
    test_true( near_eq(STD::numbers::inv_sqrt3,  0.5773502691896257) );
    test_pass("cxx20:numbers::inv_sqrt3");
    test_true( near_eq(STD::numbers::egamma,     0.5772156649015329) );
    test_pass("cxx20:numbers::egamma");
    test_true( near_eq(STD::numbers::phi,        1.618033988749895) );
    test_pass("cxx20:numbers::phi");
}

#define _TST_NUM_V(name, expect)                                                    \
    do {                                                                              \
        bool ok = STD::numbers::name##_v<float> == (float)(expect)                    \
               && STD::numbers::name##_v<double> == (expect)                          \
               && near_eq((double)STD::numbers::name##_v<long double>, (expect));     \
        test_true( ok );                                                              \
        bool same = STD::numbers::name == STD::numbers::name##_v<double>;             \
        test_true( same );                                                            \
        test_pass("cxx20:numbers::" #name "_v<T>");                                   \
    } while (0)

#if _TST_HAS_VAR_TEMPLATE
TEST_CASE(numbers, constant_templates) {
    _TST_NUM_V(e,          2.718281828459045);
    _TST_NUM_V(log2e,      1.4426950408889634);
    _TST_NUM_V(log10e,     0.4342944819032518);
    _TST_NUM_V(pi,         3.141592653589793);
    _TST_NUM_V(inv_pi,     0.3183098861837907);
    _TST_NUM_V(inv_sqrtpi, 0.5641895835477563);
    _TST_NUM_V(ln2,        0.6931471805599453);
    _TST_NUM_V(ln10,       2.302585092994046);
    _TST_NUM_V(sqrt2,      1.4142135623730951);
    _TST_NUM_V(sqrt3,      1.7320508075688772);
    _TST_NUM_V(inv_sqrt3,  0.5773502691896257);
    _TST_NUM_V(egamma,     0.5772156649015329);
    _TST_NUM_V(phi,        1.618033988749895);
}
#else
TEST_CASE(numbers, constant_templates) {
    TEST_NOTE("the _v forms are variable templates; C++03 has none");
    test_skip("cxx20:numbers::e_v<T>");
    test_skip("cxx20:numbers::log2e_v<T>");
    test_skip("cxx20:numbers::log10e_v<T>");
    test_skip("cxx20:numbers::pi_v<T>");
    test_skip("cxx20:numbers::inv_pi_v<T>");
    test_skip("cxx20:numbers::inv_sqrtpi_v<T>");
    test_skip("cxx20:numbers::ln2_v<T>");
    test_skip("cxx20:numbers::ln10_v<T>");
    test_skip("cxx20:numbers::sqrt2_v<T>");
    test_skip("cxx20:numbers::sqrt3_v<T>");
    test_skip("cxx20:numbers::inv_sqrt3_v<T>");
    test_skip("cxx20:numbers::egamma_v<T>");
    test_skip("cxx20:numbers::phi_v<T>");
}
#endif

TEST_CASE(numbers, identities) {
    test_true( near_eq(STD::numbers::pi / 4.0, STD::atan(1.0)) );
    test_true( near_eq(STD::numbers::e, STD::exp(1.0)) );
    test_true( near_eq(STD::numbers::sqrt2 * STD::numbers::sqrt2, 2.0) );
    test_true( near_eq(STD::numbers::ln2, STD::log(2.0)) );
    test_true( near_eq(STD::numbers::inv_pi * STD::numbers::pi, 1.0) );
    test_pass("cxx20:numeric constant identities");
}
#endif

TEST_CASE(numbers, feature_test_macros) {
#if defined(__cpp_lib_math_constants) && (__cpp_lib_math_constants) >= 201907L
    test_true( (__cpp_lib_math_constants) >= 201907L );
    test_pass("cxx20:__cpp_lib_math_constants=201907L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx20:__cpp_lib_math_constants=201907L");
#endif
}
