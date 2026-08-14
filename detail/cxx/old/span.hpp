/**
 *  @file   _ccwrap_detail/span.hpp
 *  @brief  std::span backport for pre-C++20 (C++03..C++17).
 *  @license Boost Software License Version 1.0
 *  @note
 *      Non-owning view over a contiguous sequence. Stores pointer + size (the
 *      static-extent size is stored too, for simplicity). No constexpr on old
 *      compilers.
 */
#ifndef _CCW_DETAIL_SPAN_HPP
#define _CCW_DETAIL_SPAN_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include <cstddef>
#include <iterator>
#include <type_traits>

namespace _CCW_STD20 {
using namespace ::std;

static const std::size_t dynamic_extent = std::size_t(-1);

template<class T, std::size_t Extent = dynamic_extent>
class span {
    T*          ptr_;
    std::size_t size_;
public:
    typedef T                                       element_type;
    typedef typename remove_cv<T>::type             value_type;
    typedef std::size_t                             size_type;
    typedef std::ptrdiff_t                          difference_type;
    typedef T*                                      pointer;
    typedef const T*                                const_pointer;
    typedef T&                                      reference;
    typedef const T&                                const_reference;
    typedef T*                                      iterator;
    typedef const T*                                const_iterator;
    typedef std::reverse_iterator<iterator>         reverse_iterator;
    typedef std::reverse_iterator<const_iterator>   const_reverse_iterator;

    static const std::size_t extent = Extent;

    span() : ptr_(0), size_(0) {}
    span(T* p, std::size_t n) : ptr_(p), size_(n) {}
    span(T* first, T* last) : ptr_(first), size_(std::size_t(last - first)) {}
    template<std::size_t N> span(T (&a)[N]) : ptr_(a), size_(N) {}
    template<class C> span(C& c) : ptr_(c.data()), size_(c.size()) {}
    template<class C> span(const C& c) : ptr_(c.data()), size_(c.size()) {}
    span(const span& r) : ptr_(r.ptr_), size_(r.size_) {}
    span& operator=(const span& r) { ptr_ = r.ptr_; size_ = r.size_; return *this; }

    pointer         data()       const { return ptr_; }
    size_type       size()       const { return size_; }
    size_type       size_bytes() const { return size_ * sizeof(T); }
    bool            empty()      const { return size_ == 0; }

    reference       operator[](size_type i) const { return ptr_[i]; }
    reference       front()      const { return ptr_[0]; }
    reference       back()       const { return ptr_[size_ - 1]; }

    iterator        begin()      const { return ptr_; }
    iterator        end()        const { return ptr_ + size_; }
    const_iterator  cbegin()     const { return ptr_; }
    const_iterator  cend()       const { return ptr_ + size_; }
    reverse_iterator        rbegin()  const { return reverse_iterator(end()); }
    reverse_iterator        rend()    const { return reverse_iterator(begin()); }
    const_reverse_iterator  crbegin() const { return const_reverse_iterator(cend()); }
    const_reverse_iterator  crend()   const { return const_reverse_iterator(cbegin()); }

    span<T> first(size_type count) const { return span<T>(ptr_, count); }
    span<T> last(size_type count)  const { return span<T>(ptr_ + (size_ - count), count); }
    span<T> subspan(size_type offset, size_type count = dynamic_extent) const {
        return span<T>(ptr_ + offset, count == dynamic_extent ? size_ - offset : count);
    }
};

template<class T, std::size_t Extent>
const std::size_t span<T, Extent>::extent;

// as_bytes / as_writable_bytes need _CCW_STD17::byte. That used to mean vc11 and up
// (the enum-class backport in vc/cstddef); vc8/9/10 now get the C++03 stand-in from
// detail/byte_cxx03.hpp, so there is no version left without one -- and the gate that
// said otherwise was why span_test could not compile on vc9.
template<class T, std::size_t E>
inline span<const _CCW_STD17::byte> as_bytes(span<T, E> s) {
    return span<const _CCW_STD17::byte>(reinterpret_cast<const _CCW_STD17::byte*>(s.data()), s.size_bytes());
}
template<class T, std::size_t E>
inline span<_CCW_STD17::byte> as_writable_bytes(span<T, E> s) {
    return span<_CCW_STD17::byte>(reinterpret_cast<_CCW_STD17::byte*>(s.data()), s.size_bytes());
}

}   // namespace _CCW_STD20

namespace _ccw {
    using _CCW_STD20::dynamic_extent;  using _CCW_STD20::span;
    using _CCW_STD20::as_bytes;  using _CCW_STD20::as_writable_bytes;
}

#endif  // _CCW_DETAIL_SPAN_HPP
