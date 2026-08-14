#include "test_cxx.hpp"
#if TEST_TARGET_CXX >= 2020
#include <source_location>
#include <cstring>

TEST_CASE(source_location, accessors) {
    STD::source_location d;

    test_ptr( d.file_name() );
    test_ptr( d.function_name() );
    test_pass("cxx20:source_location default ctor");
    test_eq( (long)d.line(), 0L );
    test_pass("cxx20:source_location::line");
    test_eq( (long)d.column(), 0L );
    test_pass("cxx20:source_location::column");
    test_ptr( d.file_name() );
    test_eq( STD::strcmp(d.file_name(), ""), 0 );
    test_pass("cxx20:source_location::file_name");
    test_ptr( d.function_name() );
    test_pass("cxx20:source_location::function_name");

    STD::source_location c = STD::source_location::current();
    test_ptr( c.file_name() );
    test_ptr( c.function_name() );
    test_true( c.column() >= 0u );
    test_pass("cxx20:source_location::current");
}

#if !defined(__WATCOMC__) && !(defined(_MSC_VER) && _MSC_VER < 1926)
TEST_CASE(source_location, captures_caller) {
    STD::source_location here = STD::source_location::current();
    test_true( here.line() > 0u );
    test_true( STD::strlen(here.file_name()) > 0u );
    test_pass("cxx20:source_location::current() captures the caller");
}
#else
TEST_CASE_SKIP(source_location, captures_caller)
#endif

TEST_CASE(source_location, feature_test_macros) {
#if defined(__cpp_lib_source_location) && (__cpp_lib_source_location) >= 201907L
    test_true( (__cpp_lib_source_location) >= 201907L );
    test_pass("cxx20:__cpp_lib_source_location=201907L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx20:__cpp_lib_source_location=201907L");
#endif
}


TEST_CASE(source_location, class_cxx20) {
#if defined(__cpp_lib_source_location)
    STD::source_location d;
    test_true( d.line() == 0 || d.line() > 0 );

    STD::source_location here = STD::source_location::current();
    test_true( here.line() > 0 );
    test_true( here.function_name() != 0 );
    test_true( here.file_name() != 0 );

    STD::source_location copy = here;
    test_true( copy.line() == here.line() );
    test_true( copy.column() == here.column() );
    test_pass("cxx20:source_location");
#else
    TEST_NOTE("source_location needs __builtin_LINE and friends");
    test_skip("cxx20:source_location");
#endif
}
#else
TEST_CASE(source_location, accessors) {
    TEST_NOTE("this is a C++20 header and the build is below C++20");
    test_skip("cxx20:source_location default ctor");
    test_skip("cxx20:source_location::line");
    test_skip("cxx20:source_location::column");
    test_skip("cxx20:source_location::file_name");
    test_skip("cxx20:source_location::function_name");
    test_skip("cxx20:source_location::current");
}
TEST_CASE(source_location, captures_caller) {
    TEST_NOTE("this is a C++20 header and the build is below C++20");
    test_skip("cxx20:source_location::current() captures the caller");
}
TEST_CASE(source_location, feature_test_macros) {
    TEST_NOTE("this is a C++20 header and the build is below C++20");
    test_skip("cxx20:__cpp_lib_source_location=201907L");
}
TEST_CASE(source_location, class_cxx20) {
    TEST_NOTE("this is a C++20 header and the build is below C++20");
    test_skip("cxx20:source_location");
}
#endif
