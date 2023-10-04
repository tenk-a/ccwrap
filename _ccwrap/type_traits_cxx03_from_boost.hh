#ifndef __CCWRAP_TYPE_TRAITS_CXX03_FROM_BOOST_HH_INCLUDED
#define __CCWRAP_TYPE_TRAITS_CXX03_FROM_BOOST_HH_INCLUDED

// (boost/type_traits/make_signed.hpp)
//
//  (C) Copyright John Maddock 2007.
//
//  (modified for ccwrap by Masashi Kitamura  2014.)
//
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

namespace ccwrap {

// make_signed
namespace detail {
    template <class T>
    struct make_signed_imp {
//      static_assert( is_integral<T>::value || is_enum<T>::value ,"");
//      static_assert(!(is_same<typename remove_cv<T>::type, bool>::value),"");

       typedef typename remove_cv<T>::type t_no_cv;

       typedef typename conditional<
          (  is_signed<T>::value
          && is_integral<T>::value
          && !is_same<t_no_cv, char>::value
          && !is_same<t_no_cv, wchar_t>::value
          && !is_same<t_no_cv, bool>::value),
          T,
          typename conditional<
             ( is_integral<T>::value
             && !is_same<t_no_cv, char>::value
             && !is_same<t_no_cv, wchar_t>::value
             && !is_same<t_no_cv, bool>::value),
             typename conditional<
                is_same<t_no_cv, unsigned char>::value,
                signed char,
                typename conditional<
                   is_same<t_no_cv, unsigned short>::value,
                   short,
                   typename conditional<
                      is_same<t_no_cv, unsigned int>::value,
                      int,
                      typename conditional<
                         is_same<t_no_cv, unsigned long>::value,
                         long,
                        #ifdef __CCWRAP_HAS_INT128
                         typename conditional<
                            (sizeof(t_no_cv) == sizeof(__ccwrap_llong)),
                            __ccwrap_llong,
                            __ccwrap_int128
                         >::type
                        #else
                         __ccwrap_llong
                        #endif
                      >::type
                   >::type
                >::type
             >::type,
             // Not a regular integer type:
             typename conditional<
                sizeof(t_no_cv) == sizeof(unsigned char),
                signed char,
                typename conditional<
                   sizeof(t_no_cv) == sizeof(unsigned short),
                   short,
                   typename conditional<
                      sizeof(t_no_cv) == sizeof(unsigned int),
                      int,
                      typename conditional<
                         sizeof(t_no_cv) == sizeof(unsigned long),
                         long,
                        #ifdef __CCWRAP_HAS_INT128
                         typename conditinal<
                            sizeof(t_no_cv) == sizeof(__ccwrap_ullong),
                            __ccwrap_llong,
                            __ccwrap_int128
                         >::type
                        #else
                         __ccwrap_llong
                        #endif
                      >::type
                   >::type
                >::type
             >::type
          >::type
       >::type base_integer_type;

       // Add back any const qualifier:
       typedef typename conditional<
          is_const<T>::value,
          typename add_const<base_integer_type>::type,
          base_integer_type
       >::type const_base_integer_type;

       // Add back any volatile qualifier:
       typedef typename conditional<
          is_volatile<T>::value,
          typename add_volatile<const_base_integer_type>::type,
          const_base_integer_type
       >::type type;
    };
} // namespace detail

template<class T>
struct make_signed {
    typedef typename detail::make_signed_imp<T>::type   type;
};


// (boost/type_traits/make_unsigned.hpp)
//
//  (C) Copyright John Maddock 2007.
//
//  (modified for ccwrap by Masashi Kitamura  2014.)
//
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

// make_unsigned
namespace detail {
    template <class T>
    struct make_unsigned_imp {
//      static_assert( is_integral<T>::value || is_enum<T>::value ,"");
//      static_assert(!(is_same<typename remove_cv<T>::type, bool>::value),"");

       typedef typename remove_cv<T>::type t_no_cv;

       typedef typename conditional<
          (  is_unsigned<T>::value
          && is_integral<T>::value
          && !is_same<t_no_cv, char>::value
          && !is_same<t_no_cv, wchar_t>::value
          && !is_same<t_no_cv, bool>::value),
          T,
          typename conditional<
             ( is_integral<T>::value
             && !is_same<t_no_cv, char>::value
             && !is_same<t_no_cv, wchar_t>::value
             && !is_same<t_no_cv, bool>::value),
             typename conditional<
                is_same<t_no_cv, signed char>::value,
                unsigned char,
                typename conditional<
                   is_same<t_no_cv, short>::value,
                   unsigned short,
                   typename conditional<
                      is_same<t_no_cv, int>::value,
                      unsigned int,
                      typename conditional<
                         is_same<t_no_cv, long>::value,
                         unsigned long,
                        #ifdef __CCWRAP_HAS_INT128
                         typename conditional<
                            sizeof(t_no_cv) == sizeof(__ccwrap_llong)::value,
                            __ccwrap_ullong,
                            __ccwrap_uint128
                         >::type
                        #else
                         __ccwrap_ullong
                        #endif
                      >::type
                   >::type
                >::type
             >::type,
             // Not a regular integer type:
             typename conditional<
                sizeof(t_no_cv) == sizeof(signed char),
                unsigned char,
                typename conditional<
                   sizeof(t_no_cv) == sizeof(short),
                   unsigned short,
                   typename conditional<
                      sizeof(t_no_cv) == sizeof(int),
                      unsigned int,
                      typename conditional<
                         sizeof(t_no_cv) == sizeof(long),
                         unsigned long,
                        #ifdef __CCWRAP_HAS_INT128
                         typename conditinal<
                            sizeof(t_no_cv) == sizeof(__ccwrap_llong),
                            __ccwrap_ullong,
                            __ccwrap_uint128
                         >::type
                        #else
                         __ccwrap_ullong
                        #endif
                      >::type
                   >::type
                >::type
             >::type
          >::type
       >::type base_integer_type;

       // Add back any const qualifier:
       typedef typename conditional<
          is_const<T>::value,
          typename add_const<base_integer_type>::type,
          base_integer_type
       >::type const_base_integer_type;

       // Add back any volatile qualifier:
       typedef typename conditional<
          is_volatile<T>::value,
          typename add_volatile<const_base_integer_type>::type,
          const_base_integer_type
       >::type type;
    };
} // namespace detail

template<class T>
struct make_unsigned {
    typedef typename detail::make_unsigned_imp<T>::type type;
};



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

}   // namespace fks

#endif  // __CCWRAP_TYPE_TRAITS_CXX03_FROM_BOOST_HH_INCLUDED
