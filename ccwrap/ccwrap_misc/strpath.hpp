/**
 *  @file   strpath.hpp
 *  @brief  Pathname operations on string types.
 *  @author Masashi Kitamura (tenka@6809.net)
 *  @license Boost Software Lisence Version 1.0
 *  @note
 *      - Function names are similar to std::filesystem::path member functions.
 */
#ifndef STRPATH_INCLUDED__
#define STRPATH_INCLUDED__

#include <cstddef>
#include <string>
#include <string_view>

#if !defined(__CCWRAP_STRPATH_MSW) && (defined(_WIN32) || defined(MSDOS))
 #define __CCWRAP_STRPATH_MSW
#endif

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4127)
#pragma warning(disable:4244)
#endif

namespace ccwrap {

using std::basic_string_view;
using std::basic_string;

namespace strpath {
  #if defined(__CCWRAP_STRPATH_MSW)
	enum { preferred_separator = '\\' };
  #else
	enum { preferred_separator = '/' };
  #endif

	template<typename C> C const*
	str_end(C const* s) noexcept {
	 #if 1 //defined(STRPATH_NO_CHAR_TRAITS)
	    while (*s)
	        ++s;
	    return s;
	 #else
	    return s + std::char_traits<C>::length(s);
     #endif
	}
	template<class S> typename S::value_type const*
	str_end(S const& s) noexcept {
		return s.data() + s.size();
	}

	template<typename C> inline bool
	is_sep(C c) noexcept {
	 #if defined __CCWRAP_STRPATH_MSW
	    return c == C('/') || c == C(preferred_separator);
	 #else
	    return c == C('/');
	 #endif
	}

	template<typename C> C const* 
	skip_sep(C const* b, C const* e) noexcept {
	    if (!b)
	        return e;
	    C const* s = b;
	    while (s < e && (is_sep(*s) || (*s == C('.') && s+1 < e && is_sep(s[1]))) )
	        ++s;
	    return s;
	}

	template<typename C> C const* 
	back_skip_sep(C const* b, C const* p) noexcept {
	    if (!p || b >= p)
	        return b;
	    while (--p >= b && (is_sep(*p) || (*p == C('.') && p-1 >= b && is_sep(p[-1]))) )
	        ;
	    ++p;
	    return p;
	}

	template<typename C> C const*
	skip_not_sep(C const* b, C const* e) noexcept {
	    if (!b)
	        return e;
	    C const* s = b;
	    while (s < e && !is_sep(*s))
	        ++s;
	    return s;
	}

	template<typename C> C const*
	back_skip_not_sep(C const* b, C const* p) noexcept {
	    if (!p || b >= p)
	        return b;
	 #if defined __CCWRAP_STRPATH_MSW
	    while (--p >= b && !is_sep(*p) && *p != C(':'))
	        ;
	 #else
	    while (--p >= b && !is_sep(*p))
	        ;
	 #endif
	    ++p;
	    return p;
	}

	template<typename C> C const* 
	skip_root_name(C const* b, C const* e) noexcept {
	    if (!b)
	        return e;
	    C const* s = b;
	    if (s < e && *s != C(':')) {
	        while (s < e && !is_sep(*s)) {
	            if (*s == C(':'))
	                return s + 1;
	            ++s;
	        }
	    }
	    return b;
	}

	template<typename C> inline C const* 
	skip_root_path(C const* b, C const* e) noexcept {
	    C const* p = skip_root_name(b, e);
	    return skip_sep(p, e);
	}


	template<typename C> C const*
	skip_to_filename(C const* b, C const* e) noexcept {
	    if (!b)
	        return e;
	    C const* p = b; //skip_root_name(b, e);
	    while (p < e) {
	        if (is_sep(*p))
	            b = p + 1;
	        ++p;    // p = char_next(p);
	    }
	    return b;
	}

	template<typename C> C const*
	skip_to_extension(C const* b, C const* e) noexcept {
	    if (!b)
	        return e;
	    C const* s = skip_to_filename(b, e);
	    while (s < e && *s == C('.'))
	        ++s;
	    C const* a = s;
	    C const* p = s;
	    while (p < e) {
	        if (*p == C('.'))
	            a = p;
	        ++p;    // p = char_next(p);
	    }
	    if (a == s)
	        a = e;
	    return a;
	}

	template<class S> inline void
	add_sep(S& str) {
		typedef typename S::value_type C;
	 #if __CCWRAP_CXX >= 201103L
	    if (!str.empty() && !is_sep(str.back()))
	        str.push_back(C(preferred_separator));
	 #else
	    if (!str.empty() && !is_sep(str[str.size()-1]))
	        str.push_back(C(preferred_separator));
	 #endif
	}

	template<class S> inline void
	remove_sep(S& str) {
	    if (!str.empty() && is_sep(str.back()))
	        str.pop_back();
	}
	template<class C> inline void
	remove_sep(basic_string_view<C>& str) {
	    if (!str.empty() && is_sep(str.back())) {
	        //str.pop_back();
	        str.assign(str.data(), str.size()-1);
	    }
	}


	// - - - std::filesystem::path member like function - - -

	template<typename C> bool
	is_absolute(C const* b, C const* e) noexcept {
	    if (b && b < e) {
	        C const* p = skip_root_name(b, e);
	        return is_sep(*p);
	    }
	    return false;
	}
	template<class S> inline bool
	is_absolute(S const& str) noexcept {
		return is_absolute(str.data(), str_end(str));
	}
	template<typename C> inline bool
	is_absolute(C const* str) noexcept {
		return is_absolute(str, str_end(str));
	}

	template<class S> inline bool
	is_relative(S const& str) noexcept {
		return !is_absolute(str);
	}
	template<typename C> inline bool
	is_relative(C const* str) noexcept {
		return !is_absolute(str);
	}

	template<typename C> basic_string_view<C>
	root_name(C const* b, C const* e) noexcept {
	    if (b && b < e) {
	        C const* p = skip_root_name(b, e);
	        return basic_string_view<C>(b, p - b);
	    }
	    return basic_string_view<C>();
	}
	template<class S> inline basic_string_view<typename S::value_type>
	root_name(S const& str) noexcept { return root_name(str.data(), str_end(str)); }
	template<typename C> inline basic_string_view<C>
	root_name(C const* str) noexcept { return root_name(str, str_end(str)); }

	template<typename C> basic_string_view<C>
	root_directory(C const* b, C const* e) noexcept {
	    if (b && b < e) {
	        b          = skip_root_name(b, e);
	        C const* p = skip_sep(b, e);
	        return basic_string_view<C>(b, p - b);
	    }
	    return basic_string_view<C>();
	}
	template<class S> inline basic_string_view<typename S::value_type>
	root_directory(S const& str) noexcept { return root_directory(str.data(), str_end(str)); }
	template<typename C> inline basic_string_view<C>
	root_directory(C const* str) noexcept { return root_directory(str, str_end(str)); }

	template<typename C> basic_string_view<C>
	root_path(C const* b, C const* e) noexcept {
	    if (b && b < e) {
	        C const* p = skip_root_name(b, e);
	        p         = skip_sep(p, e);
	        return basic_string_view<C>(b, p - b);
	    }
	    return basic_string_view<C>();
	}
	template<class S> inline basic_string_view<typename S::value_type>
	root_path(S const& str) noexcept { return root_path(str.data(), str_end(str)); }
	template<typename C> inline basic_string_view<C>
	root_path(C const* str) noexcept { return root_path(str, str_end(str)); }

	template<typename C> basic_string_view<C>
	relative_path(C const* b, C const* e) noexcept {
	    if (b && b < e) {
	        C const* p = skip_root_path(b, e);
	        return basic_string_view<C>(p, e - p);
	    }
	    return basic_string_view<C>();
	}
	template<class S> inline basic_string_view<typename S::value_type>
	relative_path(S const& str) noexcept { return relative_path(str.data(), str_end(str)); }
	template<typename C> inline basic_string_view<C>
	relative_path(C const* str) noexcept { return relative_path(str, str_end(str)); }

	template<typename C> basic_string_view<C>
	filename(C const* b, C const* e) noexcept {
	    if (b && b < e) {
	        C const* p = skip_to_filename(skip_root_path(b, e), e);
	        return basic_string_view<C>(p, e - p);
	    }
	    return basic_string_view<C>();
	}
	template<class S> inline basic_string_view<typename S::value_type>
	filename(S const& str) noexcept { return filename(str.data(), str_end(str)); }
	template<typename C> inline basic_string_view<C>
	filename(C const* str) noexcept { return filename(str, str_end(str)); }

	template<typename C> basic_string_view<C>
	extension(C const* b, C const* e) noexcept {
	    if (b && b < e) {
	        C const* p = skip_to_extension(skip_root_path(b, e), e);
	        return basic_string_view<C>(p, e - p);
	    }
	    return basic_string_view<C>();
	}
	template<class S> inline basic_string_view<typename S::value_type>
	extension(S const& str) noexcept { return extension(str.data(), str_end(str)); }
	template<typename C> inline basic_string_view<C>
	extension(C const* str) noexcept { return extension(str, str_end(str)); }

	template<typename C> basic_string_view<C>
	stem(C const* b, C const* e) noexcept {
	    if (b && b < e) {
	        C const* s = skip_to_filename(skip_root_path(b, e), e);
	        C const* p = skip_to_extension(s, e);
	        return basic_string_view<C>(s, p - s);
	    }
	    return basic_string_view<C>();
	}
	template<class S> inline basic_string_view<typename S::value_type>
	stem(S const& str) noexcept { return stem(str.data(), str_end(str)); }
	template<typename C> inline basic_string_view<C>
	stem(C const* str) noexcept { return stem(str, str_end(str)); }

	template<typename C> basic_string_view<C>
	parent_path(C const* b, C const* e) noexcept {
	    if (b && b < e) {
	        C const* r = skip_root_path(b, e);
	        C const* f = skip_to_filename(r, e);
	        f          = back_skip_sep(r, f);
	        return basic_string_view<C>(b, f - b);
	    }
	    return basic_string_view<C>();
	}
	template<class S> inline basic_string_view<typename S::value_type>
	parent_path(S const& str) noexcept { return parent_path(str.data(), str_end(str)); }
	template<typename C> inline basic_string_view<C>
	parent_path(C const* str) noexcept { return parent_path(str, str_end(str)); }

	template<typename S> inline bool has_root_name(S const& str) noexcept
		{ return !root_name(str).empty(); }
	template<typename C> inline bool has_root_name(C const* str) noexcept
		{ return !root_name(str).empty(); }

	template<typename S> inline bool has_root_path(S const& str) noexcept
		{ return !root_path(str).empty(); }
	template<typename C> inline bool has_root_path(C const* str) noexcept
		{ return !root_path(str).empty(); }

	template<typename S> inline bool has_root_directory(S const& str) noexcept
		{ return !root_directory(str).empty(); }
	template<typename C> inline bool has_root_directory(C const* str) noexcept
		{ return !root_directory(str).empty(); }

	template<typename S> inline bool has_relative_path(S const& str) noexcept
		{ return !relative_path(str).empty(); }
	template<typename C> inline bool has_relative_path(C const* str) noexcept
		{ return !relative_path(str).empty(); }

	template<typename S> inline bool has_filename(S const& str) noexcept
		{ return !filename(str).empty(); }
	template<typename C> inline bool has_filename(C const* str) noexcept
		{ return !filename(str).empty(); }

	template<typename S> inline bool has_extension(S const& str) noexcept
		{ return !extension(str).empty(); }
	template<typename C> inline bool has_extension(C const* str) noexcept
		{ return !extension(str).empty(); }

	template<typename S> inline bool has_stem(S const& str) noexcept
		{ return !stem(str).empty(); }
	template<typename C> inline bool has_stem(C const* str) noexcept
		{ return !stem(str).empty(); }

	template<typename S> inline bool has_parent_path(S const& str) noexcept
		{ return !parent_path(str).empty(); }
	template<typename C> inline bool has_parent_path(C const* str) noexcept
		{ return !parent_path(str).empty(); }

	template<class S> void
	make_preferred(S& str) {
	 #if defined(__CCWRAP_STRPATH_MSW)
	    if (!str.empty()) {
			typedef typename S::value_type C;
	        C* b = &str[0];
	        C* e = b + str.size();
	        while (b < e) {
	            if (*b == C('/'))
	                *b = C(preferred_separator);
				++b;
	        }
	    }
	 #endif
	}

	template<class S> void
	make_generic(S& str) {
	 #if defined(__CCWRAP_STRPATH_MSW)
	    if (!str.empty()) {
			typedef typename S::value_type C;
	        C* b = &str[0];
	        C* e = b + str.size();
	        while (b < e) {
	            if (*b == C(preferred_separator))
	                *b = C('/');
				++b;
	        }
	    }
	 #endif
	}

	namespace __detail {
		template<typename C> inline int
		get_ch_for_cmp(C const*& s, C const* e) noexcept {
		    if (s < e) {
		        C c = *s++;
		        if (is_sep(c)) {
		            c = C(preferred_separator);
		            s = skip_sep(s, e);
		        }
		        return c;
		    }
		    return -1;
		}

		template<typename C> int
		compare_impl(C const* lb, C const* le, C const* rb, C const* re) noexcept {
			std::size_t lsz = le - lb;
			std::size_t rsz = re - rb;
		    if (lsz == 0) {
		        return (rsz == 0) ? 0 : -1;
		    } else if (rsz == 0) {
		        return 1;
		    }

		    C const* lp = lb;
		    C const* rp = rb;

		    int lc, rc;
		    do {
		        lc = get_ch_for_cmp(lp, le);
		        rc = get_ch_for_cmp(rp, re);
		        if (lc != rc)
		            return (lc < rc) ? -1 : /*lc > rc ?*/ 1;
		    } while (lc >= 0);

		    return (lc == rc) ? 0 : (lp == le /*&& rp < re*/) ? 1 : /* (lp < le && rp == re) ?*/ -1;
		}
	}
	template<typename C> int
	compare(C const* lb, C const* le, C const* rb, C const* re) noexcept {
		typedef typename ccwrap::utfenc::to_uchar<C>::type UC;
		return __detail::compare_impl((UC const*)lb, (UC const*)le, (UC const*)rb, (UC const*)re);
	}
	template<class S, class T> inline int
	compare(S const& lstr, T const& rstr) noexcept {
		return compare(lstr.data(), str_end(lstr), rstr.data(), str_end(rstr));
	}
	template<typename C> inline int
	compare(C const* lstr, C const* rstr) noexcept {
		return compare(lstr, str_end(lstr), rstr, str_end(rstr));
	}
	template<class S> inline int
	compare(S const& lstr, typename S::value_type const* rstr) noexcept {
		return compare(lstr.data(), str_end(lstr), rstr, str_end(rstr));
	}
	template<class S> inline int
	compare(typename S::value_type const* lstr, S const& rstr) noexcept {
		return compare(lstr, str_end(lstr), rstr.data(), str_end(rstr));
	}

	template<class S> void
	remove_filename(S& str) {
	    if (!str.empty()) {
			typedef typename S::value_type C;
	        C const* b = str.data();
	        C const* e = b + str.size();
	        C const* r = skip_root_path(b, e);
	        C const* f = skip_to_filename(r, e);
	        str.resize(f - b);
	    }
	}

	template<class S> void
	replace_extension(S& str, typename S::value_type const* repl_b, typename S::value_type const* repl_e) {
	    if (!str.empty()) {
			typedef typename S::value_type C;
	        C const* b = str.data();
	        C const* e = b + str.size();
	        C const* p = skip_to_extension(skip_root_path(b, e), e);
	        size_t   t = p - b;
	        size_t   repl_sz = repl_e - repl_b;
	        bool     nopir   = repl_sz && (repl_b[0] != C('.'));
	        size_t   sz      = repl_sz + size_t(nopir);
	        str.resize(t + sz);
	        if (nopir)
	            str[t++] = C('.');
	        if (repl_sz)
	            std::copy(repl_b, repl_e, str.begin()+t);
	    }
	}
	template<class S, class T> inline void
	replace_extension(S& str, T const& repl) {
		replace_extension(str, repl.data(), str_end(repl));
	}
	template<class S> inline void
	replace_extension(S& str, typename S::value_type const* repl) {
		replace_extension(str, repl, str_end(repl));
	}

	template<class S> inline void
	remove_extension(S& str) {
		replace_extension(str, (typename S::value_type const*)nullptr, (typename S::value_type const*)nullptr);
	}

	template<class S> void
	append(S& str, typename S::value_type const* add_b, typename S::value_type const* add_e) {
		typedef typename S::value_type C;
		std::size_t add_sz = add_e - add_b;
	    if (add_sz == 0)
	        return;
		basic_string_view<C> add(add_b, add_sz);
		basic_string_view<C> add_root = root_name(add);
		if (strpath::is_absolute(add)
		 #if defined(_WIN32) //FAKESTD_MSW
			 && add_root.size() > 0
		 #endif
		) {
			str.assign(add_b, add_e-add_b);
	        return;
 	    }
	    if (add_root.size()) {
	        basic_string_view<C> str_root = root_name(str);
	        if (compare(str_root, add_root)) {
	        	str.assign(add_b, add_e-add_e);
	            return;
	        }
	        add = relative_path(add);
	    } else if (has_root_directory(add)) {
	        C const* b = str.data();
	        C const* e = b + str.size();
	        C const* p = skip_root_name(b, e);
	        str.resize(p - b);
	        str.append(add.data(), add.size());
	        return;
	    }
	    str.reserve(1 + add.size() + 1);
	    add_sep(str);
	    str.append(add.data(), add.size());
	}
	template<class S, class T> inline void append(S& str, T const& add)
		{ return append(str, add.data(), str_end(add)); }
	template<class S> inline void append(S& str, typename S::value_type const* add)
		{ return append(str, add, str_end(add)); }

	template<class S, class T> inline
    void replace_filename(S& str, T const& repl) {
		remove_filename(str);
		append(str, repl);
	}
	template<class S, class T> inline
    void replace_filename(S& str, T const* repl) {
		remove_filename(str);
		append(str, repl);
	}

	template<class S> void
	to_absolute(S& str, typename S::value_type const* base_b, typename S::value_type const* base_e) {
	    if (is_relative(str)) {
	        S    tmp(base_b, base_e);
	        append(tmp, str);
		 #if __cplusplus >= 201103L || _MSC_VER >= 1900
			str = std::move(tmp);
		 #else
			str.swap(tmp);
		 #endif
	    }
	}
	template<class S, class T> inline void to_absolute(S& str, T const& base)
		{ return to_absolute(str, base.data(), str_end(base)); }
	template<class S> inline void to_absolute(S& str, typename S::value_type const* base)
		{ return to_absolute(str, base, str_end(base)); }

	template<class S> void
	lexically_normal(S& str) {
		typedef typename S::value_type C;
	    if (str.empty())
	        return;
	    C const* b   = str.data();
	    C const* e   = b + str.size();
	    C const* s   = skip_root_name(b, e);
	    C const* top = s;
	    C* d   = (C*)s;
	    //bool    first = true;
	    C  sep = C('/');
	    if (is_sep(*s)) {
	     #if defined(__CCWRAP_STRPATH_MSW)
	        if (*s == C(preferred_separator))
	            sep = C(preferred_separator);
	     #endif
	        ++s;
			if (top - b > 2) {
				*d++ = sep;         /* '/' '//' '///' */
				if (is_sep(*s)) {
					++s;
					*d++ = sep;     /* '//' '///' */
				}
			} else if (!is_sep(*s)) {
				--s;
			}
	    }
	    while (s < e) {
	        C c = *s++;
	        if (is_sep(c)) {    // '/' or '\\'
	            c = sep;
	            if (s < e) {
	                C c2 = *s;
	                while (is_sep(c2) && s < e) {
	                    c2 = *++s;
	                }
	                if (c2 == C('.')) {
	                    if (s+1 >= e) { // Only "."
	                        break;
	                    }
	                    C c3 = s[1];
	                    if (is_sep(c3)) {   // "./"
							s += 1;
	                        continue;
	                    } else if (c3 == C('.') && (s+2 >= e || is_sep(s[2]))) {    // ".." or "../"
	                        s += 2;
	                        while (--d >= top && !is_sep(*d))   // remove 1 directory
	                            ;
	                        ++d;
	                        continue;
	                    }
	                }
	            }
	        }
	        *d++ = c;
	    }
	    if (d < e)
	        *d = C(0);
	    str.resize(d - b);
	}

	namespace __detail { 
		template<class S> void lexically_rel_impl(S& str, unsigned int flags
			, typename S::value_type const* base_b, typename S::value_type const* base_e) {
			typedef typename S::value_type C;

			if (str.empty() || base_b >= base_e)
				return;

			C const* lb = str.data();
			C const* le = lb + str.size();
			C const* lp = skip_root_name(lb, le);
			C const* rb = base_b;
			C const* re = base_e;
			C const* rp = skip_root_name(rb, re);

			if (compare(lb, lp, rb, rp) != 0) {	// root_name() != base.root_name()
				if (flags & 1)
					str.clear();
				return;
			}

			C const* lpSav = lp;
			C const* rpSav = rp;

			int lc = get_ch_for_cmp(lp, le);
			int rc = get_ch_for_cmp(rp, re);

			if (is_sep(lc) != is_sep(rc)			// is_absolute() != base.is_absolute()
				//|| (!is_sep(lc) && is_sep(rc))	// !has_root_directory() && base.has_root_directory()
			) {
				if (flags & 1)
					str.clear();
				return;
			}
			lp = lpSav;
			rp = rpSav;

			do {
				lc = get_ch_for_cmp(lp, le);
				rc = get_ch_for_cmp(rp, re);
				if (lc != rc)
					break;
				if (is_sep(lc) && is_sep(rc)) {
					lpSav = lp;
					rpSav = rp;
				}
			} while (lc >= 0);


			if (lc < 0 && rc < 0 /*&& lp == le && rp == re*/) {
				str.clear();
				str.push_back(C('.'));
				return;
			}

			if (!is_sep(lc) || (rc >= 0 && !is_sep(rc))) {
				lp = lpSav;
				rp = rpSav;
			}

			size_t  dirCnt = 0;

			C const* rp2 = rp;
			while (rp2 < re) {
				C rc2 = *rp2++;
				if (is_sep(rc2))
					++dirCnt;
			}

			basic_string<C>   tmp;
			tmp.reserve(dirCnt * 3 + le - lp + 4);
			while (rp < re) {
				if (is_sep(*rp)) {
					static C const updir[3] = { C('.'), C('.'), C('/') };
					tmp.append(updir, 3);
				}
				++rp;
			}
			tmp.append(lp, le - lp);
		 #if __cplusplus >= 201103L || _MSC_VER >= 1900
			str = std::move(tmp);
		 #else
			str.swap(tmp);
		 #endif
			return;
		}
	}

	template<typename S, class T> inline void 
	lexically_relative(S& str, T const& base) {
		__detail::lexically_rel_impl(str, true, base.data(), str_end(base));
	}
	template<typename S> inline void 
	lexically_relative(S& str, typename S::value_type const* base) {
		__detail::lexically_rel_impl(str, true, base, str_end(base));
	}

	template<typename S, class T> inline void 
	lexically_proximate(S& str, T const& base) {
		__detail::lexically_rel_impl(str, false, base.data(), str_end(base));
	}
	template<typename S> inline void 
	lexically_proximate(S& str, typename S::value_type const* base) {
		__detail::lexically_rel_impl(str, false, base, str_end(base));
	}

}	// strpath

}	// ccwrap

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif	// __CCWRAP_PATH_INCLUDED__
