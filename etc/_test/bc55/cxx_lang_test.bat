@echo off
setlocal
set SRCDIR=..\..\_cxx_lang_test
set INCDIR=..\..\borland
set CC=bcc32
set OPTS=-I%INCDIR% -I%SRCDIR%
rem set LIBS=kernel32.lib

call %SRCDIR%\sub_cxx_lang_test.bat

endlocal
