#include "test_cxx.hpp"

#include <ratio>
#include <type_traits>

TEST_CASE(ratio, basic) {
    typedef STD::ratio<1, 3> third;
    typedef STD::ratio<7> seven;

    test_eq( (long)third::num, 1L );
    test_eq( (long)STD::kilo::num, 1000L );

    test_true( STD::tera::num == 1000000000000LL );
    test_true( STD::peta::num == 1000000000000000LL );
    test_true( STD::exa::num == 1000000000000000000LL );
    test_eq( (long)seven::num, 7L );
    test_pass("cxx11:ratio::num");

    test_eq( (long)third::den, 3L );
    test_eq( (long)STD::milli::den, 1000L );
    test_true( STD::pico::den == 1000000000000LL );
    test_true( STD::femto::den == 1000000000000000LL );
    test_true( STD::atto::den == 1000000000000000000LL );
    test_eq( (long)seven::den, 1L );
    test_pass("cxx11:ratio::den");
}

namespace {
typedef STD::ratio<2, 4>     r24;
typedef STD::ratio<6, 9>     r69;
typedef STD::ratio<100, 10>  r100_10;
typedef STD::ratio<7, 3>     r73;
typedef STD::ratio<9, 4>     r94;
typedef STD::ratio<0, 5>     r05;
typedef STD::ratio<1000000000000LL, 2000000000000LL> r_big;
typedef STD::ratio<(1LL << 40), (1LL << 36)>         r_pow2;
typedef STD::ratio<-2, 4>    rm2_4;
typedef STD::ratio<2, -4>    r2_m4;
typedef STD::ratio<-2, -4>   rm2_m4;
typedef STD::ratio<-7, 1>    rm7_1;
typedef STD::ratio<1, 2>     r12;
typedef STD::ratio<2, 3>     r23;
typedef STD::ratio<1, 3>     r13;
typedef STD::ratio<-1, 2>    rm1_2;
}

TEST_CASE(ratio, reduce) {
    test_eq( (long)r24::num, 1L );
    test_eq( (long)r24::den, 2L );
    test_eq( (long)r69::num, 2L );
    test_eq( (long)r69::den, 3L );
    test_eq( (long)r100_10::num, 10L );
    test_eq( (long)r100_10::den, 1L );

    test_eq( (long)r73::num, 7L );
    test_eq( (long)r73::den, 3L );

    test_eq( (long)r94::num, 9L );
    test_eq( (long)r94::den, 4L );

    test_eq( (long)r05::num, 0L );
    test_eq( (long)r05::den, 1L );

    test_true( r_big::num == 1LL );
    test_true( r_big::den == 2LL );
    test_true( r_pow2::num == 16LL );
    test_true( r_pow2::den == 1LL );
    test_pass("cxx11:ratio reduction");
}

TEST_CASE(ratio, sign_normalisation) {
    test_eq( (long)rm2_4::num, -1L );
    test_eq( (long)rm2_4::den, 2L );
    test_eq( (long)r2_m4::num, -1L );
    test_eq( (long)r2_m4::den, 2L );
    test_eq( (long)rm2_m4::num, 1L );
    test_eq( (long)rm2_m4::den, 2L );
    test_eq( (long)rm7_1::num, -7L );
    test_eq( (long)rm7_1::den, 1L );
    test_pass("cxx11:ratio sign normalisation");
}

TEST_CASE(ratio, canonical_type) {
    test_eq( (long)r24::type::num, 1L );
    test_eq( (long)r24::type::den, 2L );
    test_true( (STD::is_same<r24::type, r12>::value) );
    test_true( (STD::is_same<r69::type, r23>::value) );
    test_true( (STD::is_same<r2_m4::type, rm1_2>::value) );

    test_true( (STD::is_same<r12::type, r12>::value) );

    test_true( !(STD::is_same<r12::type, r13>::value) );
    test_pass("cxx11:ratio::type canonical");
}

#if defined(_MSC_VER) && _MSC_VER < 1800
template <class _R> struct rat { typedef typename _R::type type; };
#else
template <class _R> struct rat { typedef _R type; };
#endif

TEST_CASE(ratio, arithmetic) {
    typedef r12 half;
    typedef r13 third;
    typedef STD::ratio<-1, 3> minus_third;
    typedef STD::ratio<-1, 2> minus_half2;

    typedef rat<STD::ratio_add<half, third> >::type sum;
    test_eq( (long)sum::num, 5L );
    test_eq( (long)sum::den, 6L );
    typedef rat<STD::ratio_add<sum, sum> >::type twice;
    test_eq( (long)twice::num, 5L );
    test_eq( (long)twice::den, 3L );
    test_pass("cxx11:ratio_add");

    typedef rat<STD::ratio_subtract<half, third> >::type diff;
    test_eq( (long)diff::num, 1L );
    test_eq( (long)diff::den, 6L );
    typedef rat<STD::ratio_subtract<half, half> >::type zero;
    test_eq( (long)zero::num, 0L );
    test_eq( (long)zero::den, 1L );
    typedef rat<STD::ratio_subtract<third, half> >::type neg;
    test_eq( (long)neg::num, -1L );
    test_eq( (long)neg::den, 6L );
    test_pass("cxx11:ratio_subtract");

    typedef rat<STD::ratio_multiply<half, third> >::type prod;
    test_eq( (long)prod::num, 1L );
    test_eq( (long)prod::den, 6L );
    typedef rat<STD::ratio_multiply<minus_half2, minus_third> >::type pos;
    test_eq( (long)pos::num, 1L );
    test_eq( (long)pos::den, 6L );
    test_pass("cxx11:ratio_multiply");

    typedef rat<STD::ratio_divide<half, third> >::type quot;
    test_eq( (long)quot::num, 3L );
    test_eq( (long)quot::den, 2L );
    typedef rat<STD::ratio_divide<half, minus_third> >::type negquot;
    test_eq( (long)negquot::num, -3L );
    test_eq( (long)negquot::den, 2L );
    test_pass("cxx11:ratio_divide");
}

TEST_CASE(ratio, compare) {
    typedef r12 half;
    typedef r24 half_unreduced;
    typedef r13 third;
    typedef rm1_2 minus_half;
    typedef STD::ratio<1, -2> minus_half_unreduced;

    TEST_SKIP_VC_RANGE(1600, 1800, "vc10/vc11 ratio_equal compares the unreduced arguments");
    test_true( (STD::ratio_equal<half, half_unreduced>::value) );
    test_true( !(STD::ratio_equal<half, third>::value) );
    TEST_SKIP_VC_RANGE(1600, 1800, "vc10/vc11 ratio_equal compares the unreduced arguments");
    test_true( (STD::ratio_equal<minus_half, minus_half_unreduced>::value) );
    test_pass("cxx11:ratio_equal");

    test_true( (STD::ratio_not_equal<half, third>::value) );
    test_pass("cxx11:ratio_not_equal");

    test_true( (STD::ratio_less<third, half>::value) );
    test_true( !(STD::ratio_less<half, third>::value) );
    test_true( (STD::ratio_less<minus_half, third>::value) );
    test_pass("cxx11:ratio_less");

    test_true( (STD::ratio_less_equal<half, half_unreduced>::value) );
    test_pass("cxx11:ratio_less_equal");

    test_true( (STD::ratio_greater<half, third>::value) );
    test_pass("cxx11:ratio_greater");

    test_true( (STD::ratio_greater_equal<half_unreduced, half>::value) );
    test_pass("cxx11:ratio_greater_equal");
}

TEST_CASE(ratio, si_prefixes) {

    test_true( STD::atto::num  == 1 && STD::atto::den  == 1000000000000000000LL ); test_pass("cxx11:atto");
    test_true( STD::femto::num == 1 && STD::femto::den == 1000000000000000LL );    test_pass("cxx11:femto");
    test_true( STD::pico::num  == 1 && STD::pico::den  == 1000000000000LL );       test_pass("cxx11:pico");
    test_true( STD::nano::num  == 1 && STD::nano::den  == 1000000000LL );          test_pass("cxx11:nano");
    test_true( STD::micro::num == 1 && STD::micro::den == 1000000LL );             test_pass("cxx11:micro");
    test_true( STD::milli::num == 1 && STD::milli::den == 1000LL );                test_pass("cxx11:milli");
    test_true( STD::centi::num == 1 && STD::centi::den == 100LL );                 test_pass("cxx11:centi");
    test_true( STD::deci::num  == 1 && STD::deci::den  == 10LL );                  test_pass("cxx11:deci");

    test_true( STD::deca::num  == 10LL       && STD::deca::den  == 1 );  test_pass("cxx11:deca");
    test_true( STD::hecto::num == 100LL      && STD::hecto::den == 1 );  test_pass("cxx11:hecto");
    test_true( STD::kilo::num  == 1000LL     && STD::kilo::den  == 1 );  test_pass("cxx11:kilo");
    test_true( STD::mega::num  == 1000000LL  && STD::mega::den  == 1 );  test_pass("cxx11:mega");
    test_true( STD::giga::num  == 1000000000LL       && STD::giga::den  == 1 ); test_pass("cxx11:giga");
    test_true( STD::tera::num  == 1000000000000LL    && STD::tera::den  == 1 ); test_pass("cxx11:tera");
    test_true( STD::peta::num  == 1000000000000000LL && STD::peta::den  == 1 ); test_pass("cxx11:peta");
    test_true( STD::exa::num   == 1000000000000000000LL && STD::exa::den == 1 ); test_pass("cxx11:exa");

    test_skip("cxx11:zepto");
    test_skip("cxx11:yocto");
    test_skip("cxx11:zetta");
    test_skip("cxx11:yotta");

    test_skip("cxx26:ronto");
    test_skip("cxx26:quecto");
    test_skip("cxx26:ronna");
    test_skip("cxx26:quetta");
}


#if TEST_TARGET_CXX >= 2017 && _TST_HAS_VAR_TEMPLATE
TEST_CASE(ratio, comparison_v_cxx17) {
    typedef STD::ratio<1, 2> half;
    typedef STD::ratio<2, 4> half_again;
    typedef STD::ratio<3, 4> three_q;

    test_true(  (STD::ratio_equal_v<half, half_again>) );
    test_true( !(STD::ratio_equal_v<half, three_q>) );
    test_true(  (STD::ratio_equal_v<half, half_again>) == (STD::ratio_equal<half, half_again>::value) );
    test_pass("cxx17:ratio_equal_v");

    test_true(  (STD::ratio_not_equal_v<half, three_q>) );
    test_true( !(STD::ratio_not_equal_v<half, half_again>) );
    test_pass("cxx17:ratio_not_equal_v");

    test_true(  (STD::ratio_less_v<half, three_q>) );
    test_true( !(STD::ratio_less_v<three_q, half>) );
    test_true( !(STD::ratio_less_v<half, half_again>) );
    test_pass("cxx17:ratio_less_v");

    test_true(  (STD::ratio_less_equal_v<half, three_q>) );
    test_true(  (STD::ratio_less_equal_v<half, half_again>) );
    test_true( !(STD::ratio_less_equal_v<three_q, half>) );
    test_pass("cxx17:ratio_less_equal_v");

    test_true(  (STD::ratio_greater_v<three_q, half>) );
    test_true( !(STD::ratio_greater_v<half, three_q>) );
    test_pass("cxx17:ratio_greater_v");

    test_true(  (STD::ratio_greater_equal_v<three_q, half>) );
    test_true(  (STD::ratio_greater_equal_v<half, half_again>) );
    test_true( !(STD::ratio_greater_equal_v<half, three_q>) );
    test_pass("cxx17:ratio_greater_equal_v");
}
#else
TEST_CASE(ratio, comparison_v_cxx17) {
    test_skip("cxx17:ratio_equal_v");
    test_skip("cxx17:ratio_not_equal_v");
    test_skip("cxx17:ratio_less_v");
    test_skip("cxx17:ratio_less_equal_v");
    test_skip("cxx17:ratio_greater_v");
    test_skip("cxx17:ratio_greater_equal_v");
}
#endif

TEST_CASE(ratio, feature_test_macros) {
#if defined(__cpp_lib_ratio) && (__cpp_lib_ratio) >= 202306L
    test_true( (__cpp_lib_ratio) >= 202306L );
    test_pass("cxx26:__cpp_lib_ratio=202306L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx26:__cpp_lib_ratio=202306L");
#endif
}


TEST_CASE(ratio, class_template_cxx11) {
    typedef STD::ratio<6, 8> R;
    test_true( R::num == 3 );
    test_true( R::den == 4 );

    typedef STD::ratio<2, -4> N;
    test_true( N::num == -1 );
    test_true( N::den == 2 );

    test_true(( STD::is_same<STD::ratio<6, 8>::type, STD::ratio<3, 4> >::value ));
    test_true( STD::ratio<5>::den == 1 );
    test_pass("cxx11:ratio");
}
