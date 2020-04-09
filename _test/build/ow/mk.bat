@echo off
setlocal
set SRCDIR=..\..\src
set INCDIR=..\..\..\..\ccwrap\ow
set CXXDIR=..\..\..\..\ccwrap\cxx

if exist smp.exe   del smp.exe
if exist smp_c.exe del smp_c.exe
rem if exist smp_cxx.exe del smp_cxx.exe

wcl386.exe -cc++ -xs -xr -bcl=nt -DCOMPILING_FOR_ERROR -I%INCDIR% -I%SRCDIR% %SRCDIR%\smp.cpp
wcl386.exe -cc++ -xs -xr -bcl=nt -I%INCDIR% -I%SRCDIR% %SRCDIR%\smp.cpp
wcl386.exe -cc           -bcl=nt -I%INCDIR% -I%SRCDIR% %SRCDIR%\smp_c.c
rem wcl386.exe -cc++ -xs -xr -bcl=nt -I%INCDIR% -I%CXXDIR% -I%SRCDIR% %SRCDIR%\smp_cxx.cpp

if exist smp.exe   smp.exe
if exist smp_c.exe smp_c.exe
rem if exist smp_cxx.exe smp_cxx.exe

endlocal
