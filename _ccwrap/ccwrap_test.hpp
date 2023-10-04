/**
 *  @file   ccwrap_test.hpp
 *  @brief  Unit test.
 *  @author Masashi Kitamura (tenka@6809.net)
 *  @date   2020
 *  @license Boost Software License Version 1.0
 *  @note
 *      Define 'USE_CCWRAP_TEST' to test.
 *      Defining 'USE_CCWRAP_TEST_MSW' will output the log to the debug window.
 *
 *      This header can be used without the need for any other ccwrap source.
 */
#ifndef __ccwrap_test_hpp_INCLUDED
#define __ccwrap_test_hpp_INCLUDED

#if !defined(USE_CCWRAP_TEST) && !defined(USE_CCWRAP_TEST_MSW)

#define CCWRAP_TEST_SUITE(name)      namespace __Fkstd_TestSuite_##name

#define CCWRAP_TEST(name)            template<class DMY> void __Fkstd_TestMethod_##name()
#define CCWRAP_TEST_RUN()

#define ccwrap_test_assert(p)
#define ccwrap_test(p)
#define ccwrap_test_ptr(p)
#define ccwrap_test_ptr0(p)
#define ccwrap_test_range(x,a,b)

#define ccwrap_test_eq(l,r)
#define ccwrap_test_ne(l,r)
#define ccwrap_test_lt(l,r)
#define ccwrap_test_le(l,r)
#define ccwrap_test_gt(l,r)
#define ccwrap_test_ge(l,r)

#define ccwrap_test_throw(x)
#define ccwrap_test_no_throw(x)

#define CCWRAP_TEST_PRINTF(...)

#else   // USE_CCWRAP_TEST

#include <cstddef>
#include <cstdio>
#include <algorithm>
#include <string>
#include <list>
#include <sstream>

#if defined(USE_CCWRAP_TEST_MSW)
#include <cstdarg>
#include <windows.h>
#if !defined(__CCWRAP_DEBUG_LOG_PRINTF)
 #if defined(_WIN32) && defined(__cplusplus)
  namespace __ccwrap {
    template<class DMY>
    int debug_log_vprintf(char const* fmt, va_list ap) {
        enum { BUF_SZ = 0x600 };
        char buf[BUF_SZ];
        vsnprintf(buf, BUF_SZ, fmt, ap);
        buf[sizeof(buf) - 1] = '\0';
        wchar_t wbuf[BUF_SZ];
        MultiByteToWideChar(0,0,buf, int(strlen(buf)+1), &wbuf[0], BUF_SZ);
        OutputDebugStringW(wbuf);
        return 0;
    }
    inline int debug_log_printf(char const* fmt, ...) {
        va_list ap;
        va_start(ap, fmt);
        debug_log_vprintf<void>(fmt, ap);
        va_end(ap);
        return 0;
    }
  }   // __ccwrap
  #define __CCWRAP_DEBUG_LOG_VPRINTF(fmt,val)    __ccwrap::debug_log_vprintf<void>(fmt,val)
  #define __CCWRAP_DEBUG_LOG_PRINTF(...)         __ccwrap::debug_log_printf(__VA_ARGS__)
 #else  // _WIN32
  #define __CCWRAP_DEBUG_LOG_VPRINTF(fmt,val)    std::vfprintf(stderr, fmt, val)
  #define __CCWRAP_DEBUG_LOG_PRINTF(...)         std::fprintf(stderr, __VA_ARGS__)
 #endif // _WIN32
#endif  // __CCWRAP_DEBUG_LOG_PRINTF
#endif  // USE_CCWRAP_TEST_MSW

#if defined(USE_CCWRAP_TEST_MSW)
#define CCWRAP_TEST_PRINTF(...)        __CCWRAP_DEBUG_LOG_PRINTF(__VA_ARGS__)
#else
#define CCWRAP_TEST_PRINTF(...)        std::printf(__VA_ARGS__)
#endif

#if defined(USE_CCWRAP_TEST_MSW)
#if !defined(__CCWRAP_DEBUG_LOG_WPRINTF)
 #if defined(_WIN32) && defined(__cplusplus)
  namespace __ccwrap {
    template<class DMY>
    int debug_log_vwprintf(wchar_t const* fmt, va_list ap) {
        enum { BUF_SZ = 0x600 };
        wchar_t buf[BUF_SZ];
        vswprintf(buf, BUF_SZ, fmt, ap);
        buf[sizeof(buf) - 1] = 0;
        OutputDebugStringW(buf);
        return 0;
    }
    inline int debug_log_wprintf(wchar_t const* fmt, ...) {
        va_list ap;
        va_start(ap, fmt);
        debug_log_vwprintf<void>(fmt, ap);
        va_end(ap);
        return 0;
    }
  }   // __ccwrap
  #define __CCWRAP_DEBUG_LOG_VWPRINTF(fmt,val)    __ccwrap::debug_log_vwprintf<void>(fmt,val)
  #define __CCWRAP_DEBUG_LOG_WPRINTF(...)         __ccwrap::debug_log_wprintf(__VA_ARGS__)
 #else  // !_WIN32
  #define __CCWRAP_DEBUG_LOG_WVPRINTF(fmt,val)    std::vfwprintf(stderr, fmt, val)
  #define __CCWRAP_DEBUG_LOG_WPRINTF(...)         std::fwprintf(stderr, __VA_ARGS__)
 #endif // _WIN32
#endif  // __CCWRAP_DEBUG_LOG_WPRINTF
#endif  // USE_CCWRAP_TEST_MSW
#if defined(USE_CCWRAP_TEST_MSW)
#define CCWRAP_TEST_WPRINTF(...)       __CCWRAP_DEBUG_LOG_WPRINTF(__VA_ARGS__)
#else
#define CCWRAP_TEST_WPRINTF(...)       std::wprintf(__VA_ARGS__)
#endif

namespace __ccwrap { namespace test {

template<class DMY=void>
struct TestSuite {
    struct Method {
        Method() : func_(NULL), errors_(0) {}
        Method(TestSuite* suite, std::string const& nm, void (*fnc)())
            : suite_(suite), name_(nm), lname_(suite_->name() + ("/" + name_)), func_(fnc), errors_(0) {}
        char const*     ename() const { return name_.c_str(); }
        char const*     lname() const { return lname_.c_str(); }
        void            addErr() { ++errors_; }
    public:
        TestSuite*      suite_;
        std::string     name_;
        std::string     lname_;
        void            (*func_)();
        unsigned        errors_;
    };
    typedef std::list<Method> Methods;
public:
    TestSuite(char const* name);
    bool run();
    void add(char const* name, void (*fnc)());
    Method&         cur() { return *cur_; }
    char const*     name() const { return name_.c_str(); }
private:
    std::string     name_;
    Methods         methods_;
    Method*         cur_;
};

template<class DMY>
TestSuite<DMY>::TestSuite(char const* name)
    : name_(name)
    , cur_(NULL)
{
}

template<class DMY>
void TestSuite<DMY>::add(char const* name, void (*fnc)()) {
    methods_.push_back(Method(this, name, fnc));    // methods_.emplace_back(this, name, fnc);
}

template<class DMY>
bool TestSuite<DMY>::run() {
    char const* name = name_.c_str();
    //CCWRAP_TEST_PRINTF("\t%s:\r", name);
    unsigned oks    = 0;
    unsigned errors = 0;
    unsigned sz     = unsigned(methods_.size());
    unsigned idx    = 0;
    for (typename Methods::iterator ite = methods_.begin(); ite != methods_.end(); ++ite, ++idx) {
        Method& tm = *ite;
        cur_    = &tm;
        try {
            tm.func_();
        } catch (...) {
            ++tm.errors_;
        }
        bool rc = tm.errors_ == 0;
        oks    += rc;
        errors += !rc;
        CCWRAP_TEST_PRINTF("   %s  %s [%d/%d]\t(%s)\n", (rc) ? " ok" : "NG!", name, idx+1, sz, tm.ename());
    }
    CCWRAP_TEST_PRINTF("\t    %s: %d %s. %d %s.\n", name
                        , oks, (oks > 1) ? "successes":"success"
                        , errors, (errors>1) ? "failures":"failure" );
    return errors == 0;
}

template<class DMY=void>
class TestMgr {
    typedef std::list< TestSuite<> >    Suites;
public:
    TestMgr();
    ~TestMgr();
    static TestMgr& instance();
    TestSuite<>*    add(char const* name);
    void            run();
private:
    Suites          suites_;
};
template<class DMY> TestMgr<DMY>::TestMgr() {}
template<class DMY> TestMgr<DMY>::~TestMgr() {}

template<class DMY>
TestMgr<DMY>& TestMgr<DMY>::instance() {
    static TestMgr s_instance;
    return s_instance;
}

template<class DMY>
TestSuite<>* TestMgr<DMY>::add(char const* name) {
    suites_.push_back( TestSuite<>(name) ); // test_.emplace_back( name );
    return &suites_.back();
}

template<class DMY>
void TestMgr<DMY>::run() {
    CCWRAP_TEST_PRINTF("[TEST]...\n");
    unsigned oks = 0;
    unsigned sz  = unsigned(suites_.size());
    for (Suites::iterator ite = suites_.begin(); ite != suites_.end(); ++ite) {
        oks += ite->run();
    }
    unsigned errors = sz - oks;
    CCWRAP_TEST_PRINTF("[Result] Ran %d %s, %d %s, %d %s.\n\n"
                        , sz, (sz > 1) ? "test-suites":"test-suite"
                        , oks, (oks > 1) ? "successes":"success"
                        , errors, (errors > 1) ? "failures" : "failure" );
    Suites().swap(suites_);
}

} } // __ccwrap::test

#define CCWRAP_TEST_SUITE(name)                                             \
namespace __Fkstd_TestSuite_##name {                                        \
    static __ccwrap::test::TestSuite<>& testSuite() {                       \
        static __ccwrap::test::TestSuite<>*  s_instance = 0;                \
        if (!s_instance)                                                    \
            s_instance =__ccwrap::test::TestMgr<>::instance().add(#name);   \
        return *s_instance;                                                 \
    }                                                                       \
}                                                                           \
namespace __Fkstd_TestSuite_##name

#define CCWRAP_TEST(name)                                                   \
struct __TestMethod_##name {                                                \
    __TestMethod_##name() {                                                 \
        testSuite().add(#name, &__TestMethod_##name::run);                  \
    }                                                                       \
    static void run();                                                      \
private:                                                                    \
    static __TestMethod_##name  s_instance;                                 \
};                                                                          \
__TestMethod_##name __TestMethod_##name::s_instance;                        \
void                __TestMethod_##name::run()

#define CCWRAP_TEST_RUN()        __ccwrap::test::TestMgr<void>::instance().run()

#define ccwrap_test_assert(c)    __ccwrap::test::__ccwrap_test_assert<void>(!!(c), #c, testSuite().cur(), __FILE__, __LINE__)
#define ccwrap_test(c)           ccwrap_test_assert(c)
#define ccwrap_test_ptr(p)       __ccwrap::test::__ccwrap_test_ptr<void>(p, #p, testSuite().cur(), __FILE__, __LINE__)
#define ccwrap_test_ptr0(p)      __ccwrap::test::__ccwrap_test_ptr0<void>(p, #p, testSuite().cur(), __FILE__, __LINE__)
#define ccwrap_test_range(x,a,b) \
     __ccwrap::test::__ccwrap_test_range<void>(x, a, b, #x, #a, #b, testSuite().cur(), __FILE__, __LINE__)

#define ccwrap_test_eq(l,r)      __ccwrap_test_cc(l,==,r)
#define ccwrap_test_ne(l,r)      __ccwrap_test_cc(l,!=,r)
#define ccwrap_test_lt(l,r)      __ccwrap_test_cc(l,< ,r)
#define ccwrap_test_le(l,r)      __ccwrap_test_cc(l,<=,r)
#define ccwrap_test_gt(l,r)      __ccwrap_test_cc(l,> ,r)
#define ccwrap_test_ge(l,r)      __ccwrap_test_cc(l,>=,r)

// #define ccwrap_test_near_eq(l,r,delta)   __ccwrap::test::__ccwrap_test_near_eq(l,==,r)

#define __ccwrap_test_cc(a,op,b) \
    __ccwrap::test::__ccwrap_test_cc_body<void>((a) op (b), a, b, #a,#op,#b, testSuite().cur(), __FILE__, __LINE__)


#define ccwrap_test_throw(x)                                    \
    do {                                                        \
        bool f = false;                                         \
        try {                                                   \
            x;                                                  \
        } catch (...) {                                         \
            f = true;                                           \
        }                                                       \
        if (!f)                                                 \
            __ccwrap::test::__ccwrap_test_throw_msg<void>       \
                (#x,testSuite().cur(),__FILE__,__LINE__);       \
    } while (0)

#define ccwrap_test_no_throw(x)                                 \
    do {                                                        \
        bool f = true;                                          \
        try {                                                   \
            x;                                                  \
        } catch (...) {                                         \
            f = false;                                          \
        }                                                       \
        if (!f)                                                 \
            __ccwrap::test::__ccwrap_test_no_throw_msg<void>    \
                (#x,testSuite().cur(),__FILE__,__LINE__);       \
    } while (0)


#ifndef __CCWRAP_IS_MEM_ADDR
 #ifdef __cplusplus
  #define __CCWRAP_INT_TO_CPTR(x)    ((char const*)std::size_t(x))
 #else
  #define __CCWRAP_INT_TO_CPTR(x)    ((char const*)size_t(x))
 #endif
 #if defined _WIN64
  #define __CCWRAP_IS_MEM_ADDR(p)    ((char const*)(p) >= __CCWRAP_INT_TO_CPTR(0x10000) && (char const*)(p) <= __CCWRAP_INT_TO_CPTR(0xFFFF000000000000LL))
 #elif defined _WIN32
  #define __CCWRAP_IS_MEM_ADDR(p)    ((char const*)(p) >= __CCWRAP_INT_TO_CPTR(0x10000) && (char const*)(p) <= __CCWRAP_INT_TO_CPTR(0xF0000000))
 #elif __CCWRAP_CPU_BIT == 64
  #define __CCWRAP_IS_MEM_ADDR(p)    ((char const*)(p) >= __CCWRAP_INT_TO_CPTR(0x100)  && (char const*)(p) <= __CCWRAP_INT_TO_CPTR(0xFFFFffffFFFFff00))
 #else
  #define __CCWRAP_IS_MEM_ADDR(p)    ((char const*)(p) >= __CCWRAP_INT_TO_CPTR(0x100)  && (char const*)(p) <= __CCWRAP_INT_TO_CPTR(0xFFFFff00))
 #endif
 #define __CCWRAP_IS_MEM_ADDR0(p)    (!(p) || __CCWRAP_IS_MEM_ADDR(p))
#endif

namespace __ccwrap { namespace test {
typedef TestSuite<>::Method     TestMethod;

template<class DMY>
void    __ccwrap_test_assert(bool cc, char const* ccStr, TestMethod& method, char const* fname, int line) {
    if (!cc) {
        method.addErr();
        CCWRAP_TEST_PRINTF("%s (%d): %s: `%s' is failed.\n", fname, line, method.lname(), ccStr);
    }
}

template<class DMY>
void    __ccwrap_test_ptr(void const* ptr, char const* ptrStr, TestMethod& method, char const* fname, int line) {
    if (!__CCWRAP_IS_MEM_ADDR(ptr)) {
        method.addErr();
        CCWRAP_TEST_PRINTF("%s (%d): %s: `%s'(0x%p) is bad pointer.\n", fname, line, method.lname(), ptrStr, ptr);
    }
}
template<class DMY, typename T>
void    __ccwrap_test_ptr(T const* ptr, char const* ptrStr, TestMethod& method, char const* fname, int line) {
    __ccwrap_test_ptr<DMY>((void const*)ptr, ptrStr, method, fname, line);
}

template<class DMY>
void    __ccwrap_test_ptr0(void const* ptr, char const* ptrStr, TestMethod& method, char const* fname, int line) {
    if (!__CCWRAP_IS_MEM_ADDR0(ptr)) {
        method.addErr();
        CCWRAP_TEST_PRINTF("%s (%d): %s: `%s'(0x%p) is bad pointer.\n", fname, line, method.lname(), ptrStr, ptr);
    }
}
template<class DMY, typename T>
void    __ccwrap_test_ptr0(T const* ptr, char const* ptrStr, TestMethod& method, char const* fname, int line) {
    __ccwrap_test_ptr0<DMY>((void const*)ptr, ptrStr, method, fname, line);
}


template<class DMY, typename A, typename B>
void    __ccwrap_test_cc_body(bool cc, A const& a, B const &b, char const* astr, char const* op, char const* bstr
                            , TestMethod& method, char const* fname, int line)
{
    if (!cc) {
        method.addErr();
        std::stringstream ss;
        ss << "\"`" << astr << "'(" << a << ") " << op << " `" << bstr << "'(" << b << ")\" is failed.";
        CCWRAP_TEST_PRINTF("%s (%d): %s: %s\n", fname, line, method.lname(), ss.str().c_str());
    }
}

template<class DMY, typename X, typename A, typename B>
void    __ccwrap_test_range(X const& x, A const& a, B const& b, char const* xstr, char const* astr, char const* bstr
                            , TestMethod& method, char const* fname, int line)
{
    if (!(a <= x && x <= b)) {
        method.addErr();
        std::stringstream ss;
        ss << "`" << xstr << "'(" << x << ") is out of range[" << a << ", " << b << "]. (["
            << astr << ", " << bstr << "])";
        CCWRAP_TEST_PRINTF("%s (%d): %s: %s\n", fname, line, method.lname(), ss.str().c_str());
    }
}

template<class DMY, typename X, typename A, typename B>
void    __ccwrap_test_range(X const* x, A const* a, B const* b, char const* xstr, char const* astr, char const* bstr
                            , TestMethod& method, char const* fname, int line)
{
    if (!((char const*)a <= (char const*)x && (char const*)x <= (char const*)b)) {
        method.addErr();
        CCWRAP_TEST_PRINTF("%s (%d): %s: `%s`(0x%p) is out of range[0x%p, 0x%p].([%s,%s])\n"
            , fname, line, method.lname(), xstr, x, a, b, astr, bstr);
    }
}

template<class DMY>
void    __ccwrap_test_throw_msg(char const* str, TestMethod& method, char const* fname, int line) {
    method.addErr();
    CCWRAP_TEST_PRINTF("%s (%d): %s: `%s' did not throw an exception.\n", fname, line, method.lname(), str);
}

template<class DMY>
void    __ccwrap_test_no_throw_msg(char const* str, TestMethod& method, char const* fname, int line) {
    method.addErr();
    CCWRAP_TEST_PRINTF("%s (%d): %s: `%s' threw an exception.\n", fname, line, method.lname(), str);
}

} } // __ccwrap::test


#endif  // USE_CCWRAP_TEST

#endif  //  __ccwrap_test_hpp_INCLUDED
