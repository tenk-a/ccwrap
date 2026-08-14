// ccwrap watcom: <complex.h>.
//
// C++ ([complex.h.syn]): this header simply behaves as if it included <complex>. That
// costs nothing here, so C++ gets it -- ccwrap supplies <complex> via libcxx03.
#pragma once

#ifdef __cplusplus
#  include <complex>
#else
#  error "ccwrap: <complex.h> is not implemented for Open Watcom yet (library-level subset planned). Note the native <complex.h> is a facade: the compiler has no _Complex type (E1060) and the runtime has no complex functions. For C++, use <complex> (std::complex)."
#endif
