rem set CCOPTS=-DCCWRAP_ASSERT_NO_ABORT
rem call mk.bat unittest_assert %*
cl -I..\..\..\ccwrap -DCCWRAP_ASSERT_NO_ABORT ..\..\src\unittest_assert\assert_ex_test.c
assert_ex_test
