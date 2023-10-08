/**
 *  @file   unique_ptr_cxx03.hpp
 *  @brief  tiny unique_ptr for c++03
 *  @author tenka@6809.net (Masashi Kitamura)
 *  @date   2020
 *  @license Boost Software Lisence Version 1.0
 */
#ifndef __CCWRAP_UNIQUE_PTR_HPP_INCLUDED
#define __CCWRAP_UNIQUE_PTR_HPP_INCLUDED

#if __CCWRAP_CXX < 201103L

#include <cstddef>
#include <cassert>

#if defined(_MSC_VER) && _MSC_VER >= 1400
# pragma warning(push)
# pragma warning(disable:4100)
#endif

namespace std {

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

    unique_ptr(T* p = NULL) noexcept { m.ptr_ = p; }
    unique_ptr(T* p, D const& d) noexcept { get_deleter() = d; m.ptr_ = p; }
    template<class U> unique_ptr(U p) noexcept { m.ptr_ = (T*)p; }
    template<class U> unique_ptr(U p, D const& d) noexcept { get_deleter() = d; m.ptr_ = (T*)p; }
    ~unique_ptr() noexcept { get_deleter()(m.ptr_); }

    T* get() const noexcept { return m.ptr_; }
    deleter_type& get_deleter() noexcept { return *(D*)&m; }
    deleter_type const& get_deleter() const noexcept { return *(D*)&m; }

    T* release() noexcept {
        T* ptr  = m.ptr_;
        m.ptr_ = NULL;
        return ptr;
    }

    void reset(T* t = 0) noexcept {
        T* pt = m.ptr_;
        if (pt != t)
            get_deleter()(pt);
        m.ptr_= t;
    }

    T&       operator*()  noexcept { /*assert(m.ptr_);*/ return *m.ptr_; }
    T const& operator*()  const noexcept { /*assert(m.ptr_);*/ return *m.ptr_; }
    T*       operator->() noexcept { /*assert(m.ptr_);*/ return m.ptr_; }
    T const* operator->() const noexcept { /*assert(m.ptr_);*/ return m.ptr_; }
 #if 0
    T&       operator[](std::size_t idx)  noexcept { /*assert(!m.ptr_);*/ return m.ptr_[idx]; }
    T const& operator[](std::size_t idx)  const noexcept { /*assert(!m.ptr_);*/ return m.ptr_[idx]; }
 #endif
    bool operator!() const { return !m.ptr_; }
    typedef T const* const* const* const* const* const* __bool_type;
    operator __bool_type() const { return reinterpret_cast<__bool_type>(std::size_t(m.ptr_ != NULL)); }

    void swap(unique_ptr& r) noexcept { T* tmp = m.ptr_; m.ptr_ = r.m.ptr_; r.m.ptr_ = tmp; }
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
    assert(rhs == 0);                                                       \
    return lhs.get() _op NULL;                                              \
}                                                                           \
template<typename T, class D>                                               \
inline bool operator _op(std::ptrdiff_t lhs, unique_ptr<T,D> const& rhs) {  \
    assert(lhs == 0);                                                       \
    return NULL _op rhs.get();                                              \
}


__CCWRAP_UNIQUE_PTR(==)
__CCWRAP_UNIQUE_PTR(!=)
__CCWRAP_UNIQUE_PTR(<)
__CCWRAP_UNIQUE_PTR(>)
__CCWRAP_UNIQUE_PTR(<=)
__CCWRAP_UNIQUE_PTR(>=)
#undef __CCWRAP_UNIQUE_PTR

}

#if defined(_MSC_VER) && _MSC_VER >= 1400
# pragma warning(pop)
#endif

#endif	// __CCWRAP_CXX

#endif  // __CCWRAP_UNIQUE_PTR_HPP_INCLUDED
