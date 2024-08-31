rem set CCOPTS=-DCCWRAP_ASSERT_NO_ABORT
rem call mk.bat unittest_assert %*
wcl386 -bcl=nt -I..\..\..\watcom -I..\..\..\ccwrap ..\..\src\unittest_assert\assert_ex_test.c
wcl386 -cc++ -xs -xr -bcl=nt -DCCWRAP_TEST_LOG_LITE -I..\..\..\watcom -I..\..\..\ccwrap ..\..\src\unittest_assert\test.cpp ..\..\src\unittest_assert\ccwrap_test_check.cpp

assert_ex_test
