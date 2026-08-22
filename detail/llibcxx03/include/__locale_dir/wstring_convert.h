//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//  __locale_dir/wstring_convert.h -- [conversions.string] wstring_convert and
//  [conversions.buffer] wbuffer_convert. Split out of <locale> so that a native
//  <locale> without them (MSVC before VS2010) can pull just these two in.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___LOCALE_DIR_WSTRING_CONVERT_H
#define _CCW_LIBCPP___LOCALE_DIR_WSTRING_CONVERT_H
#include "../__config"
#include <memory>
#include <stdexcept>
#include <streambuf>
#include <string>

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

#if defined(__WATCOMC__)
_CCW_LIBCPP_HIDE_FROM_ABI void __ccw_throw_range_error(const char* __what);   // out of line: A55
#else
_CCW_LIBCPP_HIDE_FROM_ABI inline void __ccw_throw_range_error(const char* __what) {
    _CCW_THROW(_CCW_STD::range_error(__what));
}
#endif

template <class _Codecvt>
struct __ccw_cvt_owner : _Codecvt {
    _CCW_LIBCPP_HIDE_FROM_ABI static void __destroy(_Codecvt* __p) { delete static_cast<__ccw_cvt_owner*>(__p); }
};

template <class _Codecvt, class _Elem = wchar_t,
          class _WideAlloc = allocator<_Elem>, class _ByteAlloc = allocator<char> >
class _CCW_LIBCPP_TEMPLATE_VIS wstring_convert {
public:
    typedef _CCW_STD::basic_string<char, char_traits<char>, _ByteAlloc>   byte_string;
    typedef _CCW_STD::basic_string<_Elem, char_traits<_Elem>, _WideAlloc> wide_string;
    typedef typename _Codecvt::state_type                                 state_type;
    typedef typename wide_string::traits_type::int_type                   int_type;
private:
    _Codecvt*        __cvt_;
    state_type       __st_;
    byte_string      __berr_;
    wide_string      __werr_;
    bool             __hasberr_;
    bool             __haswerr_;
    _CCW_STD::size_t __count_;

    wstring_convert(const wstring_convert&);
    wstring_convert& operator=(const wstring_convert&);

    _CCW_LIBCPP_HIDE_FROM_ABI void __zero_state() {
        char* __q = (char*)(void*)&__st_;
        for (_CCW_STD::size_t __i = 0; __i < sizeof(state_type); ++__i) __q[__i] = 0;
    }
public:
    _CCW_LIBCPP_HIDE_FROM_ABI wstring_convert()
        : __cvt_(new _Codecvt), __hasberr_(false), __haswerr_(false), __count_(0) { __zero_state(); }
    _CCW_LIBCPP_HIDE_FROM_ABI explicit wstring_convert(_Codecvt* __pcvt)
        : __cvt_(__pcvt), __hasberr_(false), __haswerr_(false), __count_(0) { __zero_state(); }
    _CCW_LIBCPP_HIDE_FROM_ABI wstring_convert(_Codecvt* __pcvt, state_type __state)
        : __cvt_(__pcvt), __st_(__state), __hasberr_(false), __haswerr_(false), __count_(0) {}
    _CCW_LIBCPP_HIDE_FROM_ABI explicit wstring_convert(const byte_string& __berr,
                                                       const wide_string& __werr = wide_string())
        : __cvt_(new _Codecvt), __berr_(__berr), __werr_(__werr),
          __hasberr_(true), __haswerr_(true), __count_(0) { __zero_state(); }
    _CCW_LIBCPP_HIDE_FROM_ABI ~wstring_convert() { __ccw_cvt_owner<_Codecvt>::__destroy(__cvt_); }

    _CCW_LIBCPP_HIDE_FROM_ABI _CCW_STD::size_t converted() const _CCW_LIBCPP_NOEXCEPT { return __count_; }
    _CCW_LIBCPP_HIDE_FROM_ABI state_type state() const { return __st_; }

    _CCW_LIBCPP_HIDE_FROM_ABI wide_string from_bytes(char __byte) { return from_bytes(&__byte, &__byte + 1); }
    _CCW_LIBCPP_HIDE_FROM_ABI wide_string from_bytes(const char* __ptr) {
        const char* __e = __ptr;
        while (*__e) ++__e;
        return from_bytes(__ptr, __e);
    }
    _CCW_LIBCPP_HIDE_FROM_ABI wide_string from_bytes(const byte_string& __str) {
        const char* __b = __str.data();
        return from_bytes(__b, __b + __str.size());
    }
    _CCW_LIBCPP_HIDE_FROM_ABI wide_string from_bytes(const char* __first, const char* __last) {
        wide_string __r;
        __count_ = 0;
        if (!__cvt_) { if (__haswerr_) return __werr_; __ccw_throw_range_error("wstring_convert::from_bytes"); }
        const char* __fn = __first;
        _Elem       __buf[64];
        bool        __bad = false;
        while (__fn != __last) {
            _Elem*               __tn = 0;
            const char*          __prev = __fn;
            codecvt_base::result __rc = __cvt_->in(__st_, __fn, __last, __fn, __buf, __buf + 64, __tn);
            for (_Elem* __k = __buf; __k != __tn; ++__k) __r.push_back(*__k);
            if (__rc == codecvt_base::error) { __bad = true; break; }
            if (__rc == codecvt_base::noconv) {
                for (; __fn != __last; ++__fn) __r.push_back((_Elem)(unsigned char)*__fn);
                break;
            }
            if (__fn == __prev && __tn == __buf) { __bad = true; break; }
        }
        __count_ = (_CCW_STD::size_t)(__fn - __first);
        if (__bad) {
            if (__haswerr_) return __werr_;
            __ccw_throw_range_error("wstring_convert::from_bytes");
        }
        return __r;
    }

    _CCW_LIBCPP_HIDE_FROM_ABI byte_string to_bytes(_Elem __wchar) { return to_bytes(&__wchar, &__wchar + 1); }
    _CCW_LIBCPP_HIDE_FROM_ABI byte_string to_bytes(const _Elem* __wptr) {
        const _Elem* __e = __wptr;
        while (*__e) ++__e;
        return to_bytes(__wptr, __e);
    }
    _CCW_LIBCPP_HIDE_FROM_ABI byte_string to_bytes(const wide_string& __wstr) {
        const _Elem* __b = __wstr.data();
        return to_bytes(__b, __b + __wstr.size());
    }
    _CCW_LIBCPP_HIDE_FROM_ABI byte_string to_bytes(const _Elem* __first, const _Elem* __last) {
        byte_string __r;
        __count_ = 0;
        if (!__cvt_) { if (__hasberr_) return __berr_; __ccw_throw_range_error("wstring_convert::to_bytes"); }
        const _Elem* __fn = __first;
        char         __buf[256];
        bool         __bad = false;
        while (__fn != __last) {
            char*                __tn = 0;
            const _Elem*         __prev = __fn;
            codecvt_base::result __rc = __cvt_->out(__st_, __fn, __last, __fn, __buf, __buf + 256, __tn);
            for (char* __k = __buf; __k != __tn; ++__k) __r.push_back(*__k);
            if (__rc == codecvt_base::error) { __bad = true; break; }
            if (__rc == codecvt_base::noconv) {
                for (; __fn != __last; ++__fn) __r.push_back((char)(unsigned long)*__fn);
                break;
            }
            if (__fn == __prev && __tn == __buf) { __bad = true; break; }
        }
        __count_ = (_CCW_STD::size_t)(__fn - __first);
        if (__bad) {
            if (__hasberr_) return __berr_;
            __ccw_throw_range_error("wstring_convert::to_bytes");
        }
        return __r;
    }
};

template <class _Codecvt, class _Elem = wchar_t, class _Tr = char_traits<_Elem> >
class _CCW_LIBCPP_TEMPLATE_VIS wbuffer_convert : public basic_streambuf<_Elem, _Tr> {
public:
    typedef typename _Codecvt::state_type state_type;
    typedef _Elem                         char_type;
    typedef typename _Tr::int_type        int_type;
private:
    _CCW_STD::streambuf* __bufptr_;
    _Codecvt*            __cvt_;
    state_type           __st_;
    char_type            __pb_;
    bool                 __haspb_;
    char_type            __pend_ch_;
    bool                 __pend_;

    wbuffer_convert(const wbuffer_convert&);
    wbuffer_convert& operator=(const wbuffer_convert&);

    _CCW_LIBCPP_HIDE_FROM_ABI void __zero_state() {
        char* __q = (char*)(void*)&__st_;
        for (_CCW_STD::size_t __i = 0; __i < sizeof(state_type); ++__i) __q[__i] = 0;
        __haspb_ = false; __pend_ = false;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI bool __get_one(char_type& __out_) {
        if (__pend_) { __out_ = __pend_ch_; __pend_ = false; return true; }
        if (!__bufptr_ || !__cvt_) return false;
        for (int __i = 0; __i < 8; ++__i) {
            int __c = __bufptr_->sbumpc();
            if (__c == EOF) return false;
            char        __one = (char)__c;
            const char* __fn  = 0;
            char_type   __w[2];
            char_type*  __tn  = 0;
            __w[0] = char_type(); __w[1] = char_type();
            codecvt_base::result __rc = __cvt_->in(__st_, &__one, &__one + 1, __fn, __w, __w + 2, __tn);
            if (__rc == codecvt_base::error) return false;
            if (__rc == codecvt_base::noconv) { __out_ = (char_type)(unsigned char)__one; return true; }
            if (__tn != __w) {
                __out_ = __w[0];
                if (__tn - __w > 1) { __pend_ch_ = __w[1]; __pend_ = true; }
                return true;
            }
        }
        return false;
    }
public:
    _CCW_LIBCPP_HIDE_FROM_ABI wbuffer_convert()
        : __bufptr_(0), __cvt_(new _Codecvt), __pb_(char_type()), __pend_ch_(char_type()) { __zero_state(); }
    _CCW_LIBCPP_HIDE_FROM_ABI explicit wbuffer_convert(_CCW_STD::streambuf* __bytebuf)
        : __bufptr_(__bytebuf), __cvt_(new _Codecvt), __pb_(char_type()), __pend_ch_(char_type()) { __zero_state(); }
    _CCW_LIBCPP_HIDE_FROM_ABI wbuffer_convert(_CCW_STD::streambuf* __bytebuf, _Codecvt* __pcvt)
        : __bufptr_(__bytebuf), __cvt_(__pcvt), __pb_(char_type()), __pend_ch_(char_type()) { __zero_state(); }
    _CCW_LIBCPP_HIDE_FROM_ABI wbuffer_convert(_CCW_STD::streambuf* __bytebuf, _Codecvt* __pcvt, state_type __state)
        : __bufptr_(__bytebuf), __cvt_(__pcvt), __st_(__state), __pb_(char_type()), __pend_ch_(char_type())
        { __haspb_ = false; __pend_ = false; }
    _CCW_LIBCPP_HIDE_FROM_ABI ~wbuffer_convert() { __ccw_cvt_owner<_Codecvt>::__destroy(__cvt_); }

    _CCW_LIBCPP_HIDE_FROM_ABI _CCW_STD::streambuf* rdbuf() const { return __bufptr_; }
    _CCW_LIBCPP_HIDE_FROM_ABI _CCW_STD::streambuf* rdbuf(_CCW_STD::streambuf* __bytebuf) {
        _CCW_STD::streambuf* __old = __bufptr_;
        __bufptr_ = __bytebuf;
        return __old;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI state_type state() const { return __st_; }
protected:
    _CCW_LIBCPP_HIDE_FROM_ABI virtual int_type underflow() {
        if (__haspb_) return _Tr::to_int_type(__pb_);
        char_type __w = char_type();
        if (!__get_one(__w)) return _Tr::eof();
        __pb_ = __w; __haspb_ = true;
        return _Tr::to_int_type(__w);
    }
    _CCW_LIBCPP_HIDE_FROM_ABI virtual int_type uflow() {
        int_type __c = underflow();
        if (!_Tr::eq_int_type(__c, _Tr::eof())) __haspb_ = false;
        return __c;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI virtual int_type pbackfail(int_type __c = _Tr::eof()) {
        if (_Tr::eq_int_type(__c, _Tr::eof()) || __haspb_) return _Tr::eof();
        __pb_ = _Tr::to_char_type(__c); __haspb_ = true;
        return __c;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI virtual int_type overflow(int_type __c = _Tr::eof()) {
        if (_Tr::eq_int_type(__c, _Tr::eof())) return _Tr::not_eof(__c);
        if (!__bufptr_ || !__cvt_) return _Tr::eof();
        char_type            __w  = _Tr::to_char_type(__c);
        const char_type*     __fn = 0;
        char                 __buf[8];
        char*                __tn = 0;
        codecvt_base::result __rc = __cvt_->out(__st_, &__w, &__w + 1, __fn, __buf, __buf + 8, __tn);
        if (__rc == codecvt_base::error) return _Tr::eof();
        if (__rc == codecvt_base::noconv) { __buf[0] = (char)(unsigned long)__w; __tn = __buf + 1; }
        for (char* __k = __buf; __k != __tn; ++__k)
            if (__bufptr_->sputc(*__k) == EOF) return _Tr::eof();
        return __c;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI virtual int sync() { return __bufptr_ ? __bufptr_->pubsync() : 0; }
};

_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_LIBCPP___LOCALE_DIR_WSTRING_CONVERT_H
