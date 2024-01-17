@echo off
setlocal
set SRCDIR=..\..\..\etc\_cxx_lang_test
set INCDIR=..\..\..\gcc
set INCDIR2=..\..\..\ccwap
set CC=g++
set OPTS=-std=c++03 -I%INCDIR% -I%INCDIR2% -I%SRCDIR% --include ccwrap_header.h
rem set LIBS=kernel32.lib

call %SRCDIR%\sub_cxx_lang_test.bat

endlocal
