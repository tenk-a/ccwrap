/**
 *  @file   _ccwrap_detail/any.hpp
 *  @brief  std::any backport for pre-C++17 (C++03..C++14).
 *  @license Boost Software License Version 1.0
 *  @note
 *      Value semantics only (copy); no move on C++03. Held object is always
 *      heap-allocated via a type-erased holder (no small-object optimization).
 *      emplace uses the fnctmpl variadic emulation (0..6 args). Requires RTTI
 *      (typeid) for type()/any_cast, as the standard does.
 */
#ifndef _CCW_DETAIL_ANY_HPP
#define _CCW_DETAIL_ANY_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include <typeinfo>
#include <exception>
#include "fnctmpl.hpp"

namespace _CCW_STD17 {
using namespace ::std;   /* resolve unqualified std names when this ns is _ccw */

// --- bad_any_cast ------------------------------------------------------------
class bad_any_cast : public std::bad_cast {
public:
    virtual const char* what() const throw() { return "bad any_cast"; }
};

// --- any ---------------------------------------------------------------------
class any {
    struct holder_base {
        virtual ~holder_base() {}
        virtual const std::type_info& type() const = 0;
        virtual holder_base* clone() const = 0;
    };
    template<class T>
    struct holder : holder_base {
        T value;
        holder(const T& v) : value(v) {}
        virtual const std::type_info& type() const { return typeid(T); }
        virtual holder_base* clone() const { return new holder<T>(value); }
    };

    holder_base* h_;

    template<class T> friend T* any_cast(any*);
    template<class T> friend const T* any_cast(const any*);

public:
    any() : h_(0) {}
    any(const any& o) : h_(o.h_ ? o.h_->clone() : 0) {}
    template<class T>
    any(const T& v) : h_(new holder<T>(v)) {}
    ~any() { delete h_; }

    any& operator=(const any& o) {
        if (this != &o) {
            holder_base* n = o.h_ ? o.h_->clone() : 0;
            delete h_;
            h_ = n;
        }
        return *this;
    }
    template<class T>
    any& operator=(const T& v) {
        holder_base* n = new holder<T>(v);
        delete h_;
        h_ = n;
        return *this;
    }

    void reset() { delete h_; h_ = 0; }
    void swap(any& o) { holder_base* t = h_; h_ = o.h_; o.h_ = t; }
    bool has_value() const { return h_ != 0; }
    const std::type_info& type() const { return h_ ? h_->type() : typeid(void); }

    // emplace<T>(args...) : replace the contained object with T(args...).
    #define _CCW_ANY_EMPLACE(N) \
        template<class T _CCW_FNCTMPL_CM_TMPLARG(N)> \
        T& emplace(_CCW_FNCTMPL_DECLARG(N)) { \
            holder<T>* n = new holder<T>(T(_CCW_FNCTMPL_FNCARG(N))); \
            delete h_; h_ = n; return n->value; \
        }
    _CCW_FNCTMPL_GENERATE(_CCW_ANY_EMPLACE)
    #undef _CCW_ANY_EMPLACE
};

inline void swap(any& a, any& b) { a.swap(b); }

// --- any_cast ----------------------------------------------------------------
template<class T>
inline T* any_cast(any* a) {
    if (a && a->h_ && a->h_->type() == typeid(T))
        return &static_cast<any::holder<T>*>(a->h_)->value;
    return 0;
}
template<class T>
inline const T* any_cast(const any* a) {
    if (a && a->h_ && a->h_->type() == typeid(T))
        return &static_cast<const any::holder<T>*>(a->h_)->value;
    return 0;
}
template<class T>
inline T any_cast(any& a) {
    T* p = any_cast<T>(&a);
    if (!p) throw bad_any_cast();
    return *p;
}
template<class T>
inline T any_cast(const any& a) {
    const T* p = any_cast<T>(&a);
    if (!p) throw bad_any_cast();
    return *p;
}

// --- make_any<T>(args...) ----------------------------------------------------
// Use _CCW_FNCTMPL_GENERATE (real variadic where available, 0..N fixed arities
// otherwise) -- NOT explicit arities, which would all collapse to one variadic
// signature (and redefine each other) on a variadic-template compiler.
#define _CCW_ANY_MAKE(N)                                                        \
    template<class T _CCW_FNCTMPL_CM_TMPLARG(N)>                                \
    inline any make_any(_CCW_FNCTMPL_DECLARG(N)) { return any(T(_CCW_FNCTMPL_FNCARG(N))); }
_CCW_FNCTMPL_GENERATE(_CCW_ANY_MAKE)
#undef _CCW_ANY_MAKE

}   // namespace _CCW_STD17

namespace _ccw {
    using _CCW_STD17::any;  using _CCW_STD17::bad_any_cast;  using _CCW_STD17::any_cast;  using _CCW_STD17::make_any;
}

#endif  /* _CCW_DETAIL_ANY_HPP */
