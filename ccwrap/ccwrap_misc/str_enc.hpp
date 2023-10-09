#ifndef __CCWRAP_STR_ENC_CVT_HPP_INCLUDED
#define __CCWRAP_STR_ENC_CVT_HPP_INCLUDED

#include "utfenc.hpp"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4127)
#pragma warning(disable:4244)
#pragma warning(disable:4100)
#endif

namespace ccwrap { namespace str_enc {
	using utfenc::char8;
	using utfenc::char16;
	using utfenc::char32;
	using utfenc::str_end;
} }

#if defined(CCWRAP_CHAR_IS_UTF8) || !defined(_WIN32)

namespace ccwrap { namespace str_enc {

	#if 0
	using utfenc::to_char32_count;
	using utfenc::char32_to_count;
	using utfenc::get_ch;
	using utfenc::set_ch;
	using utfenc::convert_chars;
	#endif
	using utfenc::count_chars;
	using utfenc::always_convert;
	using utfenc::convert;
	using utfenc::char_convert;
	using utfenc::to;
	using utfenc::char_to;

} }

#elif defined(_WIN32)

#include <windows.h>

namespace ccwrap { namespace str_enc {

template<typename C> struct to_cv_char {
	typedef C type;
};
template<> struct to_cv_char<char>     { typedef char       type; };
template<> struct to_cv_char<char8_t>  { typedef char8      type; };
#if __CCWRAP_WCHAR_BIT == 16
template<> struct to_cv_char<wchar_t>  { typedef wchar_t    type; };
template<> struct to_cv_char<char16_t> { typedef wchar_t    type; };
template<> struct to_cv_char<char32_t> { typedef char32     type; };
#else
template<> struct to_cv_char<wchar_t>  { typedef wchar_t    type; };
template<> struct to_cv_char<char16_t> { typedef char16     type; };
template<> struct to_cv_char<char32_t> { typedef wchar_t    type; };
#endif

#if 0
template<typename C> inline constexpr
std::size_t to_char32_count(C c) {
	typedef typename to_cv_char<C>::type UC;
	return utfenc::__impl::to_char32_count<UC>::count(c);
}

template<typename C> inline constexpr
std::size_t char32_to_count(char32 c) {
	typedef typename to_cv_char<C>::type UC;
	return utfenc::__impl::char32_to_count<UC>::count(c);
}

template<typename C> inline
char32	get_ch(C const*& rSrc, C const* end) {
	typedef typename to_cv_char<C>::type UC;
	return utfenc::__impl::get_ch<void>((UC const*&)rSrc, (UC const*)end);
}

template<typename C> inline
C* set_ch(C* d, C const* e, char32 c, bool* er=NULL) {
	typedef typename to_cv_char<C>::type UC;
	return (C*) utfenc::__impl::set_ch<void>((UC*)d, (UC*)e, c, er);
}
#endif

namespace __impl {
	template<typename DC, typename SC>
	struct count_chs {
		static std::size_t
		count(SC const* bgn, SC const* end, bool* er_p, char32 altc) {
			return utfenc::count_chars<DC>(bgn, end, er_p, altc);
		}
	};

	template<>
	struct count_chs<char,wchar_t> {
		static std::size_t 
		count(wchar_t const* bgn, wchar_t const* end, bool* er_p, char32 altc) {
	    	return WideCharToMultiByte(0/*CODEPAGE*/,0,bgn,int(end - bgn),nullptr,0,0,0);
		}
	};

	template<typename SC>
	struct count_chs<char,SC> {
		static std::size_t 
		count(SC const* bgn, SC const* end, bool* er_p, char32 altc) {
			std::wstring tmp = utfenc::convert<std::wstring>(bgn, end, er_p, altc);
			return count_chs<char,wchar_t>::count(tmp.data(), str_end(tmp), er_p, altc);
		}
	};

	template<>
	struct count_chs<wchar_t,char> {
		static std::size_t 
		count(char const* b,char const* e, bool* er_p, char32 altc) {
		    return MultiByteToWideChar(0,0,(char const*)b,int(e-b),nullptr, 0);
		}
	};

	template<typename DC>
	struct count_chs<DC,char> {
		static std::size_t 
		count(char const* b, char const* e, bool* er_p, char32 altc) {
			std::wstring tmp = utfenc::to<std::wstring>(b, e, er_p, altc);
			return count_chs<DC,wchar_t>::count(tmp.data(), str_end(tmp), er_p, altc);
		}
	};
}

template<typename DC, typename SC> inline std::size_t
count_chars(SC const* bgn, SC const* end, bool* er_p=NULL, char32 altc=UTFENC_CVT_ALT_CH) {
	typedef typename to_cv_char<DC>::type DUC;
	typedef typename to_cv_char<SC>::type SUC;
	return __impl::count_chs<DUC,SC>::count((SUC const*)bgn, (SUC const*)end, er_p, altc);
}

template<typename DC, typename SRC> inline std::size_t
count_chars(SRC const& src, bool* er_p=NULL, char32 altc=UTFENC_CVT_ALT_CH) {
	return count_chars<DC>(src.data(), src.data() + src.size(), er_p, altc);
}

template<typename DC, typename SC> inline std::size_t
count_chars(SC const* src, bool* er_p=NULL, char32 altc=UTFENC_CVT_ALT_CH) {
	return count_chars<DC>(src, str_end(src), er_p, altc);
}

namespace __impl {
	template<class DST> bool
	always_conv_wchar_to_dbc(DST& dst, wchar_t const* src_b,wchar_t const* src_e, char32 altc=UTFENC_CVT_ALT_CH) {
		static_assert(sizeof(dst[0]) == 1, "");
	    enum { cp = 0 };
		int ssz = int(src_e - src_b);
	    std::size_t bl = WideCharToMultiByte(cp,0,src_b,ssz,nullptr,0,0,0);
		dst.clear();
		dst.resize(bl + 1);
	    if (bl > 0) {
	        bl = WideCharToMultiByte(cp,0,src_b, ssz,(LPSTR)dst.data(),int(dst.size()),0,0);
	    }
	    dst[bl] = 0;
		dst.resize(bl);
	    return bl > 0;
	}

	template<class DST> bool
	always_conv_char_to_wchar(DST& dst, char const* src_b, char const* src_e, char32 altc=UTFENC_CVT_ALT_CH) {
		static_assert(sizeof(dst[0]) == 2, "");
	    enum { cp = 0 };
		int ssz = int(src_e - src_b);
	    std::size_t   wl = MultiByteToWideChar(cp,0,src_b,ssz,nullptr,0);
		dst.clear();
	    dst.resize(wl + 1);
	    if (wl > 0) {
	        wl = MultiByteToWideChar(cp, 0, src_b, ssz, (LPWSTR)dst.data(), int(dst.size()));
	    }
        dst[wl] = 0;
	    dst.resize(wl);
	    return wl > 0;
	}


	template<class DST, typename SC>
	struct always_cvt {
		static bool conv(DST& dst, SC const* src_b, SC const* src_e, char32 altc=UTFENC_CVT_ALT_CH) {
			return utfenc::always_convert(dst, src_b, src_e, altc);
		}
	};

	template<>
	struct always_cvt<std::string,wchar_t> {
		static bool conv(std::string& dst, wchar_t const* src_b,wchar_t const* src_e, char32 altc=UTFENC_CVT_ALT_CH) {
			return always_conv_wchar_to_dbc(dst, src_b, src_e, altc);
		}
	};

	template<typename SC>
	struct always_cvt<std::string,SC> {
		static bool conv(std::string& dst, SC const* src_b,SC const* src_e, char32 altc=UTFENC_CVT_ALT_CH) {
			std::wstring tmp;
			bool rc = utfenc::convert(tmp, src_b, src_e, altc);
			return always_cvt<std::string,wchar_t>::conv(dst, tmp.data(), str_end(tmp), altc) && rc;
		}
	};

	template<>
	struct always_cvt<std::wstring,char> {
		static bool conv(std::wstring& dst, char const* src_b, char const* src_e, char32 altc=UTFENC_CVT_ALT_CH) {
			return always_conv_char_to_wchar(dst, src_b, src_e, altc);
		}
	};

	template<>
	struct always_cvt<std::basic_string<unsigned short>,char> {
		static bool 
		conv(std::basic_string<char16>& dst, char const* src_b, char const* src_e, char32 altc=UTFENC_CVT_ALT_CH) {
			return always_conv_char_to_wchar(dst, src_b, src_e, altc);
		}
	};

 #if __CCWRAP_CXX >= 201103L
	template<> 
	struct always_cvt<std::u16string,char> {
		static bool conv(std::u16string& dst, char const* src_b, char const* src_e, char32 altc=UTFENC_CVT_ALT_CH) {
			return always_conv_char_to_wchar(dst, src_b, src_e, altc);
		}
	};
 #endif

	template<typename DST>
	struct always_cvt<DST,char> {
		static bool conv(DST& dst, char const* src_b, char const* src_e, char32 altc=UTFENC_CVT_ALT_CH) {
			std::wstring tmp;
			bool rc = always_cvt<std::wstring,char>::conv(tmp, src_b, src_e, altc);
			return utfenc::convert(dst, tmp.data(), str_end(tmp), altc) && rc;
		}
	};
}

template<class DST, typename SC> bool
always_convert(DST& dst, SC const* src_b, SC const* src_e, char32 altc=UTFENC_CVT_ALT_CH) {
	typedef typename to_cv_char<SC>::type SUC;
	return __impl::always_cvt<DST,SC>::conv(dst, (SUC const*)src_b, (SUC const*)src_e, altc);
}

template<class DST, typename SC> inline
bool convert(DST& dst, SC const* src_b, SC const* src_e, char32 altc=UTFENC_CVT_ALT_CH) {
	typedef typename DST::value_type DC;
	typedef typename to_cv_char<SC>::type SUC;
 #if __CCWRAP_CXX >= 201703L
	if constexpr (sizeof(SC) == sizeof(DC) && sizeof(SC) > 1)
 #else
	if (sizeof(SC) == sizeof(DC) && sizeof(SC) > 1)
 #endif
	{
		dst.assign((DC const*)src_b, (DC const*)src_e);
		return true;
	} else {
		return always_convert(dst, (SUC const*)src_b, (SUC const*)src_e, altc);
	}
}

template<class DST, class SC> inline
bool char_convert(DST& dst, SC sc, char32 altc=UTFENC_CVT_ALT_CH) {
	typedef typename to_cv_char<SC>::type SUC;
	SUC src[2] = { SUC(sc), 0 };
	return  convert(dst, (SUC const*)src, (SUC const*)src + 1, altc);
}

template<class DST, class SC> inline
bool convert(DST& dst, SC const* src, char32 altc=UTFENC_CVT_ALT_CH) {
	typedef typename to_cv_char<SC>::type SUC;
	return  convert(dst, (SUC const*)src, str_end((SUC const*)src), altc);
}

template<class DST, class SRC> inline
bool convert(DST& dst, SRC const& src, char32 altc=UTFENC_CVT_ALT_CH, typename SRC::value_type const* = 0) {
	typedef typename to_cv_char<typename SRC::value_type>::type SUC;
	SUC const* sb = (SUC const*)src.data();
	return convert(dst, sb, sb + src.size(), altc);
}

template<class DST, typename SC> inline
DST	to(SC const* src_b, SC const* src_e, bool* er_p=NULL, char32 altc=UTFENC_CVT_ALT_CH) {
	typedef typename to_cv_char<SC>::type SUC;
	DST dst;
	bool rc = convert(dst, (SUC const*)src_b, (SUC const*)src_e, altc);
	if (er_p) *er_p = !rc;
	return dst;
}

template<class DST, class SC> inline
DST char_to(SC sc, bool* er_p=0, char32 altc=UTFENC_CVT_ALT_CH, typename to_cv_char<SC>::type = 0) {
	typedef typename to_cv_char<SC>::type SUC;
	SUC src[2] = { SUC(sc), 0 };
	return to<DST>(src, src + 1, er_p, altc);
}

template<class DST, class SC> inline
DST	to(SC const* src, bool* er_p=NULL, char32 altc=UTFENC_CVT_ALT_CH) {
	typedef typename to_cv_char<SC>::type SUC;
	return to<DST>((SUC const*)src, str_end((SUC const*)src), er_p, altc);
}

template<class DST, class SRC> inline
DST	to(SRC const& src, bool* er_p=NULL, char32 altc=UTFENC_CVT_ALT_CH, typename SRC::value_type const* = 0) {
	typedef typename to_cv_char<typename SRC::value_type>::type SUC;
	return to<DST>((SUC const*)src.data(), (SUC const*)src.data() + src.size(), er_p, altc);
}

#if 0
namespace __impl {
	template<typename DC, typename SC> inline std::size_t
	conv_chars(DC* dst_b,DC* dst_e,SC const* src_b,SC const* src_e, bool* er_p, char32 altc) {
		return utfenc::convert_chars(dst_b, dst_e, src_b, src_e, er_p, altc);
	}

	template<> inline std::size_t
	conv_chars<char,wchar_t>(char* dst_b, char* dst_e
		,wchar_t const* src_b, wchar_t const* src_e, bool* er_p=NULL, char32 altc=UTFENC_CVT_ALT_CH)
	{
	    enum { cp = 0 };
		std::size_t ssz = src_e - src_b;
		std::size_t dsz = dst_e - dst_b;
	    std::size_t bl  = WideCharToMultiByte(cp,0,src,int(ssz),dsz_b,int(dst_sz),0,0);
		if (dsz > bl) {
	        buf[bl] = 0;
	    }
	    return bl;
	}

	template<typename SC> inline std::size_t
	conv_chars<char,SC>(char* dst_b, char* dst_e ,SC const* src_b, SC const* src_e
		, bool* er_p=NULL, char32 altc=UTFENC_CVT_ALT_CH)
	{
		std::wstring tmp = to<std::wstring>(src_b, src_e, er_p, altc);
		std::size_t  tsz = tmp.size();
		std::size_t  dsz = dst_e - dst_b;
		if (tsz >= dsz)
			tsz = dsz - 1;
		std::memcpy(dst_b, tmp.data(), tsz * sizeof(wchar_t));
		dst_b[tsz] = 0;
		return tsz;
	}

	template<typename DC> inline std::size_t
	conv_chars<DC,char>(DC* dst_b, DC* dst_e , char const* src_b, char const* src_e
		, bool* er_p=NULL, char32 altc=UTFENC_CVT_ALT_CH)
	{
		typedef typename to_cv_char<DC>::type DUC;
		std::basic_string<DUC> tmp = to< std::basic_string<DUC> >(src_b, src_e, er_p, altc);
		std::size_t  tsz = tmp.size();
		std::size_t  dsz = dst_e - dst_b;
		if (tsz >= dsz)
			tsz = dsz - 1;
		std::memcpy(dst_b, tmp.data(), tsz * sizeof(DC));
		dst_b[tsz] = 0;
		return tsz;
	}
}

template<typename DC, typename SC> inline std::size_t
convert_chars(DC* dst_b,DC* dst_e,SC const* src_b,SC const* src_e, bool* er_p, char32 altc) {
	typedef typename to_cv_char<DC>::type DUC;
	typedef typename to_cv_char<SC>::type SUC;
	return __impl::conv_chars((DUC*)dst_b, (DUC*)dst_e, (SUC const*)src_b, (SUC const*)src_e, er_p, altc);
}
#endif

} }	// ccwrap::str_enc


#elif 0	// sippai

namespace ccwrap {
	//struct char_sjis_t  { typedef unsigned char type; type value; };
	//struct char_eucjp_t { typedef unsigned char type; type value; };
	//struct char_utf8_t { typedef unsigned char type; type value; };
 #ifdef _WIN32
	struct char_dbc_t   { typedef unsigned char type; type value; };
 #endif
}

namespace utfenc { namespace __impl {

static inline bool w32_isLead(unsigned char c) { return IsDBCSLeadByte(c) != 0; }
static inline bool w32_isTrail(unsigned char c) { return ((c >= 0x30) & (c <= 0xFE)) && c != 0x7f; }

template<> struct to_char32_count<char_dbc_t> {
	static std::size_t count(char_dbc_t c) {
		return std::size_t(w32_isLead(c.value)) + 1;
	}
}
template<> struct char32_to_count<char_dbc_t> {
	static std::size_t count(char32 c) {
		c = utf32_to_dbc(c);
		return 1 + size_type(c > 0xff);
	}
}

template<typename DMY>
char32	get_ch(char_dbc_t const*& rStr, char_dbc_t const* end) {
    unsigned char const* s = (unsigned char const*)rStr;
    unsigned char const* e = (unsigned char const*)end;
    if (s >= e) {
        rStr = end;
        return char32(UTFENC_CVT_EOS);
    }
    char32 c = *s++;
    if (w32_isLead(c)) {
        if (s < e && ((ErrCk && w32_isTrail(*s)) || (!ErrCk && *s))) {
            c = (c << 8) | *s;
            ++s;
        } else if (ErrCk) {
            c = UTFENC_CVT_ERR_CH;
        }
    }
    rStr = (char_dbc_t const*)s;
	return dbc_to_utf32(c);
}

template<typename DMY>
char_dbc_t*	set_ch(char_dbc_t* str, char_dbc_t const* end, char32 c, bool* er) {
	c = utf32_to_dbc(c);
    unsigned char* d = (unsigned char*)str;
    unsigned char* e = (unsigned char*)end;
    if (c > 0xff) {
        if (d+2 > e) goto EOS_RET;
        *d++ = c >> 8;
        *d++ = c;
    } else {
        if (d >= e) goto EOS_RET;
        *d++ = c;
    }
    return (pointer)d;
EOS_RET:
	//if (er) *er = true;
    if (d < e)
        *d = 0;
    return (pointer)e;
}


template<typename DC, typename SC> inline std::size_t
ex_cvt_chars(DC* dst_b,DC* dst_e,SC const* src_b,SC const* src_e, bool* er_p, char32 altc) {
	return cvt_chars_impl(dst_b, dst_e, (SUC const*)bgn, (SUC const*)end, er_p, altc);
}

}}

namespace str_enc {

template<typename C> struct to_cv_char {
	typedef typename C type;
};
template<> struct to_cv_char<char>     { typedef char_dbc_t type; };
template<> struct to_cv_char<char8_t>  { typedef char8      type; };
#if __CCWRAP_WCHAR_BIT == 16
template<> struct to_cv_char<wchar_t>  { typedef wchar_t    type; };
template<> struct to_cv_char<char16_t> { typedef wchar_t    type; };
template<> struct to_cv_char<char32_t> { typedef char32     type; };
#else
template<> struct to_cv_char<wchar_t>  { typedef wchar_t    type; };
template<> struct to_cv_char<char16_t> { typedef char16     type; };
template<> struct to_cv_char<char32_t> { typedef wchar_t    type; };
#endif

template<typename C> inline constexpr
std::size_t to_char32_count(C c) {
	typedef typename to_cv_char<C>::type UC;
	return utfenc::__impl::to_char32_count<UC>::count(c);
}

template<typename C> inline constexpr
std::size_t char32_to_count(char32 c) {
	typedef typename to_cv_char<C>::type UC;
	return utfenc::__impl::char32_to_count<UC>::count(c);
}

template<typename C> inline
char32	get_ch(C const*& rSrc, C const* end) {
	typedef typename to_cv_char<C>::type UC;
	return utfenc::__impl::get_ch<void>((UC const*&)rSrc, (UC const*)end);
}

template<typename C> inline
C* set_ch(C* d, C const* e, char32 c, bool* er=NULL) {
	typedef typename to_cv_char<C>::type UC;
	return (C*) utfenc::__impl::set_ch<void>((UC*)d, (UC*)e, c, er);
}

template<typename DC, typename SC> inline std::size_t
count_chars(SC const* bgn, SC const* end, bool* er_p=NULL, char32 altc=UTFENC_CVT_ALT_CH) {
	return utfenc::count_chars<DUC>(bgn, end, er_p, altc);
}

template<> inline std::size_t
count_chars<char,wchar_t>(wchar_t const* bgn, wchar_t const* end, bool* er_p=NULL,char32 altc=UTFENC_CVT_ALT_CH){
    return WideCharToMultiByte(0/*CODEPAGE*/,0,src,int(size),nullptr,0,0,0);
}

template<typename SC> inline std::size_t
count_chars<char,SC>(SC const* bgn, SC const* end, bool* er_p=NULL,char32 altc=UTFENC_CVT_ALT_CH) {
	std::wstring tmp;
	utfenc::convert(tmp, bgn, end, er_p, altc);
	return count_chars(tmp.data(), str_end(tmp), er_p, altc);
}

template<> inline std::size_t
count_chars<wchar_t,char>(char const* b,char const* e,bool* er_p=NULL,char32 altc=UTFENC_CVT_ALT_CH){
    return MultiByteToWideChar(0,0,(char const*)b,int(e-b),nullptr, 0);
}

template<typename DC> inline std::size_t
count_chars<DC,char_dbc_t>(char_dbc_t const* b, char_dbc_t const* e, bool* er_p=NULL,char32 altc=UTFENC_CVT_ALT_CH) {
	std::wstring tmp;
	utfenc::convert(tmp, (char8 const*)b, (char8 const*)e, er_p, altc);
	return count_chars(tmp.data(), str_end(tmp), er_p, altc);
}

template<typename DC, typename SRC> inline std::size_t
count_chars(SRC const& src, bool* er_p=NULL, char32 altc=UTFENC_CVT_ALT_CH) {
	return count_chars(src.data(), src.data() + src.size(), er_p, altc);
}

template<typename DC, typename SC> inline std::size_t
count_chars(SC const* src, bool* er_p=NULL, char32 altc=UTFENC_CVT_ALT_CH) {
	return count_chars(src, str_end(src), er_p, altc);
}

} }	//str_enc

#endif

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif // __CCWRAP_STR_ENC_CVT_HPP_INCLUDED
