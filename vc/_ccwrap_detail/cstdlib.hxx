#if __CCWRAP_CXX < 201103L
#if _MSC_VER < 1800
namespace std {
 #if _MSC_VER < 1600
	using ::llabs;
 #endif
	using ::atoll;
	using ::strtoll;
	using ::strtoull;
	using ::wtoll;
	using ::wcstoll;
	using ::wcstoull;

	using ::strtof;
	using ::wcstof;
	using ::strtold;
	using ::wcstold;
}
#endif
#endif
