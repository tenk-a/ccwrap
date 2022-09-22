@echo off
setlocal

set SRCDIR=..\..\src\smp
set INCDIR=..\..\..\clang_cxx03
set CCOPTS= -I%INCDIR% -Wno-unused-local-typedefs  -include ccwrap_header.h


if exist smp.exe del smp.exe
if exist smp_c.exe del smp_c.exe

@echo [c++ core]
g++ -o smp.exe -std=c++03 -Wall %CCOPTS% -I%SRCDIR% %SRCDIR%\smp.cpp
@echo [c core]
rem g   -o smp_c.exe -std=c90 -Wall %CCOPTS% -I%SRCDIR% %SRCDIR%\smp_c.c
gcc -o smp_c.exe -std=c99 -Wall %CCOPTS% -I%SRCDIR% %SRCDIR%\smp_c.c

@echo -
@echo === [Compiling for error] Begin ===============================================
g++ -o smp.exe -std=c++11 -Wall %CCOPTS% -I%SRCDIR% -DCOMPILING_FOR_ERROR %SRCDIR%\smp.cpp
@echo === [Compiling for error] End =================================================

if exist smp.exe smp.exe
if exist smp_c.exe smp_c.exe
del *.obj

endlocal
