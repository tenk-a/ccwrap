#include "test_cxx.hpp"

#include <functional>
#include <string>
#include <utility>
#include <type_traits>
#include <cstring>
#include <compare>

namespace {
    struct FnBox {
        int v;
        int add(int x) const { return v + x; }
        int mem;
    };
    long fn_double(int a) { return (long)a * 2; }
}

TEST_CASE(functional, arithmetic_objects) {
    test_eq( STD::plus<int>()(2, 3), 5 );
    test_pass("cxx03:plus");
    test_eq( STD::minus<int>()(5, 2), 3 );
    test_pass("cxx03:minus");
    test_eq( STD::multiplies<int>()(4, 3), 12 );
    test_pass("cxx03:multiplies");
    test_eq( STD::divides<int>()(12, 4), 3 );
    test_pass("cxx03:divides");
    test_eq( STD::modulus<int>()(7, 3), 1 );
    test_pass("cxx03:modulus");
    test_eq( STD::negate<int>()(5), -5 );
    test_pass("cxx03:negate");
}

TEST_CASE(functional, compare_logical_objects) {
    test_true(  STD::less<int>()(1, 2) );
    test_true( !STD::less<int>()(2, 1) );
    test_pass("cxx03:less");
    test_true(  STD::greater<int>()(2, 1) );
    test_pass("cxx03:greater");
    test_true(  STD::less_equal<int>()(1, 1) );
    test_pass("cxx03:less_equal");
    test_true(  STD::greater_equal<int>()(1, 1) );
    test_pass("cxx03:greater_equal");
    test_true(  STD::equal_to<int>()(7, 7) );
    test_pass("cxx03:equal_to");
    test_true(  STD::not_equal_to<int>()(7, 8) );
    test_pass("cxx03:not_equal_to");

    test_true(  STD::logical_and<bool>()(true, true) );
    test_pass("cxx03:logical_and");
    test_true(  STD::logical_or<bool>()(false, true) );
    test_pass("cxx03:logical_or");
    test_true(  STD::logical_not<bool>()(false) );
    test_pass("cxx03:logical_not");

    test_eq( STD::bit_and<int>()(0x0F, 0x33), 0x03 );
    test_pass("cxx11:bit_and");
    test_eq( STD::bit_or<int>()(0x0F, 0x30), 0x3F );
    test_pass("cxx11:bit_or");
    test_eq( STD::bit_xor<int>()(0x0F, 0x33), 0x3C );
    test_pass("cxx11:bit_xor");
#if TEST_TARGET_CXX >= 2014
    test_eq( STD::bit_not<int>()(0), ~0 );
    test_eq( (int)(unsigned char)STD::bit_not<unsigned char>()(0x0F), 0xF0 );
    test_pass("cxx14:bit_not");
#else
    TEST_NOTE("bit_not is C++14");
    test_skip("cxx14:bit_not");
#endif
}

TEST_CASE(functional, reference_wrapper) {
    int x = 5;
    STD::reference_wrapper<int> r = STD::ref(x);
    test_eq( r.get(), 5 );
    test_pass("cxx11:ref");
    r.get() = 9;
    test_eq( x, 9 );
    test_pass("cxx11:reference_wrapper::get");
    int& ir = r;
    test_eq( ir, 9 );
    test_pass("cxx11:reference_wrapper::operator T&");

    const int y = 7;
    STD::reference_wrapper<const int> cr = STD::cref(y);
    test_eq( cr.get(), 7 );
    test_pass("cxx11:cref");
}

namespace {
struct RwFunctor {
    typedef int result_type;
    int base;
    int operator()() const { return base; }
    int operator()(int a) const { return base + a; }
    int operator()(int a, int b) const { return base + a + b; }
};
int rw_freefn(int a, int b) { return a * b; }
}

TEST_CASE(functional, reference_wrapper_invoke) {

    RwFunctor f; f.base = 10;
    STD::reference_wrapper<RwFunctor> rf = STD::ref(f);
    test_eq( rf(), 10 );
    test_eq( rf(5), 15 );
    test_eq( rf(3, 4), 17 );
    test_pass("cxx11:reference_wrapper::operator()");

    f.base = 100;
    test_eq( rf(), 100 );
    test_pass("cxx11:reference_wrapper(reference semantics)");

    int (*fp)(int, int) = &rw_freefn;
    STD::reference_wrapper<int (*)(int, int)> rp = STD::ref(fp);
    test_eq( rp(6, 7), 42 );
    test_pass("cxx11:reference_wrapper(wraps function pointer)");
}

#if TEST_TARGET_CXX >= 2017
struct Pos { bool operator()(int v) const { return v > 0; } };
TEST_CASE(functional, not_fn) {
    test_true(  STD::not_fn(Pos())(-3) );
    test_true( !STD::not_fn(Pos())(3) );
    test_pass("cxx17:not_fn");
}
#endif

#if TEST_TARGET_CXX >= 2017 && (!defined(_MSC_VER) || _MSC_VER >= 1600)
static int add3(int a, int b, int c) { return a + b + c; }
struct Obj { int v; int getv() const { return v; } };
TEST_CASE(functional, invoke) {
    test_eq( STD::invoke(add3, 1, 2, 3), 6 );
    test_pass("cxx17:invoke");
    Obj o; o.v = 42;
    test_eq( STD::invoke(&Obj::getv, o), 42 );
    test_pass("cxx17:invoke(member pointer)");
}
#endif

#if TEST_TARGET_CXX >= 2011
static int fn_add(int a, int b) { return a + b; }
struct FnMul { int operator()(int a, int b) const { return a * b; } };
TEST_CASE(functional, std_function) {
    STD::function<int(int, int)> f = fn_add;
    test_true( (bool)f );
    test_pass("cxx11:function::operator bool");
    test_eq( f(2, 3), 5 );
    test_pass("cxx11:function::operator()");

    f = FnMul();
    test_eq( f(4, 5), 20 );
    test_pass("cxx11:function::operator=");

    STD::function<int(int, int)> g;
    test_true( !g );
    test_pass("cxx11:function(default ctor is empty)");
    test_throw( g(1, 2) );
    test_pass("cxx11:function(empty call throws)");

    STD::function<int(int, int)> a = fn_add, b = FnMul();
    a.swap(b);
    test_eq( a(3, 4), 12 );
    test_eq( b(3, 4), 7 );
    test_pass("cxx11:function::swap");

    STD::swap(a, b);
    test_eq( a(3, 4), 7 );
    test_eq( b(3, 4), 12 );
    test_pass("cxx11:swap(function)");

    STD::bad_function_call e;
    STD::string w = e.what();
    test_true( w.size() > 0 );
    test_pass("cxx11:bad_function_call");
}
#endif

#if TEST_TARGET_CXX >= 2017
TEST_CASE(functional, boyer_moore_searchers) {
    STD::string hay = "the quick brown fox jumps over";
    STD::string ndl = "brown";
    STD::boyer_moore_searcher<STD::string::const_iterator> bm(ndl.begin(), ndl.end());
    STD::pair<STD::string::const_iterator, STD::string::const_iterator> r = bm(hay.begin(), hay.end());
    test_eq( (int)(r.first - hay.begin()), 10 );
    test_eq( (int)(r.second - hay.begin()), 15 );

    STD::string miss = "zzz";
    STD::boyer_moore_searcher<STD::string::const_iterator> bm2(miss.begin(), miss.end());
    STD::pair<STD::string::const_iterator, STD::string::const_iterator> r2 = bm2(hay.begin(), hay.end());
    test_true( r2.first == hay.end() );
    test_pass("cxx17:boyer_moore_searcher");

    STD::string ndl3 = "over";
    STD::boyer_moore_horspool_searcher<STD::string::const_iterator> bh(ndl3.begin(), ndl3.end());
    STD::pair<STD::string::const_iterator, STD::string::const_iterator> r3 = bh(hay.begin(), hay.end());
    test_eq( (int)(r3.first - hay.begin()), 26 );
    test_true( r3.second == hay.end() );

    STD::string big = "this is a very long needle string";
    STD::boyer_moore_horspool_searcher<STD::string::const_iterator> bh2(big.begin(), big.end());
    STD::pair<STD::string::const_iterator, STD::string::const_iterator> r4 = bh2(ndl.begin(), ndl.end());
    test_true( r4.first == ndl.end() );
    test_pass("cxx17:boyer_moore_horspool_searcher");

    STD::string ndl5 = "the";
    STD::boyer_moore_searcher<STD::string::const_iterator> bm5(ndl5.begin(), ndl5.end());
    STD::pair<STD::string::const_iterator, STD::string::const_iterator> r5 = bm5(hay.begin(), hay.end());
    test_eq( (int)(r5.first - hay.begin()), 0 );
    test_pass("cxx17:boyer_moore_searcher(match at index 0)");
}
#endif

#if TEST_TARGET_CXX >= 2017
TEST_CASE(functional, default_searcher) {
    STD::string hay = "hello world wide web";
    STD::string ndl = "world";
    STD::default_searcher<STD::string::const_iterator> s(ndl.begin(), ndl.end());
    STD::pair<STD::string::const_iterator, STD::string::const_iterator> r = s(hay.begin(), hay.end());
    test_eq( (int)(r.first - hay.begin()), 6 );
    test_eq( (int)(r.second - hay.begin()), 11 );
    test_pass("cxx17:default_searcher");

    STD::string ndl2 = "xyz";
    STD::default_searcher<STD::string::const_iterator> s2(ndl2.begin(), ndl2.end());
    STD::pair<STD::string::const_iterator, STD::string::const_iterator> r2 = s2(hay.begin(), hay.end());
    test_true( r2.first == hay.end() );
    test_true( r2.second == hay.end() );
    test_pass("cxx17:default_searcher(no match)");

    STD::string ndl3 = "web";
    STD::default_searcher<STD::string::const_iterator> s3(ndl3.begin(), ndl3.end());
    STD::pair<STD::string::const_iterator, STD::string::const_iterator> r3 = s3(hay.begin(), hay.end());
    test_eq( (int)(r3.first - hay.begin()), 17 );
    test_pass("cxx17:default_searcher(match at end)");
}
#endif

#if TEST_TARGET_CXX >= 2011
namespace {
int bt_add2(int a, int b) { return a + b; }
int bt_add3(int a, int b, int c) { return a + b + c; }
int bt_neg(int a) { return -a; }
struct BtBox {
    int v;
    int get() const { return v; }
    int addv(int x) { return v + x; }
};
template <class T> int bt_ph_index(const T&) { return STD::is_placeholder<T>::value; }
template <class T> bool bt_is_bind(const T&) { return STD::is_bind_expression<T>::value; }
}

TEST_CASE(functional, bind_placeholders) {
    using namespace STD::placeholders;

    test_eq( STD::bind(bt_add2, 2, 3)(), 5 );
    test_pass("cxx11:bind");

    test_eq( STD::bind(bt_neg, _1)(7), -7 );
    test_pass("cxx11:placeholders");

    test_eq( STD::bind(bt_add2, _1, _2)(4, 5), 9 );
    test_pass("cxx11:bind(two placeholders)");

    test_eq( STD::bind(bt_add2, _2, _1)(4, 5), 5 + 4 );
    test_eq( STD::bind(bt_add3, _1, _2, _3)(1, 2, 3), 6 );
    test_pass("cxx11:bind(argument reorder)");

    test_eq( STD::bind(bt_add2, 100, _1)(5), 105 );
    test_eq( STD::bind(bt_add3, _1, 10, _2)(1, 2), 13 );
    test_pass("cxx11:bind(mixed bound and placeholder)");
}

TEST_CASE(functional, bind_traits_and_members) {
    using namespace STD::placeholders;

    test_eq( bt_ph_index(_1), 1 );
    test_eq( bt_ph_index(_3), 3 );
    int notph = 0;
    test_eq( bt_ph_index(notph), 0 );
    test_pass("cxx11:is_placeholder");

    test_true( bt_is_bind(STD::bind(bt_neg, _1)) );
    test_true( !bt_is_bind(notph) );
    test_pass("cxx11:is_bind_expression");

    BtBox b; b.v = 40;
    test_eq( STD::bind(&BtBox::addv, _1, 2)(b), 42 );
    test_pass("cxx11:bind(member function)");

    const BtBox cb = b;
    test_eq( STD::bind(&BtBox::get, _1)(cb), 40 );
    test_pass("cxx11:bind(const member function)");
}
#endif

TEST_CASE(functional, deprecated_adapters) {
    test_skip("cxx03:unary_function (deprecated cxx11)");
    test_skip("cxx03:binary_function (deprecated cxx11)");
    test_skip("cxx03:ptr_fun (deprecated cxx11)");
    test_skip("cxx03:mem_fun (deprecated cxx11)");
    test_skip("cxx03:mem_fun_ref (deprecated cxx11)");
    test_skip("cxx03:bind1st (deprecated cxx11)");
    test_skip("cxx03:bind2nd (deprecated cxx11)");
    test_skip("cxx03:not1 (deprecated cxx17)");
    test_skip("cxx03:not2 (deprecated cxx17)");
}

TEST_CASE(functional, hash_and_mem_fn) {

    STD::hash<int> hi;
    test_true( hi(42) == hi(42) );
    STD::hash<STD::string> hs;
    test_true( hs(STD::string("abc")) == hs(STD::string("abc")) );
    test_pass("cxx11:hash");

    FnBox o; o.v = 10; o.mem = 7;
    test_eq( STD::mem_fn(&FnBox::add)(o, 5), 15 );
#if !defined(__WATCOMC__)
    test_eq( STD::mem_fn(&FnBox::mem)(o), 7 );
    test_pass("cxx11:mem_fn");
#else
    TEST_SKIP1();
    test_skip("cxx11:mem_fn");
#endif
}

#if TEST_TARGET_CXX >= 2014
TEST_CASE(functional, transparent_operators) {
    test_eq( STD::plus<void>()(2, 3), 5 );
    test_pass("cxx14:plus<void> (transparent)");
    test_eq( STD::minus<void>()(5, 2), 3 );
    test_pass("cxx14:minus<void> (transparent)");
    test_eq( STD::multiplies<void>()(4, 3), 12 );
    test_pass("cxx14:multiplies<void> (transparent)");
    test_eq( STD::divides<void>()(12, 4), 3 );
    test_pass("cxx14:divides<void> (transparent)");
    test_eq( STD::modulus<void>()(7, 3), 1 );
    test_pass("cxx14:modulus<void> (transparent)");
    test_eq( STD::negate<void>()(5), -5 );
    test_pass("cxx14:negate<void> (transparent)");

    test_true(  STD::equal_to<void>()(2, 2) );
    test_pass("cxx14:equal_to<void> (transparent)");
    test_true(  STD::not_equal_to<void>()(2, 3) );
    test_pass("cxx14:not_equal_to<void> (transparent)");
    test_true(  STD::less<void>()(1, 2) );
    test_pass("cxx14:less<void> (transparent)");
    test_true(  STD::greater<void>()(2, 1) );
    test_pass("cxx14:greater<void> (transparent)");
    test_true(  STD::less_equal<void>()(2, 2) );
    test_pass("cxx14:less_equal<void> (transparent)");
    test_true(  STD::greater_equal<void>()(2, 2) );
    test_pass("cxx14:greater_equal<void> (transparent)");

    test_true( !STD::logical_and<void>()(true, false) );
    test_pass("cxx14:logical_and<void> (transparent)");
    test_true(  STD::logical_or<void>()(true, false) );
    test_pass("cxx14:logical_or<void> (transparent)");
    test_true(  STD::logical_not<void>()(false) );
    test_pass("cxx14:logical_not<void> (transparent)");

    test_eq( STD::bit_and<void>()(6, 3), 2 );
    test_pass("cxx14:bit_and<void> (transparent)");
    test_eq( STD::bit_or<void>()(4, 1), 5 );
    test_pass("cxx14:bit_or<void> (transparent)");
    test_eq( STD::bit_xor<void>()(6, 3), 5 );
    test_pass("cxx14:bit_xor<void> (transparent)");
    test_true( STD::bit_not<void>()(5) == ~5 );
    test_pass("cxx14:bit_not<void> (transparent)");
}
#else
TEST_CASE(functional, transparent_operators) {
    TEST_NOTE("the transparent operator functors are C++14");
    test_skip("cxx14:plus<void> (transparent)");
    test_skip("cxx14:minus<void> (transparent)");
    test_skip("cxx14:multiplies<void> (transparent)");
    test_skip("cxx14:divides<void> (transparent)");
    test_skip("cxx14:modulus<void> (transparent)");
    test_skip("cxx14:negate<void> (transparent)");
    test_skip("cxx14:equal_to<void> (transparent)");
    test_skip("cxx14:not_equal_to<void> (transparent)");
    test_skip("cxx14:less<void> (transparent)");
    test_skip("cxx14:greater<void> (transparent)");
    test_skip("cxx14:less_equal<void> (transparent)");
    test_skip("cxx14:greater_equal<void> (transparent)");
    test_skip("cxx14:logical_and<void> (transparent)");
    test_skip("cxx14:logical_or<void> (transparent)");
    test_skip("cxx14:logical_not<void> (transparent)");
    test_skip("cxx14:bit_and<void> (transparent)");
    test_skip("cxx14:bit_or<void> (transparent)");
    test_skip("cxx14:bit_xor<void> (transparent)");
    test_skip("cxx14:bit_not<void> (transparent)");
}
#endif

TEST_CASE(functional, cxx20_functional) {

#if TEST_TARGET_CXX >= 2020 && !(defined(_MSC_VER) && _MSC_VER >= 1600 && _MSC_VER < 1900 && !defined(__cpp_lib_ranges))
    test_eq( STD::identity()(42), 42 );
    test_pass("cxx20:identity");
#else
    test_skip("cxx20:identity");
#endif

#if defined(__cpp_lib_bind_front)
    test_eq( (int)STD::bind_front(&fn_double, 21)(), 42 );
    test_pass("cxx20:bind_front");
#else
    test_skip("cxx20:bind_front");
#endif

#if defined(__cpp_lib_ranges)
    test_true(  STD::ranges::equal_to()(2, 2) );
    test_pass("cxx20:ranges::equal_to");
    test_true(  STD::ranges::not_equal_to()(2, 3) );
    test_pass("cxx20:ranges::not_equal_to");
    test_true(  STD::ranges::less()(1, 2) );
    test_pass("cxx20:ranges::less");
    test_true(  STD::ranges::greater()(2, 1) );
    test_pass("cxx20:ranges::greater");
    test_true(  STD::ranges::less_equal()(2, 2) );
    test_pass("cxx20:ranges::less_equal");
    test_true(  STD::ranges::greater_equal()(2, 2) );
    test_pass("cxx20:ranges::greater_equal");
#else
    test_skip("cxx20:ranges::equal_to");
    test_skip("cxx20:ranges::not_equal_to");
    test_skip("cxx20:ranges::less");
    test_skip("cxx20:ranges::greater");
    test_skip("cxx20:ranges::less_equal");
    test_skip("cxx20:ranges::greater_equal");
#endif

#if TEST_TARGET_CXX >= 2020
    test_true(( STD::is_same<STD::unwrap_reference<STD::reference_wrapper<int> >::type, int&>::value ));
    test_pass("cxx20:unwrap_reference");
    test_true(( STD::is_same<STD::unwrap_ref_decay<int>::type, int>::value ));
    test_pass("cxx20:unwrap_ref_decay");
#else
    TEST_NOTE("unwrap_reference / unwrap_ref_decay are C++20");
    test_skip("cxx20:unwrap_reference");
    test_skip("cxx20:unwrap_ref_decay");
#endif
}

TEST_CASE(functional, cxx23_cxx26_functional) {
#if defined(__cpp_lib_bind_back)
    test_eq( (int)STD::bind_back(&fn_double)(21), 42 );
    test_pass("cxx23:bind_back");
#else
    test_skip("cxx23:bind_back");
#endif

#if defined(__cpp_lib_invoke_r)
    test_eq( (int)STD::invoke_r<long>(&fn_double, 21), 42 );
    test_pass("cxx23:invoke_r");
#else
    test_skip("cxx23:invoke_r");
#endif

#if defined(__cpp_lib_move_only_function)
    STD::move_only_function<int(int)> mof = &fn_double;
    test_eq( (int)mof(21), 42 );
    test_pass("cxx23:move_only_function");
#else
    test_skip("cxx23:move_only_function");
#endif

#if defined(__cpp_lib_copyable_function)
    STD::copyable_function<int(int)> cf = &fn_double;
    test_eq( (int)cf(21), 42 );
    test_pass("cxx26:copyable_function");
#else
    test_skip("cxx26:copyable_function");
#endif

#if defined(__cpp_lib_function_ref)
    STD::function_ref<int(int)> fr = &fn_double;
    test_eq( (int)fr(21), 42 );
    test_pass("cxx26:function_ref");
#else
    test_skip("cxx26:function_ref");
#endif
}


#if TEST_TARGET_CXX >= 2011 && _TST_HAS_CXX11_LIB_MEMBERS
TEST_CASE(functional, reference_wrapper_conversion_cxx11) {
    int v = 5;
    STD::reference_wrapper<int> r(v);

    int& ref = r;
    ref = 9;
    test_eq( v, 9 );
    test_eq( r.get(), 9 );
    test_true( &ref == &v );

    test_eq( r + 1, 10 );
    test_pass("cxx11:reference_wrapper::operator T&() const noexcept");
}

TEST_CASE(functional, bind_traits_v_cxx17) {
#if TEST_TARGET_CXX >= 2017 && _TST_HAS_VAR_TEMPLATE
    test_true( STD::is_bind_expression_v<decltype(STD::bind(STD::plus<int>(), 1, 2))> );
    test_true( !STD::is_bind_expression_v<int> );
    test_true( STD::is_bind_expression_v<decltype(STD::bind(STD::plus<int>(), 1, 2))> ==
               STD::is_bind_expression<decltype(STD::bind(STD::plus<int>(), 1, 2))>::value );
    test_pass("cxx17:is_bind_expression_v");

    test_eq( (int)STD::is_placeholder_v<decltype(STD::placeholders::_1)>, 1 );
    test_eq( (int)STD::is_placeholder_v<decltype(STD::placeholders::_3)>, 3 );
    test_eq( (int)STD::is_placeholder_v<int>, 0 );
    test_pass("cxx17:is_placeholder_v");
#else
    test_skip("cxx17:is_bind_expression_v");
    test_skip("cxx17:is_placeholder_v");
#endif
}

#if TEST_HAS_EH
TEST_CASE(functional, bad_function_call_what_cxx20) {
    STD::function<int()> f;
    bool caught = false;
    try { (void)f(); }
    catch (const STD::bad_function_call& e) {
        caught = (e.what() != 0 && STD::strlen(e.what()) > 0);
    }
    test_true( caught );
    test_true(( STD::is_base_of<STD::exception, STD::bad_function_call>::value ));
    test_pass("cxx20:bad_function_call::what");
}
#else   // !TEST_HAS_EH
TEST_CASE(functional, bad_function_call_what_cxx20) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx20:bad_function_call::what");
}
#endif  // TEST_HAS_EH

TEST_CASE(functional, compare_three_way_cxx20) {
#if defined(__cpp_lib_three_way_comparison)
    STD::compare_three_way cmp;
    test_true( cmp(1, 2) < 0 );
    test_true( cmp(2, 1) > 0 );
    test_true( cmp(2, 2) == 0 );
    test_true( cmp(1, 2L) < 0 );
    test_true(( STD::is_same<STD::compare_three_way::is_transparent,
                             STD::compare_three_way::is_transparent>::value ));
    test_pass("cxx20:compare_three_way");
#else
    TEST_NOTE("compare_three_way needs the three-way comparison library");
    test_skip("cxx20:compare_three_way");
#endif
}

TEST_CASE(functional, reference_wrapper_comparison_cxx26) {
#if defined(__cpp_lib_reference_wrapper) && __cpp_lib_reference_wrapper >= 202403L
    int a = 1, b = 1, c = 2;
    STD::reference_wrapper<int> ra(a), rb(b), rc(c);
    test_true( ra == rb );
    test_true( !(ra == rc) );
    test_pass("cxx26:reference_wrapper::operator==");
    test_true( (ra <=> rc) < 0 );
    test_true( (ra <=> rb) == 0 );
    test_pass("cxx26:reference_wrapper::operator<=>");
#else
    TEST_NOTE("reference_wrapper's comparisons are C++26 (P2944); not in this library");
    test_skip("cxx26:reference_wrapper::operator==");
    test_skip("cxx26:reference_wrapper::operator<=>");
#endif

#if defined(__cpp_lib_common_reference_wrapper)
    test_true(( STD::is_same<STD::common_reference_t<STD::reference_wrapper<int>, int&>,
                             int&>::value ));
    test_pass("cxx23:basic_common_reference");
#else
    TEST_NOTE("basic_common_reference for reference_wrapper is P2655; not in this library");
    test_skip("cxx23:basic_common_reference");
#endif
}
#else
TEST_CASE(functional, reference_wrapper_conversion_cxx11) {
    test_skip("cxx11:reference_wrapper::operator T&() const noexcept");
}
TEST_CASE(functional, bind_traits_v_cxx17) {
    test_skip("cxx17:is_bind_expression_v");
    test_skip("cxx17:is_placeholder_v");
}
TEST_CASE(functional, bad_function_call_what_cxx20) {
    test_skip("cxx20:bad_function_call::what");
}
TEST_CASE(functional, compare_three_way_cxx20) {
    test_skip("cxx20:compare_three_way");
}
TEST_CASE(functional, reference_wrapper_comparison_cxx26) {
    test_skip("cxx26:reference_wrapper::operator==");
    test_skip("cxx26:reference_wrapper::operator<=>");
    test_skip("cxx23:basic_common_reference");
}
#endif

TEST_CASE(functional, feature_test_macros) {
#if defined(__cpp_lib_invoke)
    test_true( __cpp_lib_invoke > 0L );
    test_pass("cxx17:__cpp_lib_invoke");
#else
    test_skip("cxx17:__cpp_lib_invoke");
#endif
#if defined(__cpp_lib_boyer_moore_searcher)
    test_true( __cpp_lib_boyer_moore_searcher > 0L );
    test_pass("cxx17:__cpp_lib_boyer_moore_searcher");
#else
    test_skip("cxx17:__cpp_lib_boyer_moore_searcher");
#endif
#if defined(__cpp_lib_bind_front)
    test_true( __cpp_lib_bind_front > 0L );
    test_pass("cxx20:__cpp_lib_bind_front");
#else
    test_skip("cxx20:__cpp_lib_bind_front");
#endif
#if defined(__cpp_lib_constexpr_functional)
    test_true( __cpp_lib_constexpr_functional > 0L );
    test_pass("cxx20:__cpp_lib_constexpr_functional");
#else
    test_skip("cxx20:__cpp_lib_constexpr_functional");
#endif
#if defined(__cpp_lib_bind_back)
    test_true( __cpp_lib_bind_back > 0L );
    test_pass("cxx23:__cpp_lib_bind_back");
#else
    test_skip("cxx23:__cpp_lib_bind_back");
#endif
#if defined(__cpp_lib_invoke_r)
    test_true( __cpp_lib_invoke_r > 0L );
    test_pass("cxx23:__cpp_lib_invoke_r");
#else
    test_skip("cxx23:__cpp_lib_invoke_r");
#endif
#if defined(__cpp_lib_move_only_function)
    test_true( __cpp_lib_move_only_function > 0L );
    test_pass("cxx23:__cpp_lib_move_only_function");
#else
    test_skip("cxx23:__cpp_lib_move_only_function");
#endif
#if defined(__cpp_lib_copyable_function)
    test_true( __cpp_lib_copyable_function > 0L );
    test_pass("cxx26:__cpp_lib_copyable_function");
#else
    test_skip("cxx26:__cpp_lib_copyable_function");
#endif
#if defined(__cpp_lib_function_ref)
    test_true( __cpp_lib_function_ref > 0L );
    test_pass("cxx26:__cpp_lib_function_ref");
#else
    test_skip("cxx26:__cpp_lib_function_ref");
#endif
#if defined(__cpp_lib_freestanding_functional)
    test_true( __cpp_lib_freestanding_functional > 0L );
    test_pass("cxx26:__cpp_lib_freestanding_functional");
#else
    test_skip("cxx26:__cpp_lib_freestanding_functional");
#endif
}


#if TEST_TARGET_CXX >= 2011 && _TST_HAS_CXX11_LIB_MEMBERS
namespace { int ccw_fn_add2(int a, int b) { return a + b; } }

TEST_CASE(functional, function_target_cxx11) {
    STD::function<int(int, int)> f = ccw_fn_add2;
    test_true( (bool)f );
    test_eq( f(2, 3), 5 );
    test_pass("cxx11:function");
    test_pass("cxx11:function::function(F)");

#if _TST_HAS_RTTI
    test_true( f.target_type() == typeid(int (*)(int, int)) );
    STD::function<int(int, int)> empty;
    test_true( empty.target_type() == typeid(void) );
    test_pass("cxx11:function::target_type");

    int (**tp)(int, int) = f.target<int (*)(int, int)>();
    test_ptr( (void*)tp );
    test_eq( (*tp)(4, 5), 9 );
    test_true( f.target<STD::plus<int> >() == 0 );
    test_true( empty.target<int (*)(int, int)>() == 0 );
    test_pass("cxx11:function::target");
#else
    TEST_NOTE("target_type / target need RTTI; this build has none");
    test_skip("cxx11:function::target_type");
    test_skip("cxx11:function::target");
#endif
}
#else
TEST_CASE(functional, function_target_cxx11) {
    test_skip("cxx11:function");
    test_skip("cxx11:function::function(F)");
    test_skip("cxx11:function::target_type");
    test_skip("cxx11:function::target");
}
#endif

static int g_fx_live = 0;

struct FxLive {
    typedef int result_type;
    typedef int argument_type;
    char* p_;
    FxLive() : p_(new char[32]) { p_[0] = 0; ++g_fx_live; }
    FxLive(char c) : p_(new char[32]) { p_[0] = c; p_[1] = 0; ++g_fx_live; }
    FxLive(const FxLive& o) : p_(new char[32]) {
        for (int i = 0; i < 32; ++i) p_[i] = o.p_[i];
        ++g_fx_live;
    }
    FxLive& operator=(const FxLive& o) {
        if (this != &o) for (int i = 0; i < 32; ++i) p_[i] = o.p_[i];
        return *this;
    }
    ~FxLive() { delete[] p_; --g_fx_live; }
    int operator()(int x) const { return x + (int)p_[0]; }
    char tag() const { return p_[0]; }
};

TEST_CASE(functional, stateful_callable_lifetime) {
    g_fx_live = 0;
    {
        FxLive src(1);
        test_eq( g_fx_live, 1 );

        STD::function<int(int)> f = src;
        test_true( (bool)f );
        test_eq( f(10), 11 );
        test_true( g_fx_live >= 2 );
        int after_store = g_fx_live;

        STD::function<int(int)> c(f);
        test_eq( c(10), 11 );
        test_eq( g_fx_live, after_store + 1 );

        FxLive other(2);
        c = other;
        test_eq( c(10), 12 );
        test_eq( g_fx_live, after_store + 2 );

        STD::function<int(int)> e;
        test_true( !e );
        e.swap(c);
        test_true( (bool)e );
        test_true( !c );
        test_eq( e(10), 12 );
        test_eq( g_fx_live, after_store + 2 );

        STD::reference_wrapper<FxLive> r(src);
        test_eq( r.get().tag(), (char)1 );
        test_eq( r(10), 11 );
        test_eq( g_fx_live, after_store + 2 );
    }
    test_eq( g_fx_live, 0 );
}

TEST_CASE(functional, constexpr_reference_wrapper_cxx20) {
#if defined(__cpp_lib_constexpr_functional) && (__cpp_lib_constexpr_functional) >= 201907L
    struct K {
        static constexpr int run() { int v = 7; STD::reference_wrapper<int> r(v);
                                     int& q = r; return q; }
    };
    static_assert(K::run() == 7, "constexpr reference_wrapper::operator T&");
    test_eq( K::run(), 7 );
    test_pass("cxx20:constexpr reference_wrapper::operator T&() const noexcept");
#else
    TEST_NOTE("reference_wrapper became constexpr in C++20 (__cpp_lib_constexpr_functional)");
    TEST_SKIP_N(1);
    test_skip("cxx20:constexpr reference_wrapper::operator T&() const noexcept");
#endif
}
