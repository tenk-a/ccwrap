@echo off
setlocal
set SRCDIR=..\..\_cxx_lang_test
set INCDIR=..\..\ow
set CC=wcl386
set OPTS=-I%INCDIR% -I%SRCDIR% -fi=%INCDIR%\ccwrap_header.h -fx
rem set LIBS=kernel32.lib

call %SRCDIR%\sub_cxx_lang_test.bat

endlocal
