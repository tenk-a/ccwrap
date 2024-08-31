#define __STDC_LIMIT_MACROS
#include <ccwrap_misc/test.hpp>
#include <stdint.h>
#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>
#include <climits>
#include <cfloat>
#include <cmath>

#ifndef __ccwrap_llong
 typedef long long                          __ccwrap_llong;
 typedef unsigned long long                 __ccwrap_ullong;
 #define __ccwrap_llong                     __ccwrap_llong
 #define __ccwrap_ullong                    __ccwrap_ullong
#endif

TEST_SUITE(string_sub) {}

using namespace std;

TEST_CASE(string_sub, to_string) {
	{
	    test_eq(to_string('0'), "48");

	    typedef signed char schar;
	    typedef unsigned char uchar;
	    test_eq(to_string(schar( 0)), "0");
	    test_eq(to_string(schar(-1)), "-1");
	    test_eq(to_string(schar( 1)), "1");
	    test_eq(to_string(schar(SCHAR_MIN)), "-128");
	    test_eq(to_string(schar(SCHAR_MAX)), "127");
	    test_eq(to_string(uchar( 0)), "0");
	    test_eq(to_string(uchar(-1)), "255");
	    test_eq(to_string(uchar( 1)), "1");
	    test_eq(to_string(uchar(UCHAR_MAX)), "255");

	    typedef unsigned short ushort;
	    test_eq(to_string(short( 0)), "0");
	    test_eq(to_string(short(-1)), "-1");
	    test_eq(to_string(short( 1)), "1");
	    test_eq(to_string(short(SHRT_MIN)), "-32768");
	    test_eq(to_string(short(SHRT_MAX)), "32767");
	    test_eq(to_string(ushort( 0)), "0");
	    test_eq(to_string(ushort(-1)), "65535");
	    test_eq(to_string(ushort( 1)), "1");
	    test_eq(to_string(ushort(USHRT_MAX)), "65535");

	    typedef unsigned int uint;
	    test_eq(to_string(int( 0)), "0");
	    test_eq(to_string(int(-1)), "-1");
	    test_eq(to_string(int( 1)), "1");
	    test_eq(to_string(int(INT_MIN)), "-2147483648");
	    test_eq(to_string(int(INT_MAX)), "2147483647");
	    test_eq(to_string(uint( 0)), "0");
	    test_eq(to_string(uint(-1)), "4294967295");
	    test_eq(to_string(uint( 1)), "1");
	    test_eq(to_string(uint(UINT_MAX)), "4294967295");

	    typedef unsigned long ulong;
	    test_eq(to_string(long( 0)), "0");
	    test_eq(to_string(long(-1)), "-1");
	    test_eq(to_string(long( 1)), "1");
	    test_eq(to_string(long(INT_MIN)), "-2147483648");
	    test_eq(to_string(long(INT_MAX)), "2147483647");
	    test_eq(to_string(ulong( 0)), "0");
	    test_eq(to_string(ulong( 1)), "1");
	    test_eq(to_string(ulong(UINT_MAX)), "4294967295");

	    typedef __ccwrap_llong  llong;
	    typedef __ccwrap_ullong ullong;
	    test_eq(to_string(llong( 0)), "0");
	    test_eq(to_string(llong(-1)), "-1");
	    test_eq(to_string(llong( 1)), "1");
	    test_eq(to_string(llong(INT_MIN)), "-2147483648");
	    test_eq(to_string(llong(INT_MAX)), "2147483647");
	    test_eq(to_string(ullong( 0)), "0");
	    test_eq(to_string(ullong( 1)), "1");
	    test_eq(to_string(ullong(UINT_MAX)), "4294967295");
	}
 #if !defined(__CCWRAP_NO_STRINGSTREAM)
    {
        std::stringstream    ss;
        ss << "char " << to_string('0') << '\n';
        ss << "long " << to_string(-long(65535)) << "," << to_string(long(65535)) << '\n';
        ss << "ulong " << to_string((unsigned long)(65535)) << '\n';
        ss << "llong " << to_string(-(__ccwrap_llong)(65535)) << "," << to_string((__ccwrap_llong)(65535)) << '\n';
        ss << "ullong " << to_string((__ccwrap_ullong)(65535)) << '\n';
        ss << "int8_t " << to_string(0) << "," << to_string(int8_t(-1)) << "," << to_string(int8_t(1)) << "," << to_string(int8_t(INT8_MIN)) << "," << to_string(int8_t(INT8_MAX)) << '\n';
        ss << "uint8_t " << to_string(uint8_t(0U)) << "," << to_string(uint8_t(1U)) << "," << to_string(uint8_t(UINT8_MAX)) << '\n';
        ss << "int16_t " << to_string(0) << "," << to_string(int16_t(-1)) << "," << to_string(int16_t(1)) << "," << to_string(int16_t(INT16_MIN)) << "," << to_string(int16_t(INT16_MAX)) << '\n';
        ss << "uint16_t " << to_string(uint16_t(0U)) << "," << to_string(uint16_t(1U)) << "," << to_string(uint16_t(UINT16_MAX)) << '\n';
        ss << "int32_t " << to_string(0) << "," << to_string(int32_t(-1)) << "," << to_string(int32_t(1)) << "," << to_string(int32_t(INT32_MIN)) << "," << to_string(int32_t(INT32_MAX)) << '\n';
        ss << "uint32_t " << to_string(uint32_t(0U)) << "," << to_string(uint32_t(1U)) << "," << to_string(uint32_t(UINT32_MAX)) << '\n';
        ss << "int64_t " << to_string(int64_t(0)) << "," << to_string(int64_t(-1)) << "," << to_string(int64_t(1)) << "," << to_string(INT64_MIN) << "," << to_string(INT64_MAX) << '\n';
        ss << "uint64_t " << to_string(uint64_t(0)) << "," << to_string(uint64_t(1)) << "," << to_string(uint64_t(UINT64_MAX)) << '\n';

        ss << "float " << to_string(0.0f) << "," << to_string(-1.0f) << "," << to_string(1.0f) << "," << to_string(-16777215.0f) << "," << to_string(16777215.0f) << '\n';

        ss << "double " << to_string(0.0) << "," << to_string(-1.0) << "," << to_string(1.0) << "," << to_string(-16777215.0) << "," << to_string(16777215.0) << '\n';

        ss << "ldouble " << to_string(0.0L) << "," << to_string(-1.0L) << "," << to_string(1.0L) << "," << to_string(-16777215.0L) << "," << to_string(16777215.0L) << '\n';

        test_true(ss.str() ==
                    "char 48\n"
                    "long -65535,65535\n"
                    "ulong 65535\n"
                    "llong -65535,65535\n"
                    "ullong 65535\n"
                    "int8_t 0,-1,1,-128,127\n"
                    "uint8_t 0,1,255\n"
                    "int16_t 0,-1,1,-32768,32767\n"
                    "uint16_t 0,1,65535\n"
                    "int32_t 0,-1,1,-2147483648,2147483647\n"
                    "uint32_t 0,1,4294967295\n"
                    "int64_t 0,-1,1,-9223372036854775808,9223372036854775807\n"
                    "uint64_t 0,1,18446744073709551615\n"
                    "float 0.000000,-1.000000,1.000000,-16777215.000000,16777215.000000\n"
                    "double 0.000000,-1.000000,1.000000,-16777215.000000,16777215.000000\n"
                    "ldouble 0.000000,-1.000000,1.000000,-16777215.000000,16777215.000000\n"
        );
    }
 #endif
}


TEST_CASE(string_sub, stol) {
    using namespace std;
    test_eq(stoi("100"), 100);
    test_eq(stoi("-100"), -100);
    test_eq(stol("100"), 100);
    test_eq(stol("-100"), -100);
    test_eq(stoul("1000"), 1000);

    test_eq(stoll("100"), 100);
    test_eq(stoll("-100"), -100);
    test_eq(stoull("1000"), 1000);

    test_eq(stof("100.0f"), 100.0f);
    test_eq(stod("100.0"), 100.0);
    test_eq(stold("100.0"), 100.0);

 #ifndef __CCWRAP_NO_WCHAR
    test_eq(stoi(L"100"), 100);
    test_eq(stoi(L"-100"), -100);
    test_eq(stol(L"100"), 100);
    test_eq(stol(L"-100"), -100);
    test_eq(stoul(L"1000"), 1000);
    test_eq(stoll(L"100"), 100);
    test_eq(stoll(L"-100"), -100);
    test_eq(stoull(L"1000"), 1000);
    test_eq(stof(L"100.0f"), 100.0f);
    test_eq(stod(L"100.0"), 100.0);
    test_eq(stold(L"100.0"), 100.0);
 #endif
}

