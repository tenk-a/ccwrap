/**
 *  @file   test.hpp
 *  @brief  Unit test.
 *  @author Masashi Kitamura (tenka@6809.net)
 *  @date   2020-2026
 *  @license Boost Software License Version 1.0
 */
#ifndef TEST_HPP_INCLUDED__
#define TEST_HPP_INCLUDED__

#ifndef STD
#define STD std
#endif

#if defined(_MSVC_LANG)
#  define _TeST_RAW_CPLUSPLUS _MSVC_LANG
#elif defined(__cplusplus)
#  define _TeST_RAW_CPLUSPLUS __cplusplus
#else
#  define _TeST_RAW_CPLUSPLUS 0L
#endif
#if   _TeST_RAW_CPLUSPLUS >= 202602L
#  define _TeST_CXX_FROM_STD 2026
#elif _TeST_RAW_CPLUSPLUS >= 202302L
#  define _TeST_CXX_FROM_STD 2023
#elif _TeST_RAW_CPLUSPLUS >= 202002L
#  define _TeST_CXX_FROM_STD 2020
#elif _TeST_RAW_CPLUSPLUS >= 201703L
#  define _TeST_CXX_FROM_STD 2017
#elif _TeST_RAW_CPLUSPLUS >= 201402L
#  define _TeST_CXX_FROM_STD 2014
#elif _TeST_RAW_CPLUSPLUS >= 201103L
#  define _TeST_CXX_FROM_STD 2011
#else
#  define _TeST_CXX_FROM_STD 2003
#endif
#ifndef TEST_TARGET_CXX
#  if defined(_CCW_TARGET_CXX) && (_CCW_TARGET_CXX) > _TeST_CXX_FROM_STD
#    define TEST_TARGET_CXX (_CCW_TARGET_CXX)
#  else
#    define TEST_TARGET_CXX _TeST_CXX_FROM_STD
#  endif
#endif
#ifndef TEST_TARGET_C
#  if defined(_CCW_TARGET_C)
#    define TEST_TARGET_C ((_CCW_TARGET_C) < 2011 ? 2011 : (_CCW_TARGET_C))
#  else
#    define TEST_TARGET_C 0
#  endif
#endif

// 1 when the build has exceptions. A case whose body needs try/catch/throw has
// to be #if'd out when this is 0 -- the keywords are rejected, not just unusable.
#ifndef TEST_HAS_EH
#  if defined(_CCW_HAS_EXCEPTIONS)
#    define TEST_HAS_EH (_CCW_HAS_EXCEPTIONS)
#  elif defined(_MSC_VER) || defined(__WATCOMC__)
#    if defined(_CPPUNWIND)
#      define TEST_HAS_EH 1
#    else
#      define TEST_HAS_EH 0
#    endif
#  elif defined(__GNUC__) || defined(__clang__)
#    if defined(__EXCEPTIONS)
#      define TEST_HAS_EH 1
#    else
#      define TEST_HAS_EH 0
#    endif
#  else
#    define TEST_HAS_EH 1
#  endif
#endif

// Exception specification for a virtual override (codecvt::do_encoding etc.).
#ifndef TEST_NOTHROW
#  if TEST_HAS_EH
#    define TEST_NOTHROW throw()
#  else
#    define TEST_NOTHROW
#  endif
#endif

#if defined(TEST_DISABLE)

#define TEST_CASE(group,name) template<class DMY> void __TesT_##group##_##TestCase_##name()
#define TEST_CASE_SKIP(group,name) TEST_CASE(group,name) {}
#define TEST_RUN()          (0u)

#define test_true(p)        ((void)0)
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

#define TEST_PUTS(x)        ((void)0)

#define TEST_SKIP1()             ((void)0)
#define TEST_SKIP_N(n)           ((void)0)
#define test_skip(s)             ((void)sizeof(s))
#define test_pass(s)             ((void)sizeof(s))
#define TEST_NOTE(s)             ((void)sizeof(s))
#define TEST_SET_PASS_LOG_DIR(d) ((void)sizeof(d))
#define TEST_SKIP_WAT(...)       ((void)0)
#define TEST_SKIP_DOS(...)       ((void)0)
#define TEST_SKIP_VC(...)        ((void)0)
#define TEST_SKIP_GCC(...)       ((void)0)
#define TEST_SKIP_VC080(...)     ((void)0)
#define TEST_SKIP_VC090(...)     ((void)0)
#define TEST_SKIP_NATIVE_NO_MOVE(...) ((void)0)
#define TEST_SKIP_VC100(...)     ((void)0)
#define TEST_SKIP_VC110(...)     ((void)0)
#define TEST_SKIP_VC120(...)     ((void)0)
#define TEST_SKIP_VC140(...)     ((void)0)
#define TEST_SKIP_VC141(...)     ((void)0)
#define TEST_SKIP_VC142(...)     ((void)0)
#define TEST_SKIP_VC143(...)     ((void)0)
#define TEST_SKIP_VC145(...)     ((void)0)
#define TEST_SKIP_VC_RANGE(lo, hi, ...)  ((void)0)
#define TEST_C90(x)     ((void)0)
#define TEST_C99(x)     ((void)0)
#define TEST_C11(x)     ((void)0)
#define TEST_C17(x)     ((void)0)
#define TEST_C23(x)     ((void)0)
#define TEST_CXX03(x)   ((void)0)
#define TEST_CXX11(x)   ((void)0)
#define TEST_CXX14(x)   ((void)0)
#define TEST_CXX17(x)   ((void)0)
#define TEST_CXX20(x)   ((void)0)
#define TEST_CXX23(x)   ((void)0)
#define TEST_CXX26(x)   ((void)0)

#else   // ===================================================================

#include <cstddef>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <vector>
#include <string>

#if defined(TST_HEAPCHK)
extern "C" int _heapchk(void);
#endif
#include <map>
#include <memory>

#if TEST_TARGET_CXX >= 2017
#include <string_view>
#elif defined(__has_include) && __has_include(<ccwrap_common.h>)
#include "../../detail/cxx/string_view.hpp"
#else
#include <string>
#endif

#if defined(TEST_USE_STRBUFF)
#include "strbuff.hpp"
#define _TEST_STREAM_DEF(st)    char __b[1024]; strbuff_ref st(__b, 1024)
#define _TEST_STREAM_CSTR(st)   (st).c_str()
#else
#include <sstream>
#define _TEST_STREAM_DEF(st)    STD::stringstream st
#define _TEST_STREAM_CSTR(st)   (st).str().c_str()
#endif

#ifndef _TEST_STR
#define _TEST_STR               STD::string
#endif

#ifndef TEST_PUTS_BUF_SIZE
#define TEST_PUTS_BUF_SIZE      0x1000
#endif

#if defined(TEST_USE_MSW)
#include <windows.h>
#define TEST_PUTS(x)            OutputDebugStringA(x)
#else
#define TEST_PUTS(x)            STD::fputs(x, stdout)
#endif

#define TEST_PRINTF(...)        _ccw::_test::TestMgr<>::putf(__VA_ARGS__)
#define TEST_RUN()              _ccw::_test::TestMgr<>::instance().run()

/* TEST_GROUP_SUFFIX lets one test source be compiled again as another suite
   (the <cxxx> and the <xxx.h> variant; see string_h_test.cpp). */
#ifndef TEST_GROUP_SUFFIX
#define TEST_GROUP_SUFFIX
#endif
#define _TeST_CAT(a, b)             _TeST_CAT_I(a, b)
#define _TeST_CAT_I(a, b)           a##b

#define TEST_CASE(group, name)                                                 \
    _TeST_CASE_II(_TeST_CAT(group, TEST_GROUP_SUFFIX), name)
#define TEST_CASE_SKIP(group, name)                                            \
    _TeST_CASE_SKIP_II(_TeST_CAT(group, TEST_GROUP_SUFFIX), name)

#define _TeST_CASE_II(group, name)      _TeST_CASE_I(group, name)
#define _TeST_CASE_SKIP_II(group, name) _TeST_CASE_SKIP_I(group, name)

#define _TeST_CASE_I(group, name)                                              \
struct __TesT_TestCase_##group##_##name {                                   \
    __TesT_TestCase_##group##_##name() {                                    \
        testcase_ = ::_ccw::_test::TestMgr<>::instance()                    \
            .add(#group, #name, &__TesT_TestCase_##group##_##name::run);    \
    }                                                                       \
    static void run();                                                      \
    static ::_ccw::_test::TestCase*  _test_case()                           \
        { return s_inst.testcase_; }                                        \
private:                                                                    \
    ::_ccw::_test::TestCase* testcase_;                                     \
    static __TesT_TestCase_##group##_##name  s_inst;                        \
};                                                                          \
__TesT_TestCase_##group##_##name __TesT_TestCase_##group##_##name::s_inst;  \
void  __TesT_TestCase_##group##_##name::run()

#define _TeST_CASE_SKIP_I(group, name)                                      \
_TeST_CASE_I(group, name) { _ccw::_test::TestMgr<>::instance().setCaseSkip(); }

#if TEST_HAS_EH
#define _TeST_TRY         try
#define _TeST_CATCH_ALL   catch (...)
#define test_fail()       throw false
#else
#define _TeST_TRY         if (1)
#define _TeST_CATCH_ALL   else if (0)
#define test_fail()       _ccw::_test::_test_true<void>(false, "test_fail()", \
                              *_test_case(), __FILE__, __LINE__)
#endif

#define test_true(c)      _ccw::_test::_test_true<void>(!!(c)  , #c, *_test_case(), __FILE__, __LINE__)
#define test_ptr(p)       _ccw::_test::_test_ptr<void>(p, false, #p, *_test_case(), __FILE__, __LINE__)
#define test_ptr0(p)      _ccw::_test::_test_ptr<void>(p, true , #p, *_test_case(), __FILE__, __LINE__)
#define test_lim(x,a,b) \
     _ccw::_test::_test_limit<void>(x, a, b, #x, #a, #b, *_test_case(), __FILE__, __LINE__)

#define test_eq(l,r)      __TesT_cc(l,eq,"==",r)
#define test_ne(l,r)      __TesT_cc(l,ne,"!=",r)
#define test_lt(l,r)      __TesT_cc(l,lt,"<" ,r)
#define test_le(l,r)      __TesT_cc(l,le,"<=",r)
#define test_gt(l,r)      __TesT_cc(l,gt,">" ,r)
#define test_ge(l,r)      __TesT_cc(l,ge,">=",r)

#define __TesT_cc(a,mem,op,b) \
    _ccw::_test::__TesT_cc_lr_mk(a,b).mem(#a,op,#b, *_test_case(), __FILE__, __LINE__)

#define test_str_eq(a,b)  __TesT_str_cc(a,eq,"==",b)
#define test_str_ne(a,b)  __TesT_str_cc(a,ne,"!=",b)

#define __TesT_str_cc(a,mem,op,b)  \
    _ccw::_test::__TesT_cc_lr_mk(string_view(a), string_view(b))  \
        .mem(#a,op,#b, *_test_case(), __FILE__, __LINE__)

#if TEST_HAS_EH

#define test_throw(x)                                    \
    do {                                                 \
        if (_test_case()->ent()) {                       \
            bool __tst_eh_ok = false;                              \
            try {                                        \
                x;                                       \
            } catch (...) {                              \
                __tst_eh_ok = true;                                \
            }                                            \
            if (!__tst_eh_ok)                                      \
                _ccw::_test::_test_throw_msg<void>       \
                   (#x,*_test_case(),__FILE__,__LINE__); \
        }                                                \
    } while (0)

#define test_no_throw(x)                                 \
    do {                                                 \
        if (_test_case()->ent()) {                       \
            bool __tst_eh_ok = true;                               \
            try {                                        \
                x;                                       \
            } catch (...) {                              \
                __tst_eh_ok = false;                               \
            }                                            \
            if (!__tst_eh_ok)                                      \
                _ccw::_test::_test_no_throw_msg<void>    \
                   (#x,*_test_case(),__FILE__,__LINE__); \
        }                                                \
    } while (0)

#else   // !TEST_HAS_EH

// x must not run (it would abort); if(0) keeps the compile check.
#define test_throw(x)                                    \
    do {                                                 \
        if (0) { x; }                                    \
        TEST_SKIP1();                                    \
    } while (0)

// still meaningful: if x tries to throw, _CCW_THROW reports it and aborts.
#define test_no_throw(x)                                 \
    do {                                                 \
        if (_test_case()->ent()) { x; }                  \
    } while (0)

#endif  // TEST_HAS_EH

#define TEST_SKIP1()    _ccw::_test::TestMgr<>::instance().setExprSkip()

#define TEST_SKIP_N(n)  do { for (unsigned _tsN = 0; _tsN < (unsigned)(n); ++_tsN) TEST_SKIP1(); } while (0)

#if __cplusplus >= 201103L || _MSVC_LANG >= 201103L
#define TEST_SKIP_PRE11()
#else
#define TEST_SKIP_PRE11()   TEST_SKIP1()
#endif

#if __cplusplus >= 201402L  || _MSVC_LANG >= 201402L
#define TEST_SKIP_PRE14()
#else
#define TEST_SKIP_PRE14()   TEST_SKIP1()
#endif

#if __cplusplus >= 201703L || _MSVC_LANG >= 201703L
#define TEST_SKIP_PRE17()
#else
#define TEST_SKIP_PRE17()   TEST_SKIP1()
#endif

#if __cplusplus >= 202002L || _MSVC_LANG >= 202002L
#define TEST_SKIP_PRE20()
#else
#define TEST_SKIP_PRE20()   TEST_SKIP1()
#endif

#if __cplusplus >= 202302L || _MSVC_LANG >= 202302L
#define TEST_SKIP_PRE23()
#else
#define TEST_SKIP_PRE23()   TEST_SKIP1()
#endif

#if __cplusplus >= 202600L || _MSVC_LANG >= 202600L
#define TEST_SKIP_PRE26()
#else
#define TEST_SKIP_PRE26()   TEST_SKIP1()
#endif

#define test_pass(s)              _ccw::_test::TestMgr<>::instance().pass(s)
#define test_skip(s)              do { _ccw::_test::TestMgr<>::instance().setExprSkip(); \
                                       (void)_test_case()->ent(); \
                                       _ccw::_test::TestMgr<>::instance().pass(s); } while (0)
#define TEST_NOTE(s)              _ccw::_test::TestMgr<>::instance().note(s)
#define TEST_SET_PASS_LOG_DIR(d)  _ccw::_test::TestMgr<>::instance().setPassDir(d)

#define _TeST_LANG_C(v, x)    do { if ((TEST_TARGET_C) && (TEST_TARGET_C) < (v)) TEST_SKIP1(); x; } while (0)
#define TEST_C90(x)     do { x; } while (0)
#define TEST_C99(x)     _TeST_LANG_C(1999, x)
#define TEST_C11(x)     _TeST_LANG_C(2011, x)
#define TEST_C17(x)     _TeST_LANG_C(2017, x)
#define TEST_C23(x)     _TeST_LANG_C(2023, x)
#define _TeST_LANG_CXX(v, x) do { if ((TEST_TARGET_CXX) < (v)) TEST_SKIP1(); x; } while (0)
#define TEST_CXX03(x)   _TeST_LANG_CXX(2003, x)
#define TEST_CXX11(x)   _TeST_LANG_CXX(2011, x)
#define TEST_CXX14(x)   _TeST_LANG_CXX(2014, x)
#define TEST_CXX17(x)   _TeST_LANG_CXX(2017, x)
#define TEST_CXX20(x)   _TeST_LANG_CXX(2020, x)
#define TEST_CXX23(x)   _TeST_LANG_CXX(2023, x)
#define TEST_CXX26(x)   _TeST_LANG_CXX(2026, x)

#if defined(_MSC_VER)
# define TEST_SKIP_VC(...)  do { TEST_NOTE("" __VA_ARGS__); TEST_SKIP1(); } while (0)
# define _TeST_SKIP_VC_LT(floor, ...)  do { if (_MSC_VER < (floor)) { TEST_NOTE("" __VA_ARGS__); TEST_SKIP1(); } } while (0)
# define TEST_SKIP_VC_RANGE(lo, hi, ...)  do { if ((lo) <= _MSC_VER && _MSC_VER < (hi)) { TEST_NOTE("" __VA_ARGS__); TEST_SKIP1(); } } while (0)
#else
# define TEST_SKIP_VC(...)             ((void)sizeof("" __VA_ARGS__))
# define _TeST_SKIP_VC_LT(floor, ...)  ((void)sizeof("" __VA_ARGS__))
# define TEST_SKIP_VC_RANGE(lo, hi, ...)  ((void)sizeof("" __VA_ARGS__))
#endif
#define TEST_SKIP_VC080(...)   _TeST_SKIP_VC_LT(1500, __VA_ARGS__)
#define TEST_SKIP_VC090(...)   _TeST_SKIP_VC_LT(1600, __VA_ARGS__)
#define TEST_SKIP_VC100(...)   _TeST_SKIP_VC_LT(1700, __VA_ARGS__)
#define TEST_SKIP_VC110(...)   _TeST_SKIP_VC_LT(1800, __VA_ARGS__)
#define TEST_SKIP_VC120(...)   _TeST_SKIP_VC_LT(1900, __VA_ARGS__)
#define TEST_SKIP_VC140(...)   _TeST_SKIP_VC_LT(1910, __VA_ARGS__)
#define TEST_SKIP_VC141(...)   _TeST_SKIP_VC_LT(1920, __VA_ARGS__)
#define TEST_SKIP_VC142(...)   _TeST_SKIP_VC_LT(1930, __VA_ARGS__)
#define TEST_SKIP_VC143(...)   _TeST_SKIP_VC_LT(1950, __VA_ARGS__)
#define TEST_SKIP_VC145(...)   _TeST_SKIP_VC_LT(9999, __VA_ARGS__)

#if (defined(_MSC_VER) && _MSC_VER < 1600)  || (defined(__GNUC__) && !defined(_MSC_VER) && !defined(__WATCOMC__) && __cplusplus < 201103L)
# define TEST_SKIP_NATIVE_NO_MOVE(...) do { TEST_NOTE("" __VA_ARGS__); TEST_SKIP1(); } while (0)
#else
# define TEST_SKIP_NATIVE_NO_MOVE(...) ((void)sizeof("" __VA_ARGS__))
#endif

#if defined(_MSC_VER) || defined(_UCRT) || defined(__MINGW32__)
# define TEST_SKIP_UCRT(...)  do { TEST_NOTE("" __VA_ARGS__); TEST_SKIP1(); } while (0)
#else
# define TEST_SKIP_UCRT(...)  ((void)sizeof("" __VA_ARGS__))
#endif

#if defined(__GNUC__) || defined(__clang__)
# define TEST_SKIP_GCC(...)     do { TEST_NOTE("" __VA_ARGS__); TEST_SKIP1(); } while (0)
#else
# define TEST_SKIP_GCC(...)     ((void)sizeof("" __VA_ARGS__))
#endif

#if defined(__WATCOMC__)
# define TEST_SKIP_WAT(...)     do { TEST_NOTE("" __VA_ARGS__); TEST_SKIP1(); } while (0)
#else
# define TEST_SKIP_WAT(...)     ((void)sizeof("" __VA_ARGS__))
#endif

#if defined(__DOS__) || defined(MSDOS)
# define TEST_SKIP_DOS(...)     do { TEST_NOTE("" __VA_ARGS__); TEST_SKIP1(); } while (0)
#else
# define TEST_SKIP_DOS(...)     ((void)sizeof("" __VA_ARGS__))
#endif

namespace _ccw {

#if TEST_TARGET_CXX >= 2017
using STD::string_view;
#elif !defined(_CCW_DETAIL_STRING_VIEW_HPP)
typedef STD::string string_view;
#endif

namespace _test {

struct TestCase {
    TestCase() : group_(NULL), name_(NULL), func_(NULL), errors_(0), check_(0), skip_(0), case_skip_(false) {}
    TestCase(char const* g, char const* n, void (*f)())
        : group_(g), name_(n), func_(f), errors_(0), check_(0), skip_(0), case_skip_(false) {}
    template<class C> void  err(C const* fname, unsigned int ln);
    bool ent();
    void reset() { errors_ = check_ = skip_ = 0; case_skip_ = false; }

    char const* group_;
    char const* name_;
    void       (*func_)();
    unsigned   errors_;
    unsigned   check_;
    unsigned   skip_;
    bool       case_skip_;
};

template<class DMY=void>
class TestMgr {
public:
    typedef TestCase*                       TestCasePtr;
    typedef STD::vector< TestCasePtr >      TestCases;
    typedef STD::map<_TEST_STR, TestCases> TestGroups;

    TestMgr() : expr_skip_(0), case_skip_(false)
        , cur_group_(NULL), run_check_(0), run_err_(0), run_skip_(0)
        , seg_prev_check_(0), seg_prev_err_(0), seg_prev_skip_(0) {}
    ~TestMgr() {
        for (TestGroups::iterator it = groups_.begin(); it != groups_.end(); ++it) {
            TestCases& tc = it->second;
            for (STD::size_t i = 0; i < tc.size(); ++i)
                delete tc[i];
        }
    }
    static TestMgr& instance() { static TestMgr s_inst; return s_inst; }
    TestCase*       add(char const* group, char const* name, void (*fnc)());
    unsigned        run();
    static void     putf(char const* fmt, ...);
    void            setExprSkip() { ++expr_skip_; }
    bool            isExprSkip() { if (!expr_skip_) return false; --expr_skip_; return true; }
    unsigned        takeExprSkip() { unsigned n = expr_skip_; expr_skip_ = 0; return n; }
    void            setCaseSkip() { case_skip_ = true; }
    bool            takeCaseSkip() { bool b = case_skip_; case_skip_ = false; return b; }

    void  onCheck(bool skipped) { ++run_check_; if (skipped) ++run_skip_; }
    void  onErr()               { ++run_err_; }
    void  note(char const* n)   { if (n && n[0]) seg_note_ = n; }
    void  passSegStart(char const* group) {
        cur_group_ = group;
        seg_prev_check_ = run_check_; seg_prev_err_ = run_err_; seg_prev_skip_ = run_skip_;
        seg_note_.clear();
    }
#if defined(TST_HEAPCHK)
    void snapCaseGuards() {
        guards_.clear();
        for (TestGroups::iterator g = groups_.begin(); g != groups_.end(); ++g)
            for (STD::size_t i = 0; i < g->second.size(); ++i) {
                TestCase* c = g->second[i];
                Guard gd; gd.tc = c; gd.group = c->group_; gd.name = c->name_; gd.func = c->func_;
                guards_.push_back(gd);
            }
    }
    void checkCaseGuards(char const* after_group, char const* after_name) {
        for (STD::size_t i = 0; i < guards_.size(); ++i) {
            Guard const& gd = guards_[i];
            if (gd.tc->group_ != gd.group || gd.tc->name_ != gd.name || gd.tc->func_ != gd.func) {
                TEST_PRINTF("!! TESTCASE CLOBBERED (entry %u of %u) after %s.%s\n",
                            (unsigned)i, (unsigned)guards_.size(), after_group, after_name);
                guards_.clear();
                return;
            }
        }
    }
#endif
    void        setPassDir(char const* dir);
    char const* passDir() const { return pass_dir_.empty() ? (char const*)0 : pass_dir_.c_str(); }
    void        pass(char const* pass_str);
private:
    bool  passFirst(char const* suite);
    void  passWrite(char const* suite, char const* pass, char const* result, char const* note);

#if defined(TST_HEAPCHK)
    struct Guard { TestCase* tc; char const* group; char const* name; void (*func)(); };
    STD::vector<Guard> guards_;
#endif
    TestGroups  groups_;
    unsigned    expr_skip_;
    bool        case_skip_;

    _TEST_STR               pass_dir_;
    _TEST_STR               pass_path_;
    _TEST_STR               seg_note_;
    char const*             cur_group_;
    unsigned                run_check_, run_err_, run_skip_;
    unsigned                seg_prev_check_, seg_prev_err_, seg_prev_skip_;
    STD::vector<_TEST_STR>  pass_opened_;
};

template<class C>
void TestCase::err(C const* fname, unsigned int ln) {
    ++this->errors_;
    TestMgr<>::instance().onErr();
    TestMgr<>::putf("%s (%d) [%s.%s]: ", fname, ln, group_, name_);
}

inline bool TestCase::ent() {
    ++check_;
    if (TestMgr<>::instance().isExprSkip()) {
        ++skip_;
        TestMgr<>::instance().onCheck(true);
        return false;
    }
    TestMgr<>::instance().onCheck(false);
    return true;
}

template<class DMY> void
TestMgr<DMY>::putf(char const* fmt, ...) {
    char buf[TEST_PUTS_BUF_SIZE];
    STD::va_list ap;
    va_start(ap, fmt);
    STD::vsnprintf(buf, (sizeof buf) - 1, fmt, ap);
    va_end(ap);
    buf[(sizeof buf) - 1] = 0;
    TEST_PUTS(buf);
}

template<class DMY>
TestCase* TestMgr<DMY>::add(char const* group, char const* name, void (*fnc)()) {
    TestCases& cases = groups_[group];
    cases.push_back(TestCasePtr(new TestCase(group, name, fnc)));
    return &*cases.back();
}

template<class DMY>
void TestMgr<DMY>::setPassDir(char const* dir) {
    if (dir && dir[0])
        pass_dir_ = dir;
    else
        pass_dir_.clear();
    pass_opened_.clear();
}

template<class DMY>
bool TestMgr<DMY>::passFirst(char const* suite) {
    for (STD::size_t i = 0; i < pass_opened_.size(); ++i)
        if (pass_opened_[i] == suite)
            return false;
    pass_opened_.push_back(_TEST_STR(suite));
    return true;
}

template<class DMY>
void TestMgr<DMY>::passWrite(char const* suite, char const* pass,
                             char const* result, char const* note) {
    if (pass_dir_.empty() || !suite)
        return;
    pass_path_ = pass_dir_;
    pass_path_ += '/';
    pass_path_ += suite;
    pass_path_ += ".tsv";
    bool first = passFirst(suite);
    STD::FILE* fp = STD::fopen(pass_path_.c_str(), first ? "w" : "a");
    if (!fp)
        return;
    if (first)
        STD::fprintf(fp, "\"suite\"\t\"pass\"\t\"result\"\t\"note\"\n");
    STD::fprintf(fp, "\"%s\"\t\"%s\"\t\"%s\"\t\"%s\"\n",
                 suite, pass, result, note ? note : "");
    STD::fclose(fp);
}

template<class DMY>
void TestMgr<DMY>::pass(char const* pass_str) {
    unsigned pend = takeExprSkip();
    run_skip_  += pend;
    run_check_ += pend;

    if (pass_str && pass_str[0]) {
        unsigned seg_check = run_check_ - seg_prev_check_;
        unsigned seg_err   = run_err_   - seg_prev_err_;
        unsigned seg_skip  = run_skip_  - seg_prev_skip_;
        unsigned seg_ran   = (seg_check >= seg_skip) ? seg_check - seg_skip : 0;
        unsigned seg_ok    = (seg_ran  >= seg_err)   ? seg_ran  - seg_err   : 0;
        char const* result = seg_err ? "fail" : (seg_ok ? "pass" : "skip");
        passWrite(cur_group_, pass_str, result, seg_note_.c_str());
    }
    seg_prev_check_ = run_check_;
    seg_prev_err_   = run_err_;
    seg_prev_skip_  = run_skip_;
    seg_note_.clear();
}

template<class DMY>
unsigned TestMgr<DMY>::run() {
    TEST_PRINTF("[TEST]...\n");
#if defined(TST_HEAPCHK)
    snapCaseGuards();
#endif
    unsigned gok = 0;
    for (TestGroups::iterator gite = groups_.begin(); gite != groups_.end(); ++gite) {
        TestCases& cases = gite->second;
        unsigned idx = 0;
        unsigned sz  = unsigned(cases.size());
        unsigned oks = 0;
        unsigned errors = 0;
        unsigned cskips = 0;
        for (TestCases::iterator cite = cases.begin(); cite != cases.end(); ++cite, ++idx) {
            TestCase& tc = **cite;
            tc.reset();
            takeExprSkip();
            takeCaseSkip();
            passSegStart(tc.group_);
            _TeST_TRY {
                tc.func_();
            } _TeST_CATCH_ALL {
                ++tc.errors_;
            }
            tc.case_skip_ = takeCaseSkip();
#if defined(TST_HEAPCHK)
            {   int __hc = _heapchk();
                if (__hc != 0 )
                    TEST_PRINTF("!! HEAPCHK %d after %s.%s\n", __hc, tc.group_, tc.name_);
                checkCaseGuards(tc.group_, tc.name_);
            }
#endif
            unsigned pend = takeExprSkip();
            tc.skip_  += pend;
            tc.check_ += pend;
            bool rc = (tc.errors_ == 0);
            if (tc.case_skip_) {
                ++cskips;
                TEST_PRINTF("   %s  %s [%d/%d]\t%s\t(case skipped)\n"
                           , "skp", tc.group_, idx+1, sz, tc.name_);
                continue;
            }
            oks    += rc;
            errors += !rc;
            unsigned nok = (tc.check_ >= tc.errors_ + tc.skip_)
                         ? tc.check_ - tc.errors_ - tc.skip_ : 0;
            TEST_PRINTF("   %s  %s [%d/%d]\t%s\t(%d = ok%d + err%d + skip%d)\n"
                       , (rc) ? " ok" : "NG!", tc.group_, idx+1, sz
                       , tc.name_
                       , tc.check_, nok, tc.errors_, tc.skip_
                       );
        }
        if (cskips) {
            TEST_PRINTF("\t    %s: %d %s. %d %s. %d skipped.\n", gite->first.data()
                    , oks, (oks > 1) ? "successes":"success"
                    , errors, (errors>1) ? "failures":"failure"
                    , cskips );
        } else {
            TEST_PRINTF("\t    %s: %d %s. %d %s.\n", gite->first.data()
                    , oks, (oks > 1) ? "successes":"success"
                    , errors, (errors>1) ? "failures":"failure" );
        }
        gok += (errors == 0);
    }
    unsigned gsz  = unsigned(groups_.size());
    unsigned gerr = gsz - gok;
    TEST_PRINTF("[Result] Run %d %s, %d %s, %d %s.\n\n"
                , gsz , (gsz  > 1) ? "test-groups":"test-group"
                , gok , (gok  > 1) ? "successes":"success"
                , gerr, (gerr > 1) ? "failures" : "failure" );
    TestGroups().swap(groups_);
    return gerr;
}

inline bool _test_is_mem_addr(void const* p) {
    STD::size_t s = (STD::size_t)p;
    const STD::size_t mi = 0xff;
    const STD::size_t ma = ~STD::size_t(mi);
    return (mi < s && s < ma);
}

inline bool _test_is_aligned(void const* p, STD::size_t aln) {
    STD::size_t s = (STD::size_t)p;
    STD::size_t m = aln - 1;
    return ((s & m) == 0);
}
inline bool _test_is_aligned(void const* p) {
    return true;
}
template<typename T> bool _test_is_aligned(T const* p) {
#if _TeST_RAW_CPLUSPLUS >= 201103L || defined(_CCW_TARGET_CXX)
    return _test_is_aligned(p, alignof(T));
#elif defined(__GNUC__)
    return _test_is_aligned(p, __alignof__(T));
#elif defined(_MSC_VER)
    return _test_is_aligned(p, __alignof(T));
#else
    (void)p;
    return true;
#endif
}

template<class DMY> void
_test_true(bool cc, char const* ccStr, TestCase& tcase, char const* fname, unsigned int line) {
    if (!tcase.ent())
        return;
    if (!cc) {
        tcase.err(fname, line);
        TEST_PRINTF("'%s' is failed.\n", ccStr);
    }
}

template<class DMY> void
_test_ptr_impl(void const* ptr, bool zok, bool aok, char const* ptrStr
            , TestCase& tcase, char const* fname, unsigned int line)
{
    if (!tcase.ent())
        return;
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

template<class DMY, typename A, typename B> void
_test_cc_body(bool cc, A const& a, B const &b, char const* astr, char const* op, char const* bstr
               , TestCase& tcase, char const* fname, unsigned int line)
{
    if (!tcase.ent())
        return;
    if (!cc) {
        _TEST_STREAM_DEF(st);
        st << "'" << astr << ' ' << op << ' ' << bstr << "' is failed. (L='" << a << "' R='" << b << "')\n";
        tcase.err(fname, line);
        TEST_PRINTF("%s", _TEST_STREAM_CSTR(st));
    }
}

template<typename T, typename U>
struct __TesT_cc_lr {
    T const&  lhs_;
    U const&  rhs_;
    __TesT_cc_lr(T const& l, U const& r) : lhs_(l), rhs_(r) {}

#define _TeST_CC_MEM(mem, op)                                                    \
    void mem(char const* lstr, char const* opstr, char const* rstr               \
           , TestCase& tcase, char const* fname, unsigned int line) const        \
    {                                                                            \
        _test_cc_body<void>(lhs_ op rhs_, lhs_, rhs_                              \
                          , lstr, opstr, rstr, tcase, fname, line);              \
    }
    _TeST_CC_MEM(eq, ==)
    _TeST_CC_MEM(ne, !=)
    _TeST_CC_MEM(lt, < )
    _TeST_CC_MEM(le, <=)
    _TeST_CC_MEM(gt, > )
    _TeST_CC_MEM(ge, >=)
#undef _TeST_CC_MEM
};

template<typename T, typename U>
inline __TesT_cc_lr<T,U>  __TesT_cc_lr_mk(T const& l, U const& r) {
    return __TesT_cc_lr<T,U>(l, r);
}

template<class DMY, typename X, typename A, typename B> void
_test_limit(X const& x, A const& a, B const& b, char const* xstr, char const* astr, char const* bstr
            , TestCase& tcase, char const* fname, unsigned int line)
{
    if (!tcase.ent())
        return;
    if (!(a <= x && x <= b)) {
        _TEST_STREAM_DEF(st);
        st << "'" << xstr << "'(" << x << ") is out of range[" << a << ", " << b << "]. (["
            << astr << ", " << bstr << "])\n";
        tcase.err(fname, line);
        TEST_PRINTF("%s", _TEST_STREAM_CSTR(st));
    }
}

template<class DMY, typename X, typename A, typename B> void
_test_limit(X const* x, A const* a, B const* b, char const* xstr, char const* astr, char const* bstr
            , TestCase& tcase, char const* fname, unsigned int line)
{
    if (!tcase.ent())
        return;
    if (!((char const*)a <= (char const*)x && (char const*)x <= (char const*)b)) {
        tcase.err(fname, line);
        TEST_PRINTF("'%s'(%p) is out of range[%p, %p].([%s,%s])\n", xstr, x, a, b, astr, bstr);
    }
}

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

}

}

#endif  // USE_TEST

#endif  //  TEST_HPP_INCLUDED__
