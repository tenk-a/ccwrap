#ifndef STR_STREAM_HPP_INCLUDED__
#define STR_STREAM_HPP_INCLUDED__

#include <streambuf>
#include <iostream>
#include <string>	// for char_traits
#include <cassert>

#if __cplusplus >= 201703L || __MSVC_LANG >= 201703L
#include <string_view>
#else
#include "fakestd/string_view.hpp"
#endif

namespace ccwrap {

template <typename C, class T = std::char_traits<C> >
class basic_str_streambuf : public std::basic_streambuf<C, T> {
	typedef basic_str_streambuf	this_type;
public:
    typedef std::size_t size_type;

    basic_str_streambuf(C* buf, std::size_t size, std::size_t cur=0) {
		assert(buf != nullptr && size > 0 && cur <= size);
        init(buf, buf+cur, buf+size-1);
    }

    template<std::size_t N>
    basic_str_streambuf(C (&buf)[N], std::size_t cur=0) {
		assert(buf != nullptr && cur <= N);
        init(buf, buf+cur, buf+N-1);
    }

    C*          c_str()          { *this->pptr() = 0; return this->pbase(); }
	C const*	c_str()    const { return const_cast<this_type*>(this)->c_str(); }
	C*			data()			 { return c_str(); }
	C const*	data()     const { return const_cast<this_type*>(this)->data(); }
    size_type   size()     const { return this->pptr() - this->pbase(); }
    size_type   capacity() const { return 1 + this->epptr() - this->pbase(); }
    void        clear() { init(this->pbase(), this->pbase(), this->epptr()); *this->pptr() = 0; }
	std::basic_string_view<C> str() { return std::string_view(this->pbase(), this->pptr() - this->pbase()); }

private:
    void        init(C* bgn, C* cur, C* end) { this->setp( bgn, end ); this->setg( bgn, cur, end ); }
};

#if 0
template <typename C, class T = std::char_traits<C> >
class basic_str_istream : public std::basic_istream<C, T> {
	typedef std::basic_istream<C, T>	base;
	typedef basic_str_streambuf<C,T>	streambuf;
public:
	basic_str_istream(C const* buf, std::size_t n, std::size_t cur=0) : base(streambuf_ini(buf, n, cur)) {}
	template<std::size_t N>
	basic_str_istream(C const (&buf)[N], std::size_t cur=0) : base(streambuf_ini(buf, N, cur)) {}
	~basic_str_istream() { buf().~streambuf(); }
	C const*	data()	   const { return buf_.data(); }
    size_type   size()     const { return buf_.size(); }
    size_type   capacity() const { return buf_.capacity(); }
	const std::basic_string_view<C> str() const { return buf_.str(); }
private:
	streambuf&	buf() { return *(streambuf*)buf0_; }
	streambuf*	streambuf_ini(C const* buf, std::size_t n, std::size_t cur) {
		new((void*)buf0_) streambuf(const_cast<C*>(buf), n, cur);
		return (streambuf*)buf0_;
	}
private:
	char buf0_[sizeof(streambuf)];
};
#endif

template <typename C, class T = std::char_traits<C> >
class basic_str_ostream : public std::basic_ostream<C, T> {
	typedef std::basic_ostream<C, T>	base;
	typedef basic_str_streambuf<C,T>	streambuf;
public:
    typedef std::size_t 	size_type;

	basic_str_ostream(C* buf, std::size_t n, std::size_t cur=0)
		: base(streambuf_ini(buf, n, cur)) {}
	template<std::size_t N>
	basic_str_ostream(C (&buf)[N], std::size_t cur=0)
		: base(streambuf_ini(buf, N, cur)) {}
	~basic_str_ostream() { buf().~streambuf(); }
    C*          c_str()          { return buf().c_str(); }
	C*			data()			 { return buf().data(); }
    size_type   size()     const { return buf().size(); }
    size_type   capacity() const { return buf().capacity(); }
    void        clear() { buf().clear(); }
	std::basic_string_view<C> str() { return buf().str(); }
private:
	streambuf&	buf() { return *(streambuf*)buf0_; }
	streambuf*	streambuf_ini(C* buf, std::size_t n, std::size_t cur) {
		new((void*)buf0_) streambuf(buf, n, cur);
		return (streambuf*)buf0_;
	}
private:
	char buf0_[sizeof(streambuf)];
};

typedef basic_str_ostream<char> 		str_stream;
//typedef basic_str_ostream<wchar_t>	wstr_stream;

}	// ccwrap

#endif	// STR_STREAM_HPP_INCLUDED__
