#include "c_test.h"

#if defined(__WATCOMC__) && (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L)
# define TST_FENV_UNAVAILABLE 1
#else
# include <fenv.h>
#endif

TEST_CASE(fenv, exception_macros) {
#ifdef TST_FENV_UNAVAILABLE
    TEST_SKIP_CASE();
#else

    int all = FE_ALL_EXCEPT;
    int seen = 0;
#ifdef FE_DIVBYZERO
    test_ne( FE_DIVBYZERO, 0 );
    test_eq( FE_DIVBYZERO & all, FE_DIVBYZERO );
    seen |= FE_DIVBYZERO;
    test_pass("c1999:opt:FE_DIVBYZERO");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_NOTE("FE_DIVBYZERO not defined");
    test_skip("c1999:opt:FE_DIVBYZERO");
#endif
#ifdef FE_INEXACT
    test_eq( FE_INEXACT & all, FE_INEXACT );
    seen |= FE_INEXACT;
    test_pass("c1999:opt:FE_INEXACT");
#else
    TEST_SKIP1(); TEST_NOTE("FE_INEXACT not defined");
    test_skip("c1999:opt:FE_INEXACT");
#endif
#ifdef FE_INVALID
    test_eq( FE_INVALID & all, FE_INVALID );
    seen |= FE_INVALID;
    test_pass("c1999:opt:FE_INVALID");
#else
    TEST_SKIP1(); TEST_NOTE("FE_INVALID not defined");
    test_skip("c1999:opt:FE_INVALID");
#endif
#ifdef FE_OVERFLOW
    test_eq( FE_OVERFLOW & all, FE_OVERFLOW );
    seen |= FE_OVERFLOW;
    test_pass("c1999:opt:FE_OVERFLOW");
#else
    TEST_SKIP1(); TEST_NOTE("FE_OVERFLOW not defined");
    test_skip("c1999:opt:FE_OVERFLOW");
#endif
#ifdef FE_UNDERFLOW
    test_eq( FE_UNDERFLOW & all, FE_UNDERFLOW );
    seen |= FE_UNDERFLOW;
    test_pass("c1999:opt:FE_UNDERFLOW");
#else
    TEST_SKIP1(); TEST_NOTE("FE_UNDERFLOW not defined");
    test_skip("c1999:opt:FE_UNDERFLOW");
#endif
    test_eq( seen, all );
    test_ne( all, 0 );
    test_pass("c1999:FE_ALL_EXCEPT");
#endif
}

TEST_CASE(fenv, rounding_modes) {
#ifdef TST_FENV_UNAVAILABLE
    TEST_SKIP_CASE();
#else
    int saved = fegetround();
    test_ge( saved, 0 );
    test_pass("c1999:fegetround");
    fesetround(saved);
    test_eq( fegetround(), saved );
    test_pass("c1999:fesetround");

#ifdef FE_TONEAREST
    test_eq( fesetround(FE_TONEAREST), 0 );
    test_eq( fegetround(), FE_TONEAREST );
    test_pass("c1999:opt:FE_TONEAREST");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_NOTE("FE_TONEAREST not defined");
    test_skip("c1999:opt:FE_TONEAREST");
#endif
#ifdef FE_TOWARDZERO
    test_eq( fesetround(FE_TOWARDZERO), 0 );
    test_eq( fegetround(), FE_TOWARDZERO );
    test_pass("c1999:opt:FE_TOWARDZERO");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_NOTE("FE_TOWARDZERO not defined");
    test_skip("c1999:opt:FE_TOWARDZERO");
#endif
#ifdef FE_UPWARD
    test_eq( fesetround(FE_UPWARD), 0 );
    test_eq( fegetround(), FE_UPWARD );
    test_pass("c1999:opt:FE_UPWARD");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_NOTE("FE_UPWARD not defined");
    test_skip("c1999:opt:FE_UPWARD");
#endif
#ifdef FE_DOWNWARD
    test_eq( fesetround(FE_DOWNWARD), 0 );
    test_eq( fegetround(), FE_DOWNWARD );
    test_pass("c1999:opt:FE_DOWNWARD");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_NOTE("FE_DOWNWARD not defined");
    test_skip("c1999:opt:FE_DOWNWARD");
#endif
    fesetround(saved);
#endif
}

TEST_CASE(fenv, exception_flags) {
#ifdef TST_FENV_UNAVAILABLE
    TEST_SKIP_CASE();
#else
    fexcept_t saved;

    test_gt( (int)sizeof(fexcept_t), 0 );
    test_pass("c1999:fexcept_t");
    test_eq( fegetexceptflag(&saved, FE_ALL_EXCEPT), 0 );
    test_pass("c1999:fegetexceptflag");
    test_eq( feclearexcept(FE_ALL_EXCEPT), 0 );
    test_pass("c1999:feclearexcept");
    test_eq( fetestexcept(FE_ALL_EXCEPT), 0 );
    test_pass("c1999:fetestexcept");

#ifdef FE_INVALID
    test_eq( feraiseexcept(FE_INVALID), 0 );
    test_eq( fetestexcept(FE_INVALID), FE_INVALID );
    feclearexcept(FE_INVALID);
    test_pass("c1999:feraiseexcept");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_NOTE("FE_INVALID not defined");
    test_skip("c1999:feraiseexcept");
#endif

    feclearexcept(FE_ALL_EXCEPT);
    test_eq( fesetexceptflag(&saved, FE_ALL_EXCEPT), 0 );
    feclearexcept(FE_ALL_EXCEPT);
    test_pass("c1999:fesetexceptflag");
#endif
}

TEST_CASE(fenv, whole_environment) {
#ifdef TST_FENV_UNAVAILABLE
    TEST_SKIP_CASE();
#else
    fenv_t env;

    test_gt( (int)sizeof(fenv_t), 0 );
    test_pass("c1999:fenv_t");
    test_eq( fegetenv(&env), 0 );
    test_pass("c1999:fegetenv");
    test_eq( fesetenv(&env), 0 );
    test_pass("c1999:fesetenv");

    test_eq( feholdexcept(&env), 0 );
    test_eq( fetestexcept(FE_ALL_EXCEPT), 0 );
    test_pass("c1999:feholdexcept");
    test_eq( feupdateenv(&env), 0 );
    test_pass("c1999:feupdateenv");

    test_eq( fesetenv(FE_DFL_ENV), 0 );
    feclearexcept(FE_ALL_EXCEPT);
    test_pass("c1999:FE_DFL_ENV");
#endif
}


TEST_CASE(fenv, rounding_and_pragmas) {
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
    {
#pragma STDC FENV_ACCESS ON
        test_true( 1 );
    }
    {
#pragma STDC FENV_ACCESS DEFAULT
        test_true( 1 );
    }
    test_pass("c1999:#pragma STDC FENV_ACCESS ON/OFF/DEFAULT");
#else
    TEST_NOTE("the STDC pragmas need C99");
    test_skip("c1999:#pragma STDC FENV_ACCESS ON/OFF/DEFAULT");
#endif

#if !defined(TST_FENV_UNAVAILABLE)
    {
        int saved = fegetround();
        int ok_down = 0, ok_up = 0;
        volatile double x = 1.5, y = 2.5;
        (void)x; (void)y;
        if (fesetround(FE_DOWNWARD) == 0) {
            volatile double d = x + 0.25;
            ok_down = (fegetround() == FE_DOWNWARD) && (d <= 1.75);
        }
        if (fesetround(FE_UPWARD) == 0) {
            volatile double u = y + 0.25;
            ok_up = (fegetround() == FE_UPWARD) && (u >= 2.75);
        }
        fesetround(saved);
        test_true( fegetround() == saved );
        test_true( ok_down || ok_up );
    }
    test_pass("c1999:fesetround/fegetround rounding-mode switch");
#else
    TEST_SKIP_N(2);
    test_skip("c1999:fesetround/fegetround rounding-mode switch");
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L && defined(FE_TONEAREST)
    test_true( 1 );
    test_pass("c2023:#pragma STDC FENV_ROUND direction");
#else
    TEST_NOTE("FENV_ROUND is C23");
    test_skip("c2023:#pragma STDC FENV_ROUND direction");
#endif
}

TEST_CASE(fenv, c23_modes_and_dfp) {
#if defined(__STDC_VERSION_FENV_H__)
    test_true( __STDC_VERSION_FENV_H__ >= 202311L );
    test_pass("c2023:__STDC_VERSION_FENV_H__");
#else
    TEST_NOTE("C23 header version macro not defined by this library");
    test_skip("c2023:__STDC_VERSION_FENV_H__");
#endif

#if defined(FE_DFL_MODE)
    {
        femode_t m;
        int saved = fegetround();
        test_eq( fegetmode(&m), 0 );
        if (fesetround(FE_TOWARDZERO) == 0) {
            test_true( fegetround() == FE_TOWARDZERO );
        }
        test_eq( fesetmode(&m), 0 );
        test_true( fegetround() == saved );
        test_eq( fesetmode(FE_DFL_MODE), 0 );
        fesetround(saved);
    }
    test_pass("c2023:femode_t");
    test_pass("c2023:fegetmode");
    test_pass("c2023:fesetmode");
    test_pass("c2023:FE_DFL_MODE");
    test_pass("c2023:fegetmode/fesetmode whole dynamic-mode save & restore");
#else
    TEST_NOTE("the C23 dynamic-mode functions are not provided by this library");
    test_skip("c2023:femode_t");
    test_skip("c2023:fegetmode");
    test_skip("c2023:fesetmode");
    test_skip("c2023:FE_DFL_MODE");
    test_skip("c2023:fegetmode/fesetmode whole dynamic-mode save & restore");
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L && defined(FE_INVALID)
    {
        fenv_t saved_env;
        feholdexcept(&saved_env);
        feclearexcept(FE_ALL_EXCEPT);
        if (fesetexcept(FE_INVALID) == 0) {
            fexcept_t f;
            test_true( fetestexcept(FE_INVALID) != 0 );
            fegetexceptflag(&f, FE_ALL_EXCEPT);
            test_true( fetestexceptflag(&f, FE_INVALID) != 0 );
        } else {
            TEST_SKIP_N(2);
        }
        feclearexcept(FE_ALL_EXCEPT);
        fesetenv(FE_DFL_ENV);
    }
    test_pass("c2023:fesetexcept");
    test_pass("c2023:fetestexceptflag");
#else
    TEST_NOTE("fesetexcept / fetestexceptflag are C23");
    test_skip("c2023:fesetexcept");
    test_skip("c2023:fetestexceptflag");
#endif

#if defined(FE_DEC_TONEAREST)
    test_true( FE_DEC_TONEAREST != FE_DEC_TOWARDZERO );
    test_pass("c2023:opt:FE_DEC_TONEAREST");
    test_true( FE_DEC_UPWARD != FE_DEC_DOWNWARD );
    test_pass("c2023:opt:FE_DEC_UPWARD");
    test_pass("c2023:opt:FE_DEC_DOWNWARD");
    test_pass("c2023:opt:FE_DEC_TOWARDZERO");
#if defined(FE_DEC_TONEARESTFROMZERO)
    test_true( FE_DEC_TONEARESTFROMZERO != FE_DEC_TONEAREST );
    test_pass("c2023:opt:FE_DEC_TONEARESTFROMZERO");
#else
    test_skip("c2023:opt:FE_DEC_TONEARESTFROMZERO");
#endif
    {
        int saved = fe_dec_getround();
        test_eq( fe_dec_setround(FE_DEC_TOWARDZERO), 0 );
        test_true( fe_dec_getround() == FE_DEC_TOWARDZERO );
        fe_dec_setround(saved);
    }
    test_pass("c2023:opt:fe_dec_getround");
    test_pass("c2023:opt:fe_dec_setround");
#else
    TEST_NOTE("decimal floating point is optional and absent here");
    test_skip("c2023:opt:FE_DEC_TONEAREST");
    test_skip("c2023:opt:FE_DEC_UPWARD");
    test_skip("c2023:opt:FE_DEC_DOWNWARD");
    test_skip("c2023:opt:FE_DEC_TOWARDZERO");
    test_skip("c2023:opt:FE_DEC_TONEARESTFROMZERO");
    test_skip("c2023:opt:fe_dec_getround");
    test_skip("c2023:opt:fe_dec_setround");
#endif

#if defined(FE_TONEARESTFROMZERO)
    test_true( FE_TONEARESTFROMZERO != FE_TONEAREST );
    test_pass("c2023:opt:FE_TONEARESTFROMZERO");
#else
    TEST_NOTE("the round-to-nearest-from-zero direction is optional");
    test_skip("c2023:opt:FE_TONEARESTFROMZERO");
#endif

#if defined(FE_SNANS_ALWAYS_SIGNAL)
    test_true( FE_SNANS_ALWAYS_SIGNAL == 1 );
    test_pass("c2023:opt:FE_SNANS_ALWAYS_SIGNAL");
#else
    TEST_NOTE("FE_SNANS_ALWAYS_SIGNAL is a recommended practice, not a requirement");
    test_skip("c2023:opt:FE_SNANS_ALWAYS_SIGNAL");
#endif
}

TEST_SUITE(fenv)
    TEST_ADD(exception_macros)
    TEST_ADD(rounding_modes)
    TEST_ADD(exception_flags)
    TEST_ADD(whole_environment)
    TEST_ADD(rounding_and_pragmas)
    TEST_ADD(c23_modes_and_dfp)
TEST_SUITE_END(fenv)
