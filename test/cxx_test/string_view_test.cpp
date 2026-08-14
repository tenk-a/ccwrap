#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2017
#include <string_view>
#include <string>
#include <sstream>
#include <type_traits>
#include <cstddef>
#include <vector>

TEST_CASE(string_view, construct_access) {
    STD::string_view e;
    test_true( e.empty() );
    test_pass("cxx17:string_view()");
    test_eq( e.size(), 0u );
    test_pass("cxx17:string_view::size");

    STD::string_view sv = "hello world";
    test_eq( sv.size(), 11u );
    test_pass("cxx17:string_view(const char*)");
    test_eq( sv.length(), 11u );
    test_pass("cxx17:string_view::length");
    test_true( !sv.empty() );
    test_pass("cxx17:string_view::empty");
    test_true( sv.max_size() > 0u );
    test_pass("cxx17:string_view::max_size");

    test_eq( sv[0], 'h' );
    test_pass("cxx17:string_view::operator[]");
    test_eq( sv.at(1), 'e' );
    test_pass("cxx17:string_view::at");
    test_eq( sv.front(), 'h' );
    test_pass("cxx17:string_view::front");
    test_eq( sv.back(), 'd' );
    test_pass("cxx17:string_view::back");
    test_eq( *sv.data(), 'h' );
    test_pass("cxx17:string_view::data");
    test_throw( (void)sv.at(999) );
    test_pass("cxx17:string_view::at (out_of_range)");

#if !defined(__WATCOMC__)
    STD::string s = "from string";
    STD::string_view v2(s);
    test_eq( v2.size(), 11u );
    test_pass("cxx17:string_view(string)");
#else
    TEST_SKIP1();
#endif
    STD::string_view v3("abcdef", 3);
    test_eq( v3.size(), 3u );
    test_pass("cxx17:string_view(const char*, n)");
    test_true( v3 == STD::string_view("abc") );
    test_pass("cxx17:operator==(string_view)");
    test_true( STD::string_view::npos == (STD::size_t)-1 );
    test_pass("cxx17:string_view::npos");
}

TEST_CASE(string_view, iterate_modify) {
    STD::string_view sv = "abcde";
    int n = 0;
    for (STD::string_view::const_iterator it = sv.begin(); it != sv.end(); ++it) ++n;
    test_eq( n, 5 );
    test_pass("cxx17:string_view::begin");
    test_true( 1 );
    test_pass("cxx17:string_view::end");
    n = 0;
    for (STD::string_view::const_iterator it = sv.cbegin(); it != sv.cend(); ++it) ++n;
    test_eq( n, 5 );
    test_pass("cxx17:string_view::cbegin");
    test_true( 1 );
    test_pass("cxx17:string_view::cend");
    test_eq( *sv.rbegin(), 'e' );
    test_pass("cxx17:string_view::rbegin");
    test_eq( *(sv.rend() - 1), 'a' );
    test_pass("cxx17:string_view::rend");
    test_eq( *sv.crbegin(), 'e' );
    test_pass("cxx17:string_view::crbegin");
    test_eq( *(sv.crend() - 1), 'a' );
    test_pass("cxx17:string_view::crend");

    STD::string_view t = "  trim  ";
    t.remove_prefix(2);
    test_true( t == STD::string_view("trim  ") );
    test_pass("cxx17:string_view::remove_prefix");
    t.remove_suffix(2);
    test_true( t == STD::string_view("trim") );
    test_pass("cxx17:string_view::remove_suffix");

    char buf[4] = { 0, 0, 0, 0 };
    STD::size_t got = t.copy(buf, 3, 0);
    test_eq( got, 3u );
    test_true( STD::string_view(buf, 3) == STD::string_view("tri") );
    test_pass("cxx17:string_view::copy");

    test_true( sv.substr(1, 3) == STD::string_view("bcd") );
    test_pass("cxx17:string_view::substr");
}

TEST_CASE(string_view, search_compare) {
    STD::string_view sv = "abcdefabc";

    test_eq( (int)sv.find("cd"), 2 );
    test_pass("cxx17:string_view::find");
    test_eq( (int)sv.find('a'), 0 );
    test_pass("cxx17:string_view::find (char)");
    test_eq( (int)sv.rfind("abc"), 6 );
    test_pass("cxx17:string_view::rfind");
    test_eq( (int)sv.rfind('a'), 6 );
    test_pass("cxx17:string_view::rfind (char)");
    test_eq( (int)sv.find_first_of("cd"), 2 );
    test_pass("cxx17:string_view::find_first_of");
    test_eq( (int)sv.find_last_of("ab"), 7 );
    test_pass("cxx17:string_view::find_last_of");
    test_eq( (int)sv.find_first_not_of("ab"), 2 );
    test_pass("cxx17:string_view::find_first_not_of");
    test_eq( (int)sv.find_last_not_of("bc"), 6 );
    test_pass("cxx17:string_view::find_last_not_of");
    test_true( sv.find("zzz") == STD::string_view::npos );

    test_true( sv.compare("abcdefabc") == 0 );
    test_true( sv.compare("abd") < 0 );
    test_true( sv.compare(STD::string_view("aaa")) > 0 );
    test_pass("cxx17:string_view::compare");

#if TEST_TARGET_CXX >= 2020 && (__cplusplus < 201703L || _tst_cplusplus >= 202002L)
    test_true( sv.starts_with("abc") );
    test_true( sv.starts_with('a') );
    test_true( !sv.starts_with("xyz") );
    test_pass("cxx20:string_view::starts_with");
    test_true( sv.ends_with("abc") );
    test_true( sv.ends_with('c') );
    test_true( !sv.ends_with("xyz") );
    test_pass("cxx20:string_view::ends_with");
#else
    for (int __i = 0; __i < 6; ++__i) TEST_SKIP1();
#endif
}

TEST_CASE(string_view, relational_swap) {
    STD::string_view a = "abc", b = "abc", c = "abd";
    test_true( a == b );
    test_true( a != c );
    test_pass("cxx17:operator!=(string_view)");
    test_true( a <  c );
    test_pass("cxx17:operator<(string_view)");
    test_true( c >  a );
    test_pass("cxx17:operator>(string_view)");
    test_true( a <= b );
    test_pass("cxx17:operator<=(string_view)");
    test_true( a >= b );
    test_pass("cxx17:operator>=(string_view)");

    STD::string_view x = "one", y = "two";
    x.swap(y);
    test_true( x == STD::string_view("two") );
    test_true( y == STD::string_view("one") );
    test_pass("cxx17:string_view::swap");
}

TEST_CASE(string_view, hash) {
    STD::string_view a = "hello", b = "hello";
    STD::size_t ha = STD::hash<STD::string_view>()(a);
    test_eq( ha, STD::hash<STD::string_view>()(b) );
    test_eq( STD::hash<STD::string_view>()(STD::string_view()), STD::hash<STD::string_view>()(STD::string_view()) );
    test_pass("cxx17:hash<string_view>");
}

TEST_CASE(string_view, u16_u32) {
    const char16_t s16[] = { (char16_t)'a', (char16_t)'b', (char16_t)'c', 0 };
    STD::u16string_view v16(s16);
    test_eq( (int)v16.size(), 3 );
    test_true( v16[0] == (char16_t)'a' );
#if TEST_TARGET_CXX >= 2020 && (__cplusplus < 201703L || _tst_cplusplus >= 202002L)
    test_true( v16.starts_with((char16_t)'a') );
    test_true( v16.ends_with((char16_t)'c') );
#else
    TEST_SKIP_N(2);
#endif
    test_true( v16.find((char16_t)'b') == 1 );
    test_pass("cxx17:u16string_view");

    const char32_t s32[] = { (char32_t)'x', (char32_t)'y', 0 };
    STD::u32string_view v32(s32);
    test_eq( (int)v32.size(), 2 );
#if TEST_TARGET_CXX >= 2020 && (__cplusplus < 201703L || _tst_cplusplus >= 202002L)
    test_true( v32.ends_with((char32_t)'y') );
    test_pass("cxx17:u32string_view");
#else
    TEST_SKIP1();
    test_skip("cxx17:u32string_view");
#endif
}

TEST_CASE(string_view, ostream_insert) {
    STD::ostringstream os;
    STD::string_view sv = "hello";
    os << sv;
    test_eq( os.str(), STD::string("hello") );
    test_pass("cxx17:operator<<(string_view)");

    STD::ostringstream os2;
    os2.width(8);
    os2 << STD::string_view("abc");
    test_eq( os2.str(), STD::string("     abc") );

    STD::ostringstream os3;
    os3.setf(STD::ios_base::left, STD::ios_base::adjustfield);
    os3.width(6);
    os3 << STD::string_view("xy");
    test_eq( os3.str(), STD::string("xy    ") );
    test_pass("cxx17:operator<<(string_view) width/adjust");

    STD::ostringstream os4;
    os4.width(5);
    os4 << STD::string_view("a") << STD::string_view("b");
    test_eq( os4.str(), STD::string("    ab") );
    test_pass("cxx17:operator<<(string_view) width reset");
}

TEST_CASE(string_view, copy_and_typedefs) {
    STD::string_view a = "orig";
    STD::string_view b(a);
    test_true( b == a );
    test_eq( b.size(), 4u );
    test_pass("cxx17:string_view(const string_view&)");

    STD::string_view c;
    c = a;
    test_true( c == a );
    test_pass("cxx17:string_view::operator=");

    STD::wstring_view wv = L"wide";
    test_eq( wv.size(), 4u );
    test_true( wv[0] == L'w' );
    test_pass("cxx17:wstring_view");
}

TEST_CASE(string_view, udl_suffix) {
#if _TST_HAS_UDL && _TST_HAS_CXX17_LIB_MEMBERS
    using namespace STD::literals;
    STD::string_view sv = "lit"sv;
    test_eq( sv.size(), 3u );
    test_true( sv == STD::string_view("lit") );
    test_pass("cxx17:literal operator sv-suffix (string_view UDL)");
#else
    test_skip("cxx17:literal operator sv-suffix (string_view UDL)");
#endif
}

TEST_CASE(string_view, cxx20_ctor_and_types) {
#if !defined(__WATCOMC__) && TEST_TARGET_CXX >= 2020     && (__cplusplus < 201703L || _tst_cplusplus >= 202002L)
    const char* s = "abcdef";
    STD::string_view iv(s, s + 6);
    test_eq( iv.size(), 6u );
    test_true( iv == STD::string_view("abcdef") );
    test_pass("cxx20:string_view(first, last)");
#else
    test_skip("cxx20:string_view(first, last)");
#endif
#if _TST_HAS_CHAR8_NAME
#  if _TST_HAS_CHAR8_T
    STD::u8string_view u8 = u8"u8v";
#  else
    const char8_t __u8lit[4] = { (char8_t)'u', (char8_t)'8', (char8_t)'v', 0 };
    STD::u8string_view u8(__u8lit);
#  endif
    test_eq( u8.size(), 3u );
    test_true( u8[0] == (char8_t)'u' );
    test_pass("cxx20:u8string_view");
#else
    test_skip("cxx20:u8string_view");
#endif
}

TEST_CASE(string_view, spaceship) {
#if defined(__cpp_impl_three_way_comparison) && defined(__cpp_lib_three_way_comparison)
    test_true( (STD::string_view("a") <=> STD::string_view("a")) == 0 );
    test_true( (STD::string_view("a") <=> STD::string_view("b")) <  0 );
    test_true( (STD::string_view("b") <=> STD::string_view("a")) >  0 );
    test_pass("cxx20:operator<=>(string_view)");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx20:operator<=>(string_view)");
#endif
}

TEST_CASE(string_view, cxx23_members) {
#if defined(__cpp_lib_string_contains)
    STD::string_view sv = "hello world";
    test_true(  sv.contains("world") );
    test_true(  sv.contains('h') );
    test_true( !sv.contains("xyz") );
    test_pass("cxx23:string_view::contains");
#else
    test_skip("cxx23:string_view::contains");
#endif

#if defined(__cpp_lib_string_contains) && !defined(__WATCOMC__)

    bool sv_not_from_null = !STD::is_constructible<STD::string_view, STD::nullptr_t>::value;
    test_true( sv_not_from_null );
    test_pass("cxx23:string_view(nullptr_t)=delete");
#else
    test_skip("cxx23:string_view(nullptr_t)=delete");
#endif

#if defined(__cpp_lib_containers_ranges)
    {

        STD::vector<char> src;
        src.push_back('r'); src.push_back('n'); src.push_back('g');
        STD::string_view rv(src);
        test_eq( rv.size(), 3u );
        test_true( rv == STD::string_view("rng") );
        test_pass("cxx23:string_view(R&&) [range]");
    }
#else
    test_skip("cxx23:string_view(R&&) [range]");
#endif
}
#endif


#if TEST_TARGET_CXX >= 2017
TEST_CASE(string_view, member_types_cxx17) {
    typedef STD::basic_string_view<char> SV;
    SV sv("hello");

    test_true(( STD::is_same<STD::string_view, SV>::value ));
    test_true(( STD::is_same<STD::wstring_view, STD::basic_string_view<wchar_t> >::value ));
    test_eq( (int)sv.size(), 5 );
    test_pass("cxx17:basic_string_view");

    test_true(( STD::is_same<SV::traits_type, STD::char_traits<char> >::value ));
    test_pass("cxx17:basic_string_view::traits_type");

    test_true(( STD::is_same<SV::value_type, char>::value ));
    test_pass("cxx17:basic_string_view::value_type");

    test_true(( STD::is_same<SV::pointer, char*>::value ));
    test_true(( STD::is_same<SV::const_pointer, const char*>::value ));
    test_true( *sv.data() == 'h' );
    test_pass("cxx17:basic_string_view::pointer");
    test_pass("cxx17:basic_string_view::const_pointer");

    test_true(( STD::is_same<SV::reference, char&>::value ));
    test_true(( STD::is_same<SV::const_reference, const char&>::value ));
    SV::const_reference cr = sv[1];
    test_eq( cr, 'e' );
    test_pass("cxx17:basic_string_view::reference");
    test_pass("cxx17:basic_string_view::const_reference");

    test_true(( STD::is_same<SV::size_type, STD::size_t>::value ));
    test_pass("cxx17:basic_string_view::size_type");

    test_true( STD::is_signed<SV::difference_type>::value );
    SV::difference_type d = sv.end() - sv.begin();
    test_eq( (long)d, 5L );
    test_pass("cxx17:basic_string_view::difference_type");

    SV::iterator it = sv.begin();
    SV::const_iterator ci = sv.cbegin();
    test_eq( *it, 'h' );
    test_eq( *ci, 'h' );
    test_pass("cxx17:basic_string_view::iterator");
    test_pass("cxx17:basic_string_view::const_iterator");

    SV::reverse_iterator ri = sv.rbegin();
    SV::const_reverse_iterator cri = sv.crbegin();
    test_eq( *ri, 'o' );
    test_eq( *cri, 'o' );
    test_pass("cxx17:basic_string_view::reverse_iterator");
    test_pass("cxx17:basic_string_view::const_reverse_iterator");
}

TEST_CASE(string_view, subview_cxx26) {
#if defined(__cpp_lib_string_view_subview)
    STD::string_view sv("hello");
    test_true( sv.subview(1, 3) == "ell" );
    test_true( sv.subview(2) == "llo" );
    test_pass("cxx26:basic_string_view::subview");
#else
    TEST_NOTE("basic_string_view::subview is C++26; this library does not have it yet");
    test_skip("cxx26:basic_string_view::subview");
#endif
}
#else
TEST_CASE(string_view, member_types_cxx17) {
    test_skip("cxx17:basic_string_view");
    test_skip("cxx17:basic_string_view::traits_type");
    test_skip("cxx17:basic_string_view::value_type");
    test_skip("cxx17:basic_string_view::pointer");
    test_skip("cxx17:basic_string_view::const_pointer");
    test_skip("cxx17:basic_string_view::reference");
    test_skip("cxx17:basic_string_view::const_reference");
    test_skip("cxx17:basic_string_view::size_type");
    test_skip("cxx17:basic_string_view::difference_type");
    test_skip("cxx17:basic_string_view::iterator");
    test_skip("cxx17:basic_string_view::const_iterator");
    test_skip("cxx17:basic_string_view::reverse_iterator");
    test_skip("cxx17:basic_string_view::const_reverse_iterator");
}
TEST_CASE(string_view, subview_cxx26) {
    test_skip("cxx26:basic_string_view::subview");
}
#endif

TEST_CASE(string_view, feature_test_macros) {
#if defined(__cpp_lib_string_view) && (__cpp_lib_string_view) >= 201606L
    test_true( (__cpp_lib_string_view) >= 201606L );
    test_pass("cxx17:__cpp_lib_string_view=201606L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx17:__cpp_lib_string_view=201606L");
#endif
#if defined(__cpp_lib_starts_ends_with) && (__cpp_lib_starts_ends_with) >= 201711L
    test_true( (__cpp_lib_starts_ends_with) >= 201711L );
    test_pass("cxx20:__cpp_lib_starts_ends_with=201711L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx20:__cpp_lib_starts_ends_with=201711L");
#endif
#if defined(__cpp_lib_string_contains) && (__cpp_lib_string_contains) >= 202011L
    test_true( (__cpp_lib_string_contains) >= 202011L );
    test_pass("cxx23:__cpp_lib_string_contains=202011L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_string_contains=202011L");
#endif
#if defined(__cpp_lib_string_view) && (__cpp_lib_string_view) >= 202403L
    test_true( (__cpp_lib_string_view) >= 202403L );
    test_pass("cxx26:__cpp_lib_string_view=202403L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx26:__cpp_lib_string_view=202403L");
#endif
}

TEST_CASE(string_view, compare_and_find_overloads) {
    typedef STD::string_view sv;
    sv a("abcdefabc");

    test_true( a.compare(0, 3, sv("abc")) == 0 );
    test_pass("cxx17:constexpr string_view::compare(size_type pos1, size_type n1, basic_string_view sv)");
    test_true( a.compare(0, 3, sv("xabcx"), 1, 3) == 0 );
    test_pass("cxx17:constexpr string_view::compare(size_type pos1, size_type n1, basic_string_view sv, size_type pos2, size_type n2)");
    test_true( a.compare("abcdefabc") == 0 );
    test_pass("cxx17:constexpr string_view::compare(const charT* s)");
    test_true( a.compare(0, 3, "abc") == 0 );
    test_pass("cxx17:constexpr string_view::compare(size_type pos1, size_type n1, const charT* s)");
    test_true( a.compare(0, 3, "abcxx", 3) == 0 );
    test_pass("cxx17:constexpr string_view::compare(size_type pos1, size_type n1, const charT* s, size_type n2)");

    test_eq( (int)a.find("cd"), 2 );
    test_pass("cxx17:constexpr string_view::find(const charT* s)");
    test_eq( (int)a.find("cdzz", 0, 2), 2 );
    test_pass("cxx17:constexpr string_view::find(const charT* s, size_type pos, size_type n)");
    test_eq( (int)a.rfind("abc"), 6 );
    test_pass("cxx17:constexpr string_view::rfind(const charT* s)");
    test_eq( (int)a.rfind("abczz", sv::npos, 3), 6 );
    test_pass("cxx17:constexpr string_view::rfind(const charT* s, size_type pos, size_type n)");

    test_eq( (int)a.find_first_of('c'), 2 );
    test_pass("cxx17:constexpr string_view::find_first_of(charT c)");
    test_eq( (int)a.find_first_of("cd"), 2 );
    test_pass("cxx17:constexpr string_view::find_first_of(const charT* s)");
    test_eq( (int)a.find_first_of("cdzz", 0, 2), 2 );
    test_pass("cxx17:constexpr string_view::find_first_of(const charT* s, size_type pos, size_type n)");
    test_eq( (int)a.find_last_of('a'), 6 );
    test_pass("cxx17:constexpr string_view::find_last_of(charT c)");
    test_eq( (int)a.find_last_of("ab"), 7 );
    test_pass("cxx17:constexpr string_view::find_last_of(const charT* s)");
    test_eq( (int)a.find_last_of("abzz", sv::npos, 2), 7 );
    test_pass("cxx17:constexpr string_view::find_last_of(const charT* s, size_type pos, size_type n)");

    test_eq( (int)a.find_first_not_of('a'), 1 );
    test_pass("cxx17:constexpr string_view::find_first_not_of(charT c)");
    test_eq( (int)a.find_first_not_of("ab"), 2 );
    test_pass("cxx17:constexpr string_view::find_first_not_of(const charT* s)");
    test_eq( (int)a.find_first_not_of("abzz", 0, 2), 2 );
    test_pass("cxx17:constexpr string_view::find_first_not_of(const charT* s, size_type pos, size_type n)");
    test_eq( (int)a.find_last_not_of('c'), 7 );
    test_pass("cxx17:constexpr string_view::find_last_not_of(charT c)");
    test_eq( (int)a.find_last_not_of("bc"), 6 );
    test_pass("cxx17:constexpr string_view::find_last_not_of(const charT* s)");
    test_eq( (int)a.find_last_not_of("bczz", sv::npos, 2), 6 );
    test_pass("cxx17:constexpr string_view::find_last_not_of(const charT* s, size_type pos, size_type n)");

#if TEST_TARGET_CXX >= 2020 && (__cplusplus < 201703L || _tst_cplusplus >= 202002L)
    test_true( a.starts_with("abc") );
    test_pass("cxx20:constexpr string_view::starts_with(const charT* s)");
    test_true( a.ends_with("abc") );
    test_pass("cxx20:constexpr string_view::ends_with(const charT* s)");
#else
    TEST_SKIP_N(2);
#endif
#if TEST_TARGET_CXX >= 2023 && (__cplusplus < 201703L || _tst_cplusplus >= 202302L)
    test_true( a.contains("def") );
    test_pass("cxx23:constexpr string_view::contains(const charT* s)");
#else
    TEST_SKIP_N(1);
#endif
}
