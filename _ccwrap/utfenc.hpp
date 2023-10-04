/**
 *  @file   utfenc.hpp
 *  @brief  Convert between utf8, utf16, utf32 encodings. (c++03 or later)
 *  @author Masashi Kitamura (tenka@6809.net)
 *	@date	2018-2023
 *  @license Boost Software Lisence Version 1.0
 *  @note
 *    namespace utfenc
 *    - to<DST>(SRC [, er_p, altc]) -> DST string
 *    - convert(DST, SRC [, altc]) -> bool (true:ok)
 *      Convert SRC to DST[string].
 *    - convert_add(DST, SRC [, altc]) -> -> bool (true:ok)
 *      Convert SRC and add to DST[string].
 *    - convert_chars(dst_b,dst_e,src_b,src_e [,er_p, altc]) -> size_t
 *      Convert SRC to DST[character buffer].
 *    - count_chars<DST>(SRC [, altc]) -> size_t
 *      Size when converting SRC to DST.
 *	  - get_ch(src, end) -> uint32_t
 *    - set_ch(dst, end, uint32_t) -> advanced dst
 *      Get/set a single utf32(uint32_t) character and advance the pointer.
 *     altc : Alternate character for illegal characters.
 *     er_p : Address of bool variable whose error is true.
 */
#ifndef UTFENC_HPP_INCLUDED__
#define UTFENC_HPP_INCLUDED__

#include <cstddef>
//#include <string>	// for char_traits
//#include <cstdint>

#if !defined(UTFENC_CVT_ALT_CH)
#define UTFENC_CVT_ALT_CH		(0)
#endif
#if !defined(UTFENC_CVT_EOS)
#define UTFENC_CVT_EOS			(0)
#endif
#if !defined(UTFENC_CVT_ERR_CH)
#define UTFENC_CVT_ERR_CH		(-1)
#endif

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4127)
#pragma warning(disable:4244)
//#pragma warning(disable:4996)
#endif

namespace ccwrap {

namespace utfenc {

typedef unsigned char	char8;
typedef unsigned short	char16;
typedef unsigned int  	char32;
//typedef std::uint32_t char32;

template<int N> struct nbytes_to_uchar {};
template<> struct nbytes_to_uchar<1> { typedef char8  type; };
template<> struct nbytes_to_uchar<2> { typedef char16 type; };
template<> struct nbytes_to_uchar<4> { typedef char32 type; };
template<typename C> struct to_uchar { typedef typename nbytes_to_uchar<sizeof(C)>::type type; };


namespace __impl {

	template<typename C> struct to_char32_count { };
	template<typename C> struct char32_to_count { };

	template<> struct to_char32_count<char8> {
		static constexpr std::size_t count(char8 c) {
		    return (c < 0xC0) ? 1
		    	:  (c < 0xE0) ? 2
		    	:  (c < 0xF0) ? 3
		 #if 1 //defined(UTFENCFENC_USE_FULLBIT)
		    	:  (c < 0xF8) ? 4
		    	:  (c < 0xFC) ? 5
		    	:               6
		 #else
		 		: 				4
		 #endif
				;
		}
	};
	template<> struct char32_to_count<char8> {
		static constexpr std::size_t count(char32 c) {
		    return (c <        0x80) ? 1
		    	:  (c <=      0x7FF) ? 2
		    	:  (c <=     0xFFFF) ? 3
		    	:  (c <=   0x1fFFFF) ? 4
		  #if 1 //defined(UTFENCFENC_USE_FULLBIT)
		    	:  (c <= 0x3fffFFFF) ? 5
		    	:                      6
		  #else
		 		:                      4
		  #endif
				;
		}
	};

	template<> struct to_char32_count<char16> {
		static constexpr std::size_t count(char16 c) {
			return (0xD800 <= c && c <= 0xDBFF) ? 2 : 1;
		}
	};
	template<> struct char32_to_count<char16> {
		static constexpr std::size_t count(char32 c) { return (c >= 0xffff) ? 2 : 1; }
	};

	template<> struct to_char32_count<char32> {
		static constexpr std::size_t count(char32) { return 1; }
	};
	template<> struct char32_to_count<char32> {
		static constexpr std::size_t count(char32) { return 1; }
	};


	/** utf-8 get_ch
	 */
	template<typename DMY>
	char32	get_ch(char8 const*& rSrc, char8 const* e) {
		char8 const* s = rSrc;
	    if (s >= e)
			return char32(UTFENC_CVT_EOS);
	    char8  b = *s++;
	    char32 c = b;
	    char32 d = c;
		if (c < 0x80) {
			rSrc = s;
			return d;
	    }
	    if (s >= e)
	    	goto ERR;
	    b = *s++;
	    if (c < 0xC0 || b < 0x80 || b >= 0xC0)
	    	goto ERR;
	    d = (d << 6) | (b & 0x3f);	// 11=5+6 0x80 .. 0x7ff
	    if (c < 0xE0) {
	        d &= (0x1 << 11) - 1;
	        rSrc = s;
	        return d;
		}
	    if (s >= e)
	    	goto ERR;
	    b = *s++;
	    if (b < 0x80 || b >= 0xC0)
	    	goto ERR;
		d = (d << 6) | (b & 0x3f);	// 16=4+6*2 0x8000 .. 0xffff
		if (c < 0xF0) {
			d &= (0x1 << 16) - 1;
			rSrc = s;
			return d;
		}
	    if (s >= e)
	    	goto ERR;
		b = *s++;
	    if (b < 0x80 || b >= 0xC0)
	    	goto ERR;
		d = (d << 6) | (b & 0x3f);	// 21=3+6*3
		if (c < 0xF8) {
			d &= (0x1 << 21) - 1;
			rSrc = s;
			return d;
		}
	  #if !defined(UTFENCFENC_USE_FULLBIT)
		++s;
	  #else
	    if (s >= e)
	    	goto ERR;
		b = *s++;
	    if (b < 0x80 || b >= 0xC0)
	    	goto ERR;
		d = (d << 6) | (b & 0x3f);	// 26=2+6*4
		if (c < 0xFC) {
			d &= (0x1 << 26) - 1;
			rSrc = s;
			return d;
		}
	    if (s >= e)
	    	goto ERR;
		b = *s++;
	    if (b < 0x80 || b >= 0xC0)
	    	goto ERR;
		d = (d << 6) | (b & 0x3f);	// 31=1+6*5
		d &= (0x1U << 31) - 1;
		rSrc = s;
		return d;
	  #endif
	 ERR:
		rSrc = (s < e) ? s - 1 : e;
	    return char32(UTFENC_CVT_ERR_CH);
	}

	/** utf-8 set_ch
	 */
	template<typename DMY>
	char8*	set_ch(char8* d, char8 const* e, char32 c, bool* er) {
	    if (c <= 0x7F) {
	        if (d < e) {
		        *d++ = c;
		        return d;
		    }
	    } else if (c <= 0x7FF) {
	        if (d+2 <= e) {
		        *d++ = 0xC0|(c>>6);
		        *d++ = 0x80|(c&0x3f);
		        return d;
		    }
	    } else if (c <= 0xFFFF) {
	        if (d+3 <= e) {
		        *d++ = 0xE0|(c>>12);
		        *d++ = 0x80|((c>>6)&0x3f);
		        *d++ = 0x80|(c&0x3f);
		        return d;
			}
	    } else if (c <= 0x1fFFFF) {
	        if (d+4 <= e) {
		        *d++ = 0xF0|(c>>18);
		        *d++ = 0x80|((c>>12)&0x3f);
		        *d++ = 0x80|((c>>6)&0x3f);
		        *d++ = 0x80|(c&0x3f);
		        return d;
			}
	    } else {
		  #if defined(UTFENCFENC_USE_FULLBIT)
			if (c <= 0x3fffFFFF) {
		        if (d+5 <= e) {
			        *d++ = 0xF8|(c>>24);
			        *d++ = 0x80|((c>>18)&0x3f);
			        *d++ = 0x80|((c>>12)&0x3f);
			        *d++ = 0x80|((c>>6)&0x3f);
			        *d++ = 0x80|(c&0x3f);
		        	return d;
				}
		    } else {
		        if (d+6 <= e) {
			        *d++ = 0xFC|(c>>30);
			        *d++ = 0x80|((c>>24)&0x3f);
			        *d++ = 0x80|((c>>18)&0x3f);
			        *d++ = 0x80|((c>>12)&0x3f);
			        *d++ = 0x80|((c>>6)&0x3f);
			        *d++ = 0x80|(c&0x3f);
		        	return d;
			    }
		    }
		  #endif
		}
		if (er)
			*er = true;
	    return d;
	}

	/** utf-16 get_ch
	 */
	template<typename DMY>
	char32	get_ch(char16 const*& rSrc, char16 const* e) {
	    char16 const* s = rSrc;
	    if (s >= e)
			return char32(UTFENC_CVT_EOS);
	    char32 c = *s++;
	    if (c < 0xD800 || 0xDFFF < c) {
			rSrc = s;
	    	return c;
	    }
	    if (c <= 0xDBFF) {
			if (s < e) {
	            char16 d = *s;
	            if (0xDC00 <= d && d <= 0xDFFF) {
					rSrc = s + 1;
	                c = ((c & 0x3ff) << 10) | (d & 0x3ff);
	                c += 0x10000;
					return c;
	            }
	        }
		}
		rSrc = s;
		return char32(UTFENC_CVT_ERR_CH);
	}

	/** utf-16 set_ch
	 */
	template<typename DMY>
	char16*	set_ch(char16* d, char16 const* e, char32 c, bool* er) {
	    if (c < 0x10000) {
	        if (d < e) {
	        	*d++ = char16(c);
	        	return d;
	        }
	    } else if (c <= 0x10FFFF) {
			if (d + 1 < e) {
		        char32 x = c - 0x10000;
		        c = (x >> 10) | 0xD800;
		        *d++ = char16(c);
		        c = (x & 0x3ff) | 0xDC00;
		        *d++ = char16(c);
		        return d;
			}
	    }
		if (er)
			*er = true;
	    return d;
	}

	/** utf-32 get_ch
	 */
	template<typename DMY> inline
	char32	get_ch(char32 const*& rSrc, char32 const* e) {
		if (rSrc < e)
			return *rSrc++;
		return char32(UTFENC_CVT_EOS);
	}

	/** utf-32 set_ch
	 */
	template<typename DMY> inline
	char32*	set_ch(char32* d, char32 const* e, char32 c, bool*) {
		if (d < e)
			*d++ = c;
		return d;
	}

	template<typename DC, typename SC>
	std::size_t count_chars(SC const* bgn, SC const* end, bool* er_p=NULL, char32 altc=UTFENC_CVT_ALT_CH) {
		typedef typename to_uchar<SC>::type SUC;
		typedef typename to_uchar<DC>::type DUC;
		std::size_t n  = 0;
		bool        er = false;
		SUC const*	s  = (SUC const*)bgn;
		SUC const*	e  = (SUC const*)end;
		while (s < e) {
			char32 c = get_ch<void>(s, e);
			bool k = (c != char32(UTFENC_CVT_ERR_CH));
			if (!k) {
				er = true;
				if (altc) {
					c = altc;
					k = (c != char32(UTFENC_CVT_ERR_CH));
				}
			}
			if (k)
				n += char32_to_count<DUC>::count(c);
		}
		if (er_p)
			*er_p = er;
		return n;
	}

	template<typename DC, typename SC> std::size_t
	cvt_chars(DC* dst_b,DC* dst_e,SC const* src_b,SC const* src_e,bool* er_p=NULL,char32 altc=UTFENC_CVT_ALT_CH) {
		typedef typename to_uchar<SC>::type SUC;
		typedef typename to_uchar<DC>::type DUC;
		if (!src_b || !src_e || src_b >= src_e || dst_b > dst_e) {
			if (er_p)
				*er_p = true;
			return 0;
		}
		SUC const*	s  = (SUC const*)src_b;
		SUC const*	se = (SUC const*)src_e;
		DUC*	    d  = (DUC      *)dst_b;
		DUC*        de = (DUC      *)dst_e;
		if (!d || !de)
			return count_chars<DC>(s, se);
		bool		er = false;
		while (d < de) {
			if (s >= se)
				break;
			char32 c = get_ch<void>(s, se);
			bool k = (c != char32(UTFENC_CVT_ERR_CH));
			if (!k) {
				er = true;
				if (altc) {
					c = altc;
					k = (c != char32(UTFENC_CVT_ERR_CH));
				}
			}
			if (k)
				d = set_ch<void>(d, de, c, &er);
		}
		if (d < de)
			*d = 0;
		if (er_p)
			*er_p = er;
		return (DC*)d - dst_b;
	}

}	// __impl


//	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-

template<typename C> static inline //constexpr
C const* str_end(C const* s) {
 #if 1 //defined(UTFENC_NO_CHAR_TRAITS)
		while (*s)
			++s;
		return s;
 #else
	return s + std::char_traits<C>::length(s);
 #endif
}

template<typename C> inline constexpr
std::size_t to_char32_count(C c) {
	typedef typename to_uchar<C>::type UC;
	return __impl::to_char32_count<UC>::count(c);
}

template<typename C> inline constexpr
std::size_t char32_to_count(char32 c) {
	typedef typename to_uchar<C>::type UC;
	return __impl::char32_to_count<UC>::count(c);
}

template<typename C> inline
char32	get_ch(C const*& rSrc, C const* end) {
	typedef typename to_uchar<C>::type UC;
	return __impl::get_ch<void>((UC const*&)rSrc, (UC const*)end);
}

template<typename C> inline
C* set_ch(C* d, C const* e, char32 c, bool* er=NULL) {
	typedef typename to_uchar<C>::type UC;
	return (C*) __impl::set_ch<void>((UC*)d, (UC*)e, c, er);
}

template<typename DC, typename SC> inline std::size_t
count_chars(SC const* bgn, SC const* end, bool* er_p=NULL, char32 altc=UTFENC_CVT_ALT_CH) {
	typedef typename to_uchar<SC>::type SUC;
	typedef typename to_uchar<DC>::type DUC;
	return __impl::count_chars<DUC>((SUC const*)bgn, (SUC const*)end, er_p, altc);
}

template<typename DC, typename SRC> inline std::size_t
count_chars(SRC const& src, bool* er_p=NULL, char32 altc=UTFENC_CVT_ALT_CH) {
	return count_chars(src.data(), src.data() + src.size(), er_p, altc);
}

template<typename DC, typename SC> inline std::size_t
count_chars(SC const* src, bool* er_p=NULL, char32 altc=UTFENC_CVT_ALT_CH) {
	return count_chars(src, str_end(src), er_p, altc);
}

template<typename DC, typename SC> std::size_t
convert_chars(DC* dst_b,DC* dst_e,SC const* src_b,SC const* src_e,bool* er_p=NULL,char32 altc=UTFENC_CVT_ALT_CH) {
	typedef typename to_uchar<SC>::type SUC;
	typedef typename to_uchar<DC>::type DUC;
	return __impl::cvt_chars((DUC*)dst_b, (DUC*)dst_e, (SUC const*)src_b, (SUC const*)src_e, er_p, altc);
}

#if 1 // always_... is an internal function but may be used externally.
template<class DST, typename SC> bool
always_convert_add(DST& dst, SC const* src_b, SC const* src_e, char32 altc=UTFENC_CVT_ALT_CH) {
	typedef typename DST::value_type DC;
	bool er = false;
	std::size_t cnt = count_chars<DC>(src_b, src_e, &er);
	if (cnt == 0)
		return !er;
	std::size_t dst_sz = dst.size();
	dst.resize(dst_sz + cnt + 1);
	DC*  db = &dst[0] + dst_sz;
	std::size_t dl = convert_chars(db, db + cnt, src_b, src_e, &er, altc);
	if (dl > cnt) {
		er = true;
		dl = cnt;
	}
	db[dl] = 0;
	dst.resize(dst_sz + dl);
	return !er;
}

template<class DST, typename SC> inline
bool always_convert(DST& dst, SC const* src_b, SC const* src_e, char32 altc=UTFENC_CVT_ALT_CH) {
	dst.clear();
	typedef typename to_uchar<SC>::type SUC;
	return always_convert_add(dst, (SUC const*)src_b, (SUC const*)src_e, altc);
}
#endif

template<class DST, typename SC> inline
bool convert_add(DST& dst, SC const* src_b, SC const* src_e, char32 altc=UTFENC_CVT_ALT_CH) {
	typedef typename DST::value_type DC;
 #if __cplusplus >= 201703L || _MSVC_LANG >= 201703L
	if constexpr (sizeof(SC) == sizeof(DC))
 #else
	if (sizeof(SC) == sizeof(DC))
 #endif
	{
		dst.append((DC const*)src_b, (DC const*)src_e);
		return true;
	} else {
		typedef typename to_uchar<SC>::type SUC;
		return always_convert_add(dst, (SUC const*)src_b, (SUC const*)src_e, altc);
	}
}

template<class DST, class SC> inline
bool char_convert_add(DST& dst, SC sc, char32 altc=UTFENC_CVT_ALT_CH) {
	typedef typename to_uchar<SC>::type SUC;
	SUC src[2] = { SUC(sc), 0 };
	return  convert_add(dst, src, src + 1, altc);
}

template<class DST, class SC> inline
bool convert_add(DST& dst, SC const* src, char32 altc=UTFENC_CVT_ALT_CH) {
	typedef typename to_uchar<SC>::type SUC;
	return  convert_add(dst, (SUC const*)src, str_end((SUC const*)src), altc);
}

template<class DST, class SRC> inline
bool convert_add(DST& dst, SRC const& src, char32 altc=UTFENC_CVT_ALT_CH, typename SRC::value_type const* = 0) {
	typedef typename nbytes_to_uchar<sizeof(src[0])>::type SUC;
	SUC const* sb = (SUC const*)&src[0];
	return convert_add(dst, sb, sb + src.size(), altc);
}
#if 0 //!defined(__CCWRAP_NO_SFINE)
template<class DST, class SC> inline
bool convert_add(DST& dst, SC sc, char32 altc=UTFENC_CVT_ALT_CH, typename to_uchar<SC>::type = to_uchar<SC>::type(0)) {
	return char_convert_add(dst, sc, altc);
}
#endif

template<class DST, typename SC> inline
bool convert(DST& dst, SC const* src_b, SC const* src_e, char32 altc=UTFENC_CVT_ALT_CH) {
	typedef typename DST::value_type DC;
	typedef typename to_uchar<SC>::type SUC;
 #if __cplusplus >= 201703L || _MSVC_LANG >= 201703L
	if constexpr (sizeof(SC) == sizeof(DC))
 #else
	if (sizeof(SC) == sizeof(DC))
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
	typedef typename to_uchar<SC>::type SUC;
	SUC src[2] = { SUC(sc), 0 };
	return  convert(dst, (SUC const*)src, (SUC const*)src + 1, altc);
}

template<class DST, class SC> inline
bool convert(DST& dst, SC const* src, char32 altc=UTFENC_CVT_ALT_CH) {
	typedef typename to_uchar<SC>::type SUC;
	return  convert(dst, (SUC const*)src, str_end((SUC const*)src), altc);
}

template<class DST, class SRC> inline
bool convert(DST& dst, SRC const& src, char32 altc=UTFENC_CVT_ALT_CH, typename SRC::value_type const* = 0) {
	typedef typename nbytes_to_uchar<sizeof(typename SRC::value_type)>::type SUC;
	SUC const* sb = (SUC const*)src.data();
	return convert(dst, sb, sb + src.size(), altc);
}

#if 0 //!defined(__CCWRAP_NO_SFINE)
template<class DST, class SC> inline
bool convert(DST& dst, SC sc, char32 altc=UTFENC_CVT_ALT_CH, typename to_uchar<SC>::type = to_uchar<SC>::type(0)) {
	return char_convert(dst, sc, altc);
}
#endif

template<class DST, typename SC> inline
DST	to(SC const* src_b, SC const* src_e, bool* er_p=NULL, char32 altc=UTFENC_CVT_ALT_CH) {
	typedef typename to_uchar<SC>::type SUC;
	DST dst;
	bool rc = convert(dst, (SUC const*)src_b, (SUC const*)src_e, altc);
	if (er_p) *er_p = !rc;
	return dst;
}

template<class DST, class SC> inline
DST char_to(SC sc, bool* er_p=0, char32 altc=UTFENC_CVT_ALT_CH, typename to_uchar<SC>::type = 0) {
	typedef typename to_uchar<SC>::type SUC;
	SUC src[2] = { SUC(sc), 0 };
	return to<DST>(src, src + 1, er_p, altc);
}

template<class DST, class SC> inline
DST	to(SC const* src, bool* er_p=NULL, char32 altc=UTFENC_CVT_ALT_CH) {
	typedef typename to_uchar<SC>::type SUC;
	return to<DST>((SUC const*)src, str_end((SUC const*)src), er_p, altc);
}

template<class DST, class SRC> inline
DST	to(SRC const& src, bool* er_p=NULL, char32 altc=UTFENC_CVT_ALT_CH, typename SRC::value_type const* = 0) {
	typedef typename nbytes_to_uchar<sizeof(typename SRC::value_type)>::type SUC;
	return to<DST>((SUC const*)src.data(), (SUC const*)src.data() + src.size(), er_p, altc);
}
#if 0 //!defined(__CCWRAP_NO_SFINE)
template<class DST, class SC> inline
DST to(SC sc, bool* er_p=0, char32 altc=UTFENC_CVT_ALT_CH, typename to_uchar<SC>::type = to_uchar<SC>::type(0)) {
	return char_to(sc, er_p, altc);
}
#endif

}	// utfenc

}	// ccwrap

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif	// UTFENC_HPP_INCLUDED
