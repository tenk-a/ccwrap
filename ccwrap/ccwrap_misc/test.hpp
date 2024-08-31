/**
 *  @file   test.hpp
 *  @brief  Unit test.
 *  @author Masashi Kitamura (tenka@6809.net)
 *  @date   2020
 *  @license Boost Software License Version 1.0
 *  @note
 *      Define 'USE_TEST' to test.
 *      Defining 'USE_TEST_MSW' will output the log to the debug window.
 *
 *      This header can be used without the need for any other ccwrap source.
 */
#ifndef CCWRAP_TEST_HPP_INCLUDED__
#define CCWRAP_TEST_HPP_INCLUDED__


#if !defined(USE_TEST) && !defined(USE_TEST_MSW)

#define TEST_SUITE(suite)     namespace __ccwraP_TestCase_##suite
#define TEST_CASE(suite,name) template<class DMY> void _ccwrap_##suite##_##TestCase_##name()
#define TEST_SUITES_RUN()

#define test_true(p)        ((void)0)
#define test_false(p)       ((void)0)
#define test_ptr(p)         ((void)0)
#define test_ptr0(p)        ((void)0)
#define test_lim(x,a,b)     ((void)0)

#define test_eq(l,r)        ((void)0)
#define test_ne(l,r)        ((void)0)
#define test_lt(l,r)        ((void)0)
#define test_le(l,r)        ((void)0)
#define test_gt(l,r)        ((void)0)
#define test_ge(l,r)        ((void)0)

#define test_throw(x)       ((void)0)
#define test_no_throw(x)    ((void)0)
#define test_fail()         ((void)0)

#define TEST_PUTS(x)        ((void)0)

#else   // USE_TEST

#include <cstddef>
#include <cstdio>
#include <cstdarg>
//#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <string_view>
#include <cstdarg>

#if !defined(__CCWRAP_NO_STRINGSTREAM)
#include <sstream>
#include "utf_cvt_stream.hpp"
#endif

#ifndef TEST_PUTS_BUF_SIZE
#define TEST_PUTS_BUF_SIZE  0x1000
#endif

#if defined(USE_TEST_MSW)
#include <windows.h>
#define TEST_PUTS(x)        OutputDebugStringA(x)
#else
#define TEST_PUTS(x)        std::fputs((x), stdout)
#endif

#define TEST_PRINTF(...)    ::ccwrap::_test::TestMgr<>::putf(__VA_ARGS__)
#define TEST_SUITES_RUN()   ::ccwrap::_test::TestMgr<>::instance().run()

#define TEST_SUITE(suite)                    \
namespace __ccwraP_TestCase_##suite {        \
    static ::ccwrap::_test::TestCase*		 \
       __ccwraP_TestCase_current_ = nullptr; \
}                                            \
using __ccwraP_TestCase_##suite::__ccwraP_TestCase_current_; \
namespace __ccwraP_TestCase_##suite


#define TEST_CASE(suite, name)                                              \
struct __ccwraP_TestCase_##suite##_##name {                                 \
    __ccwraP_TestCase_##suite##_##name() {                                  \
        testcase_ = ::ccwrap::_test::TestMgr<>::instance()                  \
            .add(#suite, #name, &__ccwraP_TestCase_##suite##_##name::run);  \
    }                                                                       \
    static void run() {                                                     \
		__ccwraP_TestCase_current_ = s_inst.testcase_;                      \
		run1();                                                             \
	}                                                                       \
    static void run1();                                                     \
private:                                                                    \
    ::ccwrap::_test::TestCase*	testcase_;									\
    static __ccwraP_TestCase_##suite##_##name  s_inst;                      \
};                                                                          \
__ccwraP_TestCase_##suite##_##name __ccwraP_TestCase_##suite##_##name::s_inst;  \
void  __ccwraP_TestCase_##suite##_##name::run1()

#define test_fail()       throw false

#define test_true(c)      ccwrap::_test::_test_true<void>(!!(c)  , #c, *__ccwraP_TestCase_current_, __FILE__, __LINE__)
#define test_false(c)     ccwrap::_test::_test_false<void>(!!(c) , #c, *__ccwraP_TestCase_current_, __FILE__, __LINE__)
#define test_ptr(p)       ccwrap::_test::_test_ptr<void>(p, false, #p, *__ccwraP_TestCase_current_, __FILE__, __LINE__)
#define test_ptr0(p)      ccwrap::_test::_test_ptr<void>(p, true , #p, *__ccwraP_TestCase_current_, __FILE__, __LINE__)
#define test_lim(x,a,b) \
     ccwrap::_test::_test_limit<void>(x, a, b, #x, #a, #b, *__ccwraP_TestCase_current_, __FILE__, __LINE__)

#define test_eq(l,r)      __test_cc(l,==,r)
#define test_ne(l,r)      __test_cc(l,!=,r)
#define test_lt(l,r)      __test_cc(l,< ,r)
#define test_le(l,r)      __test_cc(l,<=,r)
#define test_gt(l,r)      __test_cc(l,> ,r)
#define test_ge(l,r)      __test_cc(l,>=,r)

#define __test_cc(a,op,b) \
    ccwrap::_test::_test_cc_body<void>((a) op (b), a, b, #a,#op,#b, *__ccwraP_TestCase_current_, __FILE__, __LINE__)


#define test_str_eq(a,b)  __test_str_cc(a,==,b)
#define test_str_ne(a,b)  __test_str_cc(a,!=,b)  

#define __test_str_cc(a,op,b)  \
    ccwrap::_test::_test_cc_body<void>(string_view(a) op string_view(b)  \
    , string_view(a), string_view(b), #a,#op,#b, *__ccwraP_TestCase_current_, __FILE__, __LINE__)

// #define test_near_eq(l,r,delta)   ccwrap::test::_test_near_eq(l,==,r)

#define test_throw(x)                                \
    do {                                             \
        bool f = false;                              \
        try {                                        \
            x;                                       \
        } catch (...) {                              \
            f = true;                                \
        }                                            \
        if (!f)                                      \
            ccwrap::_test::_test_throw_msg<void>     \
               (#x,*__ccwraP_TestCase_current_,__FILE__,__LINE__); \
    } while (0)

#define test_no_throw(x)                             \
    do {                                             \
        bool f = true;                               \
        try {                                        \
            x;                                       \
        } catch (...) {                              \
            f = false;                               \
        }                                            \
        if (!f)                                      \
            ccwrap::_test::_test_no_throw_msg<void>  \
               (#x,*__ccwraP_TestCase_current_,__FILE__,__LINE__); \
    } while (0)



namespace ccwrap {

using std::string;
using std::string_view;

namespace _test {

struct TestCase {
    TestCase() : group_(NULL), name_(NULL), func_(NULL), errors_(0) {}
    TestCase(char const* g, char const* n, void (*f)())
    	: group_(g), name_(n), func_(f), errors_(0) {}
    void err(char const* fname, unsigned int ln);

    char const* group_;
    char const* name_;
    void       (*func_)();
    unsigned   errors_;
};

template<class DMY=void>
class TestMgr {
public:
    typedef std::vector< std::unique_ptr<TestCase> >	TestCases;
    typedef std::map<string, TestCases> TestGroups;

    TestMgr() {}
    ~TestMgr() {}
    static TestMgr& instance() { static TestMgr s_inst; return s_inst; }
    TestCase*       add(char const* suite, char const* name, void (*fnc)());
    void            run();
    static void     putf(char const* fmt, ...);
private:
    TestGroups  groups_;
};

inline void TestCase::err(char const* fname, unsigned int ln) {
    ++this->errors_;
    TestMgr<>::putf("%s (%d) [%s.%s]: ", fname, ln, group_, name_);
}

template<class DMY>
void TestMgr<DMY>::putf(char const* fmt, ...) {
	using namespace std;
    char buf[TEST_PUTS_BUF_SIZE];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, (sizeof buf) - 1, fmt, ap);
    va_end(ap);
    buf[(sizeof buf) - 1] = 0;
    TEST_PUTS(buf);
}

template<class DMY>
inline TestCase* TestMgr<DMY>::add(char const* suite, char const* name, void (*fnc)()) {
	TestCases& cases = groups_[suite];
    cases.push_back(std::unique_ptr<TestCase>());
    cases.back().reset(new TestCase(suite, name, fnc));
    return &*cases.back();
}

template<class DMY>
void TestMgr<DMY>::run() {
    TEST_PRINTF("[TEST]...\n");
    unsigned gok = 0;
    for (TestGroups::iterator gite = groups_.begin(); gite != groups_.end(); ++gite) {
        TestCases& cases = gite->second;
        unsigned idx = 0;
        unsigned sz  = unsigned(cases.size());
        unsigned oks = 0;
        unsigned errors = 0;
        for (TestCases::iterator cite = cases.begin(); cite != cases.end(); ++cite, ++idx) {
            TestCase& tc = **cite;
            tc.errors_ = 0;
            try {
                tc.func_();
            } catch (...) {
                ++tc.errors_;
            }
            bool rc = (tc.errors_ == 0);
            oks    += rc;
            errors += !rc;
            TEST_PRINTF("   %s  %s [%d/%d]\t(%s)\n", (rc) ? " ok" : "NG!", tc.group_, idx+1, sz, tc.name_);
        }
        TEST_PRINTF("\t    %s: %d %s. %d %s.\n", gite->first.data()
                , oks, (oks > 1) ? "successes":"success"
                , errors, (errors>1) ? "failures":"failure" );
        gok += (errors == 0);
    }
    unsigned gsz  = unsigned(groups_.size());
    unsigned gerr = gsz - gok;
    TEST_PRINTF("[Result] Run %d %s, %d %s, %d %s.\n\n"
                , gsz , (gsz  > 1) ? "test-suites":"test-suite"
                , gok , (gok  > 1) ? "successes":"success"
                , gerr, (gerr > 1) ? "failures" : "failure" );
    //TestGroups().swap(groups_);
}


inline bool _test_is_mem_addr(void const* p) {
    std::size_t s = (std::size_t)p;
    const std::size_t mi = 0xff;
    const std::size_t ma = ~std::size_t(mi);
    return (mi < s && s < ma);
}

inline bool _test_is_aligned(void const* p, std::size_t aln) {
    std::size_t s = (std::size_t)p;
    std::size_t m = aln - 1;
    return ((s & m) == 0);
}
inline bool _test_is_aligned(void const* /*p*/) {
    return true;
}
template<typename T> bool _test_is_aligned(T const* p) {
    return _test_is_aligned(p, alignof(T));
}

template<class DMY> void
_test_true(bool cc, char const* ccStr, TestCase& tcase, char const* fname, unsigned int line) {
    if (!cc) {
        tcase.err(fname, line);
        TEST_PRINTF("'%s' is failed.\n", ccStr);
    }
}

template<class DMY> void
_test_false(bool cc, char const* ccStr, TestCase& tcase, char const* fname, unsigned int line) {
    if (cc) {
        tcase.err(fname, line);
        TEST_PRINTF("'%s' is true.\n", ccStr);
    }
}

template<class DMY> void
_test_ptr_impl(void const* ptr, bool zok, bool aok, char const* ptrStr
            , TestCase& tcase, char const* fname, unsigned int line)
{
    if (ptr == 0) {
        if (!zok) {
            tcase.err(fname, line);
            TEST_PRINTF("'%s'(%p) is null pointer.\n", ptrStr, ptr);
        }
    } else if (!_test_is_mem_addr(ptr)) {
        tcase.err(fname, line);
        TEST_PRINTF("'%s'(%p) is bad pointer.\n", ptrStr, ptr);
    } else if (!aok) {
        tcase.err(fname, line);
        TEST_PRINTF("'%s'(%p) is bad alignment.\n", ptrStr, ptr);
    }
}

template<class DMY, typename T> void
_test_ptr(T const* ptr, bool zok, char const* ptrStr
            , TestCase& tcase, char const* fname, unsigned int line)
{
    _test_ptr_impl<DMY>((void const*)ptr, zok, _test_is_aligned(ptr), ptrStr, tcase, fname, line);
}

#if !defined(CCWRAP_TEST_LOG_LITE)
template<class DMY, typename A, typename B> void
_test_cc_body(bool cc, A const& a, B const &b, char const* astr, char const* op, char const* bstr
               , TestCase& tcase, char const* fname, unsigned int line)
{
    if (!cc) {
        tcase.err(fname, line);
	  #if !defined(__CCWRAP_NO_STRINGSTREAM)
        std::stringstream st;
        //st << "\"'" << astr << "'(" << a << ") " << op << " '" << bstr << "'(" << b << ")\" is failed.\n";
        st << "'" << astr << ' ' << op << ' ' << bstr << "' is failed. (L='" << a << "' R='" << b << "')\n";
        TEST_PRINTF("%s", st.str().data());
	  #else
        TEST_PRINTF("'%s' %s '%s' is failed.\n", astr, op, bstr);
      #endif
    }
}
#else
template<class DMY, typename A, typename B> void
_test_cc_body(bool cc, A const& a, B const &b, char const* astr, char const* op, char const* bstr
               , TestCase& tcase, char const* fname, unsigned int line)
{
    if (!cc) {
        tcase.err(fname, line);
        TEST_PRINTF("'%s %s %s' is failed.\n", astr, op, bstr);
    }
}
#endif

#if !defined(CCWRAP_TEST_LOG_LITE)
template<class DMY, typename X, typename A, typename B> void
_test_limit(X const& x, A const& a, B const& b, char const* xstr, char const* astr, char const* bstr
            , TestCase& tcase, char const* fname, unsigned int line)
{
    if (!(a <= x && x <= b)) {
        tcase.err(fname, line);
	  #if !defined(__CCWRAP_NO_STRINGSTREAM)
        std::stringstream st;
        st << "'" << xstr << "'(" << x << ") is out of range[" << a << ", " << b << "]. (["
            << astr << ", " << bstr << "])\n";
        TEST_PRINTF("%s", st.str().data());
	  #else
        TEST_PRINTF("'%s' is out of range[%s,%s].\n", xstr, astr, bstr);
      #endif
    }
}
#else
template<class DMY, typename X, typename A, typename B> void
_test_limit(X const& x, A const& a, B const& b, char const* xstr, char const* astr, char const* bstr
            , TestCase& tcase, char const* fname, unsigned int line)
{
    if (!(a <= x && x <= b)) {
        tcase.err(fname, line);
        TEST_PRINTF("'%s' is out of range[%s,%s]\n", xstr, astr, bstr);
    }
}
#endif

#if 0
template<class DMY, typename X, typename A, typename B> void
_test_limit(X const* x, A const* a, B const* b, char const* xstr, char const* astr, char const* bstr
            , TestCase& tcase, char const* fname, unsigned int line)
{
    if (!((char const*)a <= (char const*)x && (char const*)x <= (char const*)b)) {
        tcase.err(fname, line);
        TEST_PRINTF("'%s'(%p) is out of range[%p, %p].([%s,%s])\n", xstr, x, a, b, astr, bstr);
    }
}
#endif

template<class DMY> void
_test_throw_msg(char const* str, TestCase& tcase, char const* fname, unsigned int line) {
    tcase.err(fname, line);
    TEST_PRINTF("'%s' did not throw an exception.\n", str);
}

template<class DMY> void
_test_no_throw_msg(char const* str, TestCase& tcase, char const* fname, unsigned int line) {
    tcase.err(fname, line);
    TEST_PRINTF("'%s' threw an exception.\n", str);
}

} // test

} // ccwrap

#endif  // USE_TEST

#endif  //  TEST_HPP_INCLUDED__
