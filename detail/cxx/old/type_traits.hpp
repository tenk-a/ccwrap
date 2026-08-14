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
#ifndef _CCW_DETAIL_TYPE_TRAITS_HPP
#define _CCW_DETAIL_TYPE_TRAITS_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>

#if __cplusplus && __cplusplus < 201103L

#include <cstddef>  //#include <ccwrap_header.h>

// --------------------------------------------------------------------------------------------------------------------

#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 3))
 #define __CCW_IS_UNION(T)                        __is_union(T)
 #define __CCW_IS_CLASS(T)                        __is_class(T)
 #define __CCW_IS_POD(T)                          (__is_pod(T) && __has_trivial_constructor(T))
 #define __CCW_IS_EMPTY(T)                        __is_empty(T)
 #define __CCW_IS_ABSTRACT(T)                     __is_abstract(T)
 #define __CCW_IS_ENUM(T)                         __is_enum(T)
 #define __CCW_IS_BASE_OF(B,D)                    __is_base_of(B,D)
 //#define __CCW_IS_SAME_OR_BASE_OF(B,D)          __is_base_of(B,D)
 #define __CCW_IS_CONVERTIBLE_TO_GCC(S,T)         __is_convertible_to(S,T)
 #define __CCW_HAS_TRIVIAL_CONSTRUCTOR(T)         __has_trivial_constructor(T)
 #define __CCW_HAS_VIRTUAL_DESTRUCTOR(T)          __has_virtual_destructor(T)
// #define __CCW_IS_CONVERTIBLE(T,U)              ((__is_convertible_to(T,U) || (is_same<T,U>::value && !is_function<U>::value)) && !__is_abstract(U))
#elif defined(__clang__)
 #define __CCW_IS_UNION(T)                        __is_union(T)
 #define __CCW_IS_CLASS(T)                        __is_class(T)
 #define __CCW_IS_POD(T)                          __is_pod(T)
 #define __CCW_IS_EMPTY(T)                        __is_empty(T)
 #define __CCW_IS_ABSTRACT(T)                     __is_abstract(T)
 #define __CCW_IS_ENUM(T)                         __is_enum(T)
 #define __CCW_IS_POLYMORPHIC(T)                  __is_polymorphic(T)
 #define __CCW_IS_BASE_OF(B,D)                    __is_base_of(B,D)
 //#define __CCW_IS_SAME_OR_BASE_OF(B,D)          __is_base_of(B,D)
 #define __CCW_IS_CONVERTIBLE_TO_CLANG(S,T)       __is_convertible_to(S,T)
 #define __CCW_HAS_NOTHROW_CONSTRUCTOR(T)         __has_nothrow_constructor(T)
 #define __CCW_HAS_VIRTUAL_DESTRUCTOR(T)          __has_virtual_destructor(T)
 #define __CCW_HAS_TRIVIAL_CONSTRUCTOR(T)         __has_trivial_constructor(T)
 #define __CCW_HAS_TRIVIAL_DESTRUCTOR(T)          __has_trivial_destructor(T)
 #define __CCW_HAS_TRIVIAL_MOVE_CONSTRUCTOR(T)    __has_trivial_move_constructor(T)
 #define __CCW_HAS_TRIVIAL_MOVE_ASSIGN(T)         __has_trivial_move_assign(T)
// #define __CCW_HAS_TRIVIAL_COPY(T)              (__has_trivial_copy(T) && !is_reference<T>::value && !is_volatile<T>::value)
// #define __CCW_HAS_TRIVIAL_ASSIGN(T)            (__has_trivial_assign(T) && !is_volatile<T>::value)
// #define __CCW_HAS_NOTHROW_COPY(T)              (__has_nothrow_copy(T) && !is_volatile<T>::value && !is_reference<T>::value)
// #define __CCW_HAS_NOTHROW_ASSIGN(T)            (__has_nothrow_assign(T) && !is_volatile<T>::value)
// #define __CCW_IS_CONVERTIBLE(T,U)              (__is_convertible_to(T,U) && !::boost::is_abstract<U>::value)
#elif defined(__DMC__) && (__DMC__ >= 0x848)
// #define __CCW_IS_UNION(T)                      (__typeinfo(T) & 0x0400)
 #define __CCW_IS_POD(T)                          (__typeinfo(T) & 0x0800)
// #define __CCW_IS_EMPTY(T)                      (__typeinfo(T) & 0x1000)
 #define __CCW_HAS_VIRTUAL_DESTRUCTOR(T)          (__typeinfo(T) & 0x0004)
 #define __CCW_HAS_TRIVIAL_DESTRUCTOR(T)          (__typeinfo(T) & 0x0008)
 #define __CCW_HAS_TRIVIAL_CONSTRUCTOR(T)         (__typeinfo(T) & 0x0010)
 #define __CCW_HAS_TRIVIAL_COPY(T)                (__typeinfo(T) & 0x0020)
 #define __CCW_HAS_TRIVIAL_ASSIGN(T)              (__typeinfo(T) & 0x0040)
 #define __CCW_HAS_NOTHROW_CONSTRUCTOR(T)         (__typeinfo(T) & 0x0080)
 #define __CCW_HAS_NOTHROW_COPY(T)                (__typeinfo(T) & 0x0100)
 #define __CCW_HAS_NOTHROW_ASSIGN(T)              (__typeinfo(T) & 0x0200)
#elif defined(_MSC_VER) && (_MSC_VER >= 1400)
 #define __CCW_IS_UNION(T)                        __is_union(T)
 #define __CCW_IS_CLASS(T)                        __is_class(T)
 #define __CCW_IS_POD(T)                          __is_pod(T)
 #define __CCW_IS_EMPTY(T)                        __is_empty(T)
 #define __CCW_IS_ABSTRACT(T)                     __is_abstract(T)
 #define __CCW_IS_ENUM(T)                         __is_enum(T)
 #define __CCW_IS_BASE_OF(B,D)                    __is_base_of(B,D)

 #define __CCW_HAS_TRIVIAL_MOVE_CONSTRUCTOR_VC(T) __has_trivial_move_constructor(T)
 #define __CCW_HAS_TRIVIAL_MOVE_ASSIGN_VC(T)      __has_trivial_move_assign(T)
 //#define __CCW_HAS_TRIVIAL_MOVE_CONSTRUCTOR(T)  ((__has_trivial_move_constructor(T) || ::boost::is_pod<T>::value) && !::boost::is_volatile<T>::value)
 //#define __CCW_HAS_TRIVIAL_MOVE_ASSIGN(T)       ((__has_trivial_move_assign(T) || ::boost::is_pod<T>::value) && ! ::boost::is_const<T>::value && !::boost::is_vo
#endif


// --------------------------------------------------------------------------------------------------------------------

namespace std {

#if defined(__WATCOMC__)
#define __CCW_IS_SIZEOF_1(ty)         (integral_constant<bool, (sizeof(ty) == 1)>::value)
#else
#define __CCW_IS_SIZEOF_1(ty)         (sizeof(ty) == 1)
#endif

#define __CCW_TRAITS_M_CAT(a,b)       __CCW_TRAITS_M_CAT_B(a,b)
#define __CCW_TRAITS_M_CAT_B(a,b)     __CCW_TRAITS_M_CAT_C(a##b)
#define __CCW_TRAITS_M_CAT_C(x)       x
//#define __CCW_STATIC_ASSERT(x)      typedef char __CCW_TRAITS_M_CAT(__ccw_static_assert_,__LINE__)[(x) != 0 ? 1 : -1]

#ifndef __CCW_CHAR_IS_UNSIGNED
 #if (defined __CHAR_UNSIGNED__) || (defined _CHAR_UNSIGNED && _CHAR_UNSIGNED)  \
    || (defined __CHAR_SIGNED__ && !__CHAR_SIGNED__) || (defined CHAR_MIN && !CHAR_MIN)
  #define __CCW_CHAR_IS_UNSIGNED  1
 #else
  #define __CCW_CHAR_IS_UNSIGNED  0
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

// Out-of-class definition of `value`, for ODR-use. The in-class initialiser is only a
// declaration before C++17 (and `static const`, unlike `static constexpr`, is never
// implicitly inline), so binding `value` to a reference -- passing it to a function
// taking `const T&` -- needs this to link. Watcom mis-parses the static-member
// definition of a template with a non-type parameter, so it is omitted there.
#if !defined(__WATCOMC__)
template <class T, T V>
const T integral_constant<T, V>::value;
#endif

#if 1
template<bool V>
struct bool_constant : integral_constant<bool, V> {};
#else
template<bool V>
struct bool_constant {
    typedef bool_constant   type;
    typedef bool            value_type;
    static const bool       value = V;
};
#endif

// true_type, false_type
typedef bool_constant<true>  true_type;
typedef bool_constant<false> false_type;

#define __CCW_TRAITS_SPEC0(S, V) template <> struct S : public integral_constant<bool, V> {}
#define __CCW_TRAITS_SPEC1(S, V) template <class T> struct S : public integral_constant<bool, V> {}
#define __CCW_TRAITS_SPEC2(S, V) template <class T, class ClassName> struct S : public integral_constant<bool, V> {}
// T, T const, T volatile, T const volatile
#define __CCW_TRAITS_SPEC(Order, Traits, SpecialType, Value)                                  \
    __CCW_TRAITS_M_CAT(__CCW_TRAITS_SPEC,Order)(Traits<SpecialType>,                Value); \
    __CCW_TRAITS_M_CAT(__CCW_TRAITS_SPEC,Order)(Traits<SpecialType const>,          Value); \
    __CCW_TRAITS_M_CAT(__CCW_TRAITS_SPEC,Order)(Traits<SpecialType volatile>,       Value); \
    __CCW_TRAITS_M_CAT(__CCW_TRAITS_SPEC,Order)(Traits<SpecialType const volatile>, Value)

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
__CCW_TRAITS_SPEC(0, is_void, void, true);

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
__CCW_TRAITS_SPEC(1, is_pointer, T*, true);
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
__CCW_TRAITS_SPEC(2, is_member_pointer, T ClassName::*, true);
#endif

// is_integral
template <class T> struct is_integral : public false_type {};
__CCW_TRAITS_SPEC(0, is_integral, bool,               true);
__CCW_TRAITS_SPEC(0, is_integral, char,               true);
__CCW_TRAITS_SPEC(0, is_integral, signed char,        true);
__CCW_TRAITS_SPEC(0, is_integral, unsigned char,      true);
__CCW_TRAITS_SPEC(0, is_integral, wchar_t,            true);
__CCW_TRAITS_SPEC(0, is_integral, short,              true);
__CCW_TRAITS_SPEC(0, is_integral, unsigned short,     true);
__CCW_TRAITS_SPEC(0, is_integral, int,                true);
__CCW_TRAITS_SPEC(0, is_integral, unsigned int,       true);
__CCW_TRAITS_SPEC(0, is_integral, long,               true);
__CCW_TRAITS_SPEC(0, is_integral, unsigned long,      true);
__CCW_TRAITS_SPEC(0, is_integral, long long,          true);
__CCW_TRAITS_SPEC(0, is_integral, unsigned long long, true);

// is_floating_point
template <class T> struct is_floating_point : public false_type {};
__CCW_TRAITS_SPEC(0, is_floating_point, float         , true);
__CCW_TRAITS_SPEC(0, is_floating_point, double        , true);
__CCW_TRAITS_SPEC(0, is_floating_point, long double   , true);

// is_signed
template <class T> struct is_signed : public false_type {};
__CCW_TRAITS_SPEC(0, is_signed, signed char   , true);
__CCW_TRAITS_SPEC(0, is_signed, short         , true);
__CCW_TRAITS_SPEC(0, is_signed, int           , true);
__CCW_TRAITS_SPEC(0, is_signed, long          , true);
__CCW_TRAITS_SPEC(0, is_signed, long long     , true);
__CCW_TRAITS_SPEC(0, is_signed, float         , true);
__CCW_TRAITS_SPEC(0, is_signed, double        , true);
__CCW_TRAITS_SPEC(0, is_signed, long double   , true);
#if !__CCW_CHAR_IS_UNSIGNED
__CCW_TRAITS_SPEC(0, is_signed, char          , true);
#endif

// is_unsigned
template <class T> struct is_unsigned : public false_type {};
__CCW_TRAITS_SPEC(0, is_unsigned, bool                , true);
__CCW_TRAITS_SPEC(0, is_unsigned, wchar_t             , true);
__CCW_TRAITS_SPEC(0, is_unsigned, unsigned char       , true);
__CCW_TRAITS_SPEC(0, is_unsigned, unsigned short      , true);
__CCW_TRAITS_SPEC(0, is_unsigned, unsigned int        , true);
__CCW_TRAITS_SPEC(0, is_unsigned, unsigned long       , true);
__CCW_TRAITS_SPEC(0, is_unsigned, unsigned long long  , true);
#if __CCW_CHAR_IS_UNSIGNED
__CCW_TRAITS_SPEC(0, is_unsigned, char                , true);
#endif



#define __CCW_MAKE_ST_TYPE(F,S,T)   \
    template <> struct F<S> {       \
        typedef T type;             \
    }
template <class T> struct make_signed { };
__CCW_MAKE_ST_TYPE(make_signed, char            , signed char);
__CCW_MAKE_ST_TYPE(make_signed, signed char     , signed char);
__CCW_MAKE_ST_TYPE(make_signed, unsigned char   , signed char);
__CCW_MAKE_ST_TYPE(make_signed, short           , short);
__CCW_MAKE_ST_TYPE(make_signed, unsigned short  , short);
__CCW_MAKE_ST_TYPE(make_signed, int             , int);
__CCW_MAKE_ST_TYPE(make_signed, unsigned int    , int);
__CCW_MAKE_ST_TYPE(make_signed, long            , long);
__CCW_MAKE_ST_TYPE(make_signed, unsigned long   , long);
__CCW_MAKE_ST_TYPE(make_signed, _ccw_llong , _ccw_llong);
__CCW_MAKE_ST_TYPE(make_signed, _ccw_ullong, _ccw_llong);

template <class T> struct make_unsigned { };
__CCW_MAKE_ST_TYPE(make_unsigned, char          , unsigned char);
__CCW_MAKE_ST_TYPE(make_unsigned, signed char   , unsigned char);
__CCW_MAKE_ST_TYPE(make_unsigned, unsigned char , unsigned char);
__CCW_MAKE_ST_TYPE(make_unsigned, short         , unsigned short);
__CCW_MAKE_ST_TYPE(make_unsigned, unsigned short, unsigned short);
__CCW_MAKE_ST_TYPE(make_unsigned, int           , unsigned int);
__CCW_MAKE_ST_TYPE(make_unsigned, unsigned int  , unsigned int);
__CCW_MAKE_ST_TYPE(make_unsigned, long          , unsigned long);
__CCW_MAKE_ST_TYPE(make_unsigned, unsigned long , unsigned long);
__CCW_MAKE_ST_TYPE(make_unsigned, _ccw_llong    , _ccw_ullong);
__CCW_MAKE_ST_TYPE(make_unsigned, _ccw_ullong   , _ccw_ullong);
#undef __CCW_MAKE_ST_TYPE

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

 #if defined(__CCW_IS_UNION) && defined(__CCW_IS_CLASS)
    template<class T>
    struct is_union_or_class : public integral_constant<bool, __CCW_IS_UNION(T) || __CCW_IS_CLASS(T)> {};
 #elif defined __DMC__
    template<class T>
    class is_union_or_class_helper {
        template<class U> static yes_t test(int U::*);
        template<class U> static no_t  test(...);
    public:
        static const bool value = __CCW_IS_SIZEOF_1( test<T>(0) );
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
            , __CCW_IS_SIZEOF_1( is_union_or_class_helper::test< typename remove_cv<T>::type >(0) ) >
    {};
 #endif
}

// is_union
#ifdef __CCW_IS_UNION
template <class T> struct is_union : public integral_constant<bool, __CCW_IS_UNION(T)> {};
#else
template <class T> struct is_union : public false_type {};
#endif

// is_class
#ifdef __CCW_IS_CLASS
template <class T> struct is_class : public integral_constant<bool, __CCW_IS_CLASS(T)> {};
#else
template<class T>
struct is_class : public integral_constant<bool, !is_union<T>::value && detail::is_union_or_class<T>::value  > {};
#endif

// is_empty
#ifdef __CCW_IS_EMPTY
template <class T> struct is_empty : public integral_constant<bool, __CCW_IS_EMPTY(T)> {};
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
#ifdef __CCW_IS_POLYMORPHIC
template <class T> struct is_polymorphic : public integral_constant<bool, __CCW_IS_POLYMORPHIC(T)> {};
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


#if !defined(__WATCOMC__)

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

// is_array
template <class T> struct is_array : public false_type {};
template <class T> struct is_array<T[]> : public true_type {};
template <class TB, int N> struct is_array<TB[N]> : public true_type {};

// is_function
namespace detail {
    template<class T>
    class in_array {
        template<class U> static yes_t test(U(*)[1]);
        template<class U> static no_t  test(...);
    public:
        static const bool value = __CCW_IS_SIZEOF_1(test<T>(0));
    };
}
template<class T>
struct is_function
    : public integral_constant<bool , !(  detail::is_union_or_class<T>::value
                                       || is_reference<T>::value
                                       || is_void<T>::value
                                       || detail::in_array<T>::value
                                       )>
{};


#else // __WATCOMC__ // use bugs

// rank
template<class T> struct rank : integral_constant<unsigned, 0> {};
template<class T> struct rank<T[]> : public integral_constant<unsigned, rank<T>::value + 1> {};
//template<class TB, unsigned N> struct rank<TB[N]> : public integral_constant<unsigned, rank<TB>::value + 1> {};

// remove_extent
template<class T> struct remove_extent { typedef T type; };
template<class T> struct remove_extent<T[]> { typedef T type; };
//template<class TB, int N> struct remove_extent<TB[N]> { typedef TB type; };

// remove_all_extents
template <class T > struct remove_all_extents { typedef T type; };
template <class TB> struct remove_all_extents<TB[]> { typedef typename remove_all_extents<TB>::type type; };
//template <class TB2, int N> struct remove_all_extents<TB2[N]> { typedef typename remove_all_extents<TB2>::type type; };

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
        static const bool value = __CCW_IS_SIZEOF_1(test<rcvT>(0));
    };

    // value=0: U(), AbsClass, U[], U[N]
    template<class T>
    class is_funcret_or_inarray {
        template<class U> static yes_t test(U());
        template<class U> static no_t  test(...);
    public:
        static const bool value = __CCW_IS_SIZEOF_1(test<T>(0));
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

#endif  // __CCW_USE_WATCOMC_BUG


// is_abstract
#ifdef __CCW_IS_ABSTRACT
template <class T> struct is_abstract : public integral_constant<bool, __CCW_IS_ABSTRACT(T)> {};
#else
template <class T>
struct is_abstract : public integral_constant<bool,
    (detail::is_union_or_class<T>::value && !detail::is_funcret_or_inarray<T>::value)>
{};
#endif

// is_member_object_pointer
template <class T>
struct is_member_object_pointer : public  false_type {};
__CCW_TRAITS_SPEC(2, is_member_object_pointer, T ClassName::*, !is_function<T>::value);

// is_member_function_pointer
template <class T>
struct is_member_function_pointer : public  false_type {};
__CCW_TRAITS_SPEC(2, is_member_function_pointer, T ClassName::*, is_function<T>::value);

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
#ifdef __CCW_IS_ENUM
template <class T> struct is_enum : public integral_constant<bool, __CCW_IS_ENUM(T)> {};
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
#ifdef __CCW_IS_BASE_OF
template <class B, class D> struct is_base_of : public integral_constant<bool, __CCW_IS_BASE_OF(B,D)> {};
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
        static const bool value = __CCW_IS_SIZEOF_1(test(Conv(), 0));
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
        static const bool value = __CCW_IS_SIZEOF_1(test(make_from()));
        //static const bool value = __CCW_IS_SIZEOF_1(test(s_from));
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

// type_identity (C++20)
template <class T> struct type_identity { typedef T type; };

// remove_cvref (C++20)
template <class T>
struct remove_cvref { typedef typename remove_cv<typename remove_reference<T>::type>::type type; };

// alignment_of (C++11) -- MSVC __alignof is available on vc8/9.
#if defined(_MSC_VER)
template <class T> struct alignment_of : public integral_constant<std::size_t, __alignof(T)> {};
#elif defined(__GNUC__) || defined(__clang__)
template <class T> struct alignment_of : public integral_constant<std::size_t, __alignof__(T)> {};
#endif

// extent (C++11)
template <class T, unsigned N = 0> struct extent : public integral_constant<std::size_t, 0> {};
template <class T> struct extent<T[], 0> : public integral_constant<std::size_t, 0> {};
template <class T, unsigned N> struct extent<T[], N> : public extent<T, N - 1> {};
template <class T, std::size_t I> struct extent<T[I], 0> : public integral_constant<std::size_t, I> {};
template <class T, std::size_t I, unsigned N> struct extent<T[I], N> : public extent<T, N - 1> {};

// underlying_type (C++11)
// vc8/9 lack the __underlying_type intrinsic, so pick a same-size signed integer
// (best effort). Most VC enums have int underlying type; the value cast used by
// to_underlying is bit-preserving for same-size types.
template <class E>
struct underlying_type {
    typedef typename conditional<sizeof(E) == 1, signed char,
            typename conditional<sizeof(E) == 2, short,
            typename conditional<sizeof(E) == 4, int, _ccw_llong>::type>::type>::type type;
};


// --------------------------------------------------------------------------
//TODO:
// tiny...

// is_pod
#ifdef __CCW_IS_POD
template <class T> struct is_pod : public integral_constant<bool, __CCW_IS_POD(T)> {};
#else
template <class TB>
struct is_pod : public integral_constant<bool
                , !is_void<TB>::value && (is_scalar<typename remove_all_extents<TB>::type>::value) >
{};
#endif

#if 0
// is_literal_type
template <class T> struct is_literal_type : public is_scalar<T> {};

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

#endif

// common_type
//
// The standard defines this as the type of `true ? declval<T>() : declval<U>()`, which
// needs decltype -- absent here. SUBSET: the answer is computed from the usual
// arithmetic conversions instead, so it is right for
//   - any T with itself (including class types), and
//   - any mix of arithmetic types, which is what chrono's Rep and the numeric
//     algorithms ask about.
// It is NOT right for unrelated class types with a conversion between them, or for a
// base/derived pointer pair. Those get the first operand rather than an error, since
// there is no way to detect the conversion without decltype -- so do not rely on
// common_type for class types other than the same-type case.
//
// One deliberate flattening: types of equal width are not told apart, so
// common_type<long, int>::type is `int` where the standard says `long`. On every
// target here the two are layout-identical, and asking for that pair is rare;
// common_type<long, long> still answers `long` through the same-type case.

// Integral types narrower than int promote to int, exactly as the conversions do.
template <class T> struct __ccw_ct_promote                 { typedef T type; };
template <> struct __ccw_ct_promote<bool>                  { typedef int type; };
template <> struct __ccw_ct_promote<char>                  { typedef int type; };
template <> struct __ccw_ct_promote<signed char>           { typedef int type; };
template <> struct __ccw_ct_promote<unsigned char>         { typedef int type; };
template <> struct __ccw_ct_promote<short>                 { typedef int type; };
template <> struct __ccw_ct_promote<unsigned short>        { typedef int type; };
#if !defined(_MSC_VER) || defined(_NATIVE_WCHAR_T_DEFINED)
template <> struct __ccw_ct_promote<wchar_t>               { typedef int type; };
#endif

// Rank orders the promoted types by conversion strength. ODD is signed, EVEN unsigned
// -- the parity is what the width fix-up below reads, so keep it if you extend this.
template <class T> struct __ccw_ct_rank { static const int value = 0; };
template <> struct __ccw_ct_rank<int>                { static const int value = 1; };
template <> struct __ccw_ct_rank<unsigned int>       { static const int value = 2; };
template <> struct __ccw_ct_rank<long>               { static const int value = 3; };
template <> struct __ccw_ct_rank<unsigned long>      { static const int value = 4; };
template <> struct __ccw_ct_rank<long long>          { static const int value = 5; };
template <> struct __ccw_ct_rank<unsigned long long> { static const int value = 6; };
template <> struct __ccw_ct_rank<float>              { static const int value = 7; };
template <> struct __ccw_ct_rank<double>             { static const int value = 8; };
template <> struct __ccw_ct_rank<long double>        { static const int value = 9; };

template <class A, class B, bool AFirst> struct __ccw_ct_pick { typedef A hi; typedef B lo; };
template <class A, class B> struct __ccw_ct_pick<A, B, false> { typedef B hi; typedef A lo; };

template <bool ToUnsigned, class Hi> struct __ccw_ct_fix      { typedef Hi type; };
template <class Hi> struct __ccw_ct_fix<true, Hi> { typedef typename make_unsigned<Hi>::type type; };

template <class A, class B>
struct __ccw_ct_arith {
    typedef typename __ccw_ct_promote<A>::type PA;
    typedef typename __ccw_ct_promote<B>::type PB;
    typedef __ccw_ct_pick<PA, PB, (__ccw_ct_rank<PA>::value >= __ccw_ct_rank<PB>::value)> _P;
    typedef typename _P::hi Hi;
    typedef typename _P::lo Lo;
    // A signed winner no wider than an unsigned loser cannot hold it, so the
    // conversions pick the unsigned counterpart instead. `unsigned int` against `long`
    // on Windows is the case this catches.
    typedef typename __ccw_ct_fix<
        (__ccw_ct_rank<Hi>::value <= 6 &&
         __ccw_ct_rank<Hi>::value % 2 == 1 &&
         __ccw_ct_rank<Lo>::value % 2 == 0 &&
         __ccw_ct_rank<Lo>::value != 0 &&
         sizeof(Hi) == sizeof(Lo)), Hi>::type type;
};

template <class T, class U, bool Same> struct __ccw_ct2 {
    typedef typename __ccw_ct_arith<T, U>::type type;
};
template <class T, class U> struct __ccw_ct2<T, U, true> { typedef T type; };

template <class T, class U = void, class V = void, class W = void>
struct common_type {
    typedef typename common_type<typename common_type<T, U>::type, V, W>::type type;
};
template <class T>
struct common_type<T, void, void, void> { typedef typename decay<T>::type type; };
template <class T, class U>
struct common_type<T, U, void, void> {
    typedef typename __ccw_ct2<typename decay<T>::type, typename decay<U>::type,
                               is_same<typename decay<T>::type, typename decay<U>::type>::value>::type type;
};
template <class T, class U, class V>
struct common_type<T, U, V, void> {
    typedef typename common_type<typename common_type<T, U>::type, V>::type type;
};

// --------------------------------------------------------------------------

} // namespace std


#undef __CCW_TRAITS_SPEC0
#undef __CCW_TRAITS_SPEC1
#undef __CCW_TRAITS_SPEC2
#undef __CCW_TRAITS_SPEC
#undef __CCW_IS_SIZEOF_1
#undef __CCW_TRAITS_M_CAT
#undef __CCW_TRAITS_M_CAT_B
#undef __CCW_TRAITS_M_CAT_C

#endif  // __CCW_CXX11

#endif  // _CCW_DETAIL_TYPE_TRAITS_HPP
