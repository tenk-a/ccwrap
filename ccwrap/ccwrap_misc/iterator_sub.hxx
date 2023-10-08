/**
 *  @file   iterator_sub.hxx
 *  @brief  c++11 or later <iterator>'s function for c++03
 */
#ifndef ITARATOR_SUB_HXX
#define ITARATOR_SUB_HXX

#include <cstddef>

#if defined(_MSC_VER) && _MSC_VER >= 1400
# pragma warning(push)
 #pragma warning(disable:4100)
 #pragma warning(disable:4127)
 //#pragma warning(disable:4355)
#endif

namespace std {

 #if __CCWRAP_CXX < 201103L && _MSC_VER < 1700
    template <class InpIte>
    constexpr InpIte next(InpIte x, typename std::iterator_traits<InpIte>::difference_type n=1) {
        advance(x, n);
        return x;
    }

    template <class InpIte>
    constexpr InpIte prev(InpIte x, typename std::iterator_traits<InpIte>::difference_type n=1) {
        advance(x, -n);
        return x;
    }

    template <class C> constexpr typename C::iterator           begin(C& c) { return c.begin(); }
    template <class C> constexpr typename C::iterator           end(  C& c) { return c.end()  ; }
    template <class C> constexpr typename C::const_iterator     begin(C const& c) { return c.begin(); }
    template <class C> constexpr typename C::const_iterator     end(  C const& c) { return c.end()  ; }
    template <class A, size_t N> constexpr A*                   begin(A (&a)[N]) { return (A*)&a[0]; }
    template <class A, size_t N> constexpr A*                   end(  A (&a)[N]) { return (A*)&a[N]; }
    template <class A, size_t N> constexpr A const*             begin(A const (&a)[N]) { return (A*)&a[0]; }
    template <class A, size_t N> constexpr A const*             end(  A const (&a)[N]) { return (A*)&a[N]; }
 #endif

 #if __CCWRAP_CXX < 201402L && _MSC_VER < 1800
    template <class C> constexpr typename C::reverse_iterator       rbegin(C& c) { return c.rbegin(); }
    template <class C> constexpr typename C::reverse_iterator       rend(  C& c) { return c.rend()  ; }
    template <class C> constexpr typename C::const_reverse_iterator rbegin(C const& c) { return c.rbegin(); }
    template <class C> constexpr typename C::const_reverse_iterator rend(  C const& c) { return c.rend()  ; }

    template <class A, size_t N> constexpr reverse_iterator<A*> rbegin(A (&a)[N])
    	{ return reverse_iterator<A*>(&a[N]); }
    template <class A, size_t N> constexpr reverse_iterator<A*> rend(  A (&a)[N])
    	{ return reverse_iterator<A*>(&a[0]); }
    template <class A, size_t N> constexpr reverse_iterator<A const*> rbegin(A const (&a)[N])
    	{ return reverse_iterator<A const*>(&a[N]); }
    template <class A, size_t N> constexpr reverse_iterator<A const*> rend(  A const (&a)[N])
    	{ return reverse_iterator<A const*>(&a[0]); }
    template <class C> constexpr typename C::const_iterator     cbegin(C const& c) { return c.begin(); }
    template <class C> constexpr typename C::const_iterator     cend(  C const& c) { return c.end()  ; }
    template <class A, size_t N> constexpr A const*             cbegin(A const (&a)[N]) { return (A*)&a[0]; }
    template <class A, size_t N> constexpr A const*             cend(  A const (&a)[N]) { return (A*)&a[N]; }

    template <class C> constexpr typename C::const_reverse_iterator crbegin(C const& c) { return c.rbegin(); }
    template <class C> constexpr typename C::const_reverse_iterator crend(  C const& c) { return c.rend()  ; }
    template <class A, size_t N> constexpr reverse_iterator<A const*> crbegin(A const (&a)[N])
    	{ return reverse_iterator<A const*>(&a[N]); }
    template <class A, size_t N> constexpr reverse_iterator<A const*> crend(  A const (&a)[N])
    	{ return reverse_iterator<A const*>(&a[0]); }
 #endif
    //template <class E> constexpr reverse_iterator<const E*> rbegin(initializer_list<E> il);

 #if __CCWRAP_CXX < 201703L && _MSC_VER < 1900
    template <class C>           typename C::pointer        data(C& c) { return &c[0]; }
    template <class C> constexpr typename C::const_pointer  data(C const& c) { return &c[0]; }

    template <class A, size_t N> A*                         data(A (&a)[N]) { return &a[0]; }
    template <class A, size_t N> constexpr A const*         data(A const (&a)[N]) { return &a[0]; }

    template <class C          > constexpr size_t           size(C const& c) { return c.size(); }
    template <class A, size_t N> constexpr size_t           size(A       (&a)[N]) { return N; }
    template <class A, size_t N> constexpr size_t           size(A const (&a)[N]) { return N; }

    template <class C          > constexpr bool             empty(C const& c) { return c.empty(); }
    template <class A, size_t N> constexpr bool             empty(A       (&a)[N]) { return N == 0; }
    template <class A, size_t N> constexpr bool             empty(A const (&a)[N]) { return N == 0; }
 #endif

 #if __CCWRAP_CXX < 202202L
    template <class C          > constexpr ptrdiff_t        ssize(C const& c) { return ptrdiff_t(c.size()); }
    template <class A, size_t N> constexpr ptrdiff_t        ssize(A const (&a)[N]) { return ptrdiff_t(N); }
 #endif

} // std

#if defined(_MSC_VER) && _MSC_VER >= 1400
# pragma warning(pop)
#endif

#endif // ITARATOR_SUB_HXX
