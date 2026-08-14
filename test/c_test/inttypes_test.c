#include "c_test.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include <limits.h>

static void ascii_to_wide(wchar_t* dst, const char* src) {
    while ((*dst++ = (wchar_t)(unsigned char)*src++) != L'\0') { }
}

#define CHK_PRI(ST, UT, Pd, Pi, Po, Pu, Px, PX)                         \
    do {                                                                \
        char b[64];                                                     \
        ST sv = (ST)-42;                                                \
        UT uv = (UT)0xABu;                                              \
        snprintf(b, sizeof b, "%" Pd, sv); test_str_eq(b, "-42"); test_pass("c1999:" #Pd); \
        snprintf(b, sizeof b, "%" Pi, sv); test_str_eq(b, "-42"); test_pass("c1999:" #Pi); \
        snprintf(b, sizeof b, "%" Po, uv); test_str_eq(b, "253"); test_pass("c1999:" #Po); \
        snprintf(b, sizeof b, "%" Pu, uv); test_str_eq(b, "171"); test_pass("c1999:" #Pu); \
        snprintf(b, sizeof b, "%" Px, uv); test_str_eq(b, "ab");  test_pass("c1999:" #Px); \
        snprintf(b, sizeof b, "%" PX, uv); test_str_eq(b, "AB");  test_pass("c1999:" #PX); \
    } while (0)

#define CHK_SCN(ST, UT, Sd, Si, So, Su, Sx)                             \
    do {                                                                \
        ST sd = 0, si = 0;                                              \
        UT so = 0, su = 0, sx = 0;                                      \
        sscanf("-42", "%" Sd, &sd); test_eq((long long)sd, -42LL); test_pass("c1999:" #Sd); \
        sscanf("-42", "%" Si, &si); test_eq((long long)si, -42LL); test_pass("c1999:" #Si); \
        sscanf("253", "%" So, &so); test_eq((unsigned long long)so, 171ULL); test_pass("c1999:" #So); \
        sscanf("171", "%" Su, &su); test_eq((unsigned long long)su, 171ULL); test_pass("c1999:" #Su); \
        sscanf("ab",  "%" Sx, &sx); test_eq((unsigned long long)sx, 171ULL); test_pass("c1999:" #Sx); \
    } while (0)

TEST_CASE(inttypes, imaxdiv_quot_rem) {
    imaxdiv_t d = imaxdiv((intmax_t)17, (intmax_t)5);
    imaxdiv_t a, b, c, w, cp;

    test_eq( (long long)d.quot, 3LL );
    test_eq( (long long)d.rem,  2LL );

    a = imaxdiv((intmax_t)-17, (intmax_t)5);
    test_eq( (long long)a.quot, -3LL );
    test_eq( (long long)a.rem,  -2LL );
    test_eq( (long long)(a.quot * 5 + a.rem), -17LL );

    b = imaxdiv((intmax_t)17, (intmax_t)-5);
    test_eq( (long long)b.quot, -3LL );
    test_eq( (long long)b.rem,   2LL );

    c = imaxdiv((intmax_t)-17, (intmax_t)-5);
    test_eq( (long long)c.quot, 3LL );
    test_eq( (long long)c.rem, -2LL );

    w = imaxdiv((intmax_t)9223372036854775807LL, (intmax_t)1000);
    test_eq( (long long)w.quot, 9223372036854775LL );
    test_eq( (long long)w.rem,  807LL );

    w = imaxdiv(INTMAX_MAX, (intmax_t)1);
    test_true( w.quot == INTMAX_MAX && w.rem == 0 );
    w = imaxdiv(-INTMAX_MAX, (intmax_t)-1);
    test_true( w.quot == INTMAX_MAX && w.rem == 0 );
    w = imaxdiv((intmax_t)0, INTMAX_MAX);
    test_true( w.quot == 0 && w.rem == 0 );

    test_pass("c1999:imaxdiv");
    cp = d;
    test_eq( (long long)cp.quot, 3LL );
    test_eq( (long long)cp.rem, 2LL );
    test_pass("c1999:imaxdiv_t");
}

TEST_CASE(inttypes, imaxabs_value) {
    test_eq( (long long)imaxabs((intmax_t)0),  0LL );
    test_eq( (long long)imaxabs((intmax_t)7),  7LL );
    test_eq( (long long)imaxabs((intmax_t)-7), 7LL );
    test_eq( (long long)imaxabs((intmax_t)-1234567890123LL), 1234567890123LL );
    test_eq( (long long)imaxabs(INTMAX_MAX), (long long)INTMAX_MAX );
    test_pass("c1999:imaxabs");
}

TEST_CASE(inttypes, strtoimax_strtoumax) {
    char* end = 0;
    intmax_t v, hv, bv;
    uintmax_t u;
    char* uend = 0;
    char maxbuf[3 * sizeof(intmax_t) * CHAR_BIT + 8];
    char minbuf[3 * sizeof(intmax_t) * CHAR_BIT + 8];
    char umaxbuf[3 * sizeof(uintmax_t) * CHAR_BIT + 8];
    size_t n;

    v = strtoimax("  -12345xyz", &end, 10);
    test_eq( (long long)v, -12345LL );
    test_str_eq( end, "xyz" );

    end = 0;
    hv = strtoimax("0x1Fzz", &end, 16);
    test_eq( (long long)hv, 31LL );
    test_eq( (long long)strtoimax("7f", 0, 16), 127LL );

    test_eq( (long long)strtoimax("0755", 0, 0), 493LL );
    test_eq( (long long)strtoimax("0X10", 0, 0), 16LL );
    test_eq( (long long)strtoimax("-99", 0, 0), -99LL );
    test_eq( (long long)strtoimax("9223372036854775807", 0, 10), (long long)INTMAX_MAX );

    end = 0;
    bv = strtoimax("zz", &end, 10);
    test_eq( (long long)bv, 0LL );

    sprintf(maxbuf, "%" PRIdMAX, INTMAX_MAX);
    errno = 0;
    test_true( strtoimax(maxbuf, &end, 10) == INTMAX_MAX );
    test_eq( errno, 0 );
    test_eq( (int)*end, 0 );
    n = strlen(maxbuf); maxbuf[n] = '0'; maxbuf[n + 1] = '\0';
    errno = 0;
    test_true( strtoimax(maxbuf, &end, 10) == INTMAX_MAX );
    test_eq( errno, ERANGE );
    test_eq( (int)*end, 0 );

    sprintf(minbuf, "%" PRIdMAX, INTMAX_MIN);
    n = strlen(minbuf); minbuf[n] = '0'; minbuf[n + 1] = '\0';
    errno = 0;
    test_true( strtoimax(minbuf, &end, 10) == INTMAX_MIN );
    test_eq( errno, ERANGE );
    test_eq( (int)*end, 0 );
    test_pass("c1999:strtoimax");

    u = strtoumax("18446744073709551615", &uend, 10);
    test_eq( (unsigned long long)u, (unsigned long long)UINTMAX_MAX );
    test_str_eq( uend, "" );
    test_eq( (unsigned long long)strtoumax("ff", 0, 16), 255ULL );
    test_eq( (unsigned long long)strtoumax("101", 0, 2), 5ULL );
    sprintf(umaxbuf, "%" PRIuMAX, UINTMAX_MAX);
    errno = 0;
    test_true( strtoumax(umaxbuf, &uend, 10) == UINTMAX_MAX );
    test_eq( errno, 0 );
    n = strlen(umaxbuf); umaxbuf[n] = '0'; umaxbuf[n + 1] = '\0';
    errno = 0;
    test_true( strtoumax(umaxbuf, &uend, 10) == UINTMAX_MAX );
    test_eq( errno, ERANGE );
    test_eq( (int)*uend, 0 );
    test_pass("c1999:strtoumax");
}

TEST_CASE(inttypes, wcstoimax_wcstoumax) {
    wchar_t* end = 0;
    intmax_t v, bv;
    uintmax_t u;
    wchar_t* uend = 0;
    char narrow[3 * sizeof(uintmax_t) * CHAR_BIT + 8];
    wchar_t wide[3 * sizeof(uintmax_t) * CHAR_BIT + 8];
    size_t n;

    v = wcstoimax(L"  -987654321abc", &end, 10);
    test_eq( (long long)v, -987654321LL );
    test_true( end != 0 && *end == L'a' );

    test_eq( (long long)wcstoimax(L"7f", 0, 16), 127LL );
    test_eq( (long long)wcstoimax(L"0755", 0, 0), 493LL );
    test_eq( (long long)wcstoimax(L"9223372036854775807", 0, 10), (long long)INTMAX_MAX );

    end = 0;
    bv = wcstoimax(L"zz", &end, 10);
    test_eq( (long long)bv, 0LL );
    sprintf(narrow, "%" PRIdMAX, INTMAX_MAX);
    n = strlen(narrow); narrow[n] = '0'; narrow[n + 1] = '\0';
    ascii_to_wide(wide, narrow);
    errno = 0;
    test_true( wcstoimax(wide, &end, 10) == INTMAX_MAX );
    test_eq( errno, ERANGE );
    test_eq( (int)*end, 0 );
    test_pass("c1999:wcstoimax");

    u = wcstoumax(L"18446744073709551615", &uend, 10);
    test_eq( (unsigned long long)u, (unsigned long long)UINTMAX_MAX );
    test_true( uend != 0 && *uend == L'\0' );
    test_eq( (unsigned long long)wcstoumax(L"ff", 0, 16), 255ULL );
    sprintf(narrow, "%" PRIuMAX, UINTMAX_MAX);
    n = strlen(narrow); narrow[n] = '0'; narrow[n + 1] = '\0';
    ascii_to_wide(wide, narrow);
    errno = 0;
    test_true( wcstoumax(wide, &uend, 10) == UINTMAX_MAX );
    test_eq( errno, ERANGE );
    test_eq( (int)*uend, 0 );
    test_pass("c1999:wcstoumax");
}

TEST_CASE(inttypes, pri_exact_width) {
    char b[64];
    uint64_t big = 12345678901234567890ULL;
    int64_t  neg = -9223372036854775807LL - 1;

    CHK_PRI(int8_t,  uint8_t,  PRId8,  PRIi8,  PRIo8,  PRIu8,  PRIx8,  PRIX8);
    CHK_PRI(int16_t, uint16_t, PRId16, PRIi16, PRIo16, PRIu16, PRIx16, PRIX16);
    CHK_PRI(int32_t, uint32_t, PRId32, PRIi32, PRIo32, PRIu32, PRIx32, PRIX32);
    CHK_PRI(int64_t, uint64_t, PRId64, PRIi64, PRIo64, PRIu64, PRIx64, PRIX64);

    snprintf(b, sizeof b, "%05" PRId32, (int32_t)5);
    test_str_eq( b, "00005" );
    snprintf(b, sizeof b, "%" PRIu64, big);
    test_str_eq( b, "12345678901234567890" );
    snprintf(b, sizeof b, "%" PRId64, neg);
    test_str_eq( b, "-9223372036854775808" );
}

TEST_CASE(inttypes, pri_least_fast) {
    CHK_PRI(int_least8_t,  uint_least8_t,
            PRIdLEAST8,  PRIiLEAST8,  PRIoLEAST8,  PRIuLEAST8,  PRIxLEAST8,  PRIXLEAST8);
    CHK_PRI(int_least32_t, uint_least32_t,
            PRIdLEAST32, PRIiLEAST32, PRIoLEAST32, PRIuLEAST32, PRIxLEAST32, PRIXLEAST32);
    CHK_PRI(int_fast8_t,  uint_fast8_t,
            PRIdFAST8,  PRIiFAST8,  PRIoFAST8,  PRIuFAST8,  PRIxFAST8,  PRIXFAST8);
    CHK_PRI(int_fast32_t, uint_fast32_t,
            PRIdFAST32, PRIiFAST32, PRIoFAST32, PRIuFAST32, PRIxFAST32, PRIXFAST32);
}

TEST_CASE(inttypes, pri_max_and_ptr) {
    char b[64];
    intmax_t im = INTMAX_MAX;

    CHK_PRI(intmax_t, uintmax_t, PRIdMAX, PRIiMAX, PRIoMAX, PRIuMAX, PRIxMAX, PRIXMAX);
    CHK_PRI(intptr_t, uintptr_t, PRIdPTR, PRIiPTR, PRIoPTR, PRIuPTR, PRIxPTR, PRIXPTR);

    snprintf(b, sizeof b, "%" PRIdMAX, im);
    test_str_eq( b, "9223372036854775807" );
}

TEST_CASE(inttypes, scn_exact_width) {
    int32_t hex = 0, oct = 0, dec = 0;
    uint64_t u64 = 0;

#if defined(_MSC_VER) && _MSC_VER < 1900
    TEST_SKIP1(); TEST_NOTE("%hh not supported by pre-UCRT MSVC CRT"); test_skip("c1999:SCNd8");
    TEST_SKIP1(); test_skip("c1999:SCNi8");
    TEST_SKIP1(); test_skip("c1999:SCNo8");
    TEST_SKIP1(); test_skip("c1999:SCNu8");
    TEST_SKIP1(); test_skip("c1999:SCNx8");
#else
    CHK_SCN(int8_t,  uint8_t,  SCNd8,  SCNi8,  SCNo8,  SCNu8,  SCNx8);
#endif
    CHK_SCN(int16_t, uint16_t, SCNd16, SCNi16, SCNo16, SCNu16, SCNx16);
    CHK_SCN(int32_t, uint32_t, SCNd32, SCNi32, SCNo32, SCNu32, SCNx32);
    CHK_SCN(int64_t, uint64_t, SCNd64, SCNi64, SCNo64, SCNu64, SCNx64);

    sscanf("0x1f", "%" SCNi32, &hex); test_eq( (long long)hex, 31LL );
    sscanf("0755", "%" SCNi32, &oct); test_eq( (long long)oct, 493LL );
    sscanf("0755", "%" SCNd32, &dec); test_eq( (long long)dec, 755LL );
    sscanf("18446744073709551615", "%" SCNu64, &u64);
    test_eq( (unsigned long long)u64, 18446744073709551615ULL );
}

TEST_CASE(inttypes, scn_max_and_ptr) {
    intmax_t  im = INTMAX_MIN, imback = 0;
    uintmax_t um = UINTMAX_MAX, umback = 0;
    char b[64];

    CHK_SCN(intmax_t, uintmax_t, SCNdMAX, SCNiMAX, SCNoMAX, SCNuMAX, SCNxMAX);
    CHK_SCN(intptr_t, uintptr_t, SCNdPTR, SCNiPTR, SCNoPTR, SCNuPTR, SCNxPTR);

    snprintf(b, sizeof b, "%" PRIdMAX, im);
    sscanf(b, "%" SCNdMAX, &imback);
    test_eq( (long long)imback, (long long)INTMAX_MIN );

    snprintf(b, sizeof b, "%" PRIuMAX, um);
    sscanf(b, "%" SCNuMAX, &umback);
    test_eq( (unsigned long long)umback, (unsigned long long)UINTMAX_MAX );
}

TEST_CASE(inttypes, format_macros_fast16_c1999) {
    char buf[64];
#if defined(PRIXFAST16)
    sprintf(buf, "%" PRIXFAST16, (uint_fast16_t)42);
    test_true( strcmp(buf, "2A") == 0 );
    test_pass("c1999:PRIXFAST16");
#else
    test_skip("c1999:PRIXFAST16");
#endif
#if defined(PRIdFAST16)
    sprintf(buf, "%" PRIdFAST16, (int_fast16_t)42);
    test_true( strcmp(buf, "42") == 0 );
    test_pass("c1999:PRIdFAST16");
#else
    test_skip("c1999:PRIdFAST16");
#endif
#if defined(PRIiFAST16)
    sprintf(buf, "%" PRIiFAST16, (int_fast16_t)42);
    test_true( strcmp(buf, "42") == 0 );
    test_pass("c1999:PRIiFAST16");
#else
    test_skip("c1999:PRIiFAST16");
#endif
#if defined(PRIoFAST16)
    sprintf(buf, "%" PRIoFAST16, (uint_fast16_t)42);
    test_true( strcmp(buf, "52") == 0 );
    test_pass("c1999:PRIoFAST16");
#else
    test_skip("c1999:PRIoFAST16");
#endif
#if defined(PRIuFAST16)
    sprintf(buf, "%" PRIuFAST16, (uint_fast16_t)42);
    test_true( strcmp(buf, "42") == 0 );
    test_pass("c1999:PRIuFAST16");
#else
    test_skip("c1999:PRIuFAST16");
#endif
#if defined(PRIxFAST16)
    sprintf(buf, "%" PRIxFAST16, (uint_fast16_t)42);
    test_true( strcmp(buf, "2a") == 0 );
    test_pass("c1999:PRIxFAST16");
#else
    test_skip("c1999:PRIxFAST16");
#endif
#if defined(SCNdFAST16)
    { int_fast16_t v_ = 0;
      test_eq( sscanf("42", "%" SCNdFAST16, &v_), 1 );
      test_true( (long long)v_ == 42LL ); }
    test_pass("c1999:SCNdFAST16");
#else
    test_skip("c1999:SCNdFAST16");
#endif
#if defined(SCNiFAST16)
    { int_fast16_t v_ = 0;
      test_eq( sscanf("42", "%" SCNiFAST16, &v_), 1 );
      test_true( (long long)v_ == 42LL ); }
    test_pass("c1999:SCNiFAST16");
#else
    test_skip("c1999:SCNiFAST16");
#endif
#if defined(SCNoFAST16)
    { uint_fast16_t v_ = 0;
      test_eq( sscanf("52", "%" SCNoFAST16, &v_), 1 );
      test_true( (long long)v_ == 42LL ); }
    test_pass("c1999:SCNoFAST16");
#else
    test_skip("c1999:SCNoFAST16");
#endif
#if defined(SCNuFAST16)
    { uint_fast16_t v_ = 0;
      test_eq( sscanf("42", "%" SCNuFAST16, &v_), 1 );
      test_true( (long long)v_ == 42LL ); }
    test_pass("c1999:SCNuFAST16");
#else
    test_skip("c1999:SCNuFAST16");
#endif
#if defined(SCNxFAST16)
    { uint_fast16_t v_ = 0;
      test_eq( sscanf("2a", "%" SCNxFAST16, &v_), 1 );
      test_true( (long long)v_ == 42LL ); }
    test_pass("c1999:SCNxFAST16");
#else
    test_skip("c1999:SCNxFAST16");
#endif
#if defined(PRIBFAST16)
    sprintf(buf, "%" PRIBFAST16, (uint_fast16_t)5);
    test_true( strcmp(buf, "101") == 0 );
    test_pass("c2023:PRIBFAST16");
#else
    test_skip("c2023:PRIBFAST16");
#endif
#if defined(PRIbFAST16)
    sprintf(buf, "%" PRIbFAST16, (uint_fast16_t)5);
    test_true( strcmp(buf, "101") == 0 );
    test_pass("c2023:PRIbFAST16");
#else
    test_skip("c2023:PRIbFAST16");
#endif
#if defined(SCNbFAST16)
    { uint_fast16_t v_ = 0;
      test_eq( sscanf("101", "%" SCNbFAST16, &v_), 1 );
      test_true( (long long)v_ == 5LL ); }
    test_pass("c2023:SCNbFAST16");
#else
    test_skip("c2023:SCNbFAST16");
#endif
#if defined(PRIXFAST64)
    sprintf(buf, "%" PRIXFAST64, (uint_fast64_t)42);
    test_true( strcmp(buf, "2A") == 0 );
    test_pass("c1999:PRIXFAST64");
#else
    test_skip("c1999:PRIXFAST64");
#endif
#if defined(PRIdFAST64)
    sprintf(buf, "%" PRIdFAST64, (int_fast64_t)42);
    test_true( strcmp(buf, "42") == 0 );
    test_pass("c1999:PRIdFAST64");
#else
    test_skip("c1999:PRIdFAST64");
#endif
#if defined(PRIiFAST64)
    sprintf(buf, "%" PRIiFAST64, (int_fast64_t)42);
    test_true( strcmp(buf, "42") == 0 );
    test_pass("c1999:PRIiFAST64");
#else
    test_skip("c1999:PRIiFAST64");
#endif
#if defined(PRIoFAST64)
    sprintf(buf, "%" PRIoFAST64, (uint_fast64_t)42);
    test_true( strcmp(buf, "52") == 0 );
    test_pass("c1999:PRIoFAST64");
#else
    test_skip("c1999:PRIoFAST64");
#endif
#if defined(PRIuFAST64)
    sprintf(buf, "%" PRIuFAST64, (uint_fast64_t)42);
    test_true( strcmp(buf, "42") == 0 );
    test_pass("c1999:PRIuFAST64");
#else
    test_skip("c1999:PRIuFAST64");
#endif
#if defined(PRIxFAST64)
    sprintf(buf, "%" PRIxFAST64, (uint_fast64_t)42);
    test_true( strcmp(buf, "2a") == 0 );
    test_pass("c1999:PRIxFAST64");
#else
    test_skip("c1999:PRIxFAST64");
#endif
#if defined(SCNdFAST64)
    { int_fast64_t v_ = 0;
      test_eq( sscanf("42", "%" SCNdFAST64, &v_), 1 );
      test_true( (long long)v_ == 42LL ); }
    test_pass("c1999:SCNdFAST64");
#else
    test_skip("c1999:SCNdFAST64");
#endif
#if defined(SCNiFAST64)
    { int_fast64_t v_ = 0;
      test_eq( sscanf("42", "%" SCNiFAST64, &v_), 1 );
      test_true( (long long)v_ == 42LL ); }
    test_pass("c1999:SCNiFAST64");
#else
    test_skip("c1999:SCNiFAST64");
#endif
#if defined(SCNoFAST64)
    { uint_fast64_t v_ = 0;
      test_eq( sscanf("52", "%" SCNoFAST64, &v_), 1 );
      test_true( (long long)v_ == 42LL ); }
    test_pass("c1999:SCNoFAST64");
#else
    test_skip("c1999:SCNoFAST64");
#endif
#if defined(SCNuFAST64)
    { uint_fast64_t v_ = 0;
      test_eq( sscanf("42", "%" SCNuFAST64, &v_), 1 );
      test_true( (long long)v_ == 42LL ); }
    test_pass("c1999:SCNuFAST64");
#else
    test_skip("c1999:SCNuFAST64");
#endif
#if defined(SCNxFAST64)
    { uint_fast64_t v_ = 0;
      test_eq( sscanf("2a", "%" SCNxFAST64, &v_), 1 );
      test_true( (long long)v_ == 42LL ); }
    test_pass("c1999:SCNxFAST64");
#else
    test_skip("c1999:SCNxFAST64");
#endif
#if defined(PRIBFAST64)
    sprintf(buf, "%" PRIBFAST64, (uint_fast64_t)5);
    test_true( strcmp(buf, "101") == 0 );
    test_pass("c2023:PRIBFAST64");
#else
    test_skip("c2023:PRIBFAST64");
#endif
#if defined(PRIbFAST64)
    sprintf(buf, "%" PRIbFAST64, (uint_fast64_t)5);
    test_true( strcmp(buf, "101") == 0 );
    test_pass("c2023:PRIbFAST64");
#else
    test_skip("c2023:PRIbFAST64");
#endif
#if defined(SCNbFAST64)
    { uint_fast64_t v_ = 0;
      test_eq( sscanf("101", "%" SCNbFAST64, &v_), 1 );
      test_true( (long long)v_ == 5LL ); }
    test_pass("c2023:SCNbFAST64");
#else
    test_skip("c2023:SCNbFAST64");
#endif
#if defined(PRIXLEAST16)
    sprintf(buf, "%" PRIXLEAST16, (uint_least16_t)42);
    test_true( strcmp(buf, "2A") == 0 );
    test_pass("c1999:PRIXLEAST16");
#else
    test_skip("c1999:PRIXLEAST16");
#endif
#if defined(PRIdLEAST16)
    sprintf(buf, "%" PRIdLEAST16, (int_least16_t)42);
    test_true( strcmp(buf, "42") == 0 );
    test_pass("c1999:PRIdLEAST16");
#else
    test_skip("c1999:PRIdLEAST16");
#endif
#if defined(PRIiLEAST16)
    sprintf(buf, "%" PRIiLEAST16, (int_least16_t)42);
    test_true( strcmp(buf, "42") == 0 );
    test_pass("c1999:PRIiLEAST16");
#else
    test_skip("c1999:PRIiLEAST16");
#endif
#if defined(PRIoLEAST16)
    sprintf(buf, "%" PRIoLEAST16, (uint_least16_t)42);
    test_true( strcmp(buf, "52") == 0 );
    test_pass("c1999:PRIoLEAST16");
#else
    test_skip("c1999:PRIoLEAST16");
#endif
#if defined(PRIuLEAST16)
    sprintf(buf, "%" PRIuLEAST16, (uint_least16_t)42);
    test_true( strcmp(buf, "42") == 0 );
    test_pass("c1999:PRIuLEAST16");
#else
    test_skip("c1999:PRIuLEAST16");
#endif
#if defined(PRIxLEAST16)
    sprintf(buf, "%" PRIxLEAST16, (uint_least16_t)42);
    test_true( strcmp(buf, "2a") == 0 );
    test_pass("c1999:PRIxLEAST16");
#else
    test_skip("c1999:PRIxLEAST16");
#endif
#if defined(SCNdLEAST16)
    { int_least16_t v_ = 0;
      test_eq( sscanf("42", "%" SCNdLEAST16, &v_), 1 );
      test_true( (long long)v_ == 42LL ); }
    test_pass("c1999:SCNdLEAST16");
#else
    test_skip("c1999:SCNdLEAST16");
#endif
#if defined(SCNiLEAST16)
    { int_least16_t v_ = 0;
      test_eq( sscanf("42", "%" SCNiLEAST16, &v_), 1 );
      test_true( (long long)v_ == 42LL ); }
    test_pass("c1999:SCNiLEAST16");
#else
    test_skip("c1999:SCNiLEAST16");
#endif
#if defined(SCNoLEAST16)
    { uint_least16_t v_ = 0;
      test_eq( sscanf("52", "%" SCNoLEAST16, &v_), 1 );
      test_true( (long long)v_ == 42LL ); }
    test_pass("c1999:SCNoLEAST16");
#else
    test_skip("c1999:SCNoLEAST16");
#endif
#if defined(SCNuLEAST16)
    { uint_least16_t v_ = 0;
      test_eq( sscanf("42", "%" SCNuLEAST16, &v_), 1 );
      test_true( (long long)v_ == 42LL ); }
    test_pass("c1999:SCNuLEAST16");
#else
    test_skip("c1999:SCNuLEAST16");
#endif
#if defined(SCNxLEAST16)
    { uint_least16_t v_ = 0;
      test_eq( sscanf("2a", "%" SCNxLEAST16, &v_), 1 );
      test_true( (long long)v_ == 42LL ); }
    test_pass("c1999:SCNxLEAST16");
#else
    test_skip("c1999:SCNxLEAST16");
#endif
#if defined(PRIBLEAST16)
    sprintf(buf, "%" PRIBLEAST16, (uint_least16_t)5);
    test_true( strcmp(buf, "101") == 0 );
    test_pass("c2023:PRIBLEAST16");
#else
    test_skip("c2023:PRIBLEAST16");
#endif
#if defined(PRIbLEAST16)
    sprintf(buf, "%" PRIbLEAST16, (uint_least16_t)5);
    test_true( strcmp(buf, "101") == 0 );
    test_pass("c2023:PRIbLEAST16");
#else
    test_skip("c2023:PRIbLEAST16");
#endif
#if defined(SCNbLEAST16)
    { uint_least16_t v_ = 0;
      test_eq( sscanf("101", "%" SCNbLEAST16, &v_), 1 );
      test_true( (long long)v_ == 5LL ); }
    test_pass("c2023:SCNbLEAST16");
#else
    test_skip("c2023:SCNbLEAST16");
#endif
#if defined(PRIXLEAST64)
    sprintf(buf, "%" PRIXLEAST64, (uint_least64_t)42);
    test_true( strcmp(buf, "2A") == 0 );
    test_pass("c1999:PRIXLEAST64");
#else
    test_skip("c1999:PRIXLEAST64");
#endif
#if defined(PRIdLEAST64)
    sprintf(buf, "%" PRIdLEAST64, (int_least64_t)42);
    test_true( strcmp(buf, "42") == 0 );
    test_pass("c1999:PRIdLEAST64");
#else
    test_skip("c1999:PRIdLEAST64");
#endif
#if defined(PRIiLEAST64)
    sprintf(buf, "%" PRIiLEAST64, (int_least64_t)42);
    test_true( strcmp(buf, "42") == 0 );
    test_pass("c1999:PRIiLEAST64");
#else
    test_skip("c1999:PRIiLEAST64");
#endif
#if defined(PRIoLEAST64)
    sprintf(buf, "%" PRIoLEAST64, (uint_least64_t)42);
    test_true( strcmp(buf, "52") == 0 );
    test_pass("c1999:PRIoLEAST64");
#else
    test_skip("c1999:PRIoLEAST64");
#endif
#if defined(PRIuLEAST64)
    sprintf(buf, "%" PRIuLEAST64, (uint_least64_t)42);
    test_true( strcmp(buf, "42") == 0 );
    test_pass("c1999:PRIuLEAST64");
#else
    test_skip("c1999:PRIuLEAST64");
#endif
#if defined(PRIxLEAST64)
    sprintf(buf, "%" PRIxLEAST64, (uint_least64_t)42);
    test_true( strcmp(buf, "2a") == 0 );
    test_pass("c1999:PRIxLEAST64");
#else
    test_skip("c1999:PRIxLEAST64");
#endif
#if defined(SCNdLEAST64)
    { int_least64_t v_ = 0;
      test_eq( sscanf("42", "%" SCNdLEAST64, &v_), 1 );
      test_true( (long long)v_ == 42LL ); }
    test_pass("c1999:SCNdLEAST64");
#else
    test_skip("c1999:SCNdLEAST64");
#endif
#if defined(SCNiLEAST64)
    { int_least64_t v_ = 0;
      test_eq( sscanf("42", "%" SCNiLEAST64, &v_), 1 );
      test_true( (long long)v_ == 42LL ); }
    test_pass("c1999:SCNiLEAST64");
#else
    test_skip("c1999:SCNiLEAST64");
#endif
#if defined(SCNoLEAST64)
    { uint_least64_t v_ = 0;
      test_eq( sscanf("52", "%" SCNoLEAST64, &v_), 1 );
      test_true( (long long)v_ == 42LL ); }
    test_pass("c1999:SCNoLEAST64");
#else
    test_skip("c1999:SCNoLEAST64");
#endif
#if defined(SCNuLEAST64)
    { uint_least64_t v_ = 0;
      test_eq( sscanf("42", "%" SCNuLEAST64, &v_), 1 );
      test_true( (long long)v_ == 42LL ); }
    test_pass("c1999:SCNuLEAST64");
#else
    test_skip("c1999:SCNuLEAST64");
#endif
#if defined(SCNxLEAST64)
    { uint_least64_t v_ = 0;
      test_eq( sscanf("2a", "%" SCNxLEAST64, &v_), 1 );
      test_true( (long long)v_ == 42LL ); }
    test_pass("c1999:SCNxLEAST64");
#else
    test_skip("c1999:SCNxLEAST64");
#endif
#if defined(PRIBLEAST64)
    sprintf(buf, "%" PRIBLEAST64, (uint_least64_t)5);
    test_true( strcmp(buf, "101") == 0 );
    test_pass("c2023:PRIBLEAST64");
#else
    test_skip("c2023:PRIBLEAST64");
#endif
#if defined(PRIbLEAST64)
    sprintf(buf, "%" PRIbLEAST64, (uint_least64_t)5);
    test_true( strcmp(buf, "101") == 0 );
    test_pass("c2023:PRIbLEAST64");
#else
    test_skip("c2023:PRIbLEAST64");
#endif
#if defined(SCNbLEAST64)
    { uint_least64_t v_ = 0;
      test_eq( sscanf("101", "%" SCNbLEAST64, &v_), 1 );
      test_true( (long long)v_ == 5LL ); }
    test_pass("c2023:SCNbLEAST64");
#else
    test_skip("c2023:SCNbLEAST64");
#endif
}

TEST_CASE(inttypes, format_macros_fast32_c1999) {
    char buf[64];
#if defined(SCNdFAST32)
    { int_fast32_t v_ = 0;
      test_eq( sscanf("42", "%" SCNdFAST32, &v_), 1 );
      test_true( (long long)v_ == 42LL ); }
    test_pass("c1999:SCNdFAST32");
#else
    test_skip("c1999:SCNdFAST32");
#endif
#if defined(SCNiFAST32)
    { int_fast32_t v_ = 0;
      test_eq( sscanf("42", "%" SCNiFAST32, &v_), 1 );
      test_true( (long long)v_ == 42LL ); }
    test_pass("c1999:SCNiFAST32");
#else
    test_skip("c1999:SCNiFAST32");
#endif
#if defined(SCNoFAST32)
    { uint_fast32_t v_ = 0;
      test_eq( sscanf("52", "%" SCNoFAST32, &v_), 1 );
      test_true( (long long)v_ == 42LL ); }
    test_pass("c1999:SCNoFAST32");
#else
    test_skip("c1999:SCNoFAST32");
#endif
#if defined(SCNuFAST32)
    { uint_fast32_t v_ = 0;
      test_eq( sscanf("42", "%" SCNuFAST32, &v_), 1 );
      test_true( (long long)v_ == 42LL ); }
    test_pass("c1999:SCNuFAST32");
#else
    test_skip("c1999:SCNuFAST32");
#endif
#if defined(SCNxFAST32)
    { uint_fast32_t v_ = 0;
      test_eq( sscanf("2a", "%" SCNxFAST32, &v_), 1 );
      test_true( (long long)v_ == 42LL ); }
    test_pass("c1999:SCNxFAST32");
#else
    test_skip("c1999:SCNxFAST32");
#endif
#if defined(PRIBFAST32)
    sprintf(buf, "%" PRIBFAST32, (uint_fast32_t)5);
    test_true( strcmp(buf, "101") == 0 );
    test_pass("c2023:PRIBFAST32");
#else
    test_skip("c2023:PRIBFAST32");
#endif
#if defined(PRIbFAST32)
    sprintf(buf, "%" PRIbFAST32, (uint_fast32_t)5);
    test_true( strcmp(buf, "101") == 0 );
    test_pass("c2023:PRIbFAST32");
#else
    test_skip("c2023:PRIbFAST32");
#endif
#if defined(SCNbFAST32)
    { uint_fast32_t v_ = 0;
      test_eq( sscanf("101", "%" SCNbFAST32, &v_), 1 );
      test_true( (long long)v_ == 5LL ); }
    test_pass("c2023:SCNbFAST32");
#else
    test_skip("c2023:SCNbFAST32");
#endif
#if defined(SCNdFAST8)
    { union { int_fast8_t v; long long pad; } u_; u_.pad = 0;
      test_eq( sscanf("42", "%" SCNdFAST8, &u_.v), 1 );
      test_true( (long long)u_.v == 42LL ); }
    test_pass("c1999:SCNdFAST8");
#else
    test_skip("c1999:SCNdFAST8");
#endif
#if defined(SCNiFAST8)
    { union { int_fast8_t v; long long pad; } u_; u_.pad = 0;
      test_eq( sscanf("42", "%" SCNiFAST8, &u_.v), 1 );
      test_true( (long long)u_.v == 42LL ); }
    test_pass("c1999:SCNiFAST8");
#else
    test_skip("c1999:SCNiFAST8");
#endif
#if defined(SCNoFAST8)
    { union { uint_fast8_t v; long long pad; } u_; u_.pad = 0;
      test_eq( sscanf("52", "%" SCNoFAST8, &u_.v), 1 );
      test_true( (long long)u_.v == 42LL ); }
    test_pass("c1999:SCNoFAST8");
#else
    test_skip("c1999:SCNoFAST8");
#endif
#if defined(SCNuFAST8)
    { union { uint_fast8_t v; long long pad; } u_; u_.pad = 0;
      test_eq( sscanf("42", "%" SCNuFAST8, &u_.v), 1 );
      test_true( (long long)u_.v == 42LL ); }
    test_pass("c1999:SCNuFAST8");
#else
    test_skip("c1999:SCNuFAST8");
#endif
#if defined(SCNxFAST8)
    { union { uint_fast8_t v; long long pad; } u_; u_.pad = 0;
      test_eq( sscanf("2a", "%" SCNxFAST8, &u_.v), 1 );
      test_true( (long long)u_.v == 42LL ); }
    test_pass("c1999:SCNxFAST8");
#else
    test_skip("c1999:SCNxFAST8");
#endif
#if defined(PRIBFAST8)
    sprintf(buf, "%" PRIBFAST8, (uint_fast8_t)5);
    test_true( strcmp(buf, "101") == 0 );
    test_pass("c2023:PRIBFAST8");
#else
    test_skip("c2023:PRIBFAST8");
#endif
#if defined(PRIbFAST8)
    sprintf(buf, "%" PRIbFAST8, (uint_fast8_t)5);
    test_true( strcmp(buf, "101") == 0 );
    test_pass("c2023:PRIbFAST8");
#else
    test_skip("c2023:PRIbFAST8");
#endif
#if defined(SCNbFAST8)
    { union { uint_fast8_t v; long long pad; } u_; u_.pad = 0;
      test_eq( sscanf("101", "%" SCNbFAST8, &u_.v), 1 );
      test_true( (long long)u_.v == 5LL ); }
    test_pass("c2023:SCNbFAST8");
#else
    test_skip("c2023:SCNbFAST8");
#endif
#if defined(SCNdLEAST32)
    { int_least32_t v_ = 0;
      test_eq( sscanf("42", "%" SCNdLEAST32, &v_), 1 );
      test_true( (long long)v_ == 42LL ); }
    test_pass("c1999:SCNdLEAST32");
#else
    test_skip("c1999:SCNdLEAST32");
#endif
#if defined(SCNiLEAST32)
    { int_least32_t v_ = 0;
      test_eq( sscanf("42", "%" SCNiLEAST32, &v_), 1 );
      test_true( (long long)v_ == 42LL ); }
    test_pass("c1999:SCNiLEAST32");
#else
    test_skip("c1999:SCNiLEAST32");
#endif
#if defined(SCNoLEAST32)
    { uint_least32_t v_ = 0;
      test_eq( sscanf("52", "%" SCNoLEAST32, &v_), 1 );
      test_true( (long long)v_ == 42LL ); }
    test_pass("c1999:SCNoLEAST32");
#else
    test_skip("c1999:SCNoLEAST32");
#endif
#if defined(SCNuLEAST32)
    { uint_least32_t v_ = 0;
      test_eq( sscanf("42", "%" SCNuLEAST32, &v_), 1 );
      test_true( (long long)v_ == 42LL ); }
    test_pass("c1999:SCNuLEAST32");
#else
    test_skip("c1999:SCNuLEAST32");
#endif
#if defined(SCNxLEAST32)
    { uint_least32_t v_ = 0;
      test_eq( sscanf("2a", "%" SCNxLEAST32, &v_), 1 );
      test_true( (long long)v_ == 42LL ); }
    test_pass("c1999:SCNxLEAST32");
#else
    test_skip("c1999:SCNxLEAST32");
#endif
#if defined(PRIBLEAST32)
    sprintf(buf, "%" PRIBLEAST32, (uint_least32_t)5);
    test_true( strcmp(buf, "101") == 0 );
    test_pass("c2023:PRIBLEAST32");
#else
    test_skip("c2023:PRIBLEAST32");
#endif
#if defined(PRIbLEAST32)
    sprintf(buf, "%" PRIbLEAST32, (uint_least32_t)5);
    test_true( strcmp(buf, "101") == 0 );
    test_pass("c2023:PRIbLEAST32");
#else
    test_skip("c2023:PRIbLEAST32");
#endif
#if defined(SCNbLEAST32)
    { uint_least32_t v_ = 0;
      test_eq( sscanf("101", "%" SCNbLEAST32, &v_), 1 );
      test_true( (long long)v_ == 5LL ); }
    test_pass("c2023:SCNbLEAST32");
#else
    test_skip("c2023:SCNbLEAST32");
#endif
#if defined(SCNdLEAST8)
    { union { int_least8_t v; long long pad; } u_; u_.pad = 0;
      test_eq( sscanf("42", "%" SCNdLEAST8, &u_.v), 1 );
      test_true( (long long)u_.v == 42LL ); }
    test_pass("c1999:SCNdLEAST8");
#else
    test_skip("c1999:SCNdLEAST8");
#endif
#if defined(SCNiLEAST8)
    { union { int_least8_t v; long long pad; } u_; u_.pad = 0;
      test_eq( sscanf("42", "%" SCNiLEAST8, &u_.v), 1 );
      test_true( (long long)u_.v == 42LL ); }
    test_pass("c1999:SCNiLEAST8");
#else
    test_skip("c1999:SCNiLEAST8");
#endif
#if defined(SCNoLEAST8)
    { union { uint_least8_t v; long long pad; } u_; u_.pad = 0;
      test_eq( sscanf("52", "%" SCNoLEAST8, &u_.v), 1 );
      test_true( (long long)u_.v == 42LL ); }
    test_pass("c1999:SCNoLEAST8");
#else
    test_skip("c1999:SCNoLEAST8");
#endif
#if defined(SCNuLEAST8)
    { union { uint_least8_t v; long long pad; } u_; u_.pad = 0;
      test_eq( sscanf("42", "%" SCNuLEAST8, &u_.v), 1 );
      test_true( (long long)u_.v == 42LL ); }
    test_pass("c1999:SCNuLEAST8");
#else
    test_skip("c1999:SCNuLEAST8");
#endif
#if defined(SCNxLEAST8)
    { union { uint_least8_t v; long long pad; } u_; u_.pad = 0;
      test_eq( sscanf("2a", "%" SCNxLEAST8, &u_.v), 1 );
      test_true( (long long)u_.v == 42LL ); }
    test_pass("c1999:SCNxLEAST8");
#else
    test_skip("c1999:SCNxLEAST8");
#endif
#if defined(PRIBLEAST8)
    sprintf(buf, "%" PRIBLEAST8, (uint_least8_t)5);
    test_true( strcmp(buf, "101") == 0 );
    test_pass("c2023:PRIBLEAST8");
#else
    test_skip("c2023:PRIBLEAST8");
#endif
#if defined(PRIbLEAST8)
    sprintf(buf, "%" PRIbLEAST8, (uint_least8_t)5);
    test_true( strcmp(buf, "101") == 0 );
    test_pass("c2023:PRIbLEAST8");
#else
    test_skip("c2023:PRIbLEAST8");
#endif
#if defined(SCNbLEAST8)
    { union { uint_least8_t v; long long pad; } u_; u_.pad = 0;
      test_eq( sscanf("101", "%" SCNbLEAST8, &u_.v), 1 );
      test_true( (long long)u_.v == 5LL ); }
    test_pass("c2023:SCNbLEAST8");
#else
    test_skip("c2023:SCNbLEAST8");
#endif
}

TEST_CASE(inttypes, format_macros_exact16_c1999) {
    char buf[64];
#if defined(PRIB16)
    sprintf(buf, "%" PRIB16, (uint16_t)5);
    test_true( strcmp(buf, "101") == 0 );
    test_pass("c2023:PRIB16");
#else
    test_skip("c2023:PRIB16");
#endif
#if defined(PRIb16)
    sprintf(buf, "%" PRIb16, (uint16_t)5);
    test_true( strcmp(buf, "101") == 0 );
    test_pass("c2023:PRIb16");
#else
    test_skip("c2023:PRIb16");
#endif
#if defined(SCNb16)
    { uint16_t v_ = 0;
      test_eq( sscanf("101", "%" SCNb16, &v_), 1 );
      test_true( (long long)v_ == 5LL ); }
    test_pass("c2023:SCNb16");
#else
    test_skip("c2023:SCNb16");
#endif
#if defined(PRIB32)
    sprintf(buf, "%" PRIB32, (uint32_t)5);
    test_true( strcmp(buf, "101") == 0 );
    test_pass("c2023:PRIB32");
#else
    test_skip("c2023:PRIB32");
#endif
#if defined(PRIb32)
    sprintf(buf, "%" PRIb32, (uint32_t)5);
    test_true( strcmp(buf, "101") == 0 );
    test_pass("c2023:PRIb32");
#else
    test_skip("c2023:PRIb32");
#endif
#if defined(SCNb32)
    { uint32_t v_ = 0;
      test_eq( sscanf("101", "%" SCNb32, &v_), 1 );
      test_true( (long long)v_ == 5LL ); }
    test_pass("c2023:SCNb32");
#else
    test_skip("c2023:SCNb32");
#endif
#if defined(PRIB64)
    sprintf(buf, "%" PRIB64, (uint64_t)5);
    test_true( strcmp(buf, "101") == 0 );
    test_pass("c2023:PRIB64");
#else
    test_skip("c2023:PRIB64");
#endif
#if defined(PRIb64)
    sprintf(buf, "%" PRIb64, (uint64_t)5);
    test_true( strcmp(buf, "101") == 0 );
    test_pass("c2023:PRIb64");
#else
    test_skip("c2023:PRIb64");
#endif
#if defined(SCNb64)
    { uint64_t v_ = 0;
      test_eq( sscanf("101", "%" SCNb64, &v_), 1 );
      test_true( (long long)v_ == 5LL ); }
    test_pass("c2023:SCNb64");
#else
    test_skip("c2023:SCNb64");
#endif
#if defined(PRIB8)
    sprintf(buf, "%" PRIB8, (uint8_t)5);
    test_true( strcmp(buf, "101") == 0 );
    test_pass("c2023:PRIB8");
#else
    test_skip("c2023:PRIB8");
#endif
#if defined(PRIb8)
    sprintf(buf, "%" PRIb8, (uint8_t)5);
    test_true( strcmp(buf, "101") == 0 );
    test_pass("c2023:PRIb8");
#else
    test_skip("c2023:PRIb8");
#endif
#if defined(SCNb8)
    { uint8_t v_ = 0;
      test_eq( sscanf("101", "%" SCNb8, &v_), 1 );
      test_true( (long long)v_ == 5LL ); }
    test_pass("c2023:SCNb8");
#else
    test_skip("c2023:SCNb8");
#endif
}

TEST_CASE(inttypes, format_macros_max_ptr_c2023) {
    char buf[64];
#if defined(PRIbMAX)
    sprintf(buf, "%" PRIbMAX, (uintmax_t)5);
    test_true( strcmp(buf, "101") == 0 );
    test_pass("c2023:PRIbMAX");
    sprintf(buf, "%" PRIBMAX, (uintmax_t)5);
    test_true( strcmp(buf, "101") == 0 );
    test_pass("c2023:PRIBMAX");
    { uintmax_t v_ = 0;
      test_eq( sscanf("101", "%" SCNbMAX, &v_), 1 );
      test_true( (long long)v_ == 5LL ); }
    test_pass("c2023:SCNbMAX");
#else
    TEST_NOTE("the binary conversion macros are C23");
    test_skip("c2023:PRIbMAX");
    test_skip("c2023:PRIBMAX");
    test_skip("c2023:SCNbMAX");
#endif
#if defined(PRIbPTR)
    sprintf(buf, "%" PRIbPTR, (uintptr_t)6);
    test_true( strcmp(buf, "110") == 0 );
    test_pass("c2023:PRIbPTR");
    sprintf(buf, "%" PRIBPTR, (uintptr_t)6);
    test_true( strcmp(buf, "110") == 0 );
    test_pass("c2023:PRIBPTR");
    { uintptr_t v_ = 0;
      test_eq( sscanf("110", "%" SCNbPTR, &v_), 1 );
      test_true( (long long)v_ == 6LL ); }
    test_pass("c2023:SCNbPTR");
#else
    test_skip("c2023:PRIbPTR");
    test_skip("c2023:PRIBPTR");
    test_skip("c2023:SCNbPTR");
#endif
#if defined(__STDC_VERSION_INTTYPES_H__)
    test_true( __STDC_VERSION_INTTYPES_H__ >= 202311L );
    test_pass("c2023:__STDC_VERSION_INTTYPES_H__");
#else
    TEST_NOTE("C23 header version macro not defined by this library");
    test_skip("c2023:__STDC_VERSION_INTTYPES_H__");
#endif
}

TEST_SUITE(inttypes)
    TEST_ADD(imaxdiv_quot_rem)
    TEST_ADD(imaxabs_value)
    TEST_ADD(strtoimax_strtoumax)
    TEST_ADD(wcstoimax_wcstoumax)
    TEST_ADD(pri_exact_width)
    TEST_ADD(pri_least_fast)
    TEST_ADD(pri_max_and_ptr)
    TEST_ADD(scn_exact_width)
    TEST_ADD(scn_max_and_ptr)
    TEST_ADD(format_macros_fast16_c1999)
    TEST_ADD(format_macros_fast32_c1999)
    TEST_ADD(format_macros_exact16_c1999)
    TEST_ADD(format_macros_max_ptr_c2023)
TEST_SUITE_END(inttypes)
