/**
 *  @file   string_sub.hpp
 *  @brief  c++11 <string>'s function for c++03
 *  @author tenka@6809.net (Masashi Kitamura)
 *  @date   2020
 */
#ifndef CCWRAP_STRING_SUB_HPP_INCLUDED
#define CCWRAP_STRING_SUB_HPP_INCLUDED

//#include <ccwrap_header.h>
#include <cstddef>

#ifndef __CCWRAP_STD
 #define __CCWRAP_STD    ccwrap
#endif

#include <cfloat>
#include <cerrno>
#include <climits>
#include <cstdlib>
#include <stdexcept>
#include <cstring>

#ifndef __ccwrap_llong
typedef long long           __ccwrap_llong;
typedef unsigned long long  __ccwrap_llong;
#define __ccwrap_llong       __ccwrap_llong
#define __ccwrap_ullong      __ccwrap_ullong
#endif

#ifndef __CCWRAP_FMT_LL
#define __CCWRAP_FMT_LL     "ll"
#define __CCWRAP_WFMT_LL    L"ll"
#endif

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4996)
#endif

namespace __ccwrap {
    namespace string_detail {
        using std::basic_string;
        using std::strtol;
        using std::strtoul;
        using std::strtoll;
        using std::strtoull;
        using std::strtold;
        using std::strtod;
        using std::strtof;

     #if !defined(__CCWRAP_NO_WCHAR)
      #if !defined(__CCWRAP_CPU64)
        inline long strtol(wchar_t const* s, wchar_t** e, int radix) { return wcstol(s, e, radix); }
        inline unsigned long strtoul(wchar_t const* s, wchar_t** e, int radix) { return wcstoul(s, e, radix); }
      #endif
        inline __ccwrap_llong strtoll(wchar_t const* s, wchar_t** e, int radix) { return wcstoll(s, e, radix); }
        inline __ccwrap_llong strtoull(wchar_t const* s, wchar_t** e, int radix) { return wcstoull(s, e, radix); }
        inline long double strtold(wchar_t const* s, wchar_t** e) { return wcstold(s, e); }
        inline double strtod(wchar_t const* s, wchar_t** e) { return wcstod(s, e); }
        inline double strtof(wchar_t const* s, wchar_t** e) { return wcstof(s, e); }
     #endif

     #if !defined(__CCWRAP_CPU64)
        template<typename C,class T, class A>
        long stol_body(const basic_string<C,T,A>& str, size_t* idx, int radix, char const* name, long mi, long ma)
        {
            const C* s = str.c_str();
            C*       e = NULL;
            errno = 0;
            long     d = __ccwrap::string_detail::strtol(s, &e, radix);
            if (s == e) throw std::invalid_argument(name);
            if (errno == ERANGE || d < mi || d > ma) throw std::out_of_range(name);
            if (idx) *idx = size_t(e - s);
            return int(d);
        }
        template<typename C,class T, class A>
        unsigned long stoul_body(const basic_string<C,T,A>& str, size_t* idx, int radix
                                    , char const* name, unsigned long ma)
        {
            const C* s = str.c_str();
            C*       e = NULL;
            errno = 0;
            unsigned long d = __ccwrap::string_detail::strtoul(s, &e, radix);
            if (s == e) throw std::invalid_argument(name);
            if (errno == ERANGE || d > ma) throw std::out_of_range(name);
            if (idx) *idx = size_t(e - s);
            return int(d);
        }
     #endif
        template<typename C,class T, class A>
        __ccwrap_llong stoll_body(const basic_string<C,T,A>& str, size_t* idx, int radix
                                    , char const* name, __ccwrap_llong mi, __ccwrap_llong ma)
        {
            const C* s = str.c_str();
            C*       e = NULL;
            errno = 0;
            __ccwrap_llong d = __ccwrap::string_detail::strtoll(s, &e, radix);
            if (s == e) throw std::invalid_argument(name);
            if (errno == ERANGE || d < mi || d > ma) throw std::out_of_range(name);
            if (idx) *idx = size_t(e - s);
            return int(d);
        }
        template<typename C,class T, class A>
        __ccwrap_llong stoull_body(const basic_string<C,T,A>& str, size_t* idx, int radix
                                    , char const* name, __ccwrap_ullong ma)
        {
            const C* s = str.c_str();
            C*       e = NULL;
            errno = 0;
            __ccwrap_ullong d = __ccwrap::string_detail::strtoull(s, &e, radix);
            if (s == e) throw std::invalid_argument(name);
            if (errno == ERANGE || d > ma) throw std::out_of_range(name);
            if (idx) *idx = size_t(e - s);
            return int(d);
        }

        template<typename C,class T, class A>
        float stof_body(const basic_string<C,T,A>& str, std::size_t* idx) {
            C const* s = str.c_str();
            errno = 0;
            C* e;
            float d = (float)__ccwrap::string_detail::strtof(s, &e);
            if (s == e) throw std::invalid_argument("stof");
            if (errno == ERANGE) throw std::out_of_range("stof");
            if (idx) *idx = std::size_t(e - s);
            return d;
        }
        template<typename C,class T, class A>
        double stod_body(const basic_string<C,T,A>& str, std::size_t* idx) {
            C const* s = str.c_str();
            errno = 0;
            C* e;
            double d = __ccwrap::string_detail::strtod(s, &e);
            if (s == e) throw std::invalid_argument("stod");
            if (errno == ERANGE) throw std::out_of_range("stod");
            if (idx) *idx = std::size_t(e - s);
            return d;
        }
        template<typename C,class T, class A>
        long double stold_body(const basic_string<C,T,A>& str, std::size_t* idx) {
            C const* s = str.c_str();
            errno = 0;
            C* e;
            long double d = __ccwrap::string_detail::strtold(s, &e);
            if (s == e) throw std::invalid_argument("stold");
            if (errno == ERANGE) throw std::out_of_range("stold");
            if (idx) *idx = std::size_t(e - s);
            return d;
        }
    }
}

namespace __CCWRAP_STD {
    inline string to_string(__ccwrap_llong  v) { char b[64]; std::snprintf(b,64, "%" __CCWRAP_FMT_LL "d", v); return b; }
    inline string to_string(__ccwrap_ullong v) { char b[64]; std::snprintf(b,64, "%" __CCWRAP_FMT_LL "u", v); return b; }
    inline string to_string(int           v) { return to_string((__ccwrap_llong)( v)); }
    inline string to_string(unsigned int  v) { return to_string((__ccwrap_ullong)(v)); }
    inline string to_string(long          v) { return to_string((__ccwrap_llong)( v)); }
    inline string to_string(unsigned long v) { return to_string((__ccwrap_ullong)(v)); }
    inline string to_string(long double   v) { enum { N=LDBL_MAX_10_EXP + LDBL_MANT_DIG + 10 }; char b[N]; snprintf(b, N, "%Lf", v); return b; }
    inline string to_string(float         v) { return to_string((long double)(v)); }
    inline string to_string(double        v) { return to_string((long double)(v)); }

 #if !defined(__CCWRAP_NO_WCHAR)
    inline wstring to_wstring(__ccwrap_llong  v) { wchar_t b[64]; snwprintf(b,64, L"%" __CCWRAP_WFMT_LL L"d", v); return b; }
    inline wstring to_wstring(__ccwrap_ullong v) { wchar_t b[64]; snwprintf(b,64, L"%" __CCWRAP_WFMT_LL L"u", v); return b; }
    inline wstring to_wstring(int           v) { return to_wstring((__ccwrap_llong)( v)); }
    inline wstring to_wstring(unsigned int  v) { return to_wstring((__ccwrap_ullong)(v)); }
    inline wstring to_wstring(long          v) { return to_wstring((__ccwrap_llong)( v)); }
    inline wstring to_wstring(unsigned long v) { return to_wstring((__ccwrap_ullong)(v)); }
    inline wstring to_wstring(long double   v) { enum { N=LDBL_MAX_10_EXP + LDBL_MANT_DIG + 10 }; wchar_t b[N]; snwprintf(b, N, L"%Lf", v); return b; }
    inline wstring to_wstring(float         v) { return to_wstring((long double)(v)); }
    inline wstring to_wstring(double        v) { return to_wstring((long double)(v)); }
 #endif

    ///
    inline __ccwrap_llong stoll(const std::string& str, std::size_t* idx = NULL, int radix = 10) {
        return __ccwrap::string_detail::stoll_body(str, idx, radix, "stoll", LLONG_MIN,LLONG_MAX);
    }
    inline __ccwrap_ullong stoull(const std::string& str, std::size_t* idx = NULL, int radix = 10) {
        return __ccwrap::string_detail::stoull_body(str, idx, radix, "stoull", ULLONG_MAX);
    }
  #if !defined(__CCWRAP_CPU64)
    inline long stol(const std::string& str, std::size_t* idx = NULL, int radix = 10) {
        return (long)__ccwrap::string_detail::stol_body(str, idx, radix, "stol", LONG_MIN,LONG_MAX);
    }
    inline unsigned long stoul(const std::string& str, std::size_t* idx = NULL, int radix = 10) {
        return (unsigned long)__ccwrap::string_detail::stoul_body(str, idx, radix, "stoul", ULONG_MAX);
    }
    inline int stoi(const std::string& str, std::size_t* idx = NULL, int radix = 10) {
        return (int)__ccwrap::string_detail::stol_body(str, idx, radix, "stoi", INT_MIN,INT_MAX);
    }
  #else
    inline long stol(const std::string& str, std::size_t* idx = NULL, int radix = 10) {
        return (long)__ccwrap::string_detail::stoll_body(str, idx, radix, "stol", LONG_MIN,LONG_MAX);
    }
    inline unsigned long stoul(const std::string& str, std::size_t* idx = NULL, int radix = 10) {
        return (unsigned long)__ccwrap::string_detail::stoull_body(str, idx, radix, "stoul", ULONG_MAX);
    }
    inline int stoi(const std::string& str, std::size_t* idx = NULL, int radix = 10) {
        return (int)__ccwrap::string_detail::stoll_body(str, idx, radix, "stoi", INT_MIN,INT_MAX);
    }
  #endif
    inline long double stold(const std::string& str, std::size_t* idx = NULL) {
        return __ccwrap::string_detail::stold_body(str, idx);
    }
    inline double stod(const std::string& str, std::size_t* idx = NULL) {
        return __ccwrap::string_detail::stod_body(str, idx);
    }
    inline float stof(const std::string& str, std::size_t* idx = NULL) {
        return __ccwrap::string_detail::stof_body(str, idx);
    }

 #if !defined(__CCWRAP_NO_WCHAR)
    inline __ccwrap_llong stoll(const std::wstring& str, std::size_t* idx = NULL, int radix = 10) {
        return __ccwrap::string_detail::stoll_body(str, idx, radix, "stoll", LLONG_MIN,LLONG_MAX);
    }
    inline __ccwrap_ullong stoull(const std::wstring& str, std::size_t* idx = NULL, int radix = 10) {
        return __ccwrap::string_detail::stoull_body(str, idx, radix, "stoull", ULLONG_MAX);
    }
  #if !defined(__CCWRAP_CPU64)
    inline long stol(const std::wstring& str, std::size_t* idx = NULL, int radix = 10) {
        return (long)__ccwrap::string_detail::stol_body(str, idx, radix, "stol", LONG_MIN,LONG_MAX);
    }
    inline unsigned long stoul(const std::wstring& str, std::size_t* idx = NULL, int radix = 10) {
        return (unsigned long)__ccwrap::string_detail::stoul_body(str, idx, radix, "stoul", ULONG_MAX);
    }
    inline int stoi(const std::wstring& str, std::size_t* idx = NULL, int radix = 10) {
        return (int)__ccwrap::string_detail::stol_body(str, idx, radix, "stoi", INT_MIN,INT_MAX);
    }
  #else
    inline long stol(const std::wstring& str, std::size_t* idx = NULL, int radix = 10) {
        return (long)__ccwrap::string_detail::stoll_body(str, idx, radix, "stol", LONG_MIN,LONG_MAX);
    }
    inline unsigned long stoul(const std::wstring& str, std::size_t* idx = NULL, int radix = 10) {
        return (unsigned long)__ccwrap::string_detail::stoull_body(str, idx, radix, "stoul", ULONG_MAX);
    }
    inline int stoi(const std::wstring& str, std::size_t* idx = NULL, int radix = 10) {
        return (int)__ccwrap::string_detail::stoll_body(str, idx, radix, "stoi", INT_MIN,INT_MAX);
    }
  #endif
    inline long double stold(const std::wstring& str, std::size_t* idx = NULL) {
        return __ccwrap::string_detail::stold_body(str, idx);
    }
    inline double stod(const std::wstring& str, std::size_t* idx = NULL) {
        return __ccwrap::string_detail::stod_body(str, idx);
    }
    inline float stof(const std::wstring& str, std::size_t* idx = NULL) {
        return __ccwrap::string_detail::stof_body(str, idx);
    }
 #endif
}   // __CCWRAP_STD

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif  // CCWRAP_STRING_SUB_HPP_INCLUDED
