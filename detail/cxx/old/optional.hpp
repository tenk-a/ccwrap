/**
 *  @file   _ccwrap_detail/optional.hpp
 *  @brief  std::optional backport for pre-C++17 (C++03..C++14).
 *  @license Boost Software License Version 1.0
 *  @note
 *      Value semantics only (copy). No move on C++03. emplace / in_place use the
 *      fnctmpl variadic emulation (0..6 args) where real variadic templates are
 *      missing. Storage alignment is a union of common fundamental types, so
 *      over-aligned T (alignas > 8) is not supported.
 */
#ifndef _CCW_DETAIL_OPTIONAL_HPP
#define _CCW_DETAIL_OPTIONAL_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include <cstddef>
#include <new>
#include <exception>
#include "fnctmpl.hpp"

#if defined(_MSC_VER) && _MSC_VER >= 1400
# pragma warning(push)
# pragma warning(disable:4521 4522)  /* multiple copy ctor / assign (harmless here) */
#endif

// _CCW_STD17 == std when _CCW_TARGET_CXX >= 2017, else _ccw: std::optional is
// exposed only when the target standard includes it; _ccw::optional always works
// (aliased below). See doc/ccwrap_impl_rule.md.
namespace _CCW_STD17 {
using namespace ::std;   /* resolve unqualified std names when this ns is _ccw */

// --- nullopt_t / nullopt -----------------------------------------------------
struct nullopt_t {
    struct __ccw_tag {};
    explicit nullopt_t(__ccw_tag) {}
};
static const nullopt_t nullopt = nullopt_t(nullopt_t::__ccw_tag());

// --- in_place_t / in_place (shared with the <expected> backport) -------------
#ifndef _CCW_STD_IN_PLACE_DEFINED
#define _CCW_STD_IN_PLACE_DEFINED
struct in_place_t { in_place_t() {} };
static const in_place_t in_place = in_place_t();
#endif

// --- bad_optional_access -----------------------------------------------------
class bad_optional_access : public std::exception {
public:
    virtual const char* what() const noexcept { return "bad optional access"; }
};

// --- optional ----------------------------------------------------------------
template<class T>
class optional {
    union _store_t {
        char        buf_[sizeof(T)];
        void*       a_p_;
        double      a_d_;
        long double a_ld_;
        long        a_l_;
    } store_;
    bool has_;

    void*    _storage()       { return static_cast<void*>(&store_); }
    T*       _ptr()           { return reinterpret_cast<T*>(&store_); }
    T const* _ptr()     const { return reinterpret_cast<T const*>(&store_); }

    typedef bool optional::* _bool_type;

public:
    typedef T value_type;

    optional() : has_(false) {}
    optional(nullopt_t) : has_(false) {}
    optional(const T& v) : has_(false) { ::new(_storage()) T(v); has_ = true; }
    optional(const optional& r) : has_(false) {
        if (r.has_) { ::new(_storage()) T(*r._ptr()); has_ = true; }
    }
    ~optional() { reset(); }

#if _CCW_NO_VARIADIC_TEMPLATES
    explicit optional(in_place_t) : has_(false) { ::new(_storage()) T(); has_ = true; }
#define _CCW_OPT_INPLACE_CTOR(N)                                                     \
    template<_CCW_FNCTMPL_TMPLARG(N)>                                                \
    explicit optional(in_place_t _CCW_FNCTMPL_CM_DECLARG(N)) : has_(false) {         \
        ::new(_storage()) T(_CCW_FNCTMPL_FNCARG(N)); has_ = true;                    \
    }
    _CCW_OPT_INPLACE_CTOR(1) _CCW_OPT_INPLACE_CTOR(2) _CCW_OPT_INPLACE_CTOR(3)
    _CCW_OPT_INPLACE_CTOR(4) _CCW_OPT_INPLACE_CTOR(5) _CCW_OPT_INPLACE_CTOR(6)
    _CCW_OPT_INPLACE_CTOR(7) _CCW_OPT_INPLACE_CTOR(8) _CCW_OPT_INPLACE_CTOR(9) _CCW_OPT_INPLACE_CTOR(10)
#undef _CCW_OPT_INPLACE_CTOR
#else
    template<class... _Ts>
    explicit optional(in_place_t, _Ts&&... _as) : has_(false) {
        ::new(_storage()) T(static_cast<_Ts&&>(_as)...); has_ = true;
    }
#endif

    optional& operator=(nullopt_t) { reset(); return *this; }
    optional& operator=(const T& v) {
        if (has_) *_ptr() = v;
        else { ::new(_storage()) T(v); has_ = true; }
        return *this;
    }
    optional& operator=(const optional& r) {
        if (this == &r) return *this;
        if (r.has_) {
            if (has_) *_ptr() = *r._ptr();
            else { ::new(_storage()) T(*r._ptr()); has_ = true; }
        } else {
            reset();
        }
        return *this;
    }

    bool has_value() const { return has_; }
    operator _bool_type() const { return has_ ? &optional::has_ : (_bool_type)0; }
    bool operator!() const { return !has_; }

    T&       operator*()        { return *_ptr(); }
    T const& operator*()  const { return *_ptr(); }
    T*       operator->()       { return _ptr(); }
    T const* operator->() const { return _ptr(); }

    T&       value()       { if (!has_) throw bad_optional_access(); return *_ptr(); }
    T const& value() const { if (!has_) throw bad_optional_access(); return *_ptr(); }

    T value_or(const T& dflt) const { return has_ ? *_ptr() : dflt; }

    void reset() { if (has_) { _ptr()->~T(); has_ = false; } }

    void swap(optional& r) {
        if (has_ && r.has_) {
            T tmp(*_ptr()); *_ptr() = *r._ptr(); *r._ptr() = tmp;
        } else if (has_) {
            ::new(r._storage()) T(*_ptr()); r.has_ = true; reset();
        } else if (r.has_) {
            ::new(_storage()) T(*r._ptr()); has_ = true; r.reset();
        }
    }

#if _CCW_NO_VARIADIC_TEMPLATES
    T& emplace() { reset(); ::new(_storage()) T(); has_ = true; return *_ptr(); }
#define _CCW_OPT_EMPLACE(N)                                                          \
    template<_CCW_FNCTMPL_TMPLARG(N)>                                                \
    T& emplace(_CCW_FNCTMPL_DECLARG(N)) {                                            \
        reset(); ::new(_storage()) T(_CCW_FNCTMPL_FNCARG(N)); has_ = true;           \
        return *_ptr();                                                              \
    }
    _CCW_OPT_EMPLACE(1) _CCW_OPT_EMPLACE(2) _CCW_OPT_EMPLACE(3)
    _CCW_OPT_EMPLACE(4) _CCW_OPT_EMPLACE(5) _CCW_OPT_EMPLACE(6)
    _CCW_OPT_EMPLACE(7) _CCW_OPT_EMPLACE(8) _CCW_OPT_EMPLACE(9) _CCW_OPT_EMPLACE(10)
#undef _CCW_OPT_EMPLACE
#else
    template<class... _Ts>
    T& emplace(_Ts&&... _as) {
        reset(); ::new(_storage()) T(static_cast<_Ts&&>(_as)...); has_ = true;
        return *_ptr();
    }
#endif
};

// --- make_optional -----------------------------------------------------------
template<class T>
inline optional<T> make_optional(const T& v) { return optional<T>(v); }

// --- comparisons -------------------------------------------------------------
template<class T> inline bool operator==(const optional<T>& a, const optional<T>& b) {
    return a.has_value() == b.has_value() && (!a.has_value() || *a == *b);
}
template<class T> inline bool operator!=(const optional<T>& a, const optional<T>& b) { return !(a == b); }

template<class T> inline bool operator==(const optional<T>& a, nullopt_t) { return !a.has_value(); }
template<class T> inline bool operator==(nullopt_t, const optional<T>& a) { return !a.has_value(); }
template<class T> inline bool operator!=(const optional<T>& a, nullopt_t) { return a.has_value(); }
template<class T> inline bool operator!=(nullopt_t, const optional<T>& a) { return a.has_value(); }

template<class T> inline bool operator==(const optional<T>& a, const T& v) { return a.has_value() && *a == v; }
template<class T> inline bool operator==(const T& v, const optional<T>& a) { return a.has_value() && *a == v; }
template<class T> inline bool operator!=(const optional<T>& a, const T& v) { return !(a == v); }
template<class T> inline bool operator!=(const T& v, const optional<T>& a) { return !(a == v); }

// relational (optional vs optional)
template<class T> inline bool operator<(const optional<T>& a, const optional<T>& b) {
    if (!b.has_value()) return false;
    if (!a.has_value()) return true;
    return *a < *b;
}
template<class T> inline bool operator>(const optional<T>& a, const optional<T>& b)  { return b < a; }
template<class T> inline bool operator<=(const optional<T>& a, const optional<T>& b) { return !(b < a); }
template<class T> inline bool operator>=(const optional<T>& a, const optional<T>& b) { return !(a < b); }
// relational (optional vs nullopt)
template<class T> inline bool operator<(const optional<T>&, nullopt_t)  { return false; }
template<class T> inline bool operator<(nullopt_t, const optional<T>& a){ return a.has_value(); }
template<class T> inline bool operator>(const optional<T>& a, nullopt_t){ return a.has_value(); }
template<class T> inline bool operator>(nullopt_t, const optional<T>&)  { return false; }
template<class T> inline bool operator<=(const optional<T>& a, nullopt_t){ return !a.has_value(); }
template<class T> inline bool operator<=(nullopt_t, const optional<T>&) { return true; }
template<class T> inline bool operator>=(const optional<T>&, nullopt_t) { return true; }
template<class T> inline bool operator>=(nullopt_t, const optional<T>& a){ return !a.has_value(); }
// relational (optional vs value)
template<class T> inline bool operator<(const optional<T>& a, const T& v) { return a.has_value() ? (*a < v) : true; }
template<class T> inline bool operator<(const T& v, const optional<T>& a) { return a.has_value() ? (v < *a) : false; }
template<class T> inline bool operator>(const optional<T>& a, const T& v) { return a.has_value() ? (v < *a) : false; }
template<class T> inline bool operator>(const T& v, const optional<T>& a) { return a.has_value() ? (*a < v) : true; }
template<class T> inline bool operator<=(const optional<T>& a, const T& v){ return a.has_value() ? !(v < *a) : true; }
template<class T> inline bool operator<=(const T& v, const optional<T>& a){ return a.has_value() ? !(*a < v) : false; }
template<class T> inline bool operator>=(const optional<T>& a, const T& v){ return a.has_value() ? !(*a < v) : false; }
template<class T> inline bool operator>=(const T& v, const optional<T>& a){ return a.has_value() ? !(v < *a) : true; }

template<class T> inline void swap(optional<T>& a, optional<T>& b) { a.swap(b); }

}   // namespace _CCW_STD17

// Mirror into _ccw when _CCW_STD17 is std (target >= 17). When target < 17 the
// facility is already in _ccw, so a using-declaration would just self-reference.
namespace _ccw {
    using _CCW_STD17::nullopt_t;   using _CCW_STD17::nullopt;
    using _CCW_STD17::in_place_t;  using _CCW_STD17::in_place;
    using _CCW_STD17::bad_optional_access;
    using _CCW_STD17::optional;    using _CCW_STD17::make_optional;
}

#if defined(_MSC_VER) && _MSC_VER >= 1400
# pragma warning(pop)
#endif

#endif  // _CCW_DETAIL_OPTIONAL_HPP
