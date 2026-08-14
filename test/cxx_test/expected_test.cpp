#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2023 && (!defined(_CCW_NO_EXPECTED_CXX23) || _tst_cplusplus >= 202302L)
#include <expected>
#include <string>

typedef STD::expected<int, STD::string> Exp;

TEST_CASE(expected, construct_observe) {
    Exp d;
    test_true( d.has_value() );
    test_eq( *d, 0 );

    Exp v = 5;
    test_true( v.has_value() );
    test_true( (bool)v );
    test_true( !!v );
    test_eq( *v, 5 );
    test_eq( v.value(), 5 );
    test_eq( *v.operator->(), 5 );
    test_eq( v.value_or(9), 5 );

    Exp u = STD::unexpected<STD::string>("boom");
    test_true( !u.has_value() );
    test_true( !u );
    test_eq( u.error(), STD::string("boom") );
    test_eq( u.value_or(9), 9 );
    test_throw( (void)u.value() );

    Exp ip(STD::in_place, 7);
    test_eq( *ip, 7 );
    Exp ue(STD::unexpect, "e");
    test_eq( ue.error(), STD::string("e") );

    Exp cp(v);
    test_eq( *cp, 5 );
    test_pass("cxx23:construct observe");
}

TEST_CASE(expected, assign_emplace) {
    Exp e = 1;
    e = 2;
    test_eq( *e, 2 );
    e = STD::unexpected<STD::string>("x");
    test_true( !e.has_value() );
    test_eq( e.error(), STD::string("x") );
    Exp other = 9;
    e = other;
    test_eq( *e, 9 );

    int& r = e.emplace(11);
    test_eq( r, 11 );
    test_eq( *e, 11 );
    test_pass("cxx23:assign emplace");
}

TEST_CASE(expected, unexpected_and_compare) {
    STD::unexpected<STD::string> un("err");
    test_eq( un.error(), STD::string("err") );
    un.error() = "err2";
    test_eq( un.error(), STD::string("err2") );
    STD::unexpected<STD::string> un2("err2");
    test_true( un == un2 );
    un.swap(un2);
    test_eq( un.error(), STD::string("err2") );

    Exp a = 1, b = 1, c = 2;
    test_true( a == b );
    test_true( a != c );
    test_true( a == 1 );
    Exp err = STD::unexpected<STD::string>("z");
    test_true( err == STD::unexpected<STD::string>("z") );
    test_true( !(a == STD::unexpected<STD::string>("z")) );
    test_pass("cxx23:unexpected and compare");
}

namespace {
struct ExpCounter {
    static int live;
    int v;
    ExpCounter() : v(0) { ++live; }
    explicit ExpCounter(int x) : v(x) { ++live; }
    ExpCounter(const ExpCounter& o) : v(o.v) { ++live; }
    ExpCounter& operator=(const ExpCounter& o) { v = o.v; return *this; }
    ~ExpCounter() { --live; }
};
int ExpCounter::live = 0;
}

TEST_CASE(expected, ctors_assign_value_dtor) {
    {
        const Exp a(5);
        test_eq( a.value(), 5 );
        test_pass("cxx23:expected::value (const)");
    }
    {
        Exp a(7);
        Exp b(STD::move(a));
        test_true( b.has_value() );
        test_eq( *b, 7 );
        test_pass("cxx23:expected::expected(expected&&) (move)");
    }
    {
        STD::expected<int, const char*> src(9);
        STD::expected<long, STD::string> dst(src);
        test_true( dst.has_value() );
        test_eq( *dst, 9L );
        test_pass("cxx23:expected::expected(const expected<U,G>&) (converting)");
    }
    {
        Exp a(11);
        Exp b(0);
        b = STD::move(a);
        test_eq( *b, 11 );
        test_pass("cxx23:expected::operator=(expected&&)");
    }
    {
        int before = ExpCounter::live;
        {
            STD::expected<ExpCounter, STD::string> e(ExpCounter(3));
            test_eq( ExpCounter::live, before + 1 );
        }
        test_eq( ExpCounter::live, before );
        test_pass("cxx23:expected::~expected");
    }
}

#include <vector>
TEST_CASE(expected, init_list_ctors_and_swap) {
    typedef STD::expected<STD::vector<int>, STD::vector<int> > EV;

#if _tst_cplusplus >= 201103L
    {
        EV e(STD::in_place, { 1, 2, 3 });
        test_true( e.has_value() );
        test_eq( e->size(), 3u );
        test_pass("cxx23:expected::expected(in_place_t, initializer_list, Args...)");
    }
    {
        EV e(STD::unexpect, { 4, 5 });
        test_true( !e.has_value() );
        test_eq( e.error().size(), 2u );
        test_pass("cxx23:expected::expected(unexpect_t, initializer_list, Args...)");
    }
#else
    test_skip("cxx23:expected::expected(in_place_t, initializer_list, Args...)");
    test_skip("cxx23:expected::expected(unexpect_t, initializer_list, Args...)");
#endif
    {
        Exp a(1);
        Exp b = STD::unexpected<STD::string>("err");
        a.swap(b);
        test_true( !a.has_value() && b.has_value() );
        test_eq( *b, 1 );
        test_pass("cxx23:expected::swap");
        STD::swap(a, b);
        test_true( a.has_value() && !b.has_value() );
        test_pass("cxx23:swap(expected) (non-member)");
    }
}

TEST_CASE(expected, monadic_and_error_or) {
    struct Half  { STD::expected<int, STD::string> operator()(int x) const { return STD::expected<int, STD::string>(x / 2); } };
    struct Dbl   { int operator()(int x) const { return x * 2; } };
    struct Tag   { STD::expected<int, STD::string> operator()(const STD::string&) const { return STD::expected<int, STD::string>(-1); } };
    struct Wrap  { STD::string operator()(const STD::string& e) const { return "E:" + e; } };

    Exp ok(10);
    Exp err = STD::unexpected<STD::string>("bad");

#if _TST_HAS_DECLTYPE && (defined(__WATCOMC__) || defined(__cpp_lib_expected))
    test_eq( *ok.and_then(Half()), 5 );
    test_true( !err.and_then(Half()).has_value() );
    test_pass("cxx23:expected::and_then");

    test_eq( *ok.transform(Dbl()), 20 );
    test_pass("cxx23:expected::transform");

    test_eq( *err.or_else(Tag()), -1 );
    test_true( ok.or_else(Tag()).has_value() );
    test_pass("cxx23:expected::or_else");

    STD::expected<int, STD::string> te = err.transform_error(Wrap());
    test_eq( te.error(), STD::string("E:bad") );
    test_pass("cxx23:expected::transform_error");
#else
    test_skip("cxx23:expected::and_then");
    test_skip("cxx23:expected::transform");
    test_skip("cxx23:expected::or_else");
    test_skip("cxx23:expected::transform_error");
#endif

    test_eq( ok.error_or(STD::string("none")), STD::string("none") );
    test_eq( err.error_or(STD::string("none")), STD::string("bad") );
    test_pass("cxx23:expected::error_or");
}

TEST_CASE(expected, void_spec_and_ctad) {
    {
        STD::expected<void, STD::string> ok;
        test_true( ok.has_value() );
        ok.value();
        STD::expected<void, STD::string> err = STD::unexpected<STD::string>("v");
        test_true( !err.has_value() );
        test_eq( err.error(), STD::string("v") );
        test_pass("cxx23:expected<void, E> specialization");
    }

#if _TST_HAS_CTAD
    {
        STD::unexpected u(STD::string("ctad"));
        test_eq( u.error(), STD::string("ctad") );
        test_pass("cxx23:unexpected (CTAD)");
    }
#else
    test_skip("cxx23:unexpected (CTAD)");
#endif
}

TEST_CASE(expected, unexpected_ctor_forms_cxx23) {
    {   STD::string src("boom");
        STD::unexpected<STD::string> a(src);
        test_eq( a.error(), STD::string("boom") );
        STD::unexpected<STD::string> b(STD::move(src));
        test_eq( b.error(), STD::string("boom") );
        STD::unexpected<int> c(7);
        test_eq( c.error(), 7 ); }
    test_pass("cxx23:unexpected::unexpected(E&&)");

    {   STD::unexpected<STD::string> a(STD::in_place, 3, 'x');
        test_eq( a.error(), STD::string("xxx") );
        STD::unexpected<STD::string> b(STD::in_place);
        test_true( b.error().empty() );
        STD::unexpected<STD::string> c(STD::in_place, "abc");
        test_eq( c.error(), STD::string("abc") ); }
    test_pass("cxx23:unexpected::unexpected(in_place_t, Args&&...)");
}

#endif


#if TEST_TARGET_CXX >= 2023 && defined(__cpp_lib_expected)
TEST_CASE(expected, observers_and_types_cxx23) {
    typedef STD::expected<int, STD::string> E;
    E ok(7);
    E bad(STD::unexpected<STD::string>(STD::string("boom")));

    test_true( ok.has_value() );
    test_true( !bad.has_value() );
    test_true( (bool)ok == ok.has_value() );
    test_pass("cxx23:expected::has_value");

    test_eq( *ok, 7 );
    *ok = 9;
    test_eq( *ok, 9 );
    test_pass("cxx23:expected::operator*");

    STD::expected<STD::string, int> s(STD::string("abc"));
    test_eq( s->size(), (STD::size_t)3 );
    s->append("d");
    test_true( *s == "abcd" );
    test_pass("cxx23:expected::operator->");

    test_true( bad.error() == "boom" );
    bad.error() += "!";
    test_true( bad.error() == "boom!" );
    test_pass("cxx23:expected::error");

    test_eq( bad.value_or(3), 3 );
    test_eq( ok.value_or(3), 9 );
    test_pass("cxx23:expected::value_or");

    test_true(( STD::is_same<E::value_type, int>::value ));
    test_pass("cxx23:expected::value_type");
    test_true(( STD::is_same<E::error_type, STD::string>::value ));
    test_pass("cxx23:expected::error_type");
    test_true(( STD::is_same<E::unexpected_type, STD::unexpected<STD::string> >::value ));
    test_pass("cxx23:expected::unexpected_type");
    test_true(( STD::is_same<E::rebind<char>, STD::expected<char, STD::string> >::value ));
    test_pass("cxx23:expected::rebind");

#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202506L
    test_true( bad.has_error() );
    test_true( !ok.has_error() );
#else
    TEST_NOTE("expected::has_error is C++26; this library does not have it yet");
#endif
}

#if TEST_HAS_EH
TEST_CASE(expected, comparison_and_access_error_cxx23) {
    typedef STD::expected<int, STD::string> E;
    E a(7), b(7), c(8);
    E e1(STD::unexpected<STD::string>(STD::string("x")));
    E e2(STD::unexpected<STD::string>(STD::string("x")));

    test_true( a == b );
    test_true( !(a == c) );
    test_true( e1 == e2 );
    test_true( !(a == e1) );
    test_true( a == 7 );
    test_pass("cxx23:expected::operator==");

    STD::unexpected<STD::string> u1(STD::string("y")), u2(STD::string("y")),
                                 u3(STD::string("z"));
    test_true( u1 == u2 );
    test_true( !(u1 == u3) );
    test_pass("cxx23:unexpected::operator==");

    test_true( u1.error() == "y" );
    u1.error() += "!";
    test_true( u1.error() == "y!" );
    test_pass("cxx23:unexpected::error");

    bool caught = false;
    try { (void)e1.value(); }
    catch (const STD::bad_expected_access<STD::string>& ex) {
        caught = true;
        test_true( ex.error() == "x" );
        test_pass("cxx23:bad_expected_access::error");
        test_true( ex.what() != 0 );
        test_pass("cxx23:bad_expected_access::what");
    }
    test_true( caught );
    test_true(( STD::is_base_of<STD::bad_expected_access<void>,
                                STD::bad_expected_access<STD::string> >::value ));
    test_pass("cxx23:bad_expected_access");
}
#else   // !TEST_HAS_EH
TEST_CASE(expected, comparison_and_access_error_cxx23) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx23:expected::operator==");
    test_skip("cxx23:unexpected::operator==");
    test_skip("cxx23:unexpected::error");
    test_skip("cxx23:bad_expected_access::error");
    test_skip("cxx23:bad_expected_access::what");
    test_skip("cxx23:bad_expected_access");
}
#endif  // TEST_HAS_EH
#else
TEST_CASE(expected, observers_and_types_cxx23) {
    test_skip("cxx23:expected::has_value");
    test_skip("cxx23:expected::operator*");
    test_skip("cxx23:expected::operator->");
    test_skip("cxx23:expected::error");
    test_skip("cxx23:expected::value_or");
    test_skip("cxx23:expected::value_type");
    test_skip("cxx23:expected::error_type");
    test_skip("cxx23:expected::unexpected_type");
    test_skip("cxx23:expected::rebind");
}
TEST_CASE(expected, comparison_and_access_error_cxx23) {
    test_skip("cxx23:expected::operator==");
    test_skip("cxx23:unexpected::operator==");
    test_skip("cxx23:unexpected::error");
    test_skip("cxx23:bad_expected_access::error");
    test_skip("cxx23:bad_expected_access::what");
    test_skip("cxx23:bad_expected_access");
}
#endif

TEST_CASE(expected, feature_test_macros) {
#if defined(__cpp_lib_expected) && (__cpp_lib_expected) >= 202202L
    test_true( (__cpp_lib_expected) >= 202202L );
    test_pass("cxx23:__cpp_lib_expected=202202L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_expected=202202L");
#endif
#if defined(__cpp_lib_expected) && (__cpp_lib_expected) >= 202211L
    test_true( (__cpp_lib_expected) >= 202211L );
    test_pass("cxx23:__cpp_lib_expected=202211L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_expected=202211L");
#endif
#if defined(__cpp_lib_freestanding_expected)
    test_true( __cpp_lib_freestanding_expected > 0L );
    test_pass("cxx26:__cpp_lib_freestanding_expected");
#else
    test_skip("cxx26:__cpp_lib_freestanding_expected");
#endif
#if defined(__cpp_lib_constrained_equality)
    test_true( __cpp_lib_constrained_equality > 0L );
    test_pass("cxx26:__cpp_lib_constrained_equality");
#else
    test_skip("cxx26:__cpp_lib_constrained_equality");
#endif
#if defined(__cpp_lib_constexpr_exceptions)
    test_true( __cpp_lib_constexpr_exceptions > 0L );
    test_pass("cxx26:__cpp_lib_constexpr_exceptions");
#else
    test_skip("cxx26:__cpp_lib_constexpr_exceptions");
#endif
}

#if TEST_TARGET_CXX >= 2023 && defined(__cpp_lib_expected)
#if TEST_HAS_EH
TEST_CASE(expected, special_members_cxx23) {
    typedef STD::expected<int, STD::string> E;
    typedef STD::expected<STD::string, int> ES;

    E d;
    test_true( d.has_value() );
    test_eq( *d, 0 );
    ES ds;
    test_true( ds.has_value() );
    test_eq( *ds, STD::string() );
    test_pass("cxx23:expected::expected()");

    ES src(STD::string("hello"));
    ES copy(src);
    test_true( copy.has_value() );
    test_eq( *copy, STD::string("hello") );
    test_true( &*copy != &*src );
    E err_src(STD::unexpected<STD::string>(STD::string("bad")));
    E err_copy(err_src);
    test_true( !err_copy.has_value() );
    test_eq( err_copy.error(), STD::string("bad") );
    test_pass("cxx23:expected::expected(const expected&)");

    E fromval = 5;
    test_true( fromval.has_value() );
    test_eq( *fromval, 5 );
    ES fromlit = STD::string("abc");
    test_eq( *fromlit, STD::string("abc") );
    test_pass("cxx23:expected::expected(U&&)");

    E fromunex = STD::unexpected<STD::string>(STD::string("boom"));
    test_true( !fromunex.has_value() );
    test_true( !fromunex );
    test_eq( fromunex.error(), STD::string("boom") );
    test_pass("cxx23:expected::expected(unexpected<G>)");

    ES built(STD::in_place, 3, 'x');
    test_true( built.has_value() );
    test_eq( *built, STD::string("xxx") );
    E built_int(STD::in_place, 7);
    test_eq( *built_int, 7 );
    test_pass("cxx23:expected::expected(in_place_t, Args...)");

    E built_err(STD::unexpect, 3, 'e');
    test_true( !built_err.has_value() );
    test_eq( built_err.error(), STD::string("eee") );
    test_pass("cxx23:expected::expected(unexpect_t, Args...)");

    E a = 1, b = 2;
    E ea(STD::unexpect, "p"), eb(STD::unexpect, "q");
    a = b;   test_eq( *a, 2 );
    a = ea;  test_true( !a.has_value() && a.error() == STD::string("p") );
    a = eb;  test_eq( a.error(), STD::string("q") );
    a = b;   test_eq( *a, 2 );
    test_pass("cxx23:expected::operator=(const expected&)");

    E sw(STD::unexpect, "z");
    sw = 42;
    test_true( sw.has_value() );
    test_eq( *sw, 42 );
    sw = 43;
    test_eq( *sw, 43 );
    test_pass("cxx23:expected::operator=(U&&)");

    E sw2 = 1;
    sw2 = STD::unexpected<STD::string>(STD::string("nope"));
    test_true( !sw2.has_value() );
    test_eq( sw2.error(), STD::string("nope") );
    sw2 = STD::unexpected<STD::string>(STD::string("still"));
    test_eq( sw2.error(), STD::string("still") );
    test_pass("cxx23:expected::operator=(unexpected<G>)");

    test_true( (bool)b == b.has_value() );
    test_true( (bool)ea == ea.has_value() );
    if (b) test_true( true ); else test_true( false );
    test_true( !ea );
    test_pass("cxx23:expected::operator bool");

    test_eq( b.value(), 2 );
    b.value() = 3;
    test_eq( *b, 3 );
    test_throw( (void)ea.value() );
    bool caught = false;
    try { (void)ea.value(); }
    catch (const STD::bad_expected_access<STD::string>& x) {
        caught = true;
        test_eq( x.error(), STD::string("p") );
    } catch (...) {}
    test_true( caught );
    test_pass("cxx23:expected::value");

    E em(STD::unexpect, "gone");
    int& r = em.emplace(11);
    test_eq( r, 11 );
    test_true( em.has_value() );
    test_eq( *em, 11 );
    ES ems(STD::unexpect, 1);
    STD::string& rs = ems.emplace();
    test_true( ems.has_value() );
    test_eq( rs, STD::string() );
    rs = "yy";
    test_eq( *ems, STD::string("yy") );
    test_pass("cxx23:expected::emplace");
}
#else   // !TEST_HAS_EH
TEST_CASE(expected, special_members_cxx23) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx23:expected::expected()");
    test_skip("cxx23:expected::expected(const expected&)");
    test_skip("cxx23:expected::expected(U&&)");
    test_skip("cxx23:expected::expected(unexpected<G>)");
    test_skip("cxx23:expected::expected(in_place_t, Args...)");
    test_skip("cxx23:expected::expected(unexpect_t, Args...)");
    test_skip("cxx23:expected::operator=(const expected&)");
    test_skip("cxx23:expected::operator=(U&&)");
    test_skip("cxx23:expected::operator=(unexpected<G>)");
    test_skip("cxx23:expected::operator bool");
    test_skip("cxx23:expected::value");
    test_skip("cxx23:expected::emplace");
}
#endif  // TEST_HAS_EH

TEST_CASE(expected, unexpected_and_tags_cxx23) {
    STD::unexpected<STD::string> un(STD::string("err"));
    test_eq( un.error(), STD::string("err") );
    un.error() = "err2";
    test_eq( un.error(), STD::string("err2") );
    STD::unexpected<STD::string> built(STD::in_place, 3, 'z');
    test_eq( built.error(), STD::string("zzz") );
    test_pass("cxx23:unexpected::unexpected");

    STD::unexpected<STD::string> a(STD::string("a")), b(STD::string("b"));
    a.swap(b);
    test_eq( a.error(), STD::string("b") );
    test_eq( b.error(), STD::string("a") );
    test_pass("cxx23:unexpected::swap");

    STD::unexpect_t tag = STD::unexpect_t();
    (void)tag;
    test_true(( !STD::is_same<STD::unexpect_t, STD::in_place_t>::value ));
    test_true(( STD::is_same<STD::unexpect_t,
                             STD::remove_cv<STD::remove_reference<
                                 decltype(STD::unexpect)>::type>::type>::value ));
    test_pass("cxx23:unexpect_t");

    STD::expected<int, STD::string> e(STD::unexpect, 3, 'q');
    test_true( !e.has_value() );
    test_eq( e.error(), STD::string("qqq") );
    test_pass("cxx23:unexpect");
}

#if TEST_HAS_EH
TEST_CASE(expected, constexpr_exceptions_cxx26) {
#if defined(__cpp_lib_constexpr_exceptions) && (__cpp_lib_constexpr_exceptions) >= 202502L
    {
        struct K {
            static constexpr int ok() {
                STD::expected<int, int> e(5);
                return e.value();
            }
            static constexpr int caught() {
                STD::expected<int, int> e(STD::unexpect, 9);
                try { return e.value(); }
                catch (const STD::bad_expected_access<int>& x) { return -x.error(); }
            }
        };
        static_assert(K::ok() == 5, "expected::value in a constant expression");
        static_assert(K::caught() == -9, "bad_expected_access thrown and caught at compile time");
        test_eq( K::ok(), 5 );
        test_eq( K::caught(), -9 );
    }
    test_pass("cxx26:constexpr expected exceptions");
#else
    TEST_NOTE("constexpr exceptions are C++26 (P3068); not in this library");
    test_skip("cxx26:constexpr expected exceptions");
#endif
}
#else   // !TEST_HAS_EH
TEST_CASE(expected, constexpr_exceptions_cxx26) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx26:constexpr expected exceptions");
}
#endif  // TEST_HAS_EH
#else
TEST_CASE(expected, special_members_cxx23) {
    test_skip("cxx23:expected::expected()");
    test_skip("cxx23:expected::expected(const expected&)");
    test_skip("cxx23:expected::expected(U&&)");
    test_skip("cxx23:expected::expected(unexpected<G>)");
    test_skip("cxx23:expected::expected(in_place_t, Args...)");
    test_skip("cxx23:expected::expected(unexpect_t, Args...)");
    test_skip("cxx23:expected::operator=(const expected&)");
    test_skip("cxx23:expected::operator=(U&&)");
    test_skip("cxx23:expected::operator=(unexpected<G>)");
    test_skip("cxx23:expected::operator bool");
    test_skip("cxx23:expected::value");
    test_skip("cxx23:expected::emplace");
}
TEST_CASE(expected, unexpected_and_tags_cxx23) {
    test_skip("cxx23:unexpected::unexpected");
    test_skip("cxx23:unexpected::swap");
    test_skip("cxx23:unexpect_t");
    test_skip("cxx23:unexpect");
}
TEST_CASE(expected, constexpr_exceptions_cxx26) {
    test_skip("cxx26:constexpr expected exceptions");
}
#endif
