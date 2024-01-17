@echo off
setlocal
set SRCDIR=..\..\..\etc\_cxx_lang_test
set INCDIR=..\..\..\vc
set INCDIR2=..\..\..\ccwap
set CC=cl
set OPTS=-I%INCDIR% -I%INCDIR2% -I%SRCDIR% -FIccwrap_header.h -TP -EHac -GF -Zc:wchar_t -Zc:forScope
rem set LIBS=kernel32.lib

call %SRCDIR%\sub_cxx_lang_test.bat

endlocal
