#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2017
#include <memory_resource>
#include <new>
#include <vector>

TEST_CASE(memory_resource, new_delete_and_allocator) {
    STD::pmr::memory_resource* nd = STD::pmr::new_delete_resource();
    test_true( STD::pmr::get_default_resource() == nd );
    test_pass("cxx17:new_delete_resource");
    test_true( *nd == *nd );

    STD::pmr::polymorphic_allocator<int> pa;
    int* a = pa.allocate(8);
    for (int i = 0; i < 8; ++i) pa.construct(a + i, i + 1);
    test_eq( a[0], 1 );
    test_pass("cxx17:polymorphic_allocator::allocate");
    test_eq( a[7], 8 );
    pa.deallocate(a, 8);
    test_pass("cxx17:polymorphic_allocator::deallocate");
}

TEST_CASE(memory_resource, monotonic_buffer) {
    STD::pmr::memory_resource* nd = STD::pmr::new_delete_resource();
    STD::pmr::monotonic_buffer_resource mbr(64, nd);

    STD::pmr::polymorphic_allocator<long> pa2(&mbr);
    long* b = pa2.allocate(4);
    b[0] = 100; b[3] = 400;

    char* big = (char*)mbr.allocate(300, 1);
    big[299] = 'x';

    test_eq( b[0], 100L );
    test_eq( b[3], 400L );
    test_pass("cxx17:monotonic_buffer_resource");
    test_eq( big[299], 'x' );
    test_pass("cxx17:monotonic_buffer_resource::allocate");

    mbr.release();
}

#if TEST_HAS_EH
TEST_CASE(memory_resource, null_resource_throws) {
    bool threw = false;
    try { STD::pmr::null_memory_resource()->allocate(4, 4); }
    catch (STD::bad_alloc&) { threw = true; }
    test_true( threw );
    test_pass("cxx17:null_memory_resource");
}
#else   // !TEST_HAS_EH
TEST_CASE(memory_resource, null_resource_throws) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx17:null_memory_resource");
}
#endif  // TEST_HAS_EH

TEST_CASE(memory_resource, base_resource_api) {

    STD::pmr::memory_resource* nd = STD::pmr::new_delete_resource();
    void* p = nd->allocate(32, 8);
    test_ptr( p );
    test_pass("cxx17:memory_resource::allocate");
    nd->deallocate(p, 32, 8);

    test_true( nd->is_equal(*nd) );
    test_pass("cxx17:memory_resource::is_equal");
    test_true( *nd == *nd );
    test_true( !(*nd == *STD::pmr::null_memory_resource()) );
    test_pass("cxx17:memory_resource::operator==");
    test_true( *nd != *STD::pmr::null_memory_resource() );
    test_pass("cxx17:memory_resource::operator!=");
}

TEST_CASE(memory_resource, default_resource_swap) {
    STD::pmr::memory_resource* orig = STD::pmr::get_default_resource();

    STD::pmr::memory_resource* prev = STD::pmr::set_default_resource(STD::pmr::null_memory_resource());
    test_true( prev == orig );
    test_true( STD::pmr::get_default_resource() == STD::pmr::null_memory_resource() );
    test_pass("cxx17:set_default_resource");

    STD::pmr::polymorphic_allocator<int> pa;
    test_true( pa.resource() == STD::pmr::null_memory_resource() );
    test_pass("cxx17:polymorphic_allocator::default_resource");

    STD::pmr::set_default_resource(orig);
    test_true( STD::pmr::get_default_resource() == orig );
    test_pass("cxx17:get_default_resource");
}

TEST_CASE(memory_resource, allocator_details) {
    STD::pmr::memory_resource* nd = STD::pmr::new_delete_resource();

    STD::pmr::polymorphic_allocator<int> a(nd);
    test_true( a.resource() == nd );
    test_pass("cxx17:polymorphic_allocator::resource");

    int* p = a.allocate(1);
    a.construct(p, 42);
    test_eq( *p, 42 );
    test_pass("cxx17:polymorphic_allocator::construct");
    a.deallocate(p, 1);

    STD::pmr::polymorphic_allocator<double> b(a);
    test_true( b.resource() == nd );
    test_pass("cxx17:polymorphic_allocator::rebind_ctor");

    STD::pmr::polymorphic_allocator<int> c(nd);
    test_true( a == c );
    test_pass("cxx17:polymorphic_allocator::operator==");

    STD::pmr::polymorphic_allocator<int> soc = a.select_on_container_copy_construction();
    test_true( soc.resource() == STD::pmr::get_default_resource() );
    test_pass("cxx17:polymorphic_allocator::select_on_container_copy_construction");
}

TEST_CASE(memory_resource, monotonic_upstream) {
    STD::pmr::memory_resource* nd = STD::pmr::new_delete_resource();

    STD::pmr::monotonic_buffer_resource m1(nd);
    test_true( m1.upstream_resource() == nd );
    test_pass("cxx17:monotonic_buffer_resource::upstream_resource");

    STD::pmr::monotonic_buffer_resource m2(128, nd);
    test_true( m2.upstream_resource() == nd );
    void* q = m2.allocate(16, 8);
    test_ptr( q );
    m2.release();
    test_pass("cxx17:monotonic_buffer_resource::release");

    STD::pmr::monotonic_buffer_resource m3;
    test_true( m3.upstream_resource() == STD::pmr::get_default_resource() );
    test_pass("cxx17:monotonic_buffer_resource::default_ctor");
}

TEST_CASE(memory_resource, more_cxx17) {
    STD::pmr::memory_resource* nd = STD::pmr::new_delete_resource();

    char buf[128];
    STD::pmr::monotonic_buffer_resource m(buf, sizeof(buf), nd);
    void* p = m.allocate(16, 8);
    test_ptr( p );
    test_true( p >= (void*)buf && p < (void*)(buf + sizeof(buf)) );
    test_pass("cxx17:monotonic_buffer_resource::monotonic_buffer_resource(void*, size_t, memory_resource*)");

    STD::pmr::polymorphic_allocator<int> a(&m);
    STD::pmr::polymorphic_allocator<int> b(a);
    test_true( a.resource() == b.resource() );
    test_pass("cxx17:polymorphic_allocator::polymorphic_allocator(const polymorphic_allocator&) (copy)");

    STD::pmr::polymorphic_allocator<int> c(nd);
    test_true( a != c );
    test_true( !(a != b) );
    test_pass("cxx17:operator!=(polymorphic_allocator)");

    STD::pmr::polymorphic_allocator<int> pa;
    int* obj = pa.allocate(1);
    pa.construct(obj, 42);
    test_eq( *obj, 42 );
#if !defined(_MSC_VER) || _MSC_VER >= 1920
    pa.destroy(obj);
    pa.deallocate(obj, 1);
    test_pass("cxx17:polymorphic_allocator::destroy (deprecated C++20)");
#else
    TEST_NOTE("MSVC before VS2019 has no polymorphic_allocator::destroy");
    pa.deallocate(obj, 1);
    test_skip("cxx17:polymorphic_allocator::destroy (deprecated C++20)");
#endif
}

TEST_CASE(memory_resource, pool_resources) {
#if !defined(__WATCOMC__) && _TST_HAS_CXX17_LIB_MEMBERS
    STD::pmr::pool_options opt;
    opt.max_blocks_per_chunk = 8;
    opt.largest_required_pool_block = 256;
    test_true( opt.max_blocks_per_chunk == 8u );
    test_pass("cxx17:pool_options");

    {
        STD::pmr::unsynchronized_pool_resource up(opt);
        void* q = up.allocate(32, 8);
        test_ptr( q );
        up.deallocate(q, 32, 8);
        test_true( up.upstream_resource() == STD::pmr::get_default_resource() );
        test_pass("cxx17:unsynchronized_pool_resource");
    }
    {
        STD::pmr::synchronized_pool_resource sp;
        void* q = sp.allocate(48, 8);
        test_ptr( q );
        sp.deallocate(q, 48, 8);
        test_pass("cxx17:synchronized_pool_resource");
    }
#else
    test_skip("cxx17:pool_options");
    test_skip("cxx17:unsynchronized_pool_resource");
    test_skip("cxx17:synchronized_pool_resource");
#endif
}

TEST_CASE(memory_resource, cxx20_allocator_helpers) {
#if !defined(__WATCOMC__) && TEST_TARGET_CXX >= 2020 && _TST_HAS_BYTE && _TST_HAS_CXX17_LIB_MEMBERS && (!defined(_MSC_VER) || _MSC_VER >= 1920)
    STD::pmr::polymorphic_allocator<STD::byte> pa;

    void* raw = pa.allocate_bytes(64, 8);
    test_ptr( raw );
    pa.deallocate_bytes(raw, 64, 8);
    test_pass("cxx20:polymorphic_allocator::allocate_bytes");
    test_pass("cxx20:polymorphic_allocator::deallocate_bytes");

    int* arr = pa.allocate_object<int>(4);
    test_ptr( arr );
    pa.deallocate_object(arr, 4);
    test_pass("cxx20:polymorphic_allocator::allocate_object");
    test_pass("cxx20:polymorphic_allocator::deallocate_object");

    int* one = pa.new_object<int>(7);
    test_eq( *one, 7 );
    pa.delete_object(one);
    test_pass("cxx20:polymorphic_allocator::new_object");
    test_pass("cxx20:polymorphic_allocator::delete_object");
#else
    test_skip("cxx20:polymorphic_allocator::allocate_bytes");
    test_skip("cxx20:polymorphic_allocator::deallocate_bytes");
    test_skip("cxx20:polymorphic_allocator::allocate_object");
    test_skip("cxx20:polymorphic_allocator::deallocate_object");
    test_skip("cxx20:polymorphic_allocator::new_object");
    test_skip("cxx20:polymorphic_allocator::delete_object");
#endif
}

namespace {
int g_mr_live = 0;

struct MrLive {
    char* p_;
    MrLive() : p_(new char[16]) { p_[0] = 0; ++g_mr_live; }
    MrLive(char c) : p_(new char[16]) { p_[0] = c; p_[1] = 0; ++g_mr_live; }
    MrLive(const MrLive& o) : p_(new char[16]) {
        for (int i = 0; i < 16; ++i) p_[i] = o.p_[i];
        ++g_mr_live;
    }
    MrLive& operator=(const MrLive& o) {
        if (this != &o) for (int i = 0; i < 16; ++i) p_[i] = o.p_[i];
        return *this;
    }
    ~MrLive() { delete[] p_; --g_mr_live; }
    char tag() const { return p_[0]; }
};
}

TEST_CASE(memory_resource, class_elements_lifetime) {
    g_mr_live = 0;
    {
        STD::pmr::monotonic_buffer_resource mbr(1024, STD::pmr::new_delete_resource());
        STD::pmr::polymorphic_allocator<MrLive> pa(&mbr);

        MrLive* p = pa.allocate(3);
        test_eq( g_mr_live, 0 );

        for (int i = 0; i < 3; ++i) pa.construct(p + i, MrLive((char)('a' + i)));
        test_eq( g_mr_live, 3 );
        test_eq( p[0].tag(), 'a' );
        test_eq( p[2].tag(), 'c' );

#if !defined(_MSC_VER) || _MSC_VER >= 1920
        for (int i = 0; i < 3; ++i) pa.destroy(p + i);
#else
        for (int i = 0; i < 3; ++i) (p + i)->~MrLive();
#endif
        test_eq( g_mr_live, 0 );
        pa.deallocate(p, 3);

        {
            STD::vector<MrLive, STD::pmr::polymorphic_allocator<MrLive> > v(pa);
            for (int i = 0; i < 5; ++i) v.push_back(MrLive((char)('p' + i)));
            test_eq( (int)v.size(), 5 );
            test_eq( g_mr_live, 5 );
            test_eq( v[0].tag(), 'p' );
            test_eq( v[4].tag(), 't' );
            v.erase(v.begin());
            test_eq( g_mr_live, 4 );
            v.clear();
            test_eq( g_mr_live, 0 );
        }
        test_eq( g_mr_live, 0 );
    }
    test_eq( g_mr_live, 0 );
}

#endif


#if TEST_TARGET_CXX >= 2017 && defined(__cpp_lib_memory_resource)
namespace {
int g_ccw_res_destroyed = 0;

struct CcwCountingResource : public STD::pmr::memory_resource {
    int allocs, deallocs, equals;
    STD::size_t last_bytes, last_align;
    CcwCountingResource() : allocs(0), deallocs(0), equals(0), last_bytes(0), last_align(0) {}
    ~CcwCountingResource() { ++g_ccw_res_destroyed; }
protected:
    void* do_allocate(STD::size_t bytes, STD::size_t align) {
        ++allocs;
        return STD::pmr::new_delete_resource()->allocate(bytes, align);
    }
    void do_deallocate(void* p, STD::size_t bytes, STD::size_t align) {
        ++deallocs;
        last_bytes = bytes;
        last_align = align;
        STD::pmr::new_delete_resource()->deallocate(p, bytes, align);
    }
    bool do_is_equal(const STD::pmr::memory_resource& other) const TEST_NOTHROW {
        const_cast<CcwCountingResource*>(this)->equals++;
        return this == &other;
    }
};
}

TEST_CASE(memory_resource, do_virtuals_cxx17) {
    CcwCountingResource r;
    STD::pmr::memory_resource& mr = r;

    void* p = mr.allocate(32, 8);
    test_ptr( p );
    test_eq( r.allocs, 1 );
    test_pass("cxx17:memory_resource::do_allocate");

    mr.deallocate(p, 32, 8);
    test_eq( r.deallocs, 1 );
    test_pass("cxx17:memory_resource::do_deallocate");

    CcwCountingResource other;
    test_true( mr.is_equal(r) );
    test_true( !mr.is_equal(other) );
    test_true( r.equals >= 2 );
    test_pass("cxx17:memory_resource::do_is_equal");

    test_true(( STD::is_same<STD::pmr::polymorphic_allocator<int>::value_type,
                             int>::value ));
#if TEST_TARGET_CXX >= 2020 && (!defined(_MSC_VER) || _MSC_VER >= 1920)
    test_true(( STD::is_same<STD::pmr::polymorphic_allocator<>::value_type,
                             STD::byte>::value ));
    test_pass("cxx17:polymorphic_allocator::value_type");
#else
    TEST_SKIP1();
    test_skip("cxx17:polymorphic_allocator::value_type");
#endif
}

TEST_CASE(memory_resource, standard_resource_virtuals_cxx17) {
    TEST_NOTE("do_* are protected; exercised through allocate/deallocate/is_equal");

    STD::pmr::monotonic_buffer_resource mono;
    void* a = mono.allocate(16, 8);
    void* b = mono.allocate(16, 8);
    test_ptr( a );
    test_true( a != b );
    test_pass("cxx17:monotonic_buffer_resource::do_allocate");

    mono.deallocate(a, 16, 8);
    test_true( true );
    test_pass("cxx17:monotonic_buffer_resource::do_deallocate");

    STD::pmr::monotonic_buffer_resource mono2;
    test_true( mono.is_equal(mono) );
    test_true( !mono.is_equal(mono2) );
    test_pass("cxx17:monotonic_buffer_resource::do_is_equal");

    STD::pmr::pool_options opt;
    opt.max_blocks_per_chunk = 8;
    opt.largest_required_pool_block = 64;

    STD::pmr::synchronized_pool_resource sync(opt);
    void* s = sync.allocate(24, 8);
    test_ptr( s );
    test_pass("cxx17:synchronized_pool_resource::do_allocate");
    sync.deallocate(s, 24, 8);
    test_pass("cxx17:synchronized_pool_resource::do_deallocate");
    STD::pmr::synchronized_pool_resource sync2;
    test_true( sync.is_equal(sync) );
    test_true( !sync.is_equal(sync2) );
    test_pass("cxx17:synchronized_pool_resource::do_is_equal");
    STD::pmr::pool_options got = sync.options();
    test_true( got.max_blocks_per_chunk <= 8 || got.max_blocks_per_chunk > 0 );
    test_pass("cxx17:synchronized_pool_resource::options");

    STD::pmr::unsynchronized_pool_resource uns(opt);
    void* u = uns.allocate(24, 8);
    test_ptr( u );
    test_pass("cxx17:unsynchronized_pool_resource::do_allocate");
    uns.deallocate(u, 24, 8);
    test_pass("cxx17:unsynchronized_pool_resource::do_deallocate");
    STD::pmr::unsynchronized_pool_resource uns2;
    test_true( uns.is_equal(uns) );
    test_true( !uns.is_equal(uns2) );
    test_pass("cxx17:unsynchronized_pool_resource::do_is_equal");
    STD::pmr::pool_options ugot = uns.options();
    test_true( ugot.largest_required_pool_block > 0 );
    test_pass("cxx17:unsynchronized_pool_resource::options");
}

TEST_CASE(memory_resource, lifetime_and_public_api_cxx17) {
    {
        STD::pmr::memory_resource* mr = new CcwCountingResource();
        void* p = mr->allocate(8, 8);
        mr->deallocate(p, 8, 8);
        g_ccw_res_destroyed = 0;
        delete mr;
        test_eq( g_ccw_res_destroyed, 1 );
    }
    test_pass("cxx17:memory_resource::~memory_resource");

    {
        CcwCountingResource r;
        STD::pmr::memory_resource& mr = r;
        void* p = mr.allocate(64, 16);
        test_ptr( p );
        int before = r.deallocs;
        mr.deallocate(p, 64, 16);
        test_eq( r.deallocs, before + 1 );
        test_eq( r.last_bytes, (STD::size_t)64 );
        test_eq( r.last_align, (STD::size_t)16 );
    }
    test_pass("cxx17:memory_resource::deallocate");

    {
        CcwCountingResource up;
        {
            STD::pmr::monotonic_buffer_resource mono(&up);
            void* a = mono.allocate(4096, 8);
            test_ptr( a );
            test_true( up.allocs > 0 );
            test_eq( up.deallocs, 0 );
        }
        test_eq( up.deallocs, up.allocs );
    }
    test_pass("cxx17:monotonic_buffer_resource::~monotonic_buffer_resource");
}

TEST_CASE(memory_resource, pool_resource_members_cxx17) {
    STD::pmr::pool_options opt;
    opt.max_blocks_per_chunk = 16;
    test_true(( STD::is_same<decltype(opt.max_blocks_per_chunk), STD::size_t>::value ));
    test_eq( opt.max_blocks_per_chunk, (STD::size_t)16 );
    test_pass("cxx17:pool_options::max_blocks_per_chunk");

    opt.largest_required_pool_block = 512;
    test_true(( STD::is_same<decltype(opt.largest_required_pool_block), STD::size_t>::value ));
    test_eq( opt.largest_required_pool_block, (STD::size_t)512 );
    test_pass("cxx17:pool_options::largest_required_pool_block");

    {
        CcwCountingResource up;
        {
            STD::pmr::synchronized_pool_resource sp(opt, &up);
            test_true( sp.upstream_resource() == &up );
            test_pass("cxx17:synchronized_pool_resource::upstream_resource");

            void* q = sp.allocate(32, 8);
            test_ptr( q );
            test_true( up.allocs > 0 );
            sp.release();
            test_eq( up.deallocs, up.allocs );
            void* q2 = sp.allocate(32, 8);
            test_ptr( q2 );
            test_pass("cxx17:synchronized_pool_resource::release");
        }
        test_eq( up.deallocs, up.allocs );
    }
    test_pass("cxx17:synchronized_pool_resource::~synchronized_pool_resource");

    {
        CcwCountingResource up;
        {
            STD::pmr::unsynchronized_pool_resource up_res(opt, &up);
            test_true( up_res.upstream_resource() == &up );
            test_pass("cxx17:unsynchronized_pool_resource::upstream_resource");

            void* q = up_res.allocate(32, 8);
            test_ptr( q );
            test_true( up.allocs > 0 );
            up_res.release();
            test_eq( up.deallocs, up.allocs );
            void* q2 = up_res.allocate(32, 8);
            test_ptr( q2 );
            test_pass("cxx17:unsynchronized_pool_resource::release");
        }
        test_eq( up.deallocs, up.allocs );
    }
    test_pass("cxx17:unsynchronized_pool_resource::~unsynchronized_pool_resource");
}
#else
TEST_CASE(memory_resource, do_virtuals_cxx17) {
    test_skip("cxx17:memory_resource::do_allocate");
    test_skip("cxx17:memory_resource::do_deallocate");
    test_skip("cxx17:memory_resource::do_is_equal");
    test_skip("cxx17:polymorphic_allocator::value_type");
}
TEST_CASE(memory_resource, standard_resource_virtuals_cxx17) {
    test_skip("cxx17:monotonic_buffer_resource::do_allocate");
    test_skip("cxx17:monotonic_buffer_resource::do_deallocate");
    test_skip("cxx17:monotonic_buffer_resource::do_is_equal");
    test_skip("cxx17:synchronized_pool_resource::do_allocate");
    test_skip("cxx17:synchronized_pool_resource::do_deallocate");
    test_skip("cxx17:synchronized_pool_resource::do_is_equal");
    test_skip("cxx17:synchronized_pool_resource::options");
    test_skip("cxx17:unsynchronized_pool_resource::do_allocate");
    test_skip("cxx17:unsynchronized_pool_resource::do_deallocate");
    test_skip("cxx17:unsynchronized_pool_resource::do_is_equal");
    test_skip("cxx17:unsynchronized_pool_resource::options");
}
TEST_CASE(memory_resource, lifetime_and_public_api_cxx17) {
    test_skip("cxx17:memory_resource::~memory_resource");
    test_skip("cxx17:memory_resource::deallocate");
    test_skip("cxx17:monotonic_buffer_resource::~monotonic_buffer_resource");
}
TEST_CASE(memory_resource, pool_resource_members_cxx17) {
    test_skip("cxx17:pool_options::max_blocks_per_chunk");
    test_skip("cxx17:pool_options::largest_required_pool_block");
    test_skip("cxx17:synchronized_pool_resource::upstream_resource");
    test_skip("cxx17:synchronized_pool_resource::release");
    test_skip("cxx17:synchronized_pool_resource::~synchronized_pool_resource");
    test_skip("cxx17:unsynchronized_pool_resource::upstream_resource");
    test_skip("cxx17:unsynchronized_pool_resource::release");
    test_skip("cxx17:unsynchronized_pool_resource::~unsynchronized_pool_resource");
}
#endif

TEST_CASE(memory_resource, feature_test_macros) {
#if defined(__cpp_lib_memory_resource)
    test_true( __cpp_lib_memory_resource > 0L );
    test_pass("cxx17:__cpp_lib_memory_resource");
#else
    test_skip("cxx17:__cpp_lib_memory_resource");
#endif
#if defined(__cpp_lib_polymorphic_allocator)
    test_true( __cpp_lib_polymorphic_allocator > 0L );
    test_pass("cxx20:__cpp_lib_polymorphic_allocator");
#else
    test_skip("cxx20:__cpp_lib_polymorphic_allocator");
#endif
}
