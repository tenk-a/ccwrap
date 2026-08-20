#include "test_cxx.hpp"
#include <stack>
#include <vector>
#include <deque>
#include <string>
#if defined(__cpp_lib_format_ranges)
#include <format>
#endif

TEST_CASE(stack, basic_lifo) {
    STD::stack<int> s;
    test_true( s.empty() );
    test_eq( s.size(), 0u );
    test_pass("cxx03:stack::size");

    s.push(1);
    s.push(2);
    s.push(3);
    test_eq( s.size(), 3u );
    test_true( !s.empty() );
    test_pass("cxx03:stack::push");

    test_eq( s.top(), 3 );
    const STD::stack<int>& cs = s;
    test_eq( cs.top(), 3 );
    s.top() = 30;
    test_eq( s.top(), 30 );
    test_pass("cxx03:stack::top");

    s.pop();
    test_eq( s.top(), 2 );
    s.pop();
    test_eq( s.top(), 1 );
    s.pop();
    test_true( s.empty() );
    test_pass("cxx03:stack::pop");
}

TEST_CASE(stack, ctor_emplace_swap) {

    STD::deque<int> d;
    d.push_back(1); d.push_back(2);
    STD::stack<int> s(d);
    test_eq( s.size(), 2u );
    test_eq( s.top(), 2 );
    test_pass("cxx03:stack ctor");

#if _TST_HAS_CXX11_LIB_MEMBERS
    s.emplace(9);
    test_eq( s.top(), 9 );
#else
    s.push(9);
    TEST_SKIP1();
#endif

#if _TST_HAS_CXX11_LIB_MEMBERS
    STD::stack<int> a, b;
    a.push(1); a.push(2);
    b.push(9);
    a.swap(b);
    test_eq( a.top(), 9 );
    test_eq( a.size(), 1u );
    test_eq( b.size(), 2u );
    STD::swap(a, b);
    test_eq( a.size(), 2u );
    test_pass("cxx03:stack swap");
#else
    test_skip("cxx03:stack swap");
#endif
}

TEST_CASE(stack, string_elements) {
    STD::stack<STD::string> s;
    s.push("first");
    s.push("second");
    s.push(STD::string(300, 'z'));
    test_eq( s.size(), 3u );
    test_eq( s.top().size(), 300u );
    s.pop();
    test_eq( s.top(), STD::string("second") );
    s.top() = "SECOND";
    test_eq( s.top(), STD::string("SECOND") );
    s.pop();
    test_eq( s.top(), STD::string("first") );

#if _TST_HAS_CXX11_LIB_MEMBERS
    STD::stack<STD::string> a, b;
    a.push("x");
    b.push("y"); b.push("z");
    a.swap(b);
    test_eq( a.size(), 2u );
    test_eq( a.top(), STD::string("z") );
#endif
    test_pass("cxx03:stack of strings");
}

TEST_CASE(stack, alternate_container_and_compare) {

    STD::stack<int, STD::vector<int> > s;
    s.push(5);
    s.push(6);
    test_eq( s.top(), 6 );
    s.pop();
    test_eq( s.top(), 5 );

    STD::stack<int> a, b, c;
    a.push(1); a.push(2);
    b.push(1); b.push(2);
    c.push(1);
    test_true( a == b );
    test_true( a != c );
    test_true( c <  a );
    test_true( a >  c );
    test_true( a <= b );
    test_true( a >= b );
    test_pass("cxx03:stack over alternate container + comparison");
}

TEST_CASE(stack, member_typedefs) {
    typedef STD::stack<int> S;

    S::value_type v = 5;
    test_eq( v, 5 );
    test_pass("cxx03:stack::value_type");

    S s; s.push(9);
    S::reference r = s.top();
    r = 10;
    test_eq( s.top(), 10 );
    test_pass("cxx03:stack::reference");

    S::size_type n = s.size();
    test_eq( n, (STD::size_t)1 );
    test_pass("cxx03:stack::size_type");

    S::container_type c;
    c.push_back(3);
    S s2(c);
    test_eq( s2.top(), 3 );
    test_pass("cxx03:stack::container_type");

    const S& cs = s2;
    S::const_reference cr = cs.top();
    test_eq( cr, 3 );
    test_pass("cxx03:stack::const_reference");
}

TEST_CASE(stack, empty_observer) {
    STD::stack<int> s;
    test_true( s.empty() );
    s.push(1);
    test_true( !s.empty() );
    s.pop();
    test_true( s.empty() );

    const STD::stack<int>& cs = s;
    test_true( cs.empty() == (cs.size() == 0) );
    test_pass("cxx03:stack::empty");
}

TEST_CASE(stack, move_and_allocators) {
#if TEST_TARGET_CXX >= 2011
    {
        STD::stack<STD::string> s;
        STD::string x("moved");
        s.push(STD::move(x));
        test_eq( s.top(), STD::string("moved") );
    }
    test_pass("cxx11:stack::push(T&&)");
#else
    TEST_SKIP1();
    test_skip("cxx11:stack::push(T&&)");
#endif

#if TEST_TARGET_CXX >= 2011
    {
        STD::stack<int> a; a.push(1); a.push(2);
        STD::stack<int> b(STD::move(a));
        test_eq( b.size(), (STD::size_t)2 );
        test_eq( b.top(), 2 );
    }
    test_pass("cxx11:stack::stack(stack&&)");
#else
    TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx11:stack::stack(stack&&)");
#endif

#if TEST_TARGET_CXX >= 2011
    {
        STD::deque<int> d; d.push_back(7); d.push_back(8);
        STD::stack<int> s(STD::move(d));
        test_eq( s.top(), 8 );
    }
    test_pass("cxx11:stack::stack(Container&&)");
#else
    TEST_SKIP1();
    test_skip("cxx11:stack::stack(Container&&)");
#endif

#if _TST_HAS_CXX11_LIB_MEMBERS
    {
        STD::allocator<int> al;
        STD::stack<int> s(al);
        s.push(4);
        test_eq( s.top(), 4 );
        STD::deque<int> d; d.push_back(1);
        STD::stack<int> s2(d, al);
        test_eq( s2.top(), 1 );
    }
    test_pass("cxx11:stack::stack(const Alloc&) family");
#else
    TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx11:stack::stack(const Alloc&) family");
#endif

#if TEST_TARGET_CXX >= 2011
    TEST_SKIP_NATIVE_NO_MOVE("the native pre-C++11 stack has no allocator_type, and a member cannot be added to it");
    test_true( (STD::uses_allocator<STD::stack<int>, STD::allocator<int> >::value) );
    test_pass("cxx11:uses_allocator<stack>");
#else
    TEST_SKIP1();
    test_skip("cxx11:uses_allocator<stack>");
#endif
}

TEST_CASE(stack, spaceship) {
#if defined(__cpp_impl_three_way_comparison) && defined(__cpp_lib_three_way_comparison)
    STD::stack<int> a, b;
    a.push(1); a.push(2);
    b.push(1); b.push(3);
    test_true( (a <=> a) == 0 );
    test_true( (a <=> b) <  0 );
    test_true( (b <=> a) >  0 );
    test_pass("cxx20:operator<=>(stack)");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx20:operator<=>(stack)");
#endif
}

TEST_CASE(stack, iterator_and_range_ctors) {
    int src[] = { 4, 5, 6 };
#if defined(__cpp_lib_adaptor_iterator_pair_constructor)
    {
        STD::stack<int> s(src, src + 3);
        test_eq( s.size(), (STD::size_t)3 );
        test_eq( s.top(), 6 );
        test_pass("cxx23:stack::stack(InputIt, InputIt)");
    }
#else
    test_skip("cxx23:stack::stack(InputIt, InputIt)");
#endif

#if defined(__cpp_lib_containers_ranges)
    {
        STD::stack<int> s(STD::from_range, src);
        test_eq( s.size(), (STD::size_t)3 );
        test_eq( s.top(), 6 );
        test_pass("cxx23:stack::stack(from_range_t, R&&)");
    }
    {
        STD::stack<int> s;
        s.push_range(src);
        test_eq( s.size(), (STD::size_t)3 );
        test_eq( s.top(), 6 );
        test_pass("cxx23:stack::push_range");
    }
#else
    test_skip("cxx23:stack::stack(from_range_t, R&&)");
    test_skip("cxx23:stack::push_range");
#endif
}

TEST_CASE(stack, feature_test_macros) {
#if defined(__cpp_lib_adaptor_iterator_pair_constructor) && (__cpp_lib_adaptor_iterator_pair_constructor) >= 202106L
    test_true( (__cpp_lib_adaptor_iterator_pair_constructor) >= 202106L );
    test_pass("cxx23:__cpp_lib_adaptor_iterator_pair_constructor=202106L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_adaptor_iterator_pair_constructor=202106L");
#endif
#if defined(__cpp_lib_containers_ranges) && (__cpp_lib_containers_ranges) >= 202202L
    test_true( (__cpp_lib_containers_ranges) >= 202202L );
    test_pass("cxx23:__cpp_lib_containers_ranges=202202L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_containers_ranges=202202L");
#endif
#if defined(__cpp_lib_constexpr_stack) && (__cpp_lib_constexpr_stack) >= 202502L
    test_true( (__cpp_lib_constexpr_stack) >= 202502L );
    test_pass("cxx26:__cpp_lib_constexpr_stack=202502L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx26:__cpp_lib_constexpr_stack=202502L");
#endif
}


TEST_CASE(stack, emplace_and_cxx23_cxx26) {
#if _TST_HAS_CXX11_LIB_MEMBERS
    {
        STD::stack<STD::pair<int, int> > s;
        s.emplace(1, 2);
        test_true( s.top().first == 1 && s.top().second == 2 );
        test_eq( (int)s.size(), 1 );
    }
    test_pass("cxx11:stack::emplace");
#else
    test_skip("cxx11:stack::emplace");
#endif

#if defined(__cpp_lib_format_ranges)
    {
        STD::stack<int> s;
        s.push(1); s.push(2);
        STD::string out = STD::format("{}", s);
        test_true( out.size() > 0 );
    }
    test_pass("cxx23:formatter<stack>");
#else
    TEST_NOTE("formatter<stack> comes with the C++23 range formatters");
    test_skip("cxx23:formatter<stack>");
#endif

#if defined(__cpp_lib_constexpr_stack) || defined(__cpp_lib_constexpr_deque)
    {
        STD::stack<int> s;
        s.push(5);
        test_eq( s.top(), 5 );
        s.pop();
        test_true( s.empty() );
    }
    test_pass("cxx26:constexpr stack");
#else
    TEST_NOTE("constexpr stack follows the constexpr container work (C++26)");
    test_skip("cxx26:constexpr stack");
#endif
}

static int g_st_live = 0;

struct StLive {
    STD::string s_;
    StLive() { ++g_st_live; }
    StLive(const char* s) : s_(s) { ++g_st_live; }
    StLive(const StLive& o) : s_(o.s_) { ++g_st_live; }
    StLive& operator=(const StLive& o) { s_ = o.s_; return *this; }
    ~StLive() { --g_st_live; }
};

static StLive st_live_key(int i) {
    STD::string t("k");
    t += (char)('0' + i);
    t += "_long_enough_to_leave_the_sso_buffer";
    return StLive(t.c_str());
}

TEST_CASE(stack, class_elements_lifetime) {
    g_st_live = 0;
    {
        STD::stack<StLive> s;
        for (int i = 0; i < 8; ++i) s.push(st_live_key(i));
        test_eq( (int)s.size(), 8 );
        test_eq( g_st_live, 8 );

        STD::stack<StLive> c(s);
        test_eq( g_st_live, 16 );

        s.pop();
        test_eq( (int)s.size(), 7 );
        test_eq( g_st_live, 15 );

        while (!c.empty()) c.pop();
        test_eq( g_st_live, 7 );

        STD::stack<StLive, STD::vector<StLive> > sv;
        for (int i = 0; i < 4; ++i) sv.push(st_live_key(i));
        test_eq( (int)sv.size(), 4 );
        test_eq( g_st_live, 11 );
        sv.pop();
        test_eq( g_st_live, 10 );
    }
    test_eq( g_st_live, 0 );
}
