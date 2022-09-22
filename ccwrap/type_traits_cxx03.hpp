/**
 *  @file   type_traits_cxx03.hpp
 *  @brief  type_traits for c++03
 *  @date   2013-2014,2020
 *  @license Boost Software License Version 1.0
 *  @note
        Referenced source:
        https://boost.org/
        https://faithandbrave.hateblo.jp/entry/20070906/1189072384
        https://faithandbrave.hateblo.jp/entry/20070831/1188558277
 */
#ifndef __CCWRAP_TYPE_TRAITS_CXX03_HPP_INCLUDED
#define __CCWRAP_TYPE_TRAITS_CXX03_HPP_INCLUDED

#include <cstddef>  //#include <ccwrap_header.h>



#if !defined(__CCWRAP_CXX11)
// --------------------------------------------------------------------------------------------------------------------

#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 3))
 #define __CCWRAP_IS_UNION(T)                        __is_union(T)
 #define __CCWRAP_IS_CLASS(T)                        __is_class(T)
 #define __CCWRAP_IS_POD(T)                          (__is_pod(T) && __has_trivial_constructor(T))
 #define __CCWRAP_IS_EMPTY(T)                        __is_empty(T)
 #define __CCWRAP_IS_ABSTRACT(T)                     __is_abstract(T)
 #define __CCWRAP_IS_ENUM(T)                         __is_enum(T)
 #define __CCWRAP_IS_BASE_OF(B,D)                    __is_base_of(B,D)
 //#define __CCWRAP_IS_SAME_OR_BASE_OF(B,D)          __is_base_of(B,D)
 #define __CCWRAP_IS_CONVERTIBLE_TO_GCC(S,T)         __is_convertible_to(S,T)
 #define __CCWRAP_HAS_TRIVIAL_CONSTRUCTOR(T)         __has_trivial_constructor(T)
 #define __CCWRAP_HAS_VIRTUAL_DESTRUCTOR(T)          __has_virtual_destructor(T)
// #define __CCWRAP_IS_CONVERTIBLE(T,U)              ((__is_convertible_to(T,U) || (is_same<T,U>::value && !is_function<U>::value)) && !__is_abstract(U))
#elif defined(__clang__)
 #define __CCWRAP_IS_UNION(T)                        __is_union(T)
 #define __CCWRAP_IS_CLASS(T)                        __is_class(T)
 #define __CCWRAP_IS_POD(T)                          __is_pod(T)
 #define __CCWRAP_IS_EMPTY(T)                        __is_empty(T)
 #define __CCWRAP_IS_ABSTRACT(T)                     __is_abstract(T)
 #define __CCWRAP_IS_ENUM(T)                         __is_enum(T)
 #define __CCWRAP_IS_POLYMORPHIC(T)                  __is_polymorphic(T)
 #define __CCWRAP_IS_BASE_OF(B,D)                    __is_base_of(B,D)
 //#define __CCWRAP_IS_SAME_OR_BASE_OF(B,D)          __is_base_of(B,D)
 #define __CCWRAP_IS_CONVERTIBLE_TO_CLANG(S,T)       __is_convertible_to(S,T)
 #define __CCWRAP_HAS_NOTHROW_CONSTRUCTOR(T)         __has_nothrow_constructor(T)
 #define __CCWRAP_HAS_VIRTUAL_DESTRUCTOR(T)          __has_virtual_destructor(T)
 #define __CCWRAP_HAS_TRIVIAL_CONSTRUCTOR(T)         __has_trivial_constructor(T)
 #define __CCWRAP_HAS_TRIVIAL_DESTRUCTOR(T)          __has_trivial_destructor(T)
 #define __CCWRAP_HAS_TRIVIAL_MOVE_CONSTRUCTOR(T)    __has_trivial_move_constructor(T)
 #define __CCWRAP_HAS_TRIVIAL_MOVE_ASSIGN(T)         __has_trivial_move_assign(T)
// #define __CCWRAP_HAS_TRIVIAL_COPY(T)              (__has_trivial_copy(T) && !is_reference<T>::value && !is_volatile<T>::value)
// #define __CCWRAP_HAS_TRIVIAL_ASSIGN(T)            (__has_trivial_assign(T) && !is_volatile<T>::value)
// #define __CCWRAP_HAS_NOTHROW_COPY(T)              (__has_nothrow_copy(T) && !is_volatile<T>::value && !is_reference<T>::value)
// #define __CCWRAP_HAS_NOTHROW_ASSIGN(T)            (__has_nothrow_assign(T) && !is_volatile<T>::value)
// #define __CCWRAP_IS_CONVERTIBLE(T,U)              (__is_convertible_to(T,U) && !::boost::is_abstract<U>::value)
#elif defined(__DMC__) && (__DMC__ >= 0x848)
// #define __CCWRAP_IS_UNION(T)                      (__typeinfo(T) & 0x0400)
 #define __CCWRAP_IS_POD(T)                          (__typeinfo(T) & 0x0800)
// #define __CCWRAP_IS_EMPTY(T)                      (__typeinfo(T) & 0x1000)
 #define __CCWRAP_HAS_VIRTUAL_DESTRUCTOR(T)          (__typeinfo(T) & 0x0004)
 #define __CCWRAP_HAS_TRIVIAL_DESTRUCTOR(T)          (__typeinfo(T) & 0x0008)
 #define __CCWRAP_HAS_TRIVIAL_CONSTRUCTOR(T)         (__typeinfo(T) & 0x0010)
 #define __CCWRAP_HAS_TRIVIAL_COPY(T)                (__typeinfo(T) & 0x0020)
 #define __CCWRAP_HAS_TRIVIAL_ASSIGN(T)              (__typeinfo(T) & 0x0040)
 #define __CCWRAP_HAS_NOTHROW_CONSTRUCTOR(T)         (__typeinfo(T) & 0x0080)
 #define __CCWRAP_HAS_NOTHROW_COPY(T)                (__typeinfo(T) & 0x0100)
 #define __CCWRAP_HAS_NOTHROW_ASSIGN(T)              (__typeinfo(T) & 0x0200)
#elif defined(_MSC_VER) && (_MSC_VER >= 1700)
 #define __CCWRAP_IS_UNION(T)                        __is_union(T)
 #define __CCWRAP_IS_CLASS(T)                        __is_class(T)
 #define __CCWRAP_IS_POD(T)                          __is_pod(T)
 #define __CCWRAP_IS_EMPTY(T)                        __is_empty(T)
 #define __CCWRAP_IS_ABSTRACT(T)                     __is_abstract(T)
 #define __CCWRAP_IS_ENUM(T)                         __is_enum(T)
 #define __CCWRAP_IS_BASE_OF(B,D)                    __is_base_of(B,D)

 #define __CCWRAP_HAS_TRIVIAL_MOVE_CONSTRUCTOR_VC(T) __has_trivial_move_constructor(T)
 #define __CCWRAP_HAS_TRIVIAL_MOVE_ASSIGN_VC(T)      __has_trivial_move_assign(T)
 //#define __CCWRAP_HAS_TRIVIAL_MOVE_CONSTRUCTOR(T)  ((__has_trivial_move_constructor(T) || ::boost::is_pod<T>::value) && !::boost::is_volatile<T>::value)
 //#define __CCWRAP_HAS_TRIVIAL_MOVE_ASSIGN(T)       ((__has_trivial_move_assign(T) || ::boost::is_pod<T>::value) && ! ::boost::is_const<T>::value && !::boost::is_vo
#endif


// --------------------------------------------------------------------------------------------------------------------

#ifndef __CCWRAP_STD
 #define __CCWRAP_STD    ccwrap
#endif

namespace __CCWRAP_STD {

#if defined(__WATCOMC__)
#define __CCWRAP_IS_SIZEOF_1(ty)         (integral_constant<bool, (sizeof(ty) == 1)>::value)
#define __CCWRAP_HAS_PRAGMA_PACK
#define __CCWRAP_USE_WATCOMC_BUG
#else
#define __CCWRAP_IS_SIZEOF_1(ty)         (sizeof(ty) == 1)
#endif

//#define __CCWRAP_LLONG                 long long
//#define __CCWRAP_ULLONG                unsigned long long

#define __CCWRAP_TRAITS_M_CAT(a,b)       __CCWRAP_TRAITS_M_CAT_B(a,b)
#define __CCWRAP_TRAITS_M_CAT_B(a,b)     __CCWRAP_TRAITS_M_CAT_C(a##b)
#define __CCWRAP_TRAITS_M_CAT_C(x)       x
//#define __CCWRAP_STATIC_ASSERT(x)      typedef char __CCWRAP_TRAITS_M_CAT(ccwrap_static_assert_,__LINE__)[(x) != 0 ? 1 : -1]

#ifndef __CCWRAP_CHAR_IS_UNSIGNED
 #if (defined __CHAR_UNSIGNED__) || (defined _CHAR_UNSIGNED && _CHAR_UNSIGNED)  \
    || (defined __CHAR_SIGNED__ && !__CHAR_SIGNED__) || (defined CHAR_MIN && !CHAR_MIN)
  #define __CCWRAP_CHAR_IS_UNSIGNED  1
 #else
  #define __CCWRAP_CHAR_IS_UNSIGNED  0
 #endif
#endif

// integral_constant
#if defined(__WATCOMC__)
template <class T, long V>
#else
template <class T, T V>
#endif
struct integral_constant {
    typedef integral_constant   type;
    typedef T                   value_type;
    static const T              value = V;
};

// true_type, false_type
typedef integral_constant<bool, true>  true_type;
typedef integral_constant<bool, false> false_type;

#if 1
template<bool V>    struct bool_constant : integral_constant<bool, V> {};
#else
template<bool V>
struct bool_constant {
    typedef bool_constant   type;
    typedef bool            value_type;
    static const bool       value = V;
};
#endif

#define __CCWRAP_TRAITS_SPEC0(S, V) template <> struct S : public integral_constant<bool, V> {}
#define __CCWRAP_TRAITS_SPEC1(S, V) template <class T> struct S : public integral_constant<bool, V> {}
#define __CCWRAP_TRAITS_SPEC2(S, V) template <class T, class ClassName> struct S : public integral_constant<bool, V> {}
// T, T const, T volatile, T const volatile
#define __CCWRAP_TRAITS_SPEC(Order, Traits, SpecialType, Value)                                  \
    __CCWRAP_TRAITS_M_CAT(__CCWRAP_TRAITS_SPEC,Order)(Traits<SpecialType>,                Value); \
    __CCWRAP_TRAITS_M_CAT(__CCWRAP_TRAITS_SPEC,Order)(Traits<SpecialType const>,          Value); \
    __CCWRAP_TRAITS_M_CAT(__CCWRAP_TRAITS_SPEC,Order)(Traits<SpecialType volatile>,       Value); \
    __CCWRAP_TRAITS_M_CAT(__CCWRAP_TRAITS_SPEC,Order)(Traits<SpecialType const volatile>, Value)

// conditional
template <bool C, class T, class E> struct conditional {typedef T type;};
template <class T, class E>         struct conditional<false, T, E> {typedef E type;};

// enable_if
template <bool, class T = void> struct enable_if {};
template <class T> struct enable_if<true, T> {typedef T type;};

// is_same
template <class Type1, class Type2> struct is_same : public false_type {};
template <class T> struct is_same<T, T> : public true_type {};

// is_void
template <class T> struct is_void : public false_type {};
__CCWRAP_TRAITS_SPEC(0, is_void, void, true);

// is_const
template <class T> struct is_const : public false_type {};
template <class T> struct is_const<T const> : public true_type {};

// is_volatile
template <class T> struct is_volatile : public false_type {};
template <class T> struct is_volatile<T volatile> : true_type {};

// remove_const
template <class T> struct remove_const { typedef T type; };
template <class T> struct remove_const<T const> { typedef T type; };

// remove_volatile
template <class T> struct remove_volatile { typedef T type; };
template <class T> struct remove_volatile<T volatile> { typedef T type; };

// remove_cv : remove const & volatile
template <class TB>
struct remove_cv {
    typedef typename remove_const<typename remove_volatile<TB>::type>::type type;
};

// add_const
template <class T>
struct add_const { typedef T const type; };

// add_volatile
template <class T> struct add_volatile { typedef T volatile type; };

// add_cv : add const volatile
template <class TB>
struct add_cv { typedef typename add_const<typename add_volatile<TB>::type>::type type; };


// is_reference
template <class T> struct is_reference : public false_type {};

template <class T> struct is_reference<T&> : public true_type {};

// is_lvalue_reference
template <class T> struct is_lvalue_reference : public false_type {};

template <class T> struct is_lvalue_reference<T&> : public true_type {};

// is_rvalue_reference (TODO)
template <class T> struct is_rvalue_reference : public false_type {};

// remove_reference
template <class T> struct remove_reference { typedef T type; };
template <class T> struct remove_reference<T&> { typedef T type; };
//template <class T> struct remove_reference<T const&> { typedef T const type; };
//template <class T> struct remove_reference<T volatile&> { typedef T volatile type; };
//template <class T> struct remove_reference<T const volatile&> { typedef T const volatile type; };

// remove_reference
template <class T> struct remove_lvalue_reference { typedef T type; };
template <class T> struct remove_lvalue_reference<T&> { typedef T type; };

// add_reference
namespace detail {
    template<class T, bool C>   struct add_reference_helper { typedef T& type; };
    template<class T>           struct add_reference_helper<T, true> { typedef T type; };
}
template<class T>
struct add_reference : public detail::add_reference_helper<T, (is_void<T>::value || is_reference<T>::value)> {};

template<class T>
struct add_lvalue_reference : public add_reference<T> {};

//TODO: add_rvalue_reference

// remove_pointer
template <class T> struct remove_pointer { typedef T type; };
template <class T> struct remove_pointer<T*> { typedef T type; };

// add_pointer
template <class T>
struct add_pointer { typedef typename remove_reference<T>::type* type; };

// is_pointer
#ifdef _MSC_VER
namespace detail {
    template<class T> struct is_pointer_helper : public false_type {};
    template<class T> struct is_pointer_helper<T*> : public true_type {};
}
template <class T> struct is_pointer : public detail::is_pointer_helper< typename remove_cv<T>::type > {};
#else
template <class T>
struct is_pointer : false_type {};
__CCWRAP_TRAITS_SPEC(1, is_pointer, T*, true);
#endif

// is_member_pointer
#if defined(_MSC_VER)
namespace detail {
    template<class T> struct is_member_pointer_helper : public  false_type {};
    template<class T, class C> struct is_member_pointer_helper<T C::*> : public true_type {};
}
template <class T>
struct is_member_pointer : public detail::is_member_pointer_helper<typename remove_cv<T>::type> {};
#else
template <class T>
struct is_member_pointer : public  false_type {};
__CCWRAP_TRAITS_SPEC(2, is_member_pointer, T ClassName::*, true);
#endif

// is_integral
template <class T> struct is_integral : public false_type {};
__CCWRAP_TRAITS_SPEC(0, is_integral, bool,               true);
__CCWRAP_TRAITS_SPEC(0, is_integral, char,               true);
__CCWRAP_TRAITS_SPEC(0, is_integral, signed char,        true);
__CCWRAP_TRAITS_SPEC(0, is_integral, unsigned char,      true);
__CCWRAP_TRAITS_SPEC(0, is_integral, wchar_t,            true);
__CCWRAP_TRAITS_SPEC(0, is_integral, short,              true);
__CCWRAP_TRAITS_SPEC(0, is_integral, unsigned short,     true);
__CCWRAP_TRAITS_SPEC(0, is_integral, int,                true);
__CCWRAP_TRAITS_SPEC(0, is_integral, unsigned int,       true);
__CCWRAP_TRAITS_SPEC(0, is_integral, long,               true);
__CCWRAP_TRAITS_SPEC(0, is_integral, unsigned long,      true);
__CCWRAP_TRAITS_SPEC(0, is_integral, long long,          true);
__CCWRAP_TRAITS_SPEC(0, is_integral, unsigned long long, true);

// is_floating_point
template <class T> struct is_floating_point : public false_type {};
__CCWRAP_TRAITS_SPEC(0, is_floating_point, float         , true);
__CCWRAP_TRAITS_SPEC(0, is_floating_point, double        , true);
__CCWRAP_TRAITS_SPEC(0, is_floating_point, long double   , true);

// is_signed
template <class T> struct is_signed : public false_type {};
__CCWRAP_TRAITS_SPEC(0, is_signed, signed char   , true);
__CCWRAP_TRAITS_SPEC(0, is_signed, short         , true);
__CCWRAP_TRAITS_SPEC(0, is_signed, int           , true);
__CCWRAP_TRAITS_SPEC(0, is_signed, long          , true);
__CCWRAP_TRAITS_SPEC(0, is_signed, long long     , true);
__CCWRAP_TRAITS_SPEC(0, is_signed, float         , true);
__CCWRAP_TRAITS_SPEC(0, is_signed, double        , true);
__CCWRAP_TRAITS_SPEC(0, is_signed, long double   , true);
#if !__CCWRAP_CHAR_IS_UNSIGNED
__CCWRAP_TRAITS_SPEC(0, is_signed, char          , true);
#endif

// is_unsigned
template <class T> struct is_unsigned : public false_type {};
__CCWRAP_TRAITS_SPEC(0, is_unsigned, bool                , true);
__CCWRAP_TRAITS_SPEC(0, is_unsigned, wchar_t             , true);
__CCWRAP_TRAITS_SPEC(0, is_unsigned, unsigned char       , true);
__CCWRAP_TRAITS_SPEC(0, is_unsigned, unsigned short      , true);
__CCWRAP_TRAITS_SPEC(0, is_unsigned, unsigned int        , true);
__CCWRAP_TRAITS_SPEC(0, is_unsigned, unsigned long       , true);
__CCWRAP_TRAITS_SPEC(0, is_unsigned, unsigned long long  , true);
#if __CCWRAP_CHAR_IS_UNSIGNED
__CCWRAP_TRAITS_SPEC(0, is_unsigned, char                , true);
#endif

// is_arithmetic
template <class T>
struct is_arithmetic : public integral_constant<bool, (is_integral<T>::value || is_floating_point<T>::value)> {};

// is_fundamental
template <class T>
struct is_fundamental : public integral_constant<bool, (is_arithmetic<T>::value || is_void<T>::value)> {};

// is_compound
template <class T>
struct is_compound : public integral_constant<bool, !(is_fundamental<T>::value)> {};

// is_union_or_class
namespace detail {
    typedef char    yes_t;
    typedef short   no_t;
    //STATIC_ASSERT(sizeof(yes_t) != sizeof(no_t));

 #if defined(__CCWRAP_IS_UNION) && defined(__CCWRAP_IS_CLASS)
    template<class T>
    struct is_union_or_class : public integral_constant<bool, __CCWRAP_IS_UNION(T) || __CCWRAP_IS_CLASS(T)> {};
 #elif defined __DMC__
    template<class T>
    class is_union_or_class_helper {
        template<class U> static yes_t test(int U::*);
        template<class U> static no_t  test(...);
    public:
        static const bool value = __CCWRAP_IS_SIZEOF_1( test<T>(0) );
    };
    template<class T>
    struct is_union_or_class
        : public integral_constant<bool, is_union_or_class_helper< typename remove_cv<T>::type >::value >
    {};
 #else
    struct is_union_or_class_helper {
        template<class U> static yes_t test(int U::*);
        template<class U> static no_t  test(...);
    };
    template<class T>
    struct is_union_or_class
        : public integral_constant<bool
            , __CCWRAP_IS_SIZEOF_1( is_union_or_class_helper::test< typename remove_cv<T>::type >(0) ) >
    {};
 #endif
}

// is_union
#ifdef __CCWRAP_IS_UNION
template <class T> struct is_union : public integral_constant<bool, __CCWRAP_IS_UNION(T)> {};
#else
template <class T> struct is_union : public false_type {};
#endif

// is_class
#ifdef __CCWRAP_IS_CLASS
template <class T> struct is_class : public integral_constant<bool, __CCWRAP_IS_CLASS(T)> {};
#else
template<class T>
struct is_class : public integral_constant<bool, !is_union<T>::value && detail::is_union_or_class<T>::value  > {};
#endif

// is_empty
#ifdef __CCWRAP_IS_EMPTY
template <class T> struct is_empty : public integral_constant<bool, __CCWRAP_IS_EMPTY(T)> {};
#else
namespace detail {
    template <class T, bool C>
    class is_empty_helper {
        template <class U>  struct first {};
        template <class U>  struct second : public U {};
    public:
        static const bool value = (sizeof(first<T>) == sizeof(second<T>));
    };
    template <class T>
    struct is_empty_helper<T, true> {
        static const bool value = false;
    };
}
template <class T>
struct is_empty : public integral_constant<bool, detail::is_empty_helper<T, (!is_class<T>::value) >::value> {};
#endif

// is_polymorphic
#ifdef __CCWRAP_IS_POLYMORPHIC
template <class T> struct is_polymorphic : public integral_constant<bool, __CCWRAP_IS_POLYMORPHIC(T)> {};
#else
namespace detail {
    template <class T, bool V>
    class is_polymorphic_helper {
        template <class U>
        struct first : public U {};

        template <class U>
        struct second : public U {
            virtual void dummy();
            virtual ~second() throw();
        };

    public:
        static const bool value = sizeof(first<T>) == sizeof(second<T>);
    };

    template <class T>
    struct is_polymorphic_helper<T, true> {
        static const bool value = false;
    };
}
template <class T>
struct is_polymorphic : public integral_constant<bool, detail::is_polymorphic_helper<T,!is_class<T>::value>::value> {};
#endif


// rank
template<class T> struct rank : integral_constant<unsigned, 0> {};
template<class T> struct rank<T[]> : public integral_constant<unsigned, rank<T>::value + 1> {};
template<class TB, unsigned N> struct rank<TB[N]> : public integral_constant<unsigned, rank<TB>::value + 1> {};

// remove_extent
template<class T> struct remove_extent { typedef T type; };
template<class T> struct remove_extent<T[]> { typedef T type; };
template<class TB, int N> struct remove_extent<TB[N]> { typedef TB type; };

// remove_all_extents
template <class T > struct remove_all_extents { typedef T type; };
template <class TB> struct remove_all_extents<TB[]> { typedef typename remove_all_extents<TB>::type type; };
template <class TB2, int N> struct remove_all_extents<TB2[N]> { typedef typename remove_all_extents<TB2>::type type; };


#if !defined(__CCWRAP_USE_WATCOMC_BUG)

// is_array
template <class T> struct is_array : public false_type {};
template <class T> struct is_array<T[]> : public true_type {};
template <class TB, int N> struct is_array<TB[N]> : public true_type {};

// is_function
namespace detail {
    template<class T>
    class is_funcret_or_inarray {
        template<class U> static yes_t test(U(*)[1]);
        template<class U> static no_t  test(...);
    public:
        static const bool value = __CCWRAP_IS_SIZEOF_1(test<T>(0));
    };
}
template<class T>
struct is_function
    : public integral_constant<bool , !(  detail::is_union_or_class<T>::value
                                       || is_reference<T>::value
                                       || is_void<T>::value
                                       || detail::is_funcret_or_inarray<T>::value
                                       )>
{};


#else // defined(__CCWRAP_USE_WATCOMC_BUG)

namespace detail {
    // watcom-ng T[N]
    template <class T> struct is_array_helper1 : public false_type {};
    template <class T> struct is_array_helper1<T[]> : public true_type {};
    //template <class T, unsigned N> struct is_array_helper1<T[N]> : public true_type {};

    // value=0: U&  watcom U[] U()
    template<class T>
    class is_array_helper2 {
        typedef typename remove_cv<T>::type rcvT;
        template<class U> static yes_t test(U[]);
        template<class U> static no_t  test(...);
    public:
        static const bool value = __CCWRAP_IS_SIZEOF_1(test<rcvT>(0));
    };

    // value=0: U(), AbsClass, U[], U[N]
    template<class T>
    class is_funcret_or_inarray {
        template<class U> static yes_t test(U());
        template<class U> static no_t  test(...);
    public:
        static const bool value = __CCWRAP_IS_SIZEOF_1(test<T>(0));
    };
    template<class T>
    struct is_function_or_arrayn
    : public integral_constant<bool , !(  is_union_or_class<T>::value
                                       || is_reference<T>::value
                                       || is_void<T>::value
                                       || is_array_helper1<T>::value
                                       ) && !is_funcret_or_inarray<T>::value >
    {};
}
// is_function
template<class T>
struct is_function  : public integral_constant<bool
                , detail::is_function_or_arrayn<T>::value
                        && !detail::is_array_helper2<T>::value>
{};

// is_array
template<class T>
struct is_array : public integral_constant<bool
                , detail::is_array_helper1<T>::value
                    || (detail::is_function_or_arrayn<T>::value
                        && detail::is_array_helper2<T>::value) >
{};

#endif  // __CCWRAP_USE_WATCOMC_BUG


// is_abstract
#ifdef __CCWRAP_IS_ABSTRACT
template <class T> struct is_abstract : public integral_constant<bool, __CCWRAP_IS_ABSTRACT(T)> {};
#else
template <class T>
struct is_abstract : public integral_constant<bool,
    (detail::is_union_or_class<T>::value && !detail::is_funcret_or_inarray<T>::value)>
{};
#endif

// is_member_object_pointer
template <class T>
struct is_member_object_pointer : public  false_type {};
__CCWRAP_TRAITS_SPEC(2, is_member_object_pointer, T ClassName::*, !is_function<T>::value);

// is_member_function_pointer
template <class T>
struct is_member_function_pointer : public  false_type {};
__CCWRAP_TRAITS_SPEC(2, is_member_function_pointer, T ClassName::*, is_function<T>::value);

// is_member_pointer
#if 0
template <class T>
struct is_member_pointer : public integral_constant<bool
                            , (is_member_object_pointer<T>::value || is_member_function_pointer<T>::value)>
{};
#endif

// is_object
template <class T>
struct is_object : public integral_constant<bool
                    , !(is_reference<T>::value || is_void<T>::value || is_function<T>::value)>
{};

// is_enum
#ifdef __CCWRAP_IS_ENUM
template <class T> struct is_enum : public integral_constant<bool, __CCWRAP_IS_ENUM(T)> {};
#else
template <class T>
struct is_enum : public integral_constant<bool,
        !(  is_fundamental<T>::value || is_array<T>::value || is_pointer<T>::value || is_reference<T>::value
         || is_member_pointer<T>::value || detail::is_union_or_class<T>::value || is_function<T>::value
         )>
{};
#endif

// is_scalar
template <class T>
struct is_scalar : public integral_constant<bool, (is_arithmetic<T>::value || is_member_pointer<T>::value
                                                    || is_pointer<T>::value || is_enum<T>::value ) >
{};

// is_base_of
#ifdef __CCWRAP_IS_BASE_OF
template <class B, class D> struct is_base_of : public integral_constant<bool, __CCWRAP_IS_BASE_OF(B,D)> {};
#elif defined(_MSC_VER)
namespace detail {
    template <class Base, typename Derived, bool C>
    class is_base_of_helper {
        typedef typename remove_cv<Base>::type    NoCv_Base;
        typedef typename remove_cv<Derived>::type NoCv_Derived;

        template <class U>
        static yes_t test(NoCv_Derived&, U);
        static no_t  test(NoCv_Base&, int);

        struct Conv {
            operator NoCv_Derived&();
            operator NoCv_Base&() const;
        };
    public:
        static const bool value = __CCWRAP_IS_SIZEOF_1(test(Conv(), 0));
    };

    template <class Base, class Derived>
    struct is_base_of_helper<Base, Derived, true> {
        static const bool value = is_same<Base, Derived>::value;
    };
}
template <class Base, class Derived>
struct is_base_of
    : public integral_constant<bool, detail::is_base_of_helper<Base, Derived,
                                         (  !is_class<Base>::value
                                         || !is_class<Derived>::value
                                         || is_same<Base, Derived>::value)
                                     >::value >
{};
#else
namespace detail {
    template <class B, typename D, bool C>
    class is_base_of_helper {
        typedef typename remove_cv<B>::type NoCv_Base;
        typedef typename remove_cv<D>::type NoCv_Derived;

        template <class U>
        static yes_t test(NoCv_Derived&, U);
        static no_t  test(NoCv_Base&, int);

        struct Conv {
            operator NoCv_Derived&();
            operator NoCv_Base&() const;
        };
        static Conv& s_conv;
    public:
        //struct Test : integral_constant<bool, sizeof( is_base_of_helper<B,D,C>::test( Conv(), 0) ) == 1> {};
        struct Test : integral_constant<bool, sizeof( is_base_of_helper<B,D,C>::test( s_conv, 0) ) == 1> {};
    };

    template <class Base, class Derived>
    struct is_base_of_helper<Base, Derived, true> {
        struct Test {
            static const bool value = false; //is_same<Base, Derived>::value;
        };
    };
}
template <class B, class D>
struct is_base_of
    : public integral_constant<bool, detail::is_base_of_helper<B, D
        , (!is_class<B>::value || !is_class<D>::value || is_same<B,D>::value || is_empty<B>::value ) >::Test::value >
{};
#endif

// is_convertible
#if 0
namespace detail {
    template <class From, class To>
    class is_convertible_simple {
        static yes_t test(To);
        static no_t  test(...);
        static From make_from();
        //static From& s_from;
    public:
        static const bool value = __CCWRAP_IS_SIZEOF_1(test(make_from()));
        //static const bool value = __CCWRAP_IS_SIZEOF_1(test(s_from));
    };

    template <class T>
    struct is_int_or_cref {
        typedef typename remove_reference<T>::type RefRemoveType;
        static const bool value = (is_integral<T>::value
                                    || (is_integral<RefRemoveType>::value
                                     && is_const<RefRemoveType>::value
                                     && !is_volatile<RefRemoveType>::value));
    };

    template <class From, class To, bool C>
    struct is_convertible_helper {
        static const bool value = is_convertible_simple<typename add_reference<From>::type, To>::value;
    };

    template<class From, class To>
    struct is_convertible_helper<From, To, true> {
        static const bool value = is_void<To>::value || (is_int_or_cref<To>::value && !is_void<From>::value);
    };
}

template <class From, class To>
struct is_convertible : public integral_constant<bool
    , detail::is_convertible_helper<From, To,
        (is_void<From>::value || is_void<To>::value || is_function<To>::value || is_array<To>::value
         || (is_floating_point<typename remove_reference<From>::type>::value && detail::is_int_or_cref<To>::value)
        ) >::value>
{};
#elif 0
namespace detail {
    template <class From, class To /*, class RmvRefFrom = typename remove_reference<From>::type */ >
    struct is_convertible_simple {
        static yes_t test(To);
        static no_t  test(...);
        //static RmvRefFrom& s_from;
        static From& s_from;
        struct type : integral_constant<bool, sizeof( test(s_from) ) == 1> {};
    };
#if 1
    template <class From, class To>
    struct is_convertible_simple2 : public is_convertible_simple<From, To const&>::type {};

    template <class From, class To>
    struct is_convertible_simple2<From, To&> : public is_convertible_simple<From, To&>::type {};
    template <class From, class To>
    struct is_convertible_simple2<From, To const&> : public is_convertible_simple<From, To const&>::type {};
    template <class From, class To>
    struct is_convertible_simple2<From, To volatile&> : public is_convertible_simple<From, To volatile&>::type {};
    template <class From, class To>
    struct is_convertible_simple2<From, To const volatile&> : public is_convertible_simple<From, To const volatile&>::type {};

    template <class From, class To>
    struct is_convertible_simple2<From[], To*> : public is_convertible_simple<From*, To*>::type {};

    template <class From, class To, unsigned N>
    struct is_convertible_simple2<From[N], To*> : public is_convertible_simple<From*, To*>::type {};
#endif

    template <class From, class To, bool C>
    struct is_convertible_helper : public integral_constant<bool, is_convertible_simple2<typename remove_reference<From>::type, To>::value> {};

    template <class T, class R = typename remove_reference<T>::type >
    struct is_arithmetic_or_cref : public integral_constant<bool,
        (is_arithmetic<T>::value || (is_arithmetic<R>::value && is_const<R>::value && !is_volatile<R>::value)) >
    {};

    template<class From, class To>
    struct is_convertible_helper<From, To, true> : public integral_constant<bool
        , (is_void<From>::value && is_void<To>::value) || (is_arithmetic_or_cref<To>::value && !is_void<From>::value) >
    {};
}

template <class From, class To>
struct is_convertible : public integral_constant<bool
    , detail::is_convertible_helper<From, To,
        ( is_void<From>::value || is_void<To>::value || is_function<To>::value || is_array<To>::value
         || ((is_arithmetic<typename remove_reference<typename remove_cv<From>::type>::type>::value
               || is_enum<typename remove_reference<typename remove_cv<From>::type>::type>::value)
             && detail::is_arithmetic_or_cref<To>::value)
        ) >::value>
{};
#else
namespace detail {
    template <class From, class To, class RmvRefFrom = typename remove_reference<From>::type >
    struct is_convertible_simple {
        static yes_t test(To);
        static no_t  test(...);
        static RmvRefFrom& s_from;
        //static From& s_from;
        struct type : integral_constant<bool, sizeof( test(s_from) ) == 1> {};
    };
#if 0
    template <class From, class To>
    struct is_convertible_helper2 : public is_convertible_simple<From, To>::type {};
#else
    template <class From, class To>
    struct is_convertible_helper2 : public is_convertible_simple<From, To const&>::type {};
    template <class From, class To>
    struct is_convertible_helper2<From, To&> : public is_convertible_simple<From, To&>::type {};
    template <class From, class To>
    struct is_convertible_helper2<From, To const&> : public is_convertible_simple<From, To const&>::type {};
    template <class From, class To>
    struct is_convertible_helper2<From, To volatile&> : public is_convertible_simple<From, To volatile&>::type {};
    template <class From, class To>
    struct is_convertible_helper2<From, To const volatile&> : public is_convertible_simple<From, To const volatile&>::type {};

    template <class From, class To>
    struct is_convertible_helper2<From[], To*> : public is_convertible_simple<From*, To*>::type {};

    template <class From, class To, unsigned N>
    struct is_convertible_helper2<From[N], To*> : public is_convertible_simple<From*, To*>::type {};
#endif

    template <class T, class R = typename remove_reference<T>::type >
    struct is_arithmetic_or_cref : public integral_constant<bool,
        (is_arithmetic<T>::value || (is_arithmetic<R>::value && is_const<R>::value && !is_volatile<R>::value)) >
    {};

    template <class From, class To, bool C>
    struct is_convertible_helper : public integral_constant<bool, is_convertible_helper2<From, To>::value> {};

    template<class From, class To>
    struct is_convertible_helper<From, To, true> : public integral_constant<bool
        , (is_void<From>::value && is_void<To>::value) || (is_arithmetic_or_cref<To>::value && !is_void<From>::value) >
    {};
}

template <class From, class To>
struct is_convertible : public integral_constant<bool
    , detail::is_convertible_helper<From, To,
        ( is_void<From>::value || is_void<To>::value || is_function<To>::value || is_array<To>::value
         || ((is_arithmetic<typename remove_reference<typename remove_cv<From>::type>::type>::value
               || is_enum<typename remove_reference<typename remove_cv<From>::type>::type>::value)
             && detail::is_arithmetic_or_cref<To>::value)
        ) >::value>
{};
#endif


// decay
template< class T >
struct decay {
    typedef typename remove_reference<T>::type U;
    typedef typename conditional<
        is_array<U>::value,
        typename remove_extent<U>::type*,
        typename conditional< is_function<U>::value, typename add_pointer<U>::type, typename remove_cv<U>::type >::type
    >::type type;
};


// --------------------------------------------------------------------------
//TODO:
// tiny...

// is_literal_type
template <class T> struct is_literal_type : public is_scalar<T> {};

// is_pod
#ifdef __CCWRAP_IS_POD
template <class T> struct is_pod : public integral_constant<bool, __CCWRAP_IS_POD(T)> {};
#elif !defined(__CCWRAP_USE_WATCOMC_BUG)
template <class TB>
struct is_pod : public integral_constant<bool
                , !is_void<TB>::value && (is_scalar<typename remove_all_extents<TB>::type>::value) >
{};
#else // defined(__CCWRAP_USE_WATCOMC_BUG)
template <class TB>
struct is_pod : public integral_constant<bool
                , !is_void<TB>::value && (is_scalar<typename remove_all_extents<TB>::type>::value) >
{};
#endif  // __CCWRAP_USE_WATCOMC_BUG

// is_standard_layout
template <class T> struct is_standard_layout : public is_pod<T> {};

// is_trivial
template <class T> struct is_trivial : public is_pod<T> {};

// is_trivially_copyable
template <class T> struct is_trivially_copyable : public is_pod<T> {};

// is_trivially_constructible
template <class T> struct is_trivially_constructible : public is_pod<T> {};

// is_trivially_default_constructible
template <class T> struct is_trivially_default_constructible : public is_pod<T> {};

// is_trivially_copy_constructible
template <class T> struct is_trivially_copy_constructible : public is_pod<T> {};

// is_trivially_move_assignable
template <class T> struct is_trivially_move_assignable : public is_pod<T> {};

// is_trivially_assignable
template <class T> struct is_trivially_assignable : public is_pod<T> {};

// is_trivially_destructible
template <class T> struct is_trivially_destructible : public is_pod<T> {};

// is_nothrow_constructible
template <class T> struct is_nothrow_constructible : public is_pod<T> {};

// is_nothrow_default_constructible
template <class T> struct is_nothrow_default_constructible : public is_pod<T> {};

// is_nothrow_copy_constructible
template <class T> struct is_nothrow_copy_constructible : public is_pod<T> {};

// is_nothrow_move_constructible
template <class T> struct is_nothrow_move_constructible : public is_pod<T> {};

// is_nothrow_assignable
template <class T, class U> struct is_nothrow_assignable : public is_convertible<T,U> {};

// is_nothrow_copy_assignable
template <class T> struct is_nothrow_copy_assignable : public is_pod<T> {};

// is_nothrow_move_assignable
template <class T> struct is_nothrow_move_assignable : public is_pod<T> {};

// is_nothrow_destructible
template <class T> struct is_nothrow_destructible : public is_pod<T> {};

// is_default_constructible
template <class T> struct is_default_constructible : public is_pod<T> {};

// is_copy_constructible
template <class T> struct is_copy_constructible : public is_pod<T> {};

// is_move_constructible
template <class T> struct is_move_constructible : public is_pod<T> {};

// is_assignable
template <class T,class U> struct is_assignable : public is_convertible<T,U> {};

// is_copy_assignable
template <class T> struct is_copy_assignable : public is_pod<T> {};

// is_move_assignable
template <class T> struct is_move_assignable : public is_pod<T> {};

// is_destructible
template <class T> struct is_destructible : public is_pod<T> {};

// is_constructible
template <class T> struct is_constructible : public is_pod<T> {};

// has_virtual_destructor
template <class T> struct has_virtual_destructor : public false_type {};

// result_of
//

// common_type
//

// --------------------------------------------------------------------------

} // namespace __CCWRAP_STD


#undef __CCWRAP_TRAITS_SPEC0
#undef __CCWRAP_TRAITS_SPEC1
#undef __CCWRAP_TRAITS_SPEC2
#undef __CCWRAP_TRAITS_SPEC
#undef __CCWRAP_IS_SIZEOF_1
#undef __CCWRAP_TRAITS_M_CAT
#undef __CCWRAP_TRAITS_M_CAT_B
#undef __CCWRAP_TRAITS_M_CAT_C

#endif  // __CCWRAP_CXX11

#endif  // __CCWRAP_TYPE_TRAITS_CXX03_HPP_INCLUDED
