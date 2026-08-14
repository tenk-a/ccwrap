#include "test_cxx.hpp"

TEST_CASE(debugging, header_compiles) {
#if _TST_TEST_HAS(<debugging>)
    test_true( true );
    test_pass("cxx26:<debugging> header");
#else
    TEST_NOTE("<debugging> is not shipped by this standard library");
    test_skip("cxx26:<debugging> header");
#endif
}

#if TEST_TARGET_CXX >= 2011 && _TST_TEST_HAS(<debugging>)
#include <debugging>
#include <type_traits>

TEST_CASE(debugging, is_debugger_present) {

    bool a = STD::is_debugger_present();
    bool b = STD::is_debugger_present();
    test_eq( a, b );
    test_true( a == true || a == false );

    #if _tst_cplusplus >= 201103L
    static_assert(STD::is_same<decltype(STD::is_debugger_present()), bool>::value,
                  "is_debugger_present() returns bool");
    #endif
    test_pass("cxx26:is_debugger_present");
}

TEST_CASE(debugging, breakpoint_if_debugging) {

    if (!STD::is_debugger_present()) {
        test_no_throw( STD::breakpoint_if_debugging() );
    }
    #if _tst_cplusplus >= 201103L
    static_assert(STD::is_same<decltype(STD::breakpoint_if_debugging()), void>::value,
                  "breakpoint_if_debugging() returns void");
    #endif
    test_pass("cxx26:breakpoint_if_debugging");
}

TEST_CASE(debugging, breakpoint) {

    void (*fp)() = &STD::breakpoint;
    test_true( fp != 0 );
    #if _tst_cplusplus >= 201103L
    static_assert(STD::is_same<decltype(STD::breakpoint()), void>::value,
                  "breakpoint() returns void");
    #endif

    if (false) {
        STD::breakpoint();
    }
    test_pass("cxx26:breakpoint");
}
#else
TEST_CASE(debugging, is_debugger_present) {
    TEST_NOTE("<debugging> absent or pre-C++11 front end");
    test_skip("cxx26:is_debugger_present");
}
TEST_CASE(debugging, breakpoint_if_debugging) {
    TEST_NOTE("<debugging> absent or pre-C++11 front end");
    test_skip("cxx26:breakpoint_if_debugging");
}
TEST_CASE(debugging, breakpoint) {
    TEST_NOTE("<debugging> absent or pre-C++11 front end");
    test_skip("cxx26:breakpoint");
}
#endif


TEST_CASE(debugging, feature_macro_cxx26) {
#if defined(__cpp_lib_debugging)
    test_true( __cpp_lib_debugging >= 202311L );
    test_pass("cxx26:__cpp_lib_debugging");
#else
    TEST_NOTE("__cpp_lib_debugging is not defined by this library");
    test_skip("cxx26:__cpp_lib_debugging");
#endif
}
