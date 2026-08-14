#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2017
#include <any>
#include <string>
#include <typeinfo>

TEST_CASE(any, construct_observe) {
    STD::any a;
    test_true( !a.has_value() );
    test_pass("cxx17:any::has_value");
    test_true( a.type() == typeid(void) );
    test_pass("cxx17:any::type");

    STD::any b = 42;
    test_true( b.has_value() );
    test_true( b.type() == typeid(int) );
    test_eq( STD::any_cast<int>(b), 42 );
    test_pass("cxx17:any");

    STD::any c(b);
    test_eq( STD::any_cast<int>(c), 42 );
    test_pass("cxx17:any (copy ctor)");

    a = b;
    test_eq( STD::any_cast<int>(a), 42 );
    test_pass("cxx17:operator=");
    a = STD::string("hi");
    test_eq( STD::any_cast<STD::string>(a), STD::string("hi") );
    test_pass("cxx17:operator= (value)");

    a.reset();
    test_true( !a.has_value() );
    test_pass("cxx17:any::reset");
}

TEST_CASE(any, cast_forms) {
    STD::any a = 3.5;
    const STD::any& cr = a;

    test_eq( STD::any_cast<double>(a), 3.5 );
    test_eq( STD::any_cast<double>(cr), 3.5 );
    test_pass("cxx17:any_cast");
    test_throw( (void)STD::any_cast<int>(a) );
    test_pass("cxx17:bad_any_cast");

    test_ptr( STD::any_cast<double>(&a) );
    test_eq( *STD::any_cast<double>(&a), 3.5 );
    test_true( STD::any_cast<int>(&a) == 0 );
    test_pass("cxx17:any_cast (pointer)");
    test_ptr( STD::any_cast<double>(&cr) );
    test_true( STD::any_cast<double>((STD::any*)0) == 0 );
    test_pass("cxx17:any_cast (null pointer)");
}

TEST_CASE(any, emplace_swap) {
    STD::any a;
    STD::string& r = a.emplace<STD::string>(4, 'z');
    test_eq( r, STD::string("zzzz") );
    test_eq( STD::any_cast<STD::string>(a), STD::string("zzzz") );
    test_pass("cxx17:any::emplace");

    STD::any x = 1, y = STD::string("s");
    x.swap(y);
    test_eq( STD::any_cast<STD::string>(x), STD::string("s") );
    test_eq( STD::any_cast<int>(y), 1 );
    test_pass("cxx17:any::swap");
    STD::swap(x, y);
    test_eq( STD::any_cast<int>(x), 1 );
    test_pass("cxx17:swap");
}

TEST_CASE(any, make_any) {
    STD::any a = STD::make_any<int>(42);
    test_eq( STD::any_cast<int>(a), 42 );
    test_pass("cxx17:make_any");
    STD::any s = STD::make_any<STD::string>(3u, 'x');
    test_eq( STD::any_cast<STD::string>(s), STD::string("xxx") );
    test_pass("cxx17:make_any (string args)");
    STD::any d = STD::make_any<double>();
    test_eq( STD::any_cast<double>(d), 0.0 );
    test_pass("cxx17:make_any (value-init)");
}
#endif

#if TEST_TARGET_CXX >= 2017
#include <any>
#include <string>
#include <vector>
#include <utility>
#include <typeinfo>
#include <initializer_list>

using _ccw::string_view;

namespace {

struct Point {
    int x, y, z;
    Point(int a, int b, int c) : x(a), y(b), z(c) {}
};

struct Tracer {
    static int live;
    int tag;
    explicit Tracer(int t = 0) : tag(t) { ++live; }
    Tracer(const Tracer& o) : tag(o.tag) { ++live; }
    ~Tracer() { --live; }
};
int Tracer::live = 0;

#if _tst_cplusplus >= 201103L
struct Bag {
    STD::vector<int> v;
    int extra;
    Bag(STD::initializer_list<int> il, int e) : v(il), extra(e) {}
};
#endif

}

TEST_CASE(any, construct_and_has_value) {
    STD::any a;
    test_true( !a.has_value() );
    test_true( a.type() == typeid(void) );

    STD::any b(42);
    test_true( b.has_value() );
    test_true( b.type() == typeid(int) );
    test_eq( STD::any_cast<int>(b), 42 );
    test_pass("cxx17:any (value ctor)");

    STD::any s(STD::string("hello"));
    test_true( s.type() == typeid(STD::string) );
    test_str_eq( STD::any_cast<STD::string>(s).c_str(), "hello" );
    test_pass("cxx17:any (string payload)");

    STD::any c(b);
    test_true( c.has_value() );
    test_eq( STD::any_cast<int>(c), 42 );
    test_true( b.has_value() );
    test_pass("cxx17:any (copy ctor, source intact)");

    STD::any m(STD::move(s));
    test_true( m.has_value() );
    test_str_eq( STD::any_cast<STD::string>(m).c_str(), "hello" );
    test_pass("cxx17:any (move ctor)");

    STD::any e(STD::in_place_type_t<Point>(), 1, 2, 3);
    test_true( e.type() == typeid(Point) );
    test_eq( STD::any_cast<Point&>(e).y, 2 );
    test_pass("cxx17:in_place_type_t");

#if _tst_cplusplus >= 201103L
    STD::any il(STD::in_place_type_t<Bag>(), {1, 2, 3, 4}, 9);
    test_true( il.type() == typeid(Bag) );
    test_eq( STD::any_cast<Bag&>(il).v.size(), STD::size_t(4) );
    test_eq( STD::any_cast<Bag&>(il).extra, 9 );
    test_pass("cxx17:in_place_type_t (init-list)");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
#endif
}

TEST_CASE(any, assign) {
    STD::any a;
    a = 7;
    test_true( a.type() == typeid(int) );
    test_eq( STD::any_cast<int>(a), 7 );
    test_pass("cxx17:operator= (ValueType)");

    a = STD::string("abc");
    test_true( a.type() == typeid(STD::string) );
    test_pass("cxx17:operator= (rebind type)");

    STD::any b;
    b = a;
    test_str_eq( STD::any_cast<STD::string>(b).c_str(), "abc" );
    test_true( a.has_value() );
    test_pass("cxx17:operator= (const any&)");

    STD::any c;
    c = STD::move(b);
    test_str_eq( STD::any_cast<STD::string>(c).c_str(), "abc" );
    test_pass("cxx17:operator= (any&&)");

    STD::any d(1.5);
    d = STD::any();
    test_true( !d.has_value() );
    test_pass("cxx17:operator= (empty)");
}

TEST_CASE(any, emplace_reset_swap) {
    STD::any a(1);
    int& r = a.emplace<int>(99);
    test_eq( r, 99 );
    test_eq( STD::any_cast<int>(a), 99 );
    test_pass("cxx17:any::emplace (returns ref)");

    Point& p = a.emplace<Point>(4, 5, 6);
    test_eq( p.z, 6 );
    test_true( a.type() == typeid(Point) );
    test_pass("cxx17:any::emplace (replace)");

#if _tst_cplusplus >= 201103L
    Bag& bg = a.emplace<Bag>({7, 8}, 3);
    test_eq( bg.v.size(), STD::size_t(2) );
    test_eq( bg.extra, 3 );
    test_pass("cxx17:any::emplace (init-list)");
#else
    TEST_SKIP1(); TEST_SKIP1();
#endif

    a.reset();
    test_true( !a.has_value() );
    test_true( a.type() == typeid(void) );
    a.reset();
    test_true( !a.has_value() );
    test_pass("cxx17:any::reset (idempotent)");

    STD::any x(1), y(STD::string("s"));
    x.swap(y);
    test_true( x.type() == typeid(STD::string) );
    test_true( y.type() == typeid(int) );
    test_pass("cxx17:any::swap (member 2)");

    STD::swap(x, y);
    test_true( x.type() == typeid(int) );
    test_true( y.type() == typeid(STD::string) );
    test_pass("cxx17:swap (free 2)");
}

TEST_CASE(any, destructor_runs) {
    test_eq( Tracer::live, 0 );
    {
        STD::any a(Tracer(1));
        test_eq( Tracer::live, 1 );
        a.reset();
        test_eq( Tracer::live, 0 );

        a = Tracer(2);
        test_eq( Tracer::live, 1 );
    }
    test_eq( Tracer::live, 0 );
    test_pass("cxx17:~any");
}

TEST_CASE(any, make_any_chk) {
    STD::any a = STD::make_any<int>(5);
    test_eq( STD::any_cast<int>(a), 5 );
    test_pass("cxx17:make_any (int)");

    STD::any s = STD::make_any<STD::string>(3u, 'z');
    test_str_eq( STD::any_cast<STD::string>(s).c_str(), "zzz" );
    test_pass("cxx17:make_any (string)");

    STD::any p = STD::make_any<Point>(1, 2, 3);
    test_eq( STD::any_cast<Point&>(p).x, 1 );
    test_pass("cxx17:make_any (Point args)");

#if _tst_cplusplus >= 201103L
    STD::any b = STD::make_any<Bag>({10, 20, 30}, 4);
    test_eq( STD::any_cast<Bag&>(b).v.size(), STD::size_t(3) );
    test_eq( STD::any_cast<Bag&>(b).v[2], 30 );
    test_eq( STD::any_cast<Bag&>(b).extra, 4 );
    test_pass("cxx17:make_any (init-list)");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
#endif
}

TEST_CASE(any, any_cast_value_forms) {
    STD::any a(123);
    const STD::any& ca = a;

    test_eq( STD::any_cast<int>(ca), 123 );
    test_eq( STD::any_cast<int>(a), 123 );
    test_pass("cxx17:any_cast (const any&)");

    STD::any& ra = a;
    int& ref = STD::any_cast<int&>(ra);
    ref = 456;
    test_eq( STD::any_cast<int>(a), 456 );
    test_pass("cxx17:any_cast (reference)");

    STD::any s(STD::string("move-me"));
#if _tst_cplusplus >= 201103L
    STD::string moved = STD::any_cast<STD::string&&>(STD::move(s));
#else
    STD::string moved = STD::any_cast<STD::string&>(s);
#endif
    test_str_eq( moved.c_str(), "move-me" );
    test_true( s.has_value() );
    test_pass("cxx17:any_cast (rvalue)");
}

TEST_CASE(any, any_cast_pointer_forms) {
    STD::any a(7);
    STD::any* pa = &a;
    const STD::any* pca = &a;

    int* p = STD::any_cast<int>(pa);
    test_ptr( p );
    test_eq( *p, 7 );
    *p = 8;
    test_eq( STD::any_cast<int>(a), 8 );
    test_pass("cxx17:any_cast (any*)");

    const int* cp = STD::any_cast<int>(pca);
    test_ptr( cp );
    test_eq( *cp, 8 );
    test_pass("cxx17:any_cast (const any*)");

    test_true( STD::any_cast<double>(pa) == nullptr );
    test_true( STD::any_cast<double>(pca) == nullptr );
    test_pass("cxx17:any_cast (wrong type null)");

    STD::any* nul = nullptr;
    test_true( STD::any_cast<int>(nul) == nullptr );

    STD::any empty;
    test_true( STD::any_cast<int>(&empty) == nullptr );
    test_pass("cxx17:any_cast (null/empty)");
}

#if TEST_HAS_EH
TEST_CASE(any, bad_any_cast) {
    STD::any a(1);
    test_throw( (void)STD::any_cast<double>(a) );
    test_no_throw( (void)STD::any_cast<int>(a) );
    test_pass("cxx17:bad_any_cast (value form throws)");

    STD::any empty;
    test_throw( (void)STD::any_cast<int>(empty) );

    bool caught = false;
    try {
        (void)STD::any_cast<STD::string>(a);
    } catch (const STD::bad_any_cast& e) {
        caught = true;
        test_ptr( e.what() );
        const STD::exception& base = e;
        test_ptr( base.what() );
    }
    test_true( caught );
    test_pass("cxx17:bad_any_cast::what");

    bool caught_base = false;
    try {
        (void)STD::any_cast<char>(a);
    } catch (const STD::bad_cast&) {
        caught_base = true;
    }
    test_true( caught_base );
    test_pass("cxx17:bad_cast (base catch)");
}
#else   // !TEST_HAS_EH
TEST_CASE(any, bad_any_cast) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx17:bad_any_cast (value form throws)");
    test_skip("cxx17:bad_any_cast::what");
    test_skip("cxx17:bad_cast (base catch)");
}
#endif  // TEST_HAS_EH
#else
TEST_CASE_SKIP(any, construct_and_has_value)
TEST_CASE_SKIP(any, assign)
TEST_CASE_SKIP(any, emplace_reset_swap)
TEST_CASE_SKIP(any, destructor_runs)
TEST_CASE_SKIP(any, make_any_chk)
TEST_CASE_SKIP(any, any_cast_value_forms)
TEST_CASE_SKIP(any, any_cast_pointer_forms)
TEST_CASE_SKIP(any, bad_any_cast)
#endif


#if TEST_TARGET_CXX >= 2017
TEST_CASE(any, empty_state_and_in_place_cxx17) {
    STD::any a;
    test_true( !a.has_value() );
    test_pass("cxx17:any (default ctor, empty)");
    test_pass("cxx17:any::has_value (empty)");

    test_true( a.type() == typeid(void) );
    a = 5;
    test_true( a.type() == typeid(int) );
    a.reset();
    test_true( a.type() == typeid(void) );
    test_pass("cxx17:any::type (empty)");

#if _TST_HAS_VAR_TEMPLATE
    STD::any s(STD::in_place_type<STD::string>, 3u, 'x');
    test_true( s.has_value() );
    test_true( STD::any_cast<STD::string>(s) == "xxx" );
    test_pass("cxx17:any (in_place_type, args)");

    STD::any v(STD::in_place_type<STD::vector<int> >,
               STD::initializer_list<int>(), STD::allocator<int>());
    test_true( STD::any_cast<STD::vector<int> >(v).empty() );
    {
        int raw[3] = {1, 2, 3};
        STD::any w(STD::in_place_type<STD::vector<int> >, raw, raw + 3);
        test_eq( (int)STD::any_cast<STD::vector<int> >(w).size(), 3 );
    }
    test_pass("cxx17:any (in_place_type, init-list)");
#else
    TEST_NOTE("in_place_type<T> is a variable template; C++03 has none");
    test_skip("cxx17:any (in_place_type, args)");
    test_skip("cxx17:any (in_place_type, init-list)");
#endif
}
#else
TEST_CASE(any, empty_state_and_in_place_cxx17) {
    test_skip("cxx17:any (default ctor, empty)");
    test_skip("cxx17:any::has_value (empty)");
    test_skip("cxx17:any::type (empty)");
    test_skip("cxx17:any (in_place_type, args)");
    test_skip("cxx17:any (in_place_type, init-list)");
}
#endif
