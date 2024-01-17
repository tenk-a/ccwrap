@echo off
setlocal
set SRCDIR=..\..\_cxx_lang_test
set INCDIR=..\..\dmc
set CC=dmc
set OPTS=-I%INCDIR% -I%SRCDIR% -HIccwrap_header.h
rem set LIBS=kernel32.lib

call %SRCDIR%\sub_cxx_lang_test.bat

endlocal
