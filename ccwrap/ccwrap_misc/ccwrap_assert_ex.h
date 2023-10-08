/**
 *  @file   ccwrap_assert_ex.h
 *  @brief  ex assertion macro for ccwrap-library.
 *  @author Masashi Kitamura (tenka@6809.net)
 *  @date   2000-2011,2020
 *  @license Boost Software License Version 1.0
 *  @note
 *      CCWRAP_ASSERT_NO_ABORT    no exit()/_CrtDbgBreak()
 *      _CONSOLE                 In Windows c++, standard error output if defined, debug log output if undefined.
 *
 *      This header can be used without the need for any other ccwrap source.
 */
#ifndef __CCWRAP_ASSERT_EX_H_INCLUDED
#define __CCWRAP_ASSERT_EX_H_INCLUDED

#ifdef __cplusplus
#include <cstddef>
#else
#include <stddef.h>
#endif


#ifdef NDEBUG           // ----------------------------------------------------

#ifndef CCWRAP_ASSERT
#define CCWRAP_ASSERT(x)
#endif

#define CCWRAP_PTR_ASSERT(p)
#define CCWRAP_PTR0_ASSERT(p)
#define CCWRAP_RANGE_ASSERT(x,a,b)

#define CCWRAP_ARG_ASSERT(n,x)
#define CCWRAP_ARG_PTR_ASSERT(n,x)
#define CCWRAP_ARG_PTR0_ASSERT(n,x)
#define CCWRAP_ARG_RANGE_ASSERT(n,x,a,b)

#ifndef __CCWRAP_ABORT
#define __CCWRAP_ABORT()
#endif
#ifndef __CCWRAP_ABORT_PRINTF
#define __CCWRAP_ABORT_PRINTF(...)
#endif

#else   // NDEBUG   // --------------------------------------------------------

#ifndef __CCWRAP_IS_MEM_ADDR
 #ifdef __cplusplus
  #define __CCWRAP_INT_TO_CPTR(x)    ((char const*)std::size_t(x))
 #else
  #define __CCWRAP_INT_TO_CPTR(x)    ((char const*)(size_t)(x))
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

#ifndef __CCWRAP_ABORT
 #if defined(_WIN32) && defined(_MSC_VER)
  #define __CCWRAP_ABORT()       _CrtDbgBreak()
 #elif 1
  #define __CCWRAP_ABORT()       exit(1)
 #elif 1
  #define __CCWRAP_ABORT()       ((*(char*)0) = 0)
 #endif
#endif
#if defined(CCWRAP_ASSERT_NO_ABORT)
 #undef __CCWRAP_ABORT
 #define __CCWRAP_ABORT()        (void)(0)
#endif

#ifndef __CCWRAP_M_FUNC
 #if defined(_MSC_VER) && _MSC_VER < 1900
  #define __CCWRAP_M_FUNC        __FUNCTION__
 #else
  #define __CCWRAP_M_FUNC        __func__
 #endif
#endif

#ifdef __cplusplus  // --------------------------------------------------------

#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>

#if defined(_WIN32) && !defined(__CCWRAP_ABORT_PRINTF)
#include <windows.h>
#endif

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

#ifndef __CCWRAP_ABORT_PRINTF
 namespace __ccwrap { namespace __assert_ex {
    template< class DMY >
    int abortPrintf(char const* fmt, ...) {
        using namespace std;
        va_list ap;
        va_start(ap, fmt);
     #if defined(_WIN32) && !defined(_CONSOLE)
        __CCWRAP_DEBUG_LOG_VPRINTF(fmt, ap);
     #else
        vfprintf(stderr, fmt, ap);
     #endif
        va_end(ap);
        __CCWRAP_ABORT();
        return 0;
    }
 }} // __ccwrap::__assert_ex
 #define __CCWRAP_ABORT_PRINTF(...)     (__ccwrap::__assert_ex::abortPrintf<void>(__VA_ARGS__))
#endif

namespace __ccwrap { namespace __assert_ex {

typedef char const* charp;

template<typename T=int>
struct ArgNum {
    ArgNum(T n);
    char str[64];
};
template<typename T>
ArgNum<T>::ArgNum(T n) {
    str[0] = 0;
    if (n > 0) {
        using namespace std;
        //snprintf(str, sizeof str, "(arg %u)", n);
        sprintf(str, "(arg %u)", n);    // %d ... max 10 characters
    }
}

#if 0
template<typename T> struct IfCharToInt { typedef T type; };
template<> struct IfCharToInt<char> { typedef int type; };
template<> struct IfCharToInt<signed char> { typedef int type; };
template<> struct IfCharToInt<unsigned char> { typedef unsigned int type; };
#endif

template<class DMY>
bool __bool_assert(bool b, charp bstr, charp fname, int line, charp fnc, int n) {
    if (!b) {
        __CCWRAP_ABORT_PRINTF("%s (%d): %s%s: assert(%s) is failed.\n", fname, line, fnc, ArgNum<>(n).str, bstr);
    }
    return b;
}

template<class DMY>
bool __ptr_assert_i(void const* p, charp pstr, charp fname, int line, charp fnc, int n) {
    bool rc = __CCWRAP_IS_MEM_ADDR(p);
    if (!rc)
        __CCWRAP_ABORT_PRINTF("%s (%d): %s%s: `%s'(0x%p) is bad pointer.\n"
            , fname,line,fnc, ArgNum<>(n).str, pstr, p);
    return rc;
}
template<class DMY, typename T>
bool __ptr_assert(T const& p, charp pstr, charp fname, int line, charp fnc, int n) {
    return __ptr_assert_i<DMY>((void const*)p, pstr, fname, line, fnc, n);
}

template<class DMY>
bool __ptr0_assert_i(void const* p, charp pstr, charp fname, int line, charp fnc, int n) {
    bool rc = __CCWRAP_IS_MEM_ADDR0(p);
    if (!rc)
        __CCWRAP_ABORT_PRINTF("%s (%d): %s%s: `%s'(0x%p) is bad pointer.\n"
            , fname,line,fnc, ArgNum<>(n).str, pstr, p);
    return rc;
}

template<class DMY, typename T>
bool __ptr0_assert(T const& p, charp pstr, charp fname, int line, charp fnc, int n) {
    return __ptr0_assert_i<DMY>((void const*)p, pstr, fname, line, fnc, n);
}

template<class DMY, typename X, typename A, typename B>
bool __range_assert(X const& x, A const& a, B const& b, charp xstr, charp astr, charp bstr
                            , charp fname, int line, charp fnc, int n)
{
    bool rc = (a <= x && x <= b);
    if (!rc) {
        std::stringstream ss;
        ss << "`" << xstr << "'("
            << x    // typename IfCharToInt<X>::type(x)
            << ") is out of range["
            << a    // typename IfCharToInt<A>::type(a)
            << ", "
            << b    // typename IfCharToInt<B>::type(b)
            << "]. ([" << astr << ", " << bstr << "])";
        __CCWRAP_ABORT_PRINTF("%s (%d): %s%s: %s\n", fname, line, fnc, ArgNum<>(n).str, ss.str().c_str());
    }
    return rc;
}

#if 0
template<class DMY>
bool __range_assert(void const* x, void const* a, void const* b, charp xstr, charp astr, charp bstr
                            , charp fname, int line, charp fnc, int n)
{
    bool rc = ((char const*)a <= (char const*)x && (char const*)x <= (char const*)b);
    if (!rc) {
        __CCWRAP_ABORT_PRINTF("%s (%d): %s%s: `%s'(0x%p) is out of range[0x%p, 0x%p].([%s,%s])\n"
            , fname, line, fnc, ArgNum<>(n).str, xstr, x, a, b, astr, bstr);
    }
    return rc;
}
template<class DMY, typename X, typename A, typename B>
bool __range_assert(X const* x, A const* a, B const* b, charp xstr, charp astr, charp bstr
                            , charp fname, int line, charp fnc, int n)
{
    return __range_assert<DMY>((void const*)x, (void const*)a, (void const*)b, xstr, astr, bstr, fname, line , fnc, n);
}
#endif

}}  // __ccwrap::__assert_ex

#undef CCWRAP_ASSERT
#define CCWRAP_ASSERT(b) \
    __ccwrap::__assert_ex::__bool_assert<void>((b), #b, __FILE__,__LINE__,__CCWRAP_M_FUNC,0)
#define CCWRAP_PTR_ASSERT(p) \
    __ccwrap::__assert_ex::__ptr_assert<void>( (p), #p, __FILE__,__LINE__,__CCWRAP_M_FUNC,0)
#define CCWRAP_PTR0_ASSERT(p)    \
    __ccwrap::__assert_ex::__ptr0_assert<void>((p), #p, __FILE__,__LINE__,__CCWRAP_M_FUNC,0)
#define CCWRAP_RANGE_ASSERT(t,mi,ma)   \
    __ccwrap::__assert_ex::__range_assert<void>((t),(mi),(ma), #t, #mi, #ma, __FILE__,__LINE__,__CCWRAP_M_FUNC,0)

#define CCWRAP_ARG_ASSERT(n,b)   \
    __ccwrap::__assert_ex::__bool_assert<void>((b), #b,__FILE__,__LINE__,__CCWRAP_M_FUNC,(n))
#define CCWRAP_ARG_PTR_ASSERT(n,p)   \
    __ccwrap::__assert_ex::__ptr_assert<void>( (p), #p,__FILE__,__LINE__,__CCWRAP_M_FUNC,(n))
#define CCWRAP_ARG_PTR0_ASSERT(n,p)  \
    __ccwrap::__assert_ex::__ptr0_assert<void>((p), #p,__FILE__,__LINE__,__CCWRAP_M_FUNC,(n))
#define CCWRAP_ARG_RANGE_ASSERT(n,t,mi,ma) \
    __ccwrap::__assert_ex::__range_assert<void>((t),(mi),(ma), #t, #mi, #ma, __FILE__,__LINE__,__CCWRAP_M_FUNC,(n))


#else   // for c language  // -------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

#if defined(__CCWRAP_DEBUG_LOG_PRINTF)
#define __CCWRAP_DEBUG_LOG_VPRINTF(fmt,a)    vfprintf(stderr, fmt, a)
#define __CCWRAP_DEBUG_LOG_PRINTF(...)       fprintf(stderr, __VA_ARGS__)
#endif

#ifndef __CCWRAP_ABORT_PRINTF
 static inline int __ccwrap_abort() { __CCWRAP_ABORT(); return 0; }
 #define __CCWRAP_ABORT_PRINTF(...)          (fprintf(stderr, __VA_ARGS__), __ccwrap_abort())
#endif

#ifndef CCWRAP_ASSERT
#define CCWRAP_ASSERT(x)                                                     \
        ((x) || __CCWRAP_ABORT_PRINTF("%s (%d): %s: assert(%s) is failed.\n" \
                ,__FILE__,__LINE__,__CCWRAP_M_FUNC,#x) )
#endif

#define CCWRAP_PTR_ASSERT(p)                                                 \
        (__CCWRAP_IS_MEM_ADDR(p)                                             \
         || __CCWRAP_ABORT_PRINTF("%s (%d): %s: %s(0x%p) is bad pointer.\n"  \
                , __FILE__,__LINE__,__CCWRAP_M_FUNC,#p, (p) ) )

#define CCWRAP_PTR0_ASSERT(p)                                                \
        (!p || __CCWRAP_IS_MEM_ADDR(p)                                       \
         || __CCWRAP_ABORT_PRINTF("%s (%d): %s: %s(0x%p) is bad pointer.\n"  \
                ,__FILE__,__LINE__,__CCWRAP_M_FUNC,#p, (p) ) )

#define CCWRAP_RANGE_ASSERT(x,a,b)                                           \
        ( ((a) <= (x) && (x) <= (b))                                         \
         || __CCWRAP_ABORT_PRINTF("%s (%d): %s: %s, out of range[%s, %s].\n" \
                ,__FILE__,__LINE__,__CCWRAP_M_FUNC,#x, #a, #b) )

#define CCWRAP_ARG_ASSERT(n,x)                                               \
        ((x) || __CCWRAP_ABORT_PRINTF(                                       \
                "%s (%d): %s(arg %d): assert(%s) is failed.\n"               \
                ,__FILE__,__LINE__,__CCWRAP_M_FUNC,(n),#x ) )

#define CCWRAP_ARG_PTR_ASSERT(n,p)                                           \
        ( __CCWRAP_IS_MEM_ADDR(p)                                            \
            || __CCWRAP_ABORT_PRINTF(                                        \
                "%s (%d): %s(arg %d): %s(0x%p) is bad pointer.\n"            \
                ,__FILE__,__LINE__,__CCWRAP_M_FUNC,(n), #p, (p) ) )

#define CCWRAP_ARG_PTR0_ASSERT(n,p)                                          \
        (!(p) || __CCWRAP_IS_MEM_ADDR(p)                                     \
         || __CCWRAP_ABORT_PRINTF(                                           \
                "%s (%d): %s(arg %d): %s(0x%p) is bad pointer.\n"            \
                ,__FILE__,__LINE__,__CCWRAP_M_FUNC,(n), #p, (p) ) )

#define CCWRAP_ARG_RANGE_ASSERT(n,x,a,b)                                     \
        ( ((a) <= (x) && (x) <= (b))                                         \
         || __CCWRAP_ABORT_PRINTF(                                           \
                "%s (%d): %s(arg %d): %s, out of range[%s, %s].\n"           \
                ,__FILE__,__LINE__,__CCWRAP_M_FUNC,(n), #x, #a, #b ) )

#endif  // __cplusplus

#endif  // NDEBUG

#endif  // __CCWRAP_ASSERT_EX_H_INCLUDED
