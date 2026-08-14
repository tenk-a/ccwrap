#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2023 && _TST_TEST_HAS(<stacktrace>) && _TST_HAS_CXX20_LIB_MEMBERS
#include <stacktrace>
#include <sstream>
#include <string>
#include <memory_resource>
#include <type_traits>
#include <utility>
#include <functional>

namespace {
STD::stacktrace ccw_capture() { return STD::stacktrace::current(); }
}

TEST_CASE(stacktrace, entry_cxx23) {
    STD::stacktrace_entry e;
    test_true( !e );
    test_pass("cxx23:stacktrace_entry");
    test_pass("cxx23:stacktrace_entry::stacktrace_entry (default ctor)");
    test_pass("cxx23:stacktrace_entry::operator bool");

    test_true( tst_type_is<STD::stacktrace_entry::native_handle_type>(e.native_handle()) );
    test_pass("cxx23:stacktrace_entry::native_handle");

    test_true( sizeof(STD::stacktrace_entry::native_handle_type) > 0 );
    test_pass("cxx23:stacktrace_entry::native_handle_type");

    STD::stacktrace_entry e2;
    test_true( e == e2 );
    test_pass("cxx23:stacktrace_entry::operator==");

#if _TST_HAS_THREE_WAY
    test_true( (e <=> e2) == 0 );
    test_pass("cxx23:stacktrace_entry::operator<=>");
#else
    TEST_NOTE("no operator<=> in this language mode");
    TEST_SKIP1();
    test_skip("cxx23:stacktrace_entry::operator<=>");
#endif

    STD::stacktrace st = ccw_capture();
    test_true( !st.empty() );
    STD::stacktrace_entry live = st[0];
    test_true( (bool)live );

    STD::string d = live.description();
    test_true( d.size() >= 0u );
    test_pass("cxx23:stacktrace_entry::description");

    STD::string f = live.source_file();
    test_true( f.size() >= 0u );
    test_pass("cxx23:stacktrace_entry::source_file");

    test_true( live.source_line() >= 0u );
    test_pass("cxx23:stacktrace_entry::source_line");
}

TEST_CASE(stacktrace, capture_and_size_cxx23) {
    STD::stacktrace st = STD::stacktrace::current();
    test_true( !st.empty() );
    test_pass("cxx23:basic_stacktrace");
    test_pass("cxx23:basic_stacktrace::current");

    {   STD::stacktrace all = STD::stacktrace::current();
        STD::stacktrace skipped = STD::stacktrace::current(1);
        test_true( skipped.size() <= all.size() );
        STD::stacktrace skip_all = STD::stacktrace::current(1000000);
        test_true( skip_all.empty() ); }
    test_pass("cxx23:basic_stacktrace::current(size_type skip)");

    {   STD::stacktrace one = STD::stacktrace::current(0, 1);
        test_true( one.size() <= (STD::size_t)1 );
        STD::stacktrace none = STD::stacktrace::current(0, 0);
        test_true( none.empty() );
        STD::stacktrace many = STD::stacktrace::current(0, 1000000);
        test_true( many.size() >= one.size() ); }
    test_pass("cxx23:basic_stacktrace::current(size_type skip, size_type max_depth)");
    test_pass("cxx23:stacktrace (alias)");

    test_true( st.size() >= 1u );
    test_pass("cxx23:basic_stacktrace::size");
    test_pass("cxx23:basic_stacktrace::empty");

    STD::stacktrace empty_st;
    test_true( empty_st.empty() );
    test_eq( empty_st.size(), (STD::size_t)0 );
    test_pass("cxx23:basic_stacktrace::basic_stacktrace (default ctor)");

    test_true( st.max_size() >= st.size() );
    test_pass("cxx23:basic_stacktrace::max_size");

    STD::stacktrace deep = ccw_capture();
    STD::stacktrace skipped = STD::stacktrace::current(1);
    test_true( deep.size() >= 1u );
    test_true( skipped.size() + 1u <= deep.size() + 1u );
}

#if TEST_HAS_EH
TEST_CASE(stacktrace, access_cxx23) {
    STD::stacktrace st = ccw_capture();
    test_true( !st.empty() );

    STD::stacktrace_entry a = st[0];
    test_true( (bool)a );
    test_pass("cxx23:basic_stacktrace::operator[]");

    STD::stacktrace_entry b = st.at(0);
    test_true( a == b );
    test_pass("cxx23:basic_stacktrace::at");

    bool threw = false;
    try { (void)st.at(st.size() + 100); } catch (const STD::out_of_range&) { threw = true; }
    test_true( threw );
}
#else   // !TEST_HAS_EH
TEST_CASE(stacktrace, access_cxx23) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx23:basic_stacktrace::operator[]");
    test_skip("cxx23:basic_stacktrace::at");
}
#endif  // TEST_HAS_EH

TEST_CASE(stacktrace, iterators_cxx23) {
    STD::stacktrace st = ccw_capture();

    STD::size_t n = 0;
    for (STD::stacktrace::const_iterator i = st.begin(); i != st.end(); ++i) ++n;
    test_eq( n, st.size() );
    test_pass("cxx23:basic_stacktrace::begin");
    test_pass("cxx23:basic_stacktrace::end");
    test_pass("cxx23:basic_stacktrace::const_iterator");
    test_pass("cxx23:basic_stacktrace::iterator");

    n = 0;
    for (STD::stacktrace::const_iterator i = st.cbegin(); i != st.cend(); ++i) ++n;
    test_eq( n, st.size() );
    test_pass("cxx23:basic_stacktrace::cbegin");
    test_pass("cxx23:basic_stacktrace::cend");

    n = 0;
    for (STD::stacktrace::const_reverse_iterator i = st.rbegin(); i != st.rend(); ++i) ++n;
    test_eq( n, st.size() );
    test_pass("cxx23:basic_stacktrace::rbegin");
    test_pass("cxx23:basic_stacktrace::rend");
    test_pass("cxx23:basic_stacktrace::reverse_iterator");
    test_pass("cxx23:basic_stacktrace::const_reverse_iterator");

    n = 0;
    for (STD::stacktrace::const_reverse_iterator i = st.crbegin(); i != st.crend(); ++i) ++n;
    test_eq( n, st.size() );
    test_pass("cxx23:basic_stacktrace::crbegin");
    test_pass("cxx23:basic_stacktrace::crend");

    test_true( *st.rbegin() == st[st.size() - 1] );
}

TEST_CASE(stacktrace, member_types_cxx23) {
    typedef STD::stacktrace S;
    test_true( (STD::is_same<S::value_type, STD::stacktrace_entry>::value) );
    test_pass("cxx23:basic_stacktrace::value_type");

    test_true( (STD::is_same<S::const_reference, const STD::stacktrace_entry&>::value) );
    test_pass("cxx23:basic_stacktrace::const_reference");

    test_true( (STD::is_same<S::reference, STD::stacktrace_entry&>::value) );
    test_pass("cxx23:basic_stacktrace::reference");

    const S sized;
    test_true( tst_type_is<S::size_type>(sized.size()) );
    test_true( !STD::is_signed<S::size_type>::value );
    test_pass("cxx23:basic_stacktrace::size_type");

    test_true( STD::is_signed<S::difference_type>::value );
    test_true( STD::is_integral<S::difference_type>::value );
    test_pass("cxx23:basic_stacktrace::difference_type");

    test_true( (STD::is_same<S::allocator_type,
                             STD::allocator<STD::stacktrace_entry> >::value) );
    test_pass("cxx23:basic_stacktrace::allocator_type");

    S st = ccw_capture();
    S::allocator_type al = st.get_allocator();
    (void)al;
    test_true( true );
    test_pass("cxx23:basic_stacktrace::get_allocator");

#if _TST_HAS_ALIAS_TEMPLATE
    typedef STD::pmr::stacktrace P;
    test_true( (STD::is_same<P::allocator_type,
                             STD::pmr::polymorphic_allocator<STD::stacktrace_entry> >::value) );
    test_pass("cxx23:pmr::stacktrace (alias)");
#else
    TEST_NOTE("no alias templates: pmr::stacktrace cannot be named");
    TEST_SKIP1();
    test_skip("cxx23:pmr::stacktrace (alias)");
#endif
}

TEST_CASE(stacktrace, copy_swap_compare_cxx23) {
    STD::stacktrace st = ccw_capture();

    STD::stacktrace cp(st);
    test_true( cp == st );
    test_pass("cxx23:basic_stacktrace::basic_stacktrace (copy/move ctor)");
    test_pass("cxx23:basic_stacktrace::operator==");

    STD::stacktrace mv(STD::move(cp));
    test_true( mv == st );

    STD::stacktrace as;
    as = st;
    test_true( as == st );
    STD::stacktrace as2;
    as2 = STD::move(as);
    test_true( as2 == st );
    test_pass("cxx23:basic_stacktrace::operator= (copy/move)");

    STD::stacktrace empty_st;
#if _TST_HAS_THREE_WAY
    test_true( (st <=> mv) == 0 );
    test_true( (empty_st <=> st) != 0 );
    test_pass("cxx23:basic_stacktrace::operator<=>");
#else
    TEST_NOTE("no operator<=> in this language mode");
    TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx23:basic_stacktrace::operator<=>");
#endif

    STD::stacktrace lhs = st, rhs;
    lhs.swap(rhs);
    test_true( rhs == st );
    test_true( lhs.empty() );
    test_pass("cxx23:basic_stacktrace::swap (member)");

    STD::swap(lhs, rhs);
    test_true( lhs == st );
    test_pass("cxx23:swap(basic_stacktrace&, basic_stacktrace&)");

    STD::allocator<STD::stacktrace_entry> alloc;
    STD::stacktrace with_alloc(alloc);
    test_true( with_alloc.empty() );
    test_pass("cxx23:basic_stacktrace::basic_stacktrace (allocator ctor)");

    {
        STD::stacktrace scoped = ccw_capture();
        test_true( !scoped.empty() );
    }
    test_true( true );
    test_pass("cxx23:basic_stacktrace::~basic_stacktrace");
}

TEST_CASE(stacktrace, formatting_cxx23) {
    STD::stacktrace st = ccw_capture();

    STD::string s = STD::to_string(st);
    test_true( !s.empty() );
    test_pass("cxx23:to_string(const basic_stacktrace&)");

    STD::string e = STD::to_string(st[0]);
    test_true( !e.empty() );
    test_pass("cxx23:to_string(const stacktrace_entry&)");

    STD::ostringstream os;
    os << st;
    test_true( !os.str().empty() );
    test_pass("cxx23:operator<<(ostream&, const basic_stacktrace&)");

    STD::ostringstream os2;
    os2 << st[0];
    test_true( !os2.str().empty() );
    test_pass("cxx23:operator<<(ostream&, const stacktrace_entry&)");
}

TEST_CASE(stacktrace, hash_cxx23) {
    STD::stacktrace st = ccw_capture();

    STD::hash<STD::stacktrace_entry> he;
    test_true( he(st[0]) == he(st[0]) );
    test_pass("cxx23:hash<stacktrace_entry>");

    STD::hash<STD::stacktrace> hs;
    STD::stacktrace cp(st);
    test_true( hs(st) == hs(cp) );
    test_pass("cxx23:hash<basic_stacktrace<Allocator>>");
}

#else
TEST_CASE(stacktrace, entry_cxx23) {
    test_skip("cxx23:stacktrace_entry");
    test_skip("cxx23:stacktrace_entry::stacktrace_entry (default ctor)");
    test_skip("cxx23:stacktrace_entry::operator bool");
    test_skip("cxx23:stacktrace_entry::native_handle");
    test_skip("cxx23:stacktrace_entry::native_handle_type");
    test_skip("cxx23:stacktrace_entry::operator==");
    test_skip("cxx23:stacktrace_entry::operator<=>");
    test_skip("cxx23:stacktrace_entry::description");
    test_skip("cxx23:stacktrace_entry::source_file");
    test_skip("cxx23:stacktrace_entry::source_line");
}
TEST_CASE(stacktrace, capture_and_size_cxx23) {
    test_skip("cxx23:basic_stacktrace");
    test_skip("cxx23:basic_stacktrace::current");
    test_skip("cxx23:stacktrace (alias)");
    test_skip("cxx23:basic_stacktrace::size");
    test_skip("cxx23:basic_stacktrace::empty");
    test_skip("cxx23:basic_stacktrace::basic_stacktrace (default ctor)");
    test_skip("cxx23:basic_stacktrace::max_size");
}
TEST_CASE(stacktrace, access_cxx23) {
    test_skip("cxx23:basic_stacktrace::operator[]");
    test_skip("cxx23:basic_stacktrace::at");
}
TEST_CASE(stacktrace, iterators_cxx23) {
    test_skip("cxx23:basic_stacktrace::begin");
    test_skip("cxx23:basic_stacktrace::end");
    test_skip("cxx23:basic_stacktrace::const_iterator");
    test_skip("cxx23:basic_stacktrace::iterator");
    test_skip("cxx23:basic_stacktrace::cbegin");
    test_skip("cxx23:basic_stacktrace::cend");
    test_skip("cxx23:basic_stacktrace::rbegin");
    test_skip("cxx23:basic_stacktrace::rend");
    test_skip("cxx23:basic_stacktrace::reverse_iterator");
    test_skip("cxx23:basic_stacktrace::const_reverse_iterator");
    test_skip("cxx23:basic_stacktrace::crbegin");
    test_skip("cxx23:basic_stacktrace::crend");
}
TEST_CASE(stacktrace, member_types_cxx23) {
    test_skip("cxx23:basic_stacktrace::value_type");
    test_skip("cxx23:basic_stacktrace::const_reference");
    test_skip("cxx23:basic_stacktrace::reference");
    test_skip("cxx23:basic_stacktrace::size_type");
    test_skip("cxx23:basic_stacktrace::difference_type");
    test_skip("cxx23:basic_stacktrace::allocator_type");
    test_skip("cxx23:basic_stacktrace::get_allocator");
    test_skip("cxx23:pmr::stacktrace (alias)");
}
TEST_CASE(stacktrace, copy_swap_compare_cxx23) {
    test_skip("cxx23:basic_stacktrace::basic_stacktrace (copy/move ctor)");
    test_skip("cxx23:basic_stacktrace::operator==");
    test_skip("cxx23:basic_stacktrace::operator= (copy/move)");
    test_skip("cxx23:basic_stacktrace::operator<=>");
    test_skip("cxx23:basic_stacktrace::swap (member)");
    test_skip("cxx23:swap(basic_stacktrace&, basic_stacktrace&)");
    test_skip("cxx23:basic_stacktrace::basic_stacktrace (allocator ctor)");
    test_skip("cxx23:basic_stacktrace::~basic_stacktrace");
}
TEST_CASE(stacktrace, formatting_cxx23) {
    test_skip("cxx23:to_string(const basic_stacktrace&)");
    test_skip("cxx23:to_string(const stacktrace_entry&)");
    test_skip("cxx23:operator<<(ostream&, const basic_stacktrace&)");
    test_skip("cxx23:operator<<(ostream&, const stacktrace_entry&)");
}
TEST_CASE(stacktrace, hash_cxx23) {
    test_skip("cxx23:hash<stacktrace_entry>");
    test_skip("cxx23:hash<basic_stacktrace<Allocator>>");
}
#endif

TEST_CASE(stacktrace, feature_test_macros) {
#if defined(__cpp_lib_stacktrace) && (__cpp_lib_stacktrace) >= 202011L
    test_true( (__cpp_lib_stacktrace) >= 202011L );
    test_pass("cxx23:__cpp_lib_stacktrace=202011L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_stacktrace=202011L");
#endif
}
