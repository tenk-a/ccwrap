#include "test_cxx.hpp"

#include <typeindex>
#include <map>
#include <utility>
#include <cstring>
#include <typeinfo>
#include <string>
#include <functional>

TEST_CASE(typeindex, compare) {
    STD::type_index i1(typeid(int));
    STD::type_index i2(typeid(int));
    STD::type_index d(typeid(double));

    test_true( i1 == i2 );
    test_true( i1 != d );
    test_true( !(i1 < i2) && !(i2 < i1) );
    test_true( (i1 < d) || (d < i1) );
    test_true( i1 <= i2 );
    test_true( i1 >= i2 );

    test_true( (i1 < d) != (i1 > d) );
    test_true( (i1 < d) ? (d > i1) : (d < i1) );

    STD::type_index copy = i1;
    test_true( copy == i1 );

    test_true( i1.name() != 0 );
    test_eq( i1.hash_code(), i2.hash_code() );
    test_pass("cxx11:type_index comparisons");
}

TEST_CASE(typeindex, as_hash_key) {
    STD::type_index a(typeid(int));
    STD::type_index b(typeid(double));
    STD::hash<STD::type_index> h;
    test_eq( h(a), a.hash_code() );
    test_true( h(a) != h(b) || a.name() == b.name() );
    test_pass("cxx11:type_index as unordered_map key (hash)");
}

TEST_CASE(typeindex, accessors_hash_spaceship) {
    STD::type_index ti(typeid(int));
    STD::type_index tj(typeid(int));
    STD::type_index tk(typeid(double));

    test_true( STD::string(ti.name()) == typeid(int).name() );
    test_pass("cxx11:type_index::name");

    test_eq( ti.hash_code(), tj.hash_code() );
#if _TST_HAS_CXX11_LIB_MEMBERS
    test_eq( ti.hash_code(), typeid(int).hash_code() );
#endif
    test_pass("cxx11:type_index::hash_code");

    STD::hash<STD::type_index> h;
    test_eq( h(ti), ti.hash_code() );
    test_eq( h(ti), h(tj) );
    test_pass("cxx11:hash<type_index>");

#if defined(__cpp_impl_three_way_comparison) && defined(__cpp_lib_three_way_comparison)
    test_true( (ti <=> tj) == 0 );
    bool ij = (ti <=> tk) < 0;
    bool ji = (tk <=> ti) < 0;
    test_true( ij != ji );
    test_pass("cxx20:type_index::operator<=>");
#else
    TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx20:type_index::operator<=>");
#endif
}


TEST_CASE(typeindex, class_cxx11) {
    STD::type_index a(typeid(int));
    STD::type_index b(typeid(int));
    STD::type_index c(typeid(double));

    test_true( a == b );
    test_true( a != c );
    test_true( (a < c) != (c < a) );
    test_true( a.hash_code() == b.hash_code() );
    test_true( STD::strcmp(a.name(), typeid(int).name()) == 0 );

    STD::type_index copy = c;
    copy = a;
    test_true( copy == a );

    STD::map<STD::type_index, int> m;
    m.insert(STD::pair<const STD::type_index, int>(a, 1));
    m.insert(STD::pair<const STD::type_index, int>(c, 2));
    test_eq( m[STD::type_index(typeid(int))], 1 );
    test_eq( (int)m.size(), 2 );
    test_pass("cxx11:type_index");
}
