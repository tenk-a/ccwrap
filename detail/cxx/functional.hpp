/**
 *  @file   detail/functional.hpp
 *  @brief  <functional> gap-fills (reference_wrapper/ref/cref, invoke, not_fn).
 *  @license Boost Software License Version 1.0
 *  @note   Compiler-agnostic bodies; the forwarder (vc/functional) includes native
 *          <functional> first, then this file.
 */
#ifndef _CCW_DETAIL_FUNCTIONAL_HPP
#define _CCW_DETAIL_FUNCTIONAL_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>

#if _CCW_STD_LIB_LT(1600, 201103L)
namespace std {

template<class F>
struct __ccw_rw_has_result_type {
    template<class U> static char __t(typename U::result_type*);
    template<class U> static char (&__t(...))[2];
    enum { value = sizeof(__t<F>(0)) == 1 };
};
template<class F, bool HasResultType> struct __ccw_rw_res_sel { typedef void type; };
template<class F> struct __ccw_rw_res_sel<F, true> { typedef typename F::result_type type; };

template<class F> struct __ccw_rw_result {
    typedef typename __ccw_rw_res_sel<F, (bool)__ccw_rw_has_result_type<F>::value>::type type;
};
template<class R> struct __ccw_rw_result<R(*)()> { typedef R type; };
template<class R, class A0> struct __ccw_rw_result<R(*)(A0)> { typedef R type; };
template<class R, class A0, class A1> struct __ccw_rw_result<R(*)(A0, A1)> { typedef R type; };
template<class R, class A0, class A1, class A2> struct __ccw_rw_result<R(*)(A0, A1, A2)> { typedef R type; };
template<class R> struct __ccw_rw_result<R()> { typedef R type; };
template<class R, class A0> struct __ccw_rw_result<R(A0)> { typedef R type; };
template<class R, class A0, class A1> struct __ccw_rw_result<R(A0, A1)> { typedef R type; };
template<class R, class A0, class A1, class A2> struct __ccw_rw_result<R(A0, A1, A2)> { typedef R type; };

template<class T>
class reference_wrapper {
    T* ptr_;
public:
    typedef T type;
    typedef typename __ccw_rw_result<T>::type result_type;
    reference_wrapper(T& r) : ptr_(&r) {}
    reference_wrapper(const reference_wrapper& r) : ptr_(r.ptr_) {}
    reference_wrapper& operator=(const reference_wrapper& r) { ptr_ = r.ptr_; return *this; }
    operator T&() const { return *ptr_; }
    T& get() const { return *ptr_; }

    result_type operator()() const { return (*ptr_)(); }
    template<class A0>
    result_type operator()(const A0& a0) const { return (*ptr_)(a0); }
    template<class A0, class A1>
    result_type operator()(const A0& a0, const A1& a1) const { return (*ptr_)(a0, a1); }
    template<class A0, class A1, class A2>
    result_type operator()(const A0& a0, const A1& a1, const A2& a2) const { return (*ptr_)(a0, a1, a2); }
};

template<class T> inline reference_wrapper<T>       ref(T& t)        { return reference_wrapper<T>(t); }
template<class T> inline reference_wrapper<T>       ref(reference_wrapper<T> t) { return t; }
template<class T> inline reference_wrapper<const T> cref(const T& t) { return reference_wrapper<const T>(t); }
template<class T> inline reference_wrapper<const T> cref(reference_wrapper<T> t) { return reference_wrapper<const T>(t.get()); }

#if !defined(__GLIBCXX__) && !defined(_LIBCPP_VERSION) && \
    !defined(_CCW_LIBCPP___FUNCTIONAL_OPERATIONS_H) && \
    (!defined(_MSC_VER) || _MSC_VER < 1600)
template<class T> struct bit_and { T operator()(const T& a, const T& b) const { return a & b; } };
template<class T> struct bit_or  { T operator()(const T& a, const T& b) const { return a | b; } };
template<class T> struct bit_xor { T operator()(const T& a, const T& b) const { return a ^ b; } };
#endif

}   // namespace std
#endif

#if defined(__cplusplus) && \
    !defined(_CCW_LIBCPP___FUNCTIONAL_OPERATIONS_H) && \
    (defined(_MSC_VER) ? _MSC_VER < 1800 : (!defined(__GLIBCXX__) && !defined(_LIBCPP_VERSION)))
namespace std {
template<class T> struct bit_not { T operator()(const T& a) const { return ~a; } };
}   // namespace std
#endif

#if defined(__GLIBCXX__)
#  if defined(__has_include)
#    if __has_include(<bits/predefined_ops.h>)
#      define _CCW_NATIVE_HAS_EQ_LT_VOID 1
#    endif
#  else
#    define _CCW_NATIVE_HAS_EQ_LT_VOID 1
#  endif
#endif
#ifndef _CCW_NATIVE_HAS_EQ_LT_VOID
#  define _CCW_NATIVE_HAS_EQ_LT_VOID 0
#endif

#if defined(__cplusplus) && !defined(__cpp_lib_transparent_operators) && \
    (!defined(_MSC_VER) || _MSC_VER < 1800)
namespace std {

struct __ccw_transparent_tag { typedef void is_transparent; };

#define _CCW_TRANSPARENT_BINOP(_Name, _Op)                                       \
template <> struct _Name<void> : __ccw_transparent_tag {                         \
    template <class _T1, class _T2>                                              \
    _T1 operator()(const _T1& __x, const _T2& __y) const { return __x _Op __y; } \
}
#define _CCW_TRANSPARENT_PRED(_Name, _Op)                                         \
template <> struct _Name<void> : __ccw_transparent_tag {                          \
    template <class _T1, class _T2>                                               \
    bool operator()(const _T1& __x, const _T2& __y) const { return __x _Op __y; } \
}

_CCW_TRANSPARENT_BINOP(plus,       +);
_CCW_TRANSPARENT_BINOP(minus,      -);
_CCW_TRANSPARENT_BINOP(multiplies, *);
_CCW_TRANSPARENT_BINOP(divides,    /);
_CCW_TRANSPARENT_BINOP(modulus,    %);
_CCW_TRANSPARENT_BINOP(bit_and,    &);
_CCW_TRANSPARENT_BINOP(bit_or,     |);
_CCW_TRANSPARENT_BINOP(bit_xor,    ^);
#if !_CCW_NATIVE_HAS_EQ_LT_VOID
_CCW_TRANSPARENT_PRED(equal_to,      ==);
#endif
_CCW_TRANSPARENT_PRED(not_equal_to,  !=);
_CCW_TRANSPARENT_PRED(greater,       >);
#if !_CCW_NATIVE_HAS_EQ_LT_VOID
_CCW_TRANSPARENT_PRED(less,          <);
#endif
_CCW_TRANSPARENT_PRED(greater_equal, >=);
_CCW_TRANSPARENT_PRED(less_equal,    <=);
_CCW_TRANSPARENT_PRED(logical_and,   &&);
_CCW_TRANSPARENT_PRED(logical_or,    ||);

#undef _CCW_TRANSPARENT_BINOP
#undef _CCW_TRANSPARENT_PRED

template <> struct negate<void> : __ccw_transparent_tag {
    template <class _Tp> _Tp operator()(const _Tp& __x) const { return -__x; }
};
template <> struct logical_not<void> : __ccw_transparent_tag {
    template <class _Tp> bool operator()(const _Tp& __x) const { return !__x; }
};
template <> struct bit_not<void> : __ccw_transparent_tag {
    template <class _Tp> _Tp operator()(const _Tp& __x) const { return ~__x; }
};

}   // namespace std
#endif

#if _CCW_STD_LIB_LT(1900, 201703L) && \
    ((_ccw_cplusplus >= 201103L) || (defined(_MSC_VER) && _MSC_VER >= 1600))
#include <type_traits>
#include "fnctmpl.hpp"
namespace _CCW_STD17 {
namespace __ccw_inv {
template<bool IsMemFn> struct wrap;
template<> struct wrap<false> { template<class F> static F get(F f) { return f; } };
template<> struct wrap<true>  { template<class F> static auto get(F f) -> decltype(std::mem_fn(f)) { return std::mem_fn(f); } };
}
#define _CCW_GEN_INVOKE(N)                                                                       \
    template<class F _CCW_FNCTMPL_CM_TMPLARG(N)>                                                 \
    inline auto invoke(F f _CCW_FNCTMPL_CM_DECLARG(N))                                           \
        -> decltype(__ccw_inv::wrap<is_member_function_pointer<F>::value>::get(f)(_CCW_FNCTMPL_FNCARG(N))) \
    { return __ccw_inv::wrap<is_member_function_pointer<F>::value>::get(f)(_CCW_FNCTMPL_FNCARG(N)); }
_CCW_FNCTMPL_GENERATE(_CCW_GEN_INVOKE)
#undef _CCW_GEN_INVOKE
}   // _CCW_STD17
#endif

#if __cplusplus < 201703L
#include "fnctmpl.hpp"
namespace _CCW_STD17 {
template<class F>
class __ccw_not_fn_t {
    F f_;
public:
    explicit __ccw_not_fn_t(F f) : f_(f) {}
#if _CCW_NO_VARIADIC_TEMPLATES
    bool operator()()       { return !f_(); }
    bool operator()() const { return !f_(); }
#define _CCW_GEN_NOTFN(N)                                                                        \
    template<_CCW_FNCTMPL_TMPLARG(N)> bool operator()(_CCW_FNCTMPL_DECLARG(N))       { return !f_(_CCW_FNCTMPL_FNCARG(N)); } \
    template<_CCW_FNCTMPL_TMPLARG(N)> bool operator()(_CCW_FNCTMPL_DECLARG(N)) const { return !f_(_CCW_FNCTMPL_FNCARG(N)); }
    _CCW_GEN_NOTFN(1) _CCW_GEN_NOTFN(2) _CCW_GEN_NOTFN(3) _CCW_GEN_NOTFN(4) _CCW_GEN_NOTFN(5) _CCW_GEN_NOTFN(6)
#undef _CCW_GEN_NOTFN
#else
    template<class... A> bool operator()(A&&... a)       { return !f_(static_cast<A&&>(a)...); }
    template<class... A> bool operator()(A&&... a) const { return !f_(static_cast<A&&>(a)...); }
#endif
};
template<class F> inline __ccw_not_fn_t<F> not_fn(F f) { return __ccw_not_fn_t<F>(f); }
}   // _CCW_STD17
#endif

#endif  // _CCW_DETAIL_FUNCTIONAL_HPP
