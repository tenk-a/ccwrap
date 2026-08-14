#include "test_cxx.hpp"
#include <initializer_list>
#include <type_traits>
#include <cstddef>

TEST_CASE(initializer_list, header_compiles) { test_true( true );     test_pass("cxx11:<initializer_list> header");
}

#if _tst_cplusplus >= 201103L
namespace {

int il_sum(STD::initializer_list<int> il) {
    int s = 0;
    for (const int* p = il.begin(); p != il.end(); ++p) s += *p;
    return s;
}
}

TEST_CASE(initializer_list, begin_end_size) {
    STD::initializer_list<int> il = { 1, 2, 3, 4 };
    STD::initializer_list<int> empty;

    test_eq( il.size(), (STD::size_t)4 );
    test_eq( empty.size(), (STD::size_t)0 );
    test_pass("cxx11:initializer_list::size");

    test_eq( *il.begin(), 1 );
    test_true( empty.begin() == empty.end() );
    test_eq( *STD::begin(il), 1 );
    test_pass("cxx11:initializer_list::begin");

    test_eq( il.end() - il.begin(), (STD::ptrdiff_t)4 );
    test_eq( il_sum(il), 10 );

    int prod = 1;
    for (int x : il) prod *= x;
    test_eq( prod, 24 );
    test_eq( STD::end(il) - STD::begin(il), (STD::ptrdiff_t)4 );
    test_pass("cxx11:initializer_list::end");
}

TEST_CASE(initializer_list, typedefs_and_free_functions) {
    typedef STD::initializer_list<int> IL;

    bool tv  = STD::is_same<IL::value_type, int>::value;
    bool tr  = STD::is_same<IL::reference, const int&>::value;
    bool tcr = STD::is_same<IL::const_reference, const int&>::value;
    bool tsz = STD::is_same<IL::size_type, STD::size_t>::value;
    bool ti  = STD::is_same<IL::iterator, const int*>::value;
    test_true( tv && tr && tcr && tsz && ti );
    test_pass("cxx11:initializer_list member typedefs");

    IL e;
    test_eq( e.size(), (STD::size_t)0 );
    test_true( e.begin() == e.end() );
    test_pass("cxx11:initializer_list default ctor (empty)");

    IL il = { 5, 6, 7 };
    test_eq( *STD::begin(il), 5 );
    test_eq( (long)(STD::begin(il) - il.begin()), 0L );
    test_pass("cxx11:begin(initializer_list) free function");

    test_eq( (long)(STD::end(il) - STD::begin(il)), 3L );
    test_true( STD::end(il) == il.end() );
    test_pass("cxx11:end(initializer_list) free function");
}
#else
TEST_CASE_SKIP(initializer_list, begin_end_size)
TEST_CASE_SKIP(initializer_list, typedefs_and_free_functions)
#endif


#if _TST_HAS_INIT_LIST
TEST_CASE(initializer_list, member_types_cxx11) {
    typedef STD::initializer_list<int> IL;
    IL il = {1, 2, 3};

    test_true(( STD::is_same<IL::value_type, int>::value ));
    test_pass("cxx11:initializer_list::value_type");

    test_true(( STD::is_same<IL::reference, const int&>::value ));
    test_pass("cxx11:initializer_list::reference");

    test_true(( STD::is_same<IL::const_reference, const int&>::value ));
    test_pass("cxx11:initializer_list::const_reference");

    test_true(( STD::is_same<IL::size_type, STD::size_t>::value ));
    test_eq( il.size(), (STD::size_t)3 );
    test_pass("cxx11:initializer_list::size_type");

    test_true(( STD::is_same<IL::iterator, const int*>::value ));
    test_true(( STD::is_same<IL::iterator, decltype(il.begin())>::value ));
    test_pass("cxx11:initializer_list::iterator");

    test_true(( STD::is_same<IL::const_iterator, const int*>::value ));
    test_pass("cxx11:initializer_list::const_iterator");

#if defined(__cpp_lib_initializer_list_data)
    test_true( il.data() == il.begin() );
    test_eq( il.data()[2], 3 );
    test_pass("cxx26:initializer_list::data");
#else
    TEST_NOTE("initializer_list::data is C++26; this library does not have it yet");
    test_skip("cxx26:initializer_list::data");
#endif
}
#else
TEST_CASE(initializer_list, member_types_cxx11) {
    test_skip("cxx11:initializer_list::value_type");
    test_skip("cxx11:initializer_list::reference");
    test_skip("cxx11:initializer_list::const_reference");
    test_skip("cxx11:initializer_list::size_type");
    test_skip("cxx11:initializer_list::iterator");
    test_skip("cxx11:initializer_list::const_iterator");
    test_skip("cxx26:initializer_list::data");
}
#endif

#if _TST_HAS_INIT_LIST
namespace {
int il_total(STD::initializer_list<int> il) {
    int s = 0;
    for (const int* p = il.begin(); p != il.end(); ++p) s += *p;
    return s;
}
}

TEST_CASE(initializer_list, semantics_cxx11) {
    typedef STD::initializer_list<int> IL;
    test_true( STD::is_class<IL>::value );
    test_true(( !STD::is_same<IL, STD::initializer_list<double> >::value ));
    test_true( sizeof(IL) > 0 );
    test_pass("cxx11:initializer_list");

    IL il = { 1, 2, 3 };
    test_eq( il.size(), (STD::size_t)3 );
    test_eq( *(il.begin() + 0), 1 );
    test_eq( *(il.begin() + 2), 3 );
    test_pass("cxx11:initializer_list braced-list construction (non-empty)");

    test_true(( STD::is_same<IL::reference, const int&>::value ));
    test_true(( STD::is_same<decltype(*il.begin()), const int&>::value ));
    test_true( STD::is_const<STD::remove_reference<decltype(*il.begin())>::type>::value );
    test_pass("cxx11:initializer_list element access is const");

    test_true( il.end() == il.begin() + il.size() );
    test_eq( (long)(il.end() - il.begin()), (long)il.size() );
    test_pass("cxx11:initializer_list::end == begin + size");

    IL none;
    test_true( none.begin() == none.end() );
    test_eq( none.size(), (STD::size_t)0 );
    test_pass("cxx11:initializer_list::begin == end when empty");

    IL copy(il);
    test_true( copy.begin() == il.begin() );
    test_eq( copy.size(), il.size() );
    test_pass("cxx11:initializer_list copy construction shares backing array");

    IL assigned = { 9 };
    assigned = il;
    test_true( assigned.begin() == il.begin() );
    test_eq( assigned.size(), il.size() );
    test_pass("cxx11:initializer_list copy assignment shares backing array");

    {
        IL local = { 10, 20, 30 };
        const int* first = local.begin();
        test_eq( il_total({ 4, 5, 6 }), 15 );
        test_true( local.begin() == first );
        test_eq( il_total(local), 60 );
    }
    test_pass("cxx11:initializer_list backing array lifetime extension");

    IL a = { 7, 8 };
    IL b = { 7, 8 };
    if (a.begin() == b.begin()) TEST_NOTE("this library shares the backing array");
    else                        TEST_NOTE("this library gives each list its own array");
    test_eq( il_total(a), 15 );
    test_eq( il_total(b), 15 );
    test_pass("cxx11:initializer_list backing arrays may share storage");

    TEST_NOTE("checked by test/compile_fail/initializer_list_illformed.cpp");
    test_skip("cxx11:initializer_list explicit/partial specialization is ill-formed");
    test_skip("cxx11:initializer_list narrowing element conversion is ill-formed");
}

TEST_CASE(initializer_list, additions_cxx26) {
#if defined(__cpp_lib_initializer_list) && (__cpp_lib_initializer_list) >= 202511L
    STD::initializer_list<int> il = { 1, 2 };
    STD::initializer_list<int> none;
    test_true( !il.empty() );
    test_true( none.empty() );
    test_true( il.empty() == (il.size() == 0) );
    test_pass("cxx26:initializer_list::empty");
#else
    TEST_NOTE("initializer_list::empty is C++26 (P3016R6); not in this library");
    test_skip("cxx26:initializer_list::empty");
#endif

    STD::initializer_list<int> il2 = { 4, 5, 6 };
    test_true( STD::begin(il2) == il2.begin() );
    test_true( STD::end(il2) == il2.end() );
    TEST_NOTE("only the positive half is observable: the removal itself is not");
    test_skip("cxx26:<initializer_list> does not declare free begin/end overloads");
}
#else
TEST_CASE(initializer_list, semantics_cxx11) {
    test_skip("cxx11:initializer_list");
    test_skip("cxx11:initializer_list braced-list construction (non-empty)");
    test_skip("cxx11:initializer_list element access is const");
    test_skip("cxx11:initializer_list::end == begin + size");
    test_skip("cxx11:initializer_list::begin == end when empty");
    test_skip("cxx11:initializer_list copy construction shares backing array");
    test_skip("cxx11:initializer_list copy assignment shares backing array");
    test_skip("cxx11:initializer_list backing array lifetime extension");
    test_skip("cxx11:initializer_list backing arrays may share storage");
    test_skip("cxx11:initializer_list explicit/partial specialization is ill-formed");
    test_skip("cxx11:initializer_list narrowing element conversion is ill-formed");
}
TEST_CASE(initializer_list, additions_cxx26) {
    test_skip("cxx26:initializer_list::empty");
    test_skip("cxx26:<initializer_list> does not declare free begin/end overloads");
}
#endif

TEST_CASE(initializer_list, feature_test_macros) {
#if defined(__cpp_lib_initializer_list)
    test_true( __cpp_lib_initializer_list > 0L );
    test_pass("cxx26:__cpp_lib_initializer_list == 202511L");
#else
    test_skip("cxx26:__cpp_lib_initializer_list == 202511L");
#endif
}
