#ifndef __CCWRAP_UTF_CVT_STREAM_HPP_INCLUDED
#define __CCWRAP_UTF_CVT_STREAM_HPP_INCLUDED

#include <string>
#include <string_view>
#include <fstream>


#if 1 //def CCWRAP_UTF_CVT_STREAM_USE_UTFENC
#include "utfenc.hpp"
#define __CCWRAP_STRING_CODE_CONV	ccwrap::utfenc::to
#else
#include "StringCodeConv.hpp"
#define __CCWRAP_STRING_CODE_CONV	ccwrap::stringCodeConv
#endif


static inline std::ostream& operator<<(std::ostream& ost, wchar_t const* s) {
	return (ost << __CCWRAP_STRING_CODE_CONV<std::string>(s));
}
static inline std::ostream& operator<<(std::ostream& ost, char16_t const* s) {
	return (ost << __CCWRAP_STRING_CODE_CONV<std::string>(s));
}
static inline std::ostream& operator<<(std::ostream& ost, char32_t const* s) {
	return (ost << __CCWRAP_STRING_CODE_CONV<std::string>(s));
}

static inline std::ostream& operator<<(std::ostream& ost, std::basic_string_view<wchar_t> s) {
	return (ost << __CCWRAP_STRING_CODE_CONV<std::string>(s));
}
static inline std::ostream& operator<<(std::ostream& ost, std::basic_string_view<char16_t> s) {
	return (ost << __CCWRAP_STRING_CODE_CONV<std::string>(s));
}
static inline std::ostream& operator<<(std::ostream& ost, std::basic_string_view<char32_t> s) {
	return (ost << __CCWRAP_STRING_CODE_CONV<std::string>(s));
}

static inline std::ostream& operator<<(std::ostream& ost, std::basic_string<wchar_t> const& s) {
	return (ost << __CCWRAP_STRING_CODE_CONV<std::string>(s));
}
static inline std::ostream& operator<<(std::ostream& ost, std::basic_string<char16_t> const& s) {
	return (ost << __CCWRAP_STRING_CODE_CONV<std::string>(s));
}
static inline std::ostream& operator<<(std::ostream& ost, std::basic_string<char32_t> const& s) {
	return (ost << __CCWRAP_STRING_CODE_CONV<std::string>(s));
}

static inline std::istream& operator>>(std::istream& ist, std::basic_string<wchar_t>& s) {
	std::string buf;
	ist >> buf;
	s = __CCWRAP_STRING_CODE_CONV<std::wstring>(buf);
	return ist;
}
static inline std::istream& operator>>(std::istream& ist, std::basic_string<char16_t>& s) {
	std::string buf;
	ist >> buf;
	s = __CCWRAP_STRING_CODE_CONV<std::basic_string<char16_t> >(buf);
	return ist;
}
static inline std::istream& operator>>(std::istream& ist, std::basic_string<char32_t>& s) {
	std::string buf;
	ist >> buf;
	s = __CCWRAP_STRING_CODE_CONV<std::basic_string<char32_t> >(buf);
	return ist;
}

#ifdef CCWRAP_UTF_CVT_STREAM_USE_UTFENC
static inline std::ostream& operator<<(std::ostream& ost, wchar_t s) {
	return (ost << ccwrap::utfenc::char_to<std::string>(s));
}

#if __CCWRAP_CXX >= 201103L
static inline std::ostream& operator<<(std::ostream& ost, char16_t s) {
	return (ost << ccwrap::utfenc::char_to<std::string>(s));
}
static inline std::ostream& operator<<(std::ostream& ost, char32_t s) {
	return (ost << ccwrap::utfenc::char_to<std::string>(s));
}
#endif

#endif

#if __CCWRAP_CXX >= 202002L || defined(__cpp_char8_t) || defined(__CCWRAP_HAS_CHAR8_T)
#if __CCWRAP_CXX >= 202002L && defined(CCWRAP_UTF_CVT_STREAM_USE_UTFENC)
static inline std::ostream& operator<<(std::ostream& ost, char8_t s) {
	return (ost << ccwrap::utfenc::char_to<std::string>(s));
}
#endif
static inline std::ostream& operator<<(std::ostream& ost, char8_t const* s) {
	return (ost << __CCWRAP_STRING_CODE_CONV<std::string>(s));
}
static inline std::ostream& operator<<(std::ostream& ost, std::basic_string<char8_t> const& s) {
	return (ost << __CCWRAP_STRING_CODE_CONV<std::string>(s));
}
static inline std::ostream& operator<<(std::ostream& ost, std::basic_string_view<char8_t> s) {
	return (ost << __CCWRAP_STRING_CODE_CONV<std::string>(s));
}

static inline std::istream& operator>>(std::istream& ist, std::basic_string<char8_t>& s) {
	std::string buf;
	ist >> buf;
	s = __CCWRAP_STRING_CODE_CONV<std::basic_string<char8_t> >(buf);
	return ist;
}
#endif


#endif
