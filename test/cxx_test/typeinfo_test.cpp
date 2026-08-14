#include "test_cxx.hpp"

#include <typeinfo>
#include <type_traits>
#include <string>

TEST_CASE(typeinfo, basics) {
    const STD::type_info& a = typeid(int);
    const STD::type_info& b = typeid(int);
    const STD::type_info& d = typeid(double);
    test_true( a == b );
    test_true( a != d );
    test_pass("cxx03:type_info::operator==");
    test_true( STD::string(a.name()).size() > 0 );
    test_pass("cxx03:type_info::name");
}

#if _TST_HAS_CXX11_LIB_MEMBERS
TEST_CASE(typeinfo, hash_code) {

    test_eq( typeid(int).hash_code(), typeid(int).hash_code() );
    test_eq( typeid(double).hash_code(), typeid(double).hash_code() );
    test_eq( typeid(char).hash_code(), typeid(char).hash_code() );

    test_true( typeid(int).hash_code() != typeid(double).hash_code() );
    test_true( typeid(int).hash_code() != typeid(char).hash_code() );

    const STD::type_info& a = typeid(long);
    const STD::type_info& b = typeid(long);
    test_true( a == b );
    test_eq( a.hash_code(), b.hash_code() );
    test_pass("cxx11:type_info::hash_code");
}
#else
TEST_CASE_SKIP(typeinfo, hash_code)
#endif


TEST_CASE(typeinfo, ordering_and_inequality) {
    const STD::type_info& a = typeid(int);
    const STD::type_info& b = typeid(double);

    test_true( a != b );
    test_true( !(a != typeid(int)) );
    test_pass("cxx03:type_info::operator!=");

    bool ab = a.before(b);
    bool ba = b.before(a);
    test_true( ab != ba );
    test_true( !a.before(a) );
    test_pass("cxx03:type_info::before");
}

#if TEST_HAS_EH
TEST_CASE(typeinfo, bad_cast_and_bad_typeid) {

    STD::bad_cast bc;
    const STD::exception& e1 = bc;
    test_true( STD::string(e1.what()).size() > 0 );
    bool caught = false;
    try { throw STD::bad_cast(); } catch (const STD::exception&) { caught = true; }
    test_true( caught );
    test_pass("cxx03:bad_cast");

    STD::bad_typeid bt;
    const STD::exception& e2 = bt;
    test_true( STD::string(e2.what()).size() > 0 );
    bool caught2 = false;
    try { throw STD::bad_typeid(); } catch (const STD::exception&) { caught2 = true; }
    test_true( caught2 );
    test_pass("cxx03:bad_typeid");
}
#else   // !TEST_HAS_EH
TEST_CASE(typeinfo, bad_cast_and_bad_typeid) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx03:bad_cast");
    test_skip("cxx03:bad_typeid");
}
#endif  // TEST_HAS_EH


namespace { struct CcwTiBase { virtual ~CcwTiBase() {} }; struct CcwTiDerived : CcwTiBase {}; }

TEST_CASE(typeinfo, class_cxx03) {
    const STD::type_info& ti = typeid(int);
    const STD::type_info& tj = typeid(int);
    const STD::type_info& td = typeid(double);

    test_true( ti == tj );
    test_true( ti != td );
    test_true( &ti == &tj || ti == tj );
    test_true( ti.name() != 0 );
    test_true( (ti.before(td)) != (td.before(ti)) );

    CcwTiDerived d;
    CcwTiBase& b = d;
    test_true( typeid(b) == typeid(CcwTiDerived) );
    test_true( typeid(b) != typeid(CcwTiBase) );

    TEST_SKIP_VC120("MSVC <= 12 reports an inaccessible/deleted copy assignment as assignable (vc bugs A26)");
    test_true( !STD::is_copy_assignable<STD::type_info>::value );
    test_pass("cxx03:type_info");
}
