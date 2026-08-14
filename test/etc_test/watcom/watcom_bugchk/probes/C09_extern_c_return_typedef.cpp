/* bugchk-id:     C09
 * bugchk-kind:   compile
 * bugchk-expect: bug
 * bugchk-flags:  -DWIN32_LEAN_AND_MEAN
 * bugchk-desc:   C++ rejects an extern "C" redeclaration of GetProcAddress through an
 *                identical typedef of our own
 *
 * C9: redeclaring GetProcAddress with a return typedef that expands to exactly what
 * <windef.h> spells for FARPROC is rejected:
 *
 *     typedef int (WINAPI *FARPROC)();          // h/nt/windef.h, WINAPI == __stdcall
 *     typedef int (__stdcall* ccw_farproc)();   // ours: the same tokens
 *     -> E385: attempt to overload function 'GetProcAddress' with a different return type
 *     -> E617: cannot overload extern "C" functions
 *
 * It is the return type alone: writing FARPROC and changing the PARAMETERS instead
 * (HMODULE -> struct HINSTANCE__*, LPCSTR -> const char*) compiles fine. Two typedefs
 * of our own in one file are also accepted, so the probe needs the real <windows.h>.
 *
 * This is what stops ccwrap from declaring GetProcAddress at global scope: a program
 * that also includes <windows.h> then fails in EITHER include order. The cure is to put
 * our declaration in a namespace -- extern "C" keeps the symbol, and the comparison
 * never happens. detail/c/win/systemtime.h does that.
 *
 * C is not affected: it compares types for compatibility and accepts both.
 *
 * Raw compiler and the platform headers only, no ccwrap.
 */
#include <windows.h>

typedef int (__stdcall* ccw_farproc)();

extern "C" __declspec(dllimport) ccw_farproc __stdcall GetProcAddress(HMODULE, LPCSTR);

int main(void)
{
    return GetProcAddress(0, "x") ? 1 : 0;
}
