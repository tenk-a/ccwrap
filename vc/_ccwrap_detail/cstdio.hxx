#if __CCWRAP_CXX < 201103L
#if _MSC_VER < 1900
namespace std {
	using ::vsnprintf;
	using ::snprintf;
	using ::vsnwprintf;
	using ::snwprintf;
 #if 0
	using ::vfscanf;
	using ::vscanf;
 	using ::vsscanf;
 #endif
}
#endif
#endif
