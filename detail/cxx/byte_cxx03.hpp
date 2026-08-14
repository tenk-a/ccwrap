/**
 *  @file   byte_cxx003.hpp
 *  @brief  byte for c++03
 *  @license Boost Software License Version 1.0
 */
#if defined(__cplusplus) && !defined(_CCW_CSTDDEF_BYTE)
#define _CCW_CSTDDEF_BYTE
    struct byte {
        unsigned char __v_;
        inline byte() : __v_(0) {}
        explicit byte(unsigned char __v) : __v_(__v) {}
    };
    template <class _IntT> inline _IntT to_integer(byte __b) { return (_IntT)__b.__v_; }
    inline byte  operator|(byte __l, byte __r) { return byte((unsigned char)(__l.__v_ | __r.__v_)); }
    inline byte  operator&(byte __l, byte __r) { return byte((unsigned char)(__l.__v_ & __r.__v_)); }
    inline byte  operator^(byte __l, byte __r) { return byte((unsigned char)(__l.__v_ ^ __r.__v_)); }
    inline byte  operator~(byte __b)           { return byte((unsigned char)(~__b.__v_)); }
    template <class _IntT> inline byte operator<<(byte __b, _IntT __s) { return byte((unsigned char)(__b.__v_ << __s)); }
    template <class _IntT> inline byte operator>>(byte __b, _IntT __s) { return byte((unsigned char)(__b.__v_ >> __s)); }
    inline byte& operator|=(byte& __l, byte __r) { __l.__v_ = (unsigned char)(__l.__v_ | __r.__v_); return __l; }
    inline byte& operator&=(byte& __l, byte __r) { __l.__v_ = (unsigned char)(__l.__v_ & __r.__v_); return __l; }
    inline byte& operator^=(byte& __l, byte __r) { __l.__v_ = (unsigned char)(__l.__v_ ^ __r.__v_); return __l; }
    template <class _IntT> inline byte& operator<<=(byte& __b, _IntT __s) { __b.__v_ = (unsigned char)(__b.__v_ << __s); return __b; }
    template <class _IntT> inline byte& operator>>=(byte& __b, _IntT __s) { __b.__v_ = (unsigned char)(__b.__v_ >> __s); return __b; }
#endif
