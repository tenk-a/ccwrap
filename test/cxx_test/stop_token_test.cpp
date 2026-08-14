#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2020 && _TST_TEST_HAS(<stop_token>) && _TST_HAS_CXX20_LIB_MEMBERS
#include <stop_token>
#include <type_traits>
#include <utility>

namespace {
struct CcwFlagSetter {
    bool* flag_;
    explicit CcwFlagSetter(bool* f) : flag_(f) {}
    void operator()() const { *flag_ = true; }
};
}

TEST_CASE(stop_token, source_cxx20) {
    STD::stop_source s;
    test_true( s.stop_possible() );
    test_true( !s.stop_requested() );
    test_pass("cxx20:stop_source");

    STD::stop_token t = s.get_token();
    test_true( t.stop_possible() );
    test_pass("cxx20:stop_source::get_token");

    test_true( s.request_stop() );
    test_true( s.stop_requested() );
    test_true( !s.request_stop() );
    test_pass("cxx20:stop_source::request_stop");

    test_true( s.stop_possible() );
    test_pass("cxx20:stop_source::stop_possible");
    test_pass("cxx20:stop_source::stop_requested");

    STD::stop_source a;
    STD::stop_source b = a;
    test_true( a == b );
    STD::stop_source c;
    test_true( !(a == c) );
    test_pass("cxx20:stop_source::operator==");

    STD::stop_source x, y;
    y.request_stop();
    x.swap(y);
    test_true( x.stop_requested() );
    test_true( !y.stop_requested() );
    test_pass("cxx20:stop_source::swap");
}

TEST_CASE(stop_token, token_cxx20) {
    STD::stop_source s;
    STD::stop_token t = s.get_token();
    test_true( t.stop_possible() );
    test_true( !t.stop_requested() );
    test_pass("cxx20:stop_token");
    test_pass("cxx20:stop_token::stop_possible");

    s.request_stop();
    test_true( t.stop_requested() );
    test_pass("cxx20:stop_token::stop_requested");

    STD::stop_token t2 = s.get_token();
    test_true( t == t2 );
    STD::stop_source other;
    test_true( !(t == other.get_token()) );
    test_pass("cxx20:stop_token::operator==");

    STD::stop_token u;
    test_true( !u.stop_possible() );
    test_true( !u.stop_requested() );
    u.swap(t);
    test_true( u.stop_requested() );
    test_true( !t.stop_possible() );
    test_pass("cxx20:stop_token::swap");
}

TEST_CASE(stop_token, callback_cxx20) {
    STD::stop_source s;
    bool fired = false;
    {
        STD::stop_callback<CcwFlagSetter> cb(s.get_token(), CcwFlagSetter(&fired));
        test_true( !fired );
        s.request_stop();
        test_true( fired );
    }
    test_pass("cxx20:stop_callback");

    bool late = false;
    STD::stop_callback<CcwFlagSetter> cb2(s.get_token(), CcwFlagSetter(&late));
    test_true( late );

    test_true( (STD::is_same<STD::stop_callback<CcwFlagSetter>::callback_type,
                             CcwFlagSetter>::value) );
    test_pass("cxx20:stop_callback::callback_type");

    TEST_NOTE("stop_callback::operator= is deleted by the standard");
    test_skip("cxx20:stop_callback::operator=");
}

TEST_CASE(stop_token, nostopstate_cxx20) {
    STD::stop_source s(STD::nostopstate);
    test_true( !s.stop_possible() );
    test_true( !s.stop_requested() );
    test_true( !s.request_stop() );
    test_pass("cxx20:nostopstate");

#if _TST_HAS_DECLTYPE
    test_true( (STD::is_same<const STD::nostopstate_t&,
                             decltype((STD::nostopstate))>::value) );
#else
    TEST_SKIP1();
#endif
    STD::nostopstate_t n;
    STD::stop_source s2(n);
    test_true( !s2.stop_possible() );
    test_pass("cxx20:nostopstate_t");
}

#if defined(__cpp_lib_stoptoken) && __cpp_lib_stoptoken >= 202406L
TEST_CASE(stop_token, inplace_source_cxx26) {
    STD::inplace_stop_source s;
    test_true( s.stop_possible() );
    test_true( !s.stop_requested() );
    test_pass("cxx26:inplace_stop_source");

    STD::inplace_stop_token t = s.get_token();
    test_true( t.stop_possible() );
    test_pass("cxx26:inplace_stop_source::get_token");

    test_true( s.request_stop() );
    test_true( s.stop_requested() );
    test_pass("cxx26:inplace_stop_source::request_stop");
    test_pass("cxx26:inplace_stop_source::stop_possible");
    test_pass("cxx26:inplace_stop_source::stop_requested");

    TEST_NOTE("inplace_stop_source::operator= is deleted by the standard");
    test_skip("cxx26:inplace_stop_source::operator=");
}

TEST_CASE(stop_token, inplace_token_cxx26) {
    STD::inplace_stop_source s;
    STD::inplace_stop_token t = s.get_token();
    test_true( t.stop_possible() );
    test_true( !t.stop_requested() );
    test_pass("cxx26:inplace_stop_token");
    test_pass("cxx26:inplace_stop_token::stop_possible");

    s.request_stop();
    test_true( t.stop_requested() );
    test_pass("cxx26:inplace_stop_token::stop_requested");

    STD::inplace_stop_token t2 = s.get_token();
    test_true( t == t2 );
    test_pass("cxx26:inplace_stop_token::operator==");

    STD::inplace_stop_token u;
    u.swap(t);
    test_true( u.stop_requested() );
    test_pass("cxx26:inplace_stop_token::swap");

    test_true( sizeof(STD::inplace_stop_token::callback_type<CcwFlagSetter>) > 0 );
    test_pass("cxx26:inplace_stop_token::callback_type");

    bool fired = false;
    STD::inplace_stop_source s2;
    {
        STD::inplace_stop_callback<CcwFlagSetter> cb(s2.get_token(), CcwFlagSetter(&fired));
        test_true( !fired );
        s2.request_stop();
        test_true( fired );
    }
    test_pass("cxx26:inplace_stop_callback");
    test_true( (STD::is_same<STD::inplace_stop_callback<CcwFlagSetter>::callback_type,
                             CcwFlagSetter>::value) );
    test_pass("cxx26:inplace_stop_callback::callback_type");

    TEST_NOTE("inplace_stop_callback::operator= is deleted by the standard");
    test_skip("cxx26:inplace_stop_callback::operator=");
}

TEST_CASE(stop_token, never_stop_token_cxx26) {
    STD::never_stop_token n;
    test_true( !n.stop_possible() );
    test_true( !n.stop_requested() );
    test_pass("cxx26:never_stop_token");
    test_pass("cxx26:never_stop_token::stop_possible");
    test_pass("cxx26:never_stop_token::stop_requested");

    STD::never_stop_token n2;
    test_true( n == n2 );
    test_pass("cxx26:never_stop_token::operator==");

    test_true( sizeof(STD::never_stop_token::callback_type<CcwFlagSetter>) > 0 );
    test_pass("cxx26:never_stop_token::callback_type");
}

TEST_CASE(stop_token, concepts_cxx26) {
    test_true( STD::stoppable_token<STD::stop_token> );
    test_true( STD::stoppable_token<STD::inplace_stop_token> );
    test_pass("cxx26:stoppable_token");

    test_true( STD::unstoppable_token<STD::never_stop_token> );
    test_true( !STD::unstoppable_token<STD::stop_token> );
    test_pass("cxx26:unstoppable_token");

    test_true( (STD::is_same<STD::stop_callback_for_t<STD::stop_token, CcwFlagSetter>,
                             STD::stop_callback<CcwFlagSetter> >::value) );
    test_pass("cxx26:stop_callback_for_t");

    test_true( sizeof(STD::stop_token::callback_type<CcwFlagSetter>) > 0 );
    test_pass("cxx26:stop_token::callback_type");
}
#else
TEST_CASE(stop_token, inplace_source_cxx26) {
    test_skip("cxx26:inplace_stop_source");
    test_skip("cxx26:inplace_stop_source::get_token");
    test_skip("cxx26:inplace_stop_source::request_stop");
    test_skip("cxx26:inplace_stop_source::stop_possible");
    test_skip("cxx26:inplace_stop_source::stop_requested");
    test_skip("cxx26:inplace_stop_source::operator=");
}
TEST_CASE(stop_token, inplace_token_cxx26) {
    test_skip("cxx26:inplace_stop_token");
    test_skip("cxx26:inplace_stop_token::stop_possible");
    test_skip("cxx26:inplace_stop_token::stop_requested");
    test_skip("cxx26:inplace_stop_token::operator==");
    test_skip("cxx26:inplace_stop_token::swap");
    test_skip("cxx26:inplace_stop_token::callback_type");
    test_skip("cxx26:inplace_stop_callback");
    test_skip("cxx26:inplace_stop_callback::callback_type");
    test_skip("cxx26:inplace_stop_callback::operator=");
}
TEST_CASE(stop_token, never_stop_token_cxx26) {
    test_skip("cxx26:never_stop_token");
    test_skip("cxx26:never_stop_token::stop_possible");
    test_skip("cxx26:never_stop_token::stop_requested");
    test_skip("cxx26:never_stop_token::operator==");
    test_skip("cxx26:never_stop_token::callback_type");
}
TEST_CASE(stop_token, concepts_cxx26) {
    test_skip("cxx26:stoppable_token");
    test_skip("cxx26:unstoppable_token");
    test_skip("cxx26:stop_callback_for_t");
    test_skip("cxx26:stop_token::callback_type");
}
#endif

#else   // no <stop_token> at this target / in this library
TEST_CASE(stop_token, source_cxx20) {
    test_skip("cxx20:stop_source");
    test_skip("cxx20:stop_source::get_token");
    test_skip("cxx20:stop_source::request_stop");
    test_skip("cxx20:stop_source::stop_possible");
    test_skip("cxx20:stop_source::stop_requested");
    test_skip("cxx20:stop_source::operator==");
    test_skip("cxx20:stop_source::swap");
}
TEST_CASE(stop_token, token_cxx20) {
    test_skip("cxx20:stop_token");
    test_skip("cxx20:stop_token::stop_possible");
    test_skip("cxx20:stop_token::stop_requested");
    test_skip("cxx20:stop_token::operator==");
    test_skip("cxx20:stop_token::swap");
}
TEST_CASE(stop_token, callback_cxx20) {
    test_skip("cxx20:stop_callback");
    test_skip("cxx20:stop_callback::callback_type");
    test_skip("cxx20:stop_callback::operator=");
}
TEST_CASE(stop_token, nostopstate_cxx20) {
    test_skip("cxx20:nostopstate");
    test_skip("cxx20:nostopstate_t");
}
TEST_CASE(stop_token, inplace_source_cxx26) {
    test_skip("cxx26:inplace_stop_source");
    test_skip("cxx26:inplace_stop_source::get_token");
    test_skip("cxx26:inplace_stop_source::request_stop");
    test_skip("cxx26:inplace_stop_source::stop_possible");
    test_skip("cxx26:inplace_stop_source::stop_requested");
    test_skip("cxx26:inplace_stop_source::operator=");
}
TEST_CASE(stop_token, inplace_token_cxx26) {
    test_skip("cxx26:inplace_stop_token");
    test_skip("cxx26:inplace_stop_token::stop_possible");
    test_skip("cxx26:inplace_stop_token::stop_requested");
    test_skip("cxx26:inplace_stop_token::operator==");
    test_skip("cxx26:inplace_stop_token::swap");
    test_skip("cxx26:inplace_stop_token::callback_type");
    test_skip("cxx26:inplace_stop_callback");
    test_skip("cxx26:inplace_stop_callback::callback_type");
    test_skip("cxx26:inplace_stop_callback::operator=");
}
TEST_CASE(stop_token, never_stop_token_cxx26) {
    test_skip("cxx26:never_stop_token");
    test_skip("cxx26:never_stop_token::stop_possible");
    test_skip("cxx26:never_stop_token::stop_requested");
    test_skip("cxx26:never_stop_token::operator==");
    test_skip("cxx26:never_stop_token::callback_type");
}
TEST_CASE(stop_token, concepts_cxx26) {
    test_skip("cxx26:stoppable_token");
    test_skip("cxx26:unstoppable_token");
    test_skip("cxx26:stop_callback_for_t");
    test_skip("cxx26:stop_token::callback_type");
}
#endif

TEST_CASE(stop_token, feature_test_macros) {
#if defined(__cpp_lib_jthread) && (__cpp_lib_jthread) >= 201911L
    test_true( (__cpp_lib_jthread) >= 201911L );
    test_pass("cxx20:__cpp_lib_jthread=201911L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx20:__cpp_lib_jthread=201911L");
#endif
}
