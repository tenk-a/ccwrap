#if __CCWRAP_CXX < 201103L

namespace std {
	using ::vsnprintf;
	using ::snprintf;
	using ::vsnwprintf;
	using ::snwprintf;
	using ::vfscanf;
	using ::vscanf;
 	using ::vsscanf;
}

#endif
