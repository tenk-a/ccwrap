rem @echo off
setlocal
set TestName=%1
if "%TestName%"=="" goto ERR

shift
set CCWRAP_ROOT=../../..
set CCOPTS_REL=-O2 -DNDEBUG
set CCOPTS_DBG=-O0 -D_DEBUG
set CCWARN=%CCWARN% -Wall -Wno-misleading-indentation -Wno-unused-variable
set CCWARN=%CCWARN% -Wno-unused-local-typedef -Wno-unnamed-type-template-args
set CCWARN=%CCWARN% -Wno-local-type-template-args -Wno-unused-local-typedefs
set CCWARN=%CCWARN% -Wno-c++20-compat -Wno-sign-compare -Wno-unused-but-set-variable
set CCOPTS=%CCOPTS% -D_CONSOLE -DCCWRAP_ASSERT_NO_ABORT -DUSE_TEST
set LOPTS=
if "%TestName%"=="" set TestName=test
set SRCDIR=../../src/%TestName%
set StdDir=%CCWRAP_ROOT%/gcc
set INCS=-I%StdDir% -I%CCWRAP_ROOT%/ccwrap -I%SRCDIR%
set LIBS=
rem set LIBS=%LIBS% -lkernel32 -luser32 -lshell32 -lshlwapi
set SRCS=%SRCDIR%/*.cpp
set FIRSTINC=-include %CCWRAP_ROOT%/gcc/ccwrap_header.h

rem
rem
set Conf=debug
set stdcxx=-std=c++03
set BoostDir=
set IsClean=
set CcxxName=clang

if /I "%TestName%"=="cleanAll" goto CLEAN_ALL
if /I "%TestName%"=="clean" goto ERR

:ARG_LOOP
  set A=%1

  if "%1"=="" goto ARG_LOOP_EXIT

  if /I "%1"=="debug"     set Conf=debug
  if /I "%1"=="release"   set Conf=release

  if /I "%1"=="cxx11"     set stdcxx=-std=c++11
  if /I "%1"=="cxx14"     set stdcxx=-std=c++14
  if /I "%1"=="cxx17"     set stdcxx=-std=c++17
  if /I "%1"=="cxx20"     set stdcxx=-std=c++20
  rem if /I "%1"=="utf8"  set utf8=

  if /I "%1"=="gcc"       set CcxxName=gcc
  if /I "%1"=="g++"       set CcxxName=gcc

  if /I "%1"=="clean"     set IsClean=clean
  if /I "%1"=="cleanAll"  set IsClean=cleanAll
  if /I "%1"=="allClean"  set IsClean=cleanAll

  if /I "%A:~0,6%"=="boost:" set BoostDir=%A:~6%

  shift
goto ARG_LOOP
:ARG_LOOP_EXIT


set CCXX=%CcxxName%++
if %CCXX%==gcc++ set CCXX=g++

set OBJDIR=
set ABXOPTS=
set Conf=
if /I "%Conf%"=="release" (
  set Conf=release
  set CCOPTS=%CCOPTS% %CCOPTS_REL%
  set LOPTS=%LOPTS% %CCOPTS_REL%
  set ABXOPTS=%ABXOPTS% -xm
) else (
  set Conf=debug
  set CCOPTS=%CCOPTS% %CCOPTS_DBG%
  set LOPTS=%LOPTS% %CCOPTS_DBG%
  set ABXOPTS=%ABXOPTS%
)
set EXENAME=%TestName%_%CcxxName%_%Conf%.exe
set OBJDIR=obj\%TestName%\%CcxxName%_%Conf%

if /I "%IsClean%"=="clean"    goto CLEAN
if /I "%IsClean%"=="cleanAll" goto CLEAN_ALL

if not exist %OBJDIR% mkdir %OBJDIR%

set LAST_OPTS=
set BOOST2STD=
if /I "%BoostDir%"=="" goto BOOST_SKIP
  set BOOST2STD=%CCWRAP_ROOT%/boost2std
  rem set BoostLibDir=%BoostDir%\stage\%CcxxName%_%Arch%\lib
  set BoostLibDir=%BoostDir%/stage/gcc_x64/lib
  set LAST_OPTS=%LAST_OPTS% -link /LIBPATH:%BoostLibDir%
  set INCS=-I%BOOST2STD% -I%BoostDir%
  set FIRSTINC=-include %BOOST2STD%/boost2std.hpp
:BOOST_SKIP

set CCOPTS=%CCOPTS% %CCWARN% -I. %INCS% %stdcxx%

abx %ABXOPTS% -s -x -cu%OBJDIR%\@x.o %SRCS% =%CCXX% -c %FIRSTINC% %CCOPTS% -o %OBJDIR%\@x.o @Rbf
set OBJS=
echo rem >%OBJDIR%\set_objs.bat
abx -s -k$ %OBJDIR%\*.o "=@set OBJS=%%OBJS%% %OBJDIR%\$c" >>%OBJDIR%\set_objs.bat
call %OBJDIR%\set_objs.bat
if exist %EXENAME% del %EXENAME%
%CCXX% -o%EXENAME% %LOPTS% %OBJS% %LIBS% %LAST_OPTS%
rem @del %OBJDIR%\set_objs.bat
goto END


:CLEAN
del %EXENAME%
pushd %OBJDIR%
del *.o
popd
goto END

:CLEAN_ALL
del /s *.o
rmdir /q /s obj
goto END

:ERR
echo mk.bat TEST-NAME [OPTS]
echo TEST-NAME: test, unittest_assert, boost2std_test
echo OPTS:
echo   clean     erase current exe and objs
echo   cleanAll  erase all exe and objs
echo   debug     debug build (*default)
echo   release   release build
echo   cxx17     add -std:c++17 option
rem echo   utf8      add -utf-8 option

:END
endlocal
