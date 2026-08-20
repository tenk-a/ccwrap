/**
 *  @file   _ccwrap_detail/variant.hpp
 *  @brief  std::variant (up to 10 alternatives) for pre-C++17 VC, recursive type-list.
 *  @license Boost Software License Version 1.0
 *  @note
 *      Value semantics only (copy; no move on C++03). Alternatives are matched
 *      by exact type (no conversion / overload resolution). Type-based get needs
 *      distinct alternative types. `visit` is not provided. Not strongly
 *      exception-safe on a throwing assignment (may become valueless).
 *
 *      The alternative types are handled through a recursive head/tail type-list
 *      (__ccw_vl); index-of, alternative-at, the storage size and the runtime
 *      destroy/copy/compare dispatch all recurse into the tail, so there is no
 *      per-index 0..9 boilerplate. The active alternative is still stored in one
 *      aligned buffer sized to the largest alternative.
 */
#ifndef _CCW_DETAIL_VARIANT_HPP
#define _CCW_DETAIL_VARIANT_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include <cstddef>
#include <new>
#include <exception>
#include <type_traits>
#include "fnctmpl.hpp"

#if defined(_MSC_VER) && _MSC_VER >= 1400
# pragma warning(push)
# pragma warning(disable:4127)  /* conditional expression is constant */
#endif

namespace _CCW_STD17 {
using namespace ::std;   /* resolve unqualified std names when this ns is _ccw */

// --- monostate ---------------------------------------------------------------
struct monostate {};
inline bool operator==(monostate, monostate) { return true; }
inline bool operator!=(monostate, monostate) { return false; }
inline bool operator< (monostate, monostate) { return false; }
inline bool operator> (monostate, monostate) { return false; }
inline bool operator<=(monostate, monostate) { return true; }
inline bool operator>=(monostate, monostate) { return true; }

// --- bad_variant_access ------------------------------------------------------
#if !defined(_MSC_VER) || _MSC_VER < 1910
class bad_variant_access : public std::exception {
public:
    virtual const char* what() const noexcept { return "bad variant access"; }
};
#endif

static const std::size_t variant_npos = std::size_t(-1);

// --- recursive type-list -----------------------------------------------------
struct __ccw_variant_nil {};
inline bool operator==(const __ccw_variant_nil&, const __ccw_variant_nil&) { return true; }
template<class T> struct __ccw_vnil                   { enum { v = 0 }; };
template<>        struct __ccw_vnil<__ccw_variant_nil> { enum { v = 1 }; };

template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
struct __ccw_vl {
    typedef T0 head;
    typedef __ccw_vl<T1, T2, T3, T4, T5, T6, T7, T8, T9, __ccw_variant_nil> tail;
};

// storage size = max sizeof over the non-nil alternatives (recurse, stop at nil head).
template<class VL> struct __ccw_vl_maxsize;
template<class VL, int HeadNil> struct __ccw_vl_ms_i {   /* HeadNil == 0 */
    enum { _h = (int)sizeof(typename VL::head),
           _t = __ccw_vl_maxsize<typename VL::tail>::value,
           value = _h > _t ? _h : _t };
};
template<class VL> struct __ccw_vl_ms_i<VL, 1> { enum { value = 0 }; };
template<class VL> struct __ccw_vl_maxsize {
    enum { value = __ccw_vl_ms_i<VL, __ccw_vnil<typename VL::head>::v>::value };
};

// index of U among the alternatives (recurse; -1 if absent).
template<class U, class VL> struct __ccw_vl_index;
template<class U, class VL, int HeadNil, int Match> struct __ccw_vl_idx_i {  /* not nil, no match */
    enum { _r = __ccw_vl_index<U, typename VL::tail>::value, value = _r < 0 ? -1 : _r + 1 };
};
template<class U, class VL, int HeadNil> struct __ccw_vl_idx_i<U, VL, HeadNil, 1> { enum { value = 0 }; };
template<class U, class VL>              struct __ccw_vl_idx_i<U, VL, 1, 0>       { enum { value = -1 }; };
template<class U, class VL> struct __ccw_vl_index {
    enum { value = __ccw_vl_idx_i<U, VL, __ccw_vnil<typename VL::head>::v,
                                  (is_same<U, typename VL::head>::value ? 1 : 0)>::value };
};

// the I-th alternative type (recurse into the tail).
template<std::size_t I, class VL> struct __ccw_vl_at { typedef typename __ccw_vl_at<I - 1, typename VL::tail>::type type; };
template<class VL>                struct __ccw_vl_at<0, VL> { typedef typename VL::head type; };

// runtime dispatch on the active index (recurse; the all-nil tail is the base case).
template<class VL> struct __ccw_vdispatch {
    typedef typename VL::head H;
    static void destroy(int idx, void* p) {
        if (idx == 0) reinterpret_cast<H*>(p)->~H();
        else          __ccw_vdispatch<typename VL::tail>::destroy(idx - 1, p);
    }
    static void copy(int idx, void* dst, const void* src) {
        if (idx == 0) ::new(dst) H(*reinterpret_cast<const H*>(src));
        else          __ccw_vdispatch<typename VL::tail>::copy(idx - 1, dst, src);
    }
    static bool equal(int idx, const void* a, const void* b) {
        if (idx == 0) return *reinterpret_cast<const H*>(a) == *reinterpret_cast<const H*>(b);
        return __ccw_vdispatch<typename VL::tail>::equal(idx - 1, a, b);
    }
    static bool less(int idx, const void* a, const void* b) {
        if (idx == 0) return *reinterpret_cast<const H*>(a) < *reinterpret_cast<const H*>(b);
        return __ccw_vdispatch<typename VL::tail>::less(idx - 1, a, b);
    }
};
template<> struct __ccw_vdispatch<__ccw_vl<__ccw_variant_nil, __ccw_variant_nil, __ccw_variant_nil,
                                           __ccw_variant_nil, __ccw_variant_nil, __ccw_variant_nil,
                                           __ccw_variant_nil, __ccw_variant_nil, __ccw_variant_nil,
                                           __ccw_variant_nil> > {
    static void destroy(int, void*) {}
    static void copy(int, void*, const void*) {}
    static bool equal(int, const void*, const void*) { return true; }
    static bool less(int, const void*, const void*) { return false; }
};

// --- variant -----------------------------------------------------------------
template<class T0, class T1 = __ccw_variant_nil, class T2 = __ccw_variant_nil,
         class T3 = __ccw_variant_nil, class T4 = __ccw_variant_nil, class T5 = __ccw_variant_nil,
         class T6 = __ccw_variant_nil, class T7 = __ccw_variant_nil, class T8 = __ccw_variant_nil,
         class T9 = __ccw_variant_nil>
class variant {
    typedef __ccw_vl<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> VL;
    union {
        char        buf_[__ccw_vl_maxsize<VL>::value];
        void*       a_p_;
        double      a_d_;
        long double a_ld_;
        long        a_l_;
    } store_;
    int idx_;

    void _destroy() { if (idx_ >= 0) __ccw_vdispatch<VL>::destroy(idx_, &store_); idx_ = -1; }
    void _copy_from(const variant& r) {
        if (r.idx_ >= 0) __ccw_vdispatch<VL>::copy(r.idx_, &store_, &r.store_);
        idx_ = r.idx_;
    }
    template<class X> static int _idx_of() { return __ccw_vl_index<X, VL>::value; }

public:
    // internal raw storage access (used by free get/get_if/comparisons)
    template<class X> X*       _raw_ptr()       { return reinterpret_cast<X*>(&store_); }
    template<class X> X const* _raw_ptr() const { return reinterpret_cast<X const*>(&store_); }

    variant() { ::new((void*)&store_) T0(); idx_ = 0; }

    template<class U>
    variant(const U& v) {
        static_assert((__ccw_vl_index<U, VL>::value >= 0), "type is not an alternative of this variant");
        ::new((void*)&store_) U(v);
        idx_ = __ccw_vl_index<U, VL>::value;
    }

    variant(const variant& r) { _copy_from(r); }
    ~variant() { _destroy(); }

    variant& operator=(const variant& r) {
        if (this != &r) { _destroy(); _copy_from(r); }
        return *this;
    }
    template<class U>
    variant& operator=(const U& v) {
        static_assert((__ccw_vl_index<U, VL>::value >= 0), "type is not an alternative of this variant");
        _destroy();
        ::new((void*)&store_) U(v);
        idx_ = __ccw_vl_index<U, VL>::value;
        return *this;
    }

    std::size_t index() const { return idx_ < 0 ? variant_npos : std::size_t(idx_); }
    bool valueless_by_exception() const { return idx_ < 0; }

    void swap(variant& r) { variant tmp(*this); *this = r; r = tmp; }

#if _CCW_NO_VARIADIC_TEMPLATES
    template<class X> X& emplace() {
        _destroy(); ::new((void*)&store_) X(); idx_ = _idx_of<X>(); return *_raw_ptr<X>();
    }
#define _CCW_VAR_EMPLACE(N)                                                          \
    template<class X _CCW_FNCTMPL_CM_TMPLARG(N)>                                     \
    X& emplace(_CCW_FNCTMPL_DECLARG(N)) {                                            \
        _destroy(); ::new((void*)&store_) X(_CCW_FNCTMPL_FNCARG(N));                 \
        idx_ = _idx_of<X>(); return *_raw_ptr<X>();                                  \
    }
    _CCW_VAR_EMPLACE(1) _CCW_VAR_EMPLACE(2) _CCW_VAR_EMPLACE(3)
    _CCW_VAR_EMPLACE(4) _CCW_VAR_EMPLACE(5) _CCW_VAR_EMPLACE(6)
    _CCW_VAR_EMPLACE(7) _CCW_VAR_EMPLACE(8) _CCW_VAR_EMPLACE(9) _CCW_VAR_EMPLACE(10)
#undef _CCW_VAR_EMPLACE
#else
    template<class X, class... _Ts>
    X& emplace(_Ts&&... _as) {
        _destroy(); ::new((void*)&store_) X(static_cast<_Ts&&>(_as)...);
        idx_ = _idx_of<X>(); return *_raw_ptr<X>();
    }
#endif
};

// --- variant_size / variant_alternative --------------------------------------
template<class V> struct variant_size;
template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
struct variant_size<variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> > {
    enum { value = 10 - (__ccw_vnil<T0>::v + __ccw_vnil<T1>::v + __ccw_vnil<T2>::v
                       + __ccw_vnil<T3>::v + __ccw_vnil<T4>::v + __ccw_vnil<T5>::v
                       + __ccw_vnil<T6>::v + __ccw_vnil<T7>::v + __ccw_vnil<T8>::v
                       + __ccw_vnil<T9>::v) };
};

template<std::size_t I, class V> struct variant_alternative;
template<std::size_t I, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
struct variant_alternative<I, variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> > {
    typedef typename __ccw_vl_at<I, __ccw_vl<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> >::type type;
};

// --- holds_alternative -------------------------------------------------------
template<class X, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
inline bool holds_alternative(const variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>& v) {
    return int(v.index()) == __ccw_vl_index<X, __ccw_vl<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> >::value;
}

// --- get<T> ------------------------------------------------------------------
template<class X, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
inline X& get(variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>& v) {
    if (!holds_alternative<X>(v)) throw bad_variant_access();
    return *v.template _raw_ptr<X>();
}
template<class X, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
inline X const& get(const variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>& v) {
    if (!holds_alternative<X>(v)) throw bad_variant_access();
    return *v.template _raw_ptr<X>();
}

// --- get<I> ------------------------------------------------------------------
template<std::size_t I, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
inline typename variant_alternative<I, variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> >::type&
get(variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>& v) {
    typedef typename variant_alternative<I, variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> >::type X;
    if (v.index() != I) throw bad_variant_access();
    return *v.template _raw_ptr<X>();
}
template<std::size_t I, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
inline typename variant_alternative<I, variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> >::type const&
get(const variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>& v) {
    typedef typename variant_alternative<I, variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> >::type X;
    if (v.index() != I) throw bad_variant_access();
    return *v.template _raw_ptr<X>();
}

// --- get_if ------------------------------------------------------------------
template<class X, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
inline X* get_if(variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>* v) {
    return (v && holds_alternative<X>(*v)) ? v->template _raw_ptr<X>() : (X*)0;
}
template<class X, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
inline X const* get_if(const variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>* v) {
    return (v && holds_alternative<X>(*v)) ? v->template _raw_ptr<X>() : (X const*)0;
}

// --- comparisons -------------------------------------------------------------
template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
inline bool operator==(const variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>& a,
                       const variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>& b) {
    if (a.index() != b.index()) return false;
    if (a.valueless_by_exception()) return true;   /* both valueless */
    return __ccw_vdispatch<__ccw_vl<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> >::equal(
        int(a.index()), a.template _raw_ptr<char>(), b.template _raw_ptr<char>());
}
template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
inline bool operator!=(const variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>& a,
                       const variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>& b) {
    return !(a == b);
}
template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
inline bool operator<(const variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>& a,
                      const variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>& b) {
    if (b.valueless_by_exception()) return false;
    if (a.valueless_by_exception()) return true;
    if (a.index() < b.index()) return true;
    if (a.index() > b.index()) return false;
    return __ccw_vdispatch<__ccw_vl<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> >::less(
        int(a.index()), a.template _raw_ptr<char>(), b.template _raw_ptr<char>());
}
template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
inline bool operator>(const variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>& a,
                      const variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>& b) { return b < a; }
template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
inline bool operator<=(const variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>& a,
                       const variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>& b) { return !(b < a); }
template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
inline bool operator>=(const variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>& a,
                       const variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>& b) { return !(a < b); }

template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
inline void swap(variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>& a, variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>& b) { a.swap(b); }

}   // namespace _CCW_STD17

namespace _ccw {
    using _CCW_STD17::monostate;      using _CCW_STD17::variant_npos;
    using _CCW_STD17::variant;        using _CCW_STD17::variant_size;      using _CCW_STD17::variant_alternative;
    using _CCW_STD17::holds_alternative; using _CCW_STD17::get;            using _CCW_STD17::get_if;
    using _CCW_STD17::bad_variant_access;
}

#if defined(_MSC_VER) && _MSC_VER >= 1400
# pragma warning(pop)
#endif

#endif  // _CCW_DETAIL_VARIANT_HPP
