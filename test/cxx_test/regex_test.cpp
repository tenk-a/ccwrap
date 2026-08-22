#include "test_cxx.hpp"

#include <regex>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cstring>
#include <type_traits>
#include <string>
#include <sstream>

TEST_CASE(regex, search_match_results) {
    STD::regex re("a+(b+)(c+)");
    STD::smatch m;
    STD::string s = "xx aaabbcc yy";
    test_true( STD::regex_search(s, m, re) );
    test_pass("cxx11:regex_search");
    test_eq( (int)m.size(), 3 );
    test_pass("cxx11:match_results::size");
    test_true( !m.empty() );
    test_pass("cxx11:match_results::empty");
    test_eq( m[0].str(), STD::string("aaabbcc") );
    test_pass("cxx11:match_results::operator[]");
    test_eq( m.str(1), STD::string("bb") );
    test_eq( m.str(2), STD::string("cc") );
    test_pass("cxx11:match_results::str");
    test_eq( (int)m.length(1), 2 );
    test_pass("cxx11:match_results::length");
    test_eq( (int)m.position(0), 3 );
    test_pass("cxx11:match_results::position");
    test_eq( m.prefix().str(), STD::string("xx ") );
    test_pass("cxx11:match_results::prefix");
    test_eq( m.suffix().str(), STD::string(" yy") );
    test_pass("cxx11:match_results::suffix");
    test_true( m[1].matched );
    test_pass("cxx11:sub_match::matched");

    test_eq( (int)re.mark_count(), 2 );
    test_pass("cxx11:regex::mark_count");
}

TEST_CASE(regex, match_and_replace) {
    STD::regex re("[0-9]+");
    test_true(  STD::regex_match(STD::string("12345"), re) );
    test_true( !STD::regex_match(STD::string("12a45"), re) );
    test_pass("cxx11:regex_match");

    STD::string in = "a12b345c6";
    test_eq( STD::regex_replace(in, re, STD::string("#")), STD::string("a#b#c#") );
    test_pass("cxx11:regex_replace");

    STD::regex kv("(\\w+)=(\\w+)");
    STD::string out = STD::regex_replace(STD::string("x=1"), kv, STD::string("$2:$1"));
    test_eq( out, STD::string("1:x") );
    test_pass("cxx11:regex_replace#backref");
}

TEST_CASE(regex, iterators) {
    STD::string text = "cat dog cat bird cat";
    STD::regex word("cat");
    int cnt = 0;
    for (STD::sregex_iterator it(text.begin(), text.end(), word), end; it != end; ++it) ++cnt;
    test_eq( cnt, 3 );
    test_pass("cxx11:sregex_iterator");

    STD::string csv = "a,bb,ccc";
    STD::regex comma(",");
    int parts = 0;
    STD::string last;
    for (STD::sregex_token_iterator it(csv.begin(), csv.end(), comma, -1), end; it != end; ++it) {
        ++parts; last = it->str();
    }
    test_eq( parts, 3 );
    test_eq( last, STD::string("ccc") );
    test_pass("cxx11:sregex_token_iterator");
}

TEST_CASE(regex, ecmascript_features) {

    test_true(  STD::regex_match(STD::string("aaa"),  STD::regex("a*")) );
    test_true(  STD::regex_match(STD::string(""),     STD::regex("a*")) );
    test_true( !STD::regex_match(STD::string(""),     STD::regex("a+")) );
    test_true(  STD::regex_match(STD::string("color"), STD::regex("colou?r")) );
    test_true(  STD::regex_match(STD::string("colour"),STD::regex("colou?r")) );
    test_true(  STD::regex_match(STD::string("aaa"),  STD::regex("a{3}")) );
    test_true( !STD::regex_match(STD::string("aa"),   STD::regex("a{3}")) );
    test_true(  STD::regex_match(STD::string("aaaa"), STD::regex("a{2,4}")) );
    test_pass("cxx11:regex#quantifiers");

    test_true(  STD::regex_match(STD::string("cat"),  STD::regex("cat|dog")) );
    test_true(  STD::regex_match(STD::string("dog"),  STD::regex("cat|dog")) );
    test_true(  STD::regex_match(STD::string("abab"), STD::regex("(ab)+")) );
    test_pass("cxx11:regex#alternation");

    test_true(  STD::regex_match(STD::string("7"),    STD::regex("\\d")) );
    test_true( !STD::regex_match(STD::string("x"),    STD::regex("\\d")) );
    test_true(  STD::regex_match(STD::string("_ab9"), STD::regex("\\w+")) );
    test_true(  STD::regex_match(STD::string(" \t"),  STD::regex("\\s+")) );
    test_true(  STD::regex_match(STD::string("m"),    STD::regex("[a-z]")) );
    test_true( !STD::regex_match(STD::string("M"),    STD::regex("[a-z]")) );
    test_true(  STD::regex_match(STD::string("X"),    STD::regex("[^a-z]")) );
    test_pass("cxx11:regex#char_class");

    test_true(  STD::regex_match(STD::string("aXc"),  STD::regex("a.c")) );
    STD::string s = "foo bar";
    test_true(  STD::regex_search(s, STD::regex("^foo")) );
    test_true(  STD::regex_search(s, STD::regex("bar$")) );
    test_true( !STD::regex_search(s, STD::regex("^bar")) );
    test_pass("cxx11:regex#anchors");

    test_true(  STD::regex_search(s, STD::regex("\\bbar\\b")) );
    test_true( !STD::regex_search(STD::string("embark"), STD::regex("\\bbar\\b")) );
    test_true(  STD::regex_search(STD::string("embark"), STD::regex("\\Bbar")) );
    test_pass("cxx11:regex#word_boundary");

    test_true(  STD::regex_match(STD::string("abcabc"), STD::regex("(abc)\\1")) );
    test_true( !STD::regex_match(STD::string("abcxyz"), STD::regex("(abc)\\1")) );
    test_true(  STD::regex_search(STD::string("hello"), STD::regex("(l)\\1")) );
    test_pass("cxx11:regex#backreference");
}

TEST_CASE(regex, match_results_detail) {
    STD::regex re("(\\d+)-(\\d+)");
    STD::smatch m;
    STD::string s = "10-20";
    test_true( STD::regex_match(s, m, re) );
    test_pass("cxx11:regex_match#results");
    test_eq( (int)m.size(), 3 );
    test_pass("cxx11:match_results::size#detail");

    int n = 0;
    for (STD::smatch::iterator it = m.begin(); it != m.end(); ++it) ++n;
    test_eq( n, 3 );
    test_pass("cxx11:smatch::iterator");

    test_true( m[1] == STD::string("10") );
    test_true( STD::string("10") == m[1] );
    test_true( m[2] == "20" );
    test_true( m[1] != m[2] );
    test_pass("cxx11:sub_match::operator==");
    test_eq( m[2].str(), STD::string("20") );
    test_pass("cxx11:sub_match::str");
    test_eq( (int)m[1].length(), 2 );
    test_pass("cxx11:sub_match::length");
    test_true( m[0].matched );
    test_pass("cxx11:sub_match::matched#detail");

    STD::regex opt("(a)(b)?");
    STD::smatch m2;
    STD::string one = "a";
    test_true( STD::regex_match(one, m2, opt) );
    test_true(  m2[1].matched );
    test_true( !m2[2].matched );
    test_pass("cxx11:sub_match#unmatched");

    STD::cmatch cm;
    test_true( STD::regex_search("val=42", cm, STD::regex("=(\\d+)")) );
    test_eq( cm.str(1), STD::string("42") );
    test_pass("cxx11:cmatch");
}

#if TEST_HAS_EH
TEST_CASE(regex, error_and_replace_flags) {

    bool threw = false;
    try { STD::regex bad("("); }
    catch (const STD::regex_error&) { threw = true; }
    catch (const STD::exception&) { threw = true; }
    test_true( threw );
    test_pass("cxx11:regex_error");

    bool threw2 = false;
    try { STD::regex bad2("[abc"); }
    catch (const STD::regex_error&) { threw2 = true; }
    catch (const STD::exception&) { threw2 = true; }
    test_true( threw2 );
    test_pass("cxx11:regex_error#brack");

    STD::regex d("\\d");
    STD::string once = STD::regex_replace(STD::string("a1b2"), d, STD::string("#"),
                                          STD::regex_constants::format_first_only);
    test_eq( once, STD::string("a#b2") );
    test_pass("cxx11:regex_replace#first_only");

    STD::string all = STD::regex_replace(STD::string("a1b2c3"), d, STD::string("#"));
    test_eq( all, STD::string("a#b#c#") );
    test_pass("cxx11:regex_replace#all");

    STD::string only = STD::regex_replace(STD::string("a1b2c3"), d, STD::string("#"),
                                          STD::regex_constants::format_no_copy);
    test_eq( only, STD::string("###") );
    test_pass("cxx11:regex_replace#no_copy");

    STD::string r = STD::regex_replace(STD::string("2021-07-13"), STD::regex("-"),
                                       STD::string("/"));
    test_eq( r, STD::string("2021/07/13") );
    test_pass("cxx11:regex_replace#chars");
}
#else   // !TEST_HAS_EH
TEST_CASE(regex, error_and_replace_flags) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx11:regex_error");
    test_skip("cxx11:regex_error#brack");
    test_skip("cxx11:regex_replace#first_only");
    test_skip("cxx11:regex_replace#all");
    test_skip("cxx11:regex_replace#no_copy");
    test_skip("cxx11:regex_replace#chars");
}
#endif  // TEST_HAS_EH

TEST_CASE(regex, flags) {
    STD::regex ci("hello", STD::regex_constants::icase);
    STD::string hi = "say HELLO";
    test_true(  STD::regex_search(hi, ci) );
    test_true( !STD::regex_search(hi, STD::regex("hello")) );
    test_pass("cxx11:regex_constants::icase");

#if !defined(_MSC_VER) || _MSC_VER >= 1950
    STD::smatch m;
    STD::string s = "line1\nline2";
#if TEST_TARGET_CXX >= 2017
    STD::regex ml("^line2$", STD::regex_constants::multiline);
    test_true( STD::regex_search(s, m, ml) );
    test_pass("cxx11:regex_constants::multiline");
#else
    TEST_NOTE("regex_constants::multiline is C++17");
    test_skip("cxx11:regex_constants::multiline");
#endif
#else
    TEST_SKIP1();
    test_skip("cxx11:regex_constants::multiline");
#endif
}

TEST_CASE(regex, flags_getter_assign_swap) {

    STD::regex a("x+", STD::regex_constants::icase);
    test_true( (a.flags() & STD::regex_constants::icase) != 0 );
    test_pass("cxx11:regex::flags");
    STD::regex b("y+");
    test_true( (b.flags() & STD::regex_constants::icase) == 0 );
    test_pass("cxx11:regex::flags#unset");

    STD::regex r("aaa");
    test_true(  STD::regex_search(STD::string("zaaz"), r) == false );
    r.assign("z+");
    test_true(  STD::regex_search(STD::string("zaaz"), r) );
    test_pass("cxx11:regex::assign");
    test_eq( (int)r.mark_count(), 0 );
    test_pass("cxx11:regex::mark_count#zero");
    r.assign("(a)(b)", STD::regex_constants::ECMAScript);
    test_eq( (int)r.mark_count(), 2 );
    test_pass("cxx11:regex::assign#flags");

    STD::regex q;
    q = "ab+";
    test_true( STD::regex_search(STD::string("xabbbx"), q) );
    test_pass("cxx11:regex::operator=");
    q = STD::string("q");
    test_true( STD::regex_search(STD::string("q"), q) );
    test_pass("cxx11:regex::operator=#string");

    STD::regex p1("hello"), p2("world");
    p1.swap(p2);
    test_true(  STD::regex_search(STD::string("world!"), p1) );
    test_true(  STD::regex_search(STD::string("hello!"), p2) );
    test_pass("cxx11:regex::swap");
    using STD::swap;
    swap(p1, p2);
    test_true(  STD::regex_search(STD::string("hello!"), p1) );
    test_true(  STD::regex_search(STD::string("world!"), p2) );
    test_pass("cxx11:swap");
}

#if TEST_HAS_EH
TEST_CASE(regex, error_and_constants) {
    namespace rc = STD::regex_constants;

    test_true( rc::ECMAScript != rc::icase );
    test_true( (rc::icase | rc::nosubs) != rc::icase );
    test_true( rc::optimize != rc::collate );
    test_true( rc::basic != rc::extended );
    test_true( rc::awk != rc::grep );
    test_true( rc::grep != rc::egrep );
    test_pass("cxx11:regex_constants::syntax_option_type");

    test_true( rc::match_default == 0 );
    test_true( rc::match_not_bow != rc::match_not_eow );
    test_true( rc::match_any != rc::match_not_null );
    test_true( rc::match_continuous != rc::match_prev_avail );
    test_true( rc::format_default == 0 );
    test_true( rc::format_first_only != rc::format_no_copy );
    test_pass("cxx11:regex_constants::match_flag_type");

    bool caught = false;
    try {
        STD::regex bad("(unclosed");
        (void)bad;
    } catch (const STD::regex_error& e) {
        caught = true;
        test_true( e.code() == rc::error_paren );
        test_pass("cxx11:regex_error::code");
        test_ptr( e.what() );
        test_true( STD::strlen(e.what()) > (STD::size_t)sizeof("regex_error") - 1 );
        test_pass("cxx11:regex_error::what");
        const STD::runtime_error& as_rt = e;
        test_ptr( as_rt.what() );
        test_pass("cxx11:regex_error#base");
    }
    test_true( caught );
    test_pass("cxx11:regex_error#caught");

    bool caught2 = false;
    try { STD::regex bad2("[abc"); (void)bad2; }
    catch (const STD::regex_error& e) {
        caught2 = true;
        test_true( e.code() == rc::error_brack );
        test_pass("cxx11:regex_constants::error_type");
    }
    test_true( caught2 );
    test_pass("cxx11:regex_error#unbalanced_bracket");
}
#else   // !TEST_HAS_EH
TEST_CASE(regex, error_and_constants) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx11:regex_constants::syntax_option_type");
    test_skip("cxx11:regex_constants::match_flag_type");
    test_skip("cxx11:regex_error::code");
    test_skip("cxx11:regex_error::what");
    test_skip("cxx11:regex_error#base");
    test_skip("cxx11:regex_error#caught");
    test_skip("cxx11:regex_constants::error_type");
    test_skip("cxx11:regex_error#unbalanced_bracket");
}
#endif  // TEST_HAS_EH

TEST_CASE(regex, traits) {
    STD::regex_traits<char> t;

    test_eq( (int)STD::regex_traits<char>::length("abcd"), 4 );
    test_pass("cxx11:regex_traits::length");
    test_true( t.translate('x') == 'x' );
    test_pass("cxx11:regex_traits::translate");
    test_true( t.translate_nocase('X') == 'x' );
    test_true( t.translate_nocase('x') == 'x' );
    test_pass("cxx11:regex_traits::translate_nocase");

    STD::string ab("Ab"), other("cd");
    const char* abb = ab.data();     const char* abe = abb + ab.size();
    const char* otb = other.data();  const char* ote = otb + other.size();
    test_true( t.transform(abb, abe) == t.transform(abb, abe) );
    test_true( t.transform(abb, abe) != t.transform(otb, ote) );
    test_pass("cxx11:regex_traits::transform");
    test_true( t.transform_primary(abb, abe) == t.transform_primary(abb, abe) );
    TEST_SKIP_LIBCXX("libc++ transform_primary returns an empty key for every sequence");
    test_true( t.transform_primary(abb, abe) != t.transform_primary(otb, ote) );
    test_pass("cxx11:regex_traits::transform_primary");

    STD::string one("q");
    test_true( t.lookup_collatename(one.begin(), one.end()) == STD::string("q") );
    STD::string many("comma");
    TEST_SKIP_VC120("MSVC regex_traits returns any name verbatim instead of validating it");
    test_true( t.lookup_collatename(many.begin(), many.end()) == STD::string(",") );
    STD::string nosuch("notaname");
    TEST_SKIP_VC120("MSVC regex_traits returns any name verbatim instead of validating it");
    test_true( t.lookup_collatename(nosuch.begin(), nosuch.end()).empty() );
    test_pass("cxx11:regex_traits::lookup_collatename");

    STD::string d("digit"), al("alpha"), sp("space"), up("upper"), xd("xdigit");
    STD::regex_traits<char>::char_class_type cd = t.lookup_classname(d.begin(), d.end());
    STD::regex_traits<char>::char_class_type ca = t.lookup_classname(al.begin(), al.end());
    STD::regex_traits<char>::char_class_type cs = t.lookup_classname(sp.begin(), sp.end());
    STD::regex_traits<char>::char_class_type cx = t.lookup_classname(xd.begin(), xd.end());
    test_true( cd != 0 && ca != 0 && cs != 0 && cx != 0 );
    test_pass("cxx11:regex_traits::lookup_classname");

    test_true(  t.isctype('7', cd) );
    test_true( !t.isctype('x', cd) );
    test_true(  t.isctype('x', ca) );
    test_true( !t.isctype('7', ca) );
    test_true(  t.isctype(' ', cs) );
    test_true( !t.isctype('x', cs) );
    test_true(  t.isctype('e', cx) );
    test_true(  t.isctype('9', cx) );
    test_true( !t.isctype('g', cx) );
    test_pass("cxx11:regex_traits::isctype");

    STD::string junk("nosuchclass");
    test_true( t.lookup_classname(junk.begin(), junk.end()) == 0 );
    test_pass("cxx11:regex_traits::lookup_classname#unknown");

    STD::regex_traits<char>::char_class_type cu_i = t.lookup_classname(up.begin(), up.end(), true);
    test_true( t.isctype('a', cu_i) );
    test_true( t.isctype('A', cu_i) );
    test_pass("cxx11:regex_traits::lookup_classname#icase");

    test_eq( t.value('7', 10), 7 );
    test_eq( t.value('f', 16), 15 );
    test_eq( t.value('F', 16), 15 );
    test_eq( t.value('8', 8), -1 );
    test_eq( t.value('z', 16), -1 );
    test_pass("cxx11:regex_traits::value");

    STD::regex_traits<char>::locale_type cur = t.getloc();
    STD::regex_traits<char>::locale_type old = t.imbue(cur);
    t.imbue(old);
    test_true( true );
    test_pass("cxx11:regex_traits::imbue");
}

TEST_CASE(regex, basic_regex_members) {

    test_true( (STD::is_same<STD::regex, STD::basic_regex<char> >::value) );
    test_pass("cxx11:regex");
    test_true( (STD::is_same<STD::regex::value_type, char>::value) );
    test_pass("cxx11:regex::value_type");
    test_true( (STD::is_same<STD::regex::string_type, STD::string>::value) );
    test_pass("cxx11:regex::string_type");

    test_true( STD::regex::icase == STD::regex_constants::icase );
    test_pass("cxx11:regex::icase");
    test_true( STD::regex::optimize == STD::regex_constants::optimize );
    test_pass("cxx11:regex::optimize");
    test_true( STD::regex::egrep == STD::regex_constants::egrep );
    test_pass("cxx11:regex::egrep");

    STD::regex a("a+bXXXX", 3, STD::regex_constants::ECMAScript);
    test_true( STD::regex_match("aab", a) );
    test_pass("cxx11:basic_regex");
    STD::string pat("x[0-9]y");
    STD::regex b(pat.begin(), pat.end());
    test_true( STD::regex_match("x7y", b) );
    test_pass("cxx11:basic_regex#ctor_iter");

    STD::regex c;
    c.assign("z+q", 3, STD::regex_constants::ECMAScript);
    test_true( STD::regex_match("zzq", c) );
    test_pass("cxx11:basic_regex::assign#ptr_len");
    c.assign(pat.begin(), pat.end());
    test_true( STD::regex_match("x0y", c) );
    test_pass("cxx11:basic_regex::assign#iter");

    STD::regex::locale_type cur = c.getloc();
    STD::regex::locale_type old = c.imbue(cur);
    c.imbue(old);
    test_true( true );
    test_pass("cxx11:basic_regex::imbue");

    STD::regex o("ab", STD::regex_constants::ECMAScript | STD::regex_constants::optimize);
    test_true( STD::regex_match("ab", o) );
    test_pass("cxx11:basic_regex#optimize_collate");
}

TEST_CASE(regex, sub_match_ordering_and_aliases) {
    STD::cmatch m;
    test_true( STD::regex_search("hello world", m, STD::regex("(\\w+) (\\w+)")) );
    test_pass("cxx11:sub_match");

    test_true( (STD::is_same<STD::csub_match, STD::sub_match<const char*> >::value) );
    test_pass("cxx11:csub_match");
    test_true( (STD::is_same<STD::ssub_match,
                             STD::sub_match<STD::string::const_iterator> >::value) );
    test_pass("cxx11:ssub_match");

    STD::csub_match s1 = m[1];
    STD::csub_match s2 = m[2];

    test_true( s1.str() == STD::string("hello") );
    test_true( s2.str() == STD::string("world") );
    test_pass("cxx11:sub_match::str#alias");
    test_eq( (int)s1.length(), 5 );
    test_pass("cxx11:sub_match::length#alias");
    test_true( s1.matched );
    test_pass("cxx11:sub_match::matched#alias");

    test_true( s1 == STD::string("hello") );
    test_true( STD::string("hello") == s1 );
    test_true( s1 == "hello" );
    test_true( "hello" == s1 );
    test_true( s1 != s2 );
    test_pass("cxx11:sub_match::operator==#forms");

    test_true( s1 < s2 );
    test_true( s2 > s1 );
    test_true( s1 <= s2 );
    test_true( s2 >= s1 );
    test_pass("cxx11:sub_match::operator<");
    test_true( s1 < STD::string("world") );
    test_true( s1 < "world" );
    test_pass("cxx11:sub_match::operator<#string");
    test_true( s1.str() <= s1.str() );
    test_true( STD::string("apple") < s1.str() );
    test_pass("cxx11:sub_match::operator<#str");

    test_true( s1.compare(s2) < 0 );
    test_true( s1.compare(STD::string("hello")) == 0 );
    test_true( s1.compare("hello") == 0 );
    test_pass("cxx11:sub_match::compare");
}

TEST_CASE(regex, match_results_format) {
    STD::cmatch m;
    test_true( STD::regex_search("2026-07-21", m, STD::regex("(\\d+)-(\\d+)-(\\d+)")) );

    test_true( m.format("$3/$2/$1") == STD::string("21/07/2026") );
    test_pass("cxx11:match_results::format");
    test_true( m.format("[$&]") == STD::string("[2026-07-21]") );
    test_true( m.format("$$") == STD::string("$") );
    test_true( m.format("$x") == STD::string("$x") );
    test_pass("cxx11:match_results::format#specials");

    STD::cmatch m2;
    test_true( STD::regex_search("ab12cd", m2, STD::regex("\\d+")) );
    test_true( m2.format("<$`|$&|$'>") == STD::string("<ab|12|cd>") );
    test_pass("cxx11:match_results::format#prefix_suffix");

    test_true( m2.format("plain", STD::regex_constants::format_sed) == STD::string("plain") );
    test_pass("cxx11:match_results::format#sed");

    STD::string out;
    m.format(STD::back_inserter(out), STD::string("$1"));
    test_true( out == STD::string("2026") );
    test_pass("cxx11:match_results::format#iterator");

    test_true( (STD::is_same<STD::cmatch::string_type, STD::string>::value) );
    test_pass("cxx11:cmatch::string_type");
    STD::cmatch m3((STD::cmatch::allocator_type()));
    test_true( !m3.ready() );
    test_pass("cxx11:match_results::ready");
    test_true( m3.max_size() > 0 );
    test_pass("cxx11:match_results::max_size");

    STD::cmatch a, b;
    test_true( STD::regex_search("zz9", a, STD::regex("\\d")) );
    test_true( !b.ready() );
    a.swap(b);
    test_true( b.ready() );
    test_true( !a.ready() );
    test_true( b.str(0) == STD::string("9") );
    test_pass("cxx11:match_results::swap");
}

TEST_CASE(regex, iterator_members) {
    STD::string text("a1 b2 c3");
    STD::regex re("([a-z])(\\d)");

    STD::sregex_iterator it(text.begin(), text.end(), re), last;
    STD::sregex_iterator copy(it);
    test_true( copy == it );
    test_pass("cxx11:sregex_iterator#copy_ctor");
    STD::sregex_iterator prev = it++;
    test_true( prev->str(0) == STD::string("a1") );
    test_true( it->str(0) == STD::string("b2") );
    test_pass("cxx11:sregex_iterator::operator++");

    STD::sregex_iterator assigned;
    assigned = it;
    test_true( assigned->str(0) == STD::string("b2") );
    test_pass("cxx11:sregex_iterator#copy_assign");

    int n = 0;
    for (STD::sregex_iterator i(text.begin(), text.end(), re); i != last; ++i) ++n;
    test_eq( n, 3 );
    test_pass("cxx11:sregex_iterator#loop");

    STD::sregex_token_iterator t(text.begin(), text.end(), re, 1), tend;
    STD::sregex_token_iterator tcopy(t);
    test_true( tcopy->str() == STD::string("a") );
    test_pass("cxx11:sregex_token_iterator#copy_ctor");
    STD::sregex_token_iterator tprev = t++;
    test_true( tprev->str() == STD::string("a") );
    test_true( t->str() == STD::string("b") );
    test_pass("cxx11:sregex_token_iterator::operator++");
    STD::sregex_token_iterator tassigned;
    tassigned = t;
    test_true( tassigned->str() == STD::string("b") );
    test_pass("cxx11:sregex_token_iterator#copy_assign");

    int tn = 0;
    for (STD::sregex_token_iterator i(text.begin(), text.end(), re, 2); i != tend; ++i) ++tn;
    test_eq( tn, 3 );
    test_pass("cxx11:sregex_token_iterator#loop");
}

TEST_CASE(regex, match_flag_constants) {
    namespace rc = STD::regex_constants;
    STD::string s("abc");

    rc::match_flag_type base = rc::match_default;
    test_true( (base | rc::match_not_bol)    != base );  test_pass("cxx11:regex_constants::match_not_bol");
    test_true( (base | rc::match_not_eol)    != base );  test_pass("cxx11:regex_constants::match_not_eol");
    test_true( (base | rc::match_not_bow)    != base );  test_pass("cxx11:regex_constants::match_not_bow");
    test_true( (base | rc::match_not_eow)    != base );  test_pass("cxx11:regex_constants::match_not_eow");
    test_true( (base | rc::match_any)        != base );  test_pass("cxx11:regex_constants::match_any");
    test_true( (base | rc::match_not_null)   != base );  test_pass("cxx11:regex_constants::match_not_null");
    test_true( (base | rc::match_prev_avail) != base );  test_pass("cxx11:regex_constants::match_prev_avail");

    test_true( !STD::regex_search(s, STD::regex("b"), rc::match_continuous) );
    test_pass("cxx11:regex_constants::match_continuous");

    STD::regex rn("(a)(b)", rc::nosubs);
    test_true( STD::regex_search(s, rn) );
    test_pass("cxx11:regex_constants::nosubs");
    STD::regex rcol("a", rc::collate);
    test_true( STD::regex_search(s, rcol) );
    test_pass("cxx11:regex_constants::collate");

    STD::regex_constants::syntax_option_type gb = rc::basic, ge = rc::extended,
        ga = rc::awk, gg = rc::grep;
    (void)ga; (void)gg;
    STD::regex rb("a", gb), rex("a", ge);
    test_true( STD::regex_search(s, rb) );
    test_true( STD::regex_search(s, rex) );
    test_pass("cxx11:regex_constants grammar flags (basic/extended/awk/grep)");
}

TEST_CASE(regex, wide_regex) {
    STD::wregex wre(L"a+");
    STD::wstring ws(L"baaad");
    STD::wsmatch wm;
    test_true( STD::regex_search(ws, wm, wre) );
    test_true( wm[0].str() == L"aaa" );
    test_pass("cxx11:wregex");
    test_pass("cxx11:wsmatch");

    STD::wcmatch wcm;
    test_true( STD::regex_search(L"baaad", wcm, wre) );
    test_true( wcm[0].str() == L"aaa" );
    test_pass("cxx11:wcmatch");

    STD::wstring ws2(L"a1a2a3");
    STD::wregex dig(L"a");
    STD::wsregex_iterator wit(ws2.begin(), ws2.end(), dig), wend;
    int n = 0;
    for (; wit != wend; ++wit) ++n;
    test_eq( n, 3 );
    test_pass("cxx11:wsregex_iterator");

    const wchar_t* cs = L"a.a.a";
    STD::wcregex_iterator cit(cs, cs + 5, dig), cend;
    int cn = 0;
    for (; cit != cend; ++cit) ++cn;
    test_eq( cn, 3 );
    test_pass("cxx11:wcregex_iterator");

    STD::wsregex_token_iterator wtit(ws2.begin(), ws2.end(), dig, 0), wtend;
    int wtn = 0;
    for (; wtit != wtend; ++wtit) ++wtn;
    test_eq( wtn, 3 );
    test_pass("cxx11:wsregex_token_iterator");

    STD::wcregex_token_iterator ctit(cs, cs + 5, dig, 0), ctend;
    int ctn = 0;
    for (; ctit != ctend; ++ctit) ++ctn;
    test_eq( ctn, 3 );
    test_pass("cxx11:wcregex_token_iterator");

    STD::wssub_match wss = wm[0];
    test_true( wss.matched );
    test_true( wss.str() == L"aaa" );
    test_eq( (long)wss.length(), 3L );
    test_pass("cxx11:wssub_match");

    STD::wcsub_match wcs = wcm[0];
    test_true( wcs.matched );
    test_true( wcs.str() == L"aaa" );
    test_pass("cxx11:wcsub_match");
}

TEST_CASE(regex, cregex_iterators_and_split) {
    const char* text = "a1b2c3";
    STD::regex letter("[a-c]");

    STD::cregex_iterator it(text, text + 6, letter), end;
    int n = 0;
    for (; it != end; ++it) ++n;
    test_eq( n, 3 );
    test_pass("cxx11:cregex_iterator");

    STD::cregex_token_iterator tit(text, text + 6, letter, 0), tend;
    int tn = 0;
    for (; tit != tend; ++tit) ++tn;
    test_eq( tn, 3 );
    test_pass("cxx11:cregex_token_iterator");

    STD::string csv("x,y,z");
    STD::regex comma(",");
    STD::sregex_token_iterator sit(csv.begin(), csv.end(), comma, -1), send;
    STD::string first = sit->str();
    test_true( first == "x" );
    int parts = 0;
    for (; sit != send; ++sit) ++parts;
    test_eq( parts, 3 );
    test_pass("cxx11:sregex_token_iterator (-1 split)");
}

TEST_CASE(regex, submatch_ops_and_match_results) {
    STD::string s("hello");
    STD::regex re("(hel)(lo)");
    STD::smatch m;
    test_true( STD::regex_match(s, m, re) );

    const STD::ssub_match& sm = m[1];
    test_true(   sm <= "hel" );
    test_true( !(sm >  "hel") );
    test_true(   sm >= "hel" );
    test_true(   sm <= "hem" );
    test_true(   sm <  "hem" );
    test_pass("cxx11:sub_match::operator<=");
    test_pass("cxx11:sub_match::operator>");
    test_pass("cxx11:sub_match::operator>=");

    STD::ostringstream os;
    os << m[2];
    test_true( os.str() == "lo" );
    test_pass("cxx11:sub_match::operator<< (stream)");

    int cnt = 0;
    for (STD::smatch::iterator it = m.begin(); it != m.end(); ++it) ++cnt;
    test_eq( cnt, 3 );
    test_pass("cxx11:match_results::begin");
    test_pass("cxx11:match_results::end");

    (void)m.get_allocator();
    test_true( true );
    test_pass("cxx11:match_results::get_allocator");

    STD::smatch m2;
    test_true( STD::regex_match(s, m2, re) );
#if _TST_HAS_CXX11_LIB_MEMBERS && !defined(__WATCOMC__)
    test_true( m == m2 );
    test_pass("cxx11:match_results::operator==");
#else
    TEST_SKIP1();
    test_skip("cxx11:match_results::operator==");
#endif
}

TEST_CASE(regex, getloc_and_ilist_assign) {
    STD::regex re("abc");
    (void)re.getloc();
    test_true( true );
    test_pass("cxx11:basic_regex::getloc");

    STD::regex_traits<char> tr;
    (void)tr.getloc();
    test_true( true );
    test_pass("cxx11:regex_traits::getloc");

#if _tst_cplusplus >= 201103L
    STD::regex re2("x");
    re2 = { 'a', 'b', 'c' };
    STD::string s("zzabczz");
    test_true( STD::regex_search(s, re2) );
    STD::smatch m;
    test_true( STD::regex_search(s, m, re2) && m[0].str() == "abc" );
    test_pass("cxx11:basic_regex::operator=(initializer_list)");
#else
    TEST_SKIP_N(2);
    test_skip("cxx11:basic_regex::operator=(initializer_list)");
#endif
}

TEST_CASE(regex, member_typedefs_cxx11) {
    typedef STD::regex R;

    test_true(( STD::is_same<R::traits_type, STD::regex_traits<char> >::value ));
    test_true(( STD::is_same<R::value_type, char>::value ));
    test_pass("cxx11:basic_regex::traits_type");
    test_true(( STD::is_same<R::flag_type, STD::regex_constants::syntax_option_type>::value ));
    R re("a");
    test_true( tst_type_is<R::flag_type>(re.flags()) );
    test_pass("cxx11:basic_regex::flag_type");
    test_true(( STD::is_same<R::locale_type, STD::regex_traits<char>::locale_type>::value ));
    test_true( tst_type_is<R::locale_type>(re.getloc()) );
    test_pass("cxx11:basic_regex::locale_type");

    typedef STD::regex_traits<char> T;
    test_true(( STD::is_same<T::char_type, char>::value ));
    test_pass("cxx11:regex_traits::char_type");
    test_true(( STD::is_same<T::string_type, STD::basic_string<char> >::value ));
    test_pass("cxx11:regex_traits::string_type");
    test_true(( STD::is_same<T::locale_type, STD::locale>::value ));
    test_pass("cxx11:regex_traits::locale_type");
    {
        T t;
        const char w[] = "w";
        T::char_class_type cc = t.lookup_classname(w, w + 1);
        test_true( tst_type_is<T::char_class_type>(cc) );
        test_true( t.isctype('a', cc) );
        test_true( !t.isctype('!', cc) );
        test_pass("cxx11:regex_traits::char_class_type");
    }

    typedef STD::ssub_match SM;
    test_true(( STD::is_same<SM::iterator, STD::string::const_iterator>::value ));
    test_pass("cxx11:sub_match::iterator");
    test_true(( STD::is_same<SM::value_type, char>::value ));
    test_pass("cxx11:sub_match::value_type");
    test_true(( STD::is_same<SM::string_type, STD::string>::value ));
    test_pass("cxx11:sub_match::string_type");
    test_true(( STD::is_same<SM::difference_type,
                             STD::iterator_traits<SM::iterator>::difference_type>::value ));
    test_true( STD::is_signed<SM::difference_type>::value );
    test_pass("cxx11:sub_match::difference_type");

    typedef STD::smatch MR;
    test_true(( STD::is_same<MR::value_type, STD::ssub_match>::value ));
    test_true(( STD::is_same<MR::char_type, char>::value ));
    test_pass("cxx11:match_results::char_type");
    test_true(( STD::is_same<MR::difference_type, STD::ssub_match::difference_type>::value ));
    test_pass("cxx11:match_results::difference_type");
    test_true(( STD::is_same<MR::size_type,
                STD::allocator_traits<MR::allocator_type>::size_type>::value ));
    test_pass("cxx11:match_results::size_type");
    test_true(( STD::is_same<MR::allocator_type,
                             STD::allocator<STD::ssub_match> >::value ));
    test_pass("cxx11:match_results::allocator_type");
    test_true(( STD::is_same<MR::reference, STD::ssub_match&>::value ));
    test_true(( !STD::is_same<MR::reference, MR::const_reference>::value ));
    test_pass("cxx11:match_results::reference");
    test_true(( STD::is_same<MR::const_reference, const STD::ssub_match&>::value ));
    test_pass("cxx11:match_results::const_reference");

    {
        STD::regex r("(a)(b)");
        STD::string s("ab");
        MR m;
        test_true( STD::regex_match(s, m, r) );
        test_true(( STD::is_same<MR::const_iterator, MR::iterator>::value ));
        test_pass("cxx11:match_results::const_iterator");
        test_true( m.cbegin() == m.begin() );
        test_eq( m.cbegin()->str(), STD::string("ab") );
        test_pass("cxx11:match_results::cbegin");
        test_true( m.cend() == m.end() );
        test_eq( (long)(m.cend() - m.cbegin()), (long)m.size() );
        test_pass("cxx11:match_results::cend");
    }
}

TEST_CASE(regex, syntax_option_constants_cxx11) {
    namespace rc = STD::regex_constants;

    {
        STD::regex re("a+", rc::ECMAScript);
        test_true( STD::regex_match(STD::string("aaa"), re) );
        TEST_SKIP_LIBCXX("libc++ defines regex_constants::ECMAScript as 0");
        test_true( (re.flags() & rc::ECMAScript) != 0 );
        test_pass("cxx11:regex_constants::ECMAScript");
    }
    {
        STD::regex re("a+", rc::basic);
        test_true( STD::regex_match(STD::string("a+"), re) );
        test_true( !STD::regex_match(STD::string("aaa"), re) );
        test_pass("cxx11:regex_constants::basic");
    }
    {
        STD::regex re("a+", rc::extended);
        test_true( STD::regex_match(STD::string("aaa"), re) );
        test_pass("cxx11:regex_constants::extended");
    }
    {
        STD::regex re("a+", rc::awk);
        test_true( STD::regex_match(STD::string("aaa"), re) );
        test_pass("cxx11:regex_constants::awk");
    }
    {
        STD::regex re("a+", rc::grep);
        test_true( STD::regex_match(STD::string("a+"), re) );
        test_pass("cxx11:regex_constants::grep");
    }
    {
        STD::regex re("a+", rc::egrep);
        test_true( STD::regex_match(STD::string("aaa"), re) );
        test_pass("cxx11:regex_constants::egrep");
    }
}

TEST_CASE(regex, match_and_format_flag_constants_cxx11) {
    namespace rc = STD::regex_constants;
    STD::regex re("b");
    STD::string s("abcb");

    test_eq( (int)rc::match_default, 0 );
    test_true( STD::regex_search(s, re, rc::match_default) );
    test_pass("cxx11:regex_constants::match_default");

    test_eq( (int)rc::format_default, 0 );
    test_eq( STD::regex_replace(s, re, STD::string("[$&]")), STD::string("a[b]c[b]") );
    test_eq( STD::regex_replace(s, re, STD::string("[$&]"), rc::format_default),
             STD::string("a[b]c[b]") );
    test_pass("cxx11:regex_constants::format_default");

    test_eq( STD::regex_replace(s, re, STD::string("[&]"), rc::format_sed),
             STD::string("a[b]c[b]") );
    test_pass("cxx11:regex_constants::format_sed");

    test_eq( STD::regex_replace(s, re, STD::string("X"), rc::format_no_copy),
             STD::string("XX") );
    test_pass("cxx11:regex_constants::format_no_copy");

    test_eq( STD::regex_replace(s, re, STD::string("X"), rc::format_first_only),
             STD::string("aXcb") );
    test_pass("cxx11:regex_constants::format_first_only");
}

#if TEST_HAS_EH
namespace {
STD::regex_constants::error_type ccw_re_err(const char* pat,
        STD::regex_constants::syntax_option_type f = STD::regex_constants::ECMAScript) {
    try { STD::regex re(pat, f); (void)re; }
    catch (const STD::regex_error& e) { return e.code(); }
    return STD::regex_constants::error_complexity;
}
}

TEST_CASE(regex, error_type_constants_cxx11) {
    namespace rc = STD::regex_constants;

    test_true( rc::error_collate != rc::error_ctype );
    test_true( rc::error_escape != rc::error_backref );
    test_true( rc::error_brack != rc::error_paren );
    test_true( rc::error_brace != rc::error_badbrace );
    test_true( rc::error_range != rc::error_space );
    test_true( rc::error_badrepeat != rc::error_complexity );
    test_true( rc::error_stack != rc::error_collate );

    test_true( ccw_re_err("[[.notaname.]]") == rc::error_collate );
    test_pass("cxx11:regex_constants::error_collate");
    TEST_SKIP_GCC("libstdc++ reports error_collate for an unknown character class");
    test_true( ccw_re_err("[[:notaclass:]]") == rc::error_ctype );
    test_pass("cxx11:regex_constants::error_ctype");
    test_true( ccw_re_err("a\\") == rc::error_escape );
    test_pass("cxx11:regex_constants::error_escape");
    test_true( ccw_re_err("(a)\\2") == rc::error_backref );
    test_pass("cxx11:regex_constants::error_backref");
    test_true( ccw_re_err("[a") == rc::error_brack );
    test_pass("cxx11:regex_constants::error_brack");
    test_true( ccw_re_err("(a") == rc::error_paren );
    test_pass("cxx11:regex_constants::error_paren");
    test_true( ccw_re_err("a{1", rc::extended) == rc::error_brace );
    test_pass("cxx11:regex_constants::error_brace");
    test_true( ccw_re_err("a{3,1}") == rc::error_badbrace );
    test_pass("cxx11:regex_constants::error_badbrace");
    TEST_SKIP_LIBCXX("libc++ accepts the reversed range [z-a] instead of throwing");
    test_true( ccw_re_err("[z-a]") == rc::error_range );
    test_pass("cxx11:regex_constants::error_range");
    TEST_SKIP_GCC("libstdc++ reports error_paren for a leading repeat");
    test_true( ccw_re_err("+a") == rc::error_badrepeat );
    test_pass("cxx11:regex_constants::error_badrepeat");

    test_true( (int)rc::error_space != (int)rc::error_stack );
    test_pass("cxx11:regex_constants::error_space");
    test_true( (int)rc::error_complexity != (int)rc::error_stack );
    test_pass("cxx11:regex_constants::error_complexity");
    test_true( (int)rc::error_stack != (int)rc::error_badrepeat );
    test_pass("cxx11:regex_constants::error_stack");
}
#else   // !TEST_HAS_EH
TEST_CASE(regex, error_type_constants_cxx11) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx11:regex_constants::error_collate");
    test_skip("cxx11:regex_constants::error_ctype");
    test_skip("cxx11:regex_constants::error_escape");
    test_skip("cxx11:regex_constants::error_backref");
    test_skip("cxx11:regex_constants::error_brack");
    test_skip("cxx11:regex_constants::error_paren");
    test_skip("cxx11:regex_constants::error_brace");
    test_skip("cxx11:regex_constants::error_badbrace");
    test_skip("cxx11:regex_constants::error_range");
    test_skip("cxx11:regex_constants::error_badrepeat");
    test_skip("cxx11:regex_constants::error_space");
    test_skip("cxx11:regex_constants::error_complexity");
    test_skip("cxx11:regex_constants::error_stack");
}
#endif  // TEST_HAS_EH

TEST_CASE(regex, regex_iterator_surface_cxx11) {
    typedef STD::sregex_iterator IT;

    test_true(( STD::is_same<IT::regex_type, STD::regex>::value ));
    test_pass("cxx11:regex_iterator::regex_type");
    test_true(( STD::is_same<IT::value_type, STD::smatch>::value ));
    test_true(( STD::is_same<IT::difference_type, STD::ptrdiff_t>::value ));
    test_pass("cxx11:regex_iterator::difference_type");
    test_true(( STD::is_same<IT::pointer, const STD::smatch*>::value ));
    test_pass("cxx11:regex_iterator::pointer");
    test_true(( STD::is_same<IT::reference, const STD::smatch&>::value ));
    test_pass("cxx11:regex_iterator::reference");
    test_true(( STD::is_same<IT::iterator_category, STD::forward_iterator_tag>::value ));
    test_pass("cxx11:regex_iterator::iterator_category");

    STD::string s("a1b22c333");
    STD::regex re("[0-9]+");
    IT it(s.begin(), s.end(), re), last;
    test_true( it != last );

    const STD::smatch& m = *it;
    test_eq( m.str(), STD::string("1") );
#if _TST_HAS_DECLTYPE
    test_true(( STD::is_same<decltype(*it), IT::reference>::value ));
    test_pass("cxx11:regex_iterator::operator*");
#else
    TEST_SKIP1();
    test_skip("cxx11:regex_iterator::operator*");
#endif

    test_eq( it->str(), m.str() );
    test_true( tst_type_is<IT::pointer>(it.operator->()) );
    test_pass("cxx11:regex_iterator::operator->");

    int n = 0;
    for (IT walk(s.begin(), s.end(), re); walk != last; ++walk) ++n;
    test_eq( n, 3 );
    test_true( IT() == last );
    test_pass("cxx11:regex_iterator");

#if defined(__cpp_lib_ranges) && TEST_TARGET_CXX >= 2023
    test_true(( STD::is_same<IT::iterator_concept, STD::input_iterator_tag>::value ));
    test_pass("cxx23:regex_iterator::iterator_concept");
#else
    TEST_NOTE("iterator_concept on the regex iterators is C++23 (P2210)");
    test_skip("cxx23:regex_iterator::iterator_concept");
#endif
}

TEST_CASE(regex, regex_token_iterator_surface_cxx11) {
    typedef STD::sregex_token_iterator IT;

    test_true(( STD::is_same<IT::regex_type, STD::regex>::value ));
    test_pass("cxx11:regex_token_iterator::regex_type");
    test_true(( STD::is_same<IT::value_type, STD::ssub_match>::value ));
    test_true(( STD::is_same<IT::difference_type, STD::ptrdiff_t>::value ));
    test_pass("cxx11:regex_token_iterator::difference_type");
    test_true(( STD::is_same<IT::pointer, const STD::ssub_match*>::value ));
    test_pass("cxx11:regex_token_iterator::pointer");
    test_true(( STD::is_same<IT::reference, const STD::ssub_match&>::value ));
    test_pass("cxx11:regex_token_iterator::reference");
    test_true(( STD::is_same<IT::iterator_category, STD::forward_iterator_tag>::value ));
    test_pass("cxx11:regex_token_iterator::iterator_category");
    STD::string s("a,b,c");
    STD::regex comma(",");
    {
        IT it(s.begin(), s.end(), comma, -1), last;
        test_true( it != last );
        const STD::ssub_match& sm = *it;
        test_eq( sm.str(), STD::string("a") );
#if _TST_HAS_DECLTYPE
        test_true(( STD::is_same<decltype(*it), IT::reference>::value ));
        test_pass("cxx11:regex_token_iterator::operator*");
#else
        TEST_SKIP1();
        test_skip("cxx11:regex_token_iterator::operator*");
#endif
        test_eq( it->str(), sm.str() );
        test_true( tst_type_is<IT::pointer>(it.operator->()) );
        test_pass("cxx11:regex_token_iterator::operator->");

        int n = 0;
        STD::string joined;
        for (IT walk(s.begin(), s.end(), comma, -1); walk != last; ++walk) {
            ++n;
            joined += walk->str();
        }
        test_eq( n, 3 );
        test_eq( joined, STD::string("abc") );
        test_true( IT() == last );
        test_pass("cxx11:regex_token_iterator");
    }

#if defined(__cpp_lib_ranges) && TEST_TARGET_CXX >= 2023
    test_true(( STD::is_same<IT::iterator_concept, STD::input_iterator_tag>::value ));
    test_pass("cxx23:regex_token_iterator::iterator_concept");
#else
    TEST_NOTE("iterator_concept on the regex iterators is C++23 (P2210)");
    test_skip("cxx23:regex_token_iterator::iterator_concept");
#endif
}

TEST_CASE(regex, reuse_in_loops) {
    STD::regex re("([a-z]+)([0-9]+)");
    STD::smatch m;

    STD::string subject;
    for (int i = 0; i < 40; ++i) {
        subject += "ab";
        subject += (char)('0' + (i % 10));
        subject += ' ';
    }

    int hits = 0;
    STD::string rest = subject;
    while (STD::regex_search(rest, m, re)) {
        ++hits;
        test_true( m.size() == 3 );
        test_true( m[1].matched );
        rest = m.suffix().str();
    }
    test_eq( hits, 40 );

    int iter_hits = 0;
    for (STD::sregex_iterator it(subject.begin(), subject.end(), re), e; it != e; ++it) {
        ++iter_hits;
        test_true( (*it)[2].matched );
    }
    test_eq( iter_hits, 40 );

    STD::string out;
    for (int i = 0; i < 40; ++i)
        out = STD::regex_replace(subject, re, STD::string("<$2$1>"));
    test_true( out.find("<0ab>") == 0 );
    test_eq( (int)STD::count(out.begin(), out.end(), '<'), 40 );

    STD::vector<STD::smatch> saved;
    for (STD::sregex_iterator it(subject.begin(), subject.end(), re), e; it != e; ++it)
        saved.push_back(*it);
    test_eq( (int)saved.size(), 40 );
    test_true( saved[0][1].str() == STD::string("ab") );
    test_true( saved[39][2].str() == STD::string("9") );
    saved.clear();
    test_true( saved.empty() );
}

TEST_CASE(regex, sub_match_compare_forms_cxx11) {
    STD::string text("abc");
    STD::regex re("(a)(bc)");
    STD::smatch m;
    test_true( STD::regex_search(text, m, re) );

    {   STD::string same("bc"), less("bd"), more("bb");
        test_eq( m[2].compare(same), 0 );
        test_true( m[2].compare(less) < 0 );
        test_true( m[2].compare(more) > 0 ); }
    test_pass("cxx11:sub_match::compare(const string_type&)");

    {   test_eq( m[2].compare("bc"), 0 );
        test_true( m[2].compare("bd") < 0 );
        test_true( m[2].compare("bb") > 0 );
        test_true( m[2].compare("b") > 0 ); }
    test_pass("cxx11:sub_match::compare(const value_type*)");
}
