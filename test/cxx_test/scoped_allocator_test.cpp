#include "test_cxx.hpp"
#include <scoped_allocator>
#include <memory>
#include <type_traits>
#include <utility>
#include <tuple>

namespace {

int g_outer_allocs = 0;
int g_inner_allocs = 0;

template <class T, int Tag>
struct CountingAlloc {
    typedef T              value_type;
    typedef T*             pointer;
    typedef const T*       const_pointer;
    typedef T&             reference;
    typedef const T&       const_reference;
    typedef STD::size_t    size_type;
    typedef STD::ptrdiff_t difference_type;
    template <class U> struct rebind { typedef CountingAlloc<U, Tag> other; };

    int id;
    CountingAlloc() : id(0) {}
    explicit CountingAlloc(int i) : id(i) {}
    template <class U> CountingAlloc(const CountingAlloc<U, Tag>& o) : id(o.id) {}

    T* allocate(size_type n) {
        if (Tag == 0) ++g_outer_allocs; else ++g_inner_allocs;
        return (T*)::operator new(n * sizeof(T));
    }
    T* allocate(size_type n, const void*) { return allocate(n); }
    void deallocate(T* p, size_type) { ::operator delete((void*)p); }
    size_type max_size() const { return size_type(-1) / sizeof(T); }
    void construct(T* p, const T& v) { ::new ((void*)p) T(v); }
    void destroy(T* p) { p->~T(); }
};
template <class T, int Tag, class U, int Tag2>
bool operator==(const CountingAlloc<T, Tag>& a, const CountingAlloc<U, Tag2>& b) {
    return Tag == Tag2 && a.id == b.id;
}
template <class T, int Tag, class U, int Tag2>
bool operator!=(const CountingAlloc<T, Tag>& a, const CountingAlloc<U, Tag2>& b) { return !(a == b); }

typedef CountingAlloc<int, 0> OuterI;
typedef CountingAlloc<int, 1> InnerI;

struct UsesAlloc {
    int  v;
    bool got_alloc;
    UsesAlloc() : v(0), got_alloc(false) {}
    explicit UsesAlloc(int x) : v(x), got_alloc(false) {}
    UsesAlloc(STD::allocator_arg_t, const STD::scoped_allocator_adaptor<InnerI>&)
        : v(0), got_alloc(true) {}
    UsesAlloc(STD::allocator_arg_t, const STD::scoped_allocator_adaptor<InnerI>&, int x)
        : v(x), got_alloc(true) {}
    UsesAlloc(STD::allocator_arg_t, const STD::scoped_allocator_adaptor<InnerI>&,
              const UsesAlloc& o)
        : v(o.v), got_alloc(true) {}
};

struct UsesAllocTrailing {
    int  v;
    bool got_alloc;
    UsesAllocTrailing() : v(0), got_alloc(false) {}
    UsesAllocTrailing(int x, const STD::scoped_allocator_adaptor<InnerI>&)
        : v(x), got_alloc(true) {}
};

}

namespace STD {

template <>
struct uses_allocator<UsesAlloc, STD::scoped_allocator_adaptor<InnerI> > {
    static const bool value = true;
};
template <>
struct uses_allocator<UsesAllocTrailing, STD::scoped_allocator_adaptor<InnerI> > {
    static const bool value = true;
};
}

#if defined(_MSC_VER) && _MSC_VER >= 1600 && _MSC_VER < 1800
TEST_CASE_SKIP(scoped_allocator, member_types)
#else
TEST_CASE(scoped_allocator, member_types) {
    typedef STD::scoped_allocator_adaptor<OuterI>         A1;
    typedef STD::scoped_allocator_adaptor<OuterI, InnerI> A2;

    test_true( (STD::is_same<A1::outer_allocator_type, OuterI>::value) );
    test_true( (STD::is_same<A1::value_type, int>::value) );
    test_true( (STD::is_same<A2::outer_allocator_type, OuterI>::value) );
    test_pass("cxx11:scoped_allocator_adaptor");

    test_true( (STD::is_same<A1::inner_allocator_type, A1>::value) );

    test_true( (STD::is_same<A2::inner_allocator_type,
                             STD::scoped_allocator_adaptor<InnerI> >::value) );
    test_pass("cxx11:scoped_allocator_adaptor::inner_allocator_type");

    typedef A2::rebind<char>::other A2c;
    test_true( (STD::is_same<A2c::outer_allocator_type, CountingAlloc<char, 0> >::value) );
    test_true( (STD::is_same<A2c::value_type, char>::value) );
    test_pass("cxx11:scoped_allocator_adaptor::rebind");
}
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1600 && _MSC_VER < 1800
TEST_CASE_SKIP(scoped_allocator, construction_and_accessors)
#else
TEST_CASE(scoped_allocator, construction_and_accessors) {
    OuterI o(7);
    InnerI i(9);

    STD::scoped_allocator_adaptor<OuterI> a1(o);
    test_eq( a1.outer_allocator().id, 7 );
    test_pass("cxx11:scoped_allocator_adaptor::outer_allocator");
    test_eq( a1.inner_allocator().outer_allocator().id, 7 );
    test_pass("cxx11:scoped_allocator_adaptor::inner_allocator");

    STD::scoped_allocator_adaptor<OuterI, InnerI> a2(o, STD::scoped_allocator_adaptor<InnerI>(i));
    test_eq( a2.outer_allocator().id, 7 );
    test_eq( a2.inner_allocator().outer_allocator().id, 9 );
    test_pass("cxx11:scoped_allocator_adaptor::two_arg_ctor");

    STD::scoped_allocator_adaptor<OuterI, InnerI> a2b(a2);
    test_eq( a2b.outer_allocator().id, 7 );
    test_eq( a2b.inner_allocator().outer_allocator().id, 9 );
    test_pass("cxx11:scoped_allocator_adaptor::copy_ctor");

    STD::scoped_allocator_adaptor<OuterI, InnerI> a2c;
    test_eq( a2c.outer_allocator().id, 0 );
    test_pass("cxx11:scoped_allocator_adaptor::default_ctor");
}
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1600 && _MSC_VER < 1800
TEST_CASE_SKIP(scoped_allocator, allocate_goes_to_the_outer_allocator)
#else
TEST_CASE(scoped_allocator, allocate_goes_to_the_outer_allocator) {
    g_outer_allocs = 0;
    g_inner_allocs = 0;

    STD::scoped_allocator_adaptor<OuterI, InnerI> a;
    int* p = a.allocate(4);
    test_ptr( p );
    test_eq( g_outer_allocs, 1 );
    test_eq( g_inner_allocs, 0 );
    test_pass("cxx11:scoped_allocator_adaptor::allocate");
    a.deallocate(p, 4);

    test_true( a.max_size() > 0 );
    test_pass("cxx11:scoped_allocator_adaptor::deallocate");
}
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1600 && _MSC_VER < 1920
TEST_CASE_SKIP(scoped_allocator, construct_passes_the_inner_allocator)
#else
TEST_CASE(scoped_allocator, construct_passes_the_inner_allocator) {
    STD::scoped_allocator_adaptor<CountingAlloc<UsesAlloc, 0>, InnerI> a;
    CountingAlloc<UsesAlloc, 0> raw;
    UsesAlloc* p = raw.allocate(1);

    a.construct(p, 42);
    test_eq( p->v, 42 );
    test_true( p->got_alloc );
    test_pass("cxx11:scoped_allocator_adaptor::construct");
    a.destroy(p);

    a.construct(p);
    test_true( p->got_alloc );
    test_pass("cxx11:scoped_allocator_adaptor::construct_no_arg");
    a.destroy(p);

    raw.deallocate(p, 1);
}

#endif

#if defined(_MSC_VER) && _MSC_VER >= 1600 && _MSC_VER < 1800
TEST_CASE_SKIP(scoped_allocator, construct_leaves_plain_types_alone)
#else
TEST_CASE(scoped_allocator, construct_leaves_plain_types_alone) {
    STD::scoped_allocator_adaptor<OuterI, InnerI> a;
    int* p = a.allocate(1);

    a.construct(p, 5);
    test_eq( *p, 5 );
    test_pass("cxx11:scoped_allocator_adaptor::construct_plain");
    a.destroy(p);

    a.construct(p);
    test_eq( *p, 0 );
    test_pass("cxx11:scoped_allocator_adaptor::construct_value_init");
    a.destroy(p);

    a.deallocate(p, 1);
}
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1600 && _MSC_VER < 1800
TEST_CASE_SKIP(scoped_allocator, comparison)
#else
TEST_CASE(scoped_allocator, comparison) {
    STD::scoped_allocator_adaptor<OuterI, InnerI> a(OuterI(1),
        InnerI(2));
    STD::scoped_allocator_adaptor<OuterI, InnerI> b(OuterI(1),
        InnerI(2));
    STD::scoped_allocator_adaptor<OuterI, InnerI> c(OuterI(3),
        InnerI(2));

    test_true( a == b );
    test_true( !(a != b) );
    test_pass("cxx11:scoped_allocator_adaptor::operator==");
    test_true( a != c );
    test_pass("cxx11:scoped_allocator_adaptor::operator!=");
}
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1600 && _MSC_VER < 1800
TEST_CASE_SKIP(scoped_allocator, two_inner_allocators)
#else
TEST_CASE(scoped_allocator, two_inner_allocators) {
    typedef CountingAlloc<int, 2> Inner2I;
    typedef STD::scoped_allocator_adaptor<OuterI, InnerI, Inner2I> A3;

    test_true( (STD::is_same<A3::outer_allocator_type, OuterI>::value) );
    test_true( (STD::is_same<A3::inner_allocator_type,
                             STD::scoped_allocator_adaptor<InnerI, Inner2I> >::value) );
    test_pass("cxx11:scoped_allocator_adaptor::three_level_nest");

    A3 a;
    test_eq( a.outer_allocator().id, 0 );
    test_eq( a.inner_allocator().outer_allocator().id, 0 );
    test_pass("cxx11:scoped_allocator_adaptor::nested_accessors");
}
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1600 && _MSC_VER < 1800
TEST_CASE_SKIP(scoped_allocator, typedefs_and_traits)
#else
TEST_CASE(scoped_allocator, typedefs_and_traits) {
    typedef STD::scoped_allocator_adaptor<OuterI, InnerI> A2;

    bool tp = STD::is_same<A2::pointer, int*>::value;
    bool tcp = STD::is_same<A2::const_pointer, const int*>::value;
    bool tsz = STD::is_same<A2::size_type, STD::size_t>::value;
    bool tdf = STD::is_same<A2::difference_type, STD::ptrdiff_t>::value;
    test_true( tp && tcp && tsz && tdf );
    test_pass("cxx11:scoped_allocator_adaptor member typedefs (size_type/pointer/...)");

#if !defined(_MSC_VER) || defined(__cpp_lib_allocator_traits_is_always_equal)
    bool iae = A2::is_always_equal::value;
    test_true( !iae );
    test_pass("cxx11:scoped_allocator_adaptor::is_always_equal");
#else
    TEST_SKIP1();
    test_skip("cxx11:scoped_allocator_adaptor::is_always_equal");
#endif

    bool pocca = A2::propagate_on_container_copy_assignment::value;
    bool pocma = A2::propagate_on_container_move_assignment::value;
    bool pocs  = A2::propagate_on_container_swap::value;
    test_true( !pocca && !pocma && !pocs );
    test_pass("cxx11:scoped_allocator_adaptor::propagate_on_container_*");
}
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1600 && _MSC_VER < 1800
TEST_CASE_SKIP(scoped_allocator, ctors_and_soccc)
#else
TEST_CASE(scoped_allocator, ctors_and_soccc) {
    typedef STD::scoped_allocator_adaptor<OuterI, InnerI> A2;

    STD::scoped_allocator_adaptor<CountingAlloc<char, 0>, InnerI> src(CountingAlloc<char, 0>(5), InnerI(6));
    A2 conv(src);
    test_eq( conv.outer_allocator().id, 5 );
    test_eq( conv.inner_allocator().outer_allocator().id, 6 );
    test_pass("cxx11:scoped_allocator_adaptor::scoped_allocator_adaptor(converting)");

    A2 orig(OuterI(9), InnerI(10));
    A2 moved(STD::move(orig));
    test_eq( moved.outer_allocator().id, 9 );
    test_eq( moved.inner_allocator().outer_allocator().id, 10 );
    test_pass("cxx11:scoped_allocator_adaptor::scoped_allocator_adaptor(scoped_allocator_adaptor&&) (move)");

    A2 base(OuterI(3), InnerI(4));
    A2 soccc = base.select_on_container_copy_construction();
    test_eq( soccc.outer_allocator().id, 3 );
    test_pass("cxx11:scoped_allocator_adaptor::select_on_container_copy_construction");
}
#endif

TEST_CASE(scoped_allocator, allocate_with_hint) {
    typedef STD::scoped_allocator_adaptor<OuterI> A1;
    A1 a;
    int probe = 0;
    int* p = a.allocate(4, &probe);
    test_ptr( p );
    a.deallocate(p, 4);
    test_pass("cxx11:scoped_allocator_adaptor::allocate(n, hint)");
}

#if defined(__WATCOMC__) || (defined(_MSC_VER) && _MSC_VER < 1920)
TEST_CASE_SKIP(scoped_allocator, construct_variants)
#else
TEST_CASE(scoped_allocator, construct_variants) {
    typedef STD::pair<UsesAlloc, UsesAlloc> P;
    STD::scoped_allocator_adaptor<CountingAlloc<P, 0>, InnerI> pa;

    {
        P* p = pa.allocate(1);
        pa.construct(p);
        test_true( p->first.got_alloc );
        test_true( p->second.got_alloc );
        pa.destroy(p);
        pa.deallocate(p, 1);
        test_pass("cxx11:scoped_allocator_adaptor::construct (pair default)");
    }
    {
        P* p = pa.allocate(1);
        pa.construct(p, STD::piecewise_construct,
                     STD::forward_as_tuple(11), STD::forward_as_tuple(22));
        test_eq( p->first.v, 11 );
        test_eq( p->second.v, 22 );
        test_true( p->first.got_alloc && p->second.got_alloc );
        pa.destroy(p);
        pa.deallocate(p, 1);
        test_pass("cxx11:scoped_allocator_adaptor::construct (piecewise pair)");
    }
    {
        STD::scoped_allocator_adaptor<CountingAlloc<UsesAllocTrailing, 0>, InnerI> ta;
        UsesAllocTrailing* u = ta.allocate(1);
        ta.construct(u, 7);
        test_eq( u->v, 7 );
        test_true( u->got_alloc );
        ta.destroy(u);
        ta.deallocate(u, 1);
        test_pass("cxx11:scoped_allocator_adaptor::construct (uses_allocator, trailing form)");
    }
}
#endif


#if TEST_TARGET_CXX >= 2011 && _TST_HAS_CXX11_LIB_MEMBERS
TEST_CASE(scoped_allocator, adaptor_member_types_cxx11) {
    typedef STD::scoped_allocator_adaptor<STD::allocator<int> > SA;
    SA sa;

    test_true(( STD::is_same<SA::value_type, int>::value ));
    test_pass("cxx11:value_type");
    test_pass("cxx11:scoped_allocator_adaptor::value_type");

    test_true(( STD::is_same<SA::outer_allocator_type, STD::allocator<int> >::value ));
    test_true( sa.outer_allocator() == STD::allocator<int>() );
    test_pass("cxx11:outer_allocator_type");
    test_pass("cxx11:scoped_allocator_adaptor::outer_allocator_type");

    test_true(( STD::is_same<SA::const_pointer, const int*>::value ));
    test_pass("cxx11:const_pointer");
    test_pass("cxx11:scoped_allocator_adaptor::const_pointer");

    test_true(( STD::is_same<SA::void_pointer, void*>::value ));
    test_pass("cxx11:void_pointer");
    test_pass("cxx11:scoped_allocator_adaptor::void_pointer");

    test_true(( STD::is_same<SA::const_void_pointer, const void*>::value ));
    test_pass("cxx11:const_void_pointer");
    test_pass("cxx11:scoped_allocator_adaptor::const_void_pointer");

    test_true( STD::is_signed<SA::difference_type>::value );
    test_true( sizeof(SA::difference_type) == sizeof(STD::ptrdiff_t) );
    test_pass("cxx11:difference_type");
    test_pass("cxx11:scoped_allocator_adaptor::difference_type");

    test_true( (bool)SA::propagate_on_container_copy_assignment::value ==
               (bool)STD::allocator_traits<STD::allocator<int> >::propagate_on_container_copy_assignment::value );
    test_pass("cxx11:propagate_on_container_copy_assignment");
    test_pass("cxx11:scoped_allocator_adaptor::propagate_on_container_copy_assignment");

    test_true( (bool)SA::propagate_on_container_move_assignment::value ==
               (bool)STD::allocator_traits<STD::allocator<int> >::propagate_on_container_move_assignment::value );
    test_pass("cxx11:propagate_on_container_move_assignment");
    test_pass("cxx11:scoped_allocator_adaptor::propagate_on_container_move_assignment");

    test_true( (bool)SA::propagate_on_container_swap::value ==
               (bool)STD::allocator_traits<STD::allocator<int> >::propagate_on_container_swap::value );
    test_pass("cxx11:propagate_on_container_swap");
    test_pass("cxx11:scoped_allocator_adaptor::propagate_on_container_swap");
}

namespace {
struct CcwCountedDtor {
    static int live;
    int v;
    CcwCountedDtor() : v(0) { ++live; }
    CcwCountedDtor(const CcwCountedDtor& o) : v(o.v) { ++live; }
    ~CcwCountedDtor() { --live; }
};
int CcwCountedDtor::live = 0;
}

TEST_CASE(scoped_allocator, adaptor_operations_cxx11) {
    typedef STD::scoped_allocator_adaptor<STD::allocator<CcwCountedDtor> > SA;
    SA sa;

    test_true( sa.max_size() > 0 );
    test_true( sa.max_size() <= STD::allocator_traits<STD::allocator<CcwCountedDtor> >::max_size(
                                    STD::allocator<CcwCountedDtor>()) );
    test_pass("cxx11:scoped_allocator_adaptor::max_size");

    {
        CcwCountedDtor* p = sa.allocate(1);
        test_ptr( p );
        int before = CcwCountedDtor::live;
        sa.construct(p);
        test_eq( CcwCountedDtor::live, before + 1 );
        sa.destroy(p);
        test_eq( CcwCountedDtor::live, before );
        sa.deallocate(p, 1);
    }
    test_pass("cxx11:scoped_allocator_adaptor::destroy");
}

namespace {
template <class T>
struct PropAlloc {
    typedef T value_type;
    typedef STD::true_type propagate_on_container_copy_assignment;
    typedef STD::true_type propagate_on_container_move_assignment;
    typedef STD::true_type propagate_on_container_swap;
    template <class U> struct rebind { typedef PropAlloc<U> other; };
    int id;
    PropAlloc() : id(0) {}
    explicit PropAlloc(int i) : id(i) {}
    template <class U> PropAlloc(const PropAlloc<U>& o) : id(o.id) {}
    T* allocate(STD::size_t n) { return (T*)::operator new(n * sizeof(T)); }
    void deallocate(T* p, STD::size_t) { ::operator delete((void*)p); }
};
template <class T, class U>
bool operator==(const PropAlloc<T>& a, const PropAlloc<U>& b) { return a.id == b.id; }
template <class T, class U>
bool operator!=(const PropAlloc<T>& a, const PropAlloc<U>& b) { return !(a == b); }
}

TEST_CASE(scoped_allocator, forwarded_types_cxx11) {
    typedef STD::scoped_allocator_adaptor<STD::allocator<int> > A1;
    typedef STD::scoped_allocator_adaptor<STD::allocator<int>, InnerI> A2;

    test_true(( STD::is_same<A1::pointer,
                STD::allocator_traits<STD::allocator<int> >::pointer>::value ));
    test_true(( STD::is_same<A1::pointer, int*>::value ));
    test_pass("cxx11:scoped_allocator_adaptor::pointer");

    test_true(( STD::is_same<A1::size_type,
                STD::allocator_traits<STD::allocator<int> >::size_type>::value ));
    test_true( !STD::is_signed<A1::size_type>::value );
    test_pass("cxx11:scoped_allocator_adaptor::size_type");

    typedef A2::rebind<char>::other A2c;
    test_true(( STD::is_same<A2c::value_type, char>::value ));
    test_true(( STD::is_same<A2c::outer_allocator_type, STD::allocator<char> >::value ));
    test_true(( STD::is_same<A2c::inner_allocator_type,
                STD::scoped_allocator_adaptor<InnerI> >::value ));
    test_pass("cxx11:scoped_allocator_adaptor::rebind<T>::other");

    test_true(( STD::is_same<A1::inner_allocator_type, A1>::value ));
    test_true(( STD::is_same<A1::inner_allocator_type,
                STD::scoped_allocator_adaptor<STD::allocator<int> > >::value ));
    test_pass("cxx11:scoped_allocator_adaptor::inner_allocator_type (no InnerAllocs)");

    test_true(( STD::is_same<A2::inner_allocator_type,
                STD::scoped_allocator_adaptor<InnerI> >::value ));
    test_true(( !STD::is_same<A2::inner_allocator_type, A2>::value ));
    test_pass("cxx11:scoped_allocator_adaptor::inner_allocator_type (with InnerAllocs)");
}

TEST_CASE(scoped_allocator, accessors_cxx11) {
    typedef STD::scoped_allocator_adaptor<OuterI> A1;
    typedef STD::scoped_allocator_adaptor<OuterI, InnerI> A2;

    A1 a1((OuterI(7)));
    const A1& c1 = a1;
    test_eq( c1.outer_allocator().id, 7 );
#if _TST_HAS_DECLTYPE
    test_true(( STD::is_same<decltype(c1.outer_allocator()), const OuterI&>::value ));
    test_pass("cxx11:scoped_allocator_adaptor::outer_allocator const");
#else
    TEST_SKIP1();
    test_skip("cxx11:scoped_allocator_adaptor::outer_allocator const");
#endif

    test_eq( a1.inner_allocator().outer_allocator().id, 7 );
    test_true( (void*)&a1.inner_allocator() == (void*)&a1 );
    test_pass("cxx11:scoped_allocator_adaptor::inner_allocator (no InnerAllocs)");

    A2 a2(OuterI(7), InnerI(9));
    test_eq( a2.inner_allocator().outer_allocator().id, 9 );
    test_true( (void*)&a2.inner_allocator() != (void*)&a2 );
    test_pass("cxx11:scoped_allocator_adaptor::inner_allocator (with InnerAllocs)");

    const A2& c2 = a2;
    test_eq( c2.inner_allocator().outer_allocator().id, 9 );
#if _TST_HAS_DECLTYPE
    test_true(( STD::is_same<decltype(c2.inner_allocator()),
                             const A2::inner_allocator_type&>::value ));
    test_pass("cxx11:scoped_allocator_adaptor::inner_allocator const");
#else
    TEST_SKIP1();
    test_skip("cxx11:scoped_allocator_adaptor::inner_allocator const");
#endif
}

TEST_CASE(scoped_allocator, ctors_assign_dtor_cxx11) {
    typedef STD::scoped_allocator_adaptor<OuterI, InnerI> A2;

    CountingAlloc<char, 0> other_outer(5);
    A2 built(other_outer, InnerI(6));
    test_eq( built.outer_allocator().id, 5 );
    test_eq( built.inner_allocator().outer_allocator().id, 6 );
    STD::scoped_allocator_adaptor<OuterI> none_inner(other_outer);
    test_eq( none_inner.outer_allocator().id, 5 );
    test_pass("cxx11:scoped_allocator_adaptor::scoped_allocator_adaptor(OuterA2&&, const InnerAllocs&...)");

    typedef STD::scoped_allocator_adaptor<CountingAlloc<char, 0>, InnerI> A2c;
    A2c src(CountingAlloc<char, 0>(3), InnerI(4));
    A2 conv(src);
    test_eq( conv.outer_allocator().id, 3 );
    test_eq( conv.inner_allocator().outer_allocator().id, 4 );
    test_eq( src.outer_allocator().id, 3 );
    test_pass("cxx11:scoped_allocator_adaptor::scoped_allocator_adaptor(converting copy)");

#if _TST_HAS_RVALUE_REF
    A2c src2(CountingAlloc<char, 0>(11), InnerI(12));
    A2 convm(STD::move(src2));
    test_eq( convm.outer_allocator().id, 11 );
    test_eq( convm.inner_allocator().outer_allocator().id, 12 );
    test_pass("cxx11:scoped_allocator_adaptor::scoped_allocator_adaptor(converting move)");
#else
    TEST_NOTE("no rvalue references here");
    test_skip("cxx11:scoped_allocator_adaptor::scoped_allocator_adaptor(converting move)");
#endif

    A2 a(OuterI(1), InnerI(2));
    A2 b(OuterI(8), InnerI(9));
    b = a;
    test_eq( b.outer_allocator().id, 1 );
    test_eq( b.inner_allocator().outer_allocator().id, 2 );
    test_eq( a.outer_allocator().id, 1 );
    test_pass("cxx11:scoped_allocator_adaptor::copy_assignment");

#if _TST_HAS_RVALUE_REF && !(defined(_MSC_VER) && _MSC_VER < 1900)
    A2 c(OuterI(4), InnerI(5));
    A2 d;
    d = STD::move(c);
    test_eq( d.outer_allocator().id, 4 );
    test_eq( d.inner_allocator().outer_allocator().id, 5 );
    test_pass("cxx11:scoped_allocator_adaptor::move_assignment");
#elif !_TST_HAS_RVALUE_REF
    TEST_NOTE("no rvalue references here");
    test_skip("cxx11:scoped_allocator_adaptor::move_assignment");
#else
    TEST_NOTE("MSVC before VS2015 cannot compile its own move assignment for a nested adaptor");
    test_skip("cxx11:scoped_allocator_adaptor::move_assignment");
#endif

    test_true( STD::is_destructible<A2>::value );
    {
        A2 tmp(OuterI(1), InnerI(2));
        test_eq( tmp.outer_allocator().id, 1 );
    }
    for (int i = 0; i < 32; ++i) { A2 churn; (void)churn.outer_allocator().id; }
    test_true( true );
    test_pass("cxx11:scoped_allocator_adaptor::destructor");
}

TEST_CASE(scoped_allocator, folded_traits_cxx11) {
    typedef STD::scoped_allocator_adaptor<STD::allocator<int> > Plain;
    typedef STD::scoped_allocator_adaptor<STD::allocator<int>, PropAlloc<int> > Mixed;

    test_true( !(bool)Plain::propagate_on_container_copy_assignment::value );
    test_pass("cxx11:scoped_allocator_adaptor::propagate_on_container_copy_assignment (all false)");
    test_true( (bool)Mixed::propagate_on_container_copy_assignment::value );
    test_pass("cxx11:scoped_allocator_adaptor::propagate_on_container_copy_assignment (any true)");

    typedef STD::scoped_allocator_adaptor<OuterI> AllFalse;
    typedef STD::scoped_allocator_adaptor<OuterI, PropAlloc<int> > MixedM;
    test_true( !(bool)AllFalse::propagate_on_container_move_assignment::value );
    test_pass("cxx11:scoped_allocator_adaptor::propagate_on_container_move_assignment (all false)");
    test_true( (bool)MixedM::propagate_on_container_move_assignment::value );
    test_pass("cxx11:scoped_allocator_adaptor::propagate_on_container_move_assignment (any true)");

    test_true( !(bool)AllFalse::propagate_on_container_swap::value );
    test_pass("cxx11:scoped_allocator_adaptor::propagate_on_container_swap (all false)");
    test_true( (bool)MixedM::propagate_on_container_swap::value );
    test_pass("cxx11:scoped_allocator_adaptor::propagate_on_container_swap (any true)");

#if defined(__cpp_lib_allocator_traits_is_always_equal)
    typedef STD::scoped_allocator_adaptor<STD::allocator<int>, STD::allocator<int> > BothEmpty;
    test_true( (bool)BothEmpty::is_always_equal::value );
    test_pass("cxx11:scoped_allocator_adaptor::is_always_equal (all true)");
    typedef STD::scoped_allocator_adaptor<STD::allocator<int>, PropAlloc<int> > OneStateful;
    test_true( !(bool)OneStateful::is_always_equal::value );
    test_pass("cxx11:scoped_allocator_adaptor::is_always_equal (any false)");
#else
    TEST_NOTE("allocator_traits::is_always_equal is C++17; not in this library");
    test_skip("cxx11:scoped_allocator_adaptor::is_always_equal (all true)");
    test_skip("cxx11:scoped_allocator_adaptor::is_always_equal (any false)");
#endif
}

TEST_CASE(scoped_allocator, equality_branches_cxx11) {
    typedef STD::scoped_allocator_adaptor<OuterI> A1;
    A1 a(OuterI(1)), b(OuterI(1)), c(OuterI(2));
    test_true( a == b );
    test_true( !(a == c) );
    test_pass("cxx11:scoped_allocator_adaptor::operator== (no InnerAllocs)");

    typedef STD::scoped_allocator_adaptor<OuterI, InnerI> A2;
    A2 x(OuterI(1), InnerI(2));
    A2 y(OuterI(1), InnerI(2));
    A2 z(OuterI(1), InnerI(3));
    test_true( x == y );
    test_true( !(x == z) );
    test_pass("cxx11:scoped_allocator_adaptor::operator== (with InnerAllocs)");

    test_true( x != z );
    test_true( !(x != y) );
    test_true( (x != z) == !(x == z) );
    test_pass("cxx20:scoped_allocator_adaptor operator!= (rewritten from operator==)");
}

#if defined(_MSC_VER) && _MSC_VER >= 1600 && _MSC_VER < 1920
TEST_CASE(scoped_allocator, construct_forms_cxx11) {
    test_skip("cxx11:scoped_allocator_adaptor::construct (uses_allocator, leading allocator_arg form)");
    test_skip("cxx11:scoped_allocator_adaptor::construct (uses_allocator false, plain form)");
}
TEST_CASE(scoped_allocator, construct_pair_cxx11) {
    test_skip("cxx11:scoped_allocator_adaptor::construct (pair default)");
    test_skip("cxx11:scoped_allocator_adaptor::construct (pair two args)");
    test_skip("cxx11:scoped_allocator_adaptor::construct (pair const&)");
    test_skip("cxx11:scoped_allocator_adaptor::construct (pair &&)");
    test_skip("cxx11:scoped_allocator_adaptor::construct (piecewise pair)");
    test_skip("cxx20:scoped_allocator_adaptor::construct (pair via uses_allocator_construction_args)");
}
#else
TEST_CASE(scoped_allocator, construct_forms_cxx11) {
    typedef STD::scoped_allocator_adaptor<CountingAlloc<UsesAlloc, 0>, InnerI> AU;
    AU a;
    CountingAlloc<UsesAlloc, 0> raw;
    UsesAlloc* p = raw.allocate(1);

    a.construct(p, 42);
    test_eq( p->v, 42 );
    test_true( p->got_alloc );
    a.destroy(p);
    a.construct(p);
    test_true( p->got_alloc );
    a.destroy(p);
    raw.deallocate(p, 1);
    test_pass("cxx11:scoped_allocator_adaptor::construct (uses_allocator, leading allocator_arg form)");

    STD::scoped_allocator_adaptor<OuterI, InnerI> b;
    int* q = b.allocate(1);
    b.construct(q, 5);
    test_eq( *q, 5 );
    b.destroy(q);
    b.construct(q);
    test_eq( *q, 0 );
    b.destroy(q);
    b.deallocate(q, 1);
    test_pass("cxx11:scoped_allocator_adaptor::construct (uses_allocator false, plain form)");
}

TEST_CASE(scoped_allocator, construct_pair_cxx11) {
    typedef STD::pair<UsesAlloc, int> P;
    typedef STD::scoped_allocator_adaptor<CountingAlloc<P, 0>, InnerI> AP;
    AP a;
    CountingAlloc<P, 0> raw;
    P* p = raw.allocate(1);

    a.construct(p);
    test_true( p->first.got_alloc );
    test_eq( p->second, 0 );
    a.destroy(p);
    test_pass("cxx11:scoped_allocator_adaptor::construct (pair default)");

    a.construct(p, UsesAlloc(3), 4);
    test_true( p->first.got_alloc );
    test_eq( p->second, 4 );
    a.destroy(p);
    test_pass("cxx11:scoped_allocator_adaptor::construct (pair two args)");

    P model(UsesAlloc(7), 8);
    test_true( !model.first.got_alloc );
    const P& cmodel = model;
    a.construct(p, cmodel);
    test_true( p->first.got_alloc );
    test_eq( p->second, 8 );
    a.destroy(p);
    test_pass("cxx11:scoped_allocator_adaptor::construct (pair const&)");

#if _TST_HAS_RVALUE_REF
    P model2(UsesAlloc(9), 10);
    a.construct(p, STD::move(model2));
    test_true( p->first.got_alloc );
    test_eq( p->second, 10 );
    a.destroy(p);
    test_pass("cxx11:scoped_allocator_adaptor::construct (pair &&)");
#else
    TEST_NOTE("no rvalue references here");
    test_skip("cxx11:scoped_allocator_adaptor::construct (pair &&)");
#endif

    a.construct(p, STD::piecewise_construct,
                   STD::forward_as_tuple(11), STD::forward_as_tuple(12));
    test_true( p->first.got_alloc );
    test_eq( p->first.v, 11 );
    test_eq( p->second, 12 );
    a.destroy(p);
    test_pass("cxx11:scoped_allocator_adaptor::construct (piecewise pair)");

#if defined(__cpp_lib_make_from_tuple) && TEST_TARGET_CXX >= 2020 && defined(__cpp_lib_concepts)
    {
        STD::scoped_allocator_adaptor<InnerI> inner(a.inner_allocator());
        P made = STD::make_obj_using_allocator<P>(inner, UsesAlloc(13), 14);
        test_true( made.first.got_alloc );
        test_eq( made.second, 14 );
        a.construct(p, UsesAlloc(13), 14);
        test_eq( p->first.got_alloc, made.first.got_alloc );
        a.destroy(p);
    }
    test_pass("cxx20:scoped_allocator_adaptor::construct (pair via uses_allocator_construction_args)");
#else
    TEST_NOTE("uses_allocator_construction_args is C++20; not in this library");
    test_skip("cxx20:scoped_allocator_adaptor::construct (pair via uses_allocator_construction_args)");
#endif

    raw.deallocate(p, 1);
}
#endif

TEST_CASE(scoped_allocator, deduction_guide_cxx17) {
#if _TST_HAS_CTAD && defined(_CCW_HAS_SCOPED_ALLOC_CTAD)
    STD::scoped_allocator_adaptor deduced(OuterI(1), InnerI(2));
    test_true(( STD::is_same<decltype(deduced),
                STD::scoped_allocator_adaptor<OuterI, InnerI> >::value ));
    test_eq( deduced.outer_allocator().id, 1 );
    test_eq( deduced.inner_allocator().outer_allocator().id, 2 );

    STD::scoped_allocator_adaptor alone((OuterI(3)));
    test_true(( STD::is_same<decltype(alone),
                STD::scoped_allocator_adaptor<OuterI> >::value ));
    test_pass("cxx17:scoped_allocator_adaptor deduction guide");
#else
    TEST_NOTE("no scoped_allocator_adaptor deduction guide in this library");
    test_skip("cxx17:scoped_allocator_adaptor deduction guide");
#endif
}
#else
TEST_CASE(scoped_allocator, adaptor_member_types_cxx11) {
    TEST_SKIP_N(0);
    test_skip("cxx11:value_type");
    test_skip("cxx11:scoped_allocator_adaptor::value_type");
    test_skip("cxx11:outer_allocator_type");
    test_skip("cxx11:scoped_allocator_adaptor::outer_allocator_type");
    test_skip("cxx11:const_pointer");
    test_skip("cxx11:scoped_allocator_adaptor::const_pointer");
    test_skip("cxx11:void_pointer");
    test_skip("cxx11:scoped_allocator_adaptor::void_pointer");
    test_skip("cxx11:const_void_pointer");
    test_skip("cxx11:scoped_allocator_adaptor::const_void_pointer");
    test_skip("cxx11:difference_type");
    test_skip("cxx11:scoped_allocator_adaptor::difference_type");
    test_skip("cxx11:propagate_on_container_copy_assignment");
    test_skip("cxx11:scoped_allocator_adaptor::propagate_on_container_copy_assignment");
    test_skip("cxx11:propagate_on_container_move_assignment");
    test_skip("cxx11:scoped_allocator_adaptor::propagate_on_container_move_assignment");
    test_skip("cxx11:propagate_on_container_swap");
    test_skip("cxx11:scoped_allocator_adaptor::propagate_on_container_swap");
}
TEST_CASE(scoped_allocator, adaptor_operations_cxx11) {
    test_skip("cxx11:scoped_allocator_adaptor::max_size");
    test_skip("cxx11:scoped_allocator_adaptor::destroy");
}
TEST_CASE(scoped_allocator, forwarded_types_cxx11) {
    test_skip("cxx11:scoped_allocator_adaptor::pointer");
    test_skip("cxx11:scoped_allocator_adaptor::size_type");
    test_skip("cxx11:scoped_allocator_adaptor::rebind<T>::other");
    test_skip("cxx11:scoped_allocator_adaptor::inner_allocator_type (no InnerAllocs)");
    test_skip("cxx11:scoped_allocator_adaptor::inner_allocator_type (with InnerAllocs)");
}
TEST_CASE(scoped_allocator, accessors_cxx11) {
    test_skip("cxx11:scoped_allocator_adaptor::outer_allocator const");
    test_skip("cxx11:scoped_allocator_adaptor::inner_allocator (no InnerAllocs)");
    test_skip("cxx11:scoped_allocator_adaptor::inner_allocator (with InnerAllocs)");
    test_skip("cxx11:scoped_allocator_adaptor::inner_allocator const");
}
TEST_CASE(scoped_allocator, ctors_assign_dtor_cxx11) {
    test_skip("cxx11:scoped_allocator_adaptor::scoped_allocator_adaptor(OuterA2&&, const InnerAllocs&...)");
    test_skip("cxx11:scoped_allocator_adaptor::scoped_allocator_adaptor(converting copy)");
    test_skip("cxx11:scoped_allocator_adaptor::scoped_allocator_adaptor(converting move)");
    test_skip("cxx11:scoped_allocator_adaptor::copy_assignment");
    test_skip("cxx11:scoped_allocator_adaptor::move_assignment");
    test_skip("cxx11:scoped_allocator_adaptor::destructor");
}
TEST_CASE(scoped_allocator, folded_traits_cxx11) {
    test_skip("cxx11:scoped_allocator_adaptor::propagate_on_container_copy_assignment (all false)");
    test_skip("cxx11:scoped_allocator_adaptor::propagate_on_container_copy_assignment (any true)");
    test_skip("cxx11:scoped_allocator_adaptor::propagate_on_container_move_assignment (all false)");
    test_skip("cxx11:scoped_allocator_adaptor::propagate_on_container_move_assignment (any true)");
    test_skip("cxx11:scoped_allocator_adaptor::propagate_on_container_swap (all false)");
    test_skip("cxx11:scoped_allocator_adaptor::propagate_on_container_swap (any true)");
    test_skip("cxx11:scoped_allocator_adaptor::is_always_equal (all true)");
    test_skip("cxx11:scoped_allocator_adaptor::is_always_equal (any false)");
}
TEST_CASE(scoped_allocator, equality_branches_cxx11) {
    test_skip("cxx11:scoped_allocator_adaptor::operator== (no InnerAllocs)");
    test_skip("cxx11:scoped_allocator_adaptor::operator== (with InnerAllocs)");
    test_skip("cxx20:scoped_allocator_adaptor operator!= (rewritten from operator==)");
}
TEST_CASE(scoped_allocator, construct_forms_cxx11) {
    test_skip("cxx11:scoped_allocator_adaptor::construct (uses_allocator, leading allocator_arg form)");
    test_skip("cxx11:scoped_allocator_adaptor::construct (uses_allocator false, plain form)");
}
TEST_CASE(scoped_allocator, construct_pair_cxx11) {
    test_skip("cxx11:scoped_allocator_adaptor::construct (pair default)");
    test_skip("cxx11:scoped_allocator_adaptor::construct (pair two args)");
    test_skip("cxx11:scoped_allocator_adaptor::construct (pair const&)");
    test_skip("cxx11:scoped_allocator_adaptor::construct (pair &&)");
    test_skip("cxx11:scoped_allocator_adaptor::construct (piecewise pair)");
    test_skip("cxx20:scoped_allocator_adaptor::construct (pair via uses_allocator_construction_args)");
}
TEST_CASE(scoped_allocator, deduction_guide_cxx17) {
    test_skip("cxx17:scoped_allocator_adaptor deduction guide");
}
#endif
