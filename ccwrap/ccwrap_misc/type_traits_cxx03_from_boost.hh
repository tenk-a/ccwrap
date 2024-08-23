#ifndef __CCWRAP_TYPE_TRAITS_CXX03_FROM_BOOST_HH_INCLUDED
#define __CCWRAP_TYPE_TRAITS_CXX03_FROM_BOOST_HH_INCLUDED

// (boost/type_traits/alignment_of.hpp)
//
//  (C) Copyright John Maddock 2000.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

template <class T> struct alignment_of;

// get the alignment of some arbitrary type:
namespace detail {
    template <class T>
    struct alignment_of_hack {
        char c;
        T t;
        alignment_of_hack();
    };
    template <unsigned A, unsigned S>
    struct alignment_logic {
        static const unsigned value = A < S ? A : S;
    };
    template< class T >
    struct alignment_of_impl {
        static const unsigned value = alignment_logic< sizeof(alignment_of_hack<T>) - sizeof(T), sizeof(T) >::value;
    };
} // namespace detail

template< class T >
struct alignment_of : public integral_constant<unsigned, detail::alignment_of_impl<T>::value> {};

// references have to be treated specially, assume
// that a reference is just a special pointer:
#if 1 //ndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
template <class T>
struct alignment_of<T&> : public alignment_of<T*> {};
#endif

#if defined(__BORLANDC__) || defined(__DMC__)
// long double gives an incorrect value of 10 (!)
// unless we do this...
struct long_double_wrapper{ long double ld; };
template<> struct alignment_of<long double> : public alignment_of<long_double_wrapper> {};
template<> struct alignment_of<long double const> : public alignment_of<long_double_wrapper> {};
template<> struct alignment_of<long double volatile> : public alignment_of<long_double_wrapper> {};
template<> struct alignment_of<long double const volatile> : public alignment_of<long_double_wrapper> {};
#endif

template<> struct alignment_of<void> : public integral_constant<unsigned,0> {};
template<> struct alignment_of<void const> : public integral_constant<unsigned,0> {};
template<> struct alignment_of<void volatile> : public integral_constant<unsigned,0> {};
template<> struct alignment_of<void const volatile> : public integral_constant<unsigned,0> {};

}   // namespace std

#endif  // __CCWRAP_TYPE_TRAITS_CXX03_FROM_BOOST_HH_INCLUDED
