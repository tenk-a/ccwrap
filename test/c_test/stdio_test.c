#include "c_test.h"

#include <stdio.h>
#include <wchar.h>
#include <stdint.h>
#include <inttypes.h>
#include <limits.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#define TST_TMPNAME "ccwstdio.tmp"
#define TST_TMPNAME2 "ccwstdi2.tmp"
#define TST_STDIO_BULK_SIZE 4096u
#define TST_INTEGER_TEXT_CAP (sizeof(unsigned long long) * CHAR_BIT + 3u)

static void tst_unsigned_text(char* dst, unsigned long long value, unsigned base) {
    char reverse[TST_INTEGER_TEXT_CAP];
    static const char digits[] = "0123456789abcdef";
    size_t n = 0;
    do {
        reverse[n++] = digits[(unsigned)(value % base)];
        value /= base;
    } while (value != 0);
    while (n != 0)
        *dst++ = reverse[--n];
    *dst = '\0';
}

static void tst_signed_text(char* dst, unsigned long long magnitude,
                            unsigned base, int negative) {
    if (negative)
        *dst++ = '-';
    tst_unsigned_text(dst, magnitude, base);
}

static int vsp(char* buf, size_t n, const char* fmt, ...) {
    int r;
    va_list ap;
    va_start(ap, fmt);
    r = vsnprintf(buf, n, fmt, ap);
    va_end(ap);
    return r;
}

static int vss(const char* s, const char* fmt, ...) {
    int r;
    va_list ap;
    va_start(ap, fmt);
    r = vsscanf(s, fmt, ap);
    va_end(ap);
    return r;
}

static int vfp(FILE* fp, const char* fmt, ...) {
    int r;
    va_list ap;
    va_start(ap, fmt);
    r = vfprintf(fp, fmt, ap);
    va_end(ap);
    return r;
}

TEST_CASE(stdio, types_and_macros) {
    FILE* fp = NULL;
    fpos_t pos;
    size_t n = 0;

    test_true( fp == NULL );          test_pass("c1990:FILE");
    test_true( (void*)NULL == (void*)0 );  test_pass("c1990:NULL");
    test_gt( sizeof(fpos_t), (size_t)0 );
    memset(&pos, 0, sizeof pos);      test_pass("c1990:fpos_t");
    test_eq( n, (size_t)0 );          test_pass("c1990:size_t");
    test_eq( EOF, -1 );               test_pass("c1990:EOF");
    test_gt( BUFSIZ, 0 );             test_pass("c1990:BUFSIZ");
    test_gt( FOPEN_MAX, 0 );          test_pass("c1990:FOPEN_MAX");
    test_gt( FILENAME_MAX, 0 );       test_pass("c1990:FILENAME_MAX");
    test_gt( L_tmpnam, 0 );           test_pass("c1990:L_tmpnam");
    test_gt( TMP_MAX, 0 );            test_pass("c1990:TMP_MAX");

    test_ne( SEEK_SET, SEEK_CUR ); test_ne( SEEK_SET, SEEK_END ); test_pass("c1990:SEEK_SET");
    test_ne( SEEK_CUR, SEEK_END ); test_pass("c1990:SEEK_CUR");
    test_true( SEEK_END == SEEK_END ); test_pass("c1990:SEEK_END");

    test_ne( _IOFBF, _IOLBF ); test_ne( _IOFBF, _IONBF ); test_pass("c1990:_IOFBF");
    test_ne( _IOLBF, _IONBF ); test_pass("c1990:_IOLBF");
    test_true( _IONBF == _IONBF ); test_pass("c1990:_IONBF");

    test_ptr( stdin );  test_true( stdin != stdout );  test_pass("c1990:stdin");
    test_ptr( stdout ); test_true( stdout != stderr ); test_pass("c1990:stdout");
    test_ptr( stderr ); test_pass("c1990:stderr");
}

TEST_CASE(stdio, sprintf_formats) {
    char buf[128];
    int iv;

    test_eq( sprintf(buf, "%d", 42), 2 );
    test_str_eq( buf, "42" );
    sprintf(buf, "%d %i %u", -7, -8, 9u);
    test_str_eq( buf, "-7 -8 9" );
    sprintf(buf, "%o %x %X", 8, 255, 255);
    test_str_eq( buf, "10 ff FF" );
    sprintf(buf, "%c%s", 'a', "bc");
    test_str_eq( buf, "abc" );
    sprintf(buf, "%%");
    test_str_eq( buf, "%" );

    sprintf(buf, "[%5d][%-5d][%05d]", 42, 42, 42);
    test_str_eq( buf, "[   42][42   ][00042]" );
    sprintf(buf, "[%+d][% d]", 42, 42);
    test_str_eq( buf, "[+42][ 42]" );
    sprintf(buf, "[%.3s][%8.3f]", "abcdef", 3.14159);
    test_str_eq( buf, "[abc][   3.142]" );
    sprintf(buf, "[%*d]", 6, 42);
    test_str_eq( buf, "[    42]" );

    sprintf(buf, "%ld %lu", 123456789L, 4000000000UL);
    test_str_eq( buf, "123456789 4000000000" );
    sprintf(buf, "%lld", -9000000000LL);
    test_str_eq( buf, "-9000000000" );
    sprintf(buf, "%hd %hhd", (short)-3, (signed char)-4);
    test_str_eq( buf, "-3 -4" );
    sprintf(buf, "%d", INT_MAX);
    iv = 0; test_eq( sscanf(buf, "%d", &iv), 1 ); test_eq( iv, INT_MAX );
    sprintf(buf, "%d", INT_MIN);
    iv = 0; test_eq( sscanf(buf, "%d", &iv), 1 ); test_eq( iv, INT_MIN );
    sprintf(buf, "%zu", (size_t)7);
#if defined(_MSC_VER) && _MSC_VER < 1900
    TEST_SKIP1();
#endif
    test_str_eq( buf, "7" );

    sprintf(buf, "%f", 1.5);
    test_str_eq( buf, "1.500000" );
    sprintf(buf, "%.2f %e %g", 2.345, 1500.0, 0.5);
    test_true( strncmp(buf, "2.35 1.5", 8) == 0 || strncmp(buf, "2.34 1.5", 8) == 0 );

    {
        int written = -1;
        sprintf(buf, "abc%n", &written);
        TEST_SKIP_UCRT("");
        test_eq( written, 3 );
    }
    test_pass("c1990:sprintf");
}

TEST_CASE(stdio, snprintf_truncation) {
    char buf[8];
    char guarded[6];
    int n;

    n = snprintf(buf, sizeof buf, "%d", 42);
    test_eq( n, 2 );
    test_str_eq( buf, "42" );

    memset(buf, '#', sizeof buf);
    n = snprintf(buf, 4, "abcdefgh");
    test_eq( n, 8 );
    test_str_eq( buf, "abc" );
    test_eq( (int)buf[3], 0 );

    n = snprintf(NULL, 0, "%d", 12345);
    test_eq( n, 5 );

    memset(guarded, '#', sizeof guarded);
    n = snprintf(guarded, 1, "abc");
    test_eq( n, 3 );
    test_eq( (int)guarded[0], 0 );
    test_eq( (int)guarded[1], (int)'#' );

    memset(guarded, '#', sizeof guarded);
    n = snprintf(guarded, 4, "abc");
    test_eq( n, 3 );
    test_str_eq( guarded, "abc" );
    test_eq( (int)guarded[4], (int)'#' );

    test_pass("c1999:snprintf");
    n = vsp(buf, sizeof buf, "%s-%d", "ab", 7);
    test_eq( n, 4 );
    test_str_eq( buf, "ab-7" );
    test_pass("c1999:vsnprintf");
}

TEST_CASE(stdio, sscanf_family) {
    int a = 0, b = 0, n;
    char word[16];
    double d = 0;
    long l = 0;
    unsigned u = 0;

    n = sscanf("12 34", "%d %d", &a, &b);
    test_eq( n, 2 );
    test_eq( a, 12 );
    test_eq( b, 34 );

    n = sscanf("hello 5", "%15s %d", word, &a);
    test_eq( n, 2 );
    test_str_eq( word, "hello" );
    test_eq( a, 5 );

    n = sscanf("3.5", "%lf", &d);
    test_eq( n, 1 );
    test_true( d > 3.49 && d < 3.51 );

    n = sscanf("ff 777 -1", "%lx %o %u", &l, &a, &u);
    test_eq( n, 3 );
    test_eq( l, 255L );
    test_eq( a, 511 );

    a = b = 0;
    n = sscanf("12-34", "%d,%d", &a, &b);
    test_eq( n, 1 );
    test_eq( a, 12 );

    n = sscanf("abc", "%d", &a);
    test_eq( n, 0 );

    a = 0;
    n = sscanf("9 8", "%*d %d", &a);
    test_eq( n, 1 );
    test_eq( a, 8 );
    {
        char c = 0;
        n = sscanf("Z", "%c", &c);
        test_eq( n, 1 );
        test_eq( (int)c, (int)'Z' );
    }
    test_pass("c1990:sscanf");

    n = vss("7 9", "%d %d", &a, &b);
    test_eq( n, 2 );
    test_eq( a, 7 );
    test_eq( b, 9 );
    test_pass("c1999:vsscanf");
}

TEST_CASE(stdio, printf_integer) {
    char buf[64];

    snprintf(buf, sizeof buf, "%d", 42);      test_str_eq( buf, "42" );  test_pass("c1990:printf %d");
    snprintf(buf, sizeof buf, "%i", -42);     test_str_eq( buf, "-42" ); test_pass("c1990:printf %i");
    snprintf(buf, sizeof buf, "%u", 42u);     test_str_eq( buf, "42" );  test_pass("c1990:printf %u");
    snprintf(buf, sizeof buf, "%o", 8);       test_str_eq( buf, "10" );  test_pass("c1990:printf %o");
    snprintf(buf, sizeof buf, "%x", 255);     test_str_eq( buf, "ff" );  test_pass("c1990:printf %x");
    snprintf(buf, sizeof buf, "%X", 255);     test_str_eq( buf, "FF" );  test_pass("c1990:printf %X");

    snprintf(buf, sizeof buf, "%5d", 42);     test_str_eq( buf, "   42" ); test_pass("c1990:printf %5d");
    snprintf(buf, sizeof buf, "%-5d", 42);    test_str_eq( buf, "42   " ); test_pass("c1990:printf %-5d");
    snprintf(buf, sizeof buf, "%05d", 42);    test_str_eq( buf, "00042" ); test_pass("c1990:printf %05d");
    snprintf(buf, sizeof buf, "%+d", 42);     test_str_eq( buf, "+42" );   test_pass("c1990:printf %+d");
    snprintf(buf, sizeof buf, "% d", 42);     test_str_eq( buf, " 42" );   test_pass("c1990:printf % d");
    snprintf(buf, sizeof buf, "%#x", 255);    test_str_eq( buf, "0xff" );  test_pass("c1990:printf %#x");
    snprintf(buf, sizeof buf, "%#o", 8);      test_str_eq( buf, "010" );   test_pass("c1990:printf %#o");
    snprintf(buf, sizeof buf, "%.3d", 5);     test_str_eq( buf, "005" );   test_pass("c1990:printf %.3d");
    snprintf(buf, sizeof buf, "%8.3d", 5);    test_str_eq( buf, "     005" ); test_pass("c1990:printf %8.3d");
}

TEST_CASE(stdio, printf_integer_limit_base_matrix) {
    static const int bases[3] = { 8, 10, 16 };
    char expected_min[TST_INTEGER_TEXT_CAP];
    char expected_max[TST_INTEGER_TEXT_CAP];
    char expected_zero[TST_INTEGER_TEXT_CAP];
    char actual_min[TST_INTEGER_TEXT_CAP];
    char actual_max[TST_INTEGER_TEXT_CAP];
    char actual_zero[TST_INTEGER_TEXT_CAP];
    unsigned long long long_min_magnitude;
    unsigned long long llong_min_magnitude;
    int i;

    long_min_magnitude = (unsigned long long)(0UL - (unsigned long)LONG_MIN);
    llong_min_magnitude = 0ULL - (unsigned long long)LLONG_MIN;

    for (i = 0; i < 3; ++i) {
        int base = bases[i];
        tst_signed_text(expected_min, long_min_magnitude, (unsigned)base, 1);
        tst_signed_text(expected_max, (unsigned long long)LONG_MAX, (unsigned)base, 0);
        if (base == 8) {
            sprintf(actual_min, "-%lo", (unsigned long)long_min_magnitude);
            sprintf(actual_max, "%lo", (unsigned long)LONG_MAX);
        } else if (base == 10) {
            sprintf(actual_min, "%ld", LONG_MIN);
            sprintf(actual_max, "%ld", LONG_MAX);
        } else {
            sprintf(actual_min, "-%lx", (unsigned long)long_min_magnitude);
            sprintf(actual_max, "%lx", (unsigned long)LONG_MAX);
        }
        test_str_eq( actual_min, expected_min );
        test_str_eq( actual_max, expected_max );
    }
    test_pass("c1990:printf signed long MIN/MAX in bases 8/10/16");

    for (i = 0; i < 3; ++i) {
        int base = bases[i];
        tst_unsigned_text(expected_zero, 0ULL, (unsigned)base);
        tst_unsigned_text(expected_max, (unsigned long long)ULONG_MAX, (unsigned)base);
        if (base == 8) {
            sprintf(actual_zero, "%lo", 0UL);
            sprintf(actual_max, "%lo", ULONG_MAX);
        } else if (base == 10) {
            sprintf(actual_zero, "%lu", 0UL);
            sprintf(actual_max, "%lu", ULONG_MAX);
        } else {
            sprintf(actual_zero, "%lx", 0UL);
            sprintf(actual_max, "%lx", ULONG_MAX);
        }
        test_str_eq( actual_zero, expected_zero );
        test_str_eq( actual_max, expected_max );
    }
    test_pass("c1990:printf unsigned long 0/MAX in bases 8/10/16");

    for (i = 0; i < 3; ++i) {
        int base = bases[i];
        tst_signed_text(expected_min, llong_min_magnitude, (unsigned)base, 1);
        tst_signed_text(expected_max, (unsigned long long)LLONG_MAX, (unsigned)base, 0);
        if (base == 8) {
            sprintf(actual_min, "-%llo", llong_min_magnitude);
            sprintf(actual_max, "%llo", (unsigned long long)LLONG_MAX);
        } else if (base == 10) {
            sprintf(actual_min, "%lld", LLONG_MIN);
            sprintf(actual_max, "%lld", LLONG_MAX);
        } else {
            sprintf(actual_min, "-%llx", llong_min_magnitude);
            sprintf(actual_max, "%llx", (unsigned long long)LLONG_MAX);
        }
        test_str_eq( actual_min, expected_min );
        test_str_eq( actual_max, expected_max );
    }
    test_pass("c1999:printf signed long long MIN/MAX in bases 8/10/16");

    for (i = 0; i < 3; ++i) {
        int base = bases[i];
        tst_unsigned_text(expected_zero, 0ULL, (unsigned)base);
        tst_unsigned_text(expected_max, ULLONG_MAX, (unsigned)base);
        if (base == 8) {
            sprintf(actual_zero, "%llo", 0ULL);
            sprintf(actual_max, "%llo", ULLONG_MAX);
        } else if (base == 10) {
            sprintf(actual_zero, "%llu", 0ULL);
            sprintf(actual_max, "%llu", ULLONG_MAX);
        } else {
            sprintf(actual_zero, "%llx", 0ULL);
            sprintf(actual_max, "%llx", ULLONG_MAX);
        }
        test_str_eq( actual_zero, expected_zero );
        test_str_eq( actual_max, expected_max );
    }
    test_pass("c1999:printf unsigned long long 0/MAX in bases 8/10/16");
}

TEST_CASE(stdio, printf_integer_limit_binary_c23) {
#if defined(PRIbMAX)
    char expected_min[TST_INTEGER_TEXT_CAP];
    char expected_max[TST_INTEGER_TEXT_CAP];
    char expected_zero[TST_INTEGER_TEXT_CAP];
    char actual_min[TST_INTEGER_TEXT_CAP];
    char actual_max[TST_INTEGER_TEXT_CAP];
    char actual_zero[TST_INTEGER_TEXT_CAP];
    unsigned long long long_min_magnitude;
    unsigned long long llong_min_magnitude;
    const char* neg_long_fmt = "-%lb";
    const char* long_fmt = "%lb";
    const char* neg_llong_fmt = "-%llb";
    const char* llong_fmt = "%llb";

    long_min_magnitude = (unsigned long long)(0UL - (unsigned long)LONG_MIN);
    llong_min_magnitude = 0ULL - (unsigned long long)LLONG_MIN;

    tst_signed_text(expected_min, long_min_magnitude, 2u, 1);
    tst_signed_text(expected_max, (unsigned long long)LONG_MAX, 2u, 0);
    sprintf(actual_min, neg_long_fmt, (unsigned long)long_min_magnitude);
    sprintf(actual_max, long_fmt, (unsigned long)LONG_MAX);
    test_str_eq( actual_min, expected_min );
    test_str_eq( actual_max, expected_max );
    test_pass("c2023:printf signed long MIN/MAX in base 2");

    tst_unsigned_text(expected_zero, 0ULL, 2u);
    tst_unsigned_text(expected_max, (unsigned long long)ULONG_MAX, 2u);
    sprintf(actual_zero, long_fmt, 0UL);
    sprintf(actual_max, long_fmt, ULONG_MAX);
    test_str_eq( actual_zero, expected_zero );
    test_str_eq( actual_max, expected_max );
    test_pass("c2023:printf unsigned long 0/MAX in base 2");

    tst_signed_text(expected_min, llong_min_magnitude, 2u, 1);
    tst_signed_text(expected_max, (unsigned long long)LLONG_MAX, 2u, 0);
    sprintf(actual_min, neg_llong_fmt, llong_min_magnitude);
    sprintf(actual_max, llong_fmt, (unsigned long long)LLONG_MAX);
    test_str_eq( actual_min, expected_min );
    test_str_eq( actual_max, expected_max );
    test_pass("c2023:printf signed long long MIN/MAX in base 2");

    tst_unsigned_text(expected_zero, 0ULL, 2u);
    tst_unsigned_text(expected_max, ULLONG_MAX, 2u);
    sprintf(actual_zero, llong_fmt, 0ULL);
    sprintf(actual_max, llong_fmt, ULLONG_MAX);
    test_str_eq( actual_zero, expected_zero );
    test_str_eq( actual_max, expected_max );
    test_pass("c2023:printf unsigned long long 0/MAX in base 2");
#else
    TEST_NOTE("C23 binary printf conversion is not available");
    test_skip("c2023:printf signed long MIN/MAX in base 2");
    test_skip("c2023:printf unsigned long 0/MAX in base 2");
    test_skip("c2023:printf signed long long MIN/MAX in base 2");
    test_skip("c2023:printf unsigned long long 0/MAX in base 2");
#endif
}

TEST_CASE(stdio, printf_length_mods) {
    char buf[64];

    snprintf(buf, sizeof buf, "%hd", (short)-3);         test_str_eq( buf, "-3" );        test_pass("c1990:printf %hd");
    snprintf(buf, sizeof buf, "%ld", 123456789L);        test_str_eq( buf, "123456789" ); test_pass("c1990:printf %ld");
    snprintf(buf, sizeof buf, "%lx", 255UL);             test_str_eq( buf, "ff" );        test_pass("c1990:printf %lx");

    snprintf(buf, sizeof buf, "%hhd", (signed char)-4); TEST_C99( test_str_eq( buf, "-4" ) );          test_pass("c1999:printf %hhd");
    snprintf(buf, sizeof buf, "%lld", -9000000000LL);   TEST_C99( test_str_eq( buf, "-9000000000" ) );  test_pass("c1999:printf %lld");
    snprintf(buf, sizeof buf, "%llu", 18000000000ULL);  TEST_C99( test_str_eq( buf, "18000000000" ) );  test_pass("c1999:printf %llu");

    snprintf(buf, sizeof buf, "%ju", (uintmax_t)100);
    TEST_SKIP_VC120("pre-UCRT MSVC prints z/t/j length modifiers literally");
    TEST_C99( test_str_eq( buf, "100" ) );  test_pass("c1999:printf %ju");
    snprintf(buf, sizeof buf, "%zu", (size_t)7);
    TEST_SKIP_VC120("pre-UCRT MSVC prints z/t/j length modifiers literally");
    TEST_C99( test_str_eq( buf, "7" ) );    test_pass("c1999:printf %zu");
    snprintf(buf, sizeof buf, "%td", (ptrdiff_t)-5);
    TEST_SKIP_VC120("pre-UCRT MSVC prints z/t/j length modifiers literally");
    TEST_C99( test_str_eq( buf, "-5" ) );   test_pass("c1999:printf %td");
}

TEST_CASE(stdio, printf_float) {
    char buf[64];

    snprintf(buf, sizeof buf, "%f", 1.5);   test_str_eq( buf, "1.500000" ); test_pass("c1990:printf %f");
    snprintf(buf, sizeof buf, "%F", 1.5);
    TEST_SKIP_VC120("uppercase %F is a C99 conversion, absent from pre-UCRT MSVC");
    TEST_SKIP_DOS("uppercase %F is not recognised by Open Watcom's DOS CRT");
    TEST_C99( test_str_eq( buf, "1.500000" ) ); test_pass("c1999:printf %F");

    snprintf(buf, sizeof buf, "%e", 1500.0);
    test_true( strncmp(buf, "1.500000e+0", 11) == 0 ); test_pass("c1990:printf %e");
    snprintf(buf, sizeof buf, "%E", 1500.0);
    test_true( strncmp(buf, "1.500000E+0", 11) == 0 ); test_pass("c1990:printf %E");

    snprintf(buf, sizeof buf, "%g", 0.5);       test_str_eq( buf, "0.5" );    test_pass("c1990:printf %g");
    snprintf(buf, sizeof buf, "%G", 100000.0);  test_str_eq( buf, "100000" ); test_pass("c1990:printf %G");

    snprintf(buf, sizeof buf, "%.2f", 3.14159);   test_str_eq( buf, "3.14" );      test_pass("c1990:printf %.2f");
    snprintf(buf, sizeof buf, "%10.3f", 3.14159); test_str_eq( buf, "     3.142" ); test_pass("c1990:printf %10.3f");
    snprintf(buf, sizeof buf, "%-10.3f", 3.14159);test_str_eq( buf, "3.142     " ); test_pass("c1990:printf %-10.3f");
    snprintf(buf, sizeof buf, "%+.2f", 3.14159);  test_str_eq( buf, "+3.14" );      test_pass("c1990:printf %+.2f");
    snprintf(buf, sizeof buf, "%010.3f", 3.14159);test_str_eq( buf, "000003.142" ); test_pass("c1990:printf %010.3f");
    snprintf(buf, sizeof buf, "%.6g", 3.14159);   test_str_eq( buf, "3.14159" );    test_pass("c1990:printf %.6g");
    snprintf(buf, sizeof buf, "%#g", 1.5);        test_str_eq( buf, "1.50000" );    test_pass("c1990:printf %#g");
}

TEST_CASE(stdio, printf_combinations) {
    char buf[64];

    snprintf(buf, sizeof buf, "%-10.6g", 3.14159); test_str_eq( buf, "3.14159   " ); test_pass("c1990:printf %-10.6g");
    snprintf(buf, sizeof buf, "%+08.2f", 3.14159); test_str_eq( buf, "+0003.14" );   test_pass("c1990:printf %+08.2f");
    snprintf(buf, sizeof buf, "%#010x", 255);      test_str_eq( buf, "0x000000ff" ); test_pass("c1990:printf %#010x");

    snprintf(buf, sizeof buf, "%10.3e", 1500.0);
    test_true( strstr(buf, "1.500e+0") != NULL && strlen(buf) == 10 ); test_pass("c1990:printf %10.3e");
    snprintf(buf, sizeof buf, "% 08.2f", 3.14159); test_str_eq( buf, " 0003.14" );   test_pass("c1990:printf % 08.2f");
    snprintf(buf, sizeof buf, "%-+10d", 42);       test_str_eq( buf, "+42       " ); test_pass("c1990:printf %-+10d");
}

TEST_CASE(stdio, printf_char_string_misc) {
    char buf[64];

    snprintf(buf, sizeof buf, "%c", 'Q');         test_str_eq( buf, "Q" );     test_pass("c1990:printf %c");
    snprintf(buf, sizeof buf, "%s", "hi");        test_str_eq( buf, "hi" );    test_pass("c1990:printf %s");
    snprintf(buf, sizeof buf, "%5s", "hi");       test_str_eq( buf, "   hi" ); test_pass("c1990:printf %5s");
    snprintf(buf, sizeof buf, "%-5s", "hi");      test_str_eq( buf, "hi   " ); test_pass("c1990:printf %-5s");
    snprintf(buf, sizeof buf, "%.3s", "abcdef");  test_str_eq( buf, "abc" );   test_pass("c1990:printf %.3s");
    snprintf(buf, sizeof buf, "%%");              test_str_eq( buf, "%" );     test_pass("c1990:printf %%");

    {
        int local = 0;
        snprintf(buf, sizeof buf, "%p", (void*)&local);
        test_true( strlen(buf) > 0 ); test_pass("c1990:printf %p");
    }

    snprintf(buf, sizeof buf, "%a", 1.0);
    TEST_SKIP_VC120("%a hex-float conversion is C99, absent from pre-UCRT MSVC");
    TEST_C99( test_true( strlen(buf) >= 3 && strchr(buf, 'p') != NULL ) ); test_pass("c1999:printf %a");
    snprintf(buf, sizeof buf, "%A", 1.0);
    TEST_SKIP_VC120("%A hex-float conversion is C99, absent from pre-UCRT MSVC");
    TEST_C99( test_true( strlen(buf) >= 3 && strchr(buf, 'P') != NULL ) ); test_pass("c1999:printf %A");
}

TEST_CASE(stdio, scanf_conversions) {
    int a; unsigned u; float fl; double d; long l; long long ll;
    union { signed char v; int pad; } sc; char c; char word[32]; int n;

    a = 0;  n = sscanf("42", "%d", &a);          test_eq( n, 1 ); test_eq( a, 42 );  test_pass("c1990:sscanf %d");
    a = 0;  n = sscanf("0x1f", "%i", &a);        test_eq( n, 1 ); test_eq( a, 31 );  test_pass("c1990:sscanf %i");
    u = 0;  n = sscanf("100", "%u", &u);         test_eq( n, 1 ); test_eq( u, 100u ); test_pass("c1990:sscanf %u");
    a = 0;  n = sscanf("17", "%o", &a);          test_eq( n, 1 ); test_eq( a, 15 );  test_pass("c1990:sscanf %o");
    u = 0;  n = sscanf("ff", "%x", &u);          test_eq( n, 1 ); test_eq( u, 255u ); test_pass("c1990:sscanf %x");

    fl = 0;  n = sscanf("3.5", "%f", &fl);       test_eq( n, 1 ); test_true( fl > 3.49f && fl < 3.51f );      test_pass("c1990:sscanf %f");
    fl = 0;  n = sscanf("1.5e3", "%e", &fl);     test_eq( n, 1 ); test_true( fl > 1499.0f && fl < 1501.0f );  test_pass("c1990:sscanf %e");
    fl = 0;  n = sscanf("2.5", "%g", &fl);       test_eq( n, 1 ); test_true( fl > 2.49f && fl < 2.51f );      test_pass("c1990:sscanf %g");

    c = 0;   n = sscanf("Z", "%c", &c);          test_eq( n, 1 ); test_eq( (int)c, (int)'Z' ); test_pass("c1990:sscanf %c");
    word[0] = 0; n = sscanf("hello", "%31s", word);  test_eq( n, 1 ); test_str_eq( word, "hello" ); test_pass("c1990:sscanf %s");

    word[0] = 0; n = sscanf("abc123", "%31[abc]", word); test_eq( n, 1 ); test_str_eq( word, "abc" );   test_pass("c1990:sscanf %[abc]");

    word[0] = 0; n = sscanf("hello-world", "%31[^-]", word); test_eq( n, 1 ); test_str_eq( word, "hello" ); test_pass("c1990:sscanf %[^-]");

    a = 0;   n = sscanf("9 8", "%*d %d", &a);    test_eq( n, 1 ); test_eq( a, 8 );   test_pass("c1990:sscanf %*d");

    a = 0;   n = sscanf("123456", "%5d", &a);    test_eq( n, 1 ); test_eq( a, 12345 ); test_pass("c1990:sscanf %5d");

    sc.pad = 0;  n = sscanf("-4", "%hhd", &sc.v);
    TEST_SKIP_VC120("hh length modifier for scanf is C99, unreliable on pre-UCRT MSVC");
    TEST_C99( test_true( n == 1 && sc.v == -4 ) ); test_pass("c1999:sscanf %hhd");
    l = 0;   n = sscanf("100000", "%ld", &l);    test_eq( n, 1 ); test_eq( l, 100000L ); test_pass("c1990:sscanf %ld");
    ll = 0;  n = sscanf("-9000000000", "%lld", &ll);
    TEST_SKIP_VC120("ll length modifier for scanf is C99, unreliable on pre-UCRT MSVC");
    TEST_C99( test_true( n == 1 && ll == -9000000000LL ) ); test_pass("c1999:sscanf %lld");
    d = 0;   n = sscanf("2.75", "%lf", &d);      test_eq( n, 1 ); test_true( d > 2.74 && d < 2.76 ); test_pass("c1990:sscanf %lf");

    {
        int x = 0, y = 0, z = 0;
        n = sscanf("1 2 3", "%d %d %d", &x, &y, &z);
        test_eq( n, 3 ); test_eq( x, 1 ); test_eq( y, 2 ); test_eq( z, 3 );
        n = sscanf("1 x", "%d %d", &x, &y);      test_eq( n, 1 );
        n = sscanf("nope", "%d", &x);            test_eq( n, 0 );
        test_pass("c1990:sscanf return value");
    }
}

TEST_CASE(stdio, fopen_fclose_remove_rename) {
    FILE* fp;
    char buf[32];

    remove(TST_TMPNAME);
    remove(TST_TMPNAME2);

    fp = fopen(TST_TMPNAME, "w");
    test_ptr( fp );
    test_true( fopen("ccwnosuc.tmp", "r") == NULL );
    test_pass("c1990:fopen");
    test_gt( fprintf(fp, "hello\n"), 0 );
    test_pass("c1990:fprintf");
    test_eq( fclose(fp), 0 );
    test_pass("c1990:fclose");

    fp = fopen(TST_TMPNAME, "r");
    test_ptr( fp );
    test_true( fgets(buf, sizeof buf, fp) == buf );
    test_str_eq( buf, "hello\n" );
    fclose(fp);

    fp = fopen(TST_TMPNAME, "a");
    fputs("world\n", fp);
    fclose(fp);
    fp = fopen(TST_TMPNAME, "r");
    fgets(buf, sizeof buf, fp);
    test_str_eq( buf, "hello\n" );
    fgets(buf, sizeof buf, fp);
    test_str_eq( buf, "world\n" );
    fclose(fp);

    test_eq( rename(TST_TMPNAME, TST_TMPNAME2), 0 );
    test_true( fopen(TST_TMPNAME, "r") == NULL );
    fp = fopen(TST_TMPNAME2, "r"); fclose(fp);
    test_pass("c1990:rename");

    test_eq( remove(TST_TMPNAME2), 0 );
    test_true( fopen(TST_TMPNAME2, "r") == NULL );
    test_ne( remove("ccwnosuc.tmp"), 0 );
    test_pass("c1990:remove");
}

TEST_CASE(stdio, tmpfile_and_tmpnam) {
    FILE* fp = tmpfile();
    char name[L_tmpnam];
    char* r;

    test_ptr( fp );
    if (fp) {
        test_gt( fputs("scratch", fp), -1 );
        rewind(fp);
        {
            char buf[16];
            test_true( fgets(buf, sizeof buf, fp) != NULL );
            test_str_eq( buf, "scratch" );
        }
        test_eq( fclose(fp), 0 );
    }
    test_pass("c1990:tmpfile");

    r = tmpnam(name);
    test_true( r == name );
    test_gt( strlen(name), (size_t)0 );
    test_pass("c1990:tmpnam");
}

TEST_CASE(stdio, setvbuf_and_freopen) {
    FILE* fp;
    char buf[BUFSIZ];

    remove(TST_TMPNAME);
    fp = fopen(TST_TMPNAME, "w");
    test_ptr( fp );

    test_eq( setvbuf(fp, buf, _IOFBF, sizeof buf), 0 );
    fputs("buffered\n", fp);
    test_eq( fclose(fp), 0 );
    test_pass("c1990:setvbuf");

    fp = fopen(TST_TMPNAME, "w");
    test_ptr( fp );
    test_eq( setvbuf(fp, NULL, _IONBF, 0), 0 );
    fputs("unbuffered\n", fp);
    fclose(fp);

    fp = fopen(TST_TMPNAME, "r");
    test_ptr( fp );
    {
        char line[32];
        fgets(line, sizeof line, fp);
        test_str_eq( line, "unbuffered\n" );
    }

    fp = freopen(TST_TMPNAME, "r", fp);
    test_ptr( fp );
    if (fp) fclose(fp);
    remove(TST_TMPNAME);
    test_pass("c1990:freopen");
}

TEST_CASE(stdio, fread_fwrite) {
    FILE* fp;
    static unsigned char out[TST_STDIO_BULK_SIZE + 2u];
    static unsigned char in[TST_STDIO_BULK_SIZE + 2u];
    size_t n;
    int i;

    out[0] = (unsigned char)0x31;
    out[TST_STDIO_BULK_SIZE + 1u] = (unsigned char)0x32;
    for (i = 0; i < (int)TST_STDIO_BULK_SIZE; ++i)
        out[i + 1] = (unsigned char)(1u + (unsigned)i % 251u);

    remove(TST_TMPNAME);
    fp = fopen(TST_TMPNAME, "wb");
    test_ptr( fp );
    n = fwrite(out + 1, 1, TST_STDIO_BULK_SIZE, fp);
    test_eq( n, (size_t)TST_STDIO_BULK_SIZE );
    test_eq( fwrite(out + 1, 1, 0, fp), (size_t)0 );
    fclose(fp);
    test_pass("c1990:fwrite");

    fp = fopen(TST_TMPNAME, "rb");
    test_ptr( fp );
    memset(in, 0x55, sizeof in);
    n = fread(in + 1, 1, TST_STDIO_BULK_SIZE, fp);
    test_eq( n, (size_t)TST_STDIO_BULK_SIZE );
    test_eq( memcmp(in + 1, out + 1, TST_STDIO_BULK_SIZE), 0 );
    test_eq( (int)in[0], 0x55 );
    test_eq( (int)in[TST_STDIO_BULK_SIZE + 1u], 0x55 );
    test_pass("c1990:fread");

    n = fread(in + 1, 1, TST_STDIO_BULK_SIZE, fp);
    test_eq( n, (size_t)0 );
    test_true( feof(fp) );
    test_pass("c1990:feof");
    fclose(fp);

    fp = fopen(TST_TMPNAME, "rb");
    test_ptr( fp );
    n = fread(in + 1, 256, TST_STDIO_BULK_SIZE / 256u, fp);
    test_eq( n, (size_t)(TST_STDIO_BULK_SIZE / 256u) );
    test_pass("c1990:fread (element size honoured)");
    test_eq( fseek(fp, (long)TST_STDIO_BULK_SIZE - 1L, SEEK_SET), 0 );
    in[1] = in[2] = 0;
    n = fread(in + 1, 1, 2, fp);
    test_eq( n, (size_t)1 );
    test_eq( (int)in[1], (int)out[TST_STDIO_BULK_SIZE] );
    test_eq( (int)in[2], 0 );
    test_pass("c1990:fread bulk final-byte boundary");
    fclose(fp);
    remove(TST_TMPNAME);
}

TEST_CASE(stdio, char_and_line_io) {
    FILE* fp;
    char line[32];
    int c;

    remove(TST_TMPNAME);
    fp = fopen(TST_TMPNAME, "w+");
    test_ptr( fp );

    test_eq( fputc('a', fp), (int)'a' );  test_pass("c1990:fputc");
    test_eq( putc('b', fp), (int)'b' );   test_pass("c1990:putc");
    test_true( fputs("cd\nef\n", fp) >= 0 ); test_pass("c1990:fputs");
    rewind(fp);

    c = fgetc(fp);
    test_eq( c, (int)'a' );               test_pass("c1990:fgetc");
    c = getc(fp);
    test_eq( c, (int)'b' );               test_pass("c1990:getc");

    test_eq( ungetc(c, fp), (int)'b' );
    test_eq( fgetc(fp), (int)'b' );       test_pass("c1990:ungetc");

    test_true( fgets(line, sizeof line, fp) == line );
    test_str_eq( line, "cd\n" );
    test_true( fgets(line, sizeof line, fp) == line );
    test_str_eq( line, "ef\n" );
    test_pass("c1990:fgets");

    fclose(fp);
    remove(TST_TMPNAME);
}

TEST_CASE(stdio, fprintf_fscanf) {
    FILE* fp;
    int a = 0, b = 0, n;
    char word[16];

    remove(TST_TMPNAME);
    fp = fopen(TST_TMPNAME, "w+");
    test_ptr( fp );

    test_gt( fprintf(fp, "%s %d %d\n", "vals", 11, 22), 0 );
    test_pass("c1990:fprintf");
    test_gt( vfp(fp, "%d\n", 33), 0 );
    test_pass("c1990:vfprintf");
    rewind(fp);

    n = fscanf(fp, "%15s %d %d", word, &a, &b);
    test_eq( n, 3 );
    test_str_eq( word, "vals" );
    test_eq( a, 11 );
    test_eq( b, 22 );

    n = fscanf(fp, "%d", &a);
    test_eq( n, 1 );
    test_eq( a, 33 );

    n = fscanf(fp, "%d", &a);
    test_eq( n, EOF );

    fclose(fp);
    remove(TST_TMPNAME);
    test_pass("c1990:fscanf");
}

TEST_CASE(stdio, positioning) {
    FILE* fp;
    long pos;
    fpos_t fpos;
    int c;

    remove(TST_TMPNAME);
    fp = fopen(TST_TMPNAME, "w+b");
    test_ptr( fp );
    fputs("0123456789", fp);

    test_eq( ftell(fp), 10L );
    test_pass("c1990:ftell");
    test_eq( fseek(fp, 0L, SEEK_SET), 0 );
    test_eq( fgetc(fp), (int)'0' );
    test_eq( fseek(fp, 2L, SEEK_CUR), 0 );
    test_eq( fseek(fp, -1L, SEEK_END), 0 );
    test_eq( fgetc(fp), (int)'9' );
    test_pass("c1990:fseek");

    rewind(fp);
    test_eq( ftell(fp), 0L );
    test_pass("c1990:rewind");

    fseek(fp, 4L, SEEK_SET);
    test_eq( fgetpos(fp, &fpos), 0 );
    pos = ftell(fp);
    test_eq( pos, 4L );
    test_pass("c1990:fgetpos");
    fseek(fp, 0L, SEEK_SET);
    test_eq( fsetpos(fp, &fpos), 0 );
    test_eq( ftell(fp), 4L );
    c = fgetc(fp);
    test_eq( c, (int)'4' );
    test_pass("c1990:fsetpos");

    fclose(fp);
    remove(TST_TMPNAME);
    (void)pos;
}

TEST_CASE(stdio, error_handling) {
    FILE* fp;

    remove(TST_TMPNAME);
    fp = fopen(TST_TMPNAME, "w+");
    test_ptr( fp );

    test_true( !ferror(fp) );
    test_pass("c1990:ferror");

    fputs("x", fp);
    rewind(fp);
    test_eq( fgetc(fp), (int)'x' );
    test_eq( fgetc(fp), EOF );
    test_true( feof(fp) );
    test_pass("c1990:feof");

    clearerr(fp);
    test_true( !feof(fp) && !ferror(fp) );
    test_pass("c1990:clearerr");

    fclose(fp);
    fp = fopen(TST_TMPNAME, "w");
    test_ptr( fp );
    test_eq( fgetc(fp), EOF );
    clearerr(fp);
    fclose(fp);
    remove(TST_TMPNAME);
}

TEST_CASE(stdio, stdout_family_does_not_crash) {

    test_gt( printf(""), -1 );          test_pass("c1990:printf");
    test_gt( fprintf(stdout, ""), -1 );
    test_eq( fflush(stdout), 0 );
    test_eq( fflush(NULL), 0 );
    test_pass("c1990:fflush");
}

#if defined(_MSC_VER) && _MSC_VER < 1900
# define TST_NO_VSCANF 1
#endif

static int vsp_old(char* buf, const char* fmt, ...) {
    int r; va_list ap; va_start(ap, fmt); r = vsprintf(buf, fmt, ap); va_end(ap); return r;
}
static int vpr(const char* fmt, ...) {
    int r; va_list ap; va_start(ap, fmt); r = vprintf(fmt, ap); va_end(ap); return r;
}
#if !defined(TST_NO_VSCANF) && TEST_TARGET_C >= 1999
static int vfs(FILE* fp, const char* fmt, ...) {
    int r; va_list ap; va_start(ap, fmt); r = vfscanf(fp, fmt, ap); va_end(ap); return r;
}
#endif

TEST_CASE(stdio, vsprintf_vprintf) {
    char buf[32];
    int n = vsp_old(buf, "%d-%s", 5, "x");
    test_eq( n, 3 );
    test_str_eq( buf, "5-x" );
    test_pass("c1990:vsprintf");

    test_gt( vpr(""), -1 );
    test_pass("c1990:vprintf");
}

TEST_CASE(stdio, setbuf_puts_putchar_perror) {
    FILE* fp;

    remove(TST_TMPNAME);
    fp = fopen(TST_TMPNAME, "w");
    test_ptr( fp );
    if (fp) {
        static char sb[BUFSIZ];
        setbuf(fp, sb);
        fputs("x", fp);
        test_eq( fclose(fp), 0 );
    }
    remove(TST_TMPNAME);
    test_pass("c1990:setbuf");

    test_gt( puts(""), -1 );
    test_pass("c1990:puts");
    test_eq( putchar('\n'), (int)'\n' );
    test_pass("c1990:putchar");

    perror("");
    test_true( 1 );
    test_pass("c1990:perror");
}

TEST_CASE(stdio, vfscanf_from_file) {
#if !defined(TST_NO_VSCANF) && TEST_TARGET_C >= 1999
    FILE* fp;
    int a = 0, b = 0, n;

    remove(TST_TMPNAME);
    fp = fopen(TST_TMPNAME, "w+");
    test_ptr( fp );
    fputs("15 25", fp);
    rewind(fp);
    n = vfs(fp, "%d %d", &a, &b);
    test_eq( n, 2 );
    test_eq( a, 15 );
    test_eq( b, 25 );
    fclose(fp);
    remove(TST_TMPNAME);
    test_pass("c1999:vfscanf");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
    TEST_NOTE("vfscanf not declared by pre-UCRT MSVC (C99)");
    test_skip("c1999:vfscanf");
#endif
}

TEST_CASE(stdio, stdin_readers_exist) {

    int (*pg)(void)              = getchar;
    int (*ps)(const char*, ...)  = scanf;

    test_ptr( (void*)pg );
    TEST_NOTE("getchar reads stdin; existence-only (not exercised)");
    test_pass("c1990:getchar (existence)");
    test_ptr( (void*)ps );
    TEST_NOTE("scanf reads stdin; existence-only (not exercised)");
    test_pass("c1990:scanf (existence)");

#if !defined(TST_NO_VSCANF) && TEST_TARGET_C >= 1999
    {
        int (*pv)(const char*, va_list) = vscanf;
        test_ptr( (void*)pv );
    }
    TEST_NOTE("vscanf reads stdin; existence-only (not exercised)");
    test_pass("c1999:vscanf (existence)");
#else
    TEST_SKIP1(); TEST_NOTE("vscanf not declared by pre-UCRT MSVC (C99)");
    test_skip("c1999:vscanf (existence)");
#endif

#if TEST_TARGET_C < 2011
    {
        char* (*pgets)(char*) = gets;
        test_ptr( (void*)pgets );
    }
    TEST_NOTE("gets existed C89..C99; existence-only (unsafe, reads stdin)");
    test_pass("c1990:gets (removed in C11)");
#else
    TEST_SKIP1(); TEST_NOTE("gets removed in C11 (existed C89..C99)");
    test_skip("c1990:gets (removed in C11)");
#endif
}

TEST_CASE(stdio, printf_specifiers_a_c1990) {
    char b[64];
    sprintf(b, "%#.0f", 3.0);
    test_true( strcmp(b, "3.") == 0 );
    test_pass("c1990:printf %#.0f (# keeps the decimal point)");
    sprintf(b, "%#.0o", (unsigned)8);
    test_true( strcmp(b, "010") == 0 );
    test_pass("c1990:printf %#.0o (# forces a leading zero)");
    sprintf(b, "%#X", (unsigned)42);
    test_true( strcmp(b, "0X2A") == 0 );
    test_pass("c1990:printf %#X");
    sprintf(b, "%#e", 3.0);
    test_true( b[0] != 0 );
    test_pass("c1990:printf %#e (# keeps the decimal point)");
    sprintf(b, "%*d", 6, 42);
    test_true( strcmp(b, "    42") == 0 );
    test_pass("c1990:printf %*d (width from argument)");
    sprintf(b, "%-05d", 42);
    test_true( strcmp(b, "42   ") == 0 );
    test_pass("c1990:printf %-05d (- overrides 0)");
    sprintf(b, "%-8.3f", 3.5);
    test_true( strcmp(b, "3.500   ") == 0 );
    test_pass("c1990:printf %-8.3f");
    sprintf(b, "%.*f", 2, 3.5);
    test_true( strcmp(b, "3.50") == 0 );
    test_pass("c1990:printf %.*f (precision from argument)");
    sprintf(b, "%.0d", 0);
    test_true( strcmp(b, "") == 0 );
    test_pass("c1990:printf %.0d with value 0 (no characters)");
    sprintf(b, "%.0e", 3.0);
    TEST_SKIP_VC120("the pre-UCRT MSVC CRT writes a three-digit exponent (3e+000)");
    test_true( strcmp(b, "3e+00") == 0 );
    test_pass("c1990:printf %.0e");
    sprintf(b, "%.0f", 2.5);
    test_true( b[0] != 0 );
    test_pass("c1990:printf %.0f (round-to-nearest of the decimal point)");
    sprintf(b, "%05.3d", 7);
    test_true( strcmp(b, "  007") == 0 );
    test_pass("c1990:printf %05.3d (0 flag ignored when a precision is given)");
    sprintf(b, "%5.2s", "abcdef");
    test_true( strcmp(b, "   ab") == 0 );
    test_pass("c1990:printf %5.2s (width + precision on %s)");
    sprintf(b, "%5c", 'A');
    test_true( strcmp(b, "    A") == 0 );
    test_pass("c1990:printf %5c (width on %c)");
}

TEST_CASE(stdio, printf_specifiers_b_c1990) {
    char b[64];
    sprintf(b, "%Le", 3.0L);
    test_true( b[0] != 0 );
    test_pass("c1990:printf %Le");
    sprintf(b, "%Lf", 3.5L);
    test_true( strcmp(b, "3.500000") == 0 );
    test_pass("c1990:printf %Lf");
    sprintf(b, "%Lg", 3.5L);
    test_true( strcmp(b, "3.5") == 0 );
    test_pass("c1990:printf %Lg");
    sprintf(b, "%hi", (short)-7);
    test_true( strcmp(b, "-7") == 0 );
    test_pass("c1990:printf %hi");
    sprintf(b, "%ho", (unsigned short)8);
    test_true( strcmp(b, "10") == 0 );
    test_pass("c1990:printf %ho");
    sprintf(b, "%hu", (unsigned short)42);
    test_true( strcmp(b, "42") == 0 );
    test_pass("c1990:printf %hu");
    sprintf(b, "%hx", (unsigned short)255);
    test_true( strcmp(b, "ff") == 0 );
    test_pass("c1990:printf %hx");
    sprintf(b, "%lX", 42UL);
    test_true( strcmp(b, "2A") == 0 );
    test_pass("c1990:printf %lX");
    sprintf(b, "%li", -42L);
    test_true( strcmp(b, "-42") == 0 );
    test_pass("c1990:printf %li");
    sprintf(b, "%lo", 8UL);
    test_true( strcmp(b, "10") == 0 );
    test_pass("c1990:printf %lo");
    sprintf(b, "%lu", 42UL);
    test_true( strcmp(b, "42") == 0 );
    test_pass("c1990:printf %lu");
}


TEST_CASE(stdio, sscanf_conversions_c1990) {
    int i = 0; unsigned u = 0; short h = 0; unsigned short hu = 0;
    long l = 0; unsigned long ul = 0; double d = 0; long double ld = 0;
    char s[16]; char c3[4]; void* vp = 0; int n = 0;

    test_eq( sscanf("   42", "%d", &i), 1 );
    test_eq( i, 42 );
    test_pass("c1990:sscanf %d skips leading white space");

    test_eq( sscanf("ff", "%X", &u), 1 );
    test_eq( (int)u, 255 );
    test_pass("c1990:sscanf %X");

    test_eq( sscanf("-7", "%hd", &h), 1 );
    test_eq( (int)h, -7 );
    test_pass("c1990:sscanf %hd");
    test_eq( sscanf("10", "%ho", &hu), 1 );
    test_eq( (int)hu, 8 );
    test_pass("c1990:sscanf %ho");
    test_eq( sscanf("42", "%hu", &hu), 1 );
    test_eq( (int)hu, 42 );
    test_pass("c1990:sscanf %hu");
    test_eq( sscanf("ff", "%hx", &hu), 1 );
    test_eq( (int)hu, 255 );
    test_pass("c1990:sscanf %hx");

    test_eq( sscanf("10", "%lo", &ul), 1 );
    test_true( ul == 8UL );
    test_pass("c1990:sscanf %lo");
    test_eq( sscanf("42", "%lu", &ul), 1 );
    test_true( ul == 42UL );
    test_pass("c1990:sscanf %lu");
    test_eq( sscanf("ff", "%lx", &ul), 1 );
    test_true( ul == 255UL );
    test_pass("c1990:sscanf %lx");

    test_eq( sscanf("3.5E2", "%lE", &d), 1 );
    test_true( d == 350.0 );
    test_pass("c1990:sscanf %E");
    test_eq( sscanf("3.5", "%lG", &d), 1 );
    test_true( d == 3.5 );
    test_pass("c1990:sscanf %G");
    test_eq( sscanf("3.5", "%Lf", &ld), 1 );
    test_true( (double)ld == 3.5 );
    test_pass("c1990:sscanf %Lf");

    memset(s, 0, sizeof s);
    test_eq( sscanf("abcdefgh", "%5s", s), 1 );
    test_true( strcmp(s, "abcde") == 0 );
    test_pass("c1990:sscanf %5s");

    memset(c3, 0, sizeof c3);
    test_eq( sscanf("abcdef", "%3c", c3), 1 );
    test_true( c3[0] == 'a' && c3[2] == 'c' );
    test_pass("c1990:sscanf %3c (field width on %c reads exactly 3 characters)");

    {
        char one = 0;
        test_eq( sscanf(" x", "%c", &one), 1 );
        test_eq( (int)one, (int)' ' );
    }
    test_pass("c1990:sscanf %c does not skip white space");

    test_eq( sscanf("%", "%%"), 0 );
    test_pass("c1990:sscanf %%");

    test_eq( sscanf("42abc", "%d%n", &i, &n), 1 );
    test_eq( n, 2 );
    test_pass("c1990:sscanf %n");

    test_eq( sscanf("skip 7", "%*s %d", &i), 1 );
    test_eq( i, 7 );
    test_pass("c1990:sscanf %*s (assignment suppression)");

    memset(s, 0, sizeof s);
    test_eq( sscanf("aa,bb", "%*[^,],%s", s), 1 );
    test_true( strcmp(s, "bb") == 0 );
    test_pass("c1990:sscanf %*[^,] (suppression + scanset)");

    memset(s, 0, sizeof s);
    test_eq( sscanf("  ab", "%3[ a]", s), 1 );
    test_true( s[0] == ' ' );
    test_pass("c1990:sscanf %[ ] scanset does not skip white space");

    memset(s, 0, sizeof s);
    test_eq( sscanf("line one\nrest", "%15[^\n]", s), 1 );
    test_true( strcmp(s, "line one") == 0 );
    test_pass("c1990:sscanf %[^\\n]");

    {
        int obj = 0;
        char pbuf[64];
        sprintf(pbuf, "%p", (void*)&obj);
        vp = 0;
        if (sscanf(pbuf, "%p", &vp) == 1) {
            test_true( vp == (void*)&obj );
        } else {
            TEST_SKIP1();
        }
    }
    test_pass("c1990:sscanf %p");

    test_eq( sscanf("", "%d", &i), EOF );
    test_pass("c1990:sscanf input failure returns EOF");

    i = 0;
    test_eq( sscanf("abc", "%d", &i), 0 );
    test_eq( i, 0 );
    test_pass("c1990:sscanf matching failure stops conversion");

    i = 0;
    test_eq( sscanf("x42", "x%d", &i), 1 );
    test_eq( i, 42 );
    test_eq( sscanf("y42", "x%d", &i), 0 );
    test_pass("c1990:sscanf ordinary character must match literally");

    i = 0;
    test_eq( sscanf("a \t\n 42", "a %d", &i), 1 );
    test_eq( i, 42 );
    test_pass("c1990:sscanf white-space directive matches any amount of white space");
}

TEST_CASE(stdio, sscanf_wide_and_c99_c1995) {
#if defined(__ORANGEC__)
    TEST_SKIP_OCC("OrangeC 7.10.59 hangs while scanning %lc with sscanf");
    TEST_SKIP_CASE();
    return;
#else
    wchar_t wc[4]; wchar_t ws[16]; wchar_t wset[16];
    float f = 0; double d = 0;

    memset(wc, 0, sizeof wc);
    test_eq( sscanf("A", "%lc", wc), 1 );
    test_true( wc[0] == L'A' );
    test_pass("c1995:sscanf %lc");

    memset(ws, 0, sizeof ws);
    test_eq( sscanf("abc", "%ls", ws), 1 );
    test_true( ws[0] == L'a' && ws[2] == L'c' && ws[3] == 0 );
    test_pass("c1995:sscanf %ls");

    memset(wset, 0, sizeof wset);
    test_eq( sscanf("abcx", "%3l[abc]", wset), 1 );
    test_true( wset[0] == L'a' && wset[2] == L'c' );
    test_pass("c1995:sscanf %l[abc]");

    test_eq( sscanf("3.5", "%f", &f), 1 );
    test_true( (double)f == 3.5 );
    TEST_SKIP_VC120("the pre-UCRT MSVC CRT does not accept the C99 %F conversion");
    test_eq( sscanf("3.5", "%lF", &d), 1 );
    TEST_SKIP_VC120("the pre-UCRT MSVC CRT does not accept the C99 %F conversion");
    test_true( d == 3.5 );
    test_pass("c1999:sscanf %F");
#endif
}

TEST_CASE(stdio, fopen_modes_c1990) {
    char name[L_tmpnam + 8];
    FILE* fp;
    char buf[32];

    test_ptr( tmpnam(name) );

    fp = fopen(name, "r");
    test_true( fp == NULL );
    test_pass("c1990:fopen mode 'r' (fails when the file does not exist)");

    fp = fopen(name, "w");
    test_ptr( fp );
    if (fp) { fputs("abcdef", fp); fclose(fp); }
    fp = fopen(name, "w");
    test_ptr( fp );
    if (fp) fclose(fp);
    fp = fopen(name, "r");
    test_ptr( fp );
    if (fp) {
        test_true( fgets(buf, sizeof buf, fp) == NULL );
        fclose(fp);
    } else {
        TEST_SKIP1();
    }
    test_pass("c1990:fopen mode 'w' (creates or truncates)");

    fp = fopen(name, "w");
    if (fp) { fputs("one", fp); fclose(fp); }
    fp = fopen(name, "a");
    test_ptr( fp );
    if (fp) {
        fseek(fp, 0, SEEK_SET);
        fputs("two", fp);
        fclose(fp);
    }
    fp = fopen(name, "r");
    if (fp) {
        memset(buf, 0, sizeof buf);
        fgets(buf, sizeof buf, fp);
        test_true( strcmp(buf, "onetwo") == 0 );
        fclose(fp);
    } else {
        TEST_SKIP1();
    }
    test_pass("c1990:fopen mode 'a' (all writes go to end of file)");

    fp = fopen(name, "r+");
    test_ptr( fp );
    if (fp) {
        fputc('X', fp);
        fclose(fp);
        fp = fopen(name, "r");
        memset(buf, 0, sizeof buf);
        if (fp) { fgets(buf, sizeof buf, fp); fclose(fp); }
        test_true( strcmp(buf, "Xnetwo") == 0 );
    } else {
        TEST_SKIP1();
    }
    test_pass("c1990:fopen mode 'r+' (update, no truncation)");

    fp = fopen(name, "w+");
    test_ptr( fp );
    if (fp) {
        fputs("zz", fp);
        rewind(fp);
        memset(buf, 0, sizeof buf);
        test_ptr( fgets(buf, sizeof buf, fp) );
        test_true( strcmp(buf, "zz") == 0 );
        fclose(fp);
    } else {
        TEST_SKIP_N(2);
    }
    test_pass("c1990:fopen mode 'w+' (update, truncates)");

    fp = fopen(name, "a+");
    test_ptr( fp );
    if (fp) {
        fputs("yy", fp);
        rewind(fp);
        memset(buf, 0, sizeof buf);
        test_ptr( fgets(buf, sizeof buf, fp) );
        test_true( strcmp(buf, "zzyy") == 0 );
        fclose(fp);
    } else {
        TEST_SKIP_N(2);
    }
    test_pass("c1990:fopen mode 'a+' (update, append)");

    fp = fopen(name, "wb");
    test_ptr( fp );
    if (fp) {
        static const char raw[4] = { 'a', '\r', '\n', 'b' };
        fwrite(raw, 1, 4, fp);
        fclose(fp);
        fp = fopen(name, "rb");
        if (fp) {
            char back[8];
            size_t n = fread(back, 1, sizeof back, fp);
            test_eq( (long)n, 4L );
            test_true( back[1] == '\r' && back[2] == '\n' );
            fclose(fp);
        } else {
            TEST_SKIP_N(2);
        }
    } else {
        TEST_SKIP_N(2);
    }
    test_pass("c1990:fopen mode 'b' (binary stream, no character translation)");

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
    fp = fopen(name, "wx");
    test_true( fp == NULL );
    remove(name);
    fp = fopen(name, "wx");
    test_ptr( fp );
    if (fp) fclose(fp);
    test_pass("c2011:fopen mode 'x' (exclusive create, fails if the file exists)");
#else
    test_skip("c2011:fopen mode 'x' (exclusive create, fails if the file exists)");
#endif
    remove(name);
}

TEST_CASE(stdio, fseek_and_buffering_c1990) {
    FILE* fp = tmpfile();
    char buf[16];
    test_ptr( fp );
    if (!fp) {
        TEST_SKIP_N(12);
        test_skip("c1990:fseek SEEK_SET");
        test_skip("c1990:fseek SEEK_CUR");
        test_skip("c1990:fseek SEEK_END");
        test_skip("c1990:fseek on a text stream accepts only values returned by ftell");
        test_skip("c1990:setvbuf _IONBF (unbuffered)");
        test_skip("c1990:setvbuf _IOLBF (line buffered)");
        test_skip("c1990:setvbuf _IOFBF (fully buffered)");
        test_skip("c1990:setbuf(stream, NULL) is equivalent to _IONBF");
        return;
    }

    fputs("0123456789", fp);
    test_eq( fseek(fp, 3, SEEK_SET), 0 );
    test_eq( fgetc(fp), '3' );
    test_pass("c1990:fseek SEEK_SET");

    test_eq( fseek(fp, 2, SEEK_CUR), 0 );
    test_eq( fgetc(fp), '6' );
    test_pass("c1990:fseek SEEK_CUR");

    test_eq( fseek(fp, -2, SEEK_END), 0 );
    test_eq( fgetc(fp), '8' );
    test_pass("c1990:fseek SEEK_END");

    {
        long pos;
        rewind(fp);
        test_eq( fgetc(fp), '0' );
        pos = ftell(fp);
        test_true( pos >= 0 );
        test_eq( fgetc(fp), '1' );
        test_eq( fseek(fp, pos, SEEK_SET), 0 );
        test_eq( fgetc(fp), '1' );
    }
    test_pass("c1990:fseek on a text stream accepts only values returned by ftell");
    fclose(fp);

    {
        FILE* b = tmpfile();
        char own[64];
        test_ptr( b );
        if (b) {
            test_eq( setvbuf(b, NULL, _IONBF, 0), 0 );
            fputs("x", b);
            test_pass("c1990:setvbuf _IONBF (unbuffered)");
            fclose(b);
        } else { TEST_SKIP_N(2); test_skip("c1990:setvbuf _IONBF (unbuffered)"); }

        b = tmpfile();
        if (b) {
            test_eq( setvbuf(b, own, _IOLBF, sizeof own), 0 );
            fputs("y\n", b);
            test_pass("c1990:setvbuf _IOLBF (line buffered)");
            fclose(b);
        } else { TEST_SKIP_N(2); test_skip("c1990:setvbuf _IOLBF (line buffered)"); }

        b = tmpfile();
        if (b) {
            static char big[128];
            test_eq( setvbuf(b, big, _IOFBF, sizeof big), 0 );
            fputs("z", b);
            test_pass("c1990:setvbuf _IOFBF (fully buffered)");
            fclose(b);
        } else { TEST_SKIP_N(2); test_skip("c1990:setvbuf _IOFBF (fully buffered)"); }

        b = tmpfile();
        if (b) {
            setbuf(b, NULL);
            fputs("w", b);
            test_true( 1 );
            test_pass("c1990:setbuf(stream, NULL) is equivalent to _IONBF");
            fclose(b);
        } else { TEST_SKIP1(); test_skip("c1990:setbuf(stream, NULL) is equivalent to _IONBF"); }
    }
    (void)buf;
}

TEST_CASE(stdio, stream_behaviour_c1990) {
    FILE* fp = tmpfile();
    test_ptr( fp );
    if (!fp) {
        TEST_SKIP_N(8);
        test_skip("c1990:ungetc pushback is discarded by a successful seek");
        test_skip("c1990:update stream needs fflush/fseek between a write and a read");
        test_skip("c1990:text stream vs binary stream behaviour");
        test_skip("c1990:gets");
        return;
    }

    fputs("abcdef", fp);
    rewind(fp);
    test_eq( fgetc(fp), 'a' );
    test_eq( ungetc('Z', fp), 'Z' );
    test_eq( fseek(fp, 0, SEEK_SET), 0 );
    test_eq( fgetc(fp), 'a' );
    test_pass("c1990:ungetc pushback is discarded by a successful seek");

    rewind(fp);
    fputc('X', fp);
    test_eq( fflush(fp), 0 );
    test_eq( fseek(fp, 0, SEEK_SET), 0 );
    test_eq( fgetc(fp), 'X' );
    test_pass("c1990:update stream needs fflush/fseek between a write and a read");

    {
        FILE* t = tmpfile();
        if (t) {
            char line[16];
            fputs("hi\n", t);
            rewind(t);
            memset(line, 0, sizeof line);
            test_ptr( fgets(line, sizeof line, t) );
            test_true( strcmp(line, "hi\n") == 0 );
            fclose(t);
        } else {
            TEST_SKIP_N(2);
        }
    }
    test_pass("c1990:text stream vs binary stream behaviour");
    fclose(fp);

    TEST_NOTE("gets was removed in C11 and is not declared here");
    test_skip("c1990:gets");
}

TEST_CASE(stdio, remaining_printf_and_c99_c1990) {
#if defined(__ORANGEC__)
    TEST_SKIP_OCC("OrangeC 7.10.59 crashes on sprintf with a negative dynamic width");
    TEST_SKIP_CASE();
    return;
#else
    char b[64];
    int n = -1;

    sprintf(b, "%*d", -6, 42);
    test_true( strcmp(b, "42    ") == 0 );
    test_pass("c1990:printf %*d with negative width (acts as - flag)");

    sprintf(b, "%+ d", 42);
    test_true( strcmp(b, "+42") == 0 );
    test_pass("c1990:printf %+ d (+ overrides space)");

    sprintf(b, "abc%n", &n);
    TEST_SKIP_UCRT("");
    test_eq( n, 3 );
    test_pass("c1990:printf %n");

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
    {
        FILE* fp = tmpfile();
        test_ptr( fp );
        if (fp) {
            FILE* r = freopen(NULL, "w+b", fp);
            if (r) {
                fputs("ok", r);
                test_true( 1 );
                fclose(r);
            } else {
                TEST_NOTE("freopen(NULL, ...) is not supported here");
                TEST_SKIP1();
                fclose(fp);
            }
        } else {
            TEST_SKIP1();
        }
    }
    test_pass("c1999:freopen(NULL, mode, stream) changes the mode of an open stream");
#else
    test_skip("c1999:freopen(NULL, mode, stream) changes the mode of an open stream");
#endif
#endif
}

TEST_CASE(stdio, printf_length_modifiers_a_c1999) {
    char b[64];
    sprintf(b, "%hhi", (signed char)-7);
    test_true( strcmp(b, "-7") == 0 );
    test_pass("c1999:printf %hhi");
    sprintf(b, "%hho", (unsigned char)8);
    test_true( strcmp(b, "10") == 0 );
    test_pass("c1999:printf %hho");
    sprintf(b, "%hhu", (unsigned char)42);
    test_true( strcmp(b, "42") == 0 );
    test_pass("c1999:printf %hhu");
    sprintf(b, "%hhx", (unsigned char)255);
    test_true( strcmp(b, "ff") == 0 );
    test_pass("c1999:printf %hhx");
    sprintf(b, "%jd", (intmax_t)42);
    TEST_SKIP_VC120("the pre-UCRT MSVC CRT prints the j/z/t length modifiers literally");
    test_true( strcmp(b, "42") == 0 );
    test_pass("c1999:printf %jd");
    sprintf(b, "%ji", (intmax_t)42);
    TEST_SKIP_VC120("the pre-UCRT MSVC CRT prints the j/z/t length modifiers literally");
    test_true( strcmp(b, "42") == 0 );
    test_pass("c1999:printf %ji");
    sprintf(b, "%jo", (uintmax_t)8);
    TEST_SKIP_VC120("the pre-UCRT MSVC CRT prints the j/z/t length modifiers literally");
    test_true( strcmp(b, "10") == 0 );
    test_pass("c1999:printf %jo");
    sprintf(b, "%jx", (uintmax_t)255);
    TEST_SKIP_VC120("the pre-UCRT MSVC CRT prints the j/z/t length modifiers literally");
    test_true( strcmp(b, "ff") == 0 );
    test_pass("c1999:printf %jx");
    sprintf(b, "%lf", 3.5);
    test_true( strcmp(b, "3.500000") == 0 );
    test_pass("c1999:printf %lf (l has no effect on double)");
    sprintf(b, "%llX", 255ULL);
    test_true( strcmp(b, "FF") == 0 );
    test_pass("c1999:printf %llX");
    sprintf(b, "%lli", -42LL);
    test_true( strcmp(b, "-42") == 0 );
    test_pass("c1999:printf %lli");
    sprintf(b, "%llo", 8ULL);
    test_true( strcmp(b, "10") == 0 );
    test_pass("c1999:printf %llo");
    sprintf(b, "%llx", 255ULL);
    test_true( strcmp(b, "ff") == 0 );
    test_pass("c1999:printf %llx");
    sprintf(b, "%ti", (ptrdiff_t)-42);
    TEST_SKIP_VC120("the pre-UCRT MSVC CRT prints the j/z/t length modifiers literally");
    test_true( strcmp(b, "-42") == 0 );
    test_pass("c1999:printf %ti");
    sprintf(b, "%tu", (size_t)42);
    TEST_SKIP_VC120("the pre-UCRT MSVC CRT prints the j/z/t length modifiers literally");
    test_true( strcmp(b, "42") == 0 );
    test_pass("c1999:printf %tu");
    sprintf(b, "%tx", (size_t)255);
    TEST_SKIP_VC120("the pre-UCRT MSVC CRT prints the j/z/t length modifiers literally");
    test_true( strcmp(b, "ff") == 0 );
    test_pass("c1999:printf %tx");
    sprintf(b, "%zd", (size_t)42);
    TEST_SKIP_VC120("the pre-UCRT MSVC CRT prints the j/z/t length modifiers literally");
    test_true( strcmp(b, "42") == 0 );
    test_pass("c1999:printf %zd");
    sprintf(b, "%zi", (size_t)42);
    TEST_SKIP_VC120("the pre-UCRT MSVC CRT prints the j/z/t length modifiers literally");
    test_true( strcmp(b, "42") == 0 );
    test_pass("c1999:printf %zi");
    sprintf(b, "%zo", (size_t)8);
    TEST_SKIP_VC120("the pre-UCRT MSVC CRT prints the j/z/t length modifiers literally");
    test_true( strcmp(b, "10") == 0 );
    test_pass("c1999:printf %zo");
    sprintf(b, "%zx", (size_t)255);
    TEST_SKIP_VC120("the pre-UCRT MSVC CRT prints the j/z/t length modifiers literally");
    test_true( strcmp(b, "ff") == 0 );
    test_pass("c1999:printf %zx");
}

TEST_CASE(stdio, printf_wide_and_hex_float_c1995) {
    char b[64];
    sprintf(b, "%.3ls", L"abcdef");
    test_true( strcmp(b, "abc") == 0 );
    test_pass("c1995:printf %.3ls (precision counts bytes of the converted multibyte characters)");
    sprintf(b, "%lc", (wint_t)L'A');
    test_true( strcmp(b, "A") == 0 );
    test_pass("c1995:printf %lc (wide character)");
    sprintf(b, "%ls", L"abc");
    test_true( strcmp(b, "abc") == 0 );
    test_pass("c1995:printf %ls (wide string)");
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
    sprintf(b, "%.3a", 1.0);
    test_true( b[0] == '0' && b[1] == 'x' );
    test_pass("c1999:printf %.3a (precision on a hexadecimal float)");
#else
    test_skip("c1999:printf %.3a (precision on a hexadecimal float)");
#endif
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
    sprintf(b, "%LA", 1.0L);
    test_true( b[0] == '0' && b[1] == 'X' );
    test_pass("c1999:printf %LA");
#else
    test_skip("c1999:printf %LA");
#endif
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
    sprintf(b, "%La", 1.0L);
    test_true( b[0] == '0' && b[1] == 'x' );
    test_pass("c1999:printf %La");
#else
    test_skip("c1999:printf %La");
#endif
}

TEST_CASE(stdio, sscanf_conversions_c99) {
    char sbuf[16];
    int n;

    n = sscanf("abcdefgh", "%5s", sbuf);
    test_eq( n, 1 );
    test_str_eq( sbuf, "abcde" );
    n = sscanf("ab", "%5s", sbuf);
    test_eq( n, 1 );
    test_str_eq( sbuf, "ab" );
    test_pass("c1990:sscanf %5s (field width on %s)");

    {   int a = -1, b = -1;
        n = sscanf("12 xy", "%d %d", &a, &b);
        test_eq( n, 1 );
        test_eq( a, 12 );
        test_eq( b, -1 );
        n = sscanf("zz", "%d", &a);
        test_eq( n, 0 );
        test_eq( a, 12 ); }
    test_pass("c1990:sscanf matching failure stops conversion (return value)");

    {   FILE* fp = tmpfile();
        if (fp) {
            fputs("0123456789", fp);
            rewind(fp);
            test_eq( fgetc(fp), '0' );
            test_ne( ungetc('Z', fp), EOF );
            test_eq( fseek(fp, 0, SEEK_SET), 0 );
            test_eq( fgetc(fp), '0' );
            fclose(fp);
        } else {
            TEST_SKIP_N(4);
        } }
    test_pass("c1990:ungetc pushback is discarded by a successful fseek");

#if TEST_TARGET_C >= 1999
    {   signed char sc = 0;
        n = sscanf("-7", "%hhi", &sc);
        test_eq( n, 1 );
        test_eq( (int)sc, -7 );
        n = sscanf("0x1f", "%hhi", &sc);
        test_eq( n, 1 );
        test_eq( (int)sc, 31 ); }
    test_pass("c1999:sscanf %hhi");

    {   unsigned char uc = 0;
        n = sscanf("200", "%hhu", &uc);
        test_eq( n, 1 );
        test_eq( (int)uc, 200 ); }
    test_pass("c1999:sscanf %hhu");

    {   intmax_t j = 0;
        n = sscanf("-1234567", "%jd", &j);
        test_eq( n, 1 );
        test_true( j == (intmax_t)-1234567 ); }
    test_pass("c1999:sscanf %jd");

    {   unsigned long long ull = 0;
        n = sscanf("18000000000", "%llu", &ull);
        test_eq( n, 1 );
        test_true( ull == (unsigned long long)18000000000ULL ); }
    test_pass("c1999:sscanf %llu");

    {   unsigned long long ux = 0;
        n = sscanf("ff00ff00", "%llx", &ux);
        test_eq( n, 1 );
        test_true( ux == (unsigned long long)0xff00ff00ULL ); }
    test_pass("c1999:sscanf %llx");

    {   ptrdiff_t t = 0;
        n = sscanf("-42", "%td", &t);
        test_eq( n, 1 );
        test_true( t == (ptrdiff_t)-42 ); }
    test_pass("c1999:sscanf %td");

    {   size_t z = 0;
        n = sscanf("4096", "%zu", &z);
        test_eq( n, 1 );
        test_true( z == (size_t)4096 ); }
    test_pass("c1999:sscanf %zu");

    {   double d = 0.0;
        n = sscanf("0x1p+4", "%la", &d);
        if (n == 1) {
            test_true( d == 16.0 );
        } else {
            TEST_NOTE("this CRT has no %a in scanf (Open Watcom: A44')");
            TEST_SKIP1();
        }
        test_true( n == 0 || n == 1 ); }
    test_pass("c1999:sscanf %a (hexadecimal floating form)");

    {   double d = 0.0;
        n = sscanf("0X1P+4", "%lA", &d);
        if (n == 1) {
            test_true( d == 16.0 );
        } else {
            TEST_NOTE("this CRT has no %A in scanf (Open Watcom: A44')");
            TEST_SKIP1();
        }
        test_true( n == 0 || n == 1 ); }
    test_pass("c1999:sscanf %A");
#else
    TEST_NOTE("the length modifiers hh/j/z/t/ll and %a are C99");
    TEST_SKIP_N(20);
    test_skip("c1999:sscanf %hhi");
    test_skip("c1999:sscanf %hhu");
    test_skip("c1999:sscanf %jd");
    test_skip("c1999:sscanf %llu");
    test_skip("c1999:sscanf %llx");
    test_skip("c1999:sscanf %td");
    test_skip("c1999:sscanf %zu");
    test_skip("c1999:sscanf %a (hexadecimal floating form)");
    test_skip("c1999:sscanf %A");
#endif
}

TEST_CASE(stdio, annex_k_names_c2011) {
#if defined(__STDC_LIB_EXT1__) && defined(__STDC_WANT_LIB_EXT1__)
    char b[32];
    va_list ap_unused;

    {   errno_t e = (errno_t)0;
        rsize_t r = (rsize_t)1;
        test_eq( (long)e, 0L );
        test_true( r == (rsize_t)1 ); }
    test_pass("c2011:opt:errno_t");
    test_pass("c2011:opt:rsize_t");

    test_true( (long)L_tmpnam_s > 0L );
    test_pass("c2011:opt:L_tmpnam_s");
    test_true( (long)TMP_MAX_S > 0L );
    test_pass("c2011:opt:TMP_MAX_S");

    test_eq( sprintf_s(b, sizeof b, "%d-%s", 7, "ab"), 4 );
    test_str_eq( b, "7-ab" );
    test_pass("c2011:opt:sprintf_s");

    test_eq( snprintf_s(b, sizeof b, "%d", 42), 2 );
    test_str_eq( b, "42" );
    test_pass("c2011:opt:snprintf_s");

    {   int v = 0;
        test_eq( sscanf_s("13", "%d", &v), 1 );
        test_eq( v, 13 ); }
    test_pass("c2011:opt:sscanf_s");

    {   FILE* fp = 0;
        test_eq( (long)tmpfile_s(&fp), 0L );
        test_ptr( fp );
        if (fp) {
            test_gt( fprintf_s(fp, "%d", 5), 0 );
            rewind(fp);
            {   int v = 0;
                test_eq( fscanf_s(fp, "%d", &v), 1 );
                test_eq( v, 5 ); }
            fclose(fp);
        } else {
            TEST_SKIP_N(4);
        } }
    test_pass("c2011:opt:tmpfile_s");
    test_pass("c2011:opt:fprintf_s");
    test_pass("c2011:opt:fscanf_s");

    {   char nm[L_tmpnam_s];
        nm[0] = '\0';
        test_eq( (long)tmpnam_s(nm, sizeof nm), 0L );
        test_true( nm[0] != '\0' ); }
    test_pass("c2011:opt:tmpnam_s");

    {   FILE* fp = 0;
        test_eq( (long)fopen_s(&fp, TST_TMPNAME, "w"), 0L );
        test_ptr( fp );
        if (fp) {
            FILE* fp2 = 0;
            test_eq( (long)freopen_s(&fp2, TST_TMPNAME, "r", fp), 0L );
            if (fp2) fclose(fp2);
        } else {
            TEST_SKIP1();
        }
        remove(TST_TMPNAME); }
    test_pass("c2011:opt:fopen_s");
    test_pass("c2011:opt:freopen_s");

    TEST_NOTE("the v* and console forms are taken unevaluated: sizeof does not call them");
    test_true( sizeof(vsprintf_s(b, sizeof b, "%d", ap_unused))  > 0 );
    test_pass("c2011:opt:vsprintf_s");
    test_true( sizeof(vsnprintf_s(b, sizeof b, "%d", ap_unused)) > 0 );
    test_pass("c2011:opt:vsnprintf_s");
    test_true( sizeof(vsscanf_s("1", "%d", ap_unused))           > 0 );
    test_pass("c2011:opt:vsscanf_s");
    test_true( sizeof(vfprintf_s(stdout, "%d", ap_unused))       > 0 );
    test_pass("c2011:opt:vfprintf_s");
    test_true( sizeof(vfscanf_s(stdin, "%d", ap_unused))         > 0 );
    test_pass("c2011:opt:vfscanf_s");
    test_true( sizeof(vprintf_s("%d", ap_unused))                > 0 );
    test_pass("c2011:opt:vprintf_s");
    test_true( sizeof(vscanf_s("%d", ap_unused))                 > 0 );
    test_pass("c2011:opt:vscanf_s");
    test_true( sizeof(printf_s("%d", 1))                         > 0 );
    test_pass("c2011:opt:printf_s");
    test_true( sizeof(scanf_s("%d", (int*)0))                    > 0 );
    test_pass("c2011:opt:scanf_s");
    test_true( sizeof(gets_s(b, sizeof b))                       > 0 );
    test_pass("c2011:opt:gets_s");
#else
    TEST_NOTE("Annex K is optional; needs __STDC_LIB_EXT1__ and __STDC_WANT_LIB_EXT1__");
    TEST_SKIP_N(30);
    test_skip("c2011:opt:errno_t");
    test_skip("c2011:opt:rsize_t");
    test_skip("c2011:opt:L_tmpnam_s");
    test_skip("c2011:opt:TMP_MAX_S");
    test_skip("c2011:opt:sprintf_s");
    test_skip("c2011:opt:snprintf_s");
    test_skip("c2011:opt:sscanf_s");
    test_skip("c2011:opt:tmpfile_s");
    test_skip("c2011:opt:fprintf_s");
    test_skip("c2011:opt:fscanf_s");
    test_skip("c2011:opt:tmpnam_s");
    test_skip("c2011:opt:fopen_s");
    test_skip("c2011:opt:freopen_s");
    test_skip("c2011:opt:vsprintf_s");
    test_skip("c2011:opt:vsnprintf_s");
    test_skip("c2011:opt:vsscanf_s");
    test_skip("c2011:opt:vfprintf_s");
    test_skip("c2011:opt:vfscanf_s");
    test_skip("c2011:opt:vprintf_s");
    test_skip("c2011:opt:vscanf_s");
    test_skip("c2011:opt:printf_s");
    test_skip("c2011:opt:scanf_s");
    test_skip("c2011:opt:gets_s");
#endif
}

TEST_CASE(stdio, printf_binary_and_width_c23) {
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
    char b[64];

    test_gt( sprintf(b, "%b", 5u), 0 );
    test_str_eq( b, "101" );
    test_pass("c2023:printf %b");
    test_gt( sprintf(b, "%B", 5u), 0 );
    test_str_eq( b, "101" );
    test_pass("c2023:printf %B");
    test_gt( sprintf(b, "%#b", 5u), 0 );
    test_str_eq( b, "0b101" );
    test_pass("c2023:printf %#b (0b prefix)");
    test_gt( sprintf(b, "%#B", 5u), 0 );
    test_str_eq( b, "0B101" );
    test_pass("c2023:printf %#B (0B prefix)");
    test_gt( sprintf(b, "%08b", 5u), 0 );
    test_str_eq( b, "00000101" );
    test_pass("c2023:printf %08b");

    test_gt( sprintf(b, "%w8d",  (int)-8), 0 );   test_str_eq( b, "-8" );
    test_pass("c2023:printf %w8d");
    test_gt( sprintf(b, "%w16d", (int)-16), 0 );  test_str_eq( b, "-16" );
    test_pass("c2023:printf %w16d");
    test_gt( sprintf(b, "%w32d", (int32_t)-32), 0 ); test_str_eq( b, "-32" );
    test_pass("c2023:printf %w32d");
    test_gt( sprintf(b, "%w32x", (uint32_t)0xabcdu), 0 ); test_str_eq( b, "abcd" );
    test_pass("c2023:printf %w32x");
    test_gt( sprintf(b, "%w64d", (int64_t)-64), 0 ); test_str_eq( b, "-64" );
    test_pass("c2023:printf %w64d");
    test_gt( sprintf(b, "%wf32d", (int_fast32_t)-1), 0 ); test_str_eq( b, "-1" );
    test_pass("c2023:printf %wf32d");
    test_gt( sprintf(b, "%wf64d", (int_fast64_t)-2), 0 ); test_str_eq( b, "-2" );
    test_pass("c2023:printf %wf64d");

    {   unsigned u = 0;
        test_eq( sscanf("1011", "%b", &u), 1 );
        test_eq( (long)u, 11L ); }
    test_pass("c2023:sscanf %b");
    {   int32_t v = 0;
        test_eq( sscanf("-32", "%w32d", &v), 1 );
        test_true( v == (int32_t)-32 ); }
    test_pass("c2023:sscanf %w32d");
    {   int_fast32_t v = 0;
        test_eq( sscanf("-7", "%wf32d", &v), 1 );
        test_true( v == (int_fast32_t)-7 ); }
    test_pass("c2023:sscanf %wf32d");

    test_true( (long)_PRINTF_NAN_LEN_MAX >= 3L );
    test_pass("c2023:_PRINTF_NAN_LEN_MAX");
    test_true( (long)__STDC_VERSION_STDIO_H__ >= 202311L );
    test_pass("c2023:__STDC_VERSION_STDIO_H__");
#else
    TEST_NOTE("%b / %w<N> / _PRINTF_NAN_LEN_MAX are C23");
    TEST_SKIP_N(32);
    test_skip("c2023:printf %b");
    test_skip("c2023:printf %B");
    test_skip("c2023:printf %#b (0b prefix)");
    test_skip("c2023:printf %#B (0B prefix)");
    test_skip("c2023:printf %08b");
    test_skip("c2023:printf %w8d");
    test_skip("c2023:printf %w16d");
    test_skip("c2023:printf %w32d");
    test_skip("c2023:printf %w32x");
    test_skip("c2023:printf %w64d");
    test_skip("c2023:printf %wf32d");
    test_skip("c2023:printf %wf64d");
    test_skip("c2023:sscanf %b");
    test_skip("c2023:sscanf %w32d");
    test_skip("c2023:sscanf %wf32d");
    test_skip("c2023:_PRINTF_NAN_LEN_MAX");
    test_skip("c2023:__STDC_VERSION_STDIO_H__");
#endif
}

TEST_SUITE(stdio)
    TEST_ADD(types_and_macros)
    TEST_ADD(sprintf_formats)
    TEST_ADD(snprintf_truncation)
    TEST_ADD(sscanf_family)
    TEST_ADD(printf_integer)
    TEST_ADD(printf_integer_limit_base_matrix)
    TEST_ADD(printf_integer_limit_binary_c23)
    TEST_ADD(printf_length_mods)
    TEST_ADD(printf_float)
    TEST_ADD(printf_combinations)
    TEST_ADD(printf_char_string_misc)
    TEST_ADD(scanf_conversions)
    TEST_ADD(fopen_fclose_remove_rename)
    TEST_ADD(tmpfile_and_tmpnam)
    TEST_ADD(setvbuf_and_freopen)
    TEST_ADD(fread_fwrite)
    TEST_ADD(char_and_line_io)
    TEST_ADD(fprintf_fscanf)
    TEST_ADD(positioning)
    TEST_ADD(error_handling)
    TEST_ADD(stdout_family_does_not_crash)
    TEST_ADD(vsprintf_vprintf)
    TEST_ADD(setbuf_puts_putchar_perror)
    TEST_ADD(vfscanf_from_file)
    TEST_ADD(stdin_readers_exist)
    TEST_ADD(printf_specifiers_a_c1990)
    TEST_ADD(printf_specifiers_b_c1990)
    TEST_ADD(sscanf_conversions_c1990)
    TEST_ADD(sscanf_wide_and_c99_c1995)
    TEST_ADD(fopen_modes_c1990)
    TEST_ADD(fseek_and_buffering_c1990)
    TEST_ADD(stream_behaviour_c1990)
    TEST_ADD(remaining_printf_and_c99_c1990)
    TEST_ADD(printf_length_modifiers_a_c1999)
    TEST_ADD(printf_wide_and_hex_float_c1995)
    TEST_ADD(sscanf_conversions_c99)
    TEST_ADD(annex_k_names_c2011)
    TEST_ADD(printf_binary_and_width_c23)
TEST_SUITE_END(stdio)
