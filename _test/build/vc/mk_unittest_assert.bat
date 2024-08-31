rem set CCOPTS=-DCCWRAP_ASSERT_NO_ABORT
rem call mk.bat unittest_assert %*
cl -I..\..\..\ccwrap -DCCWRAP_ASSERT_NO_ABORT ..\..\src\unittest_assert\assert_ex_test.c
assert_ex_test
cl -I..\..\..\ccwrap -DCCWRAP_ASSERT_NO_ABORT -Feccwrap_test_check.exe ..\..\src\unittest_assert\test.cpp ..\..\src\unittest_assert\ccwrap_test_check.cpp
