/**
 *  @file   c_test.h
 *  @brief  Unit test (C language port of test.hpp).
 *  @author Masashi Kitamura (tenka@6809.net)
 *  @date   2020-2026
 *  @license Boost Software License Version 1.0
 *
 *  The C port of test.hpp. It uses neither templates nor exceptions,
 *  so the test machinery itself lives in c_test.c.
 *  Printing the values on failure (L=.. R=..) is given up:
 *  test_eq and friends simply expand to test_true((a) == (b)).
 *  The output format is the same as test.hpp's.
 *
 *  Usage
 *  -----
 *  In each test file (TEST_CASE's first argument, suite,
 *  only mirrors the C++ form and is unused in C):
 *      TEST_CASE(suiteName1, name1) { test_eq(foo(1), 0); test_true(x > 0); }
 *      TEST_CASE(suiteName1, name2) { ... }
 *
 *      TEST_SUITE(suiteName1)
 *          TEST_ADD(name1)
 *          TEST_ADD(name2)
 *      TEST_SUITE_END(suiteName1)
 *
 *  In that environment's test_main.c:
 *      TEST_DECL_BEGIN()
 *          TEST_DECL_SUITE(suiteName1)
 *          TEST_DECL_SUITE(suiteName2)
 *      TEST_DECL_END()
 *
 *      int main(void) { return TEST_RUN(); }
 */
#ifndef C_TEST_H_INCLUDED__
#define C_TEST_H_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TEST_SUITE_MAX
#define TEST_SUITE_MAX      512
#endif

#ifndef TEST_PUTS_BUF_SIZE
#define TEST_PUTS_BUF_SIZE  0x1000
#endif

#if defined(TEST_USE_MSW)
#include <windows.h>
#define TEST_PUTS(x)        OutputDebugStringA(x)
#else
#define TEST_PUTS(x)        fputs((x), stdout)
#endif

typedef void (*test_case_func_t)(void);

typedef struct test_case_t {
    char const*      name;
    test_case_func_t func;
} test_case_t;

#if defined(TEST_DISABLE)

#define TEST_CASE(suite, name)  static void name(void)
#define TEST_SUITE(T)       test_case_t _TeST_suiTe_cases_##T[] = {
#define TEST_ADD(f)         { #f, f },
#define TEST_SUITE_END(T)   { (char const*)0, (test_case_func_t)0 } };

#define TEST_DECL_BEGIN()   void _TeST_decl_begin(void) {
#define TEST_DECL_SUITE(T)  ((void)0);
#define TEST_DECL(T)        TEST_DECL_SUITE(T)
#define TEST_DECL_END()     }

#define TEST_RUN()          (0u)
#define TEST_PRINTF         (void)

#define test_true(c)        ((void)0)
#define test_eq(l,r)        ((void)0)
#define test_ne(l,r)        ((void)0)
#define test_lt(l,r)        ((void)0)
#define test_le(l,r)        ((void)0)
#define test_gt(l,r)        ((void)0)
#define test_ge(l,r)        ((void)0)
#define test_lim(x,a,b)     ((void)0)
#define test_ptr(p)         ((void)0)
#define test_ptr0(p)        ((void)0)
#define test_str_eq(a,b)    ((void)0)
#define test_str_ne(a,b)    ((void)0)
#define test_fail()         ((void)0)

#define TEST_SKIP1()        ((void)0)
#define TEST_SKIP_N(n)      ((void)0)
#define TEST_SKIP_CASE()    ((void)0)
#define TEST_SKIP_WAT(note)     ((void)sizeof(note))
#define TEST_SKIP_DOS(note)     ((void)sizeof(note))
#define TEST_SKIP_VC(note)      ((void)sizeof(note))
#define TEST_SKIP_GCC(note)     ((void)sizeof(note))
#define TEST_SKIP_PCC(note)     ((void)sizeof(note))
#define TEST_SKIP_OCC(note)     ((void)sizeof(note))
#define TEST_SKIP_VC080(note)   ((void)sizeof(note))
#define TEST_SKIP_VC090(note)   ((void)sizeof(note))
#define TEST_SKIP_VC100(note)   ((void)sizeof(note))
#define TEST_SKIP_VC110(note)   ((void)sizeof(note))
#define TEST_SKIP_VC120(note)   ((void)sizeof(note))
#define TEST_SKIP_VC140(note)   ((void)sizeof(note))
#define TEST_SKIP_VC141(note)   ((void)sizeof(note))
#define TEST_SKIP_VC142(note)   ((void)sizeof(note))
#define TEST_SKIP_VC143(note)   ((void)sizeof(note))
#define TEST_SKIP_VC145(note)   ((void)sizeof(note))
#define TEST_SKIP_UCRT(note)    ((void)sizeof(note))
#define test_skip(s)            ((void)sizeof(s))
#define test_pass(s)            ((void)sizeof(s))
#define TEST_NOTE(s)            ((void)sizeof(s))
#define TEST_SET_PASS_LOG_DIR(d) ((void)sizeof(d))
#define TEST_C90(x)             ((void)0)
#define TEST_C99(x)             ((void)0)
#define TEST_C11(x)             ((void)0)
#define TEST_C17(x)             ((void)0)
#define TEST_C23(x)             ((void)0)
#define TEST_CXX03(x)           ((void)0)
#define TEST_CXX11(x)           ((void)0)
#define TEST_CXX14(x)           ((void)0)
#define TEST_CXX17(x)           ((void)0)
#define TEST_CXX20(x)           ((void)0)
#define TEST_CXX23(x)           ((void)0)
#define TEST_CXX26(x)           ((void)0)

#else   /* ================================================================ */

#define TEST_CASE(suite, name)  static void name(void)
#define TEST_SUITE(T)       test_case_t _TeST_suiTe_cases_##T[] = {
#define TEST_ADD(f)         { #f, f },
#define TEST_SUITE_END(T)   { (char const*)0, (test_case_func_t)0 } };

#define TEST_DECL_BEGIN()   void _TeST_decl_begin(void) {
#define TEST_DECL_SUITE(T) \
    { extern test_case_t _TeST_suiTe_cases_##T[]; \
      _TeST_add(_TeST_suiTe_cases_##T, #T); }
#define TEST_DECL(T)        TEST_DECL_SUITE(T)
#define TEST_DECL_END()     }

#define TEST_RUN()          _TeST_run()
#define TEST_PRINTF         _TeST_printf

#define test_true(c)        _TeST_true(!!(c), #c, __FILE__, __LINE__)

#define test_eq(l,r)        test_true((l) == (r))
#define test_ne(l,r)        test_true((l) != (r))
#define test_lt(l,r)        test_true((l) <  (r))
#define test_le(l,r)        test_true((l) <= (r))
#define test_gt(l,r)        test_true((l) >  (r))
#define test_ge(l,r)        test_true((l) >= (r))
#define test_lim(x,a,b)     test_true((a) <= (x) && (x) <= (b))

#define test_ptr(p)         _TeST_ptr((void const*)(p), 0, #p, __FILE__, __LINE__)
#define test_ptr0(p)        _TeST_ptr((void const*)(p), 1, #p, __FILE__, __LINE__)

#define test_str_eq(a,b)    _TeST_str_cc((a), (b), 1, #a " == " #b, __FILE__, __LINE__)
#define test_str_ne(a,b)    _TeST_str_cc((a), (b), 0, #a " != " #b, __FILE__, __LINE__)

#define test_fail()         _TeST_fail(__FILE__, __LINE__)

#define TEST_SKIP1()        _TeST_setExprSkip()

#define TEST_SKIP_N(n)      do { unsigned _tsN; for (_tsN = 0; _tsN < (unsigned)(n); ++_tsN) TEST_SKIP1(); } while (0)
#define TEST_SKIP_CASE()    _TeST_setCaseSkip()

#define test_pass(s)              _TeST_pass(s)

#define test_skip(s)              do { _TeST_setExprSkip(); _TeST_pass(s); } while (0)

#define TEST_NOTE(s)              _TeST_note(s)

#define TEST_SET_PASS_LOG_DIR(d)  _TeST_set_pass_dir(d)

#if defined(__STDC_VERSION__)
# define _TeST_C_FROM_STD  (__STDC_VERSION__ / 100L)
#else
# define _TeST_C_FROM_STD  1990
#endif

#if defined(_CCW_TARGET_C)
# define _TeST_C_TGT  ((_CCW_TARGET_C) < 2011 ? 2011 : (_CCW_TARGET_C))
#endif
#ifndef TEST_TARGET_C
# if defined(_CCW_TARGET_C) && _TeST_C_TGT > _TeST_C_FROM_STD
#  define TEST_TARGET_C _TeST_C_TGT
# else
#  define TEST_TARGET_C _TeST_C_FROM_STD
# endif
#endif
#define _TeST_LANG_C(v, x)    do { if (TEST_TARGET_C < (v)) _TeST_setExprSkip(); x; } while (0)
#define TEST_C90(x)     do { x; } while (0)
#define TEST_C99(x)     _TeST_LANG_C(1999, x)
#define TEST_C11(x)     _TeST_LANG_C(2011, x)
#define TEST_C17(x)     _TeST_LANG_C(2017, x)
#define TEST_C23(x)     _TeST_LANG_C(2023, x)

#ifndef TEST_TARGET_CXX
# if defined(_CCW_TARGET_CXX)
#  define TEST_TARGET_CXX (_CCW_TARGET_CXX)
# else
#  define TEST_TARGET_CXX 0
# endif
#endif
#define _TeST_LANG_CXX(v, x) do { if ((TEST_TARGET_CXX) && (TEST_TARGET_CXX) < (v)) _TeST_setExprSkip(); x; } while (0)
#define TEST_CXX03(x)   _TeST_LANG_CXX(2003, x)
#define TEST_CXX11(x)   _TeST_LANG_CXX(2011, x)
#define TEST_CXX14(x)   _TeST_LANG_CXX(2014, x)
#define TEST_CXX17(x)   _TeST_LANG_CXX(2017, x)
#define TEST_CXX20(x)   _TeST_LANG_CXX(2020, x)
#define TEST_CXX23(x)   _TeST_LANG_CXX(2023, x)
#define TEST_CXX26(x)   _TeST_LANG_CXX(2026, x)

#if defined(__WATCOMC__)
# define TEST_SKIP_WAT(note)    do { _TeST_note(note); _TeST_setExprSkip(); } while (0)
#else
# define TEST_SKIP_WAT(note)    ((void)sizeof(note))
#endif
#if defined(__DOS__) || defined(MSDOS)
# define TEST_SKIP_DOS(note)    do { _TeST_note(note); _TeST_setExprSkip(); } while (0)
#else
# define TEST_SKIP_DOS(note)    ((void)sizeof(note))
#endif
#if defined(__GNUC__) || defined(__clang__)
# define TEST_SKIP_GCC(note)    do { _TeST_note(note); _TeST_setExprSkip(); } while (0)
#else
# define TEST_SKIP_GCC(note)    ((void)sizeof(note))
#endif
#if defined(__PCC__)
# define TEST_SKIP_PCC(note)    do { _TeST_note(note); _TeST_setExprSkip(); } while (0)
#else
# define TEST_SKIP_PCC(note)    ((void)sizeof(note))
#endif
#if defined(__ORANGEC__)
# define TEST_SKIP_OCC(note)    do { _TeST_note(note); _TeST_setExprSkip(); } while (0)
#else
# define TEST_SKIP_OCC(note)    ((void)sizeof(note))
#endif

#if defined(_MSC_VER)
# define TEST_SKIP_VC(note)     do { _TeST_note(note); _TeST_setExprSkip(); } while (0)
# define _TeST_SKIP_VC_LT(floor, note)  do { if (_MSC_VER < (floor)) { _TeST_note(note); _TeST_setExprSkip(); } } while (0)
#else
# define TEST_SKIP_VC(note)            ((void)sizeof(note))
# define _TeST_SKIP_VC_LT(floor, note) ((void)sizeof(note))
#endif
#define TEST_SKIP_VC080(note)   _TeST_SKIP_VC_LT(1500, note)
#define TEST_SKIP_VC090(note)   _TeST_SKIP_VC_LT(1600, note)
#define TEST_SKIP_VC100(note)   _TeST_SKIP_VC_LT(1700, note)
#define TEST_SKIP_VC110(note)   _TeST_SKIP_VC_LT(1800, note)
#define TEST_SKIP_VC120(note)   _TeST_SKIP_VC_LT(1900, note)
#define TEST_SKIP_VC140(note)   _TeST_SKIP_VC_LT(1910, note)
#define TEST_SKIP_VC141(note)   _TeST_SKIP_VC_LT(1920, note)
#define TEST_SKIP_VC142(note)   _TeST_SKIP_VC_LT(1930, note)
#define TEST_SKIP_VC143(note)   _TeST_SKIP_VC_LT(1950, note)
#define TEST_SKIP_VC145(note)   _TeST_SKIP_VC_LT(9999, note)

#if defined(_MSC_VER) || defined(_UCRT) || defined(__MINGW32__)
# define TEST_SKIP_UCRT(note)   do { _TeST_note(note); _TeST_setExprSkip(); } while (0)
#else
# define TEST_SKIP_UCRT(note)   ((void)sizeof(note))
#endif

#endif  /* TEST_DISABLE */

void     _TeST_add(test_case_t* cases, char const* suite_name);
unsigned _TeST_run(void);

void     _TeST_true(int cc, char const* expr, char const* file, int line);
void     _TeST_ptr(void const* p, int zero_ok, char const* expr, char const* file, int line);
void     _TeST_str_cc(char const* a, char const* b, int eq, char const* expr,
                      char const* file, int line);
void     _TeST_fail(char const* file, int line);

void     _TeST_setExprSkip(void);
void     _TeST_setCaseSkip(void);

void        _TeST_pass(char const* pass_str);
void        _TeST_note(char const* note);
void        _TeST_set_pass_dir(char const* dir);

void     _TeST_printf(char const* fmt, ...);

#ifdef __cplusplus
}
#endif

#endif  /* C_TEST_H_INCLUDED__ */
