/**
 *  @file   _ccwrap_detail/expected.hpp
 *  @brief  std::expected (C++23) backport, in namespace std (with _ccw aliases).
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_DETAIL_EXPECTED_HPP
#define _CCW_DETAIL_EXPECTED_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include "eh_fail.hpp"

#if __cplusplus >= 202302L

namespace _ccw {
    using std::expected;
    using std::unexpected;
    using std::unexpect_t;
    using std::unexpect;
    using std::in_place_t;
    using std::in_place;
    using std::bad_expected_access;
    template<class E> inline std::unexpected<E> make_unexpected(const E& e) { return std::unexpected<E>(e); }
}

#elif !defined(_CCW_NO_EXPECTED_CXX23)   // ---- backport into namespace std -----

#include <cstddef>
#include <new>
#include <exception>
#include "fnctmpl.hpp"
#include "move03.hpp"   /* _CCW_RV_REF / _ccw::move -- expected and unexpected are movable */
#if __cplusplus >= 201703L
# include <utility>   /* native std::in_place / in_place_t */
#endif
#if _CCW_HAS_INIT_LIST
# include <initializer_list>
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1400
# pragma warning(push)
# pragma warning(disable:4521 4522)
#endif

#include <utility>
#if __cplusplus < 201703L
# ifndef _CCW_STD_IN_PLACE_DEFINED
# define _CCW_STD_IN_PLACE_DEFINED
namespace _CCW_STD17 {
using namespace ::std;
struct in_place_t { in_place_t() {} };
_ccw_inline_const in_place_t in_place = in_place_t();
}
# endif
#endif

namespace _CCW_STD23 {

#ifndef _CCW_STD_UNEXPECT_DEFINED
#define _CCW_STD_UNEXPECT_DEFINED
struct unexpect_t { unexpect_t() {} };
_ccw_inline_const unexpect_t unexpect = unexpect_t();
#endif

template<class E>
class unexpected {
    E err_;
public:
    unexpected(const E& e) : err_(e) {}
    explicit unexpected(_CCW_STD17::in_place_t) : err_() {}
#if _CCW_NO_VARIADIC_TEMPLATES
#define _CCW_UNEXP_CTOR(N)                                                                   \
    template<_CCW_FNCTMPL_TMPLARG(N)> explicit unexpected(_CCW_STD17::in_place_t _CCW_FNCTMPL_CM_DECLARG(N)) : err_(_CCW_FNCTMPL_FNCARG(N)) {}
    _CCW_UNEXP_CTOR(1) _CCW_UNEXP_CTOR(2) _CCW_UNEXP_CTOR(3) _CCW_UNEXP_CTOR(4) _CCW_UNEXP_CTOR(5)
    _CCW_UNEXP_CTOR(6) _CCW_UNEXP_CTOR(7) _CCW_UNEXP_CTOR(8) _CCW_UNEXP_CTOR(9) _CCW_UNEXP_CTOR(10)
#undef _CCW_UNEXP_CTOR
#else
    template<class... A> explicit unexpected(_CCW_STD17::in_place_t, A&&... a) : err_(static_cast<A&&>(a)...) {}
#endif
    template<class G> unexpected(const unexpected<G>& u) : err_(u.error()) {}
    unexpected(_CCW_RV_REF(unexpected) r) : err_(::_ccw::move(_CCW_MOVED_OBJ(r).err_)) {}
    unexpected& operator=(_CCW_RV_REF(unexpected) r) {
        unexpected& o = _CCW_MOVED_OBJ(r);
        if (this != &o) err_ = ::_ccw::move(o.err_);
        return *this;
    }
#if !_CCW_HAS_REAL_RVALUE_REF
    template<class G> unexpected(::_ccw::rv<G>& e) : err_(e) {}
#else
    unexpected(E&& e) : err_(static_cast<E&&>(e)) {}
#endif
    E&       error()       { return err_; }
    E const& error() const { return err_; }
    void swap(unexpected& o) { E t(err_); err_ = o.err_; o.err_ = t; }
};
template<class E1, class E2> inline bool operator==(const unexpected<E1>& a, const unexpected<E2>& b) { return a.error() == b.error(); }
template<class E1, class E2> inline bool operator!=(const unexpected<E1>& a, const unexpected<E2>& b) { return !(a == b); }

template<class E>
class bad_expected_access : public std::exception {
    E err_;
public:
    explicit bad_expected_access(const E& e) : err_(e) {}
    virtual const char* what() const noexcept { return "bad expected access"; }
    E&       error()       { return err_; }
    E const& error() const { return err_; }
};

template<class T, class E>
class expected {
    union {
        char        buf_[sizeof(T) > sizeof(E) ? sizeof(T) : sizeof(E)];
        void*       a_p_;
        double      a_d_;
        long double a_ld_;
        long        a_l_;
    } store_;
    bool has_;

    void*    _s()       { return static_cast<void*>(&store_); }
    T*       _v()       { return reinterpret_cast<T*>(&store_); }
    T const* _v() const { return reinterpret_cast<T const*>(&store_); }
    E*       _e()       { return reinterpret_cast<E*>(&store_); }
    E const* _e() const { return reinterpret_cast<E const*>(&store_); }
    void     _destroy() { if (has_) _v()->~T(); else _e()->~E(); }

    typedef bool expected::* _bool_type;

public:
    typedef T             value_type;
    typedef E             error_type;
    typedef unexpected<E> unexpected_type;

    expected() : has_(true) { ::new(_s()) T(); }
    expected(const T& v) : has_(true) { ::new(_s()) T(v); }
    expected(const unexpected<E>& u) : has_(false) { ::new(_s()) E(u.error()); }
    expected(const expected& r) : has_(r.has_) {
        if (has_) ::new(_s()) T(*r._v());
        else      ::new(_s()) E(*r._e());
    }
    template<class U, class G>
    expected(const expected<U, G>& r) : has_(r.has_value()) {
        if (has_) ::new(_s()) T(*r);
        else      ::new(_s()) E(r.error());
    }
    ~expected() { _destroy(); }

    expected(_CCW_RV_REF(expected) r) : has_(_CCW_MOVED_OBJ(r).has_) {
        expected& o = _CCW_MOVED_OBJ(r);
        if (has_) ::new(_s()) T(::_ccw::move(*o._v()));
        else      ::new(_s()) E(::_ccw::move(*o._e()));
    }
    expected& operator=(_CCW_RV_REF(expected) r) {
        expected& o = _CCW_MOVED_OBJ(r);
        if (this == &o) return *this;
        _destroy();
        has_ = o.has_;
        if (has_) ::new(_s()) T(::_ccw::move(*o._v()));
        else      ::new(_s()) E(::_ccw::move(*o._e()));
        return *this;
    }
#if !_CCW_HAS_REAL_RVALUE_REF
    template<class U> expected(::_ccw::rv<U>& v) : has_(true)  { ::new(_s()) T(v); }
    template<class U> expected& operator=(::_ccw::rv<U>& v) {
        _destroy(); has_ = true; ::new(_s()) T(v); return *this;
    }
    template<class G> expected(::_ccw::rv< unexpected<G> >& u) : has_(false) {
        ::new(_s()) E(::_ccw::move(_CCW_MOVED_OBJ(u).error()));
    }
#else
    expected(T&& v) : has_(true) { ::new(_s()) T(static_cast<T&&>(v)); }
    expected& operator=(T&& v) {
        if (has_) *_v() = static_cast<T&&>(v);
        else { _destroy(); has_ = true; ::new(_s()) T(static_cast<T&&>(v)); }
        return *this;
    }
    expected(unexpected<E>&& u) : has_(false) { ::new(_s()) E(static_cast<E&&>(u.error())); }
#endif

#if _CCW_NO_VARIADIC_TEMPLATES
    explicit expected(_CCW_STD17::in_place_t) : has_(true)  { ::new(_s()) T(); }
    explicit expected(unexpect_t) : has_(false) { ::new(_s()) E(); }
#define _CCW_EXP_CTOR(N)                                                                     \
    template<_CCW_FNCTMPL_TMPLARG(N)> explicit expected(_CCW_STD17::in_place_t _CCW_FNCTMPL_CM_DECLARG(N)) : has_(true)  { ::new(_s()) T(_CCW_FNCTMPL_FNCARG(N)); } \
    template<_CCW_FNCTMPL_TMPLARG(N)> explicit expected(unexpect_t _CCW_FNCTMPL_CM_DECLARG(N)) : has_(false) { ::new(_s()) E(_CCW_FNCTMPL_FNCARG(N)); }
    _CCW_EXP_CTOR(1) _CCW_EXP_CTOR(2) _CCW_EXP_CTOR(3) _CCW_EXP_CTOR(4) _CCW_EXP_CTOR(5) _CCW_EXP_CTOR(6)
    _CCW_EXP_CTOR(7) _CCW_EXP_CTOR(8) _CCW_EXP_CTOR(9) _CCW_EXP_CTOR(10)
#undef _CCW_EXP_CTOR
#else
    template<class... A> explicit expected(_CCW_STD17::in_place_t, A&&... a) : has_(true)  { ::new(_s()) T(static_cast<A&&>(a)...); }
    template<class... A> explicit expected(unexpect_t, A&&... a) : has_(false) { ::new(_s()) E(static_cast<A&&>(a)...); }
# if _CCW_HAS_INIT_LIST
    template<class U, class... A> explicit expected(_CCW_STD17::in_place_t, std::initializer_list<U> il, A&&... a) : has_(true)  { ::new(_s()) T(il, static_cast<A&&>(a)...); }
    template<class U, class... A> explicit expected(unexpect_t, std::initializer_list<U> il, A&&... a) : has_(false) { ::new(_s()) E(il, static_cast<A&&>(a)...); }
# endif
#endif

    expected& operator=(const expected& r) {
        if (this == &r) return *this;
        _destroy();
        has_ = r.has_;
        if (has_) ::new(_s()) T(*r._v());
        else      ::new(_s()) E(*r._e());
        return *this;
    }
    expected& operator=(const T& v) {
        if (has_) *_v() = v;
        else { _e()->~E(); ::new(_s()) T(v); has_ = true; }
        return *this;
    }
    expected& operator=(const unexpected<E>& u) {
        if (!has_) *_e() = u.error();
        else { _v()->~T(); ::new(_s()) E(u.error()); has_ = false; }
        return *this;
    }

    bool has_value() const { return has_; }
    operator _bool_type() const { return has_ ? &expected::has_ : (_bool_type)0; }
    bool operator!() const { return !has_; }

    T&       operator*()        { return *_v(); }
    T const& operator*()  const { return *_v(); }
    T*       operator->()       { return _v(); }
    T const* operator->() const { return _v(); }

    T&       value()       { if (!has_) _CCW_THROW(bad_expected_access<E>(*_e())); return *_v(); }
    T const& value() const { if (!has_) _CCW_THROW(bad_expected_access<E>(*_e())); return *_v(); }
    E&       error()       { return *_e(); }
    E const& error() const { return *_e(); }

    T value_or(const T& dflt) const { return has_ ? *_v() : dflt; }
    E error_or(const E& dflt) const { return has_ ? dflt : *_e(); }

    void swap(expected& o) { expected t(*this); *this = o; o = t; }

#if _CCW_NO_VARIADIC_TEMPLATES
    T& emplace() { _destroy(); ::new(_s()) T(); has_ = true; return *_v(); }
#define _CCW_EXP_EMPLACE(N)                                                                  \
    template<_CCW_FNCTMPL_TMPLARG(N)> T& emplace(_CCW_FNCTMPL_DECLARG(N)) {                   \
        _destroy(); ::new(_s()) T(_CCW_FNCTMPL_FNCARG(N)); has_ = true; return *_v();         \
    }
    _CCW_EXP_EMPLACE(1) _CCW_EXP_EMPLACE(2) _CCW_EXP_EMPLACE(3) _CCW_EXP_EMPLACE(4) _CCW_EXP_EMPLACE(5) _CCW_EXP_EMPLACE(6)
    _CCW_EXP_EMPLACE(7) _CCW_EXP_EMPLACE(8) _CCW_EXP_EMPLACE(9) _CCW_EXP_EMPLACE(10)
#undef _CCW_EXP_EMPLACE
#else
    template<class... A> T& emplace(A&&... a) { _destroy(); ::new(_s()) T(static_cast<A&&>(a)...); has_ = true; return *_v(); }
#endif
};

template<class E>
class expected<void, E> {
    union {
        char        buf_[sizeof(E)];
        void*       a_p_;
        double      a_d_;
        long double a_ld_;
        long        a_l_;
    } store_;
    bool has_;

    void*    _s()       { return static_cast<void*>(&store_); }
    E*       _e()       { return reinterpret_cast<E*>(&store_); }
    E const* _e() const { return reinterpret_cast<E const*>(&store_); }

    typedef bool expected::* _bool_type;

public:
    typedef void          value_type;
    typedef E             error_type;
    typedef unexpected<E> unexpected_type;

    expected() : has_(true) {}
    expected(const unexpected<E>& u) : has_(false) { ::new(_s()) E(u.error()); }
    expected(const expected& r) : has_(r.has_) { if (!has_) ::new(_s()) E(*r._e()); }
    ~expected() { if (!has_) _e()->~E(); }

    explicit expected(_CCW_STD17::in_place_t) : has_(true) {}
#if _CCW_NO_VARIADIC_TEMPLATES
    explicit expected(unexpect_t) : has_(false) { ::new(_s()) E(); }
#define _CCW_EXPV_CTOR(N)                                                                    \
    template<_CCW_FNCTMPL_TMPLARG(N)> explicit expected(unexpect_t _CCW_FNCTMPL_CM_DECLARG(N)) : has_(false) { ::new(_s()) E(_CCW_FNCTMPL_FNCARG(N)); }
    _CCW_EXPV_CTOR(1) _CCW_EXPV_CTOR(2) _CCW_EXPV_CTOR(3) _CCW_EXPV_CTOR(4) _CCW_EXPV_CTOR(5) _CCW_EXPV_CTOR(6)
    _CCW_EXPV_CTOR(7) _CCW_EXPV_CTOR(8) _CCW_EXPV_CTOR(9) _CCW_EXPV_CTOR(10)
#undef _CCW_EXPV_CTOR
#else
    template<class... A> explicit expected(unexpect_t, A&&... a) : has_(false) { ::new(_s()) E(static_cast<A&&>(a)...); }
# if _CCW_HAS_INIT_LIST
    template<class U, class... A> explicit expected(unexpect_t, std::initializer_list<U> il, A&&... a) : has_(false) { ::new(_s()) E(il, static_cast<A&&>(a)...); }
# endif
#endif

    expected& operator=(const expected& r) {
        if (this == &r) return *this;
        if (!has_) _e()->~E();
        has_ = r.has_;
        if (!has_) ::new(_s()) E(*r._e());
        return *this;
    }
    expected& operator=(const unexpected<E>& u) {
        if (!has_) *_e() = u.error();
        else { ::new(_s()) E(u.error()); has_ = false; }
        return *this;
    }

    bool has_value() const { return has_; }
    operator _bool_type() const { return has_ ? &expected::has_ : (_bool_type)0; }
    bool operator!() const { return !has_; }

    void operator*() const {}
    void value() const { if (!has_) _CCW_THROW(bad_expected_access<E>(*_e())); }
    E&       error()       { return *_e(); }
    E const& error() const { return *_e(); }
    E error_or(const E& dflt) const { return has_ ? dflt : *_e(); }

    void swap(expected& o) { expected t(*this); *this = o; o = t; }
};

template<class T, class E>
inline bool operator==(const expected<T, E>& a, const expected<T, E>& b) {
    if (a.has_value() != b.has_value()) return false;
    return a.has_value() ? (*a == *b) : (a.error() == b.error());
}
template<class T, class E>
inline bool operator!=(const expected<T, E>& a, const expected<T, E>& b) { return !(a == b); }
template<class T, class E>
inline bool operator==(const expected<T, E>& a, const T& v) { return a.has_value() && *a == v; }
template<class T, class E>
inline bool operator==(const T& v, const expected<T, E>& a) { return a.has_value() && *a == v; }
template<class T, class E, class E2>
inline bool operator==(const expected<T, E>& a, const unexpected<E2>& u) { return !a.has_value() && a.error() == u.error(); }
template<class T, class E, class E2>
inline bool operator==(const unexpected<E2>& u, const expected<T, E>& a) { return !a.has_value() && a.error() == u.error(); }

}   // namespace _CCW_STD23

namespace _ccw {
    using _CCW_STD23::expected;
    using _CCW_STD23::unexpected;
    using _CCW_STD23::unexpect_t;
    using _CCW_STD23::unexpect;
    using _CCW_STD23::bad_expected_access;
    using _CCW_STD17::in_place_t;
    using _CCW_STD17::in_place;
    template<class E> inline _CCW_STD23::unexpected<E> make_unexpected(const E& e) { return _CCW_STD23::unexpected<E>(e); }
}

#if defined(_MSC_VER) && _MSC_VER >= 1400
# pragma warning(pop)
#endif

#endif  // __cplusplus >= 202302L

#endif  // _CCW_DETAIL_EXPECTED_HPP
