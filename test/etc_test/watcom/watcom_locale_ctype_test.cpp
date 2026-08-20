/* Open Watcom's native <ctype.h> imports std::is* with using-declarations.
   Those declarations must be established before <locale> adds its overloads;
   <windows.h> includes <ctype.h> late through <winnt.h>. */
#include <locale>
#include <windows.h>

#include <cstdio>

int main()
{
    std::locale loc;
    if (!::isdigit('0') || !std::isdigit('0') || !std::isdigit('0', loc)) {
        std::printf("locale_ctype: classification failed\n");
        return 1;
    }
    std::printf("locale_ctype: ok\n");
    return 0;
}
