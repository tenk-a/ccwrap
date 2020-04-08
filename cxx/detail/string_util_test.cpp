#include <iostream>
#include <string>
#include <climits>
#include <cfloat>
#include <cmath>

#ifndef TEST_CHECK
#define  TEST_CHECK(cc) do {                                                            \
        try {                                                                           \
            ((cc) || printf("%s (%d) : `%s' is failed.\n", __FILE__, __LINE__, #cc));   \
        } catch (...) {                                                                 \
            printf("%s (%d) : `%s' thew.\n", __FILE__, __LINE__, #cc);                  \
        }                                                                               \
    } while (0)
#define  TEST_EQ(a,b)                                                                   \
    do {                                                                                \
        try {                                                                           \
            if ((a) != (b)) {                                                           \
                std::cout << __FILE__ << " (" << __LINE__ << ") : "                     \
                  << #a << "(" << a << ") and " << #b << "(" << b << ") are not equal.\n";  \
            }                                                                           \
        } catch (...) {                                                                 \
            std::cout << __FILE__ << " (" << __LINE__ << ") : `"                        \
                << #a << "(" << a << ") == " << #b << "(" << b << ")' are threw.\n";    \
        }                                                                               \
    } while (0)

#define TEST_THROW(exp,E)   \
    do { bool f=0; (void)f; \
        try { exp; } catch (E) { f=1; } catch (...) {}  \
        if (!f) { printf("%s (%d): (%s) threw not %s.\n", __FILE__, __LINE__, #exp, #E); } \
    } while (0)
#define TEST_NO_THROW(exp)  \
    do { bool f=0; (void)f; \
        try { exp; } catch (...) {f=1;} \
        if (f) { printf("%s (%d): (%s) threw.\n", __FILE__, __LINE__, #exp); } \
    } while (0)

#define TEST_OUT_OF_RANGE(a)        TEST_THROW(a, std::out_of_range)
#endif



static int to_string_test()
{
    using namespace std;

    cout << "char\t" << to_string('0') << endl;

    typedef signed char schar;
    cout << "schar\t" << to_string(0) << "," << to_string(schar(-1)) << "," << to_string(schar(1)) << "," << to_string(schar(SCHAR_MIN)) << "," << to_string(schar(SCHAR_MAX)) << endl;
    typedef unsigned char uchar;
    cout << "uchar\t" << to_string(uchar(0U)) << "," << to_string(uchar(1U)) << "," << to_string(uchar(UCHAR_MAX)) << endl;

    cout << "short\t" << to_string(0) << "," << to_string(short(-1)) << "," << to_string(short(1)) << "," << to_string(short(SHRT_MIN)) << "," << to_string(short(SHRT_MAX)) << endl;
    typedef unsigned short ushort;
    cout << "ushort\t" << to_string(ushort(0U)) << "," << to_string(ushort(1U)) << "," << to_string(ushort(USHRT_MAX)) << endl;

    cout << "int\t" << to_string(0) << "," << to_string(-1) << "," << to_string(1) << "," << to_string(INT_MIN) << "," << to_string(INT_MAX) << endl;
    cout << "uint\t" << to_string(0U) << "," << to_string(1U) << "," << to_string(UINT_MAX) << endl;
    cout << "long\t" << to_string(0L) << "," << to_string(-1L) << "," << to_string(1L) << "," << to_string(LONG_MIN) << "," << to_string(LONG_MAX) << endl;
    cout << "ulong\t" << to_string(0UL) << "," << to_string(1UL) << "," << to_string(ULONG_MAX) << endl;
    cout << "llong\t" << to_string(0LL) << "," << to_string(-1LL) << "," << to_string(1LL) << "," << to_string(LLONG_MIN) << "," << to_string(LLONG_MAX) << endl;
    cout << "ullong\t" << to_string(0ULL) << "," << to_string(1ULL) << "," << to_string(ULLONG_MAX) << endl;

    cout << "float\t" << to_string(0.0f) << "," << to_string(-1.0f) << "," << to_string(1.0f) << "," << to_string(-FLT_MAX) << "," << to_string(FLT_MAX) << endl;

    cout << "double\t" << to_string(0.0) << "," << to_string(-1.0) << "," << to_string(1.0) << "," << to_string(-DBL_MAX) << "," << to_string(DBL_MAX) << endl;

    cout << "ldouble\t" << to_string(0.0L) << "," << to_string(-1.0L) << "," << to_string(1.0L) << "," << to_string(-LDBL_MAX) << "," << to_string(LDBL_MAX) << endl;

    return 0;
}

static int stol_test()
{
    using namespace std;
    TEST_EQ(stoi("100"), 100);
    TEST_EQ(stoi("-100"), -100);
    TEST_EQ(stol("100"), 100);
    TEST_EQ(stol("-100"), -100);
    TEST_EQ(stoul("1000"), 1000);

    TEST_EQ(stoll("100"), 100);
    TEST_EQ(stoll("-100"), -100);
    TEST_EQ(stoull("1000"), 1000);

    TEST_EQ(stof("100.0f"), 100.0f);
    TEST_EQ(stod("100.0"), 100.0);
    TEST_EQ(stold("100.0"), 100.0);
#if 1
    TEST_EQ(stoi(L"100"), 100);
    TEST_EQ(stoi(L"-100"), -100);
    TEST_EQ(stol(L"100"), 100);
    TEST_EQ(stol(L"-100"), -100);
    TEST_EQ(stoul(L"1000"), 1000);
    TEST_EQ(stoll(L"100"), 100);
    TEST_EQ(stoll(L"-100"), -100);
    TEST_EQ(stoull(L"1000"), 1000);
    TEST_EQ(stof(L"100.0f"), 100.0f);
    TEST_EQ(stod(L"100.0"), 100.0);
    TEST_EQ(stold(L"100.0"), 100.0);
#endif
    return 0;
}

int string_util_test()
{
    to_string_test();
    stol_test();
	return 0;
}
