/* The reverse include order of watcom_win32_coexist_test.c: ccwrap first, then the
   Win32 declarations. Watcom reports the clash against whichever declaration comes
   second, so only running both orders proves the two agree. */
#include <time.h>
#include <threads.h>

#include "watcom_win32_decls.h"

#include <stdio.h>

int main(void)
{
    FILETIME ft;
    mtx_t    m;
    struct timespec ts;

    ft.dwLowDateTime = 0;
    ft.dwHighDateTime = 0;
    GetSystemTimeAsFileTime(&ft);
    if (ft.dwLowDateTime == 0 && ft.dwHighDateTime == 0) {
        printf("win32_coexist2: GetSystemTimeAsFileTime gave 0\n");
        return 1;
    }

    ts.tv_sec = 0;
    ts.tv_nsec = -1;
    if (timespec_get(&ts, TIME_UTC) != TIME_UTC || ts.tv_nsec < 0) {
        printf("win32_coexist2: timespec_get failed\n");
        return 1;
    }

    if (mtx_init(&m, mtx_plain) != thrd_success) {
        printf("win32_coexist2: mtx_init failed\n");
        return 1;
    }
    mtx_destroy(&m);

    printf("win32_coexist2: ok\n");
    return 0;
}
