#include "test_cxx.hpp"

#include <limits>
#include <string>
#include <climits>
#include <cfloat>

TEST_CASE(limits, integer) {
    test_eq( STD::numeric_limits<int>::max(), INT_MAX );
    test_eq( STD::numeric_limits<int>::min(), INT_MIN );
    test_true( STD::numeric_limits<int>::is_signed );
    test_true( STD::numeric_limits<int>::is_integer );
    test_true( !STD::numeric_limits<unsigned>::is_signed );
    test_pass("cxx03:numeric_limits::is_signed#unsigned");
    test_eq( STD::numeric_limits<unsigned>::max(), UINT_MAX );
    test_pass("cxx03:numeric_limits::max#unsigned");
    test_eq( (int)STD::numeric_limits<unsigned char>::max(), 255 );
    test_pass("cxx03:numeric_limits::max#unsigned_char");
    test_eq( (int)STD::numeric_limits<int>::digits, 31 );
    test_true( STD::numeric_limits<unsigned>::is_modulo );
    TEST_SKIP_VC120("MSVC before VS2015 reports is_modulo=true for signed int");
    test_true( !STD::numeric_limits<int>::is_modulo );
    test_pass("cxx03:numeric_limits::is_modulo#unsigned");
}

TEST_CASE(limits, floating) {
    test_true( STD::numeric_limits<double>::is_specialized );
    test_true( !STD::numeric_limits<double>::is_integer );
    test_true( STD::numeric_limits<double>::epsilon() > 0.0 );
    test_true( STD::numeric_limits<double>::max() > 1e300 );
    test_eq( (int)STD::numeric_limits<float>::radix, 2 );
    test_pass("cxx03:numeric_limits::radix#float");
#if _TST_HAS_CXX11_LIB_MEMBERS
    test_eq( (int)STD::numeric_limits<double>::max_digits10, 17 );
    test_eq( (int)STD::numeric_limits<float>::max_digits10, 9 );
    test_pass("cxx11:numeric_limits::max_digits10#float");
#else
    TEST_SKIP_N(2);
    test_skip("cxx11:numeric_limits::max_digits10#float");
#endif
    test_true( STD::numeric_limits<double>::has_quiet_NaN );
    test_true( STD::numeric_limits<double>::is_bounded );
    test_true( STD::numeric_limits<double>::round_error() == 0.5 );
    test_eq( (int)STD::numeric_limits<double>::max_exponent, (int)DBL_MAX_EXP );
}

TEST_CASE(limits, ieee_special_values) {
    union { double d; unsigned long long u; } x;

    x.d = STD::numeric_limits<double>::infinity();
    test_true( ((x.u >> 52) & 0x7FF) == 0x7FF );
    test_true( (x.u & 0xFFFFFFFFFFFFFULL) == 0 );
    test_pass("cxx03:numeric_limits::infinity (IEEE bits)");

    x.d = STD::numeric_limits<double>::quiet_NaN();
    test_true( ((x.u >> 52) & 0x7FF) == 0x7FF );
    test_true( (x.u & 0xFFFFFFFFFFFFFULL) != 0 );
    test_pass("cxx03:numeric_limits::quiet_NaN (IEEE bits)");

    x.d = STD::numeric_limits<double>::denorm_min();
    test_true( x.u == 1ULL );
    test_pass("cxx03:numeric_limits::denorm_min (IEEE bits)");
}

TEST_CASE(limits, all_members_int) {
    typedef STD::numeric_limits<int> L;
    test_true( L::is_specialized );
    test_pass("cxx03:numeric_limits::is_specialized#int");
    test_eq( L::min(), INT_MIN );
    test_pass("cxx03:numeric_limits::min#int");
    test_eq( L::max(), INT_MAX );
    test_pass("cxx03:numeric_limits::max#int");
#if _TST_HAS_CXX11_LIB_MEMBERS
    test_eq( L::lowest(), INT_MIN );
    test_pass("cxx11:numeric_limits::lowest#int");
#else
    TEST_SKIP1();
    test_skip("cxx11:numeric_limits::lowest#int");
#endif
    test_eq( (int)L::digits, 31 );
    test_pass("cxx03:numeric_limits::digits#int");
    test_eq( (int)L::digits10, 9 );
    test_pass("cxx03:numeric_limits::digits10#int");
#if _TST_HAS_CXX11_LIB_MEMBERS
    test_eq( (int)L::max_digits10, 0 );
    test_pass("cxx11:numeric_limits::max_digits10#int");
#else
    TEST_SKIP1();
    test_skip("cxx11:numeric_limits::max_digits10#int");
#endif
    test_true( L::is_signed );
    test_pass("cxx03:numeric_limits::is_signed#int");
    test_true( L::is_integer );
    test_pass("cxx03:numeric_limits::is_integer#int");
    test_true( L::is_exact );
    test_pass("cxx03:numeric_limits::is_exact#int");
    test_eq( (int)L::radix, 2 );
    test_pass("cxx03:numeric_limits::radix#int");
    test_eq( L::epsilon(), 0 );
    test_pass("cxx03:numeric_limits::epsilon#int");
    test_eq( L::round_error(), 0 );
    test_pass("cxx03:numeric_limits::round_error#int");
    test_eq( (int)L::min_exponent, 0 );
    test_pass("cxx03:numeric_limits::min_exponent#int");
    test_eq( (int)L::min_exponent10, 0 );
    test_pass("cxx03:numeric_limits::min_exponent10#int");
    test_eq( (int)L::max_exponent, 0 );
    test_pass("cxx03:numeric_limits::max_exponent#int");
    test_eq( (int)L::max_exponent10, 0 );
    test_pass("cxx03:numeric_limits::max_exponent10#int");
    test_eq( (int)L::round_style, (int)STD::round_toward_zero );
    test_pass("cxx03:numeric_limits::round_style#int");
    test_eq( (int)STD::round_indeterminate,       -1 );
    test_eq( (int)STD::round_toward_zero,          0 );
    test_eq( (int)STD::round_to_nearest,           1 );
    test_eq( (int)STD::round_toward_infinity,      2 );
    test_eq( (int)STD::round_toward_neg_infinity,  3 );
    {
        STD::float_round_style rs = L::round_style;
        test_eq( (int)rs, (int)STD::round_toward_zero );
    }
    test_pass("cxx03:float_round_style");
    test_true( !L::has_infinity );
    test_pass("cxx03:numeric_limits::has_infinity#int");
    test_true( !L::has_quiet_NaN );
    test_pass("cxx03:numeric_limits::has_quiet_NaN#int");
    test_true( !L::has_signaling_NaN );
    test_pass("cxx03:numeric_limits::has_signaling_NaN#int");
    test_eq( (int)L::has_denorm, (int)STD::denorm_absent );
    test_pass("cxx03:numeric_limits::has_denorm#int");
    test_true( !L::has_denorm_loss );
    test_pass("cxx03:numeric_limits::has_denorm_loss#int");
    test_eq( L::infinity(), 0 );
    test_pass("cxx03:numeric_limits::infinity#int");
    test_eq( L::quiet_NaN(), 0 );
    test_pass("cxx03:numeric_limits::quiet_NaN#int");
    test_eq( L::signaling_NaN(), 0 );
    test_pass("cxx03:numeric_limits::signaling_NaN#int");
    test_eq( L::denorm_min(), 0 );
    test_pass("cxx03:numeric_limits::denorm_min#int");
    test_true( !L::is_iec559 );
    test_pass("cxx03:numeric_limits::is_iec559#int");
    test_true( L::is_bounded );
    test_pass("cxx03:numeric_limits::is_bounded#int");
    TEST_SKIP_VC120("MSVC before VS2015 reports is_modulo=true for signed int");
    test_true( !L::is_modulo );
    test_pass("cxx03:numeric_limits::is_modulo#int");
    { bool t = L::traps; test_true( t == true || t == false ); }
    test_pass("cxx03:numeric_limits::traps#int");
    test_true( !L::tinyness_before );
    test_pass("cxx03:numeric_limits::tinyness_before#int");
}

TEST_CASE(limits, all_members_double) {
    typedef STD::numeric_limits<double> L;
    test_true( L::is_specialized );
    test_pass("cxx03:numeric_limits::is_specialized#double");
    test_true( L::min() == DBL_MIN );
    test_pass("cxx03:numeric_limits::min#double");
    test_true( L::max() == DBL_MAX );
    test_pass("cxx03:numeric_limits::max#double");
#if _TST_HAS_CXX11_LIB_MEMBERS
    test_true( L::lowest() == -DBL_MAX );
    test_pass("cxx11:numeric_limits::lowest#double");
#else
    TEST_SKIP1();
    test_skip("cxx11:numeric_limits::lowest#double");
#endif
    test_eq( (int)L::digits, DBL_MANT_DIG );
    test_pass("cxx03:numeric_limits::digits#double");
    test_eq( (int)L::digits10, DBL_DIG );
    test_pass("cxx03:numeric_limits::digits10#double");
#if _TST_HAS_CXX11_LIB_MEMBERS
    test_eq( (int)L::max_digits10, 17 );
    test_pass("cxx11:numeric_limits::max_digits10#double");
#else
    TEST_SKIP1();
    test_skip("cxx11:numeric_limits::max_digits10#double");
#endif
    test_true( L::is_signed );
    test_pass("cxx03:numeric_limits::is_signed#double");
    test_true( !L::is_integer );
    test_pass("cxx03:numeric_limits::is_integer#double");
    test_true( !L::is_exact );
    test_pass("cxx03:numeric_limits::is_exact#double");
    test_eq( (int)L::radix, 2 );
    test_pass("cxx03:numeric_limits::radix#double");
    test_true( L::epsilon() == DBL_EPSILON );
    test_pass("cxx03:numeric_limits::epsilon#double");
    test_true( L::round_error() == 0.5 );
    test_pass("cxx03:numeric_limits::round_error#double");
    test_eq( (int)L::min_exponent, DBL_MIN_EXP );
    test_pass("cxx03:numeric_limits::min_exponent#double");
    test_eq( (int)L::min_exponent10, DBL_MIN_10_EXP );
    test_pass("cxx03:numeric_limits::min_exponent10#double");
    test_eq( (int)L::max_exponent, DBL_MAX_EXP );
    test_pass("cxx03:numeric_limits::max_exponent#double");
    test_eq( (int)L::max_exponent10, DBL_MAX_10_EXP );
    test_pass("cxx03:numeric_limits::max_exponent10#double");
    test_eq( (int)L::round_style, (int)STD::round_to_nearest );
    test_pass("cxx03:numeric_limits::round_style#double");
    test_true( L::has_infinity );
    test_pass("cxx03:numeric_limits::has_infinity#double");
    test_true( L::has_quiet_NaN );
    test_pass("cxx03:numeric_limits::has_quiet_NaN#double");
    test_true( L::has_signaling_NaN );
    test_pass("cxx03:numeric_limits::has_signaling_NaN#double");
    test_eq( (int)L::has_denorm, (int)STD::denorm_present );
    test_pass("cxx03:numeric_limits::has_denorm#double");
    TEST_SKIP_VC120("MSVC before VS2015 reports has_denorm_loss=true for double");
    test_true( !L::has_denorm_loss );
    test_pass("cxx03:numeric_limits::has_denorm_loss#double");
    test_true( L::is_iec559 );
    test_pass("cxx03:numeric_limits::is_iec559#double");
    test_true( L::is_bounded );
    test_pass("cxx03:numeric_limits::is_bounded#double");
    test_true( !L::is_modulo );
    test_pass("cxx03:numeric_limits::is_modulo#double");
    TEST_SKIP_VC090("MSVC 9 reports traps=true for double");
    test_true( !L::traps );
    test_pass("cxx03:numeric_limits::traps#double");
    TEST_SKIP_VC120("MSVC before VS2015 reports tinyness_before=true for double");
    test_true( !L::tinyness_before );
    test_pass("cxx03:numeric_limits::tinyness_before#double");

    test_true( L::infinity() > DBL_MAX );
    test_pass("cxx03:numeric_limits::infinity#double");
    test_true( L::denorm_min() > 0.0 && L::denorm_min() < DBL_MIN );
    test_pass("cxx03:numeric_limits::denorm_min#double");
}

#if _TST_HAS_CXX11_LIB_MEMBERS
TEST_CASE(limits, lowest_vs_min) {
    test_true( STD::numeric_limits<double>::lowest() < 0.0 );
    test_true( STD::numeric_limits<double>::min() > 0.0 );
    test_true( STD::numeric_limits<double>::lowest() < STD::numeric_limits<double>::min() );
    test_eq( STD::numeric_limits<int>::lowest(), STD::numeric_limits<int>::min() );
    test_pass("cxx11:numeric_limits::lowest (float vs min)");
    test_eq( (int)STD::numeric_limits<unsigned>::lowest(), 0 );
    test_pass("cxx11:numeric_limits::lowest#unsigned");
}
#else
TEST_CASE_SKIP(limits, lowest_vs_min)
#endif

TEST_CASE(limits, integer_family) {
    test_true( STD::numeric_limits<bool>::is_specialized );
    test_eq( (int)STD::numeric_limits<bool>::digits, 1 );
    test_pass("cxx03:numeric_limits::digits#bool");
    test_true( !STD::numeric_limits<bool>::is_signed );
    test_pass("cxx03:numeric_limits::is_signed#bool");
    test_eq( (int)STD::numeric_limits<signed char>::digits, 7 );
    test_pass("cxx03:numeric_limits::digits#signed_char");
    test_eq( (int)STD::numeric_limits<unsigned char>::digits, 8 );
    test_pass("cxx03:numeric_limits::digits#unsigned_char");
    test_eq( (int)STD::numeric_limits<short>::digits, 15 );
    test_pass("cxx03:numeric_limits::digits#short");
    test_eq( (int)STD::numeric_limits<unsigned short>::digits, 16 );
    test_pass("cxx03:numeric_limits::digits#unsigned_short");
    test_eq( (int)STD::numeric_limits<unsigned>::digits, 32 );
    test_pass("cxx03:numeric_limits::digits#unsigned");
    test_true( STD::numeric_limits<long long>::max() > (long long)INT_MAX );
    test_pass("cxx03:numeric_limits::max#long_long");
    test_true( STD::numeric_limits<unsigned long long>::is_modulo );
    test_pass("cxx03:numeric_limits::is_modulo#unsigned_long_long");
    test_true( !STD::numeric_limits<unsigned long long>::is_signed );
    test_pass("cxx03:numeric_limits::is_signed#unsigned_long_long");
    test_true( STD::numeric_limits<wchar_t>::is_specialized );
    test_pass("cxx03:numeric_limits::is_specialized#wchar_t");
    test_true( STD::numeric_limits<wchar_t>::is_integer );
    test_pass("cxx03:numeric_limits::is_integer#wchar_t");

    test_eq( STD::numeric_limits<char>::is_signed, (char)-1 < 0 );
    test_pass("cxx03:numeric_limits::is_signed#char");
}

TEST_CASE(limits, non_specialized_and_cv) {
    test_true( !STD::numeric_limits<STD::string>::is_specialized );
    test_pass("cxx03:numeric_limits::is_specialized (primary template)");
    test_true( STD::numeric_limits<const int>::is_specialized );
    test_eq( STD::numeric_limits<const int>::max(), INT_MAX );
    test_pass("cxx03:numeric_limits (cv-qualified)");
}

TEST_CASE(limits, style_enumerators) {
    STD::float_denorm_style ds = STD::denorm_present;
    test_eq( (int)ds, 1 );
    test_true( (int)STD::denorm_indeterminate < (int)STD::denorm_absent );
    test_pass("cxx03:float_denorm_style");

    test_eq( (int)STD::denorm_indeterminate, -1 );
    test_pass("cxx03:denorm_indeterminate");
    test_eq( (int)STD::denorm_absent, 0 );
    test_pass("cxx03:denorm_absent");
    test_eq( (int)STD::denorm_present, 1 );
    test_pass("cxx03:denorm_present");

    test_eq( (int)STD::round_indeterminate, -1 );
    test_pass("cxx03:round_indeterminate");
    test_eq( (int)STD::round_toward_zero, 0 );
    test_pass("cxx03:round_toward_zero");
    test_eq( (int)STD::round_to_nearest, 1 );
    test_pass("cxx03:round_to_nearest");
    test_eq( (int)STD::round_toward_infinity, 2 );
    test_pass("cxx03:round_toward_infinity");
    test_eq( (int)STD::round_toward_neg_infinity, 3 );
    test_pass("cxx03:round_toward_neg_infinity");
}

TEST_CASE(limits, nan_producers_double) {
    typedef STD::numeric_limits<double> L;
    union { double d; unsigned long long u; } x;

    x.d = L::quiet_NaN();
    unsigned long long exponent = (x.u >> 52) & 0x7FFULL;
    unsigned long long mantissa = x.u & 0xFFFFFFFFFFFFFULL;
    test_eq( (long)exponent, 0x7FFL );
    test_true( mantissa != 0 );
    test_true( L::has_quiet_NaN );
    test_pass("cxx03:numeric_limits::quiet_NaN#double");

    x.d = L::signaling_NaN();
    exponent = (x.u >> 52) & 0x7FFULL;
    mantissa = x.u & 0xFFFFFFFFFFFFFULL;
    test_eq( (long)exponent, 0x7FFL );
    test_true( mantissa != 0 );
    test_true( L::has_signaling_NaN );
    test_pass("cxx03:numeric_limits::signaling_NaN#double");
}

TEST_CASE(limits, standard_specializations) {
    {
        typedef STD::numeric_limits<long long> L;
        test_true( L::is_specialized );
        test_true( L::is_integer && L::is_signed );
        test_eq( (long)L::digits, (long)(sizeof(long long) * CHAR_BIT - 1) );
        test_true( L::max() > 0 && L::min() < 0 );
        test_true( L::max() >= 9223372036854775807LL || sizeof(long long) < 8 );
        test_pass("cxx11:numeric_limits<long long>");
    }
    {
        typedef STD::numeric_limits<unsigned long long> L;
        test_true( L::is_specialized );
        test_true( L::is_integer && !L::is_signed && L::is_modulo );
        test_eq( (long)L::digits, (long)(sizeof(unsigned long long) * CHAR_BIT) );
        test_eq( (long long)L::min(), 0LL );
        test_pass("cxx11:numeric_limits<unsigned long long>");
    }
#if _TST_HAS_CHAR16_32
    {
        typedef STD::numeric_limits<char16_t> L;
        test_true( L::is_specialized );
        test_true( L::is_integer && !L::is_signed );
        test_eq( (long)L::digits, (long)(sizeof(char16_t) * CHAR_BIT) );
        test_eq( (long)L::min(), 0L );
        test_pass("cxx11:numeric_limits<char16_t>");
    }
    {
        typedef STD::numeric_limits<char32_t> L;
        test_true( L::is_specialized );
        test_true( L::is_integer && !L::is_signed );
        test_eq( (long)L::digits, (long)(sizeof(char32_t) * CHAR_BIT) );
        test_eq( (long)L::min(), 0L );
        test_pass("cxx11:numeric_limits<char32_t>");
    }
#else
    TEST_NOTE("char16_t / char32_t are not distinct types here");
    test_skip("cxx11:numeric_limits<char16_t>");
    test_skip("cxx11:numeric_limits<char32_t>");
#endif
#if _TST_HAS_CHAR8_NAME
    {
        typedef STD::numeric_limits<char8_t> L;
        test_true( L::is_specialized );
        test_true( L::is_integer && !L::is_signed );
        test_eq( (long)L::digits, (long)(sizeof(char8_t) * CHAR_BIT) );
        test_eq( (long)L::min(), 0L );
        test_eq( (long)L::max(), 255L );
        test_pass("cxx20:numeric_limits<char8_t>");
    }
#else
    TEST_NOTE("no char8_t here, not even as a typedef");
    test_skip("cxx20:numeric_limits<char8_t>");
#endif
}

TEST_CASE(limits, feature_test_macros) {
#if defined(__cpp_lib_char8_t)
    test_true( __cpp_lib_char8_t > 0L );
    test_pass("cxx20:__cpp_lib_char8_t");
#else
    test_skip("cxx20:__cpp_lib_char8_t");
#endif
}
