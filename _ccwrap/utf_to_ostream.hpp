#ifndef UTF_TO_OSTREAM_HPP__
#define UTF_TO_OSTREAM_HPP__

#include <string>
#include <string_view>
#include <fstream>

#include "utfenc.hpp"

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

static inline std::ostream& operator<<(std::ostream& ost, wchar_t const* s) {
	return (ost << ccwrap::utfenc::to<std::string>(s));
}
static inline std::ostream& operator<<(std::ostream& ost, char16_t const* s) {
	return (ost << ccwrap::utfenc::to<std::string>(s));
}
static inline std::ostream& operator<<(std::ostream& ost, char32_t const* s) {
	return (ost << ccwrap::utfenc::to<std::string>(s));
}

static inline std::ostream& operator<<(std::ostream& ost, std::wstring_view s) {
	return (ost << ccwrap::utfenc::to<std::string>(s));
}
static inline std::ostream& operator<<(std::ostream& ost, std::u16string_view s) {
	return (ost << ccwrap::utfenc::to<std::string>(s));
}
static inline std::ostream& operator<<(std::ostream& ost, std::u32string_view s) {
	return (ost << ccwrap::utfenc::to<std::string>(s));
}

static inline std::ostream& operator<<(std::ostream& ost, std::wstring const& s) {
	return (ost << ccwrap::utfenc::to<std::string>(s));
}
static inline std::ostream& operator<<(std::ostream& ost, std::u16string const& s) {
	return (ost << ccwrap::utfenc::to<std::string>(s));
}
static inline std::ostream& operator<<(std::ostream& ost, std::u32string const& s) {
	return (ost << ccwrap::utfenc::to<std::string>(s));
}

static inline std::istream& operator>>(std::istream& ist, std::wstring& s) {
	std::string buf;
	ist >> buf;
	s = ccwrap::utfenc::to<std::wstring>(buf);
	return ist;
}
static inline std::istream& operator>>(std::istream& ist, std::u16string& s) {
	std::string buf;
	ist >> buf;
	s = ccwrap::utfenc::to<std::u16string>(buf);
	return ist;
}
static inline std::istream& operator>>(std::istream& ist, std::u32string& s) {
	std::string buf;
	ist >> buf;
	s = ccwrap::utfenc::to<std::u32string>(buf);
	return ist;
}


#if __CCWRAP_CXX >= 202002L || defined(__cpp_char8_t) || defined(__CCWRAP_HAS_CHAR8_T)
#if __CCWRAP_CXX >= 202002L
static inline std::ostream& operator<<(std::ostream& ost, char8_t s) {
	return (ost << ccwrap::utfenc::char_to<std::string>(s));
}
#endif
static inline std::ostream& operator<<(std::ostream& ost, char8_t const* s) {
	return (ost << ccwrap::utfenc::to<std::string>(s));
}
static inline std::ostream& operator<<(std::ostream& ost, std::basic_string<char8_t> const& s) {
	return (ost << ccwrap::utfenc::to<std::string>(s));
}
static inline std::ostream& operator<<(std::ostream& ost, std::basic_string_view<char8_t> s) {
	return (ost << ccwrap::utfenc::to<std::string>(s));
}

static inline std::istream& operator>>(std::istream& ist, std::u8string& s) {
	std::string buf;
	ist >> buf;
	s = ccwrap::utfenc::to<std::u8string>(buf);
	return ist;
}
#endif


#endif
