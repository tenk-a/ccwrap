#include "test_cxx.hpp"
#include <stdexcept>
#include <exception>
#include <cstring>
#include <string>

TEST_CASE(stdexcept, logic_error_family) {

    STD::logic_error le("bad logic");
    test_eq( STD::strcmp(le.what(), "bad logic"), 0 );
    test_pass("cxx03:logic_error");

    STD::domain_error de("domain");
    test_eq( STD::strcmp(de.what(),  "domain"), 0 );
    test_pass("cxx03:domain_error");

    STD::invalid_argument ia("arg");
    test_eq( STD::strcmp(ia.what(),  "arg"), 0 );
    test_pass("cxx03:invalid_argument");

    STD::length_error len("len");
    test_eq( STD::strcmp(len.what(), "len"), 0 );
    test_pass("cxx03:length_error");

    STD::out_of_range oor("oor");
    test_eq( STD::strcmp(oor.what(), "oor"), 0 );

    STD::logic_error*   pl = &de;   test_ptr( pl );
    STD::exception*     pe = &ia;   test_ptr( pe );
    (void)pl; (void)pe;
    test_pass("cxx03:out_of_range");
}

TEST_CASE(stdexcept, runtime_error_family) {
    STD::runtime_error re("bad run");
    test_eq( STD::strcmp(re.what(), "bad run"), 0 );
    test_pass("cxx03:runtime_error");

    STD::range_error    rg("range");
    test_eq( STD::strcmp(rg.what(), "range"), 0 );
    test_pass("cxx03:range_error");

    STD::overflow_error ov("over");
    test_eq( STD::strcmp(ov.what(), "over"), 0 );
    test_pass("cxx03:overflow_error");

    STD::underflow_error un("under");
    test_eq( STD::strcmp(un.what(), "under"), 0 );
    STD::runtime_error* pr = &ov;   test_ptr( pr );
    STD::exception*     pe = &un;   test_ptr( pe );
    (void)pr; (void)pe;
    test_pass("cxx03:underflow_error");
}

#if TEST_HAS_EH
TEST_CASE(stdexcept, catch_by_base) {

    bool exact = false;
    try { throw STD::out_of_range("x"); }
    catch (const STD::out_of_range&) { exact = true; }
    catch (...) {}
    test_true( exact );

    bool as_logic = false;
    try { throw STD::out_of_range("x"); }
    catch (const STD::logic_error& e) { as_logic = (STD::strcmp(e.what(), "x") == 0); }
    catch (...) {}
    test_true( as_logic );

    bool as_runtime = false;
    try { throw STD::overflow_error("o"); }
    catch (const STD::runtime_error&) { as_runtime = true; }
    catch (...) {}
    test_true( as_runtime );

    bool as_exc = false;
    try { throw STD::invalid_argument("i"); }
    catch (const STD::exception& e) { as_exc = (STD::strcmp(e.what(), "i") == 0); }
    catch (...) {}
    test_true( as_exc );

    bool wrong_branch = false;
    try { throw STD::runtime_error("r"); }
    catch (const STD::logic_error&) { wrong_branch = true; }
    catch (const STD::runtime_error&) {  }
    catch (...) {}
    test_true( !wrong_branch );
    test_pass("cxx03:catch exception hierarchy by base");
}
#else   // !TEST_HAS_EH
TEST_CASE(stdexcept, catch_by_base) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx03:catch exception hierarchy by base");
}
#endif  // TEST_HAS_EH

TEST_CASE(stdexcept, copy_and_string_ctor) {

    STD::runtime_error a("copyme");
    STD::runtime_error b(a);
    test_eq( STD::strcmp(b.what(), "copyme"), 0 );
    test_pass("cxx03:exception copy ctor");

#if !defined(__WATCOMC__)
    STD::string msg = "from string";
    STD::logic_error   ls(msg);
    STD::runtime_error rs(msg);
    test_eq( STD::string(ls.what()), STD::string("from string") );
    test_eq( STD::string(rs.what()), STD::string("from string") );
#else
    TEST_SKIP1(); TEST_SKIP1();
#endif
}

TEST_CASE(stdexcept, message_and_cstr_ctors) {

    STD::logic_error le(STD::string("bad logic"));
    test_true( STD::strcmp(le.what(), "bad logic") == 0 );
    test_pass("cxx03:what() returns constructed message");

    STD::logic_error lc("from cstr");
    test_true( STD::strcmp(lc.what(), "from cstr") == 0 );
    test_pass("cxx11:logic_error(const char*) ctor");

    STD::runtime_error rc("run cstr");
    test_true( STD::strcmp(rc.what(), "run cstr") == 0 );
    test_pass("cxx11:runtime_error(const char*) ctor");
}
