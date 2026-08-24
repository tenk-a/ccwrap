#include "test_cxx.hpp"

#include <new>
#include <string>
#if TEST_TARGET_CXX >= 2026
#include <memory>
#endif

TEST_CASE(new_hdr, launder_interference) {
    int x = 5;
    int* p = &x;
#if TEST_TARGET_CXX >= 2017
    test_true( STD::launder(p) == p );
    test_eq( *STD::launder(p), 5 );

    test_true( STD::hardware_destructive_interference_size  >= 1 );
    test_true( STD::hardware_constructive_interference_size >= 1 );
    test_pass("cxx17:std::launder");
#else
    TEST_NOTE("launder and the interference sizes are C++17");
    test_skip("cxx17:std::launder");
#endif
}

TEST_CASE(new_hdr, new_handler) {

    STD::new_handler prev = STD::set_new_handler(0);
    test_true( 1 );
    test_pass("cxx03:new_handler");

    test_true( STD::get_new_handler() == 0 );
    test_pass("cxx03:set_new_handler");

    STD::set_new_handler(prev);
    test_true( STD::get_new_handler() == prev );
    test_pass("cxx03:get_new_handler");
}

#if TEST_HAS_EH
TEST_CASE(new_hdr, bad_array_new_length) {
    bool caught = false;
    try {
        throw STD::bad_array_new_length();
    } catch (STD::bad_alloc& e) {
        caught = true;
        test_true( e.what() != 0 );
        test_true( STD::string(e.what()).size() > 0 );
    }
    test_true( caught );
    test_pass("cxx11:bad_array_new_length");

    STD::bad_array_new_length b;
    test_true( b.what() != 0 );
    test_true( STD::string(b.what()).size() > 0 );
    const STD::bad_alloc& asbase = b;
    test_true( STD::string(asbase.what()) == STD::string(b.what()) );
    test_pass("cxx11:bad_array_new_length::what");
}
#else   // !TEST_HAS_EH
TEST_CASE(new_hdr, bad_array_new_length) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx11:bad_array_new_length");
    test_skip("cxx11:bad_array_new_length::what");
}
#endif  // TEST_HAS_EH

TEST_CASE(new_hdr, destroying_delete_tag) {
#if TEST_TARGET_CXX >= 2020
    STD::destroying_delete_t dd = STD::destroying_delete;
    (void)dd;
    test_true( sizeof(STD::destroying_delete_t) >= 1 );
    test_pass("cxx20:destroying_delete_t");
#else
    TEST_NOTE("destroying_delete_t is C++20");
    test_skip("cxx20:destroying_delete_t");
#endif
}

#if TEST_HAS_EH
TEST_CASE(new_hdr, operators_and_bad_alloc) {

    STD::nothrow_t nt = STD::nothrow;
    (void)nt;
    test_true( true );
    test_pass("cxx03:nothrow_t");
    test_pass("cxx03:nothrow");

    void* p = ::operator new(32);
    test_ptr( p );
    ::operator delete(p);
    test_pass("cxx03:operator new(size_t)");
    test_pass("cxx03:operator delete(void*)");

    void* pa = ::operator new[](32);
    test_ptr( pa );
    ::operator delete[](pa);
    test_pass("cxx03:operator new[](size_t)");
    test_pass("cxx03:operator delete[](void*)");

    void* pn = ::operator new(32, STD::nothrow);
    test_ptr( pn );
    test_pass("cxx03:operator new(size_t,nothrow_t)");
#if !defined(__WATCOMC__)
    ::operator delete(pn, STD::nothrow);
    test_true( true );
    test_pass("cxx03:operator delete(void*,nothrow_t)");
#else
    ::operator delete(pn);
    TEST_NOTE("Open Watcom rejects a two-argument operator delete (A7): "
              "E267 'operator delete' must have one argument");
    TEST_SKIP1();
    test_skip("cxx03:operator delete(void*,nothrow_t)");
#endif
    void* pan = ::operator new[](32, STD::nothrow);
    test_ptr( pan );
    test_pass("cxx03:operator new[](size_t,nothrow_t)");
#if !defined(__WATCOMC__)
    ::operator delete[](pan, STD::nothrow);
    test_true( true );
    test_pass("cxx03:operator delete[](void*,nothrow_t)");
#else
    ::operator delete[](pan);
    TEST_NOTE("Open Watcom rejects a two-argument operator delete (A7): "
              "E267 'operator delete' must have one argument");
    TEST_SKIP1();
    test_skip("cxx03:operator delete[](void*,nothrow_t)");
#endif

    char buf[64];
    void* pp = ::operator new(4, (void*)buf);
    test_true( pp == (void*)buf );
    test_pass("cxx03:placement operator new(size_t,void*)");
    void* ppa = ::operator new[](4, (void*)buf);
    test_true( ppa == (void*)buf );
    test_pass("cxx03:placement operator new[](size_t,void*)");

#if !defined(__WATCOMC__)
    ::operator delete(pp, (void*)buf);
    test_true( true );
    test_pass("cxx03:placement operator delete(void*,void*)");
    ::operator delete[](ppa, (void*)buf);
    test_true( true );
    test_pass("cxx03:placement operator delete[](void*,void*)");
#else
    TEST_NOTE("Open Watcom rejects a two-argument operator delete (A7): "
              "E267 'operator delete' must have one argument");
    test_skip("cxx03:placement operator delete(void*,void*)");
    TEST_NOTE("Open Watcom rejects a two-argument operator delete (A7): "
              "E267 'operator delete' must have one argument");
    test_skip("cxx03:placement operator delete[](void*,void*)");
#endif

    bool threw = false;
    try { throw STD::bad_alloc(); }
    catch (const STD::bad_alloc& e) { threw = true; test_true( STD::string(e.what()).size() > 0 ); }
    test_true( threw );
    test_pass("cxx03:bad_alloc");
    test_pass("cxx03:bad_alloc::what");

#if TEST_TARGET_CXX >= 2014 && !defined(__WATCOMC__) && \
    (!defined(_MSC_VER) || _MSC_VER >= 1900) && \
    (defined(_MSC_VER) || __cplusplus >= 201402L)
    void* ps = ::operator new(48);
    ::operator delete(ps, (STD::size_t)48);
    test_pass("cxx14:sized operator delete(void*,size_t)");
    void* psa = ::operator new[](48);
    ::operator delete[](psa, (STD::size_t)48);
    test_pass("cxx14:sized operator delete[](void*,size_t)");
#else
    test_skip("cxx14:sized operator delete(void*,size_t)");
    test_skip("cxx14:sized operator delete[](void*,size_t)");
#endif
}
#else   // !TEST_HAS_EH
TEST_CASE(new_hdr, operators_and_bad_alloc) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx03:nothrow_t");
    test_skip("cxx03:nothrow");
    test_skip("cxx03:operator new(size_t)");
    test_skip("cxx03:operator delete(void*)");
    test_skip("cxx03:operator new[](size_t)");
    test_skip("cxx03:operator delete[](void*)");
    test_skip("cxx03:operator new(size_t,nothrow_t)");
    test_skip("cxx03:operator delete(void*,nothrow_t)");
    test_skip("cxx03:operator new[](size_t,nothrow_t)");
    test_skip("cxx03:operator delete[](void*,nothrow_t)");
    test_skip("cxx03:placement operator new(size_t,void*)");
    test_skip("cxx03:placement operator new[](size_t,void*)");
    test_skip("cxx03:placement operator delete(void*,void*)");
    test_skip("cxx03:placement operator delete[](void*,void*)");
    test_skip("cxx03:bad_alloc");
    test_skip("cxx03:bad_alloc::what");
    test_skip("cxx14:sized operator delete(void*,size_t)");
    test_skip("cxx14:sized operator delete[](void*,size_t)");
}
#endif  // TEST_HAS_EH

TEST_CASE(new_hdr, aligned_and_interference) {
#if TEST_TARGET_CXX >= 2017 && !defined(__WATCOMC__) && _TST_HAS_CXX17_LIB_MEMBERS

    STD::align_val_t al = (STD::align_val_t)32;
    test_true( (STD::size_t)al == 32 );
    test_pass("cxx17:align_val_t");
    void* pa = ::operator new(64, al);
    test_true( ((STD::size_t)pa % 32) == 0 );
    ::operator delete(pa, al);
    test_pass("cxx17:operator new(size_t,align_val_t)");
    test_pass("cxx17:operator delete(void*,align_val_t)");
    void* pan = ::operator new(64, al, STD::nothrow);
    test_ptr( pan );
    ::operator delete(pan, al);
    test_pass("cxx17:operator new(size_t,align_val_t,nothrow_t)");

    void* paa = ::operator new[](64, al);
    test_true( ((STD::size_t)paa % 32) == 0 );
    test_pass("cxx17:operator new[](size_t,align_val_t)");
    ::operator delete[](paa, al);
    test_true( true );
    test_pass("cxx17:operator delete[](void*,align_val_t)");

    void* paan = ::operator new[](64, al, STD::nothrow);
    test_ptr( paan );
    test_true( ((STD::size_t)paan % 32) == 0 );
    test_pass("cxx17:operator new[](size_t,align_val_t,nothrow_t)");
    ::operator delete[](paan, al, STD::nothrow);
    test_true( true );
    test_pass("cxx17:operator delete[](void*,align_val_t,nothrow_t)");

    void* pn2 = ::operator new(64, al, STD::nothrow);
    ::operator delete(pn2, al, STD::nothrow);
    test_true( true );
    test_pass("cxx17:operator delete(void*,align_val_t,nothrow_t)");

    void* psz = ::operator new(64, al);
    ::operator delete(psz, (STD::size_t)64, al);
    test_true( true );
    test_pass("cxx17:operator delete(void*,size_t,align_val_t)");

    void* pasz = ::operator new[](64, al);
    ::operator delete[](pasz, (STD::size_t)64, al);
    test_true( true );
    test_pass("cxx17:operator delete[](void*,size_t,align_val_t)");
#else
    test_skip("cxx17:align_val_t");
    test_skip("cxx17:operator new(size_t,align_val_t)");
    test_skip("cxx17:operator delete(void*,align_val_t)");
    test_skip("cxx17:operator new(size_t,align_val_t,nothrow_t)");
    test_skip("cxx17:operator new[](size_t,align_val_t)");
    test_skip("cxx17:operator delete[](void*,align_val_t)");
    test_skip("cxx17:operator new[](size_t,align_val_t,nothrow_t)");
    test_skip("cxx17:operator delete[](void*,align_val_t,nothrow_t)");
    test_skip("cxx17:operator delete(void*,align_val_t,nothrow_t)");
    test_skip("cxx17:operator delete(void*,size_t,align_val_t)");
    test_skip("cxx17:operator delete[](void*,size_t,align_val_t)");
#endif

#if defined(__cpp_lib_hardware_interference_size)
    test_true( STD::hardware_destructive_interference_size >= 1 );
    test_pass("cxx17:hardware_destructive_interference_size");
    test_true( STD::hardware_constructive_interference_size >= 1 );
    test_pass("cxx17:hardware_constructive_interference_size");
#else
    test_skip("cxx17:hardware_destructive_interference_size");
    test_skip("cxx17:hardware_constructive_interference_size");
#endif
}

TEST_CASE(new_hdr, destroying_delete_and_slt) {
#if TEST_TARGET_CXX >= 2020

    test_true( sizeof(STD::destroying_delete_t) >= 1 );
    STD::destroying_delete_t dd = STD::destroying_delete;
    (void)dd;
    test_pass("cxx20:destroying_delete");
    test_pass("cxx20:operator delete(void*,destroying_delete_t)");
#else
    test_skip("cxx20:destroying_delete");
    test_skip("cxx20:operator delete(void*,destroying_delete_t)");
#endif

#if defined(__cpp_lib_start_lifetime_as)
    alignas(int) unsigned char storage[sizeof(int)];
    int* ip = STD::start_lifetime_as<int>(storage);
    *ip = 7;
    test_eq( *ip, 7 );
    alignas(int) unsigned char arr[sizeof(int) * 3];
    int* ap = STD::start_lifetime_as_array<int>(arr, 3);
    ap[2] = 9;
    test_eq( ap[2], 9 );
#else
#endif
}

TEST_CASE(new_hdr, feature_test_macros) {
#if defined(__cpp_aligned_new) && (__cpp_aligned_new) >= 201606L
    test_true( (__cpp_aligned_new) >= 201606L );
    test_pass("cxx17:__cpp_aligned_new=201606L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx17:__cpp_aligned_new=201606L");
#endif
#if defined(__cpp_lib_hardware_interference_size) && (__cpp_lib_hardware_interference_size) >= 201703L
    test_true( (__cpp_lib_hardware_interference_size) >= 201703L );
    test_pass("cxx17:__cpp_lib_hardware_interference_size=201703L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx17:__cpp_lib_hardware_interference_size=201703L");
#endif
#if defined(__cpp_lib_launder) && (__cpp_lib_launder) >= 201606L
    test_true( (__cpp_lib_launder) >= 201606L );
    test_pass("cxx17:__cpp_lib_launder=201606L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx17:__cpp_lib_launder=201606L");
#endif
#if defined(__cpp_lib_destroying_delete) && (__cpp_lib_destroying_delete) >= 201806L
    test_true( (__cpp_lib_destroying_delete) >= 201806L );
    test_pass("cxx20:__cpp_lib_destroying_delete=201806L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx20:__cpp_lib_destroying_delete=201806L");
#endif
#if defined(__cpp_lib_constexpr_new) && (__cpp_lib_constexpr_new) >= 202406L
    test_true( (__cpp_lib_constexpr_new) >= 202406L );
    test_pass("cxx26:__cpp_lib_constexpr_new=202406L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx26:__cpp_lib_constexpr_new=202406L");
#endif
}

TEST_CASE(new_hdr, constexpr_placement_new_cxx26) {
#if defined(__cpp_lib_constexpr_new) && (__cpp_lib_constexpr_new) >= 202406L \
    && (!defined(__cpp_constexpr) || (__cpp_constexpr) >= 202406L)
    {
        struct K {
            static constexpr int one() {
                typedef int T;
                STD::allocator<T> al;
                T* p = al.allocate(1);
                ::new (p) T(7);
                T r = *p;
                p->~T();
                al.deallocate(p, 1);
                return r;
            }
            static constexpr int many() {
                STD::allocator<int> al;
                int* p = al.allocate(3);
                ::new (p) int[3]{1, 2, 3};
                int r = p[0] + p[1] + p[2];
                al.deallocate(p, 3);
                return r;
            }
        };
        static_assert(K::one() == 7, "placement new in a constant expression");
        test_eq( K::one(), 7 );
        test_pass("cxx26:constexpr placement operator new(size_t,void*)");
#if !defined(__clang__)
        static_assert(K::many() == 6, "placement new[] in a constant expression");
#endif
        test_eq( K::many(), 6 );
        test_pass("cxx26:constexpr placement operator new[](size_t,void*)");
    }
#else
    TEST_NOTE("constexpr placement new is C++26 (P2747R2); not in this library");
    test_skip("cxx26:constexpr placement operator new(size_t,void*)");
    test_skip("cxx26:constexpr placement operator new[](size_t,void*)");
#endif
}
