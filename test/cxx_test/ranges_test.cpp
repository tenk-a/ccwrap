#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2020
#include <ranges>
#include <vector>

TEST_CASE(ranges, access_functions) {
    STD::vector<int> v;
    for (int i = 0; i < 5; ++i) v.push_back(i * 10);

    test_eq( *STD::ranges::begin(v), 0 );
    test_pass("cxx20:ranges::begin");
    test_eq( (int)(STD::ranges::end(v) - STD::ranges::begin(v)), 5 );
    test_pass("cxx20:ranges::end");

    test_eq( *STD::ranges::cbegin(v), 0 );
    test_pass("cxx20:ranges::cbegin");
    test_true( STD::ranges::cbegin(v) != STD::ranges::cend(v) );
    test_pass("cxx20:ranges::cend");

    test_eq( *STD::ranges::rbegin(v), 40 );
    test_pass("cxx20:ranges::rbegin");
    test_eq( *STD::ranges::crbegin(v), 40 );
    test_pass("cxx20:ranges::crbegin");

    test_eq( STD::ranges::size(v), (STD::size_t)5 );
    test_pass("cxx20:ranges::size");
    test_eq( STD::ranges::ssize(v), (STD::ptrdiff_t)5 );
    test_pass("cxx20:ranges::ssize");
    test_true( !STD::ranges::empty(v) );
    test_pass("cxx20:ranges::empty");
    test_eq( *STD::ranges::data(v), 0 );
    test_pass("cxx20:ranges::data");
    test_eq( *STD::ranges::cdata(v), 0 );
    test_pass("cxx20:ranges::cdata");

    STD::vector<int> e;
    test_true( STD::ranges::empty(e) );
    test_pass("cxx20:ranges::empty(empty-range)");

    STD::ranges::dangling d; (void)d;
    test_true( true );
    test_pass("cxx20:ranges::dangling");
    STD::ranges::view_base vb; (void)vb;
    test_true( 1 );
    test_pass("cxx20:ranges::view_base");
#if !defined(__GNUC__)
    STD::from_range_t fr = STD::from_range; (void)fr;
    test_true( 1 );
    test_pass("cxx23:ranges::from_range_t");
#else
    TEST_SKIP1();
#endif
}
#else
TEST_CASE_SKIP(ranges, access_functions)
#endif

TEST_CASE(ranges, feature_test_macros) {
#if defined(__cpp_lib_ranges) && (__cpp_lib_ranges) >= 201911L
    test_true( (__cpp_lib_ranges) >= 201911L );
    test_pass("cxx20:__cpp_lib_ranges=201911L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx20:__cpp_lib_ranges=201911L");
#endif
#if defined(__cpp_lib_ranges) && (__cpp_lib_ranges) >= 202302L
    test_true( (__cpp_lib_ranges) >= 202302L );
    test_pass("cxx23:__cpp_lib_ranges=202302L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_ranges=202302L");
#endif
#if defined(__cpp_lib_ranges_as_const) && (__cpp_lib_ranges_as_const) >= 202207L
    test_true( (__cpp_lib_ranges_as_const) >= 202207L );
    test_pass("cxx23:__cpp_lib_ranges_as_const=202207L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_ranges_as_const=202207L");
#endif
#if defined(__cpp_lib_ranges_as_rvalue) && (__cpp_lib_ranges_as_rvalue) >= 202207L
    test_true( (__cpp_lib_ranges_as_rvalue) >= 202207L );
    test_pass("cxx23:__cpp_lib_ranges_as_rvalue=202207L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_ranges_as_rvalue=202207L");
#endif
#if defined(__cpp_lib_ranges_cartesian_product) && (__cpp_lib_ranges_cartesian_product) >= 202207L
    test_true( (__cpp_lib_ranges_cartesian_product) >= 202207L );
    test_pass("cxx23:__cpp_lib_ranges_cartesian_product=202207L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_ranges_cartesian_product=202207L");
#endif
#if defined(__cpp_lib_ranges_chunk) && (__cpp_lib_ranges_chunk) >= 202202L
    test_true( (__cpp_lib_ranges_chunk) >= 202202L );
    test_pass("cxx23:__cpp_lib_ranges_chunk=202202L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_ranges_chunk=202202L");
#endif
#if defined(__cpp_lib_ranges_chunk_by) && (__cpp_lib_ranges_chunk_by) >= 202202L
    test_true( (__cpp_lib_ranges_chunk_by) >= 202202L );
    test_pass("cxx23:__cpp_lib_ranges_chunk_by=202202L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_ranges_chunk_by=202202L");
#endif
#if defined(__cpp_lib_ranges_enumerate) && (__cpp_lib_ranges_enumerate) >= 202302L
    test_true( (__cpp_lib_ranges_enumerate) >= 202302L );
    test_pass("cxx23:__cpp_lib_ranges_enumerate=202302L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_ranges_enumerate=202302L");
#endif
#if defined(__cpp_lib_ranges_join_with) && (__cpp_lib_ranges_join_with) >= 202202L
    test_true( (__cpp_lib_ranges_join_with) >= 202202L );
    test_pass("cxx23:__cpp_lib_ranges_join_with=202202L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_ranges_join_with=202202L");
#endif
#if defined(__cpp_lib_ranges_repeat) && (__cpp_lib_ranges_repeat) >= 202207L
    test_true( (__cpp_lib_ranges_repeat) >= 202207L );
    test_pass("cxx23:__cpp_lib_ranges_repeat=202207L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_ranges_repeat=202207L");
#endif
#if defined(__cpp_lib_ranges_slide) && (__cpp_lib_ranges_slide) >= 202202L
    test_true( (__cpp_lib_ranges_slide) >= 202202L );
    test_pass("cxx23:__cpp_lib_ranges_slide=202202L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_ranges_slide=202202L");
#endif
#if defined(__cpp_lib_ranges_stride) && (__cpp_lib_ranges_stride) >= 202207L
    test_true( (__cpp_lib_ranges_stride) >= 202207L );
    test_pass("cxx23:__cpp_lib_ranges_stride=202207L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_ranges_stride=202207L");
#endif
#if defined(__cpp_lib_ranges_to_container) && (__cpp_lib_ranges_to_container) >= 202202L
    test_true( (__cpp_lib_ranges_to_container) >= 202202L );
    test_pass("cxx23:__cpp_lib_ranges_to_container=202202L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_ranges_to_container=202202L");
#endif
#if defined(__cpp_lib_ranges_zip) && (__cpp_lib_ranges_zip) >= 202110L
    test_true( (__cpp_lib_ranges_zip) >= 202110L );
    test_pass("cxx23:__cpp_lib_ranges_zip=202110L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_ranges_zip=202110L");
#endif
#if defined(__cpp_lib_ranges) && (__cpp_lib_ranges) >= 202406L
    test_true( (__cpp_lib_ranges) >= 202406L );
    test_pass("cxx26:__cpp_lib_ranges=202406L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx26:__cpp_lib_ranges=202406L");
#endif
#if defined(__cpp_lib_ranges_cache_latest) && (__cpp_lib_ranges_cache_latest) >= 202411L
    test_true( (__cpp_lib_ranges_cache_latest) >= 202411L );
    test_pass("cxx26:__cpp_lib_ranges_cache_latest=202411L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx26:__cpp_lib_ranges_cache_latest=202411L");
#endif
#if defined(__cpp_lib_ranges_concat) && (__cpp_lib_ranges_concat) >= 202403L
    test_true( (__cpp_lib_ranges_concat) >= 202403L );
    test_pass("cxx26:__cpp_lib_ranges_concat=202403L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx26:__cpp_lib_ranges_concat=202403L");
#endif
#if defined(__cpp_lib_ranges_indices) && (__cpp_lib_ranges_indices) >= 202506L
    test_true( (__cpp_lib_ranges_indices) >= 202506L );
    test_pass("cxx26:__cpp_lib_ranges_indices=202506L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx26:__cpp_lib_ranges_indices=202506L");
#endif
#if defined(__cpp_lib_ranges_reserve_hint) && (__cpp_lib_ranges_reserve_hint) >= 202502L
    test_true( (__cpp_lib_ranges_reserve_hint) >= 202502L );
    test_pass("cxx26:__cpp_lib_ranges_reserve_hint=202502L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx26:__cpp_lib_ranges_reserve_hint=202502L");
#endif
#if defined(__cpp_lib_ranges_as_input) && (__cpp_lib_ranges_as_input) >= 202502L
    test_true( (__cpp_lib_ranges_as_input) >= 202502L );
    test_pass("cxx26:__cpp_lib_ranges_as_input=202502L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx26:__cpp_lib_ranges_as_input=202502L");
#endif
}
