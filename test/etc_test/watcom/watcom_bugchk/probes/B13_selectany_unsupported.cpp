/* bugchk-id:     B13
 * bugchk-kind:   compile
 * bugchk-expect: bug
 * bugchk-desc:   __declspec(selectany) is not supported, so header constants stay per-TU
 *
 * A namespace-scope `const` object has internal linkage in C++, so a header that writes
 * `static const X name = X();` gives every translation unit its own copy. The standard
 * spells these tag constants `inline constexpr` (in_place, nullopt, allocator_arg,
 * piecewise_construct, the <chrono> calendar constants, the bind placeholders), i.e. one
 * object for the whole program.
 *
 * Compilers without C++17 inline variables can still get there: MSVC has
 * __declspec(selectany) (measured on vc90 / vc110 / vc2017 / vc2022) and gcc has
 * __attribute__((selectany)) on PE / __attribute__((weak)) on ELF. Open Watcom has
 * neither, and rejects the declaration outright:
 *
 *     probe.cpp(N): Error! E722: col(19) __declspec('selectany') is not supported
 *
 * Consequence: _ccw_inline_const (watcom/std/ccwrap_header.h) stays `static const` here,
 * and the duplication remains. See doc/c++03_tec.md section 11.
 *
 * Raw compiler only -- no ccwrap headers, no library.
 */
#include <stdio.h>

struct tag_t { int v; tag_t() : v(7) {} };

extern __declspec(selectany) const tag_t ccw_tag = tag_t();

int main()
{
    printf("%d\n", ccw_tag.v);
    return 0;
}
