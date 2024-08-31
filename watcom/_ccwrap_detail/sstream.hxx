#if 0
/*
 * Copyright (c) 1999
 * Silicon Graphics Computer Systems, Inc.
 *
 * Copyright (c) 1999
 * Boris Fomitchev
 *
 * This material is provided "as is", with absolutely no warranty expressed
 * or implied. Any use is at your own risk.
 *
 * Permission to use or copy this software for any purpose is hereby granted
 * without fee, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 */
// moddified for ccwrap by Masashi Kitamura
#ifndef __CCWRAP_SSTREAM_CXX03_INCLUDED
#define __CCWRAP_SSTREAM_CXX03_INCLUDED


#include <cassert>
#include <cstdarg>
#include <string>
#include <iostream>
#include <streambuf>


namespace std {

template <class C, class T, class A>
class basic_stringbuf : public basic_streambuf<C, T> {
  //typedef basic_stringbuf<C,T,A>  this_type;
  //typedef basic_streambuf<C,T>    base_type;
public:
  typedef T                    traits_type;
  typedef C                    char_type;
  typedef typename T::int_type int_type;
  typedef typename T::pos_type pos_type;
  typedef typename T::off_type off_type;
  typedef basic_string<C,T,A>  string_type;

  explicit basic_stringbuf(ios_base::openmode m = ios_base::in | ios_base::out);
  explicit basic_stringbuf(string_type const& s
  			, ios_base::openmode m = ios_base::in | ios_base::out);
  virtual ~basic_stringbuf();

  string_type str() const { return str_; }
  void str(string_type const& s);

protected:
  virtual int_type underflow();
  virtual int_type uflow();
  virtual int_type pbackfail(int_type c);
  virtual int_type overflow(int_type c);

  int_type pbackfail() { return pbackfail(T::eof()); }
  int_type overflow()  { return overflow( T::eof()); }

  virtual streamsize xsputn(const char_type* s, streamsize n);
  //virtual streamsize _M_xsputnc(char_type c, streamsize n);

  virtual basic_streambuf<C,T>* setbuf(C* buf, streamsize n);
  virtual pos_type   seekoff(off_type ofs, ios_base::seekdir dir
  			, ios_base::openmode mode = ios_base::in | ios_base::out);
  virtual pos_type seekpos(pos_type pos
  			, ios_base::openmode mode = ios_base::in | ios_base::out);
private:
  void      set_ptrs();
  static C* start( string_type const& s) { return const_cast<C*>s.data(); }
  static C* finish(string_type const& s) { return const_cast<C*>s.data() + s.size(); }

private:
  ios_base::openmode	mode_;
  string_type 			str_;
};


//----------------------------------------------------------------------
// Class basic_istringstream, an input stream that uses a stringbuf.

template <class C, class T, class A>
class basic_istringstream : public basic_istream<C, T> {
  //typedef basic_istream<C,T>    base_type;
public:
  typedef T                       traits_type;
  typedef typename T::char_type   char_type;
  typedef typename T::int_type    int_type;
  typedef typename T::pos_type    pos_type;
  typedef typename T::off_type    off_type;

  typedef basic_ios<C,T>          ios_type;
  typedef basic_string<C,T,A>     string_type;
  typedef basic_stringbuf<C,T,A>  buf_type;

  basic_istringstream(ios_base::openmode mode = ios_base::in);
  basic_istringstream(string_type const& str,
                      ios_base::openmode mode = ios_base::in);
  ~basic_istringstream();

  basic_stringbuf<C,T,A>* rdbuf() const
    { return const_cast<buf_type*>(&buf_); }

  string_type str() const { return buf_.str(); }
  void str(string_type const& s) { buf_.str(s); }

private:
  typedef basic_istringstream<C,T,A> this_type;

  //explicitely defined as private to avoid warnings:
  basic_istringstream(this_type const&);
  this_type& operator = (this_type const&);

private:
  basic_stringbuf<C,T,A> buf_;
};


//----------------------------------------------------------------------
// Class basic_ostringstream, an output stream that uses a stringbuf.

template <class C, class T, class A>
class basic_ostringstream : public basic_ostream<C, T> {
public:
  //typedef basic_ostream<C, T>   base_type;
  typedef typename T::char_type   char_type;
  typedef typename T::int_type    int_type;
  typedef typename T::pos_type    pos_type;
  typedef typename T::off_type    off_type;
  typedef T                       traits_type;

  typedef basic_string<C,T,A>     string_type;
  typedef basic_stringbuf<C,T,A>  buf_type;

  basic_ostringstream(ios_base::openmode mode = ios_base::out);
  basic_ostringstream(string_type const& str
  					, ios_base::openmode mode = ios_base::out);
  ~basic_ostringstream();

  basic_stringbuf<C,T,A>* rdbuf() const { return const_cast<buf_type*>(&buf_); }

  string_type str() const { return buf_.str(); }
  void str(string_type const& s) { buf_.str(s); } // dwa 02/07/00 - BUG STOMPER DAVE

private:
  typedef basic_ostringstream<C,T,A> this_type;

  //explicitely defined as private to avoid warnings:
  basic_ostringstream(this_type const&);
  this_type& operator = (this_type const&);

private:
  basic_stringbuf<C,T,A> buf_;
};


//----------------------------------------------------------------------
// Class basic_stringstream, a bidirectional stream that uses a stringbuf.

template <class C, class T, class A>
class basic_stringstream : public basic_iostream<C, T> {
public:
  //typedef basic_iostream<C,T>    	base_type;
  typedef T                     	traits_type;
  typedef typename T::char_type 	char_type;
  typedef typename T::int_type  	int_type;
  typedef typename T::pos_type  	pos_type;
  typedef typename T::off_type  	off_type;
  typedef ios_base::openmode    	openmode;

  typedef basic_ios<C,T>         	ios_type;
  typedef basic_string<C,T,A>    	string_type;
  typedef basic_stringbuf<C,T,A> 	buf_type;

public:
  basic_stringstream(openmode mod = ios_base::in | ios_base::out);
  basic_stringstream(string_type const& str,
                     openmode mod = ios_base::in | ios_base::out);
  ~basic_stringstream();

  basic_stringbuf<C,T,A>* rdbuf() const
    { return const_cast<buf_type*>(&buf_); }

  string_type str() const { return buf_.str(); }
  void        str(string_type const& s) { buf_.str(s); }

private:
  typedef basic_stringstream<C,T,A>	this_type;
  //explicitely defined as private to avoid warnings:
  basic_stringstream(this_type const&);
  this_type& operator = (this_type const&);

private:
  basic_stringbuf<C,T,A> 			buf_;
};


//----------------------------------------------------------------------
// Non-inline stringbuf member functions.

// Constructors.  Note that the base class constructor sets all of the
// get and area pointers to null.

template <class C, class T, class A>
basic_stringbuf<C,T,A>::basic_stringbuf(ios_base::openmode mode)
    : basic_streambuf<C, T>(), mode_(mode), str_()
{
}

template <class C, class T, class A>
basic_stringbuf<C,T,A>::basic_stringbuf(const basic_string<C,T,A>& s, ios_base::openmode mode)
    : basic_streambuf<C, T>(), mode_(mode), str_(s)
{
  set_ptrs();
}

template <class C, class T, class A>
basic_stringbuf<C,T,A>::~basic_stringbuf()
{}

// Set the underlying string to a new value.
template <class C, class T, class A>
void
basic_stringbuf<C,T,A>::str(const basic_string<C,T,A>& s)
{
  str_ = s;
  set_ptrs();
}

template <class C, class T, class A>
void basic_stringbuf<C,T,A>::set_ptrs()
{
  C* data_ptr = this->start(str_);
  C* data_end = this->finish(str_);
  // The initial read position is the beginning of the string.
  if (mode_ & ios_base::in) {
    this->setg(data_ptr, (mode_ & ios_base::ate) ? data_end : data_ptr, data_end);
  }

  // The initial write position is the beginning of the string.
  if (mode_ & ios_base::out) {
    if ( mode_ & (ios_base::app | ios_base::ate) ) {
      this->setp( data_end, data_end );
    } else {
      this->setp( data_ptr, data_end );
      this->pbump((int)str_.size()); // initial write position, if we initialized with string
    }
    // this->setp((mode_ & (ios_base::app | ios_base::ate))? data_end : data_ptr, data_end);
  }
}

// Precondition: gptr() >= egptr().  Returns a character, if one is available.
template <class C, class T, class A>
int basic_stringbuf<C,T,A>::underflow()
{
  return this->gptr() != this->egptr()
    ? T::to_int_type(*this->gptr())
    : T::eof();
}

// Precondition: gptr() >= egptr().
template <class C, class T, class A>
int basic_stringbuf<C,T,A>::uflow()
{
  if (this->gptr() != this->egptr()) {
    int_type c = T::to_int_type(*this->gptr());
    this->gbump(1);
    return c;
  } else {
    return T::eof();
  }
}

template <class C, class T, class A>
int basic_stringbuf<C,T,A>::pbackfail(int_type c)
{
  if (this->gptr() != this->eback()) {
    if (!T::eq_int_type(c, T::eof())) {
      if (T::eq(T::to_char_type(c), this->gptr()[-1])) {
        this->gbump(-1);
        return c;
      }
      else if (mode_ & ios_base::out) {
        this->gbump(-1);
        *this->gptr() = T::to_char_type(c);
        return c;
      }
      else
        return T::eof();
    }
    else {
      this->gbump(-1);
      return T::not_eof(c);
    }
  }
  else
    return T::eof();
}

template <class C, class T, class A>
int basic_stringbuf<C,T,A>::overflow(int_type c)
{
  if (!T::eq_int_type(c, T::eof())) {
    if (mode_ & ios_base::out) {
      if ( this->pptr() < this->epptr() ) { // just put back in any case
        str_.push_back( T::to_char_type(c) );
        this->pbump(1);
      } else if ( mode_ & ios_base::in ) {
        ptrdiff_t offset = this->gptr() - this->eback();
        str_.push_back(T::to_char_type(c));
        C* data_ptr = this->start(str_);
        this->setg(data_ptr, data_ptr + offset, this->finish(str_));
        this->setp(data_ptr, this->finish(str_));
        this->pbump((int)str_.size());
      } else {
        str_.push_back( T::to_char_type(c) );
        this->setp(this->start(str_), this->finish(str_));
        this->pbump((int)str_.size());
      }
      return c;
    }
    return T::eof(); // Overflow always fails if it's read-only
  }
  return T::not_eof(c); // c is EOF, so we don't have to do anything
}

template <class C, class T, class A>
streamsize
basic_stringbuf<C,T,A>::xsputn(const char_type* s, streamsize n)
{
  streamsize nwritten = 0;

  if ((mode_ & ios_base::out) && n > 0) {
    // If the put pointer is somewhere in the middle of the string,
    // then overwrite instead of append.
    if ( !str_.empty() && this->pbase() == this->start(str_)) {
      ptrdiff_t avail = this->finish(str_) - this->pptr();
      if (avail > n) {
        T::copy(this->pptr(), s, size_t(n));
        this->pbump((int)n);
        return n;
      } else {
        T::copy(this->pptr(), s, avail);
        nwritten += avail;
        n -= avail;
        s += avail;
      }
    }

    // At this point we know we're appending.
    C* data_ptr;
    if (mode_ & ios_base::in) {
      ptrdiff_t get_offset = this->gptr() - this->eback();
      str_.append(s, s + ptrdiff_t(n));
      data_ptr = this->start(str_);
      this->setg(data_ptr, data_ptr + get_offset, this->finish(str_));
    } else {
      str_.append(s, s + ptrdiff_t(n));
      data_ptr = this->start(str_);
    }

    this->setp(data_ptr, this->finish(str_));
    this->pbump((int)str_.size());
    nwritten += n;
  }

  return nwritten;
}

#if 0
template <class C, class T, class A>
streamsize
basic_stringbuf<C,T,A>::_M_xsputnc(char_type c, streamsize n) {
  streamsize nwritten = 0;

  if ((mode_ & ios_base::out) && n > 0) {
    // If the put pointer is somewhere in the middle of the string,
    // then overwrite instead of append.
    if (this->pbase() == this->start(str_)) {
      ptrdiff_t avail = this->finish(str_) - this->pptr();
      if (avail > n) {
        T::assign(this->pptr(), size_t(n), c);
        this->pbump(int(n));
        return n;
      }
      else {
        T::assign(this->pptr(), avail, c);
        nwritten += avail;
        n -= avail;
      }
    }

    // At this point we know we're appending.
    size_t app_size = sizeof(streamsize) > sizeof(size_t)
    				  ? size_t((min)(n, streamsize(str_.max_size())))
                      : size_t(n);
    C* data_ptr;
    if (this->mode_ & ios_base::in) {
      ptrdiff_t get_offset = this->gptr() - this->eback();
      str_.append(app_size, c);
      data_ptr = this->start(str_);
      this->setg(data_ptr, data_ptr + get_offset, this->finish(str_));
    } else {
      str_.append(app_size, c);
      data_ptr = this->start(str_);
    }

    this->setp(data_ptr, this->finish(str_));
    this->pbump((int)str_.size());
    nwritten += app_size;
  }

  return nwritten;
}
#endif

// According to the C++ standard the effects of setbuf are implementation
// defined, except that setbuf(0, 0) has no effect.  In this implementation,
// setbuf(<anything>, n), for n > 0, calls reserve(n) on the underlying
// string.
template <class C, class T, class A>
basic_streambuf<C, T>*
basic_stringbuf<C,T,A>::setbuf(C*, streamsize n)
{
  if (n > 0) {
    bool do_get_area = false;
    bool do_put_area = false;
    ptrdiff_t offg = 0;
    ptrdiff_t offp = 0;

    if (this->pbase() == this->start(str_)) {
      do_put_area = true;
      offp = this->pptr() - this->pbase();
    }

    if (this->eback() == this->start(str_)) {
      do_get_area = true;
      offg = this->gptr() - this->eback();
    }

    str_.reserve(sizeof(streamsize) > sizeof(size_t)
    			 ? size_t((min)(n, streamsize(str_.max_size())))
                 : size_t(n)
                 );

    C* data_ptr = this->start(str_);

    if (do_get_area) {
      this->setg(data_ptr, data_ptr + offg, this->finish(str_));
    }

    if (do_put_area) {
      this->setp(data_ptr, this->finish(str_));
      this->pbump((int)offp);
    }
  }

  return this;
}

template <class C, class T, class A>
streampos 
basic_stringbuf<C,T,A>::seekoff(off_type ofs, ios_base::seekdir dir, ios_base::openmode mode)
{
  mode &= mode_;

  bool imode  = (mode & ios_base::in) != 0;
  bool omode = (mode & ios_base::out) != 0;

  if ( !(imode || omode) )
    return pos_type(off_type(-1));

  if ( (imode && (this->gptr() == 0)) || (omode && (this->pptr() == 0)) )
    return pos_type(off_type(-1));

  streamoff newoff;
  switch(dir) {
    case ios_base::beg:
      newoff = 0;
      break;
    case ios_base::end:
      newoff = str_.size();
      break;
    case ios_base::cur:
      newoff = imode ? this->gptr() - this->eback() : this->pptr() - this->pbase();
      if ( ofs == 0 ) {
        return pos_type(newoff);
      }
      break;
    default:
      return pos_type(off_type(-1));
  }

  ofs += newoff;

  if (imode) {
    ptrdiff_t n = this->egptr() - this->eback();

    if (ofs < 0 || ofs > n)
      return pos_type(off_type(-1));
    this->setg(this->eback(), this->eback() + ptrdiff_t(ofs),
                              this->eback() + ptrdiff_t(n));
  }

  if (omode) {
    ptrdiff_t n = this->epptr() - this->pbase();

    if (ofs < 0 || ofs > n)
      return pos_type(off_type(-1));
    this->setp(this->pbase(), this->pbase() + n);
    this->pbump((int)ofs);
  }

  return pos_type(ofs);
}

template <class C, class T, class A>
streampos
basic_stringbuf<C,T,A>::seekpos(pos_type pos, ios_base::openmode mode)
{
  mode &= mode_;

  bool imode = (mode & ios_base::in) != 0;
  bool omode = (mode & ios_base::out) != 0;

  if ( !(imode || omode) )
    return pos_type(off_type(-1));

  if ( (imode && (this->gptr() == 0)) || (omode && (this->pptr() == 0)) )
    return pos_type(off_type(-1));

  const off_type n = pos - pos_type(off_type(0));

  if (imode) {
    if (n < 0 || n > this->egptr() - this->eback())
      return pos_type(off_type(-1));
    this->setg(this->eback(), this->eback() + ptrdiff_t(n), this->egptr());
  }

  if (omode) {
    if (n < 0 || size_t(n) > str_.size())
      return pos_type(off_type(-1));

    this->setp(this->start(str_), this->finish(str_));
    this->pbump((int)n);
  }

  return pos;
}

//----------------------------------------------------------------------
// Non-inline istringstream member functions.

template <class C, class T, class A>
basic_istringstream<C,T,A>::basic_istringstream(ios_base::openmode mode)
    : basic_istream<C, T>(0)
    , buf_(mode | ios_base::in)
{
  this->init(&buf_);
}

template <class C, class T, class A>
basic_istringstream<C,T,A>::basic_istringstream(string_type const& str,ios_base::openmode mode)
    : basic_istream<C, T>(0)
    , buf_(str, mode | ios_base::in)
{
  this->init(&buf_);
}

template <class C, class T, class A>
basic_istringstream<C,T,A>::~basic_istringstream()
{
}

//----------------------------------------------------------------------
// Non-inline ostringstream member functions.

template <class C, class T, class A>
basic_ostringstream<C,T,A>::basic_ostringstream(ios_base::openmode mode)
    : basic_ostream<C, T>(0)
    , buf_(mode | ios_base::out)
{
  this->init(&buf_);
}

template <class C, class T, class A>
basic_ostringstream<C,T,A>::basic_ostringstream(string_type const& str, ios_base::openmode mode)
    : basic_ostream<C, T>(0)
    , buf_(str, mode | ios_base::out)
{
  this->init(&buf_);
}

template <class C, class T, class A>
basic_ostringstream<C,T,A>::~basic_ostringstream()
{
}

//----------------------------------------------------------------------
// Non-inline stringstream member functions.

template <class C, class T, class A>
basic_stringstream<C,T,A>::basic_stringstream(ios_base::openmode mode)
  	: basic_iostream<C, T>(0)
  	, buf_(mode)
{
   this->init(&buf_);
}

template <class C, class T, class A>
basic_stringstream<C,T,A>::basic_stringstream(string_type const& str, ios_base::openmode mode)
    : basic_iostream<C, T>(0)
    , buf_(str, mode)
{
  this->init(&buf_);
}

template <class C, class T, class A>
basic_stringstream<C,T,A>::~basic_stringstream()
{
}


}	// std

#endif
#endif
