//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom. _CCW_STD::hash for the built-in types, pointers,
// and _CCW_STD::string (used by the unordered containers).
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___FUNCTIONAL_HASH_H
#define _CCW_LIBCPP___FUNCTIONAL_HASH_H
#include "../__config"
#include <cstddef>
#if _CCW_LIBCPP_HAS_NATIVE_CXX11_LIB
#  if defined(_LIBCPP_VERSION)
#    include <type_traits>
#  else
#    include <functional>
#  endif
#else

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

#if defined(_MSC_VER) && _MSC_VER < 1600
template <class _Tp> struct hash {
    _CCW_STD::size_t operator()(const _Tp&) const;
};
#else
template <class _Tp> struct hash;
#endif

#define _CCW_HASH_INT(T) \
    template <> struct hash<T> { _CCW_LIBCPP_HIDE_FROM_ABI _CCW_STD::size_t operator()(T __v) const { return static_cast<_CCW_STD::size_t>(__v); } };
_CCW_HASH_INT(bool)
_CCW_HASH_INT(char)
_CCW_HASH_INT(signed char)
_CCW_HASH_INT(unsigned char)
_CCW_HASH_INT(wchar_t)
_CCW_HASH_INT(short)
_CCW_HASH_INT(unsigned short)
_CCW_HASH_INT(int)
_CCW_HASH_INT(unsigned int)
_CCW_HASH_INT(long)
_CCW_HASH_INT(unsigned long)
#undef _CCW_HASH_INT

#define _CCW_HASH_INT64(T) \
    template <> struct hash<T> { \
        _CCW_LIBCPP_HIDE_FROM_ABI _CCW_STD::size_t operator()(T __v) const { \
            unsigned long long __u = (unsigned long long)__v; \
            if (sizeof(_CCW_STD::size_t) >= sizeof(unsigned long long)) return (_CCW_STD::size_t)__u; \
            return (_CCW_STD::size_t)(__u ^ (__u >> 32)); \
        } \
    };
_CCW_HASH_INT64(long long)
_CCW_HASH_INT64(unsigned long long)
#undef _CCW_HASH_INT64

template <> struct hash<float> {
    _CCW_LIBCPP_HIDE_FROM_ABI _CCW_STD::size_t operator()(float __v) const {
        if (__v == 0.0f) return 0;   // -0.0 and +0.0 hash alike
        union { float __f; unsigned __u; } __x; __x.__f = __v; return static_cast<_CCW_STD::size_t>(__x.__u);
    }
};
template <> struct hash<double> {
    _CCW_LIBCPP_HIDE_FROM_ABI _CCW_STD::size_t operator()(double __v) const {
        if (__v == 0.0) return 0;
        union { double __d; unsigned __u[2]; } __x; __x.__d = __v; return static_cast<_CCW_STD::size_t>(__x.__u[0] ^ __x.__u[1]);
    }
};

template <> struct hash<long double> {
    _CCW_LIBCPP_HIDE_FROM_ABI _CCW_STD::size_t operator()(long double __v) const {
        if (__v == 0.0L) return 0;
        union { long double __l; unsigned char __b[sizeof(long double)]; } __x;
        __x.__l = __v;
        _CCW_STD::size_t __h = 2166136261u;
        for (unsigned __i = 0; __i < 10 && __i < sizeof(long double); ++__i)
            __h = (__h ^ (_CCW_STD::size_t)__x.__b[__i]) * 16777619u;
        return __h;
    }
};

template <class _Tp> struct hash<_Tp*> {
    _CCW_LIBCPP_HIDE_FROM_ABI _CCW_STD::size_t operator()(_Tp* __v) const { return reinterpret_cast<_CCW_STD::size_t>(__v); }
};

_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_LIBCPP_HAS_NATIVE_CXX11_LIB
#endif
