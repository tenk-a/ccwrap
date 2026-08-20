#include "test_cxx.hpp"

#include <tuple>
#include <array>
#include <string>
#include <type_traits>
#include <utility>
#include <memory>

namespace {
    int tup_add(int a, int b, int c) { return a + b + c; }
    struct TupPoint { int x, y; TupPoint(int a, int b) : x(a), y(b) {} };
}

TEST_CASE(tuple, construct_get) {
    STD::tuple<int, double, STD::string> t(1, 2.5, STD::string("hi"));
    test_eq( STD::get<0>(t), 1 );
    test_eq( STD::get<1>(t), 2.5 );
    test_eq( STD::get<2>(t), STD::string("hi") );
    test_pass("cxx11:tuple");

    STD::get<0>(t) = 10;
    test_eq( STD::get<0>(t), 10 );
    test_pass("cxx11:get");

    const STD::tuple<int, double, STD::string>& cr = t;
    test_eq( STD::get<0>(cr), 10 );
    test_eq( STD::get<2>(cr), STD::string("hi") );
    test_pass("cxx11:get (const)");

    STD::tuple<int, int> d;
    test_eq( STD::get<0>(d), 0 );
    test_eq( STD::get<1>(d), 0 );
    test_pass("cxx11:tuple (value-init)");

    STD::tuple<int, double, STD::string> mt = STD::make_tuple(5, 1.5, STD::string("x"));
    test_eq( STD::get<0>(mt), 5 );
    test_eq( STD::get<2>(mt), STD::string("x") );
    test_pass("cxx11:make_tuple");
}

TEST_CASE(tuple, size_element) {
    typedef STD::tuple<char, int, long, double> T;
    test_eq( (int)STD::tuple_size<T>::value, 4 );
    test_true( (STD::is_same<STD::tuple_element<0, T>::type, char>::value) );
    test_true( (STD::is_same<STD::tuple_element<1, T>::type, int>::value) );
    test_true( (STD::is_same<STD::tuple_element<2, T>::type, long>::value) );
    test_true( (STD::is_same<STD::tuple_element<3, T>::type, double>::value) );
    test_pass("cxx11:tuple_element");
    test_eq( (int)STD::tuple_size<STD::tuple<int> >::value, 1 );
    test_pass("cxx11:tuple_size");
}

TEST_CASE(tuple, compare) {
    STD::tuple<int, int> a = STD::make_tuple(1, 2);
    STD::tuple<int, int> b = STD::make_tuple(1, 2);
    STD::tuple<int, int> c = STD::make_tuple(1, 3);
    test_true( a == b );
    test_pass("cxx11:operator==");
    test_true( a != c );
    test_pass("cxx11:operator!=");
    test_true( a <  c );
    test_pass("cxx11:operator<");
    test_true( c >  a );
    test_pass("cxx11:operator>");
    test_true( a <= b );
    test_pass("cxx11:operator<=");
    test_true( a >= b );
    test_true( c >= a );
    test_pass("cxx11:operator>=");
    test_true( a <= c );
}

TEST_CASE(tuple, arities) {
    STD::tuple<int> t1 = STD::make_tuple(1);
    test_eq( STD::get<0>(t1), 1 );

    STD::tuple<int, int, int, int, int> t5 = STD::make_tuple(1, 2, 3, 4, 5);
    test_eq( STD::get<0>(t5), 1 );
    test_eq( STD::get<2>(t5), 3 );
    test_eq( STD::get<4>(t5), 5 );

    typedef STD::tuple<int, int, int, int, int, int, int> T7;
    T7 t7 = STD::make_tuple(1, 2, 3, 4, 5, 6, 7);
    test_eq( STD::get<6>(t7), 7 );
    test_eq( (int)STD::tuple_size<T7>::value, 7 );
    test_pass("cxx11:tuple (variadic arity)");
}

TEST_CASE(tuple, swap_get_by_type) {
    STD::tuple<int, STD::string> a = STD::make_tuple(1, STD::string("a"));
    STD::tuple<int, STD::string> b = STD::make_tuple(2, STD::string("b"));
    a.swap(b);
    test_eq( STD::get<0>(a), 2 );
    test_eq( STD::get<1>(b), STD::string("a") );
    test_pass("cxx11:swap");
    using STD::swap;
    swap(a, b);
    test_eq( STD::get<0>(a), 1 );
    test_pass("cxx11:swap (free)");

#if !_TST_HAS_CXX11_LIB_MEMBERS || defined(__WATCOMC__) || _TST_HAS_CXX14_LIB_MEMBERS
    STD::tuple<int, double, STD::string> g = STD::make_tuple(5, 2.5, STD::string("z"));
    test_eq( STD::get<int>(g), 5 );
    test_eq( STD::get<double>(g), 2.5 );
    test_eq( STD::get<STD::string>(g), STD::string("z") );
    STD::get<int>(g) = 9;
    test_eq( STD::get<0>(g), 9 );
    test_pass("cxx14:get<Type>");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
#endif
}

TEST_CASE(tuple, ignore) {

    STD::ignore = 42;
    STD::ignore = STD::string("discarded");
    const char* kept = "still here";
    STD::ignore = kept;
    test_true( STD::string(kept) == "still here" );
    test_pass("cxx11:ignore");
}

TEST_CASE(tuple, ctors_and_assign) {
    typedef STD::tuple<int, STD::string> T;

    T a(1, "x");
    T b(a);
    test_true( STD::get<0>(b) == 1 && STD::get<1>(b) == "x" );
    test_pass("cxx11:tuple::tuple(const tuple&) (copy)");

    T c(STD::move(a));
    TEST_SKIP_NATIVE_NO_MOVE("no move below C++11: the type here is the native library's own, and a move cannot be added to it");
    test_true( STD::get<1>(c) == "x" && STD::get<1>(a).empty() );
    test_pass("cxx11:tuple::tuple(tuple&&) (move)");

    STD::tuple<long, STD::string> conv(2, "y");
    test_true( STD::get<0>(conv) == 2 );
    test_pass("cxx11:tuple::tuple(UTypes&&...) (converting)");

    STD::pair<int, STD::string> pr(4, "p");
#if _TST_HAS_ALIAS_TEMPLATE
    STD::tuple<int, STD::string> src(3, "z");
    STD::tuple<long, STD::string> from(src);
    test_true( STD::get<0>(from) == 3 );
    test_pass("cxx11:tuple::tuple(const tuple<UTypes...>&)");
#else
    TEST_SKIP1();
    test_skip("cxx11:tuple::tuple(const tuple<UTypes...>&)");
#endif

#if _TST_HAS_ALIAS_TEMPLATE
    STD::tuple<int, STD::string> fp(pr);
    test_true( STD::get<0>(fp) == 4 && STD::get<1>(fp) == "p" );
    test_pass("cxx11:tuple::tuple(const pair<U1,U2>&)");
#else
    TEST_SKIP1();
    test_skip("cxx11:tuple::tuple(const pair<U1,U2>&)");
#endif

#if _TST_HAS_ALIAS_TEMPLATE
    STD::tuple<int> at(STD::allocator_arg, STD::allocator<int>(), 9);
    test_eq( STD::get<0>(at), 9 );
    test_pass("cxx11:tuple::tuple(allocator_arg_t, Alloc, ...)");
#else
    TEST_SKIP1();
    test_skip("cxx11:tuple::tuple(allocator_arg_t, Alloc, ...)");
#endif

    T e(0, "");
    e = b;
    test_true( STD::get<0>(e) == 1 );
    test_pass("cxx11:tuple::operator=(const tuple&)");
    T mv(5, "m");
    e = STD::move(mv);
    TEST_SKIP_NATIVE_NO_MOVE("no move below C++11: the type here is the native library's own, and a move cannot be added to it");
    test_true( STD::get<1>(e) == "m" && STD::get<1>(mv).empty() );
    test_pass("cxx11:tuple::operator=(tuple&&)");
#if _TST_HAS_ALIAS_TEMPLATE
    STD::tuple<int, STD::string> fromp;
    fromp = pr;
    test_true( STD::get<0>(fromp) == 4 );
    test_pass("cxx11:tuple::operator=(const pair<U1,U2>&)");
#else
    TEST_SKIP1();
    test_skip("cxx11:tuple::operator=(const pair<U1,U2>&)");
#endif

    test_true(( STD::uses_allocator<STD::tuple<int>, STD::allocator<int> >::value ));
    test_pass("cxx11:uses_allocator<tuple>");
}

TEST_CASE(tuple, helpers) {
    int x = 7; STD::string s;
#if _TST_HAS_ALIAS_TEMPLATE
    x = 0;
    STD::tie(x, s) = STD::make_tuple(7, STD::string("tied"));
    test_true( x == 7 && s == "tied" );
    test_pass("cxx11:tie");
#else
    TEST_SKIP1();
    test_skip("cxx11:tie");
#endif

#if _TST_HAS_ALIAS_TEMPLATE
    STD::tuple<int&, const char(&)[3]> ft = STD::forward_as_tuple(x, "ab");
    test_eq( STD::get<0>(ft), 7 );
    test_pass("cxx11:forward_as_tuple");
#else
    TEST_SKIP1();
    test_skip("cxx11:forward_as_tuple");
#endif

#if _TST_HAS_ALIAS_TEMPLATE
    STD::tuple<int, char, double> cat = STD::tuple_cat(STD::make_tuple(1),
                                                       STD::make_tuple('a', 2.0));
    test_true( STD::get<0>(cat) == 1 && STD::get<1>(cat) == 'a' && STD::get<2>(cat) == 2.0 );
    test_pass("cxx11:tuple_cat");
#else
    TEST_SKIP1();
    test_skip("cxx11:tuple_cat");
#endif

    test_eq( STD::get<0>(STD::make_tuple(42, 'q')), 42 );
    test_pass("cxx11:get<I>(tuple&&)");

#if TEST_TARGET_CXX >= 2014 && _TST_HAS_ALIAS_TEMPLATE && (_TST_HAS_CXX14_LIB_MEMBERS || !_TST_HAS_CXX11_LIB_MEMBERS || defined(__WATCOMC__))
    test_true(( STD::is_same<STD::tuple_element_t<1, STD::tuple<int, char> >, char>::value ));
    test_pass("cxx14:tuple_element_t");
#else
    test_skip("cxx14:tuple_element_t");
#endif

#if TEST_TARGET_CXX >= 2017 && _TST_HAS_VAR_TEMPLATE
    int tsv = (int)STD::tuple_size_v<STD::tuple<int, char, double> >;
    test_eq( tsv, 3 );
    test_pass("cxx17:tuple_size_v");
    test_eq( STD::apply(tup_add, STD::make_tuple(1, 2, 3)), 6 );
    test_pass("cxx17:apply");
    TupPoint p = STD::make_from_tuple<TupPoint>(STD::make_tuple(4, 5));
    test_true( p.x == 4 && p.y == 5 );
    test_pass("cxx17:make_from_tuple");
#else
    test_skip("cxx17:tuple_size_v"); test_skip("cxx17:apply"); test_skip("cxx17:make_from_tuple");
#endif

#if defined(__cpp_lib_three_way_comparison)
    STD::tuple<int, int> l(1, 2), h(1, 3);
    test_true( (l <=> h) < 0 && (l <=> l) == 0 );
    test_pass("cxx20:operator<=>(tuple)");
#else
    test_skip("cxx20:operator<=>(tuple)");
#endif

#if defined(__cpp_lib_tuple_like)
    STD::pair<int, int> pl(8, 9);
    STD::tuple<int, int> tl(pl);
    test_true( STD::get<0>(tl) == 8 && STD::get<1>(tl) == 9 );
    test_pass("cxx23:tuple::tuple(tuple-like)");
#else
    test_skip("cxx23:tuple::tuple(tuple-like)");
#endif
}

#if TEST_TARGET_CXX >= 2023 && defined(__cpp_lib_ranges) && _TST_HAS_ALIAS_TEMPLATE
TEST_CASE(tuple, common_type_cxx23) {
    typedef STD::tuple<int, double>  TA;
    typedef STD::tuple<long, float>  TB;
    test_true(( STD::is_same<STD::common_type<TA, TB>::type,
                             STD::tuple<long, double> >::value ));
    test_true(( STD::is_same<STD::common_type<TA, TA>::type, TA>::value ));
    test_pass("cxx23:common_type");

    typedef STD::tuple<int&, double&>              TR;
    typedef STD::tuple<const int&, const double&>  TCR;
    test_true(( STD::is_same<STD::common_reference<TR, TCR>::type, TCR>::value ));
    test_pass("cxx23:basic_common_reference");
}
#else
TEST_CASE(tuple, common_type_cxx23) {
    test_skip("cxx23:common_type");
    test_skip("cxx23:basic_common_reference");
}
#endif

TEST_CASE(tuple, feature_test_macros) {
#if defined(__cpp_lib_tuples_by_type) && (__cpp_lib_tuples_by_type) >= 201304L
    test_true( (__cpp_lib_tuples_by_type) >= 201304L );
    test_pass("cxx14:__cpp_lib_tuples_by_type=201304L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx14:__cpp_lib_tuples_by_type=201304L");
#endif
#if defined(__cpp_lib_tuple_element_t) && (__cpp_lib_tuple_element_t) >= 201402L
    test_true( (__cpp_lib_tuple_element_t) >= 201402L );
    test_pass("cxx14:__cpp_lib_tuple_element_t=201402L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx14:__cpp_lib_tuple_element_t=201402L");
#endif
#if defined(__cpp_lib_apply) && (__cpp_lib_apply) >= 201603L
    test_true( (__cpp_lib_apply) >= 201603L );
    test_pass("cxx17:__cpp_lib_apply=201603L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx17:__cpp_lib_apply=201603L");
#endif
#if defined(__cpp_lib_make_from_tuple) && (__cpp_lib_make_from_tuple) >= 201606L
    test_true( (__cpp_lib_make_from_tuple) >= 201606L );
    test_pass("cxx17:__cpp_lib_make_from_tuple=201606L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx17:__cpp_lib_make_from_tuple=201606L");
#endif
#if defined(__cpp_lib_tuple_like) && (__cpp_lib_tuple_like) >= 202207L
    test_true( (__cpp_lib_tuple_like) >= 202207L );
    test_pass("cxx23:__cpp_lib_tuple_like=202207L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_tuple_like=202207L");
#endif
}


TEST_CASE(tuple, tuple_like_cxx23) {
#if defined(__cpp_lib_tuple_like)
    int np = (int)STD::tuple_size<STD::pair<int, char> >::value;
    int na = (int)STD::tuple_size<STD::array<int, 3> >::value;
    test_eq( np, 2 );
    test_eq( na, 3 );
    test_true(( STD::is_same<STD::tuple_element<0, STD::pair<int, char> >::type,
                             int>::value ));

    STD::pair<int, char> p(1, 'a');
    STD::tuple<int, char> t(p);
    test_true( STD::get<0>(t) == 1 && STD::get<1>(t) == 'a' );

    STD::array<int, 2> arr = {{4, 5}};
    test_eq( STD::get<1>(arr), 5 );
    test_pass("cxx23:tuple-like");
#else
    TEST_NOTE("the tuple-like protocol is C++23 (P2165); not in this library");
    test_skip("cxx23:tuple-like");
#endif
}

static int g_tp_live = 0;

struct TpLive {
    char* p_;
    TpLive() : p_(new char[32]) { p_[0] = 0; ++g_tp_live; }
    TpLive(char c) : p_(new char[32]) { p_[0] = c; p_[1] = 0; ++g_tp_live; }
    TpLive(const TpLive& o) : p_(new char[32]) {
        for (int i = 0; i < 32; ++i) p_[i] = o.p_[i];
        ++g_tp_live;
    }
    TpLive& operator=(const TpLive& o) {
        if (this != &o) for (int i = 0; i < 32; ++i) p_[i] = o.p_[i];
        return *this;
    }
    ~TpLive() { delete[] p_; --g_tp_live; }
    char tag() const { return p_[0]; }
    bool operator==(const TpLive& o) const { return p_[0] == o.p_[0]; }
    bool operator< (const TpLive& o) const { return p_[0] <  o.p_[0]; }
};

TEST_CASE(tuple, class_elements_lifetime) {
    g_tp_live = 0;
    {
        TpLive a('a');
        STD::tuple<TpLive, STD::string> t(a, STD::string("v_long_enough_to_leave_sso"));
        test_eq( g_tp_live, 2 );
        test_eq( STD::get<0>(t).tag(), 'a' );
        test_eq( STD::get<1>(t), STD::string("v_long_enough_to_leave_sso") );

        STD::tuple<TpLive, STD::string> c(t);
        test_eq( g_tp_live, 3 );
        test_eq( STD::get<0>(c).tag(), 'a' );

        TpLive b('b');
        STD::tuple<TpLive, STD::string> u(b, STD::string("w_long_enough_to_leave_sso"));
        test_eq( g_tp_live, 5 );
        c = u;
        test_eq( g_tp_live, 5 );
        test_eq( STD::get<0>(c).tag(), 'b' );
        test_eq( STD::get<1>(c), STD::string("w_long_enough_to_leave_sso") );

        STD::tuple<TpLive, TpLive> m = STD::make_tuple(a, b);
        test_eq( g_tp_live, 7 );
        test_eq( STD::get<0>(m).tag(), 'a' );
        test_eq( STD::get<1>(m).tag(), 'b' );
    }
    test_eq( g_tp_live, 0 );
}
