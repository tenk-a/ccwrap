@echo off
setlocal

set SRCDIR=..\..\src\smp
set INCDIR=..\..\..\gcc
set INCDIR2=..\..\..\ccwrap
set CCOPTS= -I%INCDIR% -I%INCDIR2% -include ccwrap_header.h

set WRN_OPTXX=-Wall -Wno-comment -Wno-unused-local-typedefs -Wno-gnu-folding-constant
set WRN_OPT=-Wall -Wno-comment -Wno-unused-local-typedefs -Wno-gnu-folding-constant

if /I "%1"=="gcc" goto GCC
set CC=clang
set CXXC=clang++
goto SKIP_GCC
:GCC
set CC=clang
set CXXC=clang++
:SKIP_GCC

if exist *.exe del *.exe

@echo [c90 core]
%CC%   -o smp_c90.exe -std=gnu90 %WRN_OPT% %CCOPTS% -I%SRCDIR% %SRCDIR%\smp_c.c

@echo [c99 core]
%CC%   -o smp_c99.exe -std=c99 %WRN_OPT% %CCOPTS% -I%SRCDIR% %SRCDIR%\smp_c.c

@echo [c11 core]
%CC%   -o smp_c11.exe -std=c11 %WRN_OPT% %CCOPTS% -I%SRCDIR% %SRCDIR%\smp_c.c

@echo [c17 core]
%CC%   -o smp_c17.exe -std=c17 %WRN_OPT% %CCOPTS% -I%SRCDIR% %SRCDIR%\smp_c.c

@echo [c++03 core]
%CXXC% -o smp03.exe -std=c++03 %WRN_OPT% %CCOPTS% -I%SRCDIR% %SRCDIR%\smp.cpp

@echo [c++11 core]
%CXXC% -o smp11.exe -std=c++11 %WRN_OPT% %CCOPTS% -I%SRCDIR% %SRCDIR%\smp.cpp

@echo [c++14 core]
%CXXC% -o smp14.exe -std=c++14 %WRN_OPT% %CCOPTS% -I%SRCDIR% %SRCDIR%\smp.cpp

@echo [c++17 core]
%CXXC% -o smp17.exe -std=c++17 %WRN_OPT% %CCOPTS% -I%SRCDIR% %SRCDIR%\smp.cpp

@echo [c++20 core]
%CXXC% -o smp20.exe -std=c++20 %WRN_OPT% %CCOPTS% -I%SRCDIR% %SRCDIR%\smp.cpp

@echo -
set CCOPTS=%CCOPTS% -DCOMPILING_FOR_ERROR
@echo === [11 Compiling for error] Begin ===============================================
@echo [c90 core]
%CC%   -o smp_c90.exe -std=gnu90 %WRN_OPT% %CCOPTS% -I%SRCDIR% %SRCDIR%\smp_c.c

@echo [c99 core]
%CC%   -o smp_c99.exe -std=c99 %WRN_OPT% %CCOPTS% -I%SRCDIR% %SRCDIR%\smp_c.c

@echo [c11 core]
%CC%   -o smp_c11.exe -std=c11 %WRN_OPT% %CCOPTS% -I%SRCDIR% %SRCDIR%\smp_c.c

@echo [c17 core]
%CC%   -o smp_c17.exe -std=c17 %WRN_OPT% %CCOPTS% -I%SRCDIR% %SRCDIR%\smp_c.c

@echo [c++03 core]
%CXXC% -o smp03.exe -std=c++03 %WRN_OPT% %CCOPTS% -I%SRCDIR% %SRCDIR%\smp.cpp

@echo [c++11 core]
%CXXC% -o smp11.exe -std=c++11 %WRN_OPT% %CCOPTS% -I%SRCDIR% %SRCDIR%\smp.cpp

@echo [c++14 core]
%CXXC% -o smp14.exe -std=c++14 %WRN_OPT% %CCOPTS% -I%SRCDIR% %SRCDIR%\smp.cpp

@echo [c++17 core]
%CXXC% -o smp17.exe -std=c++17 %WRN_OPT% %CCOPTS% -I%SRCDIR% %SRCDIR%\smp.cpp

@echo [c++20 core]
%CXXC% -o smp20.exe -std=c++20 %WRN_OPT% %CCOPTS% -I%SRCDIR% %SRCDIR%\smp.cpp
@echo === [Compiling for error] End =================================================

if exist *obj        del *.obj

if exist smp_c90.exe smp_c90.exe
if exist smp_c99.exe smp_c99.exe
if exist smp_c11.exe smp_c11.exe
if exist smp_c17.exe smp_c17.exe

if exist smp03.exe   smp03.exe
if exist smp11.exe   smp11.exe
if exist smp14.exe   smp14.exe
if exist smp17.exe   smp17.exe
if exist smp20.exe   smp20.exe

endlocal
