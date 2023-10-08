rem set CCOPTS=-DCCWRAP_ASSERT_NO_ABORT
rem call mk.bat unittest_assert %*
wcl386 -cc++ -xs -xr -bcl=nt -I..\..\..\watcom -I..\..\..\ccwrap ..\..\src\unittest_assert\assert_ex_test.c

assert_ex_test
