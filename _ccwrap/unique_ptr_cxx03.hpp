/**
 *  @file   unique_ptr_cxx03.hpp
 *  @brief  tiny unique_ptr for c++03
 *  @author tenka@6809.net (Masashi Kitamura)
 *  @date   2020
 *  @license Boost Software Lisence Version 1.0
 */
#ifndef __ccwrap_unique_ptr_hpp_INCLUDED
#define __ccwrap_unique_ptr_hpp_INCLUDED

#include <cstddef>
#include <cassert>

#ifndef __CCWRAP_STD
#define __CCWRAP_STD     ccwrap
#endif

namespace __CCWRAP_STD {

#ifndef __CCWRAP_DEFAULT_DELETE_DEFINED
#define __CCWRAP_DEFAULT_DELETE_DEFINED
template<class T>
struct default_delete {
    void operator()(T* p) { delete p; }
};
template<class T>
struct default_delete<T[]> {
    void operator()(T (*p)[]) { T* q=(T*)p; delete[] q; }
};
#endif

template <class T,  class D = default_delete<T> >
class unique_ptr {
    struct PtDe : D {
        T*          ptr_;
    };
    PtDe            m;
public:
    typedef T*      pointer;
    typedef T       element_type;
    typedef D       deleter_type;

    unique_ptr(T* p = NULL) throw() { m.ptr_ = p; }
    unique_ptr(T* p, D const& d) throw() { get_deleter() = d; m.ptr_ = p; }
    template<class U> unique_ptr(U p) throw() { m.ptr_ = (T*)p; }
    template<class U> unique_ptr(U p, D const& d) throw() { get_deleter() = d; m.ptr_ = (T*)p; }
    ~unique_ptr() throw() { get_deleter()(m.ptr_); }

    T* get() const throw() { return m.ptr_; }
    deleter_type& get_deleter() throw() { return *(D*)&m; }
    deleter_type const& get_deleter() const throw() { return *(D*)&m; }

    T* release() throw() {
        T* ptr  = m.ptr_;
        m.ptr_ = NULL;
        return ptr;
    }

    void reset(T* t = 0) throw() {
        T* pt = m.ptr_;
        if (pt != t)
            get_deleter()(pt);
        m.ptr_= t;
    }

    T&       operator*()  throw() { assert(m.ptr_); return *m.ptr_; }
    T const& operator*()  const throw() { assert(m.ptr_); return *m.ptr_; }
    T*       operator->() throw() { assert(m.ptr_); return m.ptr_; }
    T const* operator->() const throw() { assert(m.ptr_); return m.ptr_; }
 #if 0
    T&       operator[](std::size_t idx)  throw() { assert(!m.ptr_); return m.ptr_[idx]; }
    T const& operator[](std::size_t idx)  const throw() { assert(!m.ptr_); return m.ptr_[idx]; }
 #endif
    bool operator!() const { return !m.ptr_; }
    typedef T const* const* const* const* const* const* __bool_type;
    operator __bool_type() const { return reinterpret_cast<__bool_type>(std::size_t(m.ptr_ != NULL)); }

    void swap(unique_ptr& r) throw() { T* tmp = m.ptr_; m.ptr_ = r.m.ptr_; r.m.ptr_ = tmp; }
};

template<typename T, class D>
inline void swap(unique_ptr<T,D>& l, unique_ptr<T,D>& r) { l.swap(r); }

#define __CCWRAP_UNIQUE_PTR(_op)                                            \
template <class T1, class D1, class T2, class D2>                           \
inline bool operator _op(unique_ptr<T1,D1> const& lhs                       \
    , unique_ptr<T2,D2> const& rhs)                                         \
{                                                                           \
    return lhs.get() _op rhs.get();                                         \
}                                                                           \
template<typename T, class D>                                               \
inline bool operator _op(unique_ptr<T,D> const& lhs, std::ptrdiff_t rhs) {  \
    assert(rhs == NULL);                                                    \
    return lhs.get() _op NULL;                                              \
}                                                                           \
template<typename T, class D>                                               \
inline bool operator _op(std::ptrdiff_t lhs, unique_ptr<T,D> const& rhs) {  \
    assert(lhs == NULL);                                                    \
    return NULL _op rhs.get();                                              \
}


__CCWRAP_UNIQUE_PTR(==)
__CCWRAP_UNIQUE_PTR(!=)
__CCWRAP_UNIQUE_PTR(<)
__CCWRAP_UNIQUE_PTR(>)
__CCWRAP_UNIQUE_PTR(<=)
__CCWRAP_UNIQUE_PTR(>=)
#undef __CCWRAP_UNIQUE_PTR

}   // __CCWRAP_STD


#endif  // __CCWRAP_unique_ptr_hpp_INCLUDED
