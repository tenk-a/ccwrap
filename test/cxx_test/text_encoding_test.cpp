#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2026 && _TST_TEST_HAS(<text_encoding>)
#include <text_encoding>
#include <string_view>
#include <cstring>
#include <type_traits>

namespace {
template <class It, class Se>
STD::size_t ccw_alias_count(It it, Se last) {
    STD::size_t n = 0;
    for (; !(it == last); ++it) ++n;
    return n;
}
template <class It, class Se>
bool ccw_alias_has(It it, Se last, const char* want) {
    for (; !(it == last); ++it)
        if (STD::strcmp(*it, want) == 0) return true;
    return false;
}
}

TEST_CASE(text_encoding, by_id_and_by_name) {
    STD::text_encoding u8(STD::text_encoding::UTF8);
    test_true( u8.mib() == STD::text_encoding::UTF8 );
    test_eq( STD::strcmp(u8.name(), "UTF-8"), 0 );

    STD::text_encoding byname(STD::string_view("utf-8"));
    test_true( byname == u8 );
    test_true( byname.mib() == STD::text_encoding::UTF8 );

    STD::text_encoding def;
    test_true( def.mib() == STD::text_encoding::unknown );
    test_true( def != u8 );
    test_pass("cxx26:by id and by name");
}

TEST_CASE(text_encoding, copy_keeps_name) {
    STD::text_encoding u8(STD::text_encoding::UTF8);
    STD::text_encoding cp = u8;
    test_eq( STD::strcmp(cp.name(), "UTF-8"), 0 );
    test_true( ccw_alias_count(cp.aliases().begin(), cp.aliases().end()) >= (STD::size_t)1 );
    test_pass("cxx26:copy keeps name");
}

TEST_CASE(text_encoding, several_encodings) {

    STD::text_encoding ascii(STD::text_encoding::ASCII);
    test_eq( STD::strcmp(ascii.name(), "US-ASCII"), 0 );
    test_true( ascii.mib() == STD::text_encoding::ASCII );

    STD::text_encoding latin1(STD::text_encoding::ISOLatin1);
    test_true( ccw_alias_has(latin1.aliases().begin(), latin1.aliases().end(), "ISO-8859-1") );
    test_true( STD::text_encoding(STD::string_view(latin1.name())) == latin1 );

    STD::text_encoding u16(STD::text_encoding::UTF16);
    test_true( ccw_alias_has(u16.aliases().begin(), u16.aliases().end(), "UTF-16") );
    STD::text_encoding u32le(STD::text_encoding::UTF32LE);
    test_true( ccw_alias_has(u32le.aliases().begin(), u32le.aliases().end(), "UTF-32LE") );

    STD::text_encoding w1252(STD::text_encoding::windows1252);
    test_true( w1252.mib() == STD::text_encoding::windows1252 );

    test_true( ascii != u16 );
    test_true( latin1 != w1252 );

    STD::text_encoding byname(STD::string_view("us-ascii"));
    test_true( byname == ascii );

    STD::text_encoding bad(STD::string_view("no-such-encoding-xyz"));
    test_true( bad.mib() == STD::text_encoding::other );
    test_eq( STD::strcmp(bad.name(), "no-such-encoding-xyz"), 0 );

    STD::text_encoding::id which = u16.mib();
    test_true( which == STD::text_encoding::UTF16 );
    test_pass("cxx26:several encodings");
}

TEST_CASE(text_encoding, aliases_iteration) {
    STD::text_encoding u8(STD::text_encoding::UTF8);
    STD::text_encoding::aliases_view av = u8.aliases();
    test_true( ccw_alias_count(av.begin(), av.end()) >= (STD::size_t)1 );

    test_true( ccw_alias_has(av.begin(), av.end(), "UTF-8") );
    test_pass("cxx26:aliases iteration");
}
#else
TEST_CASE(text_encoding, by_id_and_by_name) {
    TEST_NOTE("<text_encoding> is not shipped by this standard library");
    test_skip("cxx26:by id and by name");
}
TEST_CASE(text_encoding, copy_keeps_name) {
    TEST_NOTE("<text_encoding> is not shipped by this standard library");
    test_skip("cxx26:copy keeps name");
}
TEST_CASE(text_encoding, several_encodings) {
    TEST_NOTE("<text_encoding> is not shipped by this standard library");
    test_skip("cxx26:several encodings");
}
TEST_CASE(text_encoding, aliases_iteration) {
    TEST_NOTE("<text_encoding> is not shipped by this standard library");
    test_skip("cxx26:aliases iteration");
}
#endif


#if TEST_TARGET_CXX >= 2026 && _TST_TEST_HAS(<text_encoding>)
TEST_CASE(text_encoding, members_cxx26) {
    STD::text_encoding u8(STD::text_encoding::UTF8);

    test_true( u8.mib() == STD::text_encoding::UTF8 );
    test_true( STD::text_encoding().mib() == STD::text_encoding::unknown );
    test_pass("cxx26:text_encoding::mib");

    test_true( STD::strcmp(u8.name(), "UTF-8") == 0 );
    test_true( STD::text_encoding().name()[0] == '\0' );
    test_pass("cxx26:text_encoding::name");

    STD::text_encoding::id which = u8.mib();
    test_true( which == STD::text_encoding::UTF8 );
    test_true( (int)STD::text_encoding::UTF8 == 106 );
    test_true( (int)STD::text_encoding::ASCII == 3 );
    test_pass("cxx26:text_encoding::id enum");

    STD::text_encoding same(STD::string_view("utf-8"));
    test_true( same == u8 );
    test_true( !(same == STD::text_encoding(STD::text_encoding::ASCII)) );
    test_true( !(STD::text_encoding(STD::string_view("x-a")) ==
                 STD::text_encoding(STD::string_view("x-b"))) );
    test_pass("cxx26:text_encoding::operator==");

    test_true( ccw_alias_count(u8.aliases().begin(), u8.aliases().end()) >= (STD::size_t)1 );
    test_true( ccw_alias_has(u8.aliases().begin(), u8.aliases().end(), "UTF-8") );
    test_pass("cxx26:text_encoding::aliases (view)");

    STD::text_encoding env = STD::text_encoding::environment();
    test_true( env.name() != 0 );
    test_pass("cxx26:text_encoding::environment");

    bool is_u8 = STD::text_encoding::environment_is<STD::text_encoding::UTF8>();
    bool is_ascii = STD::text_encoding::environment_is<STD::text_encoding::ASCII>();
    test_true( is_u8 == (env.mib() == STD::text_encoding::UTF8) );
    test_true( is_ascii == (env.mib() == STD::text_encoding::ASCII) );
    test_pass("cxx26:text_encoding::environment_is");

    STD::hash<STD::text_encoding> h;
    test_true( h(u8) == h(same) );
    test_pass("cxx26:hash<text_encoding>");
}
#else
TEST_CASE(text_encoding, members_cxx26) {
    test_skip("cxx26:text_encoding::mib");
    test_skip("cxx26:text_encoding::name");
    test_skip("cxx26:text_encoding::id enum");
    test_skip("cxx26:text_encoding::operator==");
    test_skip("cxx26:text_encoding::aliases (view)");
    test_skip("cxx26:text_encoding::environment");
    test_skip("cxx26:text_encoding::environment_is");
    test_skip("cxx26:hash<text_encoding>");
}
#endif

TEST_CASE(text_encoding, feature_test_macros) {
#if defined(__cpp_lib_text_encoding) && (__cpp_lib_text_encoding) >= 202306L
    test_true( (__cpp_lib_text_encoding) >= 202306L );
    test_pass("cxx26:__cpp_lib_text_encoding=202306L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx26:__cpp_lib_text_encoding=202306L");
#endif
}


#if TEST_TARGET_CXX >= 2026 && _TST_TEST_HAS(<text_encoding>)
TEST_CASE(text_encoding, class_and_limits_cxx26) {
    STD::text_encoding u8(STD::text_encoding::UTF8);
    STD::text_encoding copy = u8;
    test_true( copy == u8 );
    test_true( STD::is_copy_constructible<STD::text_encoding>::value );
    test_pass("cxx26:text_encoding");

    STD::text_encoding::aliases_view av = u8.aliases();
    test_true( ccw_alias_count(av.begin(), av.end()) >= (STD::size_t)1 );
    test_pass("cxx26:text_encoding::aliases_view");

    test_true( STD::text_encoding::max_name_length >= (STD::size_t)10 );
    test_true( STD::strlen(u8.name()) <= STD::text_encoding::max_name_length );
    {
        STD::text_encoding other(STD::string_view("x-some-encoding"));
        test_true( STD::strlen(other.name()) <= STD::text_encoding::max_name_length );
    }
    test_pass("cxx26:text_encoding::max_name_length");

    STD::text_encoding lit = STD::text_encoding::literal();
    test_true( lit.name() != 0 );
    test_true( lit == STD::text_encoding::literal() );
    test_pass("cxx26:text_encoding::literal");
}
#else
TEST_CASE(text_encoding, class_and_limits_cxx26) {
    test_skip("cxx26:text_encoding");
    test_skip("cxx26:text_encoding::aliases_view");
    test_skip("cxx26:text_encoding::max_name_length");
    test_skip("cxx26:text_encoding::literal");
}
#endif
