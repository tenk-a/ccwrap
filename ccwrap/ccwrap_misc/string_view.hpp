/**
 *  @file   string_view
 *  @brief  string_view for c++14 or earlier
 *  @author tenka@6809.net (Masashi Kitamura)
 *  @date   2020
 *  @license Boost Software Lisence Version 1.0
 *  @note
 *      For c++11 and above this header can be used without the need for any other ccwrap source. (vc2015,vc2017)
 *
 *      (base: http://www.6809.net/tenk/html/sbr/ya_string.zip )
 *      TODO: constexpr, operator<<
 */
#ifndef __CCWRAP_STRING_VIEW_HPP_INCLUDED
#define __CCWRAP_STRING_VIEW_HPP_INCLUDED

#include <cstddef>

#include <cstring>
#include <iterator>
#include <algorithm>
#include <stdexcept>
#include <ostream>
#include <string>
//#include <cassert>

namespace std {

template< typename C, class T=std::char_traits<C> >
class basic_string_view {
    C const*        ptr_;
    std::size_t     size_;
public:
    typedef C                                       value_type;
    typedef std::size_t                             size_type;
    typedef std::ptrdiff_t                          difference_type;
    typedef C*                                      pointer;
    typedef C const*                                const_pointer;
    typedef C&                                      reference;
    typedef C const&                                const_reference;
    typedef C*                                      iterator;
    typedef C const*                                const_iterator;
    typedef std::reverse_iterator<iterator>         reverse_iterator;
    typedef std::reverse_iterator<const_iterator>   const_reverse_iterator;
 #if _CCWRAP_CXX >= 201103L //ndef __CCWRAP_NO_STATIC_CONST_INT_MEMBER_INITIALIZATION
    static constexpr size_type                      npos = size_type(-1);
 #elif __CCWRAP_NO_STATIC_CONST_INT_MEMBER_INITIALIZATION
    static const size_type                          npos;
 #else
    static const size_type                          npos = size_type(-1);
 #endif
    static C const   s_nil_[1];
public:
    basic_string_view() noexcept : ptr_(nullptr), size_(0) {}
    //basic_string_view(const value_type* cstr) : ptr_(cstr), size_(T::length(cstr)) {}
    basic_string_view(const value_type* cstr) { C const* cs = cstr ? cstr : s_nil_; ptr_ = cs; size_ = T::length(cs);}
    basic_string_view(const value_type* cary, size_type n) : ptr_(cary), size_(n) {}
    basic_string_view(const basic_string_view& str) noexcept : ptr_(str.ptr_), size_(str.size_) {}
    template<typename A> basic_string_view(const std::basic_string<C, T, A>& str) noexcept
                            : ptr_(str.data()), size_(str.length()) {}

    bool                    empty()    const noexcept { return !size_; }
    size_type               size()     const noexcept { return size_; }
    size_type               length()   const noexcept { return size_; }
    size_type               max_size() const noexcept { return size_; }
    size_type               capacity() const noexcept { return size_; }
    const_pointer           data()     const noexcept { return ptr_; }
    const_reference         front()    const { /*assert(size_ > 0);*/ return *ptr_; }
    const_reference         back()     const { /*assert(size_ > 0);*/ return ptr_[size_-1]; }
    const_iterator          begin()    const noexcept { return ptr_; }
    const_iterator          cbegin()   const noexcept { return ptr_; }
    const_iterator          end()      const noexcept { return ptr_ + size_; }
    const_iterator          cend()     const noexcept { return ptr_ + size_; }
    const_reverse_iterator  rbegin()   const noexcept { return const_reverse_iterator(end()); }
    const_reverse_iterator  crbegin()  const noexcept { return const_reverse_iterator(end()); }
    const_reverse_iterator  rend()     const noexcept { return const_reverse_iterator(begin()); }
    const_reverse_iterator  crend()    const noexcept { return const_reverse_iterator(begin()); }

    const_reference         operator[](size_type i) const { /*assert(i<size_);*/ return ptr_[i]; }
    const_reference         at(size_type i) const {if(i>=size_){throw_out_of_range();} return ptr_[i];}
    void                    swap(basic_string_view& r) noexcept { std::swap(ptr_, r.ptr_); std::swap(size_, r.size_); }
    void                    remove_prefix(size_type n) { if (n > size_) n = size_; size_ -= n; ptr_ += n; }
    void                    remove_suffix(size_type n) { size_ = (size_ > n) ? size_ - n : 0; }
    size_type               copy(value_type* cary, size_type n, size_type pos = 0) const;
    basic_string_view       substr(size_type pos = 0, size_type n = npos) const;

    int compare(const basic_string_view& str) const;
    int compare(size_type pos1, size_type n1, const basic_string_view& str) const;
    int compare(size_type pos1, size_type n1, const basic_string_view& str,size_type pos2,size_type n2) const;
    int compare(const value_type* s) const;
    int compare(const value_type* s, size_type n) const;
    int compare(size_type pos1, size_type n1, const value_type* cstr) const;
    int compare(size_type pos1, size_type n1, const value_type* cary, size_type n2) const;

    size_type find(value_type c2) const noexcept { return find_1(ptr_, 0, size_, c2); }
    size_type find(value_type c2, size_type pos1) const noexcept;
    size_type find(const basic_string_view& str2)  const noexcept;
    size_type find(const basic_string_view& str2, size_type pos1) const noexcept;
    size_type find(const value_type* cstr2) const;
    size_type find(const value_type* cstr2, size_type pos1) const;
    size_type find(const value_type* cary2, size_type pos1, size_type n2) const noexcept;

    size_type rfind(value_type c2) const noexcept { return rfind_1(ptr_, size_, c2); }
    size_type rfind(value_type c2, size_type pos1) const noexcept;
    size_type rfind(const basic_string_view& str2) const noexcept;
    size_type rfind(const basic_string_view& str2, size_type pos1) const noexcept;
    size_type rfind(const value_type* cstr2) const;
    size_type rfind(const value_type* cstr2, size_type pos1) const;
    size_type rfind(const value_type* cary2, size_type pos1, size_type n2) const noexcept;

    size_type find_first_of(value_type c2) const noexcept { return find(c2); }
    size_type find_first_of(value_type c2, size_type pos1) const noexcept { return find(c2, pos1); }
    size_type find_first_of(const basic_string_view& str2) const noexcept;
    size_type find_first_of(const basic_string_view& str2, size_type pos1) const noexcept;
    size_type find_first_of(const value_type *cstr2) const;
    size_type find_first_of(const value_type *cstr2, size_type pos1) const;
    size_type find_first_of(const value_type *cary2, size_type pos1, size_type n2) const noexcept;

    size_type find_last_of(value_type c2) const noexcept { return rfind(c2); }
    size_type find_last_of(value_type c2, size_type pos1) const noexcept { return rfind(c2, pos1); }
    size_type find_last_of(const basic_string_view& str2) const noexcept;
    size_type find_last_of(const basic_string_view& str2, size_type pos1) const noexcept;
    size_type find_last_of(const value_type *cstr2) const;
    size_type find_last_of(const value_type *cstr2, size_type pos1) const;
    size_type find_last_of(const value_type *cary2, size_type pos1, size_type n2) const noexcept;

    size_type find_first_not_of(value_type c2) const noexcept;
    size_type find_first_not_of(value_type c2, size_type pos1) const noexcept;
    size_type find_first_not_of(const basic_string_view& str2) const noexcept;
    size_type find_first_not_of(const basic_string_view& str2, size_type pos1) const noexcept;
    size_type find_first_not_of(const value_type *cstr2) const;
    size_type find_first_not_of(const value_type *cstr2, size_type pos1) const;
    size_type find_first_not_of(const value_type *cary2, size_type pos1, size_type n2) const noexcept;

    size_type find_last_not_of(value_type c2) const noexcept { return fnd_last_not_of(ptr_, size_, c2); }
    size_type find_last_not_of(value_type c2, size_type pos2) const noexcept;
    size_type find_last_not_of(const basic_string_view& str2) const noexcept;
    size_type find_last_not_of(const basic_string_view& str2, size_type pos1) const noexcept;
    size_type find_last_not_of(const value_type *cstr2) const;
    size_type find_last_not_of(const value_type *cstr2, size_type pos1) const;
    size_type find_last_not_of(const value_type *cary2, size_type pos1, size_type n2) const noexcept;

 #if 1
    template<class A> operator std::basic_string<C,T,A>() const { return std::basic_string<C,T,A>(begin(), end()); }
 #endif
private:
    basic_string_view substr_ne(std::size_t pos, std::size_t n) const noexcept;
    static void         throw_out_of_range();
    static void         check_pos_len(std::size_t size, std::size_t& pos, std::size_t& len);
    static const C*     find_1(const C* cary1, std::size_t n1, C c2) noexcept;
    static std::size_t  find_1(const C* cary1, std::size_t ofs1, std::size_t n1, C c2) noexcept;
    static std::size_t  find_str(const C* cary1, std::size_t ofs1, std::size_t n1, const C* cary2, std::size_t n2) noexcept;
    static std::size_t  rfind_1(const C* cary1, std::size_t n1, C c2) noexcept;
    static std::size_t  rfind_str(const C* cary1, std::size_t n1, const C* cary2, std::size_t n2) noexcept;
    static std::size_t  fnd_first_of(const C* cary1, std::size_t ofs1, std::size_t n1, const C* cary2, std::size_t n2) noexcept;
    static std::size_t  fnd_last_of(const C* cary1, std::size_t n1, const C* cary2, std::size_t n2) noexcept;
    static std::size_t  fnd_first_not_of(const C* cary1, std::size_t ofs1, std::size_t n1, C c2) noexcept;
    static std::size_t  fnd_first_not_of(const C* cary1, std::size_t ofs1, std::size_t n1, const C* cary2, std::size_t n2) noexcept;
    static std::size_t  fnd_last_not_of(const C* cary1, std::size_t n1, C c2) noexcept;
    static std::size_t  fnd_last_not_of(const C* cary1, std::size_t n1, const C* cary2, std::size_t n2) noexcept;
};

template< typename C, class T >
C const basic_string_view<C,T>::s_nil_[1] = {0};

#if _CCWRAP_CXX < 201103L //ndef __CCWRAP_NO_STATIC_CONST_INT_MEMBER_INITIALIZATION
#ifdef __CCWRAP_NO_STATIC_CONST_INT_MEMBER_INITIALIZATION
template< typename C, class T >
const typename basic_string_view<C,T>::size_type basic_string_view<C,T>::npos = basic_string_view<C,T>::size_type(-1);
#else
template< typename C, class T >
const typename basic_string_view<C,T>::size_type basic_string_view<C,T>::npos;
#endif
#endif

typedef basic_string_view< char   ,  std::char_traits<char   > > string_view;
typedef basic_string_view< wchar_t,  std::char_traits<wchar_t> > wstring_view;
typedef basic_string_view< char16_t, std::char_traits<char16_t> > u16string_view;
typedef basic_string_view< char32_t, std::char_traits<char32_t> > u32string_view;
#if defined(__cpp_char8_t) || defined(__CCWRAP_HAS_CHAR8_T)
typedef basic_string_view< char8_t, std::char_traits<char8_t> >   u8string_view;
#endif

template< typename C, class T >
basic_string_view<C,T> basic_string_view<C,T>::substr_ne(std::size_t pos, std::size_t n) const noexcept {
    std::size_t l = size_;
    if (pos > l) pos = l;
    if (n > l - pos) n = l - pos;
    return basic_string_view<C,T>(ptr_+pos,n);
}


// -------------------------------

template< typename C, class T >
int basic_string_view<C,T>::compare(const basic_string_view<C,T>& str2) const {
    std::size_t l1 = size_;
    std::size_t l2 = str2.size_;
    difference_type d  = difference_type(l1 - l2);
    if (d > 0)
        l1 = l2;
    int c = 0;
    if (l1 > 0)
        c = T::compare(ptr_, str2.ptr_, l1);
    if (c == 0)
        c = (sizeof(c) == sizeof(d)) ? int(d) : (d < 0) ? -1 : (d > 0) ? 1 : 0;
    return c;
}

template< typename C, class T >
int basic_string_view<C,T>::compare(std::size_t pos1, std::size_t n1, const basic_string_view<C,T>& str2) const {
 #if 0
    return substr_ne(pos1, n1).compare(str2);
 #else
    std::size_t l1 = size_;
    std::size_t l2 = str2.size_;
    if (pos1 > l1)
        pos1 = l1;
    if (n1 > l1 - pos1)
        n1 = l1 - pos1;
    difference_type d = difference_type (n1 - l2);
    if (d > 0)
        n1 = l2;
    int c = 0;
    if (n1 > 0)
        c = T::compare(ptr_ + pos1, str2.ptr_, n1);
    if (c == 0)
        c = (sizeof(c) == sizeof(d)) ? int(d) : (d < 0) ? -1 : (d > 0) ? 1 : 0;
    return c;
 #endif
}

template< typename C, class T >
int basic_string_view<C,T>::compare(std::size_t pos1, std::size_t n1
        , const basic_string_view<C,T>& str2, std::size_t pos2, std::size_t n2) const
{
 #if 0
    return substr_ne(pos1, n1).compare(str2.substr(pos2, n2));
 #else
    std::size_t l1 = size_;
    std::size_t l2 = str2.size_;
    if (pos1 > l1)
        pos1 = l1;
    if (n1 > l1 - pos1)
        n1 = l1 - pos1;
    if (pos2 > l2)
        pos2 = l2;
    if (n2 > l2 - pos2)
        n2 = l2 - pos2;
    int d = int (n1 - n2);
    if (d > 0)
        n1 = n2;
    int c = 0;
    if (n1 > 0)
        c = T::compare(ptr_ + pos1, str2.ptr_ + pos2, n1);
    if (c == 0)
        c = (sizeof(c) == sizeof(d)) ? int(d) : (d < 0) ? -1 : (d > 0) ? 1 : 0;
    return c;
 #endif
}

template< typename C, class T >
int basic_string_view<C,T>::compare(const C* cstr2) const {
 #if 0
    return compare(basic_string_view<C,T>(cstr2));
 #else
    if (!cstr2) {
        throw_out_of_range();
        static C const s_nil[1] = { 0 };
        cstr2 = s_nil;
    }
    std::size_t l1 = size_;
    std::size_t l2 = T::length(cstr2);
    difference_type d  = difference_type(l1 - l2);
    if (d > 0)
        l1 = l2;
    int c = 0;
    if (l1 > 0)
        c = T::compare(ptr_, cstr2, l1);
    if (c == 0)
        c = (sizeof(c) == sizeof(d)) ? int(d) : (d < 0) ? -1 : (d > 0) ? 1 : 0;
    return c;
 #endif
}

template< typename C, class T >
int basic_string_view<C,T>::compare(const C* cary2, std::size_t n2) const {
 #if 0
    return compare(basic_string_view<C,T>(cary2, n2));
 #else
    if (!cary2) {
        throw_out_of_range();
        cary2 = "";
    }
    std::size_t n1 = size_;
    difference_type d  = difference_type(n1 - n2);
    if (d > 0)
        n1 = n2;
    int c = 0;
    if (n1 > 0)
        c = T::compare(ptr_, cary2, n1);
    if (c == 0)
        c = (sizeof(c) == sizeof(d)) ? int(d) : (d < 0) ? -1 : (d > 0) ? 1 : 0;
    return c;
 #endif
}

template< typename C, class T >
int basic_string_view<C,T>::compare(std::size_t pos1, std::size_t n1, const C* cstr2) const {
 #if 0
    return substr_ne(pos1, n1).compare(basic_string_view<C,T>(cstr2));
 #else
    std::size_t l1 = size_;
    std::size_t n2 = T::length(cstr2);
    if (pos1 > l1)
        pos1 = l1;
    if (n1 > l1 - pos1)
        n1 = l1 - pos1;
    difference_type d = difference_type(n1 - n2);
    if (d > 0)
        n1 = n2;
    int c = 0;
    if (n1 > 0)
        c = T::compare(ptr_ + pos1, cstr2, n1);
    if (c == 0)
        c = (sizeof(c) == sizeof(d)) ? int(d) : (d < 0) ? -1 : (d > 0) ? 1 : 0;
    return c;
 #endif
}

template< typename C, class T >
int basic_string_view<C,T>::compare(std::size_t pos1, std::size_t n1, const C* cary2, std::size_t n2) const {
 #if 0
    return substr_ne(pos1, n1).compare(basic_string_view<C,T>(cary2, n2));
 #else
    std::size_t l1 = size_;
    if (pos1 > l1)
        pos1 = l1;
    if (n1 > l1 - pos1)
        n1 = l1 - pos1;
    difference_type d = difference_type(n1 - n2);
    if (d > 0)
        n1 = n2;
    int c = 0;
    if (n1 > 0)
        c = T::compare(ptr_ + pos1, cary2, n1);
    if (c == 0) {
        c = (sizeof(c) == sizeof(d)) ? int(d) : (d < 0) ? -1 : (d > 0) ? 1 : 0;
    }
    return c;
 #endif
}


// -------------------------------

template<typename C, class T> std::size_t
basic_string_view<C,T>::find(C c2, std::size_t pos1) const noexcept {
    std::size_t l1 = size_;
    if (pos1 > l1)
        pos1 = l1;
    l1 -= pos1;
    return find_1(ptr_, pos1, l1, c2);
}

template<typename C, class T> inline std::size_t
basic_string_view<C,T>::find(const basic_string_view<C,T>& str2) const noexcept {
    //x return find(str2.ptr_, 0, str2.size_);
    return find_str( ptr_, 0, size_, str2.ptr_, str2.size_ );
}

template<typename C, class T> inline std::size_t
basic_string_view<C,T>::find(const basic_string_view<C,T>& str2, std::size_t pos1) const noexcept {
    return find(str2.ptr_, pos1, str2.size_);
}

template<typename C, class T> std::size_t
basic_string_view<C,T>::find(const C* cstr2) const {
    std::size_t l2 = cstr2 ? T::length(cstr2) : 0;
    //x return find(cstr2, 0, l2);
    return find_str(ptr_, 0, size_, cstr2, l2);
}

template<typename C, class T> std::size_t
basic_string_view<C,T>::find(const C* cstr2, std::size_t pos1) const {
    std::size_t l2 = cstr2 ? T::length(cstr2) : 0;
    return find(cstr2, pos1, l2);
}

template<typename C, class T> std::size_t
basic_string_view<C,T>::find(const C* cary2, std::size_t pos1, std::size_t n2) const noexcept {
    std::size_t     n1 = size_;
    if (pos1 > n1)
        return npos;    // pos1 = n1;
    n1 -= pos1;
    return find_str(ptr_, pos1, n1, cary2, n2);
}

// [private]
template<typename C, class T> std::size_t
basic_string_view<C,T>::find_str(
        const C* cary1, std::size_t ofs1, std::size_t n1,
        const C* cary2,                   std::size_t n2) noexcept
{
    if (ofs1 > n1)
        return npos;    // pos1 = n1;

    if (n2 == 0 || cary2 == nullptr)
        return ofs1;

    if (n1 == 0)
        return npos;

    if (n2 > n1)
        return npos;

    if (n2 == 1)
        return find_1(cary1, ofs1, n1, cary2[0]);

    const C*    src1     = cary1+ofs1;
    const C*    cary1end = src1 + n1 - n2;
    const C*    key2     = cary2;
    const C*    key2end  = key2 + n2;
    C           k2       = *key2++;

  RETRY:
    while (src1 <= cary1end) {
        C   c = *src1++;
        if (T::eq(c,k2)) {
            const C* s1  = src1;
            const C* kp2 = key2;
            C        k;
            // if (k2 != key2end)       // keyE - key >= 1
            {
                do {
                    //x if (t == se) return npos;
                    c = *s1++;
                    k = *kp2++;
                    if (!T::eq(c,k))
                        goto RETRY;
                } while (kp2 != key2end);
            }
            return src1 - 1 - cary1;
        }
    }
    return npos;
}

// [private]
template<typename C, class T> std::size_t
basic_string_view<C,T>::find_1(const C* cary1, std::size_t ofs1, std::size_t n1, C c2) noexcept {
    if (n1 == 0 || cary1 == nullptr)
        return npos;
    const C*    p1 = find_1(cary1+ofs1, n1, c2);
    std::size_t n  = npos;
    if (p1)
        n = p1 - cary1;
    return n;
}

// same char_traits<C>::find(src, n, key)
template<typename C, class T> const C*
basic_string_view<C,T>::find_1(const C* src, std::size_t n, C key) noexcept {
    if (n && src) {
        const C *s = (const C *)src;
        const C *e = s + n;
        const C  k = key;
        do {
            if (T::eq(*s, k))
                return s;
            ++s;
        } while (s != e);
    }
    return nullptr;
}

//-----------------------

template<typename C, class T> std::size_t
basic_string_view<C,T>::rfind(C c2, std::size_t pos1) const noexcept {
    if (pos1 < npos)
        ++pos1;
    std::size_t l1 = size_;
    if (l1 > pos1)
        l1 = pos1;
    return rfind_1(ptr_, l1, c2);
}

template<typename C, class T> inline std::size_t
basic_string_view<C,T>::rfind(const basic_string_view<C,T>& str2) const noexcept {
    return rfind_str( ptr_, size_, str2.ptr_, str2.size_ );
}

template<typename C, class T> inline std::size_t
basic_string_view<C,T>::rfind(const basic_string_view<C,T>& str2, std::size_t pos1) const noexcept {
    return rfind(str2.ptr_, pos1, str2.size_);
}

template<typename C, class T> std::size_t
basic_string_view<C,T>::rfind(const C* cstr2) const {
    std::size_t l2 = cstr2 ? T::length(cstr2) : 0;
    return rfind_str(ptr_, size_, cstr2, l2);
}

template<typename C, class T> std::size_t
basic_string_view<C,T>::rfind(const C* cstr2, std::size_t pos1) const {
    std::size_t l2 = cstr2 ? T::length(cstr2) : 0;
    return rfind(cstr2, pos1, l2);
}

template<typename C, class T> std::size_t
basic_string_view<C,T>::rfind(const C* cary2, std::size_t pos1, std::size_t n2) const noexcept {
    if (pos1 < npos)
        ++pos1;
    std::size_t     n1 = size_;
    if (n1 > pos1)
        n1 = pos1;
    return rfind_str(ptr_, n1, cary2, n2);
}

// [private]
template<typename C, class T> std::size_t
basic_string_view<C,T>::rfind_str(const C* cary1, std::size_t n1, const C* cary2, std::size_t n2) noexcept {
    if (n2 == 0 || cary2 == nullptr)
        return 0;

    if (n1 == 0)
        return npos;

    if (n2 > n1)
        return npos;

    if (n2 == 1)
        return rfind_1(cary1, n1, cary2[0]);

    //const C*  cary1end = cary1 + n1 - n2;
    const C*    cary1end = cary1 + n1; // - n2;
    const C*    src1     = cary1end;
    const C*    key2     = cary2;
    const C*    key2end  = key2 + n2;
    C           k2       = *key2++;

  RETRY:
    while (--src1 >= cary1) {
        C c = *src1;
        if (T::eq(c, k2)) {
            const C* s1  = src1+1;
            const C* kp2 = key2;
            C        k;
            // if (k2 != key2end)       // keyE - key >= 1
            {
                do {
                    //x if (t == se) return npos;
                    c = *s1++;
                    k = *kp2++;
                    if (!T::eq(c,k))
                        goto RETRY;
                } while (kp2 != key2end);
            }
            return src1 - cary1;
        }
    }
    return npos;
}

// [private]
template<typename C, class T> std::size_t
basic_string_view<C,T>::rfind_1(const C* cary1, std::size_t n1, C c2) noexcept {
    if (n1 == 0 || cary1 == 0)
        return npos;
    const C*    s1 = cary1 + n1;
    do {
        --s1;
        if (T::eq(*s1, c2))
            return s1 - cary1;
    } while (s1 >= cary1);
    return npos;
}


//------------------------

template<typename C, class T> std::size_t
basic_string_view<C,T>::find_first_of(const C* cary2, std::size_t pos1, std::size_t n2) const noexcept {
    std::size_t     n1 = size_;
    if (pos1 > n1)
        return npos;    // pos1 = n1;
    n1 -= pos1;
    return fnd_first_of(ptr_, pos1, n1, cary2, n2);
}

template<typename C, class T> inline std::size_t
basic_string_view<C,T>::find_first_of(const basic_string_view<C,T>& str2) const noexcept {
    return fnd_first_of(ptr_, 0U, size_, str2.ptr_, str2.size_);
}

template<typename C, class T> inline std::size_t
basic_string_view<C,T>::find_first_of(const basic_string_view<C,T>& str2, std::size_t pos1) const noexcept {
    return find_first_of(str2.ptr_, pos1, str2.size_);
}

template<typename C, class T> std::size_t
basic_string_view<C,T>::find_first_of(const C* cstr2) const {
    std::size_t     n2 = cstr2 ? T::length(cstr2) : 0;
    std::size_t     n1 = size_;
    return fnd_first_of(ptr_, 0, n1, cstr2, n2);
}

template<typename C, class T> std::size_t
basic_string_view<C,T>::find_first_of(const C* cstr2, std::size_t pos1) const {
    std::size_t     n2 = cstr2 ? T::length(cstr2) : 0;
    return find_first_of( cstr2, pos1, n2 );
}

// [private]
template<typename C, class T> std::size_t
basic_string_view<C,T>::fnd_first_of(
        const C* cary1, std::size_t ofs1, std::size_t n1,
        const C* cary2,                   std::size_t n2) noexcept
{
    if (n2 == 0 || cary2 == nullptr)
        return npos;

    if (n1 == 0)
        return npos;

    if (n2 == 1)
        return find_1(cary1, ofs1, n1, cary2[0]);

    const C*    s1 = cary1+ofs1;
    do {
        C        c1 = *s1++;
        const C* p  = find_1(cary2, n2, c1);
        if (p)
            return  s1-1-cary1;
    } while (--n1);
    return npos;
}


//------------------------

template<typename C, class T> std::size_t
basic_string_view<C,T>::find_last_of(const C* cary2, std::size_t pos1, std::size_t n2) const noexcept {
    if (pos1 < npos)
        ++pos1;
    std::size_t     n1 = size_;
    if (n1 > pos1)
        n1 = pos1;
    //n1 -= pos1;
    return fnd_last_of(ptr_, n1, cary2, n2);
}

template<typename C, class T> inline std::size_t
basic_string_view<C,T>::find_last_of(const basic_string_view<C,T>& str2) const noexcept {
    return fnd_last_of(ptr_, size_, str2.ptr_, str2.size_);
}

template<typename C, class T> inline std::size_t
basic_string_view<C,T>::find_last_of(const basic_string_view<C,T>& str2, std::size_t pos1) const noexcept {
    return find_last_of(str2.ptr_, pos1, str2.size_);
}

template<typename C, class T>
std::size_t     basic_string_view<C,T>::find_last_of(const C* cstr2) const {
    std::size_t     n2 = cstr2 ? T::length(cstr2) : 0;
    return fnd_last_of(ptr_, size_, cstr2, n2);
}

template<typename C, class T> std::size_t
basic_string_view<C,T>::find_last_of(const C* cstr2, std::size_t pos1) const {
    std::size_t     n2 = cstr2 ? T::length(cstr2) : 0;
    return find_last_of( cstr2, pos1, n2 );
}

// [private]
template<typename C, class T> std::size_t
basic_string_view<C,T>::fnd_last_of(
        const C*    cary1, std::size_t n1,
        const C*    cary2, std::size_t n2) noexcept
{
    if (n2 == 0 || cary2 == nullptr)
        return npos;

    if (n1 == 0)
        return npos;

    if (n2 == 1)
        return rfind_1(cary1, n1, cary2[0]);

    const C*    s1  = cary1+n1;
    do {
        C        c1 = *--s1;
        const C* p  = find_1(cary2, n2, c1);
        if (p)
            return  s1-cary1;
    } while (s1 > cary1);
    return npos;
}


//------------------------

template<typename C, class T> inline std::size_t
basic_string_view<C,T>::find_first_not_of(C c) const noexcept {
    return fnd_first_not_of(ptr_, 0, size_, c);
}

template<typename C, class T> std::size_t
basic_string_view<C,T>::find_first_not_of(C c2, std::size_t pos1) const noexcept {
    std::size_t n1 = size_;
    if (pos1 > n1)
        return npos;    //x pos1 = l1;
    n1 -= npos;
    return fnd_first_not_of(ptr_, pos1, n1, c2);
}

template<typename C, class T> std::size_t
basic_string_view<C,T>::find_first_not_of(const C* cary2, std::size_t pos1, std::size_t n2) const noexcept {
    std::size_t     n1 = size_;
    if (pos1 > n1)
        return npos;    // pos1 = n1;
    n1 -= pos1;
    return fnd_first_not_of(ptr_, pos1, n1, cary2, n2);
}

template<typename C, class T> inline std::size_t
basic_string_view<C,T>::find_first_not_of(const basic_string_view<C,T>& str2) const noexcept {
    return fnd_first_not_of(ptr_, 0U, size_, str2.ptr_, str2.size_);
}

template<typename C, class T> inline std::size_t
basic_string_view<C,T>::find_first_not_of(const basic_string_view<C,T>& str2, std::size_t pos1) const noexcept {
    return find_first_not_of(str2.ptr_, pos1, str2.size_);
}

template<typename C, class T>
std::size_t     basic_string_view<C,T>::find_first_not_of(const C* cstr2) const {
    std::size_t     n2 = cstr2 ? T::length(cstr2) : 0;
    std::size_t     n1 = size_;
    return fnd_first_not_of(ptr_, 0, n1, cstr2, n2);
}

template<typename C, class T>
std::size_t     basic_string_view<C,T>::find_first_not_of(const C* cstr2, std::size_t pos1) const {
    std::size_t     n2 = cstr2 ? T::length(cstr2) : 0;
    return find_first_not_of( cstr2, pos1, n2 );
}

// [private]
template<typename C, class T> std::size_t
basic_string_view<C,T>::fnd_first_not_of(
        const C*    cary1, std::size_t ofs1, std::size_t n1,
        const C*    cary2,                   std::size_t n2) noexcept
{
    if (n2 == 0 || cary2 == nullptr)
        return ofs1;

    if (n1 == 0)
        return npos;

    if (n2 == 1)
        return fnd_first_not_of(cary1, ofs1, n1, cary2[0]);

    const C*    s1 = cary1+ofs1;
    do {
        C        c1 = *s1++;
        const C* p  = find_1(cary2, n2, c1);
        if (p == nullptr)
            return  s1-1-cary1;
    } while (--n1);
    return npos;
}

// [private]
template<typename C, class T> std::size_t
basic_string_view<C,T>::fnd_first_not_of(const C* cary1, std::size_t ofs1, std::size_t n1, C c2) noexcept {
    if (cary1 == 0 || n1 == 0)
        return npos;
    const C* s = cary1 + ofs1;
    const C* e = s + n1;
    do {
        if (!T::eq(*s,c2))
            return s - cary1;
        ++s;
    } while (s != e);
    return npos;
}


//------------------------

template<typename C, class T> std::size_t
basic_string_view<C,T>::find_last_not_of(C c2, std::size_t pos1) const noexcept {
    if (pos1 < npos)
        ++pos1;
    std::size_t     n1 = size_;
    if (n1 > pos1)
        n1 = pos1;
    return fnd_last_not_of(ptr_, n1, c2);
}

template<typename C, class T> std::size_t
basic_string_view<C,T>::find_last_not_of(const C* cary2, std::size_t pos1, std::size_t n2) const noexcept {
    if (pos1 < npos)
        ++pos1;
    std::size_t     n1 = size_;
    if (n1 > pos1)
        n1 = pos1;
    return fnd_last_not_of(ptr_, n1, cary2, n2);
}

template<typename C, class T> inline std::size_t
basic_string_view<C,T>::find_last_not_of(const basic_string_view<C,T>& str2) const noexcept {
    return fnd_last_not_of(ptr_, size_, str2.ptr_, str2.size_);
}

template<typename C, class T> inline std::size_t
basic_string_view<C,T>::find_last_not_of(const basic_string_view<C,T>& str2, std::size_t pos1) const noexcept {
    return find_last_not_of(str2.ptr_, pos1, str2.size_);
}

template<typename C, class T> std::size_t
basic_string_view<C,T>::find_last_not_of(const C* cstr2) const {
    std::size_t     n2 = cstr2 ? T::length(cstr2) : 0;
    std::size_t     n1 = size_;
    return fnd_last_not_of(ptr_, n1, cstr2, n2);
}

template<typename C, class T> std::size_t
basic_string_view<C,T>::find_last_not_of(const C* cstr2, std::size_t pos1) const {
    std::size_t     n2 = cstr2 ? T::length(cstr2) : 0;
    return find_last_not_of( cstr2, pos1, n2 );
}

// [private]
template<typename C, class T> std::size_t
basic_string_view<C,T>::fnd_last_not_of(
        const C*    cary1, std::size_t n1,
        const C*    cary2, std::size_t n2) noexcept
{
    if (n2 == 0 || cary2 == nullptr)
        return n1-1;    //npos;

    if (n1 == 0)
        return npos;

    if (n2 == 1)
        return fnd_last_not_of(cary1, n1, cary2[0]);

    const C*        s1 = cary1+n1;
    do {
        C           c1 = *--s1;
        const C*    p  = find_1(cary2, n2, c1);
        if (p == nullptr)
            return  s1-cary1;
    } while (--n1);
    return npos;
}

// [private]
template<typename C, class T> std::size_t
basic_string_view<C,T>::fnd_last_not_of(const C* cary1, std::size_t n1, C c2) noexcept {
    if (cary1 == 0 || n1 == 0)
        return npos;
    const C* s = cary1 + n1;
    do {
        --s;
        if (!T::eq(*s, c2))
            return s - cary1;
    } while (s != cary1);
    return npos;
}

// ---------------

template< typename C, class T > std::size_t
basic_string_view<C,T>::copy(C* ary, std::size_t n, std::size_t pos) const {
    check_pos_len(size_, pos, n);
    if (n) {
        if (ary == 0)
            return 0;
        T::copy(ary, ptr_+pos, n);
    }
    return n;
}

template< typename C, class T > basic_string_view<C,T>
basic_string_view<C,T>::substr(std::size_t pos, std::size_t n) const {
    check_pos_len(size_, pos, n);
    return basic_string_view<C,T>( ptr_ + pos, n );
}

// [private]
template< typename C, class T > inline
void basic_string_view<C,T>::check_pos_len(std::size_t l, std::size_t& pos, std::size_t& len) {
    if (pos > l) {
        throw_out_of_range();
        pos = l;
    }
    if (len > l - pos)
        len = l - pos;
}

// [private]
template< typename C, class T > inline
void basic_string_view<C,T>::throw_out_of_range() {
    throw std::out_of_range("std::basic_string_view");
}


#undef  __CCWRAP_SV 
#undef  __CCWRAP_CS 
#undef  __CCWRAP_STRING_VIEW_OP 
#define __CCWRAP_SV     const basic_string_view<C,T>&
#define __CCWRAP_CS     const C*
#define __CCWRAP_STRING_VIEW_OP(_op,_op2)                       \
template<typename C, class T>                                   \
inline bool operator _op(__CCWRAP_SV lhs, __CCWRAP_SV rhs) {    \
    return lhs.compare(rhs) _op 0;                              \
}                                                               \
template<typename C, class T>                                   \
inline bool operator _op(__CCWRAP_SV lhs, __CCWRAP_CS rhs) {    \
    return lhs.compare(rhs) _op 0;                              \
}                                                               \
template<typename C, class T>                                   \
inline bool operator _op(__CCWRAP_CS lhs, __CCWRAP_SV rhs) {    \
    return rhs.compare(lhs) _op2 0;                             \
}

__CCWRAP_STRING_VIEW_OP(==,==);
__CCWRAP_STRING_VIEW_OP(!=,!=);
__CCWRAP_STRING_VIEW_OP(<,>);
__CCWRAP_STRING_VIEW_OP(>,<);
__CCWRAP_STRING_VIEW_OP(<=,>=);
__CCWRAP_STRING_VIEW_OP(>=,<=);

#undef __CCWRAP_SV
#undef __CCWRAP_CS
#undef __CCWRAP_STRING_VIEW_OP


}   // std

//	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-

#if !defined(__WATCOMC__)
// ostream << basic_string_view
template<typename C, class T>
std::basic_ostream<C,T>& operator<<(std::basic_ostream<C,T>& os, const std::basic_string_view<C,T>& str) {
    os << std::basic_string<C>(str.data(), str.size());
    return os;
}
#else
// ostream << basic_string_view
inline std::ostream& operator<<(std::ostream& os, const std::string_view& str) {
    os << std::string(str.data(), str.size());
    return os;
}
#endif

#endif      //  CCWRAP_STRING_VIEW_HPP_INCLUDED
