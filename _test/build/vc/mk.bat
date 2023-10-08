rem @echo off
setlocal
set TestName=%1
shift

if /i "%1"=="NOFIRSTINC" (
  set NOFIRSTINC=NOFIRSTINC
  shift
)

set CCXX=cl
set CCOPTS_REL=-O2 -Oy -GS- -MT -DNDEBUG
set CCOPTS_DBG=-Od -Ob0 -Zi -MTd -D_DEBUG
set CCWARN=%CCWARN% -W4 -wd4067 -wd4996 -wd4828
rem set CCWARN=%CCWARN% -wd4067 -wd4996 -wd4828
set CCOPTS=%CCOPTS% -TP -EHac -GF -Zc:wchar_t -Zc:forScope -D_CONSOLE -DCCWRAP_ASSERT_NO_ABORT -DUSE_CCWRAP_TEST
set FIRSTINC=-FIccwrap_header.h
set LOPTS=%LOPTS% -EHac -GF
if "%TestName%"=="" set TestName=test
set SRCDIR=..\..\src\%TestName%
set INCS=%INCS% -I..\..\.. -I..\.. -I..\..\..\vc -I..\..\..\ccwrap -I%SRCDIR%
set LIBS=%LIBS% kernel32.lib user32.lib shell32.lib shlwapi.lib
set SRCS=%SRCDIR%\*.cpp

rem
rem
set Conf=debug
set cxx=
set BoostDir=
set IsClean=

if /I "%TestName%"=="cleanAll" goto CLEAN_ALL
if /I "%TestName%"=="clean" goto ERR

:ARG_LOOP
  set A=%1

  if "%1"=="" goto ARG_LOOP_EXIT

  if /I "%1"=="debug"     set Conf=debug
  if /I "%1"=="release"   set Conf=release

  if /I "%1"=="cxx14"     set cxx=-std:c++14
  if /I "%1"=="cxx17"     set cxx=-std:c++17
  if /I "%1"=="utf8"      set utf8=-utf-8

  if /I "%1"=="clean"     set IsClean=clean
  if /I "%1"=="cleanAll"  set IsClean=cleanAll
  if /I "%1"=="allClean"  set IsClean=cleanAll

  if /I "%A:~0,6%"=="boost:" set BoostDir=%A:~6%

  shift
goto ARG_LOOP
:ARG_LOOP_EXIT


set VcVer=
@if /I not "%PATH:Microsoft Visual Studio .NET 2003=%"=="%PATH%" set VcVer=vc71
@if /I not "%PATH:Microsoft Visual Studio 8=%"=="%PATH%"    set VcVer=vc80
@if /I not "%PATH:Microsoft Visual Studio 9.0=%"=="%PATH%"  set VcVer=vc90
@if /I not "%PATH:Microsoft Visual Studio 10.0=%"=="%PATH%" set VcVer=vc100
@if /I not "%PATH:Microsoft Visual Studio 11.0=%"=="%PATH%" set VcVer=vc110
@if /I not "%PATH:Microsoft Visual Studio 12.0=%"=="%PATH%" set VcVer=vc120
@if /I not "%PATH:Microsoft Visual Studio 13.0=%"=="%PATH%" set VcVer=vc130
@if /I not "%PATH:Microsoft Visual Studio 14.0=%"=="%PATH%" set VcVer=vc140
@if /I not "%PATH:Microsoft Visual Studio\2017=%"=="%PATH%" set VcVer=vc141
@if /I not "%PATH:Microsoft Visual Studio\2019=%"=="%PATH%" set VcVer=vc142
@if /I not "%PATH:Microsoft Visual Studio\2022=%"=="%PATH%" set VcVer=vc143
if "%VcVer%"=="" goto ERR
set Arch=Win32
@if "%VcVer%"=="vc143"    if /I not "%PATH:\bin\HostX64\x64=%"=="%PATH%" set Arch=x64
@if "%VcVer%"=="vc142"    if /I not "%PATH:\bin\HostX64\x64=%"=="%PATH%" set Arch=x64
@if "%VcVer%"=="vc141"    if /I not "%PATH:\bin\HostX64\x64=%"=="%PATH%" set Arch=x64
@if /I not "%PATH:Microsoft Visual Studio 14.0\VC\BIN\amd64=%"=="%PATH%" set Arch=x64
@if /I not "%PATH:Microsoft Visual Studio 13.0\VC\BIN\amd64=%"=="%PATH%" set Arch=x64
@if /I not "%PATH:Microsoft Visual Studio 12.0\VC\BIN\amd64=%"=="%PATH%" set Arch=x64
@if /I not "%PATH:Microsoft Visual Studio 11.0\VC\BIN\amd64=%"=="%PATH%" set Arch=x64
@if /I not "%PATH:Microsoft Visual Studio 10.0\VC\BIN\amd64=%"=="%PATH%" set Arch=x64
@if /I not "%PATH:Microsoft Visual Studio 9.0\VC\BIN\amd64=%"=="%PATH%"  set Arch=x64
@if /I not "%PATH:Microsoft Visual Studio 8\VC\BIN\amd64=%"=="%PATH%"    set Arch=x64
rem echo %VcVer% %Arch%

set OBJDIR=
set ABXOPTS=
set Conf=
if /I "%Conf%"=="release" (
  set Conf=release
  set CCOPTS=%CCOPTS% %CCOPTS_REL%
  set LOPTS=%LOPTS% %CCOPTS_REL%
  set ABXOPTS=%ABXOPTS% -xm
  if exist test.pdb del test.pdb
  if exist test.ilk del test.ilk
) else (
  set Conf=debug
  set CCOPTS=%CCOPTS% %CCOPTS_DBG%
  set LOPTS=%LOPTS% %CCOPTS_DBG%
  set ABXOPTS=%ABXOPTS%
)
set EXENAME=%TestName%_%VcVer%_%Arch%_%Conf%.exe
set OBJDIR=obj\%TestName%\%VcVer%_%Arch%_%Conf%

if /I not "%Conf%"=="release" set CCOPTS=%CCOPTS% -Fd%OBJDIR%\%TestName%.pdb

rem if not "%utf8%"=="" set CCOPTS=%CCOPTS% %utf8% -DCCWRAP_CHAR_IS_UTF8
if not "%utf8%"=="" set CCOPTS=%CCOPTS% %utf8%
if     "%utf8%"=="" set CCOPTS=%CCOPTS% -DUSE_SJIS

set Vc2015orLater= 
if "%VcVer%"=="vc140" set Vc2015orLater=1
if "%VcVer%"=="vc141" set Vc2015orLater=1
if "%VcVer%"=="vc142" set Vc2015orLater=1
if "%VcVer%"=="vc143" set Vc2015orLater=1

if "%VcVer%"=="vc142" set BoostDir=
if "%VcVer%"=="vc143" set BoostDir=

set StdDir=..\..\..\vc

if "%VcVer%"=="vc140" set CCOPTS=%CCOPTS% %utf8%
if "%VcVer%"=="vc141" set CCOPTS=%CCOPTS% %cxx% %utf8%
if "%VcVer%"=="vc142" set CCOPTS=%CCOPTS% %cxx% %utf8%
if "%VcVer%"=="vc143" set CCOPTS=%CCOPTS% %cxx% %utf8%

set LAST_OPTS=
set BOOST2STD=
if /I "%BoostDir%"=="" goto BOOST_SKIP
  set BOOST2STD=..\..\..\boost2std
  set StdDir=%BOOST2STD%\detail\vc
  set BoostLibDir=%BoostDir%\stage\%VcVer%_%Arch%\lib
  set LAST_OPTS=%LAST_OPTS% -link /LIBPATH:%BoostLibDir%
  set INCS=-I%BOOST2STD% -I%BoostDir% -I..\..\.. -I..\..
  set FIRSTINC=-FIboost2std.hpp
  set EXENAME=%TestName%_%VcVer%_%Arch%_%Conf%_b2s.exe
  goto BOOST_SKIP_2
:BOOST_SKIP
  set INCS=-I%StdDir% %INCS%
:BOOST_SKIP_2

if not "%NOFIRSTINC%"=="" set FIRSTINC=

set CCOPTS=%CCOPTS% %CCWARN% %FIRSTINC% -I. %INCS%

if /I "%IsClean%"=="clean"    goto CLEAN
if /I "%IsClean%"=="cleanAll" goto CLEAN_ALL
if not exist %OBJDIR% mkdir %OBJDIR%

abx %ABXOPTS% -s -x -cu%OBJDIR%\@x.obj %SRCS% =%CCXX% -c %CCOPTS% -Fo%OBJDIR%\@x.obj @Rbf
set OBJS=
echo rem >%OBJDIR%\set_objs.bat
abx -s -k$ %OBJDIR%\*.obj "=@set OBJS=%%OBJS%% %OBJDIR%\$c" >>%OBJDIR%\set_objs.bat
call %OBJDIR%\set_objs.bat
if exist %EXENAME% del %EXENAME%
%CCXX% -Fe%EXENAME% %LOPTS% %OBJS% %LIBS% %LAST_OPTS%
rem @del %OBJDIR%\set_objs.bat
goto END


:CLEAN
del %EXENAME%
pushd %OBJDIR%
del *.obj *.pdb *.ilk *.bak *.tlog
popd
goto END

:CLEAN_ALL
del /s *.obj *.exe *.pdb *.ilk *.bak *.tlog
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
echo   cxx17     add -std:c++17 option for vc2017 or later
rem echo   utf8      add -utf-8 option for vc2015 or later

:END
endlocal
