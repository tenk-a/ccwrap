/**
 *  @file   array
 *  @brief  std::array for c++03
 *  @author tenka@6809.net (Masashi Kitamura)
 *  @date   2003-07-19,2010,2020
 *  @license Boost Software License Version 1.0
 *  @note
 *      (base: http://www.6809.net/tenk/html/sbr/ya_array.h.html )
 */
#ifndef __CCWRAP_ARRAY_HPP_INCLUDED
#define __CCWRAP_ARRAY_HPP_INCLUDED

#include <cstddef>

#include <iterator>
#include <algorithm>
#include <stdexcept>
//#include <cassert>

namespace std {

template<class T, std::size_t N>
struct array {
    T       elements_[N];

public:
    static_assert(N > 0, "");
    typedef std::size_t     size_type;
    typedef std::ptrdiff_t  difference_type;
    typedef T               value_type;
    typedef T&              reference;
    typedef const T&        const_reference;
    typedef T*              pointer;
    typedef const T*        const_pointer;
    typedef T*              iterator;
    typedef const T*        const_iterator;
    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    size_type       size()          const   { return N; }
    size_type       max_size()      const   { return N; }
    bool            empty()         const   { return 0; }

    reference       front()                 { return *elements_;}
    const_reference front()         const   { return *elements_;}
    reference       back()                  { return *(elements_ + N - 1); }
    const_reference back()          const   { return *(elements_ + N - 1); }
    reference       at(size_type n)         { if (n >= N) { outOfRng(); n = N-1; } return *(elements_ + n); }
    const_reference at(size_type n) const   { if (n >= N) { outOfRng(); n = N-1; } return *(elements_ + n); }
    reference       operator[](size_type n)       { /*assert(n < N );*/ return *(elements_ + n);}
    const_reference operator[](size_type n) const { /*assert(n < N );*/ return *(elements_ + n);}

    const_pointer           data()    const noexcept { return elements_;}
    iterator                begin()         noexcept { return elements_;}
    const_iterator          begin()   const noexcept { return elements_;}
    const_iterator          cbegin()  const noexcept { return elements_;}
    iterator                end()           noexcept { return elements_ + N;}
    const_iterator          end()     const noexcept { return elements_ + N;}
    const_iterator          cend()    const noexcept { return elements_ + N;}
    reverse_iterator        rbegin()        noexcept { return reverse_iterator(elements_ + N);}
    const_reverse_iterator  rbegin()  const noexcept { return const_reverse_iterator(elements_ + N);}
    const_reverse_iterator  crbegin() const noexcept { return const_reverse_iterator(elements_ + N);}
    reverse_iterator        rend()          noexcept { return reverse_iterator(elements_); }
    const_reverse_iterator  rend()    const noexcept { return const_reverse_iterator(elements_);}
    const_reverse_iterator  crend()   const noexcept { return const_reverse_iterator(elements_);}

    void swap(array& x) noexcept { for (size_type i = 0; i < N; i++) std::swap(elements_[i], x[i]); }
    void fill(const T& t) { std::fill_n(begin(), N, t); }

    bool operator==(const array& r) const { return std::equal(begin(), end(), r.begin()); }
    bool operator!=(const array& r) const { return !(*this == r); }
    bool operator< (const array& r) const { return std::lexicographical_compare(begin(),end(),r.begin(),r.end());}
    bool operator>=(const array& r) const { return  !(*this < r); }
    bool operator> (const array& r) const { return    r < *this ; }
    bool operator<=(const array& r) const { return ! (r < *this); }

	friend void swap(array& l, array& r) {
	    l.swap(r);
	}

private:
    void outOfRng() const {
     #if 1
        throw std::out_of_range("invalid array<T,N> subscript");
     #else
        assert(0 && "invalid array<T,N> subscript\n");
     #endif
    }
};

#if 0
template<class T, size_t N>
void swap(array<T,N>& l, array<T,N>& r) {
    l.swap(r);
}
#endif

#if 1
template<class T, size_t N>
array<T, N> to_array(T (&a)[N]) {
    return *(array<T, N>*)(&a[0]);
}

#if 0
template<class T, size_t N>
array<T, N> to_array(T (&&a)[N]) {
    return std::move(*(array<T, N>*)(&a[0]));
}
#endif
#else   // TODO
template<class T, size_t N>
constexpr array<remove_cv_t<T>, N> to_array(T (&a)[N]) {
    return *(array<remove_cv_t<T>, N>*)(&a[0]);
}

template<class T, size_t N>
constexpr array<remove_cv_t<T>, N> to_array(T (&&a)[N]) {
    return std::move(*(array<remove_cv_t<T>, N>*)(&a[0]));
}
#endif

}   // std


#endif  // CCWRAP_ARRAY_INCLUDED
