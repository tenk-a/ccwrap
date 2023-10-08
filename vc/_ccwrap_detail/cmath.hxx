#if __CCWRAP_CXX < 201103L
#if _MSC_VER < 1800
namespace std {
	using ::isnan;
	using ::isinf;
	using ::fpclassify;
	using ::isfinite;
	using ::isnormal;
	using ::signbit;
	using ::isunordered;
	using ::isgreater;
	using ::isgreaterequal;
	using ::isless;
	using ::islessequal;

	typedef float	float_t;
	typedef double	double_t;
}
#endif
#endif
