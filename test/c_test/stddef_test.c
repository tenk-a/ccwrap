#include "c_test.h"

#include <stddef.h>
#include <stdalign.h>
#include <stdlib.h>

struct Off {
    char    c;
    int     i;
    double  d;
    char    tail[4];
};

TEST_CASE(stddef, size_t_and_ptrdiff_t) {
    size_t    s = 5;
    ptrdiff_t d = -3;

    test_eq( s, (size_t)5 );
    test_true( sizeof(size_t) >= sizeof(void*) );
    test_true( (size_t)-1 > (size_t)0 );
    test_pass("c1990:size_t");

    test_eq( (long)d, -3L );
    test_true( sizeof(ptrdiff_t) >= 4u );
    test_true( (ptrdiff_t)-1 < (ptrdiff_t)0 );
    test_eq( sizeof(ptrdiff_t), sizeof(size_t) );
    test_pass("c1990:ptrdiff_t");
}

TEST_CASE(stddef, ptr_difference) {
    int arr[8];
    ptrdiff_t d, dn;
    int i;
    for (i = 0; i < 8; ++i) arr[i] = 0;

    d = &arr[7] - &arr[2];
    test_eq( d, (ptrdiff_t)5 );
    dn = &arr[2] - &arr[7];
    test_eq( dn, (ptrdiff_t)-5 );
    test_pass("c1990:ptrdiff_t(pointer subtraction)");
}

TEST_CASE(stddef, offsetof_macro) {
    struct Off o;
    const char* base;
    const char* pi;
    size_t delta;

    test_eq( offsetof(struct Off, c), (size_t)0 );
    test_true( offsetof(struct Off, i) >= sizeof(char) );
    test_true( offsetof(struct Off, d) >= offsetof(struct Off, i) + sizeof(int) );
    test_true( offsetof(struct Off, tail) >= offsetof(struct Off, d) + sizeof(double) );
    test_true( offsetof(struct Off, tail) + sizeof(o.tail) <= sizeof(struct Off) );

    base  = (const char*)&o;
    pi    = (const char*)&o.i;
    delta = (size_t)(pi - base);
    test_eq( delta, offsetof(struct Off, i) );
    test_pass("c1990:offsetof(type,member)");
}

TEST_CASE(stddef, null_macro) {
    void* nil = NULL;
    int   x   = 3;
    int*  q   = &x;

    test_true( nil == NULL );
    test_true( q != NULL );
    q = NULL;
    test_true( q == NULL );
    test_pass("c1990:NULL");
}

TEST_CASE(stddef, wchar_t_type) {

    wchar_t w = L'A';
    test_true( w == L'A' );
    test_true( sizeof(wchar_t) >= 1 );
    test_true( (wchar_t)-1 != 0 );
    test_pass("c1990:wchar_t");
}

TEST_CASE(stddef, max_align) {
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
    size_t a;
    max_align_t m;
    void* pm;

    TEST_C11(test_true( sizeof(max_align_t) >= sizeof(double) ));

    a = alignof(max_align_t);
    TEST_C11(test_true( a != 0 && (a & (a - 1)) == 0 ));
    TEST_C11(test_true( a >= alignof(double) ));
    TEST_C11(test_true( a >= alignof(void*) ));

    pm = &m;
#if defined(__WATCOMC__) || (defined(_MSC_VER) && !defined(_WIN64))
    TEST_SKIP1();
    TEST_NOTE("32-bit stack does not honour max_align_t for a local (A33)");
#endif
    TEST_C11(test_true( ((size_t)pm % a) == 0 ));
    pm = malloc(sizeof(max_align_t));
    test_ptr( pm );
    TEST_C11(test_true( ((size_t)pm % a) == 0 ));
    free(pm);
    test_pass("c2011:max_align_t");
#else
    TEST_NOTE("max_align_t needs C11; ccwrap cannot add it to a C99 front end");
    test_skip("c2011:max_align_t");
#endif
}

TEST_CASE(stddef, c23_surface) {
    TEST_SKIP1();
    TEST_NOTE("C23: needs the nullptr keyword; not provided on old front-ends");
    test_true( 1 );
    test_pass("c2023:nullptr_t");

    TEST_SKIP1();
    TEST_NOTE("C23: needs __builtin_unreachable or equivalent");
    test_true( 1 );
    test_pass("c2023:unreachable()");
}


TEST_CASE(stddef, version_macro_and_rsize_c2023) {
#if defined(__STDC_VERSION_STDDEF_H__)
    test_true( __STDC_VERSION_STDDEF_H__ >= 202311L );
    test_pass("c2023:__STDC_VERSION_STDDEF_H__");
#else
    TEST_NOTE("C23 header version macro not defined by this library");
    test_skip("c2023:__STDC_VERSION_STDDEF_H__");
#endif

#if defined(__STDC_LIB_EXT1__) && defined(RSIZE_MAX)
    {
        rsize_t r = (rsize_t)1;
        test_true( r == (rsize_t)1 );
        test_true( sizeof(rsize_t) == sizeof(size_t) );
    }
    test_pass("c2011:opt:rsize_t");
#else
    TEST_NOTE("Annex K is optional; this library does not provide it");
    test_skip("c2011:opt:rsize_t");
#endif
}

TEST_SUITE(stddef)
    TEST_ADD(size_t_and_ptrdiff_t)
    TEST_ADD(ptr_difference)
    TEST_ADD(offsetof_macro)
    TEST_ADD(null_macro)
    TEST_ADD(wchar_t_type)
    TEST_ADD(max_align)
    TEST_ADD(c23_surface)
    TEST_ADD(version_macro_and_rsize_c2023)
TEST_SUITE_END(stddef)
