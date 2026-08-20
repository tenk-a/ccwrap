/**
 * @file strbuff.hpp
 * @brief Simple ostream-like string conversion using snprintf. (c++03)
 * @author Masashi Kitamura (tenka@6809.net)
 * @date 2020
 */
#ifndef STRBUFF_HPP_INCLUDE__
#define STRBUFF_HPP_INCLUDE__

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cstdarg>
#include <string>

#if defined(__DOS__) && !defined(STRBUFF_NO_UNICODE)
#define STRBUFF_NO_UNICODE
#endif

#if !defined(STRBUFF_NO_UNICODE)
#if defined __has_include || defined(STRBUFF_USE_UTFENC)
 #if __has_include("utfenc.hpp") || defined(STRBUFF_USE_UTFENC)
  #include "utfenc.hpp"
 #endif
  namespace ccw {
    template<class C> inline
    std::size_t str_from_unienc(char buf[], size_t buf_sz, C const* src, size_t src_sz) {
        return utfenc::convert_chars(buf, buf+buf_sz, src, src+src_sz, NULL, 0 );
    }
  }
#else
 #if defined(_WIN32)
  #ifndef _CCW_WINAPI
   #define _CCW_WINAPI(ty)      __declspec(dllimport) ty __stdcall
  #endif
  extern "C" _CCW_WINAPI(int)
  WideCharToMultiByte(unsigned int, unsigned long, const wchar_t*, int, char*, int, const char*, int*);
 #endif
  namespace ccw {
    inline std::size_t str_from_unienc(char buf[], size_t buf_sz, char const* src, size_t src_sz) {
        std::size_t l = src_sz < buf_sz ? src_sz : buf_sz - 1;
        memcpy(buf, src, l);
        buf[l] = 0;
        return l;
    }
    inline std::size_t str_from_unienc(char buf[], size_t buf_sz, wchar_t const* src, size_t src_sz) {
     #if defined(_WIN32)
        return WideCharToMultiByte(0, 0, src, (int)src_sz, buf, (int)buf_sz, 0, 0);
     #else
        std::size_t l = std::wcstombs(buf, src, buf_sz);
        if (l == (std::size_t)-1) l = 0;
        if (buf_sz) buf[buf_sz - 1] = 0;
        (void)src_sz;
        return l;
     #endif
    }
 #if (__cplusplus >= 201103L || _MSVC_LANG >= 201103L || _MSC_VER >= 1900)
  #if defined(_WIN32)
    inline std::size_t str_from_unienc(char buf[], size_t buf_sz, char16_t const* src, size_t src_sz)
  #else
    inline std::size_t str_from_unienc(char buf[], size_t buf_sz, char32_t const* src, size_t src_sz)
  #endif
    {
        return str_from_unienc(buf, buf_sz, (wchar_t const*)src, src_sz);
    }
  }
 #endif
#endif
#endif  // STRBUFF_NO_UNICODE


namespace ccw {

struct strbuff_ref {
    strbuff_ref(char* buf, std::size_t l)
        : cur_(buf), bgn_(buf), end_(buf+l-1)
        { assert(buf && l > 2); buf[0] = 0; }

    template<unsigned N>
    strbuff_ref(char (&buf)[N])
        : cur_(buf), bgn_(&buf[0]), end_(&buf[N-1])
        { assert(buf && N > 2); buf[0] = 0; }

    operator char*() { return bgn_; }
    operator char const*() const { return bgn_; }

    char const* c_str() const { return bgn_; }
    char const* data()  const { return bgn_; }
    std::size_t size()  const { return cur_ - bgn_; }
    void        clear() { cur_ = bgn_; *cur_ = 0; }

    template<typename T> strbuff_ref& operator<<(T const& t) { return put(t); }

    template<typename C>
    strbuff_ref& fmt(C const* fmt, ...);

    strbuff_ref& put(bool b) { if (cur_ < end_) { *cur_++ = '0'+int(b); *cur_ = 0; } return *this; }
    strbuff_ref& put(char c) { if (cur_ < end_) { *cur_++ = c; *cur_ = 0; } return *this; }
    strbuff_ref& put(char const* s) { return put_sl(s, std::strlen(s)); }
    strbuff_ref& put(char const* s, std::size_t l) { return put_sl(s, l); }
    strbuff_ref& put(void const* addr) { return fmt("%p", addr); }
    strbuff_ref& put(long double f) { return fmt("%lg", f); }
    strbuff_ref& put(double f) { return fmt("%g", f); }
    strbuff_ref& put(float f) { return put((long double)f); }
    strbuff_ref& put(long long n) { return fmt("%lld", n); }
    strbuff_ref& put(unsigned long long n) { return fmt("%llu", n); }
    strbuff_ref& put(long n) { return fmt("%ld", n); }
    strbuff_ref& put(unsigned long n) { return fmt("%lu", n); }
    strbuff_ref& put(int n) { return put(long(n)); }
    strbuff_ref& put(unsigned int n) { return put((unsigned long)n); }
    strbuff_ref& put(short n) { return put(long(n)); }
    strbuff_ref& put(unsigned short n) { return put((unsigned long)n); }
    strbuff_ref& put(signed char n) { return put(long(n)); }
    strbuff_ref& put(unsigned char n) { return put((unsigned long)n); }

 #if defined(__WATCOMC__)
    template<class _Tr, class _Al>
    strbuff_ref& put(std::basic_string<char, _Tr, _Al> const& str) {
        return put_sl(str.data(), str.size());
    }
 #else
    template<class STR>
    strbuff_ref& put(STR const& str, typename STR::value_type const* = STR().data() + STR().size()) {
        return put_sl(str.data(), str.size());
    }
 #endif

 #if !defined(STRBUFF_NO_UNICODE)
    strbuff_ref& put(wchar_t c) { return put_utf_ch(c); }
    strbuff_ref& put(wchar_t const* s, std::size_t l) { return put_utf(s, l); }
    strbuff_ref& put(wchar_t const* s) { return put(s, std::char_traits<wchar_t>::length(s)); }
  #if __cplusplus >= 202002L || _MSVC_LANG >= 202002L
    strbuff_ref& put(char8_t c) { return put(char(c)); }
    strbuff_ref& put(char8_t const* s, std::size_t l) { return put((char const*)s, l); }
    strbuff_ref& put(char8_t const* s) { return put((char const*)s); }
  #endif
  #if __cplusplus >= 201103L || _MSVC_LANG >= 201103L || _MSC_VER >= 1900
    strbuff_ref& put(char16_t c) { return put_utf_ch(c); }
    strbuff_ref& put(char16_t const* s, std::size_t l) { return put_utf(s, l); }
    strbuff_ref& put(char16_t const* s) { return put(s, std::char_traits<char16_t>::length(s)); }
    strbuff_ref& put(char32_t c) { return put_utf_ch(c); }
    strbuff_ref& put(char32_t const* s, std::size_t l) { return put_utf(s, l); }
    strbuff_ref& put(char32_t const* s) { return put(s, std::char_traits<char32_t>::length(s)); }
  #endif
 #endif

 #if 0
    template<class T>
    strbuff_ref& put(T const& t) {
     #if STRBUFF_USE_STRSTREM
        std::stringstream ss;
        ss << t;
        return put(ss.str());
     #else
        return *this;
     #endif
    }
 #endif

private:
    template<class C>
    strbuff_ref& put_sl(C const* s, std::size_t l);
 #if !defined(STRBUFF_NO_UNICODE)
    template<class C> strbuff_ref& put_utf_ch(C c) { C buf[2] = { c , 0 }; return put_utf(buf, 1); }
    template<class C> strbuff_ref& put_utf(C const* src, std::size_t src_sz);
 #endif

private:
    char* cur_;
    char* bgn_;
    char* end_;
};

template<class C>
strbuff_ref& strbuff_ref::put_sl(C const* s, std::size_t l) {
    if (cur_ < end_) {
        std::size_t cap = end_ - cur_;
        if (l > cap)
            l = cap;
        std::memcpy(cur_, s, l);
        cur_ += l;
        *cur_ = 0;
    }
    return *this;
}

template<class C>
strbuff_ref& strbuff_ref::fmt(C const* fmt, ...) {
    char* en = end_;
    if (cur_ < en) {
        std::va_list ap;
        va_start(ap, fmt);
        cur_ += std::vsnprintf(cur_, en - cur_, fmt, ap);
        va_end(ap);
    }
    return *this;
}

#if !defined(STRBUFF_NO_UNICODE)
template<class C>
strbuff_ref& strbuff_ref::put_utf(C const* src, std::size_t src_sz) {
    unsigned buf_sz = unsigned(src_sz * 6);
    char  buf0[1024];
    char* buf = buf0;
    if (buf_sz > sizeof(buf0)-1) {
     #if defined(_MSC_VER)
        buf = (char*)_alloca(buf_sz+1);
     #else
        buf = (char*)alloca(buf_sz+1);
     #endif
    }
    std::size_t l = str_from_unienc(buf, buf_sz, src, src_sz);
    return put(buf, l);
}
#endif

template<std::size_t N>
struct strbuff : public strbuff_ref {
    strbuff() : strbuff_ref(buf_, N) {}
private:
    char buf_[N];
};

}

#endif  // STRBUFF_HPP_INCLUDE__
