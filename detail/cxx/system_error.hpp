/**
 *  @file   detail/system_error.hpp
 *  @brief  <system_error> gap-fills: a minimal error_category / error_code for
 *          compilers with no native <system_error>, plus a real std::errc struct.
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_DETAIL_SYSTEM_ERROR_HPP
#define _CCW_DETAIL_SYSTEM_ERROR_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>

#if _CCW_STD_LIB_LT(1600, 201103L)
#include <string>
#include <stdexcept>   // runtime_error (system_error derives from it)
#include <cerrno>      // the E* macros the errc enumerators are defined from
#include <iosfwd>      // basic_ostream, for the error_code inserter below
#ifndef _CCW_SYSERR_MIN_DEFINED
#define _CCW_SYSERR_MIN_DEFINED
namespace std {

class error_condition;
class error_code;
struct errc;      // the error CONDITION enumeration, defined further down
struct io_errc;   // the iostream error CODE enumeration

class error_category {
public:
    virtual ~error_category() {}
    virtual const char* name() const { return "generic"; }
    virtual std::string message(int) const { return "error"; }
    inline error_condition default_error_condition(int __v) const;
    inline bool equivalent(int __code, const error_condition& __cond) const;
    inline bool equivalent(const error_code& __code, int __cond) const;
    bool operator==(const error_category& o) const { return this == &o; }
    bool operator!=(const error_category& o) const { return this != &o; }
    bool operator<(const error_category& o) const { return this < &o; }
    error_category() {}
private:
    error_category(const error_category&);
    error_category& operator=(const error_category&);
};
inline const error_category& generic_category() { static error_category c; return c; }
inline const error_category& system_category()  { static error_category c; return c; }

class error_code {
    int                   v_;
    const error_category* cat_;
    typedef int error_code::* _bt;
public:
    error_code() : v_(0), cat_(&generic_category()) {}
    error_code(int v, const error_category& c) : v_(v), cat_(&c) {}
    void assign(int v, const error_category& c) { v_ = v; cat_ = &c; }
    void clear() { v_ = 0; cat_ = &generic_category(); }
    int  value() const { return v_; }
    const error_category& category() const { return *cat_; }
    std::string message() const { return cat_->message(v_); }
    inline error_condition default_error_condition() const;
    inline error_code(io_errc __e);
    inline error_code& operator=(io_errc __e);
    operator _bt() const { return v_ ? &error_code::v_ : (_bt)0; }
};
inline bool operator==(const error_code& a, const error_code& b) { return a.value() == b.value() && a.category() == b.category(); }
inline bool operator!=(const error_code& a, const error_code& b) { return !(a == b); }
inline bool operator<(const error_code& a, const error_code& b) {
    return a.category() != b.category() ? &a.category() < &b.category() : a.value() < b.value();
}
class error_condition {
    int                   v_;
    const error_category* cat_;
    typedef int error_condition::* _bt;
public:
    error_condition() : v_(0), cat_(&generic_category()) {}
    error_condition(int v, const error_category& c) : v_(v), cat_(&c) {}
    void assign(int v, const error_category& c) { v_ = v; cat_ = &c; }
    void clear() { v_ = 0; cat_ = &generic_category(); }
    int  value() const { return v_; }
    const error_category& category() const { return *cat_; }
    std::string message() const { return cat_->message(v_); }
    inline error_condition(errc __e);
    inline error_condition& operator=(errc __e);
    operator _bt() const { return v_ ? &error_condition::v_ : (_bt)0; }
};
inline bool operator==(const error_condition& a, const error_condition& b) { return a.value() == b.value() && a.category() == b.category(); }
inline bool operator!=(const error_condition& a, const error_condition& b) { return !(a == b); }
inline bool operator<(const error_condition& a, const error_condition& b) {
    return a.category() != b.category() ? a.category() < b.category() : a.value() < b.value();
}

inline error_condition error_code::default_error_condition() const { return cat_->default_error_condition(v_); }
inline error_condition error_category::default_error_condition(int __v) const { return error_condition(__v, *this); }
inline bool error_category::equivalent(int __code, const error_condition& __cond) const {
    return default_error_condition(__code) == __cond;
}
inline bool error_category::equivalent(const error_code& __code, int __cond) const {
    return *this == __code.category() && __code.value() == __cond;
}

inline bool operator==(const error_code& a, const error_condition& b) {
    return a.category().equivalent(a.value(), b) || b.category().equivalent(a, b.value());
}
inline bool operator==(const error_condition& a, const error_code& b) { return b == a; }
inline bool operator!=(const error_code& a, const error_condition& b) { return !(a == b); }
inline bool operator!=(const error_condition& a, const error_code& b) { return !(a == b); }

class system_error : public runtime_error {
    error_code __c_;
    static string __compose(const error_code& __ec, const string& __what) {
        return __what.empty() ? __ec.message() : __what + ": " + __ec.message();
    }
public:
    system_error(error_code __ec, const string& __what) : runtime_error(__compose(__ec, __what)), __c_(__ec) {}
    system_error(error_code __ec, const char* __what)   : runtime_error(__compose(__ec, string(__what))), __c_(__ec) {}
    explicit system_error(error_code __ec)              : runtime_error(__ec.message()), __c_(__ec) {}
    system_error(int __v, const error_category& __cat, const string& __what)
        : runtime_error(__compose(error_code(__v, __cat), __what)), __c_(__v, __cat) {}
    system_error(int __v, const error_category& __cat)
        : runtime_error(error_code(__v, __cat).message()), __c_(__v, __cat) {}
    const error_code& code() const throw() { return __c_; }
};

template <class _CharT, class _Traits>
basic_ostream<_CharT, _Traits>& operator<<(basic_ostream<_CharT, _Traits>& __os, const error_code& __ec) {
    return __os << __ec.category().name() << ':' << __ec.value();
}

}   // namespace std
#endif  // _CCW_SYSERR_MIN_DEFINED
#endif  // _CCW_STD_LIB_LT(1600, 201103L)

#if _CCW_STD_LIB_LT(1800, 201103L)
#include <cerrno>      // the E* macros the network/stream enumerators are defined from
#if _CCW_STD_LIB_LT(1600, 201103L)
#ifndef _CCW_ERRC_TRAITS_DEFINED
#define _CCW_ERRC_TRAITS_DEFINED
namespace std {
template <class _Tp> struct is_error_code_enum      { static const bool value = false; };
template <class _Tp> struct is_error_condition_enum { static const bool value = false; };
}
#endif
#endif

#ifndef _CCW_ERRC_STRUCT_DEFINED
#define _CCW_ERRC_STRUCT_DEFINED
namespace std {

struct errc {
    enum enum_type {
        _ccw_success                = 0,
        operation_not_permitted     = EPERM,
        no_such_file_or_directory   = ENOENT,
        no_such_process             = ESRCH,
        interrupted                 = EINTR,
        io_error                    = EIO,
        no_such_device_or_address   = ENXIO,
        argument_list_too_long      = E2BIG,
        executable_format_error     = ENOEXEC,
        bad_file_descriptor         = EBADF,
        no_child_process            = ECHILD,
        resource_unavailable_try_again = EAGAIN,
        not_enough_memory           = ENOMEM,
        permission_denied           = EACCES,
        bad_address                 = EFAULT,
        device_or_resource_busy     = EBUSY,
        file_exists                 = EEXIST,
        cross_device_link           = EXDEV,
        no_such_device              = ENODEV,
        not_a_directory             = ENOTDIR,
        is_a_directory              = EISDIR,
        invalid_argument            = EINVAL,
        too_many_files_open_in_system = ENFILE,
        too_many_files_open         = EMFILE,
        file_too_large              = EFBIG,
        no_space_on_device          = ENOSPC,
        invalid_seek                = ESPIPE,
        read_only_file_system       = EROFS,
        too_many_links              = EMLINK,
        broken_pipe                 = EPIPE,
        argument_out_of_domain      = EDOM,
        result_out_of_range         = ERANGE,
        resource_deadlock_would_occur = EDEADLK,
        filename_too_long           = ENAMETOOLONG,
        no_lock_available           = ENOLCK,
        function_not_supported      = ENOSYS,
        directory_not_empty         = ENOTEMPTY,
        illegal_byte_sequence       = EILSEQ,
        not_supported               = ENOTSUP,
        value_too_large             = EOVERFLOW,
        address_family_not_supported   = EAFNOSUPPORT,
        address_in_use                 = EADDRINUSE,
        address_not_available          = EADDRNOTAVAIL,
        already_connected              = EISCONN,
        bad_message                    = EBADMSG,
        connection_aborted             = ECONNABORTED,
        connection_already_in_progress = EALREADY,
        connection_refused             = ECONNREFUSED,
        connection_reset               = ECONNRESET,
        destination_address_required   = EDESTADDRREQ,
        host_unreachable               = EHOSTUNREACH,
        identifier_removed             = EIDRM,
        inappropriate_io_control_operation = ENOTTY,
        message_size                   = EMSGSIZE,
        network_down                   = ENETDOWN,
        network_reset                  = ENETRESET,
        network_unreachable            = ENETUNREACH,
        no_buffer_space                = ENOBUFS,
        no_link                        = ENOLINK,
        no_message                     = ENOMSG,
        no_message_available           = ENODATA,
        no_protocol_option             = ENOPROTOOPT,
        no_stream_resources            = ENOSR,
        not_a_socket                   = ENOTSOCK,
        not_a_stream                   = ENOSTR,
        not_connected                  = ENOTCONN,
        operation_canceled             = ECANCELED,
        operation_in_progress          = EINPROGRESS,
        operation_not_supported        = EOPNOTSUPP,
        operation_would_block          = EWOULDBLOCK,
        owner_dead                     = EOWNERDEAD,
        protocol_error                 = EPROTO,
        protocol_not_supported         = EPROTONOSUPPORT,
        state_not_recoverable          = ENOTRECOVERABLE,
        stream_timeout                 = ETIME,
        text_file_busy                 = ETXTBSY,
        timed_out                      = ETIMEDOUT,
        too_many_symbolic_link_levels  = ELOOP,
        wrong_protocol_type            = EPROTOTYPE
    };
    errc() : v_(_ccw_success) {}
    errc(enum_type v) : v_(v) {}
    operator enum_type() const { return v_; }
private:
    enum_type v_;
};

inline bool operator==(errc __a, errc __b) { return errc::enum_type(__a) == errc::enum_type(__b); }
inline bool operator!=(errc __a, errc __b) { return !(__a == __b); }
inline bool operator==(errc __a, errc::enum_type __b) { return errc::enum_type(__a) == __b; }
inline bool operator==(errc::enum_type __a, errc __b) { return __a == errc::enum_type(__b); }
inline bool operator!=(errc __a, errc::enum_type __b) { return !(__a == __b); }
inline bool operator!=(errc::enum_type __a, errc __b) { return !(__a == __b); }

inline error_code make_error_code(errc::enum_type e) { return error_code(int(e), generic_category()); }
inline error_code make_error_code(errc e)       { return error_code(int(errc::enum_type(e)), generic_category()); }
inline error_condition make_error_condition(errc::enum_type e) { return error_condition(int(e), generic_category()); }
inline error_condition make_error_condition(errc e) { return error_condition(int(errc::enum_type(e)), generic_category()); }

#if _CCW_STD_LIB_LT(1600, 201103L)   // vc10/11 keep their own io_errc (see vc/system_error)
#if _CCW_STD_LIB_LT(1600, 201103L)
class __ccw_iostream_category : public error_category {
public:
    virtual const char* name() const { return "iostream"; }
    virtual std::string message(int __v) const { return __v == 1 ? "unspecified iostream_category error" : "iostream error"; }
};
inline const error_category& iostream_category() { static __ccw_iostream_category __c; return __c; }
#endif

struct io_errc {
    enum enum_type { stream = 1 };
    io_errc() : v_(stream) {}
    io_errc(enum_type __v) : v_(__v) {}
    operator enum_type() const { return v_; }
private:
    enum_type v_;
};
inline bool operator==(io_errc __a, io_errc __b) { return io_errc::enum_type(__a) == io_errc::enum_type(__b); }
inline bool operator!=(io_errc __a, io_errc __b) { return !(__a == __b); }

inline error_code make_error_code(io_errc __e) { return error_code(int(io_errc::enum_type(__e)), iostream_category()); }
inline error_code make_error_code(io_errc::enum_type __e) { return error_code(int(__e), iostream_category()); }
inline error_condition make_error_condition(io_errc __e) { return error_condition(int(io_errc::enum_type(__e)), iostream_category()); }
inline error_condition make_error_condition(io_errc::enum_type __e) { return error_condition(int(__e), iostream_category()); }
#if _CCW_STD_LIB_LT(1600, 201103L)   // see the errc note below: C2908 on vc10/11
template <> struct is_error_code_enum<io_errc>            { static const bool value = true; };
template <> struct is_error_code_enum<io_errc::enum_type> { static const bool value = true; };
#endif

#if _CCW_STD_LIB_LT(1600, 201103L)   // native error_code cannot take new members
inline error_code::error_code(io_errc __e) : v_(int(io_errc::enum_type(__e))), cat_(&iostream_category()) {}
inline error_code& error_code::operator=(io_errc __e) { assign(int(io_errc::enum_type(__e)), iostream_category()); return *this; }
#endif

#endif

#if _CCW_STD_LIB_LT(1600, 201103L)
template <> struct is_error_condition_enum<errc>            { static const bool value = true; };
template <> struct is_error_condition_enum<errc::enum_type> { static const bool value = true; };
#endif

#if _CCW_STD_LIB_LT(1600, 201103L)   // native error_condition cannot take new members
inline error_condition::error_condition(errc __e) : v_(int(errc::enum_type(__e))), cat_(&generic_category()) {}
inline error_condition& error_condition::operator=(errc __e) { assign(int(errc::enum_type(__e)), generic_category()); return *this; }
#endif

#if !_CCW_STD_LIB_LT(1600, 201103L)
inline bool operator==(const error_code& __a, errc __b)      { return __a == make_error_condition(__b); }
inline bool operator==(errc __a, const error_code& __b)      { return make_error_condition(__a) == __b; }
inline bool operator!=(const error_code& __a, errc __b)      { return !(__a == __b); }
inline bool operator!=(errc __a, const error_code& __b)      { return !(__a == __b); }
inline bool operator==(const error_condition& __a, errc __b) { return __a == make_error_condition(__b); }
inline bool operator==(errc __a, const error_condition& __b) { return make_error_condition(__a) == __b; }
inline bool operator!=(const error_condition& __a, errc __b) { return !(__a == __b); }
inline bool operator!=(errc __a, const error_condition& __b) { return !(__a == __b); }
#endif

}   // namespace std
#endif  // _CCW_ERRC_STRUCT_DEFINED
#endif  // _CCW_STD_LIB_LT(1800, 201103L)

#if _CCW_STD_LIB_LT(1600, 201103L)
namespace std {
template <class _Tp> struct hash;
template <> struct hash< ::std::error_code> {
    typedef ::std::error_code argument_type;
    typedef ::std::size_t     result_type;
    ::std::size_t operator()(const ::std::error_code& __e) const { return (::std::size_t)__e.value(); }
};
}
#endif
#if defined(__GLIBCXX__) || defined(_LIBCPP_VERSION)
#  define _CCW_HAS_HASH_ERROR_CONDITION (__cplusplus >= 201703L)
#elif defined(_MSC_VER) && _MSC_VER == 1600
#  define _CCW_HAS_HASH_ERROR_CONDITION 1
#elif defined(__cpp_lib_unordered_map_try_emplace)
#  define _CCW_HAS_HASH_ERROR_CONDITION 1
#else
#  define _CCW_HAS_HASH_ERROR_CONDITION 0
#endif
#if !_CCW_HAS_HASH_ERROR_CONDITION
namespace std {
template <class _Tp> struct hash;
template <> struct hash< ::std::error_condition> {
    typedef ::std::error_condition argument_type;
    typedef ::std::size_t          result_type;
    ::std::size_t operator()(const ::std::error_condition& __e) const { return (::std::size_t)__e.value(); }
};
}
#endif

#if defined(__cplusplus) && !defined(__cpp_lib_three_way_comparison)
namespace _CCW_STD20 {
inline bool operator> (const ::std::error_code& __a, const ::std::error_code& __b) { return __b < __a; }
inline bool operator<=(const ::std::error_code& __a, const ::std::error_code& __b) { return !(__b < __a); }
inline bool operator>=(const ::std::error_code& __a, const ::std::error_code& __b) { return !(__a < __b); }
inline bool operator> (const ::std::error_condition& __a, const ::std::error_condition& __b) { return __b < __a; }
inline bool operator<=(const ::std::error_condition& __a, const ::std::error_condition& __b) { return !(__b < __a); }
inline bool operator>=(const ::std::error_condition& __a, const ::std::error_condition& __b) { return !(__a < __b); }
}
#endif

#endif  // _CCW_DETAIL_SYSTEM_ERROR_HPP
