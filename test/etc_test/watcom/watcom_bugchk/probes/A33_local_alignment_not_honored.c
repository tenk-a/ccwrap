/* bugchk-id:     A33
 * bugchk-kind:   run
 * bugchk-expect: bug
 * bugchk-desc:   8-byte scalars get 4-byte-aligned automatic storage though alignof says 8
 *
 * Open Watcom reports an alignment of 8 for double / long double (via the classic
 * struct-offset trick, which is what ccwrap's <stdalign.h> `alignof` uses), but places
 * automatic objects of those types on 4-byte boundaries: the 32-bit stack frame is only
 * 4-byte aligned and there is no alignment specifier to ask for more (_Alignas /
 * __declspec(align) / __alignof are all unsupported).
 *
 * Consequence: max_align_t is nameable and reports the right alignment, but a *local*
 * max_align_t object is not guaranteed to satisfy it. Anything that assumes the
 * alignment of a local (SIMD, atomics, hand-rolled allocators) is on thin ice.
 * Heap storage is fine -- malloc returns suitably aligned memory.
 *
 * Raw compiler only: the alignment is computed here rather than taken from a ccwrap
 * header, so this measures the compiler, not the wrapper.
 */
#include <stdio.h>
#include <stddef.h>

#define ALIGNOF(type)  ((size_t)&(((struct { char __c; type __x; }*)0)->__x))

int main(void)
{
    double      d;
    long double ld;
    size_t ad = ALIGNOF(double), al = ALIGNOF(long double);
    size_t od = (size_t)(void*)&d, ol = (size_t)(void*)&ld;
    int bad = 0;

    printf("alignof: double=%u long double=%u\n", (unsigned)ad, (unsigned)al);
    printf("locals mod alignment: double=%u long double=%u\n",
           (unsigned)(od % ad), (unsigned)(ol % al));

    if (od % ad != 0) { puts("BROKEN: a local double is not aligned to alignof(double)"); bad = 1; }
    if (ol % al != 0) { puts("BROKEN: a local long double is not aligned to alignof(long double)"); bad = 1; }
    if (!bad) puts("ok: automatic storage honours the reported alignment");
    return bad;
}
