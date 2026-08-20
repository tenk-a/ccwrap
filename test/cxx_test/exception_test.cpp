#include "test_cxx.hpp"

#include <exception>
#include <string>

#if TEST_HAS_EH
TEST_CASE(exception, hierarchy) {

    STD::exception e;
    test_true( e.what() != 0 );
    test_pass("cxx03:exception::what");
    STD::bad_alloc ba;
    test_true( ba.what() != 0 );

    bool caught = false;
    try { throw STD::bad_exception(); }
    catch (STD::exception& ex) { caught = true; test_true( ex.what() != 0 ); }
    test_true( caught );
    test_pass("cxx03:bad_exception");
}
#else   // !TEST_HAS_EH
TEST_CASE(exception, hierarchy) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx03:exception::what");
    test_skip("cxx03:bad_exception");
}
#endif  // TEST_HAS_EH

static void ccw_th() {}
#define _TST_PRE17  (!(_tst_cplusplus >= 201703L))
#if _TST_PRE17
static void ccw_uh() {}
#endif
TEST_CASE(exception, handlers) {

    STD::terminate_handler tp = STD::set_terminate(ccw_th);
    test_true( STD::get_terminate() == ccw_th );
    test_pass("cxx03:set_terminate");
    STD::set_terminate(tp);
    test_true( STD::get_terminate() == tp );
    test_pass("cxx03:get_terminate");

#if _TST_PRE17
    STD::unexpected_handler up = STD::set_unexpected(ccw_uh);
    test_true( STD::get_unexpected() == ccw_uh );
    test_pass("cxx03:set_unexpected");
#else
    TEST_SKIP1();
    test_skip("cxx03:set_unexpected");
#endif
#if _TST_PRE17
    STD::set_unexpected(up);
    test_true( STD::get_unexpected() == up );
    test_pass("cxx03:get_unexpected");
#else
    TEST_SKIP1();
    test_skip("cxx03:get_unexpected");
#endif
}

#if TEST_TARGET_CXX >= 2011
#include <exception>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <type_traits>
#include <utility>

using _ccw::string_view;

namespace {

struct MyError : STD::exception {
    int code;
    explicit MyError(int c) : code(c) {}
    const char* what() const noexcept override { return "MyError"; }
};

int g_handler_calls = 0;
void my_terminate_handler() { ++g_handler_calls; }

}

TEST_CASE(exception, exception_class) {

    STD::exception e;
    test_ptr( e.what() );

    STD::exception c(e);
    test_ptr( c.what() );

    STD::exception a;
    a = e;
    test_ptr( a.what() );

    test_true( STD::is_copy_constructible<STD::exception>::value );
    test_true( STD::is_copy_assignable<STD::exception>::value );
#if !defined(__WATCOMC__)
    test_true( STD::has_virtual_destructor<STD::exception>::value );
    test_pass("cxx03:exception (copy/assign)");
#else
    TEST_SKIP1();
    test_skip("cxx03:exception (copy/assign)");
#endif

    MyError me(7);
    const STD::exception& ref = me;
    test_str_eq( ref.what(), "MyError" );
    test_pass("cxx03:exception::what (virtual dispatch)");

    test_true( (STD::is_base_of<STD::exception, STD::runtime_error>::value) );
    test_true( (STD::is_base_of<STD::exception, STD::bad_exception>::value) );
    test_pass("cxx03:exception (base class)");
}

#if TEST_HAS_EH
TEST_CASE(exception, bad_exception_class) {
    STD::bad_exception b;
    test_ptr( b.what() );

    STD::bad_exception c(b);
    test_ptr( c.what() );

    STD::bad_exception a;
    a = b;
    test_str_eq( a.what(), b.what() );
    test_pass("cxx03:bad_exception::what");

    test_true( STD::is_destructible<STD::bad_exception>::value );

    bool caught = false;
    try {
        throw STD::bad_exception();
    } catch (const STD::exception& e) {
        caught = true;
        test_ptr( e.what() );
    }
    test_true( caught );
    test_pass("cxx03:bad_exception (catchable as exception)");
}
#else   // !TEST_HAS_EH
TEST_CASE(exception, bad_exception_class) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx03:bad_exception::what");
    test_skip("cxx03:bad_exception (catchable as exception)");
}
#endif  // TEST_HAS_EH

#if defined(__WATCOMC__)
TEST_CASE_SKIP(exception, uncaught_exceptions)
#else
#if TEST_HAS_EH
TEST_CASE(exception, uncaught_exceptions) {

#if TEST_TARGET_CXX >= 2017
    test_eq( STD::uncaught_exceptions(), 0 );

    int during = -1;
    try {
        struct Runner {
            int* out;
            ~Runner() { *out = STD::uncaught_exceptions(); }
        } r = { &during };
        throw STD::runtime_error("boom");
    } catch (const STD::runtime_error&) {
    }
    test_ge( during, 1 );
    test_eq( STD::uncaught_exceptions(), 0 );
    test_pass("cxx17:uncaught_exceptions");
#else
    TEST_NOTE("uncaught_exceptions is C++17");
    test_skip("cxx17:uncaught_exceptions");
#endif

}
#else   // !TEST_HAS_EH
TEST_CASE(exception, uncaught_exceptions) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx17:uncaught_exceptions");
}
#endif  // TEST_HAS_EH

#endif

#if _TST_HAS_EXCEPTION_PTR
#if TEST_HAS_EH
TEST_CASE(exception, exception_ptr_basics) {

    STD::exception_ptr p;
#if !defined(__WATCOMC__)
    STD::exception_ptr q(nullptr);
#else

    STD::exception_ptr q;
#endif
    test_true( !p );
    test_true( !q );
    test_true( p == q );
    test_true( p == nullptr );
    test_pass("cxx11:exception_ptr");

    STD::exception_ptr e;
    try {
        throw MyError(42);
    } catch (...) {
        e = STD::current_exception();
    }
    test_true( static_cast<bool>(e) );
    test_true( e != p );
    test_pass("cxx11:current_exception");

    STD::exception_ptr copy(e);
    test_true( copy == e );
    STD::exception_ptr assigned;
    assigned = e;
    test_true( assigned == e );
    test_pass("cxx11:exception_ptr (copy)");

    STD::exception_ptr src(e);
    STD::exception_ptr moved(STD::move(src));
    test_true( moved == e );

    STD::exception_ptr src2(e);
    STD::exception_ptr moveass;
    moveass = STD::move(src2);
    test_true( moveass == e );

#if !defined(__WATCOMC__)
    moveass = nullptr;
#else
    moveass = STD::exception_ptr();
#endif
    test_true( !moveass );
    test_pass("cxx11:exception_ptr (move)");

    {
        STD::exception_ptr tmp(e);
        test_true( static_cast<bool>(tmp) );
    }
    test_true( static_cast<bool>(e) );
    test_pass("cxx11:exception_ptr (destructor)");
}
#else   // !TEST_HAS_EH
TEST_CASE(exception, exception_ptr_basics) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx11:exception_ptr");
    test_skip("cxx11:current_exception");
    test_skip("cxx11:exception_ptr (copy)");
    test_skip("cxx11:exception_ptr (move)");
    test_skip("cxx11:exception_ptr (destructor)");
}
#endif  // TEST_HAS_EH

#if TEST_HAS_EH
TEST_CASE(exception, exception_ptr_swap) {
    STD::exception_ptr a = STD::make_exception_ptr(MyError(1));
    STD::exception_ptr b = STD::make_exception_ptr(MyError(2));
    STD::exception_ptr a0 = a, b0 = b;

    STD::swap(a, b);
    test_true( a == b0 );
    test_true( b == a0 );

#if !defined(__WATCOMC__)
    swap(a, b);
#else
    STD::swap(a, b);
#endif
    test_true( a == a0 );
    test_true( b == b0 );

    STD::exception_ptr n;
    STD::swap(a, n);
    test_true( !a );
    test_true( n == a0 );
    test_pass("cxx11:swap(exception_ptr)");
}
#else   // !TEST_HAS_EH
TEST_CASE(exception, exception_ptr_swap) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx11:swap(exception_ptr)");
}
#endif  // TEST_HAS_EH

#if TEST_HAS_EH
TEST_CASE(exception, rethrow_and_make_exception_ptr) {

    STD::exception_ptr p = STD::make_exception_ptr(MyError(99));
    test_true( static_cast<bool>(p) );
    test_pass("cxx11:make_exception_ptr");

    int code = 0;
    try {
        STD::rethrow_exception(p);
        test_fail();
    } catch (const MyError& e) {
        code = e.code;
    }
    test_eq( code, 99 );

    test_throw( STD::rethrow_exception(p) );
    test_pass("cxx11:rethrow_exception");

    STD::exception_ptr p2;
    try {
        STD::rethrow_exception(p);
    } catch (...) {
        p2 = STD::current_exception();
    }
    test_true( static_cast<bool>(p2) );
#if !defined(__WATCOMC__)
    int code2 = 0;
    try { STD::rethrow_exception(p2); } catch (const MyError& e) { code2 = e.code; }
    test_eq( code2, 99 );
#else
    bool as_base = false;
    try { STD::rethrow_exception(p2); } catch (const STD::exception&) { as_base = true; }
    test_true( as_base );
#endif
    test_pass("cxx11:current_exception (round trip)");

    STD::exception_ptr pi = STD::make_exception_ptr(int(5));
    int got = 0;
    try { STD::rethrow_exception(pi); } catch (int v) { got = v; }
    test_eq( got, 5 );
    test_pass("cxx11:exception_ptr (non-std payload)");

#if !defined(__WATCOMC__)
    test_true( !STD::current_exception() );
    test_pass("cxx11:current_exception (outside handler)");
#else

    TEST_SKIP1();
    test_skip("cxx11:current_exception (outside handler)");
#endif
}
#else   // !TEST_HAS_EH
TEST_CASE(exception, rethrow_and_make_exception_ptr) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx11:make_exception_ptr");
    test_skip("cxx11:rethrow_exception");
    test_skip("cxx11:current_exception (round trip)");
    test_skip("cxx11:exception_ptr (non-std payload)");
    test_skip("cxx11:current_exception (outside handler)");
}
#endif  // TEST_HAS_EH
#else
TEST_CASE_SKIP(exception, exception_ptr_basics)
TEST_CASE_SKIP(exception, exception_ptr_swap)
TEST_CASE_SKIP(exception, rethrow_and_make_exception_ptr)
#endif

#if defined(__WATCOMC__) || !_TST_HAS_EXCEPTION_PTR
TEST_CASE_SKIP(exception, nested_exception_class)
#else
#if TEST_HAS_EH
TEST_CASE(exception, nested_exception_class) {

    STD::nested_exception ne;
    test_true( !ne.nested_ptr() );

    STD::nested_exception copy(ne);
    test_true( copy.nested_ptr() == ne.nested_ptr() );

    STD::nested_exception assigned;
    assigned = ne;
    test_true( assigned.nested_ptr() == ne.nested_ptr() );

#if !defined(__WATCOMC__)
    test_true( STD::has_virtual_destructor<STD::nested_exception>::value );
    test_pass("cxx11:nested_exception");
#else
    TEST_SKIP1();
    test_skip("cxx11:nested_exception");
#endif

    try {
        throw STD::runtime_error("inner");
    } catch (...) {
        STD::nested_exception n;
        test_true( static_cast<bool>(n.nested_ptr()) );

        STD::string msg;
        try {
            n.rethrow_nested();
            test_fail();
        } catch (const STD::runtime_error& e) {
            msg = e.what();
        }
        test_str_eq( msg, "inner" );
    }
    test_pass("cxx11:nested_exception::rethrow_nested");
}
#else   // !TEST_HAS_EH
TEST_CASE(exception, nested_exception_class) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx11:nested_exception");
    test_skip("cxx11:nested_exception::rethrow_nested");
}
#endif  // TEST_HAS_EH

#endif

#if defined(__WATCOMC__) || !_TST_HAS_EXCEPTION_PTR
TEST_CASE_SKIP(exception, throw_with_nested_and_rethrow_if_nested)
#else
#if TEST_HAS_EH
TEST_CASE(exception, throw_with_nested_and_rethrow_if_nested) {

    STD::string outer, inner;
    try {
        try {
            throw STD::runtime_error("inner");
        } catch (...) {
            STD::throw_with_nested( STD::logic_error("outer") );
        }
        test_fail();
    } catch (const STD::logic_error& e) {
        outer = e.what();

        try {
            STD::rethrow_if_nested(e);
            test_fail();
        } catch (const STD::runtime_error& ie) {
            inner = ie.what();
        }
    }
    test_str_eq( outer, "outer" );
    test_str_eq( inner, "inner" );
    test_pass("cxx11:throw_with_nested");

    STD::runtime_error plain("plain");
    test_no_throw( STD::rethrow_if_nested(plain) );
    test_pass("cxx11:rethrow_if_nested");

    struct Wrapped : STD::runtime_error, STD::nested_exception {
        Wrapped() : STD::runtime_error("w") {}
    };
    Wrapped w;
    test_true( !w.nested_ptr() );

    bool got_int = false;
    try {
        STD::throw_with_nested( 3 );
    } catch (int v) {
        got_int = (v == 3);
    }
    test_true( got_int );

    int depth = 0;
    try {
        try {
            try {
                throw MyError(1);
            } catch (...) {
                STD::throw_with_nested( STD::runtime_error("mid") );
            }
        } catch (...) {
            STD::throw_with_nested( STD::logic_error("top") );
        }
    } catch (const STD::logic_error& top) {
        ++depth;
        try {
            STD::rethrow_if_nested(top);
        } catch (const STD::runtime_error& mid) {
            ++depth;
            try {
                STD::rethrow_if_nested(mid);
            } catch (const MyError& leaf) {
                depth += leaf.code;
            }
        }
    }
    test_eq( depth, 3 );
    test_pass("cxx11:throw_with_nested (nested levels)");
}
#else   // !TEST_HAS_EH
TEST_CASE(exception, throw_with_nested_and_rethrow_if_nested) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx11:throw_with_nested");
    test_skip("cxx11:rethrow_if_nested");
    test_skip("cxx11:throw_with_nested (nested levels)");
}
#endif  // TEST_HAS_EH

#endif

TEST_CASE(exception, terminate_handler) {

    test_true( (STD::is_same<STD::terminate_handler, void(*)()>::value) );
    test_pass("cxx03:terminate_handler (type)");

    STD::terminate_handler saved = STD::get_terminate();

    STD::terminate_handler prev = STD::set_terminate(&my_terminate_handler);
    test_true( prev == saved );
    test_true( STD::get_terminate() == &my_terminate_handler );

    int before = g_handler_calls;
    STD::get_terminate()();
    test_eq( g_handler_calls, before + 1 );
    test_pass("cxx03:set_terminate (installs handler)");

    void (*t)() = &STD::terminate;
    test_true( t != 0 );
    test_pass("cxx03:terminate (function)");

    STD::terminate_handler back = STD::set_terminate(saved);
    test_true( back == &my_terminate_handler );
    test_true( STD::get_terminate() == saved );
    test_pass("cxx03:set_terminate (returns previous)");
}
#else
namespace {
struct WatErr : STD::exception {
    int code;
    WatErr(int c) : code(c) {}
    virtual const char* what() const { return "WatErr"; }
};
}

#if _TST_HAS_EXCEPTION_PTR
#if TEST_HAS_EH
TEST_CASE(exception, exception_ptr_subset) {
    STD::exception_ptr p;
    test_true( !p );
    STD::exception_ptr q;
    test_true( p == q );
    test_pass("cxx11:exception_ptr");

    STD::exception_ptr e = STD::make_exception_ptr(WatErr(42));
    test_true( !!e );
    test_true( e != p );
    test_pass("cxx11:make_exception_ptr");

    STD::exception_ptr c(e);
    test_true( c == e );
    STD::exception_ptr a;
    a = e;
    test_true( a == e );

    { STD::exception_ptr tmp(e); test_true( !!tmp ); }
    test_true( !!e );
    test_pass("cxx11:exception_ptr (copy)");

    STD::exception_ptr x = STD::make_exception_ptr(WatErr(1));
    STD::exception_ptr y = STD::make_exception_ptr(WatErr(2));
    STD::exception_ptr x0 = x, y0 = y;
    STD::swap(x, y);
    test_true( x == y0 );
    test_true( y == x0 );
    x.swap(y);
    test_true( x == x0 );
    test_true( y == y0 );
    test_pass("cxx11:swap(exception_ptr)");
}
#else   // !TEST_HAS_EH
TEST_CASE(exception, exception_ptr_subset) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx11:exception_ptr");
    test_skip("cxx11:make_exception_ptr");
    test_skip("cxx11:exception_ptr (copy)");
    test_skip("cxx11:swap(exception_ptr)");
}
#endif  // TEST_HAS_EH

#if TEST_HAS_EH
TEST_CASE(exception, make_rethrow_exact) {

    STD::exception_ptr p = STD::make_exception_ptr(WatErr(99));
    int code = 0;
    try { STD::rethrow_exception(p); test_fail(); }
    catch (const WatErr& ex) { code = ex.code; }
    test_eq( code, 99 );
    test_pass("cxx11:rethrow_exception");

    STD::exception_ptr pi = STD::make_exception_ptr(int(5));
    int got = 0;
    try { STD::rethrow_exception(pi); } catch (int v) { got = v; }
    test_eq( got, 5 );
    test_pass("cxx11:exception_ptr (non-std payload)");

    test_throw( STD::rethrow_exception(p) );
    test_pass("cxx11:rethrow_exception");
}
#else   // !TEST_HAS_EH
TEST_CASE(exception, make_rethrow_exact) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx11:rethrow_exception");
    test_skip("cxx11:exception_ptr (non-std payload)");
    test_skip("cxx11:rethrow_exception");
}
#endif  // TEST_HAS_EH

#if TEST_HAS_EH
TEST_CASE(exception, current_exception_subset) {

    STD::exception_ptr e;
    try { throw WatErr(7); }
    catch (...) { e = STD::current_exception(); }
    test_true( !!e );

    const char* msg = 0;
    try { STD::rethrow_exception(e); }
    catch (const STD::exception& ex) { msg = ex.what(); }
    test_true( msg != 0 && msg[0] == 'W' );
    test_pass("cxx11:current_exception");

    STD::exception_ptr eb;
    try { throw STD::bad_alloc(); }
    catch (...) { eb = STD::current_exception(); }
    bool got_bad_alloc = false;
    try { STD::rethrow_exception(eb); }
    catch (const STD::bad_alloc&) { got_bad_alloc = true; }
    catch (...) {}
    test_true( got_bad_alloc );
    test_pass("cxx11:current_exception (round trip)");
}
#else   // !TEST_HAS_EH
TEST_CASE(exception, current_exception_subset) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx11:current_exception");
    test_skip("cxx11:current_exception (round trip)");
}
#endif  // TEST_HAS_EH

#if TEST_HAS_EH
TEST_CASE(exception, nested_subset) {

    STD::exception_ptr inner;
    try {
        try { throw WatErr(3); }
        catch (...) { STD::nested_exception ne; inner = ne.nested_ptr(); }
    } catch (...) {}
    test_true( !!inner );
    test_pass("cxx11:nested_exception");

    bool got = false;
    try {
        try { throw WatErr(8); }
        catch (...) { STD::nested_exception ne; ne.rethrow_nested(); }
    } catch (const STD::exception& ex) { got = (ex.what()[0] == 'W'); }
    test_true( got );
    test_pass("cxx11:nested_exception::rethrow_nested");
}
#else   // !TEST_HAS_EH
TEST_CASE(exception, nested_subset) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx11:nested_exception");
    test_skip("cxx11:nested_exception::rethrow_nested");
}
#endif  // TEST_HAS_EH
#else
TEST_CASE_SKIP(exception, exception_ptr_subset)
TEST_CASE_SKIP(exception, make_rethrow_exact)
TEST_CASE_SKIP(exception, current_exception_subset)
TEST_CASE_SKIP(exception, nested_subset)
#endif

TEST_CASE_SKIP(exception, exception_class)
TEST_CASE_SKIP(exception, bad_exception_class)
TEST_CASE_SKIP(exception, uncaught_exceptions)

TEST_CASE_SKIP(exception, throw_with_nested_and_rethrow_if_nested)
TEST_CASE_SKIP(exception, terminate_handler)
#endif

#if _TST_HAS_EXCEPTION_PTR
#if TEST_HAS_EH
TEST_CASE(exception, gap_fill) {

    STD::exception_ptr n;
    test_true( !n );
    test_true( !static_cast<bool>(n) );
    STD::exception_ptr v = STD::make_exception_ptr(MyError(1));
    test_true( static_cast<bool>(v) );
    test_pass("cxx11:exception_ptr null/operator bool");

    STD::exception_ptr v2 = v;
    test_true( v == v2 );
    test_true( v != n );
    test_true( !(v == n) );
    test_pass("cxx11:exception_ptr comparison");

    STD::exception_ptr inner;
    try {
        throw STD::runtime_error("np");
    } catch (...) {
        STD::nested_exception ne;
        inner = ne.nested_ptr();
    }
    test_true( static_cast<bool>(inner) );
    test_pass("cxx11:nested_exception::nested_ptr");

#if _TST_PRE17
    STD::unexpected_handler oh = STD::set_unexpected(&STD::terminate);
    test_true( oh != 0 || oh == 0 );
    test_pass("cxx03:unexpected_handler (type)");
    STD::set_unexpected(oh);
    test_true( true );
    test_pass("cxx03:unexpected (function)");

#if !defined(__WATCOMC__)
    test_true( STD::uncaught_exception() == false );
    test_pass("cxx03:uncaught_exception (deprecated)");
#else
    test_skip("cxx03:uncaught_exception (deprecated)");
#endif
#else
    test_skip("cxx03:unexpected_handler (type)");
    test_skip("cxx03:unexpected (function)");
    test_skip("cxx03:uncaught_exception (deprecated)");
#endif
}
#else   // !TEST_HAS_EH
TEST_CASE(exception, gap_fill) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx11:exception_ptr null/operator bool");
    test_skip("cxx11:exception_ptr comparison");
    test_skip("cxx11:nested_exception::nested_ptr");
    test_skip("cxx03:unexpected_handler (type)");
    test_skip("cxx03:unexpected (function)");
    test_skip("cxx03:uncaught_exception (deprecated)");
}
#endif  // TEST_HAS_EH

#if TEST_HAS_EH
TEST_CASE(exception, exception_ptr_cast_cxx26) {
#if defined(__cpp_lib_exception_ptr_cast)
    STD::exception_ptr p;
    try { throw 42; } catch (...) { p = STD::current_exception(); }
    const int* hit = STD::exception_ptr_cast<int>(p);
    test_ptr( (void*)hit );
    test_eq( *hit, 42 );
    test_true( STD::exception_ptr_cast<double>(p) == 0 );

    STD::exception_ptr none;
    test_true( STD::exception_ptr_cast<int>(none) == 0 );
    test_pass("cxx26:exception_ptr_cast");
#else
    TEST_NOTE("exception_ptr_cast is C++26 (P2927); no library here defines it yet");
    test_skip("cxx26:exception_ptr_cast");
#endif
}
#else   // !TEST_HAS_EH
TEST_CASE(exception, exception_ptr_cast_cxx26) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx26:exception_ptr_cast");
}
#endif  // TEST_HAS_EH
#else
TEST_CASE_SKIP(exception, gap_fill)
TEST_CASE(exception, exception_ptr_cast_cxx26) {
    test_skip("cxx26:exception_ptr_cast");
}
#endif

TEST_CASE(exception, feature_test_macros) {
#if defined(__cpp_lib_uncaught_exceptions)
    test_true( __cpp_lib_uncaught_exceptions > 0L );
    test_pass("cxx17:__cpp_lib_uncaught_exceptions");
#else
    test_skip("cxx17:__cpp_lib_uncaught_exceptions");
#endif
#if defined(__cpp_lib_constexpr_exceptions)
    test_true( __cpp_lib_constexpr_exceptions > 0L );
    test_pass("cxx26:__cpp_lib_constexpr_exceptions");
#else
    test_skip("cxx26:__cpp_lib_constexpr_exceptions");
#endif
#if defined(__cpp_lib_exception_ptr_cast)
    test_true( __cpp_lib_exception_ptr_cast > 0L );
    test_pass("cxx26:__cpp_lib_exception_ptr_cast");
#else
    test_skip("cxx26:__cpp_lib_exception_ptr_cast");
#endif
}


TEST_CASE(exception, constexpr_facilities_cxx26) {
#if defined(__cpp_lib_constexpr_exceptions)
    {
        STD::exception e;
        test_true( e.what() != 0 );
    }
    test_pass("cxx26:constexpr exception facilities");
#else
    TEST_NOTE("constexpr exceptions are C++26 (P3068); not in this library");
    test_skip("cxx26:constexpr exception facilities");
#endif
}
