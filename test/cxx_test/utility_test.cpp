#include "test_cxx.hpp"

#include <utility>
#include <string>
#include <vector>
#include <type_traits>
#include <optional>
#include <variant>
#include <functional>

static STD::string make_key(const char* s) { return STD::string(s); }

struct RelT { int v; };
inline bool operator==(const RelT& a, const RelT& b) { return a.v == b.v; }
inline bool operator<(const RelT& a, const RelT& b) { return a.v < b.v; }

TEST_CASE(utility, rel_ops) {
    using namespace STD::rel_ops;
    RelT a; a.v = 1;
    RelT b; b.v = 2;
    test_true(  (a != b) );
    test_true( !(a != a) );
    test_pass("cxx03:rel_ops::operator!=");
    test_true(  (b > a) );
    test_true( !(a > b) );
    test_pass("cxx03:rel_ops::operator>");
    test_true(  (a <= b) );
    test_true(  (a <= a) );
    test_pass("cxx03:rel_ops::operator<=");
    test_true(  (b >= a) );
    test_pass("cxx03:rel_ops::operator>=");
}

TEST_CASE(utility, pair) {
    STD::pair<int, STD::string> p(1, "one");
    test_eq( p.first, 1 );
    test_eq( p.second, STD::string("one") );
    test_pass("cxx03:pair");

    STD::pair<int, int> q = STD::make_pair(2, 3);
    test_eq( q.first, 2 );
    test_eq( q.second, 3 );
    {
        STD::pair<STD::string, int> sp = STD::make_pair(make_key("ab"), 7);
        test_eq( sp.first, STD::string("ab") );
        test_eq( sp.second, 7 );
        STD::pair<STD::string, int> lp = STD::make_pair(make_key("a_long_key_past_the_sso_limit"), 9);
        test_eq( lp.first, STD::string("a_long_key_past_the_sso_limit") );
        STD::pair<const char*, int> cq = STD::make_pair("abc", 4);
        test_eq( STD::string(cq.first), STD::string("abc") );
        STD::vector<STD::pair<STD::string, int> > vp;
        for (int i = 0; i < 40; ++i)
            vp.push_back(STD::make_pair(make_key("token_that_needs_a_heap_buffer"), i));
        test_eq( vp.size(), 40u );
        test_eq( vp[39].first, STD::string("token_that_needs_a_heap_buffer") );
        test_eq( vp[39].second, 39 );
    }
    test_pass("cxx03:make_pair");

    STD::pair<int, int> cp(q);
    test_eq( cp.first, 2 );
    test_pass("cxx03:pair::pair(copy)");

    STD::pair<int, int> a(1, 2), b(3, 4);
#if defined(_MSC_VER) || _tst_cplusplus >= 201103L
    a.swap(b);
    test_eq( a.first, 3 );
    a.swap(b);
    test_pass("cxx11:pair::swap");
#else
    TEST_SKIP1();
    test_skip("cxx11:pair::swap");
#endif
    STD::swap(a, b);
    test_eq( a.first, 3 );
    test_pass("cxx03:swap(pair)");

    test_true( STD::make_pair(1, 2) == STD::make_pair(1, 2) );
    test_pass("cxx03:pair::operator==");
    test_true( STD::make_pair(1, 2) != STD::make_pair(1, 3) );
    test_pass("cxx03:pair::operator!=");
    test_true( STD::make_pair(1, 2) <  STD::make_pair(1, 3) );
    test_pass("cxx03:pair::operator<");
    test_true( STD::make_pair(1, 3) >  STD::make_pair(1, 2) );
    test_pass("cxx03:pair::operator>");
    test_true( STD::make_pair(1, 2) <= STD::make_pair(1, 2) );
    test_pass("cxx03:pair::operator<=");
    test_true( STD::make_pair(1, 2) >= STD::make_pair(1, 2) );
    test_pass("cxx03:pair::operator>=");
}

#if TEST_TARGET_CXX >= 2014
TEST_CASE(utility, exchange) {
    int v = 5;
    int old = STD::exchange(v, 9);
    test_eq( old, 5 );
    test_eq( v, 9 );
    STD::string s = "a";
    STD::string so = STD::exchange(s, STD::string("b"));
    test_eq( so, STD::string("a") );
    test_eq( s, STD::string("b") );
    test_pass("cxx14:exchange");
}
#endif

#if TEST_TARGET_CXX >= 2017
TEST_CASE(utility, as_const) {
    int x = 42;
    const int& r = STD::as_const(x);
    test_eq( r, 42 );
    test_pass("cxx17:as_const");
}
#endif

#if TEST_TARGET_CXX >= 2011
TEST_CASE(utility, piecewise_tag) {
    STD::piecewise_construct_t t = STD::piecewise_construct;
    (void)t;
    test_eq( (int)sizeof(STD::piecewise_construct_t), (int)sizeof(STD::piecewise_construct_t) );
    test_true( true );
    test_pass("cxx11:piecewise_construct");
}
#endif

#if TEST_TARGET_CXX >= 2017
TEST_CASE(utility, in_place_tags) {
    STD::in_place_t it = STD::in_place;
    (void)it;
    STD::in_place_type_t<int> tt;
    (void)tt;
    STD::in_place_index_t<2> ii;
    (void)ii;

    test_true( sizeof(STD::in_place_type_t<int>) >= 1 );
    test_pass("cxx17:in_place_type_t");
    test_true( sizeof(STD::in_place_index_t<0>) >= 1 );
    test_pass("cxx17:in_place_index_t");
    test_true( true );
    test_pass("cxx17:in_place");
}
#endif

#if TEST_TARGET_CXX >= 2023
TEST_CASE(utility, unreachable_exists) {

    void (*p)() = &STD::unreachable;
    test_true( p != 0 );
    test_pass("cxx23:unreachable");
}
#endif

#if TEST_TARGET_CXX >= 2020
TEST_CASE(utility, integer_compare) {
    test_true(  STD::cmp_equal(1, 1) );
    test_true(  STD::cmp_equal(1, 1u) );
    test_true( !STD::cmp_equal(-1, 0xFFFFFFFFu) );
    test_pass("cxx20:cmp_equal");
    test_true(  STD::cmp_not_equal(1, 2) );
    test_pass("cxx20:cmp_not_equal");
    test_true(  STD::cmp_less(-1, 1u) );
    test_true( !STD::cmp_less(1u, -1) );
    test_pass("cxx20:cmp_less");
    test_true(  STD::cmp_less_equal(1, 1u) );
    test_pass("cxx20:cmp_less_equal");
    test_true(  STD::cmp_greater(2, 1) );
    test_true(  STD::cmp_greater(1u, -1) );
    test_pass("cxx20:cmp_greater");
    test_true(  STD::cmp_greater_equal(2, 2) );
    test_pass("cxx20:cmp_greater_equal");

    test_true(  STD::in_range<int>(5) );
    test_true( !STD::in_range<unsigned char>(-1) );
    test_true( !STD::in_range<unsigned char>(256) );
    test_true(  STD::in_range<unsigned char>(255) );
    test_pass("cxx20:in_range");
}
#endif

#if TEST_TARGET_CXX >= 2023
enum ColorFs { redFs = 5, greenFs = 6 };
#if defined(__WATCOMC__)
TEST_CASE_SKIP(utility, to_underlying)
#else
TEST_CASE(utility, to_underlying) {
    test_eq( (int)STD::to_underlying(redFs), 5 );
    test_eq( (int)STD::to_underlying(greenFs), 6 );
#if _tst_cplusplus >= 201103L
    enum Color { red = 5, green = 6 };
    test_eq( (int)STD::to_underlying(red), 5 );
    test_eq( (int)STD::to_underlying(green), 6 );
    test_pass("cxx23:to_underlying");
#else
    TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx23:to_underlying");
#endif
}
#endif  // __WATCOMC__ (no __underlying_type intrinsic)
#endif

#include <tuple>
TEST_CASE(utility, pair_tuple_protocol) {
    STD::pair<int, double> p(7, 2.5);

    test_eq( (int)(STD::tuple_size<STD::pair<int, double> >::value), 2 );
    test_pass("cxx11:tuple_size<pair>");
    test_eq( sizeof(STD::tuple_element<0, STD::pair<int, double> >::type), sizeof(int) );
    test_eq( sizeof(STD::tuple_element<1, STD::pair<int, double> >::type), sizeof(double) );
    test_pass("cxx11:tuple_element<pair>");

    test_eq( STD::get<0>(p), 7 );
    test_true( STD::get<1>(p) == 2.5 );
    STD::get<0>(p) = 42;
    test_eq( p.first, 42 );
    const STD::pair<int, double>& cp = p;
    test_eq( STD::get<0>(cp), 42 );
    test_true( STD::get<1>(cp) == 2.5 );
    test_pass("cxx11:get<I>(pair)");

#if TEST_TARGET_CXX >= 2014 && (!defined(_MSC_VER) || defined(__cpp_lib_tuples_by_type))
    test_eq( STD::get<int>(p), 42 );
    test_true( STD::get<double>(p) == 2.5 );
    STD::get<int>(p) = 5;
    test_eq( p.first, 5 );
    test_true( STD::get<double>(cp) == 2.5 );
    test_pass("cxx14:get<T>(pair)");
#else
    TEST_SKIP_N(4);
    test_skip("cxx14:get<T>(pair)");
#endif

    test_eq( (int)(STD::tuple_size<STD::tuple<int, char> >::value), 2 );
}

TEST_CASE(utility, move_forward_helpers) {

    STD::string s("payload");
    STD::string d = STD::move(s);
    test_true( d == "payload" );
    test_pass("cxx11:move");

#if _TST_HAS_RVALUE_REF
    int b = 3;
    int& lr = STD::forward<int&>(b);
    test_true( &lr == &b );
    int v = STD::forward<int>(5);
    test_eq( v, 5 );
    test_pass("cxx11:forward");
#else
    TEST_SKIP_N(2);
    test_skip("cxx11:forward");
#endif

#if _TST_HAS_RVALUE_REF
    test_true(( STD::is_same<decltype(STD::declval<int>()), int&&>::value ));
    test_pass("cxx11:declval");
#else
    TEST_SKIP1();
    test_skip("cxx11:declval");
#endif

    STD::string mn("mn");
    STD::string mnr = STD::move_if_noexcept(mn);
    test_true( mnr == "mn" );
    test_pass("cxx11:move_if_noexcept");

    int x = 1, y = 2;
    STD::swap(x, y);
    test_true( x == 2 && y == 1 );
    test_pass("cxx11:swap (generic)");
}

TEST_CASE(utility, pair_ctors_assign) {
    STD::pair<int, int> pd;
    test_true( pd.first == 0 && pd.second == 0 );
    test_pass("cxx03:pair::pair() (default)");

    STD::pair<int, int> pc(1, 2), pc2;
    pc2 = pc;
    test_true( pc2.first == 1 && pc2.second == 2 );
    test_pass("cxx03:pair::operator=(const pair&)");

    STD::pair<long, double> cv(1, 2.5);
    test_true( cv.first == 1 && cv.second == 2.5 );
    test_pass("cxx11:pair::pair(U1&&, U2&&) (converting)");

    STD::pair<int, int> src(3, 4);
    STD::pair<long, double> cvp(src);
    test_true( cvp.first == 3 && cvp.second == 4 );
    test_pass("cxx11:pair::pair(const pair<U1,U2>&) (converting)");

    STD::pair<STD::string, STD::string> pm("a", "b");
    STD::pair<STD::string, STD::string> pm2(STD::move(pm));
    TEST_SKIP_VC090("no move on vc8/9: the type here is MSVC's own, and a move cannot be added to it");
    test_true( pm2.first == "a" && pm.first.empty() );
    test_pass("cxx11:pair::pair(pair&&) (move)");

#if _TST_HAS_ALIAS_TEMPLATE || defined(_CCW_HAS_PAIR_PIECEWISE)
    STD::pair<STD::string, int> pw(STD::piecewise_construct,
                                   STD::make_tuple(STD::string("hi")), STD::make_tuple(7));
    test_true( pw.first == "hi" && pw.second == 7 );
    test_pass("cxx11:pair::pair(piecewise_construct_t, tuple, tuple)");
#else
    TEST_SKIP1();
    test_skip("cxx11:pair::pair(piecewise_construct_t, tuple, tuple)");
#endif

    STD::pair<STD::string, STD::string> am("x", "y"), am2;
    am2 = STD::move(am);
    TEST_SKIP_VC090("no move on vc8/9: the type here is MSVC's own, and a move cannot be added to it");
    test_true( am2.first == "x" && am.first.empty() );
    test_pass("cxx11:pair::operator=(pair&&)");

    STD::pair<int, int> ai(5, 6);
    STD::pair<long, double> ao;
    ao = ai;
    test_true( ao.first == 5 && ao.second == 6 );
    test_pass("cxx11:pair::operator=(const pair<U1,U2>&)");

#if defined(__cpp_lib_three_way_comparison)
    STD::pair<int, int> l(1, 2), h(1, 3);
    test_true( (l <=> h) < 0 );
    test_true( (l <=> l) == 0 );
    test_pass("cxx20:operator<=>(pair)");
#else
    test_skip("cxx20:operator<=>(pair)");
#endif
}

#if TEST_TARGET_CXX >= 2014 && _TST_HAS_ALIAS_TEMPLATE
TEST_CASE(utility, integer_sequences) {
    typedef STD::integer_sequence<int, 1, 2, 3> ISeq;
    test_eq( (int)ISeq::size(), 3 );
    test_true(( STD::is_same<ISeq::value_type, int>::value ));
    test_pass("cxx14:integer_sequence");

    typedef STD::index_sequence<0, 1, 2> IdxSeq;
    test_eq( (int)IdxSeq::size(), 3 );
    test_pass("cxx14:index_sequence");

    test_true(( STD::is_same<STD::make_integer_sequence<int, 3>,
                             STD::integer_sequence<int, 0, 1, 2> >::value ));
    test_pass("cxx14:make_integer_sequence");
    test_true(( STD::is_same<STD::make_index_sequence<3>, STD::index_sequence<0, 1, 2> >::value ));
    test_pass("cxx14:make_index_sequence");
    test_true(( STD::is_same<STD::index_sequence_for<char, int, double>,
                             STD::index_sequence<0, 1, 2> >::value ));
    test_pass("cxx14:index_sequence_for");
}
#else
TEST_CASE(utility, integer_sequences) {
    test_skip("cxx14:integer_sequence"); test_skip("cxx14:index_sequence");
    test_skip("cxx14:make_integer_sequence"); test_skip("cxx14:make_index_sequence");
    test_skip("cxx14:index_sequence_for");
}
#endif

TEST_CASE(utility, cxx23_forward_like) {
#if defined(__cpp_lib_forward_like) && _TST_HAS_DECLTYPE
    int x = 5;
    test_true(( STD::is_same<decltype(STD::forward_like<const int&>(x)), const int&>::value ));
    test_true(( STD::is_same<decltype(STD::forward_like<int&&>(x)), int&&>::value ));
    test_pass("cxx23:forward_like");
#else
    test_skip("cxx23:forward_like");
#endif
}


TEST_CASE(utility, pair_member_types_cxx11) {
    typedef STD::pair<int, STD::string> P;
    P p(1, STD::string("a"));

    test_true(( STD::is_same<P::first_type, int>::value ));
    p.first = 2;
    test_eq( p.first, 2 );
    test_pass("cxx03:pair::first_type");

    test_true(( STD::is_same<P::second_type, STD::string>::value ));
    p.second += "b";
    test_true( p.second == "ab" );
    test_pass("cxx03:pair::second_type");
}

#if TEST_TARGET_CXX >= 2014 && _TST_HAS_ALIAS_TEMPLATE
TEST_CASE(utility, integer_sequence_members_cxx14) {
    typedef STD::integer_sequence<int, 3, 1, 4> Seq;

    test_true(( STD::is_same<Seq::value_type, int>::value ));
    test_true(( STD::is_same<STD::index_sequence<0, 1>::value_type,
                             STD::size_t>::value ));
    test_pass("cxx14:integer_sequence::value_type");

    test_eq( (int)Seq::size(), 3 );
    test_eq( (int)STD::make_index_sequence<5>::size(), 5 );
    test_eq( (int)STD::integer_sequence<int>::size(), 0 );
    test_pass("cxx14:integer_sequence::size");
}
#else
TEST_CASE(utility, integer_sequence_members_cxx14) {
    test_skip("cxx14:integer_sequence::value_type");
    test_skip("cxx14:integer_sequence::size");
}
#endif

#if TEST_TARGET_CXX >= 2017
TEST_CASE(utility, in_place_t_cxx17) {
    STD::optional<STD::string> o(STD::in_place, 3u, 'x');
    test_true( *o == "xxx" );
    const STD::in_place_t* in_place_p = &STD::in_place;
    test_true( in_place_p != 0 );

    STD::in_place_t tag = STD::in_place;
    STD::optional<STD::string> o2(tag, 2u, 'y');
    test_true( *o2 == "yy" );
    test_pass("cxx17:in_place_t");
}
#else
TEST_CASE(utility, in_place_t_cxx17) {
    test_skip("cxx17:in_place_t");
}
#endif

#if TEST_TARGET_CXX >= 2023 && defined(__cpp_lib_ranges) && _TST_HAS_ALIAS_TEMPLATE
TEST_CASE(utility, pair_common_type_cxx23) {
    typedef STD::pair<int, double> PA;
    typedef STD::pair<long, float> PB;
    test_true(( STD::is_same<STD::common_type<PA, PB>::type,
                             STD::pair<long, double> >::value ));
    test_true(( STD::is_same<STD::common_type<PA, PA>::type, PA>::value ));
    test_pass("cxx23:common_type");

    typedef STD::pair<int&, double&>             PR;
    typedef STD::pair<const int&, const double&> PCR;
    test_true(( STD::is_same<STD::common_reference<PR, PCR>::type, PCR>::value ));
    test_pass("cxx23:basic_common_reference");
}
#else
TEST_CASE(utility, pair_common_type_cxx23) {
    test_skip("cxx23:common_type");
    test_skip("cxx23:basic_common_reference");
}
#endif

TEST_CASE(utility, cxx26_additions) {
#if defined(__cpp_lib_constant_wrapper)
    test_eq( (int)STD::cw<5>.value, 5 );
    test_true( tst_type_is<STD::constant_wrapper<5> >(STD::cw<5>) );
    test_pass("cxx26:constant_wrapper");
    test_pass("cxx26:cw");
#else
    TEST_NOTE("constant_wrapper/cw are C++26 (P2781); not in this library");
    test_skip("cxx26:constant_wrapper");
    test_skip("cxx26:cw");
#endif

#if defined(__cpp_lib_monostate_utility) || (TEST_TARGET_CXX >= 2026 && defined(__cpp_lib_variant))
    {
        STD::monostate m1, m2;
        STD::hash<STD::monostate> h;
        test_true( m1 == m2 );
        test_true( h(m1) == h(m2) );
    }
    test_pass("cxx26:monostate");
    test_pass("cxx26:hash<monostate>");
#else
    TEST_NOTE("monostate reaches <utility> only in C++26");
    test_skip("cxx26:monostate");
    test_skip("cxx26:hash<monostate>");
#endif
}

TEST_CASE(utility, feature_test_macros) {
#if defined(__cpp_lib_forward_like) && (__cpp_lib_forward_like) >= 202207L
    test_true( (__cpp_lib_forward_like) >= 202207L );
    test_pass("cxx23:__cpp_lib_forward_like=202207L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_forward_like=202207L");
#endif
#if defined(__cpp_lib_to_underlying) && (__cpp_lib_to_underlying) >= 202102L
    test_true( (__cpp_lib_to_underlying) >= 202102L );
    test_pass("cxx23:__cpp_lib_to_underlying=202102L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_to_underlying=202102L");
#endif
#if defined(__cpp_lib_unreachable) && (__cpp_lib_unreachable) >= 202202L
    test_true( (__cpp_lib_unreachable) >= 202202L );
    test_pass("cxx23:__cpp_lib_unreachable=202202L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_unreachable=202202L");
#endif
}


TEST_CASE(utility, observable_checkpoint_cxx26) {
#if defined(__cpp_lib_observable_checkpoint)
    volatile int step = 0;
    step = 1;
    STD::observable_checkpoint();
    step = 2;
    test_eq( (int)step, 2 );
    test_pass("cxx26:observable_checkpoint");
#else
    TEST_NOTE("observable_checkpoint is C++26 (P1494); not in this library");
    test_skip("cxx26:observable_checkpoint");
#endif
}

static int g_pr_live = 0;

struct PrLive {
    char* p_;
    PrLive() : p_(new char[32]) { p_[0] = 0; ++g_pr_live; }
    PrLive(char c) : p_(new char[32]) { p_[0] = c; p_[1] = 0; ++g_pr_live; }
    PrLive(const PrLive& o) : p_(new char[32]) {
        for (int i = 0; i < 32; ++i) p_[i] = o.p_[i];
        ++g_pr_live;
    }
    PrLive& operator=(const PrLive& o) {
        if (this != &o) for (int i = 0; i < 32; ++i) p_[i] = o.p_[i];
        return *this;
    }
    ~PrLive() { delete[] p_; --g_pr_live; }
    char tag() const { return p_[0]; }
    bool operator==(const PrLive& o) const { return p_[0] == o.p_[0]; }
    bool operator< (const PrLive& o) const { return p_[0] <  o.p_[0]; }
};

TEST_CASE(utility, class_elements_lifetime) {
    g_pr_live = 0;
    {
        PrLive a('a');
        STD::pair<PrLive, STD::string> p(a, STD::string("v_long_enough_to_leave_sso"));
        test_eq( g_pr_live, 2 );
        test_eq( p.first.tag(), 'a' );

        STD::pair<PrLive, STD::string> c(p);
        test_eq( g_pr_live, 3 );

        PrLive b('b');
        STD::pair<PrLive, STD::string> q(b, STD::string("w_long_enough_to_leave_sso"));
        test_eq( g_pr_live, 5 );

        STD::swap(p, q);
        test_eq( g_pr_live, 5 );
        test_eq( p.first.tag(), 'b' );
        test_eq( q.first.tag(), 'a' );
        test_eq( p.second, STD::string("w_long_enough_to_leave_sso") );

        c = q;
        test_eq( g_pr_live, 5 );
        test_eq( c.first.tag(), 'a' );

        STD::pair<PrLive, PrLive> m = STD::make_pair(a, b);
        test_eq( g_pr_live, 7 );
        test_eq( m.first.tag(), 'a' );
        test_eq( m.second.tag(), 'b' );

        STD::vector<STD::pair<PrLive, PrLive> > v;
        v.push_back(m);
        v.push_back(m);
        test_eq( g_pr_live, 11 );
        v.clear();
        test_eq( g_pr_live, 7 );
    }
    test_eq( g_pr_live, 0 );
}
