#include "test_cxx.hpp"

#if _TST_TEST_HAS(<version>)
#  include <version>
#endif

#include <memory>
#include <string>
#include <type_traits>
#include <sstream>
#include <utility>
#include <cstddef>

namespace {
    struct MemFoo { int v; MemFoo(int x = 0) : v(x) {} };
    int g_mem_del = 0;
    struct MemDeleter { void operator()(MemFoo* p) const { ++g_mem_del; delete p; } };
    struct MemBase { virtual ~MemBase() {} int b; };
    struct MemDer : MemBase { int d; };
    int g_mem_dtor = 0;
    struct MemCount { ~MemCount() { ++g_mem_dtor; } };
}

TEST_CASE(memory, unique_ptr) {
    STD::unique_ptr<int> p(new int(5));
    test_ptr( p.get() );
    test_true( (bool)p );
    test_true( !!p );
    test_eq( *p, 5 );
    test_eq( *p.operator->(), 5 );
    *p = 9;
    test_eq( *p, 9 );
    test_pass("cxx11:unique_ptr");

    int* raw = p.release();
    test_true( !p );
    test_pass("cxx11:unique_ptr::release");
    delete raw;

    STD::unique_ptr<int> q(new int(1));
    q.reset(new int(2));
    test_eq( *q, 2 );
    q.reset();
    test_true( !q );
    test_pass("cxx11:unique_ptr::reset");

    STD::unique_ptr<int> a(new int(1)), b(new int(2));
    a.swap(b);
    test_eq( *a, 2 );
    test_pass("cxx11:unique_ptr::swap");
    (void)a.get_deleter();

    STD::unique_ptr<int> e1, e2;
    test_true( e1 == e2 );
    test_pass("cxx11:unique_ptr::operator==");
}

TEST_CASE(memory, shared_ptr) {
    STD::shared_ptr<int> a(new int(7));
    test_eq( *a, 7 );
    test_eq( *a.operator->(), 7 );
    test_ptr( a.get() );
    test_true( (bool)a );
    test_pass("cxx11:shared_ptr");
    test_eq( (long)a.use_count(), 1 );
    {
        STD::shared_ptr<int> b = a;
        test_eq( (long)a.use_count(), 2 );
        test_pass("cxx11:shared_ptr::use_count");
        test_eq( *b, 7 );
        test_true( a == b );
        test_true( !(a != b) );
        test_pass("cxx11:shared_ptr::operator==");
    }
    test_eq( (long)a.use_count(), 1 );

    STD::shared_ptr<int> x(new int(1)), y(new int(2));
    x.swap(y);
    test_eq( *x, 2 );
    STD::swap(x, y);
    test_eq( *x, 1 );
    test_pass("cxx11:shared_ptr::swap");
    x.reset();
    test_true( !x );
    x.reset(new int(3));
    test_eq( *x, 3 );
    test_pass("cxx11:shared_ptr::reset");

    STD::shared_ptr<int> ms = STD::make_shared<int>(11);
    test_eq( *ms, 11 );
    test_eq( (long)ms.use_count(), 1 );
    test_pass("cxx11:make_shared");

    STD::shared_ptr<int> e;
    test_true( !e );
    test_eq( (long)e.use_count(), 0 );
    test_pass("cxx11:shared_ptr (default ctor)");
}

TEST_CASE(memory, weak_ptr) {
    STD::shared_ptr<int> s(new int(5));
    STD::weak_ptr<int> w = s;
    test_true( !w.expired() );
    test_pass("cxx11:weak_ptr");
    test_eq( (long)w.use_count(), 1 );
    test_pass("cxx11:weak_ptr::use_count");

    STD::shared_ptr<int> locked = w.lock();
    test_eq( *locked, 5 );
    test_eq( (long)s.use_count(), 2 );
    test_pass("cxx11:weak_ptr::lock");

    STD::weak_ptr<int> w2 = w;
    w2.swap(w);
    test_true( !w2.expired() );
    test_pass("cxx11:weak_ptr::swap");
    w.reset();
    test_true( w.expired() );
    test_pass("cxx11:weak_ptr::reset");

    locked.reset();
    s.reset();
    test_true( w2.expired() );
    test_true( !w2.lock() );
    test_pass("cxx11:weak_ptr::expired");
}

struct Node : STD::enable_shared_from_this<Node> {
    int v;
    Node() : v(0) {}
    STD::shared_ptr<Node> self() { return shared_from_this(); }
};

struct Base { virtual ~Base() {} int b; };
struct Derived : Base { int d; };

TEST_CASE(memory, pointer_cast) {
    STD::shared_ptr<Derived> d(new Derived);
    d->b = 1; d->d = 2;
    STD::shared_ptr<Base> b = STD::static_pointer_cast<Base>(d);
    test_eq( b->b, 1 );
    test_eq( (long)d.use_count(), 2 );
    test_pass("cxx11:static_pointer_cast");

#if defined(__WATCOMC__)

    TEST_SKIP1(); TEST_SKIP1();
#else
    STD::shared_ptr<Derived> d2 = STD::dynamic_pointer_cast<Derived>(b);
    test_ptr( d2.get() );
    test_eq( d2->d, 2 );
#endif
    test_pass("cxx11:dynamic_pointer_cast");

    STD::shared_ptr<const Base> cb = b;
    STD::shared_ptr<Base> mb = STD::const_pointer_cast<Base>(cb);
    test_eq( mb->b, 1 );
    test_pass("cxx11:const_pointer_cast");

    STD::shared_ptr<int> p1(new int(1)), p2(new int(2));
    test_true( (p1 < p2) || (p2 < p1) );
    test_true( p1 <= p1 );
    test_true( p1 >= p1 );
    STD::shared_ptr<int> p1b = p1;
    test_true( p1 == p1b );
    test_true( !(p1 < p1b) && !(p1b < p1) );
    test_pass("cxx11:shared_ptr::operator<");
}

TEST_CASE(memory, enable_shared_addressof) {
    STD::shared_ptr<Node> n(new Node);
    n->v = 42;
    STD::shared_ptr<Node> s = n->self();
    test_eq( s->v, 42 );
    test_eq( (long)n.use_count(), 2 );
    test_pass("cxx11:enable_shared_from_this");

    int x = 5;
    test_true( STD::addressof(x) == &x );
    test_pass("cxx11:addressof");
}

#if TEST_TARGET_CXX >= 2014 && (!defined(_MSC_VER) || _MSC_VER >= 1600)
TEST_CASE(memory, make_unique) {
    STD::unique_ptr<int> p = STD::make_unique<int>(42);
    test_eq( *p, 42 );
    test_pass("cxx14:make_unique");

#if TEST_TARGET_CXX >= 2020

    STD::unique_ptr<int> q = STD::make_unique_for_overwrite<int>();
    test_true( q.get() != 0 );
    *q = 7;
    test_eq( *q, 7 );
    test_pass("cxx20:make_unique_for_overwrite");
#endif
}
#endif

#if TEST_TARGET_CXX >= 2017
TEST_CASE(memory, uninitialized) {
    typedef STD::string S;
    union { double _a; void* _b; unsigned char buf[sizeof(S) * 3]; } u;
    S* arr = reinterpret_cast<S*>(u.buf);

    STD::uninitialized_default_construct(arr, arr + 3);
    arr[0] = "a"; arr[1] = "bb"; arr[2] = "ccc";
    test_eq( arr[1], STD::string("bb") );
    test_pass("cxx17:uninitialized_default_construct");
    STD::destroy(arr, arr + 3);

    S* p = STD::uninitialized_value_construct_n(arr, 2);
    test_true( p == arr + 2 );
    test_eq( arr[0], STD::string() );
    test_pass("cxx17:uninitialized_value_construct_n");
    S* q = STD::destroy_n(arr, 2);
    test_true( q == arr + 2 );
    test_pass("cxx17:destroy_n");

    STD::uninitialized_default_construct_n(arr, 1);
    arr[0] = "x";
    STD::uninitialized_value_construct(arr + 1, arr + 2);
    test_eq( arr[1], STD::string() );
    test_pass("cxx17:uninitialized_value_construct");
    STD::destroy_at(arr);
    STD::destroy_at(arr + 1);
}
#endif

#if TEST_TARGET_CXX >= 2017
TEST_CASE(memory, construct) {
    typedef STD::string S;
    union { double _a; void* _b; unsigned char buf[sizeof(S)]; } u;
    S* p = reinterpret_cast<S*>(u.buf);
#if TEST_TARGET_CXX >= 2020
    S* r = STD::construct_at(p, "hi");
    test_true( r == p );
    test_pass("cxx20:construct_at");
#else
    ::new (static_cast<void*>(p)) S("hi");
    TEST_SKIP1();
    test_skip("cxx20:construct_at");
#endif
    test_eq( *p, STD::string("hi") );
    STD::destroy_at(p);
    test_pass("cxx17:destroy_at");
}
#endif

#if TEST_TARGET_CXX >= 2011
TEST_CASE(memory, pointer_traits_and_to_address) {
    typedef STD::pointer_traits<int*> PT;
    int mv = 7;
    test_true( PT::pointer_to(mv) == &mv );
    test_pass("cxx11:pointer_traits");
#if TEST_TARGET_CXX >= 2020
    test_true( STD::to_address(&mv) == &mv );
    test_pass("cxx20:to_address");
#else
    TEST_NOTE("to_address is C++20");
    test_skip("cxx20:to_address");
#endif

#if _TST_HAS_ALIAS_TEMPLATE
    typedef PT::rebind<double> DP;
#else
    typedef PT::rebind<double>::other DP;
#endif
    test_eq( sizeof(DP), sizeof(double*) );
    test_pass("cxx11:pointer_traits::rebind");
}

TEST_CASE(memory, align_and_uses_allocator) {
    char mbuf[64];
    void* mp = mbuf + 1;
    STD::size_t mspace = 63;
    void* ma = STD::align(8, 16, mp, mspace);
    test_ptr( ma );
    test_eq( (long)(reinterpret_cast<STD::size_t>(ma) % 8), 0L );
    TEST_SKIP_VC_RANGE(1600, 1800, "vc10/vc11 align() advances ptr past the block instead of to it");
    test_true( ma == mp );
    test_pass("cxx11:align");

    STD::default_delete<int> dd;
    dd(new int(3));

    bool ua = STD::uses_allocator<int, STD::allocator<int> >::value;
    test_true( ua == false );
    test_pass("cxx11:uses_allocator");
}
#endif

#if TEST_TARGET_CXX >= 2020
TEST_CASE(memory, align_tags_alloc_result) {

    STD::allocator_arg_t at = STD::allocator_arg;
    (void)at;
    test_true( sizeof(STD::allocator_arg_t) >= 1 );
    test_pass("cxx11:allocator_arg_t");

    static int sx = 0;
    int* p = &sx;
    test_true( STD::assume_aligned<4>(p) == p );
    test_pass("cxx20:assume_aligned");

#if TEST_TARGET_CXX >= 2023 && \
    (defined(__cpp_lib_allocate_at_least) || (!defined(__GNUC__) && !defined(_MSC_VER)))
    STD::allocation_result<int*, STD::size_t> ar;
    ar.ptr = p; ar.count = 5;
    test_true( ar.ptr == p );
    test_eq( (int)ar.count, 5 );
    test_pass("cxx23:allocation_result");
#else
    test_skip("cxx23:allocation_result");
#endif

#if TEST_TARGET_CXX >= 2026 && \
    (defined(__cpp_lib_is_sufficiently_aligned) || (!defined(__GNUC__) && !defined(_MSC_VER)))
    test_true( STD::is_sufficiently_aligned<1>(p) );
    test_true( STD::is_sufficiently_aligned<4>(p) );
    char cb[4];
    char* c0 = cb;
    char* codd = ((reinterpret_cast<STD::size_t>(c0) & 1) ? c0 : c0 + 1);
    test_true( !STD::is_sufficiently_aligned<2>(codd) );
    test_pass("cxx26:is_sufficiently_aligned");
#else
    test_skip("cxx26:is_sufficiently_aligned");
#endif
}

TEST_CASE(memory, allocator_traits_members) {
    typedef STD::allocator<int> A;
    typedef STD::allocator_traits<A> T;

    test_true( (STD::is_same<T::value_type, int>::value) );
    test_true( (STD::is_same<T::pointer, int*>::value) );
    test_true( (STD::is_same<T::const_pointer, const int*>::value) );
    test_true( (STD::is_same<T::void_pointer, void*>::value) );
    test_true( (STD::is_same<T::const_void_pointer, const void*>::value) );
    test_pass("cxx11:allocator_traits");

    test_true( !T::propagate_on_container_copy_assignment::value );
    test_true( !T::propagate_on_container_swap::value );
    test_pass("cxx11:allocator_traits::propagate_on_container_copy_assignment");

#if defined(__WATCOMC__) || (defined(_MSC_VER) && _MSC_VER < 1900)
    test_true( !T::propagate_on_container_move_assignment::value );
#else
    test_true( T::propagate_on_container_move_assignment::value );
#endif
    test_pass("cxx11:allocator_traits::propagate_on_container_move_assignment");

#if defined(__WATCOMC__)
    test_true( !T::is_always_equal::value );
    test_pass("cxx17:allocator_traits::is_always_equal");
#elif !defined(_MSC_VER) || defined(__cpp_lib_allocator_traits_is_always_equal)
    test_true( T::is_always_equal::value );
    test_pass("cxx17:allocator_traits::is_always_equal");
#else
    TEST_SKIP1();
    test_skip("cxx17:allocator_traits::is_always_equal");
#endif

#if !_TST_HAS_ALIAS_TEMPLATE
#if defined(_MSC_VER) && _MSC_VER >= 1600
    test_true( (STD::is_same<T::rebind_alloc<double>::other, STD::allocator<double> >::value) );
    test_true( (STD::is_same<T::rebind_traits<double>::other::value_type, double>::value) );
#else
    test_true( (STD::is_same<T::rebind_alloc<double>::type, STD::allocator<double> >::value) );
    test_true( (STD::is_same<T::rebind_traits<double>::type::value_type, double>::value) );
#endif
#else
    test_true( (STD::is_same<T::rebind_alloc<double>, STD::allocator<double> >::value) );
    test_true( (STD::is_same<T::rebind_traits<double>::value_type, double>::value) );
#endif
    test_pass("cxx11:allocator_traits::rebind_alloc");

    A a;
    T::pointer p = T::allocate(a, 3);
    test_true( p != 0 );
    test_pass("cxx11:allocator_traits::allocate");
    T::construct(a, p, 42);
    test_eq( *p, 42 );
    test_pass("cxx11:allocator_traits::construct");
    T::destroy(a, p);
    T::deallocate(a, p, 3);

    test_true( T::max_size(a) > 0 );
    test_pass("cxx11:allocator_traits::max_size");
    A a2 = T::select_on_container_copy_construction(a);
    (void)a2;

#if TEST_TARGET_CXX >= 2023
# if defined(__cpp_lib_allocate_at_least) || (!defined(__GNUC__) && !defined(_MSC_VER))

    STD::allocation_result<T::pointer, T::size_type> r = T::allocate_at_least(a, 4);
    test_true( r.ptr != 0 );
    test_true( r.count >= 4 );
    test_pass("cxx23:allocator_traits::allocate_at_least");
    T::deallocate(a, r.ptr, r.count);
# else
    TEST_SKIP1(); TEST_SKIP1();
# endif
#endif
}
#endif

TEST_CASE(memory, owner_based_ops) {
    STD::shared_ptr<int> a(new int(1));
    STD::shared_ptr<int> b = a;
    STD::shared_ptr<int> c(new int(1));
    STD::weak_ptr<int> wa(a);

    test_true( !a.owner_before(b) && !b.owner_before(a) );
    test_true(  a.owner_before(c) || c.owner_before(a) );
    test_true( !a.owner_before(wa) && !wa.owner_before(a) );
    test_pass("cxx11:shared_ptr::owner_before");

    STD::owner_less<STD::shared_ptr<int> > ol;
    test_true( !ol(a, b) && !ol(b, a) );
    test_true(  ol(a, c) || ol(c, a) );
    test_pass("cxx11:owner_less");

#if TEST_TARGET_CXX >= 2026 && \
    (defined(__cpp_lib_smart_ptr_owner_equality) || (!defined(__GNUC__) && !defined(_MSC_VER)))
    test_true(  a.owner_equal(b) );
    test_true( !a.owner_equal(c) );
    test_true(  a.owner_equal(wa) );
    test_true(  wa.owner_equal(a) );
    test_true( !wa.owner_equal(c) );
    test_pass("cxx26:shared_ptr::owner_equal");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx26:shared_ptr::owner_equal");
#endif
#if TEST_TARGET_CXX >= 2026 && \
    (defined(__cpp_lib_smart_ptr_owner_equality) || (!defined(__GNUC__) && !defined(_MSC_VER)))
    test_true( a.owner_hash() == b.owner_hash() );
    test_true( a.owner_hash() == wa.owner_hash() );
    test_true( a.owner_hash() != c.owner_hash() );
    test_pass("cxx26:shared_ptr::owner_hash");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx26:shared_ptr::owner_hash");
#endif
}

#if TEST_TARGET_CXX >= 2011
TEST_CASE(memory, unique_ptr_members) {
    STD::unique_ptr<MemFoo> d;
    test_true( !d );
    test_pass("cxx11:unique_ptr::unique_ptr() (default)");

    STD::unique_ptr<MemFoo> z(nullptr);
    test_true( !z );
    test_pass("cxx11:unique_ptr::unique_ptr(nullptr_t)");

#if !defined(__WATCOMC__)
    g_mem_del = 0;
    {
        STD::unique_ptr<MemFoo, MemDeleter> pd(new MemFoo(3), MemDeleter());
        test_eq( pd->v, 3 );
    }
    test_eq( g_mem_del, 1 );
    test_pass("cxx11:unique_ptr::unique_ptr(pointer, deleter)");
#else
    TEST_SKIP_N(2);
    test_skip("cxx11:unique_ptr::unique_ptr(pointer, deleter)");
#endif

    STD::unique_ptr<MemFoo> a(new MemFoo(5));
#if _TST_HAS_RVALUE_REF
    STD::unique_ptr<MemFoo> b(STD::move(a));
    test_true( !a );
    test_eq( b->v, 5 );
    test_pass("cxx11:unique_ptr::unique_ptr(unique_ptr&&) (move)");

    a = STD::move(b);
    test_true( !b );
    test_eq( a->v, 5 );
    test_pass("cxx11:unique_ptr::operator=(unique_ptr&&)");
#else
    STD::unique_ptr<MemFoo> b(a.release());
    test_true( !a );
    test_eq( b->v, 5 );
    test_skip("cxx11:unique_ptr::unique_ptr(unique_ptr&&) (move)");
    a.reset(b.release());
    test_true( !b );
    test_eq( a->v, 5 );
    test_skip("cxx11:unique_ptr::operator=(unique_ptr&&)");
#endif

#if !defined(__WATCOMC__)
    a = nullptr;
    test_true( !a );
    test_pass("cxx11:unique_ptr::operator=(nullptr_t)");
#else
    a.reset();
    TEST_SKIP1();
    test_skip("cxx11:unique_ptr::operator=(nullptr_t)");
#endif

    STD::unique_ptr<MemFoo> p(new MemFoo(1));
    test_true( !(p < p) );  test_pass("cxx11:unique_ptr::operator<");
    test_true(  (p <= p) ); test_pass("cxx11:unique_ptr::operator<=");
    test_true( !(p > p) );  test_pass("cxx11:unique_ptr::operator>");
    test_true(  (p >= p) ); test_pass("cxx11:unique_ptr::operator>=");

    STD::unique_ptr<int[]> arr(new int[3]);
    arr[0] = 10; arr[1] = 20; arr[2] = 30;
    test_eq( arr[1], 20 );
    test_pass("cxx11:unique_ptr<T[]>::operator[] (array specialization)");

#if defined(__cpp_lib_three_way_comparison)
    STD::unique_ptr<MemFoo> u1(new MemFoo), u2;
    test_true( (u1 <=> u1) == 0 );
    test_true( (u2 <=> u2) == 0 );
    test_pass("cxx20:operator<=>(unique_ptr)");
#else
    test_skip("cxx20:operator<=>(unique_ptr)");
#endif
}

TEST_CASE(memory, shared_ptr_ctors) {
#if _TST_HAS_NULLPTR
    STD::shared_ptr<MemFoo> z(nullptr);
    test_true( !z );
    test_pass("cxx11:shared_ptr::shared_ptr(nullptr_t)");
#else
    test_skip("cxx11:shared_ptr::shared_ptr(nullptr_t)");
#endif

    g_mem_del = 0;
    {
        STD::shared_ptr<MemFoo> pd(new MemFoo(3), MemDeleter());
        test_eq( pd->v, 3 );
    }
    test_eq( g_mem_del, 1 );
    test_pass("cxx11:shared_ptr::shared_ptr(pointer, deleter)");

    STD::shared_ptr<MemDer> owner(new MemDer());
    owner->b = 42;
    STD::shared_ptr<int> alias(owner, &owner->b);
    test_eq( *alias, 42 );
    test_eq( alias.use_count(), owner.use_count() );
    test_pass("cxx11:shared_ptr::shared_ptr(aliasing ctor)");

    STD::shared_ptr<MemFoo> sp(new MemFoo(9));
    STD::weak_ptr<MemFoo> w = sp;
    STD::shared_ptr<MemFoo> s2(w);
    test_eq( s2->v, 9 );
    test_pass("cxx11:shared_ptr::shared_ptr(const weak_ptr&)");

#if _TST_HAS_RVALUE_REF
    STD::shared_ptr<MemFoo> ma(new MemFoo(5));
    STD::shared_ptr<MemFoo> mb(STD::move(ma));
    test_true( !ma );
    test_eq( mb->v, 5 );
    test_pass("cxx11:shared_ptr::shared_ptr(shared_ptr&&) (move)");
#else
    TEST_SKIP_N(2);
    test_skip("cxx11:shared_ptr::shared_ptr(shared_ptr&&) (move)");
#endif

#if _TST_HAS_RVALUE_REF
    STD::unique_ptr<MemFoo> u(new MemFoo(7));
    STD::shared_ptr<MemFoo> su(STD::move(u));
    test_true( !u );
    test_eq( su->v, 7 );
    test_pass("cxx11:shared_ptr::shared_ptr(unique_ptr&&)");
#else
    TEST_SKIP_N(2);
    test_skip("cxx11:shared_ptr::shared_ptr(unique_ptr&&)");
#endif

    STD::shared_ptr<MemFoo> pd(new MemFoo, MemDeleter());
    test_ptr( STD::get_deleter<MemDeleter>(pd) );
    test_true( STD::get_deleter<STD::default_delete<MemFoo> >(pd) == 0 );
    test_pass("cxx11:get_deleter(shared_ptr)");

    STD::ostringstream os;
    STD::shared_ptr<MemFoo> pr(new MemFoo(1));
    os << pr;
    test_true( !os.str().empty() );
    test_pass("cxx11:operator<<(ostream, shared_ptr)");

    test_skip("cxx11:shared_ptr::unique (deprecated C++17, removed C++20)");

#if defined(__cpp_lib_three_way_comparison)
    STD::shared_ptr<MemFoo> c1(new MemFoo), c0;
    test_true( (c1 <=> c1) == 0 );
    test_true( (c0 <=> c0) == 0 );
    test_pass("cxx20:operator<=>(shared_ptr)");
#else
    test_skip("cxx20:operator<=>(shared_ptr)");
#endif
}

#if TEST_HAS_EH
TEST_CASE(memory, weak_ptr_members) {
    STD::weak_ptr<MemFoo> w;
    test_eq( (int)w.use_count(), 0 );
    test_pass("cxx11:weak_ptr::weak_ptr() (default)");

    STD::shared_ptr<MemFoo> sp(new MemFoo(1));
    w = sp;
    test_eq( (int)w.use_count(), (int)sp.use_count() );
    test_pass("cxx11:weak_ptr::operator=");

    STD::shared_ptr<MemFoo> sp2(new MemFoo(2));
    STD::weak_ptr<MemFoo> w2 = sp2;
    test_true( w.owner_before(w2) != w2.owner_before(w) );
    test_true( !w.owner_before(sp) && !w.owner_before(w) );
    test_pass("cxx11:weak_ptr::owner_before");

#if TEST_TARGET_CXX >= 2017
    STD::owner_less<void> ol;
    test_true( ol(sp, sp2) != ol(sp2, sp) );
    test_true( !ol(sp, sp) );
    test_pass("cxx17:owner_less<void>");
#else
    test_skip("cxx17:owner_less<void>");
#endif

    STD::weak_ptr<MemFoo> we;
    { STD::shared_ptr<MemFoo> tmp(new MemFoo(3)); we = tmp; }
    bool threw = false;
    try { STD::shared_ptr<MemFoo> boom(we); }
    catch (const STD::bad_weak_ptr&) { threw = true; }
    test_true( threw );
    test_pass("cxx11:bad_weak_ptr");
}
#else   // !TEST_HAS_EH
TEST_CASE(memory, weak_ptr_members) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx11:weak_ptr::weak_ptr() (default)");
    test_skip("cxx11:weak_ptr::operator=");
    test_skip("cxx11:weak_ptr::owner_before");
    test_skip("cxx17:owner_less<void>");
    test_skip("cxx11:bad_weak_ptr");
}
#endif  // TEST_HAS_EH

TEST_CASE(memory, allocator_and_uninitialized) {

    test_skip("cxx03:allocator::construct");
    test_skip("cxx03:allocator::destroy");
    test_skip("cxx11:allocator::max_size");

    MemFoo src[3];
    src[0] = MemFoo(1); src[1] = MemFoo(2); src[2] = MemFoo(3);
    STD::allocator<MemFoo> ab;
    MemFoo* dst = ab.allocate(3);

    STD::uninitialized_copy(src, src + 3, dst);
    test_eq( dst[1].v, 2 );
    test_pass("cxx03:uninitialized_copy");
    STD::uninitialized_fill(dst, dst + 3, MemFoo(9));
    test_eq( dst[0].v, 9 );
    test_pass("cxx03:uninitialized_fill");
    STD::uninitialized_fill_n(dst, 3, MemFoo(8));
    test_eq( dst[2].v, 8 );
    test_pass("cxx03:uninitialized_fill_n");

#if TEST_TARGET_CXX >= 2017
    STD::uninitialized_copy_n(src, 3, dst);
    test_eq( dst[2].v, 3 );
    test_pass("cxx17:uninitialized_copy_n");
    STD::uninitialized_move(src, src + 3, dst);
    test_eq( dst[0].v, 1 );
    test_pass("cxx17:uninitialized_move");
    STD::uninitialized_move_n(src, 3, dst);
    test_eq( dst[1].v, 2 );
    test_pass("cxx17:uninitialized_move_n");
#else
    test_skip("cxx17:uninitialized_copy_n");
    test_skip("cxx17:uninitialized_move");
    test_skip("cxx17:uninitialized_move_n");
#endif
    ab.deallocate(dst, 3);

    STD::shared_ptr<MemFoo> asp = STD::allocate_shared<MemFoo>(STD::allocator<MemFoo>(), 5);
    test_eq( asp->v, 5 );
    test_pass("cxx11:allocate_shared");

#if _TST_HAS_RVALUE_REF && TEST_TARGET_CXX >= 2014
    STD::unique_ptr<int[]> ua = STD::make_unique<int[]>(4);
    ua[3] = 7;
    test_eq( ua[3], 7 );
    test_pass("cxx14:make_unique (array form)");
#else
    test_skip("cxx14:make_unique (array form)");
#endif

    STD::default_delete<int[]> dd;
    dd(new int[5]);
    test_true( true );
    test_pass("cxx11:default_delete<T[]>");

#if TEST_TARGET_CXX >= 2017

    STD::shared_ptr<int> pi(new int(0x41424344));
    STD::shared_ptr<unsigned> pu = STD::reinterpret_pointer_cast<unsigned>(pi);
    test_eq( pu.use_count(), pi.use_count() );
    test_pass("cxx17:reinterpret_pointer_cast");
#else
    test_skip("cxx17:reinterpret_pointer_cast");
#endif
}

#if TEST_TARGET_CXX >= 2017 && (!defined(_MSC_VER) || _MSC_VER >= 1910)
struct MemESFT : STD::enable_shared_from_this<MemESFT> { int v; };
TEST_CASE(memory, weak_from_this) {
    STD::shared_ptr<MemESFT> sp(new MemESFT());
    sp->v = 5;
    STD::weak_ptr<MemESFT> w = sp->weak_from_this();
    test_eq( w.lock()->v, 5 );
    test_pass("cxx17:enable_shared_from_this::weak_from_this");
}
#else
TEST_CASE(memory, weak_from_this) {
    TEST_NOTE("weak_from_this is a member of the native enable_shared_from_this; MSVC ships it from VS2017");
    test_skip("cxx17:enable_shared_from_this::weak_from_this");
}
#endif

TEST_CASE(memory, cxx20_memory) {
#if defined(__cpp_lib_smart_ptr_for_overwrite)
    STD::shared_ptr<int> so = STD::make_shared_for_overwrite<int>();
    test_ptr( so.get() );
    test_pass("cxx20:make_shared_for_overwrite");
    STD::shared_ptr<int> ao = STD::allocate_shared_for_overwrite<int>(STD::allocator<int>());
    test_ptr( ao.get() );
    test_pass("cxx20:allocate_shared_for_overwrite");
#else
    test_skip("cxx20:make_shared_for_overwrite");
    test_skip("cxx20:allocate_shared_for_overwrite");
#endif

#if defined(__cpp_lib_ranges)
    STD::allocator<MemFoo> ral;
    MemFoo* one = ral.allocate(1);
    STD::ranges::construct_at(one, 11);
    test_eq( one->v, 11 );
    test_pass("cxx20:ranges::construct_at");
    STD::ranges::destroy_at(one);
    test_true( true );
    test_pass("cxx20:ranges::destroy_at");
    ral.deallocate(one, 1);
    MemFoo arr2[2];
    STD::ranges::destroy(arr2, arr2 + 2);
    test_true( true );
    test_pass("cxx20:ranges::destroy");
#else
    test_skip("cxx20:ranges::construct_at");
    test_skip("cxx20:ranges::destroy_at");
    test_skip("cxx20:ranges::destroy");
#endif
}

TEST_CASE(memory, ranges_uninitialized_cxx20) {
#if defined(__cpp_lib_ranges)
    STD::allocator<MemFoo> al;
    MemFoo src[3];
    src[0] = MemFoo(1); src[1] = MemFoo(2); src[2] = MemFoo(3);
    MemFoo* d = al.allocate(3);

    STD::ranges::uninitialized_copy(src, src + 3, d, d + 3);
    test_eq( d[0].v, 1 );
    test_eq( d[2].v, 3 );
    STD::ranges::destroy(d, d + 3);
    test_pass("cxx20:ranges::uninitialized_copy");

    STD::ranges::uninitialized_copy_n(src, 2, d, d + 3);
    test_eq( d[1].v, 2 );
    STD::ranges::destroy(d, d + 2);
    test_pass("cxx20:ranges::uninitialized_copy_n");

    STD::ranges::uninitialized_fill(d, d + 3, MemFoo(7));
    test_eq( d[0].v, 7 );
    test_eq( d[2].v, 7 );
    STD::ranges::destroy(d, d + 3);
    test_pass("cxx20:ranges::uninitialized_fill");

    STD::ranges::uninitialized_fill_n(d, 2, MemFoo(8));
    test_eq( d[1].v, 8 );
    STD::ranges::destroy(d, d + 2);
    test_pass("cxx20:ranges::uninitialized_fill_n");

    STD::ranges::uninitialized_move(src, src + 3, d, d + 3);
    test_eq( d[2].v, 3 );
    STD::ranges::destroy(d, d + 3);
    test_pass("cxx20:ranges::uninitialized_move");

    STD::ranges::uninitialized_move_n(src, 2, d, d + 3);
    test_eq( d[0].v, 1 );
    STD::ranges::destroy(d, d + 2);
    test_pass("cxx20:ranges::uninitialized_move_n");

    MemFoo* e = STD::ranges::uninitialized_default_construct(d, d + 3);
    test_true( e == d + 3 );
    STD::ranges::destroy(d, d + 3);
    test_pass("cxx20:ranges::uninitialized_default_construct");

    MemFoo* e2 = STD::ranges::uninitialized_default_construct_n(d, 2);
    test_true( e2 == d + 2 );
    STD::ranges::destroy(d, d + 2);
    test_pass("cxx20:ranges::uninitialized_default_construct_n");

    d[0].v = 55; d[1].v = 55; d[2].v = 55;
    STD::ranges::uninitialized_value_construct(d, d + 3);
    test_eq( d[0].v, 0 );
    test_eq( d[2].v, 0 );
    STD::ranges::destroy(d, d + 3);
    test_pass("cxx20:ranges::uninitialized_value_construct");

    d[0].v = 55; d[1].v = 55;
    STD::ranges::uninitialized_value_construct_n(d, 2);
    test_eq( d[1].v, 0 );
    STD::ranges::destroy(d, d + 2);
    test_pass("cxx20:ranges::uninitialized_value_construct_n");

    al.deallocate(d, 3);
#else
    test_skip("cxx20:ranges::uninitialized_copy");
    test_skip("cxx20:ranges::uninitialized_copy_n");
    test_skip("cxx20:ranges::uninitialized_fill");
    test_skip("cxx20:ranges::uninitialized_fill_n");
    test_skip("cxx20:ranges::uninitialized_move");
    test_skip("cxx20:ranges::uninitialized_move_n");
    test_skip("cxx20:ranges::uninitialized_default_construct");
    test_skip("cxx20:ranges::uninitialized_default_construct_n");
    test_skip("cxx20:ranges::uninitialized_value_construct");
    test_skip("cxx20:ranges::uninitialized_value_construct_n");
#endif
}

TEST_CASE(memory, uninitialized_results_cxx20) {
#if defined(__cpp_lib_ranges)
    STD::allocator<MemFoo> al;
    MemFoo src[2];
    src[0] = MemFoo(1); src[1] = MemFoo(2);
    MemFoo* d = al.allocate(2);

    STD::ranges::uninitialized_copy_result<MemFoo*, MemFoo*> rc =
        STD::ranges::uninitialized_copy(src, src + 2, d, d + 2);
    test_true( rc.in == src + 2 );
    test_true( rc.out == d + 2 );
    STD::ranges::destroy(d, d + 2);
    test_pass("cxx20:uninitialized_copy_result");

    STD::ranges::uninitialized_copy_n_result<MemFoo*, MemFoo*> rcn =
        STD::ranges::uninitialized_copy_n(src, 2, d, d + 2);
    test_true( rcn.in == src + 2 );
    test_true( rcn.out == d + 2 );
    STD::ranges::destroy(d, d + 2);
    test_pass("cxx20:uninitialized_copy_n_result");

    STD::ranges::uninitialized_move_result<MemFoo*, MemFoo*> rm =
        STD::ranges::uninitialized_move(src, src + 2, d, d + 2);
    test_true( rm.in == src + 2 );
    test_true( rm.out == d + 2 );
    STD::ranges::destroy(d, d + 2);
    test_pass("cxx20:uninitialized_move_result");

    STD::ranges::uninitialized_move_n_result<MemFoo*, MemFoo*> rmn =
        STD::ranges::uninitialized_move_n(src, 2, d, d + 2);
    test_true( rmn.in == src + 2 );
    test_true( rmn.out == d + 2 );
    STD::ranges::destroy(d, d + 2);
    test_pass("cxx20:uninitialized_move_n_result");

    al.deallocate(d, 2);
#else
    test_skip("cxx20:uninitialized_copy_result");
    test_skip("cxx20:uninitialized_copy_n_result");
    test_skip("cxx20:uninitialized_move_result");
    test_skip("cxx20:uninitialized_move_n_result");
#endif
}

TEST_CASE(memory, cxx23_memory) {
#if defined(__cpp_lib_out_ptr)
    STD::unique_ptr<MemFoo> up;
    {
        STD::out_ptr_t<STD::unique_ptr<MemFoo>, MemFoo*> op = STD::out_ptr(up);
        MemFoo** raw = op;
        *raw = new MemFoo(21);
    }
    test_eq( up->v, 21 );
    test_pass("cxx23:out_ptr");
    test_pass("cxx23:out_ptr_t");

    {
        STD::inout_ptr_t<STD::unique_ptr<MemFoo>, MemFoo*> ip = STD::inout_ptr(up);
        MemFoo** raw = ip;
        delete *raw;
        *raw = new MemFoo(22);
    }
    test_eq( up->v, 22 );
    test_pass("cxx23:inout_ptr");
    test_pass("cxx23:inout_ptr_t");
#else
    test_skip("cxx23:out_ptr");
    test_skip("cxx23:out_ptr_t");
    test_skip("cxx23:inout_ptr");
    test_skip("cxx23:inout_ptr_t");
#endif

#if defined(__cpp_lib_allocate_at_least)
    STD::allocator<int> al;
    STD::allocation_result<int*> ar = al.allocate_at_least(4);
    test_true( ar.count >= 4 );
    al.deallocate(ar.ptr, ar.count);
    test_pass("cxx23:allocator::allocate_at_least");
#else
    test_skip("cxx23:allocator::allocate_at_least");
#endif
}
#endif

TEST_CASE(memory, feature_test_macros) {
#if defined(__cpp_lib_constexpr_memory) && (__cpp_lib_constexpr_memory) >= 202506L
    test_true( (__cpp_lib_constexpr_memory) >= 202506L );
    test_pass("cxx26:__cpp_lib_constexpr_memory");
#else
    test_skip("cxx26:__cpp_lib_constexpr_memory");
#endif
}

TEST_CASE(memory, allocator_members_cxx03) {
    typedef STD::allocator<MemFoo> A;
    A a;

    A::value_type v(7);
    test_eq( v.v, 7 );
    test_pass("cxx03:allocator::value_type");

    A::size_type n = 3;
    MemFoo* p = a.allocate(n);
    test_ptr( p );
    test_pass("cxx03:allocator::size_type");

    A::difference_type d = (p + 3) - p;
    test_eq( (int)d, 3 );
    test_true( (A::difference_type)(d - 4) < 0 );
    test_pass("cxx03:allocator::difference_type");

    a.deallocate(p, n);
    MemFoo* p2 = a.allocate(n);
    test_ptr( p2 );
    a.deallocate(p2, n);
    test_pass("cxx03:allocator::deallocate");

#if !_TST_HAS_CXX20_LIB_MEMBERS || defined(__WATCOMC__)
    MemFoo one(1);
    A::reference r = one;
    r.v = 4;
    test_eq( one.v, 4 );
    test_pass("cxx03:allocator::reference");

    A::const_reference cr = one;
    test_eq( cr.v, 4 );
    test_pass("cxx03:allocator::const_reference");

    A::pointer ap = a.address(r);
    test_true( ap == &one );
    test_pass("cxx03:allocator::pointer");

    A::const_pointer cap = a.address(cr);
    test_true( cap == &one );
    test_pass("cxx03:allocator::const_pointer");

    test_true( a.address(one) == &one );
    test_pass("cxx03:allocator::address");

    typedef A::rebind<double>::other DA;
    DA da;
    double* dp = da.allocate(2);
    test_ptr( dp );
    da.deallocate(dp, 2);
    test_pass("cxx03:allocator::rebind");
#else
    test_skip("cxx03:allocator::reference");
    test_skip("cxx03:allocator::const_reference");
    test_skip("cxx03:allocator::pointer");
    test_skip("cxx03:allocator::const_pointer");
    test_skip("cxx03:allocator::address");
    test_skip("cxx03:allocator::rebind");
#endif

#if ((!_TST_HAS_CXX20_LIB_MEMBERS) || defined(__GLIBCXX__)) && !defined(__WATCOMC__)
    STD::pair<MemFoo*, STD::ptrdiff_t> buf = STD::get_temporary_buffer<MemFoo>(4);
    test_true( buf.second >= 0 );
    test_true( buf.second == 0 || buf.first != 0 );
    if (buf.first) STD::return_temporary_buffer(buf.first);
    TEST_NOTE("read as get_temporary_buffer / return_temporary_buffer");
    test_pass("cxx03:buffers");
#else
    test_skip("cxx03:buffers");
#endif
}

#if TEST_TARGET_CXX >= 2011
TEST_CASE(memory, allocator_traits_types_cxx11) {
    typedef STD::allocator<MemFoo> A;
    typedef STD::allocator_traits<A> T;
    A a;

    T::allocator_type at = a;
    MemFoo* q = at.allocate(1);
    test_ptr( q );
    at.deallocate(q, 1);
    test_pass("cxx11:allocator_traits::allocator_type");

    T::value_type v(6);
    test_eq( v.v, 6 );
    test_pass("cxx11:allocator_traits::value_type");

    T::size_type n = 2;
    T::pointer p = T::allocate(a, n);
    test_ptr( p );
    test_pass("cxx11:allocator_traits::size_type");

    T::const_pointer cp = p;
    test_true( cp == p );
    test_pass("cxx11:allocator_traits::const_pointer");

    T::void_pointer vp = p;
    test_true( vp == static_cast<void*>(p) );
    test_pass("cxx11:allocator_traits::void_pointer");

    T::const_void_pointer cvp = cp;
    test_true( cvp == static_cast<const void*>(p) );
    test_pass("cxx11:allocator_traits::const_void_pointer");

    T::difference_type d = (p + 2) - p;
    test_eq( (int)d, 2 );
    test_true( (T::difference_type)(d - 3) < 0 );
    test_pass("cxx11:allocator_traits::difference_type");

    T::deallocate(a, p, n);
    T::pointer p2 = T::allocate(a, n);
    test_ptr( p2 );
    T::deallocate(a, p2, n);
    test_pass("cxx11:allocator_traits::deallocate");

    test_true( !T::propagate_on_container_swap::value );
    test_pass("cxx11:allocator_traits::propagate_on_container_swap");

    A sel = T::select_on_container_copy_construction(a);
    MemFoo* s = sel.allocate(1);
    test_ptr( s );
    sel.deallocate(s, 1);
    test_true( sel == a );
    test_pass("cxx11:allocator_traits::select_on_container_copy_construction");
}
#else
TEST_CASE_SKIP(memory, allocator_traits_types_cxx11)
#endif

TEST_CASE(memory, smart_ptr_members_cxx11) {
    STD::shared_ptr<MemFoo> sp(new MemFoo(3));

    STD::shared_ptr<MemFoo>::element_type se = *sp;
    test_eq( se.v, 3 );
    test_pass("cxx11:shared_ptr::element_type");

    test_true( sp.get() == &*sp );
    test_eq( sp.get()->v, 3 );
    test_pass("cxx11:shared_ptr::get");

    test_eq( (*sp).v, 3 );
    (*sp).v = 8;
    test_eq( sp.get()->v, 8 );
    test_pass("cxx11:shared_ptr::operator*");

    test_eq( sp->v, 8 );
    sp->v = 3;
    test_eq( (*sp).v, 3 );
    test_pass("cxx11:shared_ptr::operator->");

    STD::weak_ptr<MemFoo> wp(sp);
    STD::weak_ptr<MemFoo>::element_type we = *wp.lock();
    test_eq( we.v, 3 );
    test_pass("cxx11:weak_ptr::element_type");

    STD::unique_ptr<MemFoo> up(new MemFoo(4));

    STD::unique_ptr<MemFoo>::element_type ue = *up;
    test_eq( ue.v, 4 );
    test_pass("cxx11:unique_ptr::element_type");

    test_true( up.get() == &*up );
    test_eq( up.get()->v, 4 );
    test_pass("cxx11:unique_ptr::get");

    test_eq( (*up).v, 4 );
    (*up).v = 9;
    test_eq( up.get()->v, 9 );
    test_pass("cxx11:unique_ptr::operator*");

    test_eq( up->v, 9 );
    up->v = 4;
    test_eq( (*up).v, 4 );
    test_pass("cxx11:unique_ptr::operator->");

    g_mem_del = 0;
#if !defined(__WATCOMC__)
    {
        STD::unique_ptr<MemFoo>::deleter_type dt;
        dt(new MemFoo(1));
        test_eq( g_mem_del, 0 );
    }
    test_pass("cxx11:unique_ptr::deleter_type");
#else
    test_skip("cxx11:unique_ptr::deleter_type");
#endif

    g_mem_dtor = 0;
    STD::default_delete<MemCount> dd;
    dd(new MemCount);
    test_eq( g_mem_dtor, 1 );
    dd(new MemCount);
    test_eq( g_mem_dtor, 2 );
    test_pass("cxx11:default_delete::operator()");
}

TEST_CASE(memory, shared_from_this_and_hash_cxx11) {
    STD::shared_ptr<Node> n(new Node);
    n->v = 11;
    STD::shared_ptr<Node> s = n->shared_from_this();
    test_true( s.get() == n.get() );
    test_eq( (long)n.use_count(), 2L );
    test_pass("cxx11:enable_shared_from_this::shared_from_this");

#if !defined(__WATCOMC__)
    STD::shared_ptr<MemFoo> a(new MemFoo(1));
    STD::shared_ptr<MemFoo> b = a;
    STD::shared_ptr<MemFoo> c(new MemFoo(1));
    STD::hash<STD::shared_ptr<MemFoo> > hs;
    test_eq( hs(a), hs(b) );
    test_true( hs(a) != hs(c) || a.get() == c.get() );
    STD::shared_ptr<MemFoo> e;
    test_eq( hs(e), STD::hash<MemFoo*>()(static_cast<MemFoo*>(0)) );
    test_pass("cxx11:hash");
#else
    test_skip("cxx11:hash");
#endif
}

#if defined(_MSVC_LANG)
#  define _TST_MEM_LANG _MSVC_LANG
#elif defined(__WATCOMC__)
#  define _TST_MEM_LANG 199711L
#else
#  define _TST_MEM_LANG __cplusplus
#endif

TEST_CASE(memory, allocator_is_always_equal_cxx11) {
#if _TST_MEM_LANG < 202400L && !defined(__WATCOMC__) && _TST_HAS_CXX11_LIB_MEMBERS \
    && (!defined(_MSC_VER) || _TST_HAS_CXX14_LIB_MEMBERS)
    test_true( STD::allocator<int>::is_always_equal::value );
    STD::allocator<int> a, b;
    test_true( a == b );
    test_pass("cxx11:allocator::is_always_equal");
#else
    test_skip("cxx11:allocator::is_always_equal");
#endif
}

#if !defined(__WATCOMC__) && (!defined(_MSC_VER) || _MSC_VER >= 1900) && \
    _TST_HAS_CXX11_LIB_MEMBERS
TEST_CASE(memory, atomic_shared_ptr_free_cxx11) {
    STD::shared_ptr<int> p(new int(1));
    STD::shared_ptr<int> q(new int(2));

    bool lf = STD::atomic_is_lock_free(&p);
    test_true( lf == true || lf == false );
    test_pass("cxx11:atomic_is_lock_free(shared_ptr)");

    STD::shared_ptr<int> r = STD::atomic_load(&p);
    test_eq( *r, 1 );
    test_true( r.get() == p.get() );
    test_pass("cxx11:atomic_load(shared_ptr)");

    STD::atomic_store(&p, q);
    STD::shared_ptr<int> after_store = STD::atomic_load(&p);
    test_eq( *after_store, 2 );
    test_pass("cxx11:atomic_store(shared_ptr)");

    STD::shared_ptr<int> old = STD::atomic_exchange(&p, r);
    test_eq( *old, 2 );
    STD::shared_ptr<int> after_xchg = STD::atomic_load(&p);
    test_eq( *after_xchg, 1 );
    test_pass("cxx11:atomic_exchange(shared_ptr)");

    STD::shared_ptr<int> expected = STD::atomic_load(&p);
    bool ok = false;
    for (int i = 0; i < 100 && !ok; ++i)
        ok = STD::atomic_compare_exchange_weak(&p, &expected, q);
    test_true( ok );
    STD::shared_ptr<int> after_weak = STD::atomic_load(&p);
    test_eq( *after_weak, 2 );
    test_pass("cxx11:atomic_compare_exchange_weak(shared_ptr)");

    STD::shared_ptr<int> wrong(new int(9));
    STD::shared_ptr<int> exp2 = wrong;
    bool bad = STD::atomic_compare_exchange_strong(&p, &exp2, r);
    test_true( !bad );
    test_true( exp2.get() == q.get() );
    bool good = STD::atomic_compare_exchange_strong(&p, &exp2, r);
    test_true( good );
    STD::shared_ptr<int> after_strong = STD::atomic_load(&p);
    test_eq( *after_strong, 1 );
    test_pass("cxx11:atomic_compare_exchange_strong(shared_ptr)");
}
#else
TEST_CASE(memory, atomic_shared_ptr_free_cxx11) {
    test_skip("cxx11:atomic_is_lock_free(shared_ptr)");
    test_skip("cxx11:atomic_load(shared_ptr)");
    test_skip("cxx11:atomic_store(shared_ptr)");
    test_skip("cxx11:atomic_exchange(shared_ptr)");
    test_skip("cxx11:atomic_compare_exchange_weak(shared_ptr)");
    test_skip("cxx11:atomic_compare_exchange_strong(shared_ptr)");
}
#endif

#if defined(__cpp_lib_atomic_shared_ptr)
TEST_CASE(memory, atomic_smart_ptr_cxx20) {
    STD::shared_ptr<int> p(new int(4));
    STD::atomic<STD::shared_ptr<int> > as(p);
    STD::shared_ptr<int> l1 = as.load();
    test_eq( *l1, 4 );
    STD::shared_ptr<int> q(new int(5));
    as.store(q);
    STD::shared_ptr<int> l2 = as.load();
    test_eq( *l2, 5 );
    STD::shared_ptr<int> old = as.exchange(p);
    test_eq( *old, 5 );
    STD::shared_ptr<int> l3 = as.load();
    test_eq( *l3, 4 );
    test_pass("cxx20:atomic<shared_ptr>");

    STD::atomic<STD::weak_ptr<int> > aw(p);
    STD::shared_ptr<int> w1 = aw.load().lock();
    test_eq( *w1, 4 );
    aw.store(q);
    STD::shared_ptr<int> w2 = aw.load().lock();
    test_true( w2.get() == q.get() );
    test_pass("cxx20:atomic<weak_ptr>");
}
#else
TEST_CASE(memory, atomic_smart_ptr_cxx20) {
    test_skip("cxx20:atomic<shared_ptr>");
    test_skip("cxx20:atomic<weak_ptr>");
}
#endif

TEST_CASE(memory, cxx17_additions) {
#if _TST_HAS_CXX17_LIB_MEMBERS && !defined(__WATCOMC__)
    STD::shared_ptr<MemFoo> sp(new MemFoo(2));
    STD::shared_ptr<MemFoo>::weak_type w = sp;
    test_eq( (long)w.use_count(), 1L );
    test_eq( w.lock()->v, 2 );
    sp.reset();
    test_true( w.expired() );
    test_pass("cxx17:shared_ptr::weak_type");
#else
    test_skip("cxx17:shared_ptr::weak_type");
#endif

#if _TST_HAS_VAR_TEMPLATE && TEST_TARGET_CXX >= 2017
    bool uv = STD::uses_allocator_v<int, STD::allocator<int> >;
    bool ut = STD::uses_allocator<int, STD::allocator<int> >::value;
    test_true( uv == false );
    test_true( uv == ut );
    test_pass("cxx17:uses_allocator_v");
#else
    test_skip("cxx17:uses_allocator_v");
#endif

#if TEST_TARGET_CXX >= 2017
    STD::allocator<STD::string> sal;
    STD::string* sb = sal.allocate(3);
    STD::string* end = STD::uninitialized_default_construct_n(sb, 3);
    test_true( end == sb + 3 );
    sb[2] = "zz";
    test_eq( sb[2], STD::string("zz") );
    STD::destroy_n(sb, 3);
    sal.deallocate(sb, 3);
    test_pass("cxx17:uninitialized_default_construct_n");
#else
    test_skip("cxx17:uninitialized_default_construct_n");
#endif
}

#if TEST_HAS_EH
TEST_CASE(memory, cxx20_additions) {
    STD::bad_weak_ptr ex;
    const char* msg = ex.what();
    test_ptr( (void*)msg );
    test_true( msg[0] != 0 );
    bool threw = false;
    STD::weak_ptr<MemFoo> we;
    { STD::shared_ptr<MemFoo> tmp(new MemFoo(1)); we = tmp; }
    try { STD::shared_ptr<MemFoo> boom(we); }
    catch (const STD::bad_weak_ptr& e) { threw = (e.what()[0] != 0); }
    test_true( threw );
    test_pass("cxx20:bad_weak_ptr::what");

#if defined(__cpp_lib_make_obj_using_allocator)
    typedef STD::pair<int, int> PI;
    STD::allocator<int> al;

    PI made = STD::make_obj_using_allocator<PI>(al, 1, 2);
    test_eq( made.first, 1 );
    test_eq( made.second, 2 );
    test_pass("cxx20:make_obj_using_allocator");

    STD::tuple<int&&> args = STD::uses_allocator_construction_args<int>(al, 3);
    int arg0 = STD::get<0>(args);
    test_eq( arg0, 3 );
    test_pass("cxx20:uses_allocator_construction_args");

    void* raw = ::operator new(sizeof(PI));
    PI* q = STD::uninitialized_construct_using_allocator(static_cast<PI*>(raw), al, 5, 6);
    test_true( q == raw );
    test_eq( q->first, 5 );
    test_eq( q->second, 6 );
    q->~PI();
    ::operator delete(raw);
    test_pass("cxx20:uninitialized_construct_using_allocator");
#else
    test_skip("cxx20:make_obj_using_allocator");
    test_skip("cxx20:uses_allocator_construction_args");
    test_skip("cxx20:uninitialized_construct_using_allocator");
#endif
}
#else   // !TEST_HAS_EH
TEST_CASE(memory, cxx20_additions) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx20:bad_weak_ptr::what");
    test_skip("cxx20:make_obj_using_allocator");
    test_skip("cxx20:uses_allocator_construction_args");
    test_skip("cxx20:uninitialized_construct_using_allocator");
}
#endif  // TEST_HAS_EH

TEST_CASE(memory, cxx23_start_lifetime_as) {
#if defined(__cpp_lib_start_lifetime_as)
    alignas(int) unsigned char buf[sizeof(int) * 2];
    int* p = STD::start_lifetime_as<int>(buf);
    *p = 7;
    test_eq( *p, 7 );
    test_pass("cxx23:start_lifetime_as");

    int* a = STD::start_lifetime_as_array<int>(buf, 2);
    a[1] = 9;
    test_eq( a[1], 9 );
    test_pass("cxx23:start_lifetime_as_array");
#else
    TEST_NOTE("no library provides start_lifetime_as yet (libstdc++ 15.2, MSVC 14.5, libcxx03)");
    test_skip("cxx23:start_lifetime_as");
    test_skip("cxx23:start_lifetime_as_array");
#endif
}

TEST_CASE(memory, cxx26_additions) {
#if defined(__cpp_lib_indirect)
    STD::indirect<int> ind(3);
    test_eq( *ind, 3 );
    test_pass("cxx26:indirect");
#else
    TEST_NOTE("std::indirect (P3019) is in no library yet");
    test_skip("cxx26:indirect");
#endif

#if defined(__cpp_lib_polymorphic)
    STD::polymorphic<int> poly(4);
    test_eq( *poly, 4 );
    test_pass("cxx26:polymorphic");
#else
    TEST_NOTE("std::polymorphic (P3019) is in no library yet");
    test_skip("cxx26:polymorphic");
#endif

    TEST_NOTE("pointer tagging (N5054 working draft) is in no library yet");
    test_skip("cxx26:pointer_tag_pair");
    test_skip("cxx26:pointer_bits_available");
    test_skip("cxx26:max_pointer_bits_available");
    test_skip("cxx26:tuple_size");
    test_skip("cxx26:tuple_element");
    test_skip("cxx26:get");
    test_skip("cxx26:start_lifetime");
}

static int g_mm_live = 0;

struct MmLive {
    char* p_;
    MmLive() : p_(new char[32]) { p_[0] = 0; ++g_mm_live; }
    MmLive(char c) : p_(new char[32]) { p_[0] = c; p_[1] = 0; ++g_mm_live; }
    MmLive(const MmLive& o) : p_(new char[32]) {
        for (int i = 0; i < 32; ++i) p_[i] = o.p_[i];
        ++g_mm_live;
    }
    MmLive& operator=(const MmLive& o) {
        if (this != &o) for (int i = 0; i < 32; ++i) p_[i] = o.p_[i];
        return *this;
    }
    ~MmLive() { delete[] p_; --g_mm_live; }
    char tag() const { return p_[0]; }
    bool operator==(const MmLive& o) const { return p_[0] == o.p_[0]; }
    bool operator< (const MmLive& o) const { return p_[0] <  o.p_[0]; }
};

TEST_CASE(memory, uninitialized_with_class_elements) {
    g_mm_live = 0;
    {
        STD::allocator<MmLive> al;
        MmLive* raw = al.allocate(4);
        test_eq( g_mm_live, 0 );

        {
            MmLive seed('s');
            STD::uninitialized_fill_n(raw, 4, seed);
            test_eq( g_mm_live, 5 );
        }
        test_eq( g_mm_live, 4 );
        test_eq( raw[0].tag(), 's' );
        test_eq( raw[3].tag(), 's' );

        MmLive src[3];
        src[0] = MmLive('a'); src[1] = MmLive('b'); src[2] = MmLive('c');
        test_eq( g_mm_live, 7 );

        MmLive* raw2 = al.allocate(3);
        STD::uninitialized_copy(src, src + 3, raw2);
        test_eq( g_mm_live, 10 );
        test_eq( raw2[2].tag(), 'c' );

        for (int i = 0; i < 3; ++i) raw2[i].~MmLive();
        test_eq( g_mm_live, 7 );
        al.deallocate(raw2, 3);

        MmLive* raw3 = al.allocate(2);
        STD::uninitialized_fill(raw3, raw3 + 2, src[1]);
        test_eq( g_mm_live, 9 );
        test_eq( raw3[1].tag(), 'b' );
        for (int i = 0; i < 2; ++i) raw3[i].~MmLive();
        al.deallocate(raw3, 2);

        for (int i = 0; i < 4; ++i) raw[i].~MmLive();
        al.deallocate(raw, 4);
        test_eq( g_mm_live, 3 );
    }
    test_eq( g_mm_live, 0 );
}
