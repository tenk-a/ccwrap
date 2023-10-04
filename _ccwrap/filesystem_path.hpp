/**
 *  @file   file_path.hpp
 *  @brief  std::path like class for UNICODE (Not support 'MBC')
 *  @author Masashi Kitamura (tenka@6809.net)
 *  @license Boost Software Lisence Version 1.0
 *  @note
 */
#ifndef CCWRAP_FILESYSTEM_PATH_HPP__
#define CCWRAP_FILESYSTEM_PATH_HPP__

#include "../boost2std/boost2std_common.hpp"
#include <cstddef>

#include <iostream>
#include <cassert>
#include <string>
#include <string_view>

#ifndef CCWRAP_PATH_CHAR
 #if defined(_WIN32) //|| defined(MSDOS)
  #define CCWRAP_PATH_CHAR    wchar_t
 #else
  #define CCWRAP_PATH_CHAR    char
 #endif
#endif

#include "utfenc.hpp"
#include "strpath.hpp"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4018)
#endif

namespace _ccwrap {
	using std::basic_string;
	using std::basic_string_view;

	template<typename C>
	class basic_path;

	namespace __path_detail { 
		template<typename C>
		class path_iterator {
		    typedef typename basic_string<C>::iterator	iterator;
		public:
			typedef C					char_type;
			typedef basic_string<C> 	string_type;
		    typedef basic_path<C>		value_type;

		    path_iterator() noexcept : str_(nullptr), cur_(0), rootNameLen_(0),rootPathLen_(0) {}
		    path_iterator(path_iterator const& r) noexcept
		    	: str_(r.str_), cur_(r.cur_), rootNameLen_(0), rootPathLen_(0) {}
		    path_iterator(basic_string<C> const* s, bool is_end);

		    value_type     operator*() const;
		    path_iterator& operator++();
		    path_iterator  operator++(int) const { path_iterator it = *this; operator++(); return it; }
		    path_iterator& operator--();
		    path_iterator  operator--(int) const { path_iterator it = *this; operator--(); return it; }

			bool operator==(path_iterator const& r) const { return str_ == r.str_ && cur_ == r.cur_; }
			bool operator!=(path_iterator const& r) const { return !operator==(r); }
		private:
		    string_type const* str_;
		    size_t          cur_;
		    int/*size_t*/   rootNameLen_;
		    int/*size_t*/   rootPathLen_;
		};

	}   // __path_detail


	/// like std::path
	template<typename C>
	class basic_path {
	public:
	    typedef C									value_type;
	    typedef basic_string<value_type>			string_type;
	    typedef basic_string_view<value_type>		string_view_type;
	    typedef __path_detail::path_iterator<value_type> iterator;
	    typedef iterator                			const_iterator;

	    enum format { native_format, generic_format, auto_format };

	 #if __cplusplus >= 201703L || _MSVC_LANG >= 201703L
	    static constexpr value_type preferred_separator = value_type(ccwrap::strpath::preferred_separator);
	 #else
	    static const value_type preferred_separator = value_type(ccwrap::strpath::preferred_separator);
	 #endif

	    basic_path() noexcept {}
	    basic_path(basic_path const& r) : str_(r.str_) {}
     #if __cplusplus >= 201103L || _MSC_VER >= 1900
	    basic_path(basic_path&& r) noexcept : str_(std::move(r.str_)) {}
	    basic_path(string_type&& r, format fmt = auto_format) : str_(std::move(r)) { adjust_fmt(fmt); }
	 #endif
	    basic_path(string_type const& r, format fmt = auto_format) : str_(r) { adjust_fmt(fmt); }

	    //basic_path(iterator const& r, format fmt = auto_format) : str_((*r).native()) { adjust_fmt(fmt); }

	    //basic_path(value_type  const* r, format fmt = auto_format) : str_(r) { adjust_fmt(fmt); }
	    //basic_path(string_view_type r, format fmt = auto_format) : str_(r.data(), r.size()) { adjust_fmt(fmt); }
	    template <class S> basic_path(S const& r, format fmt = auto_format)
	    	: str_(conv_to<string_type>(r)){adjust_fmt(fmt);}
	    template <class InpIte> basic_path(InpIte f, InpIte l, format fmt=auto_format)
	    	: str_(f,l) { adjust_fmt(fmt); }
	    //TODO: template <class S> basic_path(S const& r, const locale& loc, format fmt = auto_format);
	    //TODO: template <class InpIte> basic_path(InpIte first,InpIte last,const locale& loc,format fmt=auto_format);

	    basic_path& operator=(basic_path const& r) { if (this != &r) str_ = r.str_; return *this; }
	    template <class S> basic_path& operator=(S const& r) { basic_path(r).swap(*this); return *this; }
	    template <class S> basic_path& assign(S const& r) { return operator=(r); }

     #if __cplusplus >= 201103L || _MSVC_LANG >= 201103L || _MSC_VER >= 1900
	    basic_path& operator=(basic_path&& r) noexcept { if (this != &r) str_ = std::move(r.str_); return *this; }
	    basic_path& operator=(string_type&& r) noexcept { if (&str_ != &r) str_ = std::move(r); return *this; }
	    basic_path& assign(string_type&& r) { return operator=(std::move(r)); }
	 #endif
	    template <class InpIte> basic_path& assign(InpIte first, InpIte last)
	    	{ basic_path(first,last).swap(*this); return *this; }

	    void clear() noexcept { str_.clear(); }
	    void swap(basic_path& r) noexcept { str_.swap(r.str_); }

	    string_type const& native() const noexcept { return str_; }
	    value_type const*  c_str() const noexcept { return str_.c_str(); }
	    operator string_type() const { return str_; }

     #if __cplusplus >= 201103L || _MSVC_LANG >= 201103L || _MSC_VER >= 1900
	    template <class OC, class TR=std::char_traits<OC>, class A=std::allocator<OC> > basic_string<OC,TR,A>
	    string(A const& a) const { basic_string<OC,TR,A> dst(a); conv(dst, str_); return dst; }
	    template <class OC, class TR=std::char_traits<OC>, class A=std::allocator<OC> > basic_string<OC,TR,A>
	    string() const { basic_string<OC,TR,A> dst; conv(dst, str_); return dst; }
	 #else
	    template <class OC, class TR, class A> basic_string<OC,TR,A>
	    string(A const& a) const { basic_string<OC,TR,A> dst(a); conv(dst, str_); return dst; }
	    template <class OC> basic_string<OC>
	    string() const { basic_string<OC> dst; conv(dst, str_); return dst; }
	 #endif

	    basic_string<char>      string()    const { return this->string<char    >(); }
	    basic_string<wchar_t>   wstring()   const { return this->string<wchar_t >(); }
	    basic_string<char16_t>  u16string() const { return this->string<char16_t>(); }
	    basic_string<char32_t>  u32string() const { return this->string<char32_t>(); }

	  #if defined(__cpp_char8_t) || defined(FAKESTD_USE_CHAR8_T)
	    basic_string<char8_t>   u8string()  const { return this->string<char8_t >(); }
	  #else
	    basic_string<char>		u8string()  const { return this->string<char    >(); }
	  #endif

      #if __cplusplus >= 201103L || _MSVC_LANG >= 201103L || _MSC_VER >= 1900
	    template <class OC, class TR=std::char_traits<OC>, class A=std::allocator<OC> > basic_string<OC,TR,A>
	    generic_string(const A& a) const { basic_string<OC,TR,A> dst(a); generic_conv(dst, str_); return dst; }
	    template <class OC, class TR=std::char_traits<OC>, class A=std::allocator<OC> > basic_string<OC,TR,A>
	    generic_string() const { basic_string<OC,TR,A> dst; generic_conv(dst, str_); return dst; }
	  #else
	    template <class OC, class TR, class A> basic_string<OC,TR,A>
	    generic_string(const A& a) const { basic_string<OC,TR,A> dst(a); generic_conv(dst, str_); return dst; }
	    template <class OC> basic_string<OC>
	    generic_string() const { basic_string<OC> dst; generic_conv(dst, str_); return dst; }
	  #endif

	    basic_string<char>      generic_string()    const { return generic_string<char    >(); }
	    basic_string<wchar_t>   generic_wstring()   const { return generic_string<wchar_t >(); }
	    basic_string<char16_t>  generic_u16string() const { return generic_string<char16_t>(); }
	    basic_string<char32_t>  generic_u32string() const { return generic_string<char32_t>(); }
	  #if defined(__cpp_char8_t) || defined(FAKESTD_USE_CHAR8_T)
	    basic_string<char8_t>   generic_u8string()  const { return generic_string<char8_t >(); }
	  #else
	    basic_string<char>		generic_u8string()  const { return generic_string<char    >(); }
	  #endif

	    basic_path& make_preferred() { ccwrap::strpath::make_preferred(str_); return *this; }

	    int compare(basic_path  const& r) const noexcept { return ccwrap::strpath::compare(str_, r.str_); }
	    int compare(string_type const& r) const noexcept { return ccwrap::strpath::compare(str_, r); }
	    int compare(string_view_type   r) const noexcept { return ccwrap::strpath::compare(str_, r); }
	    int compare(value_type  const* r) const noexcept { return ccwrap::strpath::compare(str_, r); }

	    basic_path root_name()      const { return basic_path(ccwrap::strpath::root_name(     str_)); }
	    basic_path root_directory() const { return basic_path(ccwrap::strpath::root_directory(str_)); }
	    basic_path root_path()      const { return basic_path(ccwrap::strpath::root_path(     str_)); }
	    basic_path relative_path()  const { return basic_path(ccwrap::strpath::relative_path( str_)); }
	    basic_path parent_path()    const { return basic_path(ccwrap::strpath::parent_path(   str_)); }
	    basic_path filename()       const { return basic_path(ccwrap::strpath::filename(      str_)); }
	    basic_path stem()           const { return basic_path(ccwrap::strpath::stem(          str_)); }
	    basic_path extension()      const { return basic_path(ccwrap::strpath::extension(     str_)); }

	    bool empty() 			  const noexcept { return str_.empty(); }	// [[nodiscard]]
	    bool has_root_name()      const noexcept { return ccwrap::strpath::has_root_name(     str_); }
	    bool has_root_directory() const noexcept { return ccwrap::strpath::has_root_directory(str_); }
	    bool has_root_path()      const noexcept { return ccwrap::strpath::has_root_path(     str_); }
	    bool has_relative_path()  const noexcept { return ccwrap::strpath::has_relative_path( str_); }
	    bool has_parent_path()    const noexcept { return ccwrap::strpath::has_parent_path(   str_); }
	    bool has_filename()       const noexcept { return ccwrap::strpath::has_filename(      str_); }
	    bool has_stem()           const noexcept { return ccwrap::strpath::has_stem(          str_); }
	    bool has_extension()      const noexcept { return ccwrap::strpath::has_extension(     str_); }
	    bool is_absolute()        const noexcept { return ccwrap::strpath::is_absolute(       str_); }
	    bool is_relative()        const noexcept { return ccwrap::strpath::is_relative(       str_); }

	    basic_path lexically_normal() const {
	        basic_path tmp(*this);
	        ccwrap::strpath::lexically_normal(tmp.str_);
	        return tmp;
	    }
	    basic_path lexically_relative(basic_path const& base) const {
	        basic_path tmp(*this);
	        ccwrap::strpath::lexically_relative(tmp.str_, base.str_);
	        return tmp;
	    }
	    basic_path lexically_proximate(basic_path const& base) const {
	        basic_path tmp(*this);
	        ccwrap::strpath::lexically_proximate(tmp.str_, base.str_);
	        return tmp;
	    }

	    iterator begin() { return iterator(&str_, false); }
	    iterator end()   { return iterator(&str_, true ); }

	    basic_path& operator+=(basic_path const& r) { str_ += r.str_; return *this; }
	    basic_path& operator+=(const string_type& r) { str_ += r; return *this; }
	    basic_path& operator+=(string_view_type r) { str_ += r; return *this; }
	    basic_path& operator+=(value_type const* r) { str_ += r; return *this; }
	    basic_path& operator+=(value_type r) { str_ += r; return *this; }
	    template <class S> basic_path& operator+=(S const& r) { conv_add(str_, r); return *this; }
	    template <class S> basic_path& concat(S const& r) { return operator+=(r); }
	    template <class InpIte> basic_path&
	    concat(InpIte first, InpIte last) { return operator+=(basic_path(first, last)); }

	    basic_path& operator/=(basic_path const& r) { ccwrap::strpath::append(str_, r.str_); return *this; }
	    template <class S> basic_path& operator/=(S const& r) { return operator/=(basic_path(r)); }
	    template <class S> basic_path& append(S const& r) { return operator/=(r); }
	    template <class InpIte> basic_path&
	    append(InpIte first, InpIte last) { return operator/=(basic_path(first, last)); }

	    basic_path& remove_filename() { ccwrap::strpath::remove_filename(str_); return *this; }
	    basic_path& replace_filename(basic_path const& repl)
	    	{ ccwrap::strpath::replace_filename(str_, repl.str_); return *this; }
	    basic_path& replace_extension(basic_path const& r = basic_path())
	    	{ ccwrap::strpath::replace_extension(str_,r.str_); return *this; }

		friend void swap(basic_path& l, basic_path& r) noexcept { l.swap(r); }

		friend basic_path operator/(basic_path const& l, basic_path const& r) { return basic_path(l) /= r; }

     #if __cplusplus >= 201103L || _MSVC_LANG >= 201103L || _MSC_VER >= 1900
		friend basic_path operator/(basic_path&& l, basic_path const& r) { return basic_path(std::move(l)) /= r; }
	 #endif

		bool operator==(basic_path const& r) const noexcept { return compare(r) == 0; }
		bool operator!=(basic_path const& r) const noexcept { return compare(r) != 0; }
		bool operator< (basic_path const& r) const noexcept { return compare(r) <  0; }
		bool operator>=(basic_path const& r) const noexcept { return compare(r) >= 0; }
		bool operator> (basic_path const& r) const noexcept { return compare(r) >  0; }
		bool operator<=(basic_path const& r) const noexcept { return compare(r) <= 0; }
		template<class L> friend bool operator==(L const& l, basic_path const& r) noexcept { return r.operator==(l); }
		template<class L> friend bool operator< (L const& l, basic_path const& r) noexcept { return r.operator> (l); }
		template<class L> friend bool operator> (L const& l, basic_path const& r) noexcept { return r.operator< (l); }
		template<class L> friend bool operator!=(L const& l, basic_path const& r) noexcept { return r.operator!=(l); }
		template<class L> friend bool operator>=(L const& l, basic_path const& r) noexcept { return r.operator<=(l); }
		template<class L> friend bool operator<=(L const& l, basic_path const& r) noexcept { return r.operator>=(l); }

	 #if 0	// Input/output stream is declaration only. Define the basic_path template after the entity.
		template <class OC, class TR> friend std::basic_ostream<OC, TR>&
		operator<<(std::basic_ostream<OC, TR>& ost, basic_path const& r) { return r._output_stream(ost); }

		template <class OC, class TR> friend std::basic_istream<OC, TR>&
		operator>>(std::basic_istream<OC, TR>& ist, basic_path& p) { return p._input_stream(ist); }
	 #endif

	//public for ccwrap
		string_type& mutable_native() { return str_; }
		string_type  take_out() { return std::move(str_); }

		template <class S, class TR> std::basic_ostream<S, TR>&
			_output_stream(std::basic_ostream<S, TR>& ost) const {
			ost << S('"') << this->string<S>() << S('"');
			return ost;
		}

		template <class S, class TR> std::basic_istream<S, TR>& _input_stream(std::basic_istream<S, TR>& ist);

	private:
	    void adjust_fmt(format fmt) {
			if (fmt == auto_format)
				;
			else if (fmt == generic_format)
				ccwrap::strpath::make_generic(str_);
			else if (fmt == native_format)
				ccwrap::strpath::make_preferred(str_);
		}

	    template <class D, class S> static void generic_conv(D& dst, S& src) {
			conv(dst, src);
			ccwrap::strpath::make_generic(dst);
		}

	    template<class D, class S> static D conv_to(S const& src) {
			D dst;
	        conv(dst, src);
	        return dst;
	    }

	    template <class D, class S> static void conv(D& dst, S& src) {
			utfenc::convert(dst, src);
		}

	    template <class D, class S> static void conv_add(D& dst, S& src) {
			utfenc::convert_add(dst, src);
		}

	private:
	    string_type     str_;
	};

	template<typename C>
	template <class S, class TR> std::basic_istream<S, TR>&
	basic_path<C>::_input_stream(std::basic_istream<S, TR>& ist) {
		// ist >> std::quoted(p.str_); //c++14
		if __ccwrap_constexpr_cxx14 (sizeof(S) == sizeof(*str_.c_str())) {
			ist >> str_;
		} else {
			string_type buf;
			ist >> buf;
			str_ = buf;
		}
		if (str_.size() > 1) {
			if (str_.back() == S('"'))
				str_.pop_back();
			if (str_.front() == S('"'))
				str_.erase(str_.begin());
		}
		return ist;
	}

	//TODO: std::size_t hash_value(basic_path const& p) noexcept;

	namespace __path_detail {
		template<typename C>
		path_iterator<C>::path_iterator(string_type const* str, bool is_end)
		    : str_(str)
		{
		    C const* b  	= str->data();
		    size_t   l  	= str->size();
		    cur_        	= (is_end) ? l : 0;
		    C const* e  	= b + l;
		    C const* r  	= ccwrap::strpath::skip_root_name(b, e);
		    rootNameLen_	= int(r - b);
		    C const* d  	= ccwrap::strpath::skip_sep(r, e);
		    rootPathLen_	= int(d - b);
		}

		template<typename C>
		path_iterator<C>& path_iterator<C>::operator++() {
		    if (cur_ < rootPathLen_) {
		        cur_        = (cur_ < rootNameLen_) ? rootNameLen_ : rootPathLen_;
		    } else {
		        C const* b  = str_->data();
		        C const* e  = b + str_->size();
		        C const* p  = ccwrap::strpath::skip_not_sep(b + cur_, e);
		        C const* s	= ccwrap::strpath::skip_sep(p, e);
		        cur_        = s - b;
		    }
		    return *this;
		}

		template<typename C>
		path_iterator<C>& path_iterator<C>::operator--() {
		    if (cur_ <= rootPathLen_) {
		        if (cur_ < rootPathLen_ || rootPathLen_ == rootNameLen_) {
		            assert(cur_ > 0);
		            cur_        = 0;
		        } else {
		            cur_ = rootNameLen_;
		        }
		    } else {
		        C const* b  = str_->data();
				C const* e  = b + cur_; // str_->size();
		        C const* s  = ccwrap::strpath::back_skip_sep(b, e);
		        C const* t  = ccwrap::strpath::back_skip_not_sep(b, s);
		        cur_        = t - b;
		    }
		    return *this;
		}

		template<typename C>
		basic_path<C>    path_iterator<C>::operator*() const {
		    basic_path<C> p;
		    string_type& dst = const_cast<string_type&>(p.native());
			C const* b = str_->data();
			if (cur_ < rootPathLen_) {
		        if (cur_ < rootNameLen_) {
		            dst.assign(b, rootNameLen_);
		        } else {
		            dst.assign(b+rootNameLen_, rootPathLen_-rootNameLen_);
		        }
		    } else {
		        C const* e = b + str_->size();
		        C const* s = b + cur_;
		        C const* t = ccwrap::strpath::skip_not_sep(s, e);
		        dst.assign(s, t - s);
		    }
		    return p;
		}
	}   // __path_detail

}	// ccwrap


namespace std {
	namespace filesystem {
		typedef _ccwrap::basic_path<CCWRAP_PATH_CHAR>	path;

		template <class S>
		path u8path(S const& s) {
		    //return basic_path(unicode::conv_to<u8string>(s).c_str());
		    return path(s);
		}

		template <class InpIte>
		path u8path(InpIte first, InpIte last) {
		    //return basic_path(unicode::conv_to<u8string>(first, last).c_str());
		    return path(first, last);
		}
	}

#if 1
	template <class C, class TR> std::basic_ostream<C, TR>&
		operator<<(std::basic_ostream<C, TR>& ost, filesystem::path const& r) {
		return r._output_stream(ost);
	}

	template <class C, class TR> std::basic_istream<C, TR>&
		operator>>(std::basic_istream<C, TR>& ist, filesystem::path& p) {
		return p._input_stream(ist);
	}
#endif
}


#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif	// FILE_PATH_HPP__
