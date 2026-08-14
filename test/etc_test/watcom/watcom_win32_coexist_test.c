/* Compiling this file IS the check: ccwrap's C headers must declare a Win32 API
   with the same types the Windows headers declare it with, so that a program using
   Win32 (wxWidgets, for one) can include both. Both include orders are covered --
   this file puts Win32 first, watcom_win32_coexist2_test.c puts ccwrap first. */
#include "watcom_win32_decls.h"

#include <time.h>
#include <threads.h>
#include <stdio.h>

int main(void)
{
    struct timespec ts;
    FILETIME ft;

    ft.dwLowDateTime = 0;
    ft.dwHighDateTime = 0;
    GetSystemTimeAsFileTime(&ft);
    if (ft.dwLowDateTime == 0 && ft.dwHighDateTime == 0) {
        printf("win32_coexist: GetSystemTimeAsFileTime gave 0\n");
        return 1;
    }

    ts.tv_sec = 0;
    ts.tv_nsec = -1;
    if (timespec_get(&ts, TIME_UTC) != TIME_UTC) {
        printf("win32_coexist: timespec_get failed\n");
        return 1;
    }
    if (ts.tv_sec <= 0 || ts.tv_nsec < 0 || ts.tv_nsec > 999999999L) {
        printf("win32_coexist: timespec_get gave %ld.%09ld\n",
               (long)ts.tv_sec, ts.tv_nsec);
        return 1;
    }

    {   /* the C11 threads layer over the same critical sections */
        mtx_t m;
        if (mtx_init(&m, mtx_plain) != thrd_success) {
            printf("win32_coexist: mtx_init failed\n");
            return 1;
        }
        if (mtx_lock(&m) != thrd_success || mtx_unlock(&m) != thrd_success) {
            printf("win32_coexist: mtx_lock/unlock failed\n");
            return 1;
        }
        mtx_destroy(&m);
    }

    printf("win32_coexist: ok\n");
    return 0;
}
