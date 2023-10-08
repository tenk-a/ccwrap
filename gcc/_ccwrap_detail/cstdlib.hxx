#if __CCWRAP_CXX < 201103L

namespace std {
	using ::llabs;
	using ::atoll;
	using ::strtoll;
	using ::strtoull;
 #if 0
	using ::wtoll;
	using ::wcstoll;
	using ::wcstoull;
 #endif
	using ::strtof;
	using ::wcstof;
	using ::strtold;
	using ::wcstold;
}

#endif
