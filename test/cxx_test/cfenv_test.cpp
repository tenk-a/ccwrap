#include "test_cxx.hpp"
#include <cfenv>

TEST_CASE(cfenv, header_compiles) { test_true( true );  test_pass("cxx11:<cfenv> header compiles"); }

#if TEST_TARGET_CXX >= 2011
#include <cfenv>
#include <type_traits>

#pragma fenv_access(on)

TEST_CASE(cfenv, types) {

    STD_NS fenv_t    env;
    test_eq( STD_NS fegetenv(&env), 0 );
    test_true( sizeof(env)   > 0 );
#if !defined(__WATCOMC__)
    test_true( STD::is_trivially_copyable<STD_NS fenv_t>::value );
#else
    TEST_SKIP1();
#endif

    STD_NS fenv_t   env2  = env;
    (void)env2;
    test_pass("cxx11:fenv_t");

    STD_NS fexcept_t flags;
    test_eq( STD_NS fegetexceptflag(&flags, FE_ALL_EXCEPT), 0 );
    test_true( sizeof(flags) > 0 );
#if !defined(__WATCOMC__)
    test_true( STD::is_trivially_copyable<STD_NS fexcept_t>::value );
#else
    TEST_SKIP1();
#endif

    STD_NS fexcept_t flags2 = flags;
    (void)flags2;
    test_pass("cxx11:fexcept_t");
}

TEST_CASE(cfenv, exception_macros) {

    const int all = FE_DIVBYZERO | FE_INEXACT | FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW;

    test_ne( FE_DIVBYZERO, 0 );
    test_eq( FE_DIVBYZERO & FE_INVALID,  0 );
    test_pass("cxx11:opt:FE_DIVBYZERO");

    test_ne( FE_INEXACT,   0 );
    test_eq( FE_INEXACT   & FE_INVALID,  0 );
    test_pass("cxx11:opt:FE_INEXACT");

    test_ne( FE_INVALID,   0 );
    test_pass("cxx11:opt:FE_INVALID");

    test_ne( FE_OVERFLOW,  0 );
    test_eq( FE_OVERFLOW  & FE_UNDERFLOW, 0 );
    test_pass("cxx11:opt:FE_OVERFLOW");

    test_ne( FE_UNDERFLOW, 0 );
    test_pass("cxx11:opt:FE_UNDERFLOW");

    test_eq( FE_ALL_EXCEPT & all, all );
    test_eq( FE_ALL_EXCEPT | all, FE_ALL_EXCEPT );
    test_pass("cxx11:FE_ALL_EXCEPT");
}

TEST_CASE(cfenv, clear_raise_test) {
    STD_NS fenv_t saved;
    test_eq( STD_NS fegetenv(&saved), 0 );

    test_eq( STD_NS feclearexcept(FE_ALL_EXCEPT), 0 );
    test_eq( STD_NS fetestexcept(FE_ALL_EXCEPT), 0 );
    test_pass("cxx11:feclearexcept");

    test_eq( STD_NS feraiseexcept(FE_INVALID), 0 );
    test_ne( STD_NS fetestexcept(FE_INVALID), 0 );
    test_eq( STD_NS fetestexcept(FE_DIVBYZERO), 0 );

    test_eq( STD_NS feraiseexcept(FE_DIVBYZERO), 0 );
    test_ne( STD_NS fetestexcept(FE_INVALID),   0 );
    test_ne( STD_NS fetestexcept(FE_DIVBYZERO), 0 );
    test_ne( STD_NS fetestexcept(FE_INVALID | FE_DIVBYZERO), 0 );
    test_pass("cxx11:feraiseexcept");

    test_eq( STD_NS feclearexcept(FE_INVALID), 0 );
    test_eq( STD_NS fetestexcept(FE_INVALID),   0 );
    test_ne( STD_NS fetestexcept(FE_DIVBYZERO), 0 );

    test_eq( STD_NS feclearexcept(0), 0 );
    test_eq( STD_NS feraiseexcept(0), 0 );
    test_eq( STD_NS fetestexcept(0),  0 );

    test_eq( STD_NS feclearexcept(FE_ALL_EXCEPT), 0 );
    test_eq( STD_NS fesetenv(&saved), 0 );
    test_pass("cxx11:fetestexcept");
}

TEST_CASE(cfenv, exceptflag_get_set) {
    STD_NS fenv_t saved;
    test_eq( STD_NS fegetenv(&saved), 0 );

    test_eq( STD_NS feclearexcept(FE_ALL_EXCEPT), 0 );
    test_eq( STD_NS feraiseexcept(FE_OVERFLOW | FE_INEXACT), 0 );

    STD_NS fexcept_t flags;
    test_eq( STD_NS fegetexceptflag(&flags, FE_ALL_EXCEPT), 0 );

    test_eq( STD_NS feclearexcept(FE_ALL_EXCEPT), 0 );
    test_eq( STD_NS fetestexcept(FE_ALL_EXCEPT), 0 );
    test_pass("cxx11:fegetexceptflag");

    test_eq( STD_NS fesetexceptflag(&flags, FE_ALL_EXCEPT), 0 );
    test_ne( STD_NS fetestexcept(FE_OVERFLOW), 0 );
    test_ne( STD_NS fetestexcept(FE_INEXACT),  0 );
    test_eq( STD_NS fetestexcept(FE_INVALID),  0 );

    test_eq( STD_NS feclearexcept(FE_ALL_EXCEPT), 0 );
    test_eq( STD_NS fesetexceptflag(&flags, FE_OVERFLOW), 0 );
    test_ne( STD_NS fetestexcept(FE_OVERFLOW), 0 );

    test_eq( STD_NS feclearexcept(FE_ALL_EXCEPT), 0 );
    test_eq( STD_NS fesetenv(&saved), 0 );
    test_pass("cxx11:fesetexceptflag");
}

TEST_CASE(cfenv, rounding_modes) {

    test_ge( FE_TONEAREST,  0 );
    test_pass("cxx11:opt:FE_TONEAREST");
    test_ge( FE_DOWNWARD,   0 );
    test_pass("cxx11:opt:FE_DOWNWARD");
    test_ge( FE_UPWARD,     0 );
    test_pass("cxx11:opt:FE_UPWARD");
    test_ge( FE_TOWARDZERO, 0 );
    test_pass("cxx11:opt:FE_TOWARDZERO");

    test_ne( FE_TONEAREST, FE_DOWNWARD );
    test_ne( FE_TONEAREST, FE_UPWARD );
    test_ne( FE_TONEAREST, FE_TOWARDZERO );
    test_ne( FE_DOWNWARD,  FE_UPWARD );
    test_ne( FE_DOWNWARD,  FE_TOWARDZERO );
    test_ne( FE_UPWARD,    FE_TOWARDZERO );
    test_pass("cxx11:rounding direction macros are distinct");

    const int saved_round = STD_NS fegetround();
    test_ge( saved_round, 0 );
    test_pass("cxx11:fegetround");

    test_eq( STD_NS fesetround(FE_TONEAREST),  0 );
    test_eq( STD_NS fegetround(), FE_TONEAREST );

    test_eq( STD_NS fesetround(FE_DOWNWARD),   0 );
    test_eq( STD_NS fegetround(), FE_DOWNWARD );

    test_eq( STD_NS fesetround(FE_UPWARD),     0 );
    test_eq( STD_NS fegetround(), FE_UPWARD );

    test_eq( STD_NS fesetround(FE_TOWARDZERO), 0 );
    test_eq( STD_NS fegetround(), FE_TOWARDZERO );

    test_eq( STD_NS fesetround(saved_round), 0 );
    test_eq( STD_NS fegetround(), saved_round );
    test_pass("cxx11:fesetround");
}

TEST_CASE(cfenv, rounding_is_observable) {
    const int saved_round = STD_NS fegetround();

    volatile double one   = 1.0;
    volatile double small = 1.0 / (1 << 30) / (1 << 30);

    test_eq( STD_NS fesetround(FE_UPWARD), 0 );
    volatile double up = one + small;
    test_gt( (double)up, 1.0 );

    test_eq( STD_NS fesetround(FE_DOWNWARD), 0 );
    volatile double down = one + small;
    test_eq( (double)down, 1.0 );

    test_eq( STD_NS fesetround(FE_TOWARDZERO), 0 );
    volatile double zero = one + small;
    test_eq( (double)zero, 1.0 );

    test_eq( STD_NS fesetround(FE_TONEAREST), 0 );
    volatile double near_ = one + small;
    test_eq( (double)near_, 1.0 );

    test_eq( STD_NS fesetround(saved_round), 0 );
    test_eq( STD_NS feclearexcept(FE_ALL_EXCEPT), 0 );
    test_pass("cxx11:rounding mode is observable");
}

TEST_CASE(cfenv, env_get_set) {
    STD_NS fenv_t saved;
    test_eq( STD_NS fegetenv(&saved), 0 );

    const int saved_round = STD_NS fegetround();

    test_eq( STD_NS fesetround(FE_TONEAREST), 0 );
    test_eq( STD_NS feclearexcept(FE_ALL_EXCEPT), 0 );

    STD_NS fenv_t nearest;
    test_eq( STD_NS fegetenv(&nearest), 0 );
    test_pass("cxx11:fegetenv");

    test_eq( STD_NS fesetround(FE_UPWARD), 0 );
    test_eq( STD_NS feraiseexcept(FE_INVALID), 0 );

    test_eq( STD_NS fesetenv(&nearest), 0 );
    test_eq( STD_NS fegetround(), FE_TONEAREST );
    test_eq( STD_NS fetestexcept(FE_ALL_EXCEPT), 0 );
    test_pass("cxx11:fesetenv");

    test_eq( STD_NS fesetenv(FE_DFL_ENV), 0 );
    test_eq( STD_NS fetestexcept(FE_ALL_EXCEPT), 0 );
    test_eq( STD_NS feupdateenv(FE_DFL_ENV), 0 );

    test_eq( STD_NS fesetenv(&saved), 0 );
    test_eq( STD_NS fesetround(saved_round), 0 );
    test_eq( STD_NS feclearexcept(FE_ALL_EXCEPT), 0 );
    test_pass("cxx11:FE_DFL_ENV");
}

TEST_CASE(cfenv, holdexcept_updateenv) {
    STD_NS fenv_t saved;
    test_eq( STD_NS fegetenv(&saved), 0 );

    test_eq( STD_NS feclearexcept(FE_ALL_EXCEPT), 0 );

    STD_NS fenv_t held;
    test_eq( STD_NS feholdexcept(&held), 0 );
    test_eq( STD_NS fetestexcept(FE_ALL_EXCEPT), 0 );

    test_eq( STD_NS feraiseexcept(FE_OVERFLOW), 0 );
    test_eq( STD_NS feraiseexcept(FE_DIVBYZERO), 0 );
    test_ne( STD_NS fetestexcept(FE_OVERFLOW),  0 );
    test_ne( STD_NS fetestexcept(FE_DIVBYZERO), 0 );
    test_pass("cxx11:feholdexcept");

    test_eq( STD_NS feupdateenv(&held), 0 );
    test_ne( STD_NS fetestexcept(FE_DIVBYZERO), 0 );
    test_ne( STD_NS fetestexcept(FE_OVERFLOW),  0 );

    test_eq( STD_NS feclearexcept(FE_ALL_EXCEPT), 0 );
    test_eq( STD_NS fesetenv(&saved), 0 );
    test_eq( STD_NS feclearexcept(FE_ALL_EXCEPT), 0 );
    test_pass("cxx11:feupdateenv");
}
#else
TEST_CASE_SKIP(cfenv, types)
TEST_CASE_SKIP(cfenv, exception_macros)
TEST_CASE_SKIP(cfenv, clear_raise_test)
TEST_CASE_SKIP(cfenv, exceptflag_get_set)
TEST_CASE_SKIP(cfenv, rounding_modes)
TEST_CASE_SKIP(cfenv, rounding_is_observable)
TEST_CASE_SKIP(cfenv, env_get_set)
TEST_CASE_SKIP(cfenv, holdexcept_updateenv)
#endif
