#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2020
#include <span>
#include <stdexcept>
#include <vector>

TEST_CASE(span, view_and_subviews) {
    int a[5] = { 10, 20, 30, 40, 50 };
    STD::span<int> sp(a, 5);
    test_eq( sp.size(), 5u );
    test_eq( sp[0], 10 );
    test_eq( sp.front(), 10 );
    test_eq( sp.back(), 50 );
    test_true( !sp.empty() );

    sp[1] = 21;
    test_eq( a[1], 21 );
    test_pass("cxx20:span view");

    STD::span<int> f = sp.first(2);
    test_eq( f.size(), 2u );
    test_eq( f[0], 10 );
    test_pass("cxx20:span first");

    STD::span<int> l = sp.last(2);
    test_eq( l.size(), 2u );
    test_eq( l[1], 50 );
    test_pass("cxx20:span last");

    STD::span<int> ss = sp.subspan(1, 2);
    test_eq( ss.size(), 2u );
    test_eq( ss[0], 21 );
    test_eq( ss[1], 30 );
    test_pass("cxx20:span subspan");
}

TEST_CASE(span, iterate) {
    int a[5] = { 10, 21, 30, 40, 50 };
    STD::span<int> sp(a, 5);
    int sum = 0;
    for (STD::span<int>::iterator it = sp.begin(); it != sp.end(); ++it) sum += *it;
    test_eq( sum, 10 + 21 + 30 + 40 + 50 );
    test_eq( sp.size_bytes(), 5 * sizeof(int) );
    test_pass("cxx20:span iteration");
}

TEST_CASE(span, ctors_data_reverse) {
    int a[4] = { 1, 2, 3, 4 };

    STD::span<int> e;
    test_true( e.empty() );
    test_eq( e.size(), 0u );

    STD::span<int> fl(a, a + 4);
    test_eq( fl.size(), 4u );
    test_eq( fl.back(), 4 );

    STD::vector<int> v;
    v.push_back(7); v.push_back(8); v.push_back(9);
    STD::span<int> sv(STD::data(v), v.size());
    test_eq( sv.size(), 3u );
    test_eq( sv[2], 9 );
    sv[0] = 70;
    test_eq( v[0], 70 );

    const int c[3] = { 5, 6, 7 };
    STD::span<const int> cs(c, 3);
    test_eq( cs.front(), 5 );
    test_eq( cs.back(), 7 );
    test_pass("cxx20:span ctors");

    test_eq( fl.data(), a );
    test_eq( *fl.data(), 1 );

    STD::span<int> tail = fl.subspan(2);
    test_eq( tail.size(), 2u );
    test_eq( tail[0], 3 );
    test_pass("cxx20:span data");

    long rev = 0;
    for (STD::span<int>::reverse_iterator it = fl.rbegin(); it != fl.rend(); ++it) rev = rev * 10 + *it;
    test_eq( rev, 4321L );
    test_pass("cxx20:span reverse iterators");
}

TEST_CASE(span, extent_at_bytes) {
    int a[5] = { 10, 20, 30, 40, 50 };
    STD::span<int> s(a, 5);

    test_true( STD::dynamic_extent == (STD::size_t)-1 );
    STD::size_t ex = STD::span<int>::extent;
    test_true( ex == STD::dynamic_extent );
    test_pass("cxx20:span extent");

#if TEST_TARGET_CXX >= 2026
    test_eq( s.at(0), 10 );
    test_eq( s.at(4), 50 );
    test_throw( (void)s.at(5) );
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
#endif

#if _TST_HAS_BYTE
    STD::span<const STD::byte> b = STD::as_bytes(s);
    test_eq( (int)b.size(), (int)(5 * sizeof(int)) );
    STD::span<STD::byte> wb = STD::as_writable_bytes(s);
    test_eq( (int)wb.size(), (int)(5 * sizeof(int)) );

    wb[0] = STD::byte((unsigned char)0xFF);
    test_true( a[0] != 10 );
    test_pass("cxx20:span as_bytes");
#else
    TEST_SKIP_N(3);
    test_skip("cxx20:span as_bytes");
#endif
}

TEST_CASE(span, member_types_and_ops) {
    typedef STD::span<int> S;
    test_true(( STD::is_same<S::element_type, int>::value ));       test_pass("cxx20:span::element_type");
    test_true(( STD::is_same<S::value_type, int>::value ));         test_pass("cxx20:span::value_type");
    test_true(( STD::is_same<S::size_type, STD::size_t>::value ));  test_pass("cxx20:span::size_type");
    test_true(( STD::is_same<S::difference_type, STD::ptrdiff_t>::value )); test_pass("cxx20:span::difference_type");
    test_true(( STD::is_same<S::pointer, int*>::value ));           test_pass("cxx20:span::pointer");
    test_true(( STD::is_same<S::reference, int&>::value ));         test_pass("cxx20:span::reference");

    int a[5] = { 1, 2, 3, 4, 5 };
    S s(a, 5);
    test_true( !s.empty() );
    S e;
    test_true( e.empty() );
    test_pass("cxx20:span::empty");
    test_eq( (int)s.size_bytes(), (int)(5 * sizeof(int)) );
    test_pass("cxx20:span::size_bytes");

    S s2;
    s2 = s;
    test_true( s2.data() == a && s2.size() == 5 );
    test_pass("cxx20:span::operator=(const span&)");

    STD::vector<int> v; v.push_back(7); v.push_back(8);
    S sr(v);
    test_true( sr.size() == 2 && sr[1] == 8 );
    test_pass("cxx20:span::span(R&&)");

    STD::span<int, 5> fixed(a);
    S dyn(fixed);
    test_true( dyn.size() == 5 && dyn[0] == 1 );
    test_pass("cxx20:span::span(const span<U,N>&) converting");

#if _TST_HAS_BYTE
    STD::span<STD::byte> wb = STD::as_writable_bytes(s);
    test_eq( (int)wb.size(), (int)(5 * sizeof(int)) );
    test_pass("cxx20:as_writable_bytes");
#else
    TEST_SKIP1();
    test_skip("cxx20:as_writable_bytes");
#endif

#if defined(__cpp_lib_ranges)
    test_true( STD::ranges::enable_borrowed_range<S> );
    test_pass("cxx20:enable_borrowed_range<span>");
#else
    TEST_SKIP1();
    test_skip("cxx20:enable_borrowed_range<span>");
#endif
}

TEST_CASE(span, cxx23_const_iterators) {
#if defined(__cpp_lib_ranges_as_const) || (defined(__cpp_lib_span) && __cpp_lib_span >= 202311L)
    int a[4] = { 10, 20, 30, 40 };
    STD::span<int> s(a, 4);
    test_true( tst_type_is<STD::span<int>::const_iterator>(s.cbegin()) );
    test_pass("cxx23:span::const_iterator");
    test_true( tst_type_is<STD::span<int>::const_reverse_iterator>(s.crbegin()) );
    test_pass("cxx23:span::const_reverse_iterator");
    test_eq( *s.cbegin(), 10 );
    test_true( s.cbegin() != s.cend() );
    test_pass("cxx23:span::cbegin");
    test_pass("cxx23:span::cend");
    test_eq( *s.crbegin(), 40 );
    test_true( s.crbegin() != s.crend() );
    test_pass("cxx23:span::crbegin");
    test_pass("cxx23:span::crend");
#else
    test_skip("cxx23:span::const_iterator"); test_skip("cxx23:span::const_reverse_iterator");
    test_skip("cxx23:span::cbegin"); test_skip("cxx23:span::cend");
    test_skip("cxx23:span::crbegin"); test_skip("cxx23:span::crend");
#endif
}
#endif


#if TEST_TARGET_CXX >= 2020
TEST_CASE(span, element_access_cxx20) {
    int a[5] = {1, 2, 3, 4, 5};
    STD::span<int> s(a, 5);

    test_eq( (int)s.size(), 5 );
    test_true( tst_type_is<STD::size_t>(s.size()) );
    test_pass("cxx20:span::size");

    test_eq( s[0], 1 );
    s[0] = 9;
    test_eq( a[0], 9 );
    test_pass("cxx20:span::operator[]");

    test_eq( s.front(), 9 );
    s.front() = 1;
    test_eq( a[0], 1 );
    test_pass("cxx20:span::front");

    test_eq( s.back(), 5 );
    s.back() = 6;
    test_eq( a[4], 6 );
    test_pass("cxx20:span::back");

    test_true( STD::dynamic_extent == (STD::size_t)-1 );
    test_true( STD::span<int>::extent == STD::dynamic_extent );
    test_true( (STD::span<int, 5>::extent) == (STD::size_t)5 );
    test_pass("cxx20:dynamic_extent");
}

TEST_CASE(span, iterators_cxx20) {
    int a[4] = {1, 2, 3, 4};
    STD::span<int> s(a, 4);

    STD::span<int>::iterator it = s.begin();
    test_eq( *it, 1 );
    *it = 7;
    test_eq( a[0], 7 );
    test_pass("cxx20:span::begin");
    test_pass("cxx20:span::iterator");

    test_eq( (int)(s.end() - s.begin()), 4 );
    test_pass("cxx20:span::end");

    STD::span<int>::reverse_iterator ri = s.rbegin();
    test_eq( *ri, 4 );
    test_pass("cxx20:span::rbegin");
    test_pass("cxx20:span::reverse_iterator");

    test_eq( (int)(s.rend() - s.rbegin()), 4 );
    test_pass("cxx20:span::rend");

    test_true(( STD::is_same<STD::span<int>::const_pointer, const int*>::value ));
    STD::span<int>::const_pointer cp = s.data();
    test_eq( *cp, 7 );
    test_pass("cxx20:span::const_pointer");

    test_true(( STD::is_same<STD::span<int>::const_reference, const int&>::value ));
    STD::span<int>::const_reference cr = s[1];
    test_eq( cr, 2 );
    test_pass("cxx20:span::const_reference");
}
#else
TEST_CASE(span, element_access_cxx20) {
    test_skip("cxx20:span::size");
    test_skip("cxx20:span::operator[]");
    test_skip("cxx20:span::front");
    test_skip("cxx20:span::back");
    test_skip("cxx20:dynamic_extent");
}
TEST_CASE(span, iterators_cxx20) {
    test_skip("cxx20:span::begin");
    test_skip("cxx20:span::iterator");
    test_skip("cxx20:span::end");
    test_skip("cxx20:span::rbegin");
    test_skip("cxx20:span::reverse_iterator");
    test_skip("cxx20:span::rend");
    test_skip("cxx20:span::const_pointer");
    test_skip("cxx20:span::const_reference");
}
#endif

TEST_CASE(span, feature_test_macros) {
#if defined(__cpp_lib_span) && (__cpp_lib_span) >= 202002L
    test_true( (__cpp_lib_span) >= 202002L );
    test_pass("cxx20:__cpp_lib_span=202002L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx20:__cpp_lib_span=202002L");
#endif
#if defined(__cpp_lib_span) && (__cpp_lib_span) >= 202311L
    test_true( (__cpp_lib_span) >= 202311L );
    test_pass("cxx26:__cpp_lib_span=202311L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx26:__cpp_lib_span=202311L");
#endif
}


#if TEST_HAS_EH
TEST_CASE(span, at_cxx26) {
#if defined(__cpp_lib_span_at)
    int a[4] = {1, 2, 3, 4};
    STD::span<int> s(a, 4);
    test_eq( s.at(0), 1 );
    test_eq( s.at(3), 4 );
    s.at(1) = 9;
    test_eq( a[1], 9 );
    test_pass("cxx26:span::at");

    bool threw = false;
    try { (void)s.at(4); } catch (const STD::out_of_range&) { threw = true; }
    test_true( threw );
    threw = false;
    try { (void)s.at((STD::size_t)-1); } catch (const STD::out_of_range&) { threw = true; }
    test_true( threw );
    test_pass("cxx26:span::at (out_of_range)");
#else
    TEST_NOTE("span::at is C++26 (P2821); not in this library");
    test_skip("cxx26:span::at");
    test_skip("cxx26:span::at (out_of_range)");
#endif
}
#else   // !TEST_HAS_EH
TEST_CASE(span, at_cxx26) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx26:span::at");
    test_skip("cxx26:span::at (out_of_range)");
}
#endif  // TEST_HAS_EH

#if TEST_TARGET_CXX >= 2020

static int g_sp_live = 0;

struct SpLive {
    char* p_;
    SpLive() : p_(new char[32]) { p_[0] = 0; ++g_sp_live; }
    SpLive(char c) : p_(new char[32]) { p_[0] = c; p_[1] = 0; ++g_sp_live; }
    SpLive(const SpLive& o) : p_(new char[32]) {
        for (int i = 0; i < 32; ++i) p_[i] = o.p_[i];
        ++g_sp_live;
    }
    SpLive& operator=(const SpLive& o) {
        if (this != &o) for (int i = 0; i < 32; ++i) p_[i] = o.p_[i];
        return *this;
    }
    ~SpLive() { delete[] p_; --g_sp_live; }
    char tag() const { return p_[0]; }
    bool operator==(const SpLive& o) const { return p_[0] == o.p_[0]; }
    bool operator< (const SpLive& o) const { return p_[0] <  o.p_[0]; }
};

TEST_CASE(span, class_elements_non_owning) {
    g_sp_live = 0;
    {
        STD::vector<SpLive> v;
        for (int i = 0; i < 5; ++i) v.push_back(SpLive((char)('a' + i)));
        test_eq( g_sp_live, 5 );

        STD::span<SpLive> s(&v[0], v.size());
        test_eq( (int)s.size(), 5 );
        test_eq( g_sp_live, 5 );

        STD::span<SpLive> c(s);
        test_eq( g_sp_live, 5 );
        test_eq( c.front().tag(), 'a' );
        test_eq( c.back().tag(), 'e' );

        STD::span<SpLive> mid = s.subspan(1, 3);
        test_eq( (int)mid.size(), 3 );
        test_eq( mid[0].tag(), 'b' );
        test_eq( g_sp_live, 5 );

        mid[0] = SpLive('Z');
        test_eq( v[1].tag(), 'Z' );
        test_eq( g_sp_live, 5 );

        int n = 0;
        for (STD::span<SpLive>::iterator it = s.begin(); it != s.end(); ++it) ++n;
        test_eq( n, 5 );
    }
    test_eq( g_sp_live, 0 );
}

#else   // TEST_TARGET_CXX < 2020

TEST_CASE(span, class_elements_non_owning) {
    TEST_NOTE("span is C++20; nothing to view here");
    TEST_SKIP1();
}

#endif  // TEST_TARGET_CXX >= 2020
