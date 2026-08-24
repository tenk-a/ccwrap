#include "test_cxx.hpp"

#if _TST_TEST_HAS(<version>)
#  include <version>
#endif

#include <type_traits>
#include <functional>
#include <cstddef>

#if defined(_MSVC_LANG)
#  define _TST_TT_LANG _MSVC_LANG
#elif defined(__WATCOMC__)
#  define _TST_TT_LANG 199711L
#else
#  define _TST_TT_LANG __cplusplus
#endif

namespace { enum PlainEnum { EV_A = 1, EV_B = 2 }; struct Empty {}; struct Poly { virtual ~Poly() {} }; }

namespace {
    union TTUnion { int a; double b; };
    struct TTAgg { int x; double y; };
    struct TTNonAgg { TTNonAgg(); int x; };
    struct TTAbstract { virtual void f() = 0; };
    struct TTBase {};
    struct TTDerived : TTBase { int y; };
#if _TST_HAS_FINAL
    struct TTFinalCls final { int x; };
#endif
    typedef long (*TTFnPtr)(int);
    struct TTLayoutA { int x; double y; };
    struct TTLayoutB { int x; char z; };
    struct TTVBase { int v; };
    struct TTVDerived : virtual TTVBase { int w; };
}
#if defined(__cpp_lib_is_scoped_enum)
namespace { enum class TTScopedEnum { X, Y }; }
#endif

TEST_CASE(type_traits, primary_categories) {
    test_true(  STD::is_void<void>::value );
    test_true( !STD::is_void<int>::value );
    test_pass("cxx11:is_void");
    test_true(  STD::is_integral<int>::value );
    test_true(  STD::is_integral<unsigned char>::value );
    test_true( !STD::is_integral<double>::value );
    test_pass("cxx11:is_integral");
    test_true(  STD::is_floating_point<double>::value );
    test_pass("cxx11:is_floating_point");
    test_true(  STD::is_arithmetic<int>::value );
    test_pass("cxx11:is_arithmetic");
    test_true(  STD::is_fundamental<int>::value );
    test_pass("cxx11:is_fundamental");
    test_true(  STD::is_pointer<int*>::value );
    test_true( !STD::is_pointer<int>::value );
    test_pass("cxx11:is_pointer");
    test_true(  STD::is_reference<int&>::value );
    test_pass("cxx11:is_reference");
    test_true(  STD::is_lvalue_reference<int&>::value );
    test_pass("cxx11:is_lvalue_reference");
    TEST_SKIP_WAT("bounded array T[N] does not match a partial specialization (A12)");
    test_true(  STD::is_array<int[3]>::value );
    test_pass("cxx11:is_array");

    test_true(  STD::is_enum<PlainEnum>::value );
    test_true( !STD::is_enum<Empty>::value );
    test_true( !STD::is_enum<int>::value );
    test_pass("cxx11:is_enum");
#if 1   /* is_class / is_object / is_base_of are intrinsic-free in llibcxx03 */
    test_true(  STD::is_class<Empty>::value );
    test_true(  STD::is_object<int>::value );
    test_pass("cxx11:is_class");
#else
    TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx11:is_class");
#endif
    test_true(  STD::is_scalar<int>::value );
    test_pass("cxx11:is_scalar");
    test_true(  STD::is_compound<int*>::value );
    test_pass("cxx11:is_compound");
}

TEST_CASE(type_traits, cv_qualified_categories) {
    test_true(  STD::is_void<const void>::value );
    test_true(  STD::is_void<volatile void>::value );
    test_true(  STD::is_void<const volatile void>::value );
    test_pass("cxx11:is_void (cv)");

    test_true(  STD::is_integral<const int>::value );
    test_true(  STD::is_integral<volatile unsigned char>::value );
    test_true(  STD::is_integral<const volatile long>::value );
    test_true( !STD::is_integral<const double>::value );
    test_pass("cxx11:is_integral (cv)");

    test_true(  STD::is_floating_point<const double>::value );
    test_true(  STD::is_floating_point<const volatile float>::value );
    test_pass("cxx11:is_floating_point (cv)");

    test_true(  STD::is_pointer<int* const>::value );
    test_true(  STD::is_pointer<const int*>::value );
    test_true(  STD::is_pointer<int* const volatile>::value );
    test_pass("cxx11:is_pointer (cv)");

    test_true(  STD::is_signed<const int>::value );
    test_pass("cxx11:is_signed (cv)");
    test_true(  STD::is_unsigned<const volatile unsigned>::value );
    test_pass("cxx11:is_unsigned (cv)");

    test_true(  STD::is_arithmetic<const int>::value );
    test_pass("cxx11:is_arithmetic (cv)");
    test_true(  STD::is_fundamental<const volatile double>::value );
    test_pass("cxx11:is_fundamental (cv)");

    test_true( (STD::is_same<STD::remove_cv<const volatile int>::type, int>::value) );
    test_true( (STD::is_same<STD::remove_cv<const int*>::type, const int*>::value) );
    test_pass("cxx11:remove_cv");
    test_true( (STD::is_same<STD::decay<const int&>::type, int>::value) );
    test_pass("cxx11:decay");
}

TEST_CASE(type_traits, properties) {
    test_true(  STD::is_const<const int>::value );
    test_true( !STD::is_const<int>::value );
    test_pass("cxx11:is_const");
    test_true(  STD::is_volatile<volatile int>::value );
    test_pass("cxx11:is_volatile");
    test_true(  STD::is_signed<int>::value );
    test_pass("cxx11:is_signed");
    test_true(  STD::is_unsigned<unsigned>::value );
    test_pass("cxx11:is_unsigned");
    test_true(  STD::is_empty<Empty>::value );
    test_true( !STD::is_empty<TTAgg>::value );
    test_pass("cxx11:is_empty");
    test_true(  STD::is_polymorphic<Poly>::value );
    test_true( !STD::is_polymorphic<Empty>::value );
    test_pass("cxx11:is_polymorphic");
    test_true(  (STD::alignment_of<double>::value >= 1u) );
    test_pass("cxx11:alignment_of");

    TEST_SKIP_WAT("bounded array T[N] does not match a partial specialization (A12)");
    test_eq( (int)STD::rank<int[3][4]>::value, 2 );
    test_pass("cxx11:rank");
    TEST_SKIP_WAT("bounded array T[N] does not match a partial specialization (A12)");
    test_eq( (int)STD::extent<int[3][4]>::value, 3 );
    test_pass("cxx11:extent");
}

TEST_CASE(type_traits, relations) {
    test_true(  (STD::is_same<int, int>::value) );
    test_true( !(STD::is_same<int, long>::value) );
    test_pass("cxx11:is_same");
#if 1   /* is_base_of is intrinsic-free in llibcxx03 */
    test_true(  (STD::is_base_of<Poly, Poly>::value) );
    test_pass("cxx11:is_base_of");
#else
    TEST_SKIP1();
    test_skip("cxx11:is_base_of");
#endif
    test_true(  (STD::is_convertible<int, double>::value) );
    test_true( !(STD::is_convertible<int*, double*>::value) );
    test_pass("cxx11:is_convertible");
}

TEST_CASE(type_traits, transforms) {
    test_true( (STD::is_same<STD::remove_const<const int>::type, int>::value) );
    test_pass("cxx11:remove_const");
    test_true( (STD::is_same<STD::remove_volatile<volatile int>::type, int>::value) );
    test_pass("cxx11:remove_volatile");
    test_true( (STD::is_same<STD::remove_cv<const volatile int>::type, int>::value) );
    test_pass("cxx11:remove_cv (transforms)");
    test_true( (STD::is_same<STD::remove_reference<int&>::type, int>::value) );
    test_pass("cxx11:remove_reference");
    test_true( (STD::is_same<STD::remove_pointer<int*>::type, int>::value) );
    test_pass("cxx11:remove_pointer");
    test_true( (STD::is_same<STD::add_pointer<int>::type, int*>::value) );
    test_pass("cxx11:add_pointer");
    test_true( (STD::is_same<STD::add_const<int>::type, const int>::value) );
    test_pass("cxx11:add_const");
    test_true( (STD::is_same<STD::make_unsigned<int>::type, unsigned int>::value) );
    test_pass("cxx11:make_unsigned");
    test_true( (STD::is_same<STD::make_signed<unsigned>::type, int>::value) );
    test_pass("cxx11:make_signed");
    test_true( (STD::is_same<STD::conditional<true, int, long>::type, int>::value) );
    test_true( (STD::is_same<STD::conditional<false, int, long>::type, long>::value) );
    test_pass("cxx11:conditional");
    TEST_SKIP_WAT("bounded array T[N] does not match a partial specialization (A12)");
    test_true( (STD::is_same<STD::remove_extent<int[3]>::type, int>::value) );
    test_pass("cxx11:remove_extent");

    test_true( (STD::is_same<STD::underlying_type<PlainEnum>::type, STD::underlying_type<PlainEnum>::type>::value) );
    test_true( sizeof(STD::underlying_type<PlainEnum>::type) == sizeof(PlainEnum) );
    test_pass("cxx11:underlying_type");
}

TEST_CASE(type_traits, helpers) {
    { int __ic = STD::integral_constant<int, 7>::value;
      test_eq( __ic, 7 ); }
    test_pass("cxx11:integral_constant");
    test_true( STD::true_type::value );
    test_pass("cxx11:true_type");
    test_true( !STD::false_type::value );
    test_pass("cxx11:false_type");
    test_true( (STD::is_same<STD::enable_if<true, int>::type, int>::value) );
    test_true( (STD::is_same<STD::enable_if<true>::type, void>::value) );
    test_pass("cxx11:enable_if");
}

typedef int func_type(int);

TEST_CASE(type_traits, is_function) {
    test_true(  STD::is_function<func_type>::value );
    test_true(  STD::is_function<void()>::value );
    test_true( !STD::is_function<int>::value );
    test_true( !STD::is_function<int*>::value );
    test_true( !STD::is_function<int&>::value );
    test_true( !STD::is_function<void>::value );
    test_true( !STD::is_function<Empty>::value );
    test_true( !STD::is_function<func_type*>::value );
    test_pass("cxx11:is_function");
}

TEST_CASE(type_traits, is_object) {
    test_true(  STD::is_object<int>::value );
    test_true(  STD::is_object<int*>::value );
    test_true(  STD::is_object<Empty>::value );
    test_true(  STD::is_object<int[3]>::value );
    test_true( !STD::is_object<void>::value );
    test_true( !STD::is_object<int&>::value );
    test_true( !STD::is_object<func_type>::value );
    test_pass("cxx11:is_object");
}

TEST_CASE(type_traits, type_identity_and_negation) {
#if TEST_TARGET_CXX >= 2020
    test_true( (STD::is_same<STD::type_identity<int>::type, int>::value) );
    test_true( (STD::is_same<STD::type_identity<const int>::type, const int>::value) );
    test_true( (STD::is_same<STD::type_identity<int&>::type, int&>::value) );
    test_pass("cxx20:type_identity");
#else
    TEST_NOTE("type_identity is C++20");
    test_skip("cxx20:type_identity");
#endif

#if TEST_TARGET_CXX >= 2017
    test_true(  STD::negation<STD::false_type>::value );
    test_true( !STD::negation<STD::true_type>::value );
    test_true( (STD::negation<STD::integral_constant<bool, false> >::value) );
    test_pass("cxx17:negation");
#else
    TEST_NOTE("negation is C++17");
    test_skip("cxx17:negation");
#endif
}

namespace {
    struct CtPod { int x; };
    struct CtHasCtor { CtHasCtor(int) {} };
    struct CtNoCopy { CtNoCopy(); private: CtNoCopy(const CtNoCopy&); };
    struct CtNoDflt { CtNoDflt(const CtNoDflt&); };
    struct AsNoAssign { AsNoAssign(); private: AsNoAssign& operator=(const AsNoAssign&); };
    struct DtOk { int x; };
    struct DtHasDtor { ~DtHasDtor() {} };
    struct DtNoDtor { private: ~DtNoDtor() {} };
}

namespace { struct MpS { int m; void f() {} int g(int) { return 0; } }; }

TEST_CASE(type_traits, member_pointers_and_remove_cvref) {
    typedef MpS S;

    test_true(  STD::is_member_object_pointer<int S::*>::value );
    test_true( !STD::is_member_object_pointer<void (S::*)()>::value );
    test_true( !STD::is_member_object_pointer<int>::value );
    test_true( !STD::is_member_object_pointer<int*>::value );
    test_pass("cxx11:is_member_object_pointer");

    test_true(  STD::is_member_function_pointer<void (S::*)()>::value );
    test_true(  STD::is_member_function_pointer<int (S::*)(int)>::value );
    test_true( !STD::is_member_function_pointer<int S::*>::value );
    test_true( !STD::is_member_function_pointer<int>::value );
    test_true( !STD::is_member_function_pointer<int (*)(int)>::value );
    test_pass("cxx11:is_member_function_pointer");

    test_true( STD::is_member_pointer<int S::*>::value );
    test_true( STD::is_member_pointer<void (S::*)()>::value );
    test_pass("cxx11:is_member_pointer");

    test_true( STD::is_member_object_pointer<int S::* const>::value );
    test_true( STD::is_member_function_pointer<void (S::* const)()>::value );
    test_pass("cxx11:is_member_pointer (cv)");

#if TEST_TARGET_CXX >= 2020
    test_true( (STD::is_same<STD::remove_cvref<int>::type, int>::value) );
    test_true( (STD::is_same<STD::remove_cvref<const int>::type, int>::value) );
    test_true( (STD::is_same<STD::remove_cvref<int&>::type, int>::value) );
    test_true( (STD::is_same<STD::remove_cvref<const int&>::type, int>::value) );
    test_true( (STD::is_same<STD::remove_cvref<volatile int&>::type, int>::value) );
    test_true( (STD::is_same<STD::remove_cvref<const volatile int&>::type, int>::value) );
    test_true( (STD::is_same<STD::remove_cvref<int* const&>::type, int*>::value) );
    test_pass("cxx20:remove_cvref");
#else
    TEST_NOTE("remove_cvref is C++20");
    test_skip("cxx20:remove_cvref");
#endif

    test_true( !STD::is_rvalue_reference<int>::value );
    test_true( !STD::is_rvalue_reference<int&>::value );
    test_true(  STD::is_lvalue_reference<int&>::value );
    test_pass("cxx11:is_rvalue_reference");
}

#if TEST_TARGET_CXX >= 2017
TEST_CASE(type_traits, conjunction_disjunction) {
    using STD::true_type; using STD::false_type;
    typedef STD::integral_constant<int, 3> three;

    test_true(  STD::conjunction<>::value );
    test_true(  STD::conjunction<true_type>::value );
    test_true( !STD::conjunction<false_type>::value );
    test_true(( STD::conjunction<true_type, true_type, true_type>::value ));
    test_true(( !STD::conjunction<true_type, false_type, true_type>::value ));
    test_true(( STD::conjunction<true_type, true_type, true_type, true_type, true_type>::value ));
    test_true(( !STD::conjunction<true_type, true_type, true_type, true_type, false_type>::value ));

    test_true(( STD::conjunction<true_type, true_type, true_type, true_type, three>::value == 3 ));
    test_true(( STD::conjunction<true_type, three>::value != 0 ));
    test_pass("cxx17:conjunction");

    test_true( !STD::disjunction<>::value );
    test_true(  STD::disjunction<true_type>::value );
    test_true( !STD::disjunction<false_type>::value );
    test_true(( STD::disjunction<false_type, false_type, true_type>::value ));
    test_true(( !STD::disjunction<false_type, false_type, false_type>::value ));
    test_true(( STD::disjunction<false_type, false_type, false_type, false_type, true_type>::value ));

    test_true(( STD::disjunction<false_type, three>::value == 3 ));
    test_pass("cxx17:disjunction");
}
#else
TEST_CASE(type_traits, conjunction_disjunction) {
    TEST_NOTE("conjunction / disjunction are C++17");
    test_skip("cxx17:conjunction");
    test_skip("cxx17:disjunction");
}
#endif

TEST_CASE(type_traits, is_destructible) {
    typedef DtOk Ok;
    typedef DtHasDtor HasDtor;
    typedef DtNoDtor NoDtor;

    test_true(  STD::is_destructible<int>::value );
    test_true(  STD::is_destructible<double>::value );
    test_true(  STD::is_destructible<int*>::value );
    test_true(  STD::is_destructible<Ok>::value );
    test_true(  STD::is_destructible<HasDtor>::value );
    TEST_SKIP_VC120("MSVC before VS2015 has no expression SFINAE: the answer here needs it");
    test_true( !STD::is_destructible<NoDtor>::value );
    TEST_SKIP_VC_RANGE(1600, 1800, "vc10/vc11 answer true for void here");
    test_true( !STD::is_destructible<void>::value );
    TEST_SKIP_VC_RANGE(1600, 1800, "vc10/vc11 answer true for void here");
    test_true( !STD::is_destructible<const void>::value );
    test_true(  STD::is_destructible<int&>::value );
    test_true(  STD::is_destructible<const int>::value );
    test_pass("cxx11:is_destructible");

    test_true(  STD::is_nothrow_destructible<int>::value );
    test_true(  STD::is_nothrow_destructible<Ok>::value );
    TEST_SKIP_VC110("MSVC before VS2013 has no expression SFINAE: cannot detect a private destructor");
    test_true( !STD::is_nothrow_destructible<NoDtor>::value );
    TEST_SKIP_VC_RANGE(1600, 1800, "vc10/vc11 answer true for void here");
    test_true( !STD::is_nothrow_destructible<void>::value );
    test_pass("cxx11:is_nothrow_destructible");
}

TEST_CASE(type_traits, is_constructible_family) {
    typedef CtPod Pod;
    typedef CtHasCtor HasCtor;
    typedef CtNoCopy NoCopy;
    typedef CtNoDflt NoDflt;

    test_true(  STD::is_default_constructible<int>::value );
    test_true(  STD::is_default_constructible<Pod>::value );
    TEST_SKIP_VC090("MSVC 9 has no expression SFINAE: cannot detect a missing default constructor");
    test_true( !STD::is_default_constructible<HasCtor>::value );
    TEST_SKIP_VC090("MSVC 9 has no expression SFINAE: cannot detect a missing default constructor");
    test_true( !STD::is_default_constructible<NoDflt>::value );
    test_true( !STD::is_default_constructible<int&>::value );
    TEST_SKIP_VC_RANGE(1600, 1800, "vc10/vc11 answer true for void here");
    test_true( !STD::is_default_constructible<void>::value );
    TEST_SKIP_VC_RANGE(1600, 1800, "vc10/vc11 answer true for void here");
    test_true( !STD::is_default_constructible<const void>::value );
    test_pass("cxx11:is_default_constructible");

    test_true(  STD::is_copy_constructible<int>::value );
    test_true(  STD::is_copy_constructible<int*>::value );
    test_true(  STD::is_copy_constructible<Pod>::value );
    test_true(  STD::is_copy_constructible<NoDflt>::value );
    TEST_SKIP_WAT("Open Watcom does not apply access control during template argument deduction, so an inaccessible copy constructor is not detected");
    TEST_SKIP_VC110("MSVC before VS2013 has no expression SFINAE: cannot detect a private copy constructor");
    test_true( !STD::is_copy_constructible<NoCopy>::value );
    TEST_SKIP_VC_RANGE(1600, 1800, "vc10/vc11 answer false for a reference, which is copy-constructible");
    test_true(  STD::is_copy_constructible<int&>::value );
    TEST_SKIP_VC_RANGE(1600, 1800, "vc10/vc11 answer true for void here");
    test_true( !STD::is_copy_constructible<void>::value );
    test_pass("cxx11:is_copy_constructible");

    test_true(  STD::is_move_constructible<int>::value );
    test_true(  STD::is_move_constructible<Pod>::value );
    TEST_SKIP_WAT("Open Watcom does not apply access control during template argument deduction, so an inaccessible copy constructor is not detected");
    TEST_SKIP_VC110("MSVC before VS2013 has no expression SFINAE: cannot detect a private copy constructor");
    test_true( !STD::is_move_constructible<NoCopy>::value );
    test_pass("cxx11:is_move_constructible");

    test_true(  STD::is_nothrow_default_constructible<Pod>::value );
    TEST_SKIP_VC090("MSVC 9 has no expression SFINAE: cannot detect a missing default constructor");
    test_true( !STD::is_nothrow_default_constructible<HasCtor>::value );
    test_pass("cxx11:is_nothrow_default_constructible");
    test_true(  STD::is_nothrow_copy_constructible<Pod>::value );
    TEST_SKIP_WAT("Open Watcom does not apply access control during template argument deduction, so an inaccessible copy constructor is not detected");
    TEST_SKIP_VC110("MSVC before VS2013 has no expression SFINAE: cannot detect a private copy constructor");
    test_true( !STD::is_nothrow_copy_constructible<NoCopy>::value );
    test_pass("cxx11:is_nothrow_copy_constructible");
    test_true(  STD::is_nothrow_move_constructible<int>::value );
    test_pass("cxx11:is_nothrow_move_constructible");
}

TEST_CASE(type_traits, is_assignable_family) {
    typedef CtPod Pod;
    typedef AsNoAssign NoAssign;

    test_true((  STD::is_assignable<int&, int>::value ));
    test_true((  STD::is_assignable<Pod&, const Pod&>::value ));
    TEST_SKIP_VC120("MSVC before VS2015 has no expression SFINAE: the answer here needs it");
    test_true(( !STD::is_assignable<const int&, int>::value ));
    TEST_SKIP_VC_RANGE(1600, 1800, "vc10/vc11 miss that an rvalue int is not assignable");
    test_true(( !STD::is_assignable<int, int>::value ));
    test_true(( !STD::is_assignable<void, int>::value ));
    test_pass("cxx11:is_assignable");

    test_true(  STD::is_copy_assignable<int>::value );
    test_true(  STD::is_copy_assignable<Pod>::value );
    TEST_SKIP_VC120("MSVC before VS2015 has no expression SFINAE: the answer here needs it");
    test_true( !STD::is_copy_assignable<const int>::value );
    TEST_SKIP_VC120("MSVC before VS2015 has no expression SFINAE: the answer here needs it");
    test_true( !STD::is_copy_assignable<NoAssign>::value );
    TEST_SKIP_VC_RANGE(1600, 1800, "vc10/vc11 answer true for void here");
    test_true( !STD::is_copy_assignable<void>::value );
    test_pass("cxx11:is_copy_assignable");
    test_true(  STD::is_move_assignable<int>::value );
    test_true(  STD::is_move_assignable<Pod>::value );
    TEST_SKIP_VC120("MSVC before VS2015 has no expression SFINAE: the answer here needs it");
    test_true( !STD::is_move_assignable<NoAssign>::value );
    test_pass("cxx11:is_move_assignable");

    test_true((  STD::is_nothrow_assignable<int&, int>::value ));
    TEST_SKIP_VC120("MSVC before VS2015 has no expression SFINAE: the answer here needs it");
    test_true((  !STD::is_nothrow_assignable<const int&, int>::value ));
    test_pass("cxx11:is_nothrow_assignable");
    test_true(  STD::is_nothrow_copy_assignable<Pod>::value );
    TEST_SKIP_VC090("MSVC 9 has no expression SFINAE: cannot detect a private operator=");
    test_true( !STD::is_nothrow_copy_assignable<NoAssign>::value );
    test_pass("cxx11:is_nothrow_copy_assignable");
    TEST_SKIP_VC_RANGE(1600, 1800, "vc10/vc11 have no noexcept deduction for the implicit assignment");
    test_true(  STD::is_nothrow_move_assignable<int>::value );
    test_pass("cxx11:is_nothrow_move_assignable");
}

TEST_CASE(type_traits, transform_traits_more) {
    test_true(( STD::is_same<STD::add_cv<int>::type, const volatile int>::value ));
    test_pass("cxx11:add_cv");
    test_true(( STD::is_same<STD::add_volatile<int>::type, volatile int>::value ));
    test_pass("cxx11:add_volatile");

    test_true(( STD::is_same<STD::add_lvalue_reference<int>::type, int&>::value ));
    test_true(( STD::is_same<STD::add_lvalue_reference<void>::type, void>::value ));
    test_pass("cxx11:add_lvalue_reference");

#if _TST_HAS_RVALUE_REF
    test_true(  STD::is_rvalue_reference< STD::add_rvalue_reference<int>::type >::value );
    test_true(( STD::is_same<STD::add_rvalue_reference<void>::type, void>::value ));
    test_pass("cxx11:add_rvalue_reference");
#else
    TEST_SKIP_N(2);
    test_skip("cxx11:add_rvalue_reference");
#endif
    TEST_SKIP_WAT("bounded array T[N] does not match a partial specialization (A12)");
    test_true(( STD::is_same<STD::remove_all_extents<int[3][4]>::type, int>::value ));
    test_true(( STD::is_same<STD::remove_all_extents<int>::type, int>::value ));
    test_pass("cxx11:remove_all_extents");

#if 1   /* llibcxx03 writes common_type without an intrinsic */
    test_true(( STD::is_same<STD::common_type<int, long>::type, long>::value ));
    test_true(( STD::is_same<STD::common_type<int, int>::type, int>::value ));
    test_pass("cxx11:common_type");
#else
    TEST_SKIP_N(2);
    test_skip("cxx11:common_type");
#endif
}

#if defined(__WATCOMC__)
TEST_CASE(type_traits, class_property_traits) {
    TEST_NOTE("Open Watcom has no __is_union intrinsic and a union cannot be told from a "
              "class in the language (A16)");
    test_skip("cxx11:is_union");
    TEST_NOTE("Open Watcom has no __has_virtual_destructor intrinsic; is_polymorphic "
              "cannot tell a virtual destructor from any other virtual function (A16)");
    test_skip("cxx11:has_virtual_destructor");
    test_true(  STD::is_abstract<TTAbstract>::value );
    test_true( !STD::is_abstract<Empty>::value );
    test_pass("cxx11:is_abstract");
    TEST_NOTE("no triviality intrinsic: llibcxx03 answers true only where the type "
              "category alone guarantees it (scalars and arrays of scalars)");
    test_true(  STD::is_standard_layout<int>::value );
    test_true( !STD::is_standard_layout<Poly>::value );
    test_pass("cxx11:is_standard_layout");
    TEST_NOTE("no triviality intrinsic: llibcxx03 answers true only where the type "
              "category alone guarantees it (scalars and arrays of scalars)");
    test_true(  STD::is_trivial<int>::value );
    test_true( !STD::is_trivial<Poly>::value );
    test_pass("cxx11:is_trivial");
    TEST_NOTE("no triviality intrinsic: llibcxx03 answers true only where the type "
              "category alone guarantees it (scalars and arrays of scalars)");
    test_true(  STD::is_trivially_copyable<int>::value );
    test_true( !STD::is_trivially_copyable<Poly>::value );
    test_pass("cxx11:is_trivially_copyable");
    TEST_NOTE("no triviality intrinsic: llibcxx03 answers true only where the type "
              "category alone guarantees it (scalars and arrays of scalars)");
    test_true(  STD::is_pod<int>::value );
    test_true( !STD::is_pod<Poly>::value );
    test_pass("cxx11:is_pod");
}
#else
TEST_CASE(type_traits, class_property_traits) {
    test_true(  STD::is_union<TTUnion>::value );
    test_true( !STD::is_union<Empty>::value );
    test_pass("cxx11:is_union");
    test_true(  STD::has_virtual_destructor<Poly>::value );
    test_true( !STD::has_virtual_destructor<Empty>::value );
    test_pass("cxx11:has_virtual_destructor");
    test_true(  STD::is_abstract<TTAbstract>::value );
    test_true( !STD::is_abstract<Empty>::value );
    test_pass("cxx11:is_abstract");
    test_true(  STD::is_standard_layout<TTAgg>::value );
    test_true( !STD::is_standard_layout<Poly>::value );
    test_pass("cxx11:is_standard_layout");
    test_true(  STD::is_trivial<TTAgg>::value );
    test_true( !STD::is_trivial<Poly>::value );
    test_pass("cxx11:is_trivial");
    test_true(  STD::is_trivially_copyable<TTAgg>::value );
    test_true( !STD::is_trivially_copyable<Poly>::value );
    test_pass("cxx11:is_trivially_copyable");

    test_true(  STD::is_pod<TTAgg>::value );
    test_true( !STD::is_pod<Poly>::value );
    test_pass("cxx11:is_pod");
}
#endif

#if defined(__WATCOMC__)
TEST_CASE(type_traits, constructible_traits) {
    test_true(( STD::is_constructible<int, int>::value ));
    test_true(( STD::is_constructible<TTNonAgg>::value ));
    test_true(( !STD::is_constructible<TTNonAgg, int>::value ));
    test_true(( !STD::is_constructible<TTAbstract>::value ));
    test_pass("cxx11:is_constructible");
    test_true(( STD::is_nothrow_constructible<int, int>::value ));
    test_pass("cxx11:is_nothrow_constructible");

    TEST_NOTE("no triviality intrinsic: llibcxx03 answers true only where the type "
              "category alone guarantees it (scalars and arrays of scalars)");
    test_true(  STD::is_trivially_constructible<int>::value );
    test_true( !STD::is_trivially_constructible<Poly>::value );
    test_pass("cxx11:is_trivially_constructible");
    TEST_NOTE("no triviality intrinsic: scalars only");
    test_true(  STD::is_trivially_default_constructible<int>::value );
    test_true( !STD::is_trivially_default_constructible<Poly>::value );
    test_pass("cxx11:is_trivially_default_constructible");
    TEST_NOTE("no triviality intrinsic: scalars only");
    test_true(  STD::is_trivially_copy_constructible<int>::value );
    test_true( !STD::is_trivially_copy_constructible<Poly>::value );
    test_pass("cxx11:is_trivially_copy_constructible");
    TEST_NOTE("no triviality intrinsic: scalars only");
    test_true(  STD::is_trivially_move_constructible<int>::value );
    test_pass("cxx11:is_trivially_move_constructible");
    TEST_NOTE("no triviality intrinsic: scalars only");
    test_true(( STD::is_trivially_assignable<int&, int>::value ));
    test_pass("cxx11:is_trivially_assignable");
    TEST_NOTE("no triviality intrinsic: scalars only");
    test_true(  STD::is_trivially_copy_assignable<int>::value );
    test_true( !STD::is_trivially_copy_assignable<Poly>::value );
    test_pass("cxx11:is_trivially_copy_assignable");
    TEST_NOTE("no triviality intrinsic: scalars only");
    test_true(  STD::is_trivially_move_assignable<int>::value );
    test_pass("cxx11:is_trivially_move_assignable");
    TEST_NOTE("no triviality intrinsic: scalars only");
    test_true(  STD::is_trivially_destructible<int>::value );
    test_true( !STD::is_trivially_destructible<Poly>::value );
    test_pass("cxx11:is_trivially_destructible");
}
#else
TEST_CASE(type_traits, constructible_traits) {
    test_true(( STD::is_constructible<int, int>::value ));
    test_true(( STD::is_constructible<TTNonAgg>::value ));
    TEST_SKIP_VC090("MSVC 9 has no expression SFINAE: cannot detect a missing converting constructor");
    test_true(( !STD::is_constructible<TTNonAgg, int>::value ));
    TEST_SKIP_VC120("MSVC before VS2015 has no expression SFINAE: the answer here needs it");
    test_true(( !STD::is_constructible<TTAbstract>::value ));
    test_pass("cxx11:is_constructible");
    test_true(( STD::is_nothrow_constructible<int, int>::value ));
    test_pass("cxx11:is_nothrow_constructible");

    test_true(  STD::is_trivially_constructible<TTAgg>::value );
    test_pass("cxx11:is_trivially_constructible");
    test_true(  STD::is_trivially_default_constructible<TTAgg>::value );
    test_pass("cxx11:is_trivially_default_constructible");
    test_true(  STD::is_trivially_copy_constructible<TTAgg>::value );
    test_pass("cxx11:is_trivially_copy_constructible");
    test_true(  STD::is_trivially_move_constructible<TTAgg>::value );
    test_pass("cxx11:is_trivially_move_constructible");
    test_true(( STD::is_trivially_assignable<int&, int>::value ));
    test_pass("cxx11:is_trivially_assignable");
    test_true(  STD::is_trivially_copy_assignable<TTAgg>::value );
    test_pass("cxx11:is_trivially_copy_assignable");
    TEST_SKIP_VC_RANGE(1600, 1800, "vc10/vc11 have no __is_trivially_assignable intrinsic");
    test_true(  STD::is_trivially_move_assignable<TTAgg>::value );
    test_pass("cxx11:is_trivially_move_assignable");
    test_true(  STD::is_trivially_destructible<TTAgg>::value );
    test_true( !STD::is_trivially_destructible<Poly>::value );
    test_pass("cxx11:is_trivially_destructible");
}
#endif

#if defined(__WATCOMC__)
TEST_CASE(type_traits, aligned_and_result_of) {
    TEST_NOTE("Open Watcom has no alignof; alignment_of stands in. Over-alignment beyond "
              "the widest fundamental type cannot be expressed at all (A33)");
    typedef STD::aligned_storage<16, 8>::type S16;
    test_true( sizeof(S16) >= 16 );
    test_true( STD::alignment_of<S16>::value == 8 );
    test_pass("cxx11:aligned_storage");

    typedef STD::aligned_union<0, char, double>::type AU;
    test_true( sizeof(AU) >= sizeof(double) );
    test_pass("cxx11:aligned_union");
    TEST_NOTE("removed in C++20; llibcxx03 has no result_of");
    test_skip("cxx11:result_of");
}
#else
TEST_CASE(type_traits, aligned_and_result_of) {

    typedef STD::aligned_storage<16, 8>::type S16;
    test_true( sizeof(S16) >= 16 );
    test_true( alignof(S16) == 8 );
    test_pass("cxx11:aligned_storage");

    typedef STD::aligned_union<0, char, double>::type AU;
    test_true( sizeof(AU) >= sizeof(double) );
    test_pass("cxx11:aligned_union");
    TEST_SKIP1();
    TEST_NOTE("removed in C++20");
    test_skip("cxx11:result_of");
}
#endif

TEST_CASE(type_traits, cxx14_traits) {
#if _TST_HAS_FINAL && TEST_TARGET_CXX >= 2014
    test_true(  STD::is_final<TTFinalCls>::value );
    test_true( !STD::is_final<Empty>::value );
    test_pass("cxx14:is_final");
#else
    test_skip("cxx14:is_final");
#endif
#if _TST_HAS_NULLPTR && TEST_TARGET_CXX >= 2014
    test_true(  STD::is_null_pointer<STD::nullptr_t>::value );
    test_true( !STD::is_null_pointer<int>::value );
    test_pass("cxx14:is_null_pointer");
#else
    test_skip("cxx14:is_null_pointer");
#endif
}

#if TEST_TARGET_CXX >= 2017
TEST_CASE(type_traits, cxx17_helpers) {

    test_true(( STD::bool_constant<true>::value ));
#if _TST_HAS_ALIAS_TEMPLATE
    test_true(( STD::is_same<STD::bool_constant<true>, STD::integral_constant<bool, true> >::value ));
    test_pass("cxx17:bool_constant");
#else
    TEST_SKIP1();
    test_skip("cxx17:bool_constant");
#endif

#if _TST_HAS_ALIAS_TEMPLATE
    test_true(( STD::is_same<STD::void_t<int, char>, void>::value ));
    test_pass("cxx17:void_t");
#else
    test_skip("cxx17:void_t");
#endif

#if defined(__cpp_lib_has_unique_object_representations)
    test_true(  STD::has_unique_object_representations<int>::value );
    test_true( !STD::has_unique_object_representations<TTAgg>::value );
    test_pass("cxx17:has_unique_object_representations");
#else
    TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx17:has_unique_object_representations");
#endif

#if defined(__cpp_lib_is_aggregate)
    test_true(  STD::is_aggregate<TTAgg>::value );
    test_true( !STD::is_aggregate<TTNonAgg>::value );
    test_pass("cxx17:is_aggregate");
#else
    TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx17:is_aggregate");
#endif
}
#else
TEST_CASE(type_traits, cxx17_helpers) {
    TEST_NOTE("the C++17 trait helpers (bool_constant / void_t / ...)");
    test_skip("cxx17:bool_constant");
    test_skip("cxx17:void_t");
    test_skip("cxx17:has_unique_object_representations");
    test_skip("cxx17:is_aggregate");
}
#endif

TEST_CASE(type_traits, cxx17_invocable) {
#if defined(__cpp_lib_is_invocable)
    test_true(( STD::is_invocable<TTFnPtr, int>::value ));
    test_true(( !STD::is_invocable<TTFnPtr>::value ));
    test_pass("cxx17:is_invocable");
    test_true(( STD::is_invocable_r<long, TTFnPtr, int>::value ));
    test_true(( !STD::is_invocable_r<int*, TTFnPtr, int>::value ));
    test_pass("cxx17:is_invocable_r");
    test_true(( STD::is_same<STD::invoke_result<TTFnPtr, int>::type, long>::value ));
    test_pass("cxx17:invoke_result");

    test_true(( !STD::is_nothrow_invocable<TTFnPtr, int>::value ));
    test_pass("cxx17:is_nothrow_invocable");
    test_true(( !STD::is_nothrow_invocable_r<long, TTFnPtr, int>::value ));
    test_pass("cxx17:is_nothrow_invocable_r");
#else
    TEST_SKIP1(); TEST_SKIP1(); test_skip("cxx17:is_invocable");
    TEST_SKIP1(); TEST_SKIP1(); test_skip("cxx17:is_invocable_r");
    TEST_SKIP1(); test_skip("cxx17:invoke_result");
    TEST_SKIP1(); test_skip("cxx17:is_nothrow_invocable");
    TEST_SKIP1(); test_skip("cxx17:is_nothrow_invocable_r");
#endif
}

TEST_CASE(type_traits, cxx17_swappable) {
#if defined(__cpp_lib_is_swappable)
    test_true(  STD::is_swappable<int>::value );
    test_pass("cxx17:is_swappable");
    test_true(( STD::is_swappable_with<int&, int&>::value ));
    test_pass("cxx17:is_swappable_with");
    test_true(  STD::is_nothrow_swappable<int>::value );
    test_pass("cxx17:is_nothrow_swappable");
    test_true(( STD::is_nothrow_swappable_with<int&, int&>::value ));
    test_pass("cxx17:is_nothrow_swappable_with");
#else
    TEST_SKIP1(); test_skip("cxx17:is_swappable");
    TEST_SKIP1(); test_skip("cxx17:is_swappable_with");
    TEST_SKIP1(); test_skip("cxx17:is_nothrow_swappable");
    TEST_SKIP1(); test_skip("cxx17:is_nothrow_swappable_with");
#endif
}

TEST_CASE(type_traits, cxx20_array_and_convert) {
#if defined(__cpp_lib_bounded_array_traits)
    test_true(  STD::is_bounded_array<int[3]>::value );
    test_true( !STD::is_bounded_array<int[]>::value );
    test_pass("cxx20:is_bounded_array");
    test_true(  STD::is_unbounded_array<int[]>::value );
    test_true( !STD::is_unbounded_array<int[3]>::value );
    test_pass("cxx20:is_unbounded_array");
#else
    TEST_SKIP1(); TEST_SKIP1(); test_skip("cxx20:is_bounded_array");
    TEST_SKIP1(); TEST_SKIP1(); test_skip("cxx20:is_unbounded_array");
#endif

#if defined(__cpp_lib_is_nothrow_convertible)
    test_true(( STD::is_nothrow_convertible<int, long>::value ));
    test_pass("cxx20:is_nothrow_convertible");
#else
    TEST_SKIP1();
    test_skip("cxx20:is_nothrow_convertible");
#endif

#if defined(__cpp_lib_unwrap_ref)
    test_true(( STD::is_same<STD::unwrap_reference<STD::reference_wrapper<int> >::type, int&>::value ));
    test_true(( STD::is_same<STD::unwrap_reference<int>::type, int>::value ));
    test_pass("cxx20:unwrap_reference");
    test_true(( STD::is_same<STD::unwrap_ref_decay<int>::type, int>::value ));
    test_true(( STD::is_same<STD::unwrap_ref_decay<STD::reference_wrapper<int> >::type, int&>::value ));
    test_pass("cxx20:unwrap_ref_decay");
#else
    TEST_SKIP1(); TEST_SKIP1(); test_skip("cxx20:unwrap_reference");
    TEST_SKIP1(); TEST_SKIP1(); test_skip("cxx20:unwrap_ref_decay");
#endif
}

TEST_CASE(type_traits, cxx20_reference_and_layout) {
#if defined(__cpp_lib_common_reference)
    test_true(( STD::is_same<STD::common_reference<int&, int&>::type, int&>::value ));
    test_pass("cxx20:common_reference");

    (void)sizeof(STD::basic_common_reference<int, int, STD::type_identity_t, STD::type_identity_t>);
    test_true( true );
    test_pass("cxx20:basic_common_reference");
#else
    TEST_SKIP1(); test_skip("cxx20:common_reference");
    TEST_SKIP1(); test_skip("cxx20:basic_common_reference");
#endif

#if defined(__cpp_lib_is_layout_compatible)
    test_true(( STD::is_layout_compatible<int, int>::value ));
    test_true(( !STD::is_layout_compatible<int, float>::value ));
    test_pass("cxx20:is_layout_compatible");
#else
    TEST_SKIP1(); TEST_SKIP1(); test_skip("cxx20:is_layout_compatible");
#endif

#if defined(__cpp_lib_is_pointer_interconvertible)
    test_true(( STD::is_pointer_interconvertible_base_of<TTBase, TTDerived>::value ));
    test_true(( !STD::is_pointer_interconvertible_base_of<TTDerived, TTBase>::value ));
    test_pass("cxx20:is_pointer_interconvertible_base_of");
#else
    TEST_SKIP1(); TEST_SKIP1(); test_skip("cxx20:is_pointer_interconvertible_base_of");
#endif
}

TEST_CASE(type_traits, cxx23_cxx26_traits) {
#if defined(__cpp_lib_is_scoped_enum)
    test_true(  STD::is_scoped_enum<TTScopedEnum>::value );
    test_true( !STD::is_scoped_enum<PlainEnum>::value );
    test_pass("cxx23:is_scoped_enum");
#else
    test_skip("cxx23:is_scoped_enum");
#endif

#if defined(__cpp_lib_reference_from_temporary)
    test_true(( STD::reference_constructs_from_temporary<const int&, long>::value ));
    test_pass("cxx23:reference_constructs_from_temporary");
    test_true(( STD::reference_converts_from_temporary<const int&, long>::value ));
    test_pass("cxx23:reference_converts_from_temporary");
#else
    test_skip("cxx23:reference_constructs_from_temporary");
    test_skip("cxx23:reference_converts_from_temporary");
#endif

#if defined(__cpp_lib_is_virtual_base_of)

    test_true(( !STD::is_virtual_base_of<TTBase, TTDerived>::value ));
    test_pass("cxx26:is_virtual_base_of");
#else
    test_skip("cxx26:is_virtual_base_of");
#endif
}

TEST_CASE(type_traits, variable_templates_a_cxx17) {
#if _TST_HAS_VAR_TEMPLATE && _TST_HAS_CXX17_LIB_MEMBERS
    typedef long (*TTFnPtr)(int);
    {
        const bool v = STD::is_void_v<void>;
        const bool c = STD::is_void<void>::value;
        const bool f = STD::is_void_v<int>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_void_v");
    {
        const bool v = STD::is_null_pointer_v<STD::nullptr_t>;
        const bool c = STD::is_null_pointer<STD::nullptr_t>::value;
        const bool f = STD::is_null_pointer_v<int>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_null_pointer_v");
    {
        const bool v = STD::is_integral_v<int>;
        const bool c = STD::is_integral<int>::value;
        const bool f = STD::is_integral_v<double>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_integral_v");
    {
        const bool v = STD::is_floating_point_v<double>;
        const bool c = STD::is_floating_point<double>::value;
        const bool f = STD::is_floating_point_v<int>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_floating_point_v");
    {
        const bool v = STD::is_array_v<int[3]>;
        const bool c = STD::is_array<int[3]>::value;
        const bool f = STD::is_array_v<int>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_array_v");
    {
        const bool v = STD::is_pointer_v<int*>;
        const bool c = STD::is_pointer<int*>::value;
        const bool f = STD::is_pointer_v<int>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_pointer_v");
    {
        const bool v = STD::is_lvalue_reference_v<int&>;
        const bool c = STD::is_lvalue_reference<int&>::value;
        const bool f = STD::is_lvalue_reference_v<int>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_lvalue_reference_v");
    {
        const bool v = STD::is_rvalue_reference_v<int&&>;
        const bool c = STD::is_rvalue_reference<int&&>::value;
        const bool f = STD::is_rvalue_reference_v<int&>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_rvalue_reference_v");
    {
        const bool v = STD::is_member_object_pointer_v<int TTAgg::*>;
        const bool c = STD::is_member_object_pointer<int TTAgg::*>::value;
        const bool f = STD::is_member_object_pointer_v<void (TTAbstract::*)()>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_member_object_pointer_v");
    {
        const bool v = STD::is_member_function_pointer_v<void (TTAbstract::*)()>;
        const bool c = STD::is_member_function_pointer<void (TTAbstract::*)()>::value;
        const bool f = STD::is_member_function_pointer_v<int TTAgg::*>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_member_function_pointer_v");
    {
        const bool v = STD::is_enum_v<PlainEnum>;
        const bool c = STD::is_enum<PlainEnum>::value;
        const bool f = STD::is_enum_v<int>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_enum_v");
    {
        const bool v = STD::is_union_v<TTUnion>;
        const bool c = STD::is_union<TTUnion>::value;
        const bool f = STD::is_union_v<TTAgg>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_union_v");
    {
        const bool v = STD::is_class_v<Empty>;
        const bool c = STD::is_class<Empty>::value;
        const bool f = STD::is_class_v<int>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_class_v");
    {
        const bool v = STD::is_function_v<int(int)>;
        const bool c = STD::is_function<int(int)>::value;
        const bool f = STD::is_function_v<int>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_function_v");
    {
        const bool v = STD::is_reference_v<int&>;
        const bool c = STD::is_reference<int&>::value;
        const bool f = STD::is_reference_v<int>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_reference_v");
    {
        const bool v = STD::is_arithmetic_v<int>;
        const bool c = STD::is_arithmetic<int>::value;
        const bool f = STD::is_arithmetic_v<int*>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_arithmetic_v");
    {
        const bool v = STD::is_fundamental_v<int>;
        const bool c = STD::is_fundamental<int>::value;
        const bool f = STD::is_fundamental_v<int*>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_fundamental_v");
    {
        const bool v = STD::is_object_v<int>;
        const bool c = STD::is_object<int>::value;
        const bool f = STD::is_object_v<int&>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_object_v");
    {
        const bool v = STD::is_scalar_v<int>;
        const bool c = STD::is_scalar<int>::value;
        const bool f = STD::is_scalar_v<Empty>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_scalar_v");
    {
        const bool v = STD::is_compound_v<int*>;
        const bool c = STD::is_compound<int*>::value;
        const bool f = STD::is_compound_v<int>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_compound_v");
    {
        const bool v = STD::is_member_pointer_v<int TTAgg::*>;
        const bool c = STD::is_member_pointer<int TTAgg::*>::value;
        const bool f = STD::is_member_pointer_v<int*>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_member_pointer_v");
    {
        const bool v = STD::is_const_v<const int>;
        const bool c = STD::is_const<const int>::value;
        const bool f = STD::is_const_v<int>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_const_v");
    {
        const bool v = STD::is_volatile_v<volatile int>;
        const bool c = STD::is_volatile<volatile int>::value;
        const bool f = STD::is_volatile_v<int>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_volatile_v");
#else
    test_skip("cxx17:is_void_v");
    test_skip("cxx17:is_null_pointer_v");
    test_skip("cxx17:is_integral_v");
    test_skip("cxx17:is_floating_point_v");
    test_skip("cxx17:is_array_v");
    test_skip("cxx17:is_pointer_v");
    test_skip("cxx17:is_lvalue_reference_v");
    test_skip("cxx17:is_rvalue_reference_v");
    test_skip("cxx17:is_member_object_pointer_v");
    test_skip("cxx17:is_member_function_pointer_v");
    test_skip("cxx17:is_enum_v");
    test_skip("cxx17:is_union_v");
    test_skip("cxx17:is_class_v");
    test_skip("cxx17:is_function_v");
    test_skip("cxx17:is_reference_v");
    test_skip("cxx17:is_arithmetic_v");
    test_skip("cxx17:is_fundamental_v");
    test_skip("cxx17:is_object_v");
    test_skip("cxx17:is_scalar_v");
    test_skip("cxx17:is_compound_v");
    test_skip("cxx17:is_member_pointer_v");
    test_skip("cxx17:is_const_v");
    test_skip("cxx17:is_volatile_v");
#endif
}

TEST_CASE(type_traits, variable_templates_b_cxx17) {
#if _TST_HAS_VAR_TEMPLATE && _TST_HAS_CXX17_LIB_MEMBERS
    typedef long (*TTFnPtr)(int);
    {
        const bool v = STD::is_trivial_v<int>;
        const bool c = STD::is_trivial<int>::value;
        const bool f = STD::is_trivial_v<TTNonAgg>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_trivial_v");
    {
        const bool v = STD::is_trivially_copyable_v<int>;
        const bool c = STD::is_trivially_copyable<int>::value;
        const bool f = STD::is_trivially_copyable_v<Poly>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_trivially_copyable_v");
    {
        const bool v = STD::is_standard_layout_v<TTAgg>;
        const bool c = STD::is_standard_layout<TTAgg>::value;
        const bool f = STD::is_standard_layout_v<Poly>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_standard_layout_v");
    {
        const bool v = STD::is_empty_v<Empty>;
        const bool c = STD::is_empty<Empty>::value;
        const bool f = STD::is_empty_v<TTAgg>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_empty_v");
    {
        const bool v = STD::is_polymorphic_v<Poly>;
        const bool c = STD::is_polymorphic<Poly>::value;
        const bool f = STD::is_polymorphic_v<Empty>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_polymorphic_v");
    {
        const bool v = STD::is_abstract_v<TTAbstract>;
        const bool c = STD::is_abstract<TTAbstract>::value;
        const bool f = STD::is_abstract_v<Empty>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_abstract_v");
    {
        const bool v = STD::is_aggregate_v<TTAgg>;
        const bool c = STD::is_aggregate<TTAgg>::value;
        const bool f = STD::is_aggregate_v<TTNonAgg>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_aggregate_v");
    {
        const bool v = STD::is_signed_v<int>;
        const bool c = STD::is_signed<int>::value;
        const bool f = STD::is_signed_v<unsigned>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_signed_v");
    {
        const bool v = STD::is_unsigned_v<unsigned>;
        const bool c = STD::is_unsigned<unsigned>::value;
        const bool f = STD::is_unsigned_v<int>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_unsigned_v");
    {
        const bool v = STD::is_constructible_v<int, int>;
        const bool c = STD::is_constructible<int, int>::value;
        const bool f = STD::is_constructible_v<int, void*>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_constructible_v");
    {
        const bool v = STD::is_default_constructible_v<int>;
        const bool c = STD::is_default_constructible<int>::value;
        const bool f = STD::is_default_constructible_v<TTAbstract>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_default_constructible_v");
    {
        const bool v = STD::is_copy_constructible_v<int>;
        const bool c = STD::is_copy_constructible<int>::value;
        const bool f = STD::is_copy_constructible_v<TTAbstract>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_copy_constructible_v");
    {
        const bool v = STD::is_move_constructible_v<int>;
        const bool c = STD::is_move_constructible<int>::value;
        const bool f = STD::is_move_constructible_v<TTAbstract>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_move_constructible_v");
    {
        const bool v = STD::is_assignable_v<int&, int>;
        const bool c = STD::is_assignable<int&, int>::value;
        const bool f = STD::is_assignable_v<int, int>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_assignable_v");
    {
        const bool v = STD::is_copy_assignable_v<int>;
        const bool c = STD::is_copy_assignable<int>::value;
        const bool f = STD::is_copy_assignable_v<const int>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_copy_assignable_v");
    {
        const bool v = STD::is_move_assignable_v<int>;
        const bool c = STD::is_move_assignable<int>::value;
        const bool f = STD::is_move_assignable_v<const int>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_move_assignable_v");
    {
        const bool v = STD::is_destructible_v<int>;
        const bool c = STD::is_destructible<int>::value;
        const bool f = STD::is_destructible_v<void>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_destructible_v");
    {
        const bool v = STD::is_trivially_constructible_v<int, int>;
        const bool c = STD::is_trivially_constructible<int, int>::value;
        const bool f = STD::is_trivially_constructible_v<int, void*>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_trivially_constructible_v");
    {
        const bool v = STD::is_trivially_copy_constructible_v<int>;
        const bool c = STD::is_trivially_copy_constructible<int>::value;
        const bool f = STD::is_trivially_copy_constructible_v<TTAbstract>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_trivially_copy_constructible_v");
    {
        const bool v = STD::is_trivially_move_constructible_v<int>;
        const bool c = STD::is_trivially_move_constructible<int>::value;
        const bool f = STD::is_trivially_move_constructible_v<TTAbstract>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_trivially_move_constructible_v");
    {
        const bool v = STD::is_trivially_assignable_v<int&, int>;
        const bool c = STD::is_trivially_assignable<int&, int>::value;
        const bool f = STD::is_trivially_assignable_v<int, int>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_trivially_assignable_v");
    {
        const bool v = STD::is_trivially_copy_assignable_v<int>;
        const bool c = STD::is_trivially_copy_assignable<int>::value;
        const bool f = STD::is_trivially_copy_assignable_v<const int>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_trivially_copy_assignable_v");
    {
        const bool v = STD::is_trivially_move_assignable_v<int>;
        const bool c = STD::is_trivially_move_assignable<int>::value;
        const bool f = STD::is_trivially_move_assignable_v<const int>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_trivially_move_assignable_v");
#else
    test_skip("cxx17:is_trivial_v");
    test_skip("cxx17:is_trivially_copyable_v");
    test_skip("cxx17:is_standard_layout_v");
    test_skip("cxx17:is_empty_v");
    test_skip("cxx17:is_polymorphic_v");
    test_skip("cxx17:is_abstract_v");
    test_skip("cxx17:is_aggregate_v");
    test_skip("cxx17:is_signed_v");
    test_skip("cxx17:is_unsigned_v");
    test_skip("cxx17:is_constructible_v");
    test_skip("cxx17:is_default_constructible_v");
    test_skip("cxx17:is_copy_constructible_v");
    test_skip("cxx17:is_move_constructible_v");
    test_skip("cxx17:is_assignable_v");
    test_skip("cxx17:is_copy_assignable_v");
    test_skip("cxx17:is_move_assignable_v");
    test_skip("cxx17:is_destructible_v");
    test_skip("cxx17:is_trivially_constructible_v");
    test_skip("cxx17:is_trivially_copy_constructible_v");
    test_skip("cxx17:is_trivially_move_constructible_v");
    test_skip("cxx17:is_trivially_assignable_v");
    test_skip("cxx17:is_trivially_copy_assignable_v");
    test_skip("cxx17:is_trivially_move_assignable_v");
#endif
}

TEST_CASE(type_traits, variable_templates_c_cxx17) {
#if _TST_HAS_VAR_TEMPLATE && _TST_HAS_CXX17_LIB_MEMBERS
    typedef long (*TTFnPtr)(int);
    {
        const bool v = STD::is_trivially_destructible_v<int>;
        const bool c = STD::is_trivially_destructible<int>::value;
        const bool f = STD::is_trivially_destructible_v<Poly>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_trivially_destructible_v");
    {
        const bool v = STD::is_nothrow_constructible_v<int, int>;
        const bool c = STD::is_nothrow_constructible<int, int>::value;
        const bool f = STD::is_nothrow_constructible_v<int, void*>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_nothrow_constructible_v");
    {
        const bool v = STD::is_nothrow_default_constructible_v<int>;
        const bool c = STD::is_nothrow_default_constructible<int>::value;
        const bool f = STD::is_nothrow_default_constructible_v<TTAbstract>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_nothrow_default_constructible_v");
    {
        const bool v = STD::is_nothrow_copy_constructible_v<int>;
        const bool c = STD::is_nothrow_copy_constructible<int>::value;
        const bool f = STD::is_nothrow_copy_constructible_v<TTAbstract>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_nothrow_copy_constructible_v");
    {
        const bool v = STD::is_nothrow_move_constructible_v<int>;
        const bool c = STD::is_nothrow_move_constructible<int>::value;
        const bool f = STD::is_nothrow_move_constructible_v<TTAbstract>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_nothrow_move_constructible_v");
    {
        const bool v = STD::is_nothrow_assignable_v<int&, int>;
        const bool c = STD::is_nothrow_assignable<int&, int>::value;
        const bool f = STD::is_nothrow_assignable_v<int, int>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_nothrow_assignable_v");
    {
        const bool v = STD::is_nothrow_copy_assignable_v<int>;
        const bool c = STD::is_nothrow_copy_assignable<int>::value;
        const bool f = STD::is_nothrow_copy_assignable_v<const int>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_nothrow_copy_assignable_v");
    {
        const bool v = STD::is_nothrow_move_assignable_v<int>;
        const bool c = STD::is_nothrow_move_assignable<int>::value;
        const bool f = STD::is_nothrow_move_assignable_v<const int>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_nothrow_move_assignable_v");
    {
        const bool v = STD::is_nothrow_destructible_v<int>;
        const bool c = STD::is_nothrow_destructible<int>::value;
        const bool f = STD::is_nothrow_destructible_v<void>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_nothrow_destructible_v");
    {
        const bool v = STD::has_virtual_destructor_v<Poly>;
        const bool c = STD::has_virtual_destructor<Poly>::value;
        const bool f = STD::has_virtual_destructor_v<Empty>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:has_virtual_destructor_v");
    {
        const bool v = STD::has_unique_object_representations_v<int>;
        const bool c = STD::has_unique_object_representations<int>::value;
        const bool f = STD::has_unique_object_representations_v<TTAgg>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:has_unique_object_representations_v");
    {
        const bool v = STD::is_same_v<int, int>;
        const bool c = STD::is_same<int, int>::value;
        const bool f = STD::is_same_v<int, long>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_same_v");
    {
        const bool v = STD::is_base_of_v<TTBase, TTDerived>;
        const bool c = STD::is_base_of<TTBase, TTDerived>::value;
        const bool f = STD::is_base_of_v<TTDerived, TTBase>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_base_of_v");
    {
        const bool v = STD::is_convertible_v<int, long>;
        const bool c = STD::is_convertible<int, long>::value;
        const bool f = STD::is_convertible_v<int*, long*>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_convertible_v");
    {
        const bool v = STD::is_invocable_v<TTFnPtr, int>;
        const bool c = STD::is_invocable<TTFnPtr, int>::value;
        const bool f = STD::is_invocable_v<TTFnPtr, void*>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_invocable_v");
    {
        const bool v = STD::is_invocable_r_v<int, TTFnPtr, int>;
        const bool c = STD::is_invocable_r<int, TTFnPtr, int>::value;
        const bool f = STD::is_invocable_r_v<int*, TTFnPtr, int>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_invocable_r_v");
    {
        const bool v = STD::is_swappable_v<int>;
        const bool c = STD::is_swappable<int>::value;
        const bool f = STD::is_swappable_v<void>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_swappable_v");
    {
        const bool v = STD::is_swappable_with_v<int&, int&>;
        const bool c = STD::is_swappable_with<int&, int&>::value;
        const bool f = STD::is_swappable_with_v<int, int>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_swappable_with_v");
    {
        const bool v = STD::is_nothrow_swappable_v<int>;
        const bool c = STD::is_nothrow_swappable<int>::value;
        const bool f = STD::is_nothrow_swappable_v<void>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_nothrow_swappable_v");
    {
        const bool v = STD::is_nothrow_swappable_with_v<int&, int&>;
        const bool c = STD::is_nothrow_swappable_with<int&, int&>::value;
        const bool f = STD::is_nothrow_swappable_with_v<int, int>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_nothrow_swappable_with_v");
    {
        const bool v = STD::is_pod_v<TTAgg>;
        const bool c = STD::is_pod<TTAgg>::value;
        const bool f = STD::is_pod_v<Poly>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_pod_v");
#else
    test_skip("cxx17:is_trivially_destructible_v");
    test_skip("cxx17:is_nothrow_constructible_v");
    test_skip("cxx17:is_nothrow_default_constructible_v");
    test_skip("cxx17:is_nothrow_copy_constructible_v");
    test_skip("cxx17:is_nothrow_move_constructible_v");
    test_skip("cxx17:is_nothrow_assignable_v");
    test_skip("cxx17:is_nothrow_copy_assignable_v");
    test_skip("cxx17:is_nothrow_move_assignable_v");
    test_skip("cxx17:is_nothrow_destructible_v");
    test_skip("cxx17:has_virtual_destructor_v");
    test_skip("cxx17:has_unique_object_representations_v");
    test_skip("cxx17:is_same_v");
    test_skip("cxx17:is_base_of_v");
    test_skip("cxx17:is_convertible_v");
    test_skip("cxx17:is_invocable_v");
    test_skip("cxx17:is_invocable_r_v");
    test_skip("cxx17:is_swappable_v");
    test_skip("cxx17:is_swappable_with_v");
    test_skip("cxx17:is_nothrow_swappable_v");
    test_skip("cxx17:is_nothrow_swappable_with_v");
    test_skip("cxx17:is_pod_v");
#endif
}

TEST_CASE(type_traits, variable_templates_misc_cxx17) {
#if _TST_HAS_VAR_TEMPLATE && _TST_HAS_CXX17_LIB_MEMBERS
    {
        const STD::size_t v = STD::extent_v<int[3]>;
        const STD::size_t v2 = STD::extent_v<int[2][7], 1>;
        test_eq( (int)v, 3 );
        test_eq( (int)v2, 7 );
    }
    test_pass("cxx17:extent_v");
    {
        const STD::size_t v = STD::rank_v<int[2][3]>;
        test_eq( (int)v, 2 );
        test_eq( (int)v, (int)STD::rank<int[2][3]>::value );
    }
    test_pass("cxx17:rank_v");
    {
        const STD::size_t v = STD::alignment_of_v<double>;
        test_eq( (int)v, (int)STD::alignment_of<double>::value );
        test_true( v >= 1 );
    }
    test_pass("cxx17:alignment_of_v");
    {
        const bool v = STD::conjunction_v<STD::true_type, STD::true_type>;
        const bool f = STD::conjunction_v<STD::true_type, STD::false_type>;
        test_true( v && !f );
        test_true( v == (STD::conjunction<STD::true_type, STD::true_type>::value) );
    }
    test_pass("cxx17:conjunction_v");
    {
        const bool v = STD::disjunction_v<STD::false_type, STD::true_type>;
        const bool f = STD::disjunction_v<STD::false_type, STD::false_type>;
        test_true( v && !f );
        test_true( v == (STD::disjunction<STD::false_type, STD::true_type>::value) );
    }
    test_pass("cxx17:disjunction_v");
    {
        const bool v = STD::negation_v<STD::false_type>;
        const bool f = STD::negation_v<STD::true_type>;
        test_true( v && !f );
        test_true( v == (STD::negation<STD::false_type>::value) );
    }
    test_pass("cxx17:negation_v");
    {
        typedef long (*TTFnPtr)(int);
        const bool v = STD::is_nothrow_invocable_v<TTFnPtr, int>;
        test_true( v == (STD::is_nothrow_invocable<TTFnPtr, int>::value) );
        test_true( !(STD::is_nothrow_invocable_v<TTFnPtr, void*>) );
    }
    test_pass("cxx17:is_nothrow_invocable_v");
    {
        typedef long (*TTFnPtr)(int);
        const bool r = STD::is_nothrow_invocable_r_v<long, TTFnPtr, int>;
        test_true( r == (STD::is_nothrow_invocable_r<long, TTFnPtr, int>::value) );
        test_true( !(STD::is_nothrow_invocable_r_v<int*, TTFnPtr, int>) );
    }
    test_pass("cxx17:is_nothrow_invocable_r_v");
#else
    test_skip("cxx17:extent_v");
    test_skip("cxx17:rank_v");
    test_skip("cxx17:alignment_of_v");
    test_skip("cxx17:conjunction_v");
    test_skip("cxx17:disjunction_v");
    test_skip("cxx17:negation_v");
    test_skip("cxx17:is_nothrow_invocable_v");
    test_skip("cxx17:is_nothrow_invocable_r_v");
#endif

#if _TST_HAS_VAR_TEMPLATE && _TST_HAS_CXX17_LIB_MEMBERS && _TST_HAS_FINAL
    {
        const bool v = STD::is_final_v<TTFinalCls>;
        const bool c = STD::is_final<TTFinalCls>::value;
        const bool f = STD::is_final_v<Empty>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:is_final_v");
#else
    test_skip("cxx17:is_final_v");
#endif

#if _TST_HAS_VAR_TEMPLATE && _TST_HAS_CXX17_LIB_MEMBERS && !defined(__WATCOMC__) && \
    (_TST_TT_LANG < 202002L || defined(__GLIBCXX__))
    {
        const bool v = STD::is_literal_type_v<int>;
        test_true( v == (STD::is_literal_type<int>::value) );
        test_true( v );
    }
    test_pass("cxx17:is_literal_type_v");
#else
    test_skip("cxx17:is_literal_type_v");
#endif
}

TEST_CASE(type_traits, is_literal_type_cxx11) {
#if _TST_TT_LANG < 202002L || defined(__GLIBCXX__)
    test_true(  STD::is_literal_type<int>::value );
    test_true( !STD::is_literal_type<Poly>::value );
    test_pass("cxx11:is_literal_type");
#else
    test_skip("cxx11:is_literal_type");
#endif
}


TEST_CASE(type_traits, transformation_aliases_cxx14) {
#if _TST_HAS_ALIAS_TEMPLATE && TEST_TARGET_CXX >= 2014
    test_true(( STD::is_same<STD::remove_const_t<const int>, int>::value ));
    test_true(( STD::is_same<STD::remove_const_t<const int>, STD::remove_const<const int>::type>::value ));
    test_pass("cxx14:remove_const_t");
    test_true(( STD::is_same<STD::remove_volatile_t<volatile int>, int>::value ));
    test_true(( STD::is_same<STD::remove_volatile_t<volatile int>, STD::remove_volatile<volatile int>::type>::value ));
    test_pass("cxx14:remove_volatile_t");
    test_true(( STD::is_same<STD::remove_cv_t<const volatile int>, int>::value ));
    test_true(( STD::is_same<STD::remove_cv_t<const volatile int>, STD::remove_cv<const volatile int>::type>::value ));
    test_pass("cxx14:remove_cv_t");
    test_true(( STD::is_same<STD::remove_reference_t<int&>, int>::value ));
    test_true(( STD::is_same<STD::remove_reference_t<int&>, STD::remove_reference<int&>::type>::value ));
    test_pass("cxx14:remove_reference_t");
    test_true(( STD::is_same<STD::remove_pointer_t<int*>, int>::value ));
    test_true(( STD::is_same<STD::remove_pointer_t<int*>, STD::remove_pointer<int*>::type>::value ));
    test_pass("cxx14:remove_pointer_t");
    test_true(( STD::is_same<STD::remove_extent_t<int[3]>, int>::value ));
    test_true(( STD::is_same<STD::remove_extent_t<int[3]>, STD::remove_extent<int[3]>::type>::value ));
    test_pass("cxx14:remove_extent_t");
    test_true(( STD::is_same<STD::remove_all_extents_t<int[2][3]>, int>::value ));
    test_true(( STD::is_same<STD::remove_all_extents_t<int[2][3]>, STD::remove_all_extents<int[2][3]>::type>::value ));
    test_pass("cxx14:remove_all_extents_t");
    test_true(( STD::is_same<STD::make_signed_t<unsigned>, int>::value ));
    test_true(( STD::is_same<STD::make_signed_t<unsigned>, STD::make_signed<unsigned>::type>::value ));
    test_pass("cxx14:make_signed_t");
    test_true(( STD::is_same<STD::make_unsigned_t<int>, unsigned>::value ));
    test_true(( STD::is_same<STD::make_unsigned_t<int>, STD::make_unsigned<int>::type>::value ));
    test_pass("cxx14:make_unsigned_t");
    test_true(( STD::is_same<STD::decay_t<int&>, int>::value ));
    test_true(( STD::is_same<STD::decay_t<int&>, STD::decay<int&>::type>::value ));
    test_pass("cxx14:decay_t");
    test_true(( STD::is_same<STD::conditional_t<true, int, long>, int>::value ));
    test_true(( STD::is_same<STD::conditional_t<true, int, long>, STD::conditional<true, int, long>::type>::value ));
    test_pass("cxx14:conditional_t");
    test_true(( STD::is_same<STD::common_type_t<int, int>, int>::value ));
    test_true(( STD::is_same<STD::common_type_t<int, int>, STD::common_type<int, int>::type>::value ));
    test_pass("cxx14:common_type_t");
    test_true(( STD::is_same<STD::underlying_type_t<PlainEnum>, STD::underlying_type<PlainEnum>::type>::value ));
    test_true(( STD::is_same<STD::underlying_type_t<PlainEnum>, STD::underlying_type<PlainEnum>::type>::value ));
    test_pass("cxx14:underlying_type_t");
    test_true(( STD::is_same<STD::enable_if_t<true, int>, int>::value ));
    test_true(( STD::is_same<STD::enable_if_t<true, int>, STD::enable_if<true, int>::type>::value ));
    test_pass("cxx14:enable_if_t");
#else
    test_skip("cxx14:remove_const_t");
    test_skip("cxx14:remove_volatile_t");
    test_skip("cxx14:remove_cv_t");
    test_skip("cxx14:remove_reference_t");
    test_skip("cxx14:remove_pointer_t");
    test_skip("cxx14:remove_extent_t");
    test_skip("cxx14:remove_all_extents_t");
    test_skip("cxx14:make_signed_t");
    test_skip("cxx14:make_unsigned_t");
    test_skip("cxx14:decay_t");
    test_skip("cxx14:conditional_t");
    test_skip("cxx14:common_type_t");
    test_skip("cxx14:underlying_type_t");
    test_skip("cxx14:enable_if_t");
#endif
}

TEST_CASE(type_traits, storage_aliases_cxx14) {
#if _TST_HAS_ALIAS_TEMPLATE && TEST_TARGET_CXX >= 2014
    {
        STD::aligned_storage_t<8, 8> buf;
        test_true( sizeof(buf) >= 8 );
        test_true(( STD::is_same<STD::aligned_storage_t<8, 8>,
                                 STD::aligned_storage<8, 8>::type>::value ));
    }
    test_pass("cxx14:aligned_storage_t");
    {
        STD::aligned_union_t<8, int, double> buf;
        test_true( sizeof(buf) >= sizeof(double) );
        test_true(( STD::is_same<STD::aligned_union_t<8, int, double>,
                                 STD::aligned_union<8, int, double>::type>::value ));
    }
    test_pass("cxx14:aligned_union_t");
#else
    test_skip("cxx14:aligned_storage_t");
    test_skip("cxx14:aligned_union_t");
#endif

#if _TST_HAS_ALIAS_TEMPLATE && TEST_TARGET_CXX >= 2014 && (_TST_TT_LANG < 202002L || defined(__GLIBCXX__))
    {
        typedef long (*TTFnPtr)(int);
        test_true(( STD::is_same<STD::result_of_t<TTFnPtr(int)>, long>::value ));
        test_true(( STD::is_same<STD::result_of_t<TTFnPtr(int)>,
                                 STD::result_of<TTFnPtr(int)>::type>::value ));
    }
    test_pass("cxx14:result_of_t");
#else
    test_skip("cxx14:result_of_t");
#endif

#if _TST_HAS_ALIAS_TEMPLATE && _TST_HAS_CXX17_LIB_MEMBERS
    {
        typedef long (*TTFnPtr)(int);
        test_true(( STD::is_same<STD::invoke_result_t<TTFnPtr, int>, long>::value ));
        test_true(( STD::is_same<STD::invoke_result_t<TTFnPtr, int>,
                                 STD::invoke_result<TTFnPtr, int>::type>::value ));
    }
    test_pass("cxx17:invoke_result_t");
#else
    test_skip("cxx17:invoke_result_t");
#endif
}


TEST_CASE(type_traits, aliases_and_values_cxx20) {
#if defined(__cpp_lib_type_identity) && _TST_HAS_ALIAS_TEMPLATE
    test_true(( STD::is_same<STD::type_identity_t<int>, int>::value ));
    test_true(( STD::is_same<STD::type_identity_t<const int&>, const int&>::value ));
    test_pass("cxx20:type_identity_t");
#else
    test_skip("cxx20:type_identity_t");
#endif

#if defined(__cpp_lib_remove_cvref) && _TST_HAS_ALIAS_TEMPLATE
    test_true(( STD::is_same<STD::remove_cvref_t<const int&>, int>::value ));
    test_true(( STD::is_same<STD::remove_cvref_t<const int&>,
                             STD::remove_cvref<const int&>::type>::value ));
    test_pass("cxx20:remove_cvref_t");
#else
    test_skip("cxx20:remove_cvref_t");
#endif

#if defined(__cpp_lib_unwrap_ref) && _TST_HAS_ALIAS_TEMPLATE
    test_true(( STD::is_same<STD::unwrap_reference_t<STD::reference_wrapper<int> >, int&>::value ));
    test_true(( STD::is_same<STD::unwrap_reference_t<int>, int>::value ));
    test_pass("cxx20:unwrap_reference_t");
    test_true(( STD::is_same<STD::unwrap_ref_decay_t<STD::reference_wrapper<int> >, int&>::value ));
    test_true(( STD::is_same<STD::unwrap_ref_decay_t<int&>, int>::value ));
    test_pass("cxx20:unwrap_ref_decay_t");
#else
    test_skip("cxx20:unwrap_reference_t");
    test_skip("cxx20:unwrap_ref_decay_t");
#endif

#if defined(__cpp_lib_common_reference) && _TST_HAS_ALIAS_TEMPLATE
    test_true(( STD::is_same<STD::common_reference_t<int&, int&>, int&>::value ));
    test_true(( STD::is_same<STD::common_reference_t<int&, const int&>, const int&>::value ));
    test_pass("cxx20:common_reference_t");
#else
    test_skip("cxx20:common_reference_t");
#endif

#if defined(__cpp_lib_bounded_array_traits) && _TST_HAS_VAR_TEMPLATE
    {
        const bool b = STD::is_bounded_array_v<int[3]>;
        test_true( b == (STD::is_bounded_array<int[3]>::value) );
        test_true( b && !STD::is_bounded_array_v<int[]> );
    }
    test_pass("cxx20:is_bounded_array_v");
    {
        const bool u = STD::is_unbounded_array_v<int[]>;
        test_true( u == (STD::is_unbounded_array<int[]>::value) );
        test_true( u && !STD::is_unbounded_array_v<int[3]> );
    }
    test_pass("cxx20:is_unbounded_array_v");
#else
    test_skip("cxx20:is_bounded_array_v");
    test_skip("cxx20:is_unbounded_array_v");
#endif

#if defined(__cpp_lib_is_nothrow_convertible) && _TST_HAS_VAR_TEMPLATE
    {
        const bool v = STD::is_nothrow_convertible_v<int, long>;
        test_true( v == (STD::is_nothrow_convertible<int, long>::value) );
        test_true( v && !(STD::is_nothrow_convertible_v<int*, long*>) );
    }
    test_pass("cxx20:is_nothrow_convertible_v");
#else
    test_skip("cxx20:is_nothrow_convertible_v");
#endif

#if defined(__cpp_lib_is_layout_compatible) && _TST_HAS_VAR_TEMPLATE
    {
        const bool v = STD::is_layout_compatible_v<int, int>;
        test_true( v == (STD::is_layout_compatible<int, int>::value) );
        test_true( v && !(STD::is_layout_compatible_v<int, float>) );
    }
    test_pass("cxx20:is_layout_compatible_v");
#else
    test_skip("cxx20:is_layout_compatible_v");
#endif
}

TEST_CASE(type_traits, constant_evaluated_and_members_cxx20) {
#if defined(__cpp_lib_is_constant_evaluated)
    {
        bool at_runtime = STD::is_constant_evaluated();
        constexpr bool at_compile_time = STD::is_constant_evaluated();
        test_true( !at_runtime );
        test_true( at_compile_time );
    }
    test_pass("cxx20:is_constant_evaluated");
#else
    test_skip("cxx20:is_constant_evaluated");
#endif

#if defined(__cpp_lib_is_pointer_interconvertible)
    test_true( STD::is_corresponding_member(&TTLayoutA::x, &TTLayoutB::x) );
    test_true( !STD::is_corresponding_member(&TTLayoutA::y, &TTLayoutB::x) );
    test_pass("cxx20:is_corresponding_member");

    test_true( STD::is_pointer_interconvertible_with_class(&TTLayoutA::x) );
    test_true( !STD::is_pointer_interconvertible_with_class(&TTLayoutA::y) );
    test_pass("cxx20:is_pointer_interconvertible_with_class");
#else
    test_skip("cxx20:is_corresponding_member");
    test_skip("cxx20:is_pointer_interconvertible_with_class");
#endif
}

TEST_CASE(type_traits, traits_cxx23_cxx26_v) {
#if defined(__cpp_lib_is_scoped_enum) && _TST_HAS_VAR_TEMPLATE
    {
        const bool v = STD::is_scoped_enum_v<TTScopedEnum>;
        test_true( v == (STD::is_scoped_enum<TTScopedEnum>::value) );
        test_true( v && !(STD::is_scoped_enum_v<PlainEnum>) );
    }
    test_pass("cxx23:is_scoped_enum_v");
#else
    test_skip("cxx23:is_scoped_enum_v");
#endif

#if defined(__cpp_lib_reference_from_temporary) && _TST_HAS_VAR_TEMPLATE
    {
        const bool v = STD::reference_constructs_from_temporary_v<const int&, long>;
        test_true( v == (STD::reference_constructs_from_temporary<const int&, long>::value) );
        test_true( v && !(STD::reference_constructs_from_temporary_v<const int&, int&>) );
    }
    test_pass("cxx23:reference_constructs_from_temporary_v");
    {
        const bool v = STD::reference_converts_from_temporary_v<const int&, long>;
        test_true( v == (STD::reference_converts_from_temporary<const int&, long>::value) );
        test_true( v && !(STD::reference_converts_from_temporary_v<const int&, int&>) );
    }
    test_pass("cxx23:reference_converts_from_temporary_v");
#else
    test_skip("cxx23:reference_constructs_from_temporary_v");
    test_skip("cxx23:reference_converts_from_temporary_v");
#endif

#if defined(__cpp_lib_is_implicit_lifetime)
    test_true(  STD::is_implicit_lifetime<int>::value );
    test_true( !STD::is_implicit_lifetime<Poly>::value );
    test_pass("cxx23:is_implicit_lifetime");
# if _TST_HAS_VAR_TEMPLATE
    {
        const bool v = STD::is_implicit_lifetime_v<int>;
        test_true( v == (STD::is_implicit_lifetime<int>::value) );
        test_true( v );
    }
    test_pass("cxx23:is_implicit_lifetime_v");
# else
    test_skip("cxx23:is_implicit_lifetime_v");
# endif
#else
    TEST_NOTE("libstdc++ 15.2 has no is_implicit_lifetime (P2674)");
    test_skip("cxx23:is_implicit_lifetime");
    test_skip("cxx23:is_implicit_lifetime_v");
#endif

#if defined(__cpp_lib_is_virtual_base_of) && _TST_HAS_VAR_TEMPLATE
    {
        const bool v = STD::is_virtual_base_of_v<TTVBase, TTVDerived>;
        test_true( v == (STD::is_virtual_base_of<TTVBase, TTVDerived>::value) );
        test_true( v && !(STD::is_virtual_base_of_v<TTBase, TTDerived>) );
    }
    test_pass("cxx26:is_virtual_base_of_v");
#else
    test_skip("cxx26:is_virtual_base_of_v");
#endif

#if defined(__cpp_lib_within_lifetime)
    {
        static const int live = 3;
        const bool v = STD::is_within_lifetime(&live);
        test_true( v );
        test_eq( live, 3 );
    }
    test_pass("cxx26:is_within_lifetime");
#else
    TEST_NOTE("libstdc++ 15.2 has no is_within_lifetime (P2641)");
    test_skip("cxx26:is_within_lifetime");
#endif

    TEST_NOTE("reflection (P2996) and is_structural are in no library in reach");
    test_skip("cxx26:is_reflection");
    test_skip("cxx26:is_reflection_v");
    test_skip("cxx26:is_structural");
    test_skip("cxx26:is_structural_v");
}
