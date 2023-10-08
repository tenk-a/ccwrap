#include <ccwrap_misc/ccwrap_assert_ex.h>
#include <stdio.h>

#ifdef __cplusplus
#include <string>
//#include <string_view>
#endif

#ifndef __cplusplus
#define true        1
#define false       0
#endif

static void assert_ex_test_ok() {
    char c;
    signed char sc;
    signed char sht;
    int i;
    long li;
    long long lli;
    unsigned char uc;
    unsigned short ush;
    unsigned int u;
    unsigned long ul;
    unsigned long long ull;
    float   flt;
    double  dbl;
    long double ldbl;
    char const* cp = "0123456789abcdef";
    unsigned char const* cup = (unsigned char const*)cp;

    printf("---- [CCWRAP_ASSERT Success TEST] Begin ----\n");

    CCWRAP_ASSERT(true);

    CCWRAP_PTR_ASSERT(cp);
    CCWRAP_PTR0_ASSERT(cp);
    CCWRAP_PTR0_ASSERT(NULL);

    CCWRAP_RANGE_ASSERT((void const*)&cp[1], (void const*)&cp[0], (void const*)&cp[15]);
    CCWRAP_RANGE_ASSERT(&cp[1], &cp[0], &cp[15]);
    CCWRAP_RANGE_ASSERT(&cup[1], &cup[0], &cup[15]);

    c  = '@'; CCWRAP_RANGE_ASSERT(c, ' ', '~');
    c  = ' '; CCWRAP_RANGE_ASSERT(c, ' ', '~');
    c  = '~'; CCWRAP_RANGE_ASSERT(c, ' ', '~');

    sc  =  2; CCWRAP_RANGE_ASSERT(sc, -3, 6);
    sc  = -3; CCWRAP_RANGE_ASSERT(sc, -3, 6);
    sc  =  6; CCWRAP_RANGE_ASSERT(sc, -3, 6);

    sht =  2; CCWRAP_RANGE_ASSERT(sht, -3, 6);
    sht = -3; CCWRAP_RANGE_ASSERT(sht, -3, 6);
    sht =  6; CCWRAP_RANGE_ASSERT(sht, -3, 6);

    i   =  2; CCWRAP_RANGE_ASSERT(i, -3, 6);
    i   = -3; CCWRAP_RANGE_ASSERT(i, -3, 6);
    i   =  6; CCWRAP_RANGE_ASSERT(i, -3, 6);

    li  =  2; CCWRAP_RANGE_ASSERT(li, -3l, 6l);
    li  = -3; CCWRAP_RANGE_ASSERT(li, -3l, 6l);
    li  =  6; CCWRAP_RANGE_ASSERT(li, -3l, 6l);

    lli =  2; CCWRAP_RANGE_ASSERT(lli, -3ll, 6ll);
    lli = -3; CCWRAP_RANGE_ASSERT(lli, -3ll, 6ll);
    lli =  6; CCWRAP_RANGE_ASSERT(lli, -3ll, 6ll);

    uc  =  2; CCWRAP_RANGE_ASSERT(uc, 1, 5);
    uc  =  1; CCWRAP_RANGE_ASSERT(uc, 1, 5);
    uc  =  5; CCWRAP_RANGE_ASSERT(uc, 1, 5);

    ush =  2; CCWRAP_RANGE_ASSERT(ush, 1, 5);
    ush =  1; CCWRAP_RANGE_ASSERT(ush, 1, 5);
    ush =  5; CCWRAP_RANGE_ASSERT(ush, 1, 5);

    u   =  2; CCWRAP_RANGE_ASSERT(u, 1U, 5U);
    u   =  1; CCWRAP_RANGE_ASSERT(u, 1U, 5U);
    u   =  5; CCWRAP_RANGE_ASSERT(u, 1U, 5U);

    ul  =  2; CCWRAP_RANGE_ASSERT(ul, 1ul, 5ul);
    ul  =  1; CCWRAP_RANGE_ASSERT(ul, 1ul, 5ul);
    ul  =  5; CCWRAP_RANGE_ASSERT(ul, 1ul, 5ul);

    ull =  2; CCWRAP_RANGE_ASSERT(ull, 1ull, 5ull);
    ull =  1; CCWRAP_RANGE_ASSERT(ull, 1ull, 5ull);
    ull =  5; CCWRAP_RANGE_ASSERT(ull, 1ull, 5ull);

    flt =  2.0f; CCWRAP_RANGE_ASSERT(flt, 1.0f, 5.0f);
    flt =  1.0f; CCWRAP_RANGE_ASSERT(flt, 1.0f, 5.0f);
    flt =  5.0f; CCWRAP_RANGE_ASSERT(flt, 1.0f, 5.0f);

    dbl =  2.0; CCWRAP_RANGE_ASSERT(dbl, 1.0, 5.0);
    dbl =  1.0; CCWRAP_RANGE_ASSERT(dbl, 1.0, 5.0);
    dbl =  5.0; CCWRAP_RANGE_ASSERT(dbl, 1.0, 5.0);

    ldbl =  2.0l; CCWRAP_RANGE_ASSERT(ldbl, 1.0l, 5.0l);
    ldbl =  1.0l; CCWRAP_RANGE_ASSERT(ldbl, 1.0l, 5.0l);
    ldbl =  5.0l; CCWRAP_RANGE_ASSERT(ldbl, 1.0l, 5.0l);

 #ifdef __cplusplus
    std::string ss = "test";
    CCWRAP_RANGE_ASSERT(ss, "abc", std::string("xyz"));
    CCWRAP_RANGE_ASSERT(ss, "abc", "xyz");
    #if 0 //__cplusplus >= 201703L || defined(__CCWRAP_CXX17)
     std::string_view sv = "test2";
     CCWRAP_ASSERT_EQ(sv, std::string_view("test2"));
     CCWRAP_ASSERT_NE(sv, std::string_view("test"));
     CCWRAP_RANGE_ASSERT(sv, std::string_view("abc"), std::string_view("xyz"));
    #endif
 #endif
    printf("---- [CCWRAP_ASSERT Success TEST] End ----\n");
}

static void assert_ex_test_arg_ok() {
    char c;
    signed char sc;
    signed char sht;
    int i;
    long li;
    long long lli;
    unsigned char uc;
    unsigned short ush;
    unsigned int u;
    unsigned long ul;
    unsigned long long ull;
    float   flt;
    double  dbl;
    long double ldbl;
    char const* cp = "0123456789abcdef";
    unsigned char const* cup = (unsigned char const*)cp;

    printf("---- [CCWRAP_ARG_ASSERT Success TEST] Begin ----\n");

    CCWRAP_ARG_ASSERT(1,true);

    CCWRAP_ARG_PTR_ASSERT(2,cp);
    CCWRAP_ARG_PTR0_ASSERT(3,cp);
    CCWRAP_ARG_PTR0_ASSERT(4,NULL);

    CCWRAP_ARG_RANGE_ASSERT(1,(void const*)&cp[1], (void const*)&cp[0], (void const*)&cp[15]);
    CCWRAP_ARG_RANGE_ASSERT(2,&cp[1], &cp[0], &cp[15]);
    CCWRAP_ARG_RANGE_ASSERT(4,&cup[1], &cup[0], &cup[15]);

    c  = '@'; CCWRAP_ARG_RANGE_ASSERT(1, c, ' ', '~');
    c  = ' '; CCWRAP_ARG_RANGE_ASSERT(2, c, ' ', '~');
    c  = '~'; CCWRAP_ARG_RANGE_ASSERT(3, c, ' ', '~');

    sc  =  2; CCWRAP_ARG_RANGE_ASSERT(4, sc, -3, 6);
    sc  = -3; CCWRAP_ARG_RANGE_ASSERT(5, sc, -3, 6);
    sc  =  6; CCWRAP_ARG_RANGE_ASSERT(6, sc, -3, 6);

    sht =  2; CCWRAP_ARG_RANGE_ASSERT(7, sht, -3, 6);
    sht = -3; CCWRAP_ARG_RANGE_ASSERT(8, sht, -3, 6);
    sht =  6; CCWRAP_ARG_RANGE_ASSERT(9, sht, -3, 6);

    i   =  2; CCWRAP_ARG_RANGE_ASSERT(10, i, -3, 6);
    i   = -3; CCWRAP_ARG_RANGE_ASSERT(11, i, -3, 6);
    i   =  6; CCWRAP_ARG_RANGE_ASSERT(12, i, -3, 6);

    li  =  2; CCWRAP_ARG_RANGE_ASSERT(13, li, -3l, 6l);
    li  = -3; CCWRAP_ARG_RANGE_ASSERT(14, li, -3l, 6l);
    li  =  6; CCWRAP_ARG_RANGE_ASSERT(15, li, -3l, 6l);

    lli =  2; CCWRAP_ARG_RANGE_ASSERT(16, lli, -3ll, 6ll);
    lli = -3; CCWRAP_ARG_RANGE_ASSERT(17, lli, -3ll, 6ll);
    lli =  6; CCWRAP_ARG_RANGE_ASSERT(18, lli, -3ll, 6ll);

    uc  =  2; CCWRAP_ARG_RANGE_ASSERT(19, uc, 1, 5);
    uc  =  1; CCWRAP_ARG_RANGE_ASSERT(20, uc, 1, 5);
    uc  =  5; CCWRAP_ARG_RANGE_ASSERT(21, uc, 1, 5);

    ush =  2; CCWRAP_ARG_RANGE_ASSERT(22, ush, 1, 5);
    ush =  1; CCWRAP_ARG_RANGE_ASSERT(23, ush, 1, 5);
    ush =  5; CCWRAP_ARG_RANGE_ASSERT(24, ush, 1, 5);

    u   =  2; CCWRAP_ARG_RANGE_ASSERT(25, u, 1U, 5U);
    u   =  1; CCWRAP_ARG_RANGE_ASSERT(26, u, 1U, 5U);
    u   =  5; CCWRAP_ARG_RANGE_ASSERT(27, u, 1U, 5U);

    ul  =  2; CCWRAP_ARG_RANGE_ASSERT(28, ul, 1ul, 5ul);
    ul  =  1; CCWRAP_ARG_RANGE_ASSERT(29, ul, 1ul, 5ul);
    ul  =  5; CCWRAP_ARG_RANGE_ASSERT(30, ul, 1ul, 5ul);

    ull =  2; CCWRAP_ARG_RANGE_ASSERT(31, ull, 1ull, 5ull);
    ull =  1; CCWRAP_ARG_RANGE_ASSERT(32, ull, 1ull, 5ull);
    ull =  5; CCWRAP_ARG_RANGE_ASSERT(33, ull, 1ull, 5ull);

    flt =  2.0f; CCWRAP_ARG_RANGE_ASSERT(34, flt, 1.0f, 5.0f);
    flt =  1.0f; CCWRAP_ARG_RANGE_ASSERT(35, flt, 1.0f, 5.0f);
    flt =  5.0f; CCWRAP_ARG_RANGE_ASSERT(36, flt, 1.0f, 5.0f);

    dbl =  2.0; CCWRAP_ARG_RANGE_ASSERT(37, dbl, 1.0, 5.0);
    dbl =  1.0; CCWRAP_ARG_RANGE_ASSERT(38, dbl, 1.0, 5.0);
    dbl =  5.0; CCWRAP_ARG_RANGE_ASSERT(39, dbl, 1.0, 5.0);

    ldbl =  2.0l; CCWRAP_ARG_RANGE_ASSERT(40, ldbl, 1.0l, 5.0l);
    ldbl =  1.0l; CCWRAP_ARG_RANGE_ASSERT(41, ldbl, 1.0l, 5.0l);
    ldbl =  5.0l; CCWRAP_ARG_RANGE_ASSERT(42, ldbl, 1.0l, 5.0l);

 #ifdef __cplusplus
    std::string ss = "test";
    CCWRAP_ARG_RANGE_ASSERT(43, ss, "abc", std::string("xyz"));
    CCWRAP_ARG_RANGE_ASSERT(44, ss, "abc", "xyz");
    #if 0 //__cplusplus >= 201703L || defined(CCWRAP_ARG_CXX17)
     std::string_view sv = "test2";
     CCWRAP_ARG_ASSERT_EQ(sv, std::string_view("test2"));
     CCWRAP_ARG_ASSERT_NE(sv, std::string_view("test"));
     CCWRAP_ARG_RANGE_ASSERT(sv, std::string_view("abc"), std::string_view("xyz"));
    #endif
 #endif

    printf("---- [CCWRAP_ARG_ASSERT Success TEST] End ----\n");
}

#ifdef CCWRAP_ASSERT_NO_ABORT
static void assert_ex_test_ng() {
    char c;
    signed char sc;
    signed short sht;
    int i;
    long li;
    long long lli;
    unsigned char uc;
    unsigned short ush;
    unsigned int u;
    unsigned long ul;
    unsigned long long ull;
    float   flt;
    double  dbl;
    long double ldbl;
    char const* cp = "0123456789abcdef";
    unsigned char const* cup = (unsigned char const*)cp;

    //using namespace __ccwrap;

    printf("---- [CCWRAP_ASSERT Failure TEST] Begin ----\n");

    CCWRAP_ASSERT(false);

    CCWRAP_PTR_ASSERT(NULL);
    CCWRAP_PTR0_ASSERT((void const*)1);

    CCWRAP_RANGE_ASSERT((void const*)&cp[16], (void const*)&cp[0], (void const*)&cp[15]);
    CCWRAP_RANGE_ASSERT(&cp[16], &cp[0], &cp[15]);
    CCWRAP_RANGE_ASSERT(&cup[16], &cup[0], &cup[15]);

    c  = '\t'; CCWRAP_RANGE_ASSERT(c, ' ', '~');

    sc  = 33; CCWRAP_RANGE_ASSERT(sc, -3, 6);

    sht = -4; CCWRAP_RANGE_ASSERT(sht, -3, 6);
    i   = -5; CCWRAP_RANGE_ASSERT(i, -3, 6);
    li  =-32; CCWRAP_RANGE_ASSERT(li, -3l, 6l);
    lli =-33; CCWRAP_RANGE_ASSERT(lli, -3ll, 6ll);

    uc  = 33; CCWRAP_RANGE_ASSERT(uc, 1, 5);
    ush =  0; CCWRAP_RANGE_ASSERT(ush, 1, 5);
    u   =  7; CCWRAP_RANGE_ASSERT(u, 1U, 5U);
    ul  =  8; CCWRAP_RANGE_ASSERT(ul, 1ul, 5ul);
    ull =  9; CCWRAP_RANGE_ASSERT(ull, 1ull, 5ull);

    flt =  10.0f; CCWRAP_RANGE_ASSERT(flt, 1.0f, 5.0f);
    dbl =  11.0; CCWRAP_RANGE_ASSERT(dbl, 1.0, 5.0);
    ldbl = 12.0l; CCWRAP_RANGE_ASSERT(ldbl, 1.0l, 5.0l);

 #ifdef __cplusplus
    std::string ss = "test";
    CCWRAP_RANGE_ASSERT(ss, "abc", std::string("rrr"));
  #if 0 //__cplusplus >= 201703L || defined(__CCWRAP_CXX17)
    std::string_view sv = "test2";
    CCWRAP_RANGE_ASSERT(sv, std::string_view("abc"), std::string_view("rrr"));
    CCWRAP_ASSERT_EQ(sv, std::string_view("test"));
    CCWRAP_ASSERT_NE(sv, std::string_view("test2"));
    //CCWRAP_RANGE_ASSERT(sv.data(), "abc", "rrr");
     //CCWRAP_RANGE_ASSERT(sv, "abc", "rrr");
  #endif
 #endif

    printf("---- [CCWRAP_ASSERT Failure TEST] End ----\n");
}


static void assert_ex_test_arg_ng() {
    char c;
    signed char sc;
    signed short sht;
    int i;
    long li;
    long long lli;
    unsigned char uc;
    unsigned short ush;
    unsigned int u;
    unsigned long ul;
    unsigned long long ull;
    float   flt;
    double  dbl;
    long double ldbl;
    char const* cp = "0123456789abcdef";
    unsigned char const* cup = (unsigned char const*)cp;

    printf("---- [CCWRAP_ARG_ASSERT Failure TEST] Begin ----\n");

    CCWRAP_ARG_ASSERT(1,false);

    CCWRAP_ARG_PTR_ASSERT(2,NULL);
    CCWRAP_ARG_PTR0_ASSERT(3,(void const*)1);

    CCWRAP_ARG_RANGE_ASSERT(4, (void const*)&cp[16], (void const*)&cp[0], (void const*)&cp[15]);
    CCWRAP_ARG_RANGE_ASSERT(5, &cp[16], &cp[0], &cp[15]);
    CCWRAP_ARG_RANGE_ASSERT(6, &cup[16], &cup[0], &cup[15]);
    c  = '\t'; CCWRAP_ARG_RANGE_ASSERT(7, c, ' ', '~');

    sc  = 33; CCWRAP_ARG_RANGE_ASSERT(8, sc, -3, 6);

    sht = -4; CCWRAP_ARG_RANGE_ASSERT(9, sht, -3, 6);
    i   = -5; CCWRAP_ARG_RANGE_ASSERT(10, i, -3, 6);
    li  =-32; CCWRAP_ARG_RANGE_ASSERT(11, li, -3l, 6l);
    lli =-33; CCWRAP_ARG_RANGE_ASSERT(12, lli, -3ll, 6ll);

    uc  = 33; CCWRAP_ARG_RANGE_ASSERT(13, uc, 1, 5);
    ush =  0; CCWRAP_ARG_RANGE_ASSERT(14, ush, 1, 5);
    u   =  6; CCWRAP_ARG_RANGE_ASSERT(15, u, 1U, 5U);
    ul  =  8; CCWRAP_ARG_RANGE_ASSERT(16, ul, 1ul, 5ul);
    ull =  9; CCWRAP_ARG_RANGE_ASSERT(17, ull, 1ull, 5ull);

    flt =  10.0f; CCWRAP_ARG_RANGE_ASSERT(18, flt, 1.0f, 5.0f);
    dbl =  11.0; CCWRAP_ARG_RANGE_ASSERT(19, dbl, 1.0, 5.0);
    ldbl = 12.0l; CCWRAP_ARG_RANGE_ASSERT(20, ldbl, 1.0l, 5.0l);

 #ifdef __cplusplus
    std::string ss = "test";
    CCWRAP_ARG_RANGE_ASSERT(21, ss, std::string("abc"), std::string("rrr"));
    CCWRAP_ARG_RANGE_ASSERT(22, ss, "abc", "rrr");
  #if 0 //__cplusplus >= 201703L || defined(__CCWRAP_CXX17)
    std::string_view sv = "test2";
    CCWRAP_ARG_RANGE_ASSERT(22, sv, std::string_view("abc"), std::string_view("rrr"));
    //CCWRAP_ARG_RANGE_ASSERT(sv.data(), "abc", "rrr");
     //CCWRAP_ARG_RANGE_ASSERT(sv, "abc", "rrr");
  #endif
 #endif

    printf("---- [CCWRAP_ARG_ASSERT Failure TEST] End ----\n");
}
#endif


void assert_ex_test() {
    assert_ex_test_ok();
    assert_ex_test_arg_ok();
 #ifdef CCWRAP_ASSERT_NO_ABORT
    //printf("---- !!!! Purposely error !!!! ---- BEGIN\n");
    assert_ex_test_ng();
    assert_ex_test_arg_ng();
    //printf("---- !!!! Purposely error !!!! ---- END\n");
 #endif
}
