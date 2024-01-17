rem @echo off
rem This batch-file license: boost software license version 1.0
rem Please rewrite PATH according to your environment.

set CcName=%1
set CcArch=%2

set INCLUDE=
set LIB=

if /i "%ProgramFiles(x86)%"=="" set "ProgramFiles(x86)=%ProgramFiles%"

if "%setcc_save_path%"=="" set "setcc_save_path=%path%"

set "setcc_base_path=%setcc_save_path%"
if not "%CcNasmDir%"==""  set "setcc_base_path=%CcNasmDir%;%setcc_base_path%"
if not "%CcCMakeDir%"=="" set "setcc_base_path=%CcCMakeDir%;%setcc_base_path%"
if not "%CcPerlDir%"==""  set "setcc_base_path=%CcPerlDir%;%setcc_base_path%"

set VCVARS_ARGS=
set VCPKG_DEFAULT_TRIPLET=x64-windows-static-md

set COMPILER=

if "%CcArch%"==""    set CcArch=x64
if "%CcArch%"=="64"  set CcArch=x64
if "%CcArch%"=="32"  set CcArch=Win32
if "%CcArch%"=="x86" set CcArch=Win32

rem
if /i "%CcName%"=="vc143"      goto L_VC143
if /i "%CcName%"=="vc142"      goto L_VC142
if /i "%CcName%"=="vc141"      goto L_VC141
if /i "%CcName%"=="vc140"      goto L_VC14
if /i "%CcName%"=="vc120"      goto L_VC12
if /i "%CcName%"=="vc110"      goto L_VC11
if /i "%CcName%"=="vc100"      goto L_VC10
if /i "%CcName%"=="vc90"       goto L_VC9
if /i "%CcName%"=="vc80"       goto L_VC8
if /i "%CcName%"=="vc71"       goto L_VC71
if /i "%CcName%"=="vc70"       goto L_VC70
if /i "%CcName%"=="vc60"       goto L_VC6

if /i "%CcName%"=="vc14.3"     goto L_VC143
if /i "%CcName%"=="vc14.2"     goto L_VC142
if /i "%CcName%"=="vc14.1"     goto L_VC141
if /i "%CcName%"=="vc14"       goto L_VC14
if /i "%CcName%"=="vc13"       goto L_VC13
if /i "%CcName%"=="vc12"       goto L_VC12
if /i "%CcName%"=="vc11"       goto L_VC11
if /i "%CcName%"=="vc10"       goto L_VC10
if /i "%CcName%"=="vc9"        goto L_VC9
if /i "%CcName%"=="vc8"        goto L_VC8
if /i "%CcName%"=="vc7.1"      goto L_VC71
if /i "%CcName%"=="vc7"        goto L_VC70
if /i "%CcName%"=="vc6"        goto L_VC6

if /i "%CcName%"=="msys"       goto L_MSYS2
if /i "%CcName%"=="msys2"      goto L_MSYS2
if /i "%CcName%"=="mingw"      goto L_MSYS2
if /i "%CcName%"=="gcc"        goto L_MSYS2
if /i "%CcName%"=="clang"      goto L_MSYS2_CLANG
if /i "%CcName%"=="watcom"     goto L_WATCOM
if /i "%CcName%"=="ow"         goto L_WATCOM
if /i "%CcName%"=="dmc"        goto L_DMC
if /i "%CcName%"=="orangec"    goto L_ORANGEC
if /i "%CcName%"=="occ"        goto L_ORANGEC
if /i "%CcName%"=="bcc32"      goto L_BCC55
if /i "%CcName%"=="bcc55"      goto L_BCC55
if /i "%CcName%"=="bc55"       goto L_BCC55

rem
if /i "%CcName%"=="lsic86"     goto L_LSIC86
if /i "%CcName%"=="tinyc"      goto L_TINYC
if /i "%CcName%"=="tcc"        goto L_TINYC
if /i "%CcName%"=="pcc"        goto L_PCC
if /i "%CcName%"=="pellesc"    goto L_PELLESC
if /i "%CcName%"=="lcc"        goto L_LCC
if /i "%CcName%"=="lccwin"     goto L_LCCWIN
if /i "%CcName%"=="coins"      goto L_COINS

rem
if /i "%CcName%"=="rust"       goto L_RUST
if /i "%CcName%"=="dmd1"       goto L_DMD1
if /i "%CcName%"=="dmd2"       goto L_DMD2
if /i "%CcName%"=="ldc2"       goto L_LDC2
if /i "%CcName%"=="go"         goto L_GO
if /i "%CcName%"=="java"       goto L_JAVA

@echo setcc [COMPILER] [x86/x64]
@echo   COMPILER:
@echo       vc142,vc141,vc140,vc120,vc110,vc100,vc90,vc80
@echo       msys2,gcc,clang,dmc,watcom,oragnec
@goto L_END


rem ## vc ######################################

:L_VC143
    if /I "%CcArch%"=="x64" goto L_VC143x64
    set COMPILER=VC143
    rem set VCVARS_BAT=vcvars32.bat
    set "VCVARS_BAT=vcvarsall.bat"
    set VCVARS_ARGS=x86
    set VCYEAR=2022
    goto L_VC143_SKIP_1

:L_VC143x64
    set COMPILER=VC143x64
    rem set VCVARS_BAT=vcvars64.bat
    set "VCVARS_BAT=vcvarsall.bat"
    rem set VCVARS_ARGS=x86_amd64 10.0.22621.0
    set VCVARS_ARGS=amd64
    set VCYEAR=2022
    goto L_VC143_SKIP_1

:L_VC143_SKIP_1
    set "PATH=%setcc_base_path%"
    if exist "%ProgramFiles%\Microsoft Visual Studio\%VCYEAR%\Enterprise\Common7\Tools\VsMSBuildCmd.bat"   goto VC141_Enterprise
    if exist "%ProgramFiles%\Microsoft Visual Studio\%VCYEAR%\Professional\Common7\Tools\VsMSBuildCmd.bat" goto VC141_Professional
    if exist "%ProgramFiles%\Microsoft Visual Studio\%VCYEAR%\Community\Common7\Tools\VsMSBuildCmd.bat"    goto VC141_Community
    echo ERROR: Not found "Microsoft Visual Studio %VCYEAR%"
    goto L_END

:L_VC142
    if /I "%CcArch%"=="x64" goto L_VC142x64
    set COMPILER=vc142
    set VCVARS_BAT=vcvars32.bat
    set VCYEAR=2019
    goto L_VC141_SKIP_1

:L_VC142x64
    set COMPILER=vc142x64
    set VCVARS_BAT=vcvars64.bat
    set VCYEAR=2019
    goto L_VC141_SKIP_1

:L_VC141
    if /I "%CcArch%"=="x64" goto L_VC141x64
    set COMPILER=vc141
    set VCVARS_BAT=vcvars32.bat
    set VCYEAR=2017
    goto L_VC141_SKIP_1

:L_VC141x64
    set COMPILER=vc141x64
    set VCVARS_BAT=vcvars64.bat
    set VCYEAR=2017
    goto L_VC141_SKIP_1

:L_VC141_SKIP_1
    set "PATH=%setcc_base_path%"
    @if exist "%ProgramFiles%\Microsoft Visual Studio\%VCYEAR%\Community\Common7\Tools\VsMSBuildCmd.bat"    goto VC141_Community
    @if exist "%ProgramFiles%\Microsoft Visual Studio\%VCYEAR%\Professional\Common7\Tools\VsMSBuildCmd.bat" goto VC141_Professional
    @if exist "%ProgramFiles%\Microsoft Visual Studio\%VCYEAR%\Enterprise\Common7\Tools\VsMSBuildCmd.bat"   goto VC141_Enterprise
    @if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\%VCYEAR%\Community\Common7\Tools\VsMSBuildCmd.bat"    goto VC141_Community64
    @if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\%VCYEAR%\Professional\Common7\Tools\VsMSBuildCmd.bat" goto VC141_Professional64
    @if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\%VCYEAR%\Enterprise\Common7\Tools\VsMSBuildCmd.bat"   goto VC141_Enterprise64
    echo ERROR: Not found "Microsoft Visual Studio %VCYEAR%"
    goto L_END
:VC141_Enterprise64
    call "%ProgramFiles(x86)%\Microsoft Visual Studio\%VCYEAR%\Enterprise\VC\Auxiliary\Build\%VCVARS_BAT%" %VCVARS_ARGS%
    goto L_END
:VC141_Professional64
    call "%ProgramFiles(x86)%\Microsoft Visual Studio\%VCYEAR%\Professional\VC\Auxiliary\Build\%VCVARS_BAT%" %VCVARS_ARGS%
    goto L_END
:VC141_Community64
    call "%ProgramFiles(x86)%\Microsoft Visual Studio\%VCYEAR%\Community\VC\Auxiliary\Build\%VCVARS_BAT%" %VCVARS_ARGS%
    goto L_END
:VC141_Enterprise
    call "%ProgramFiles%\Microsoft Visual Studio\%VCYEAR%\Enterprise\VC\Auxiliary\Build\%VCVARS_BAT%" %VCVARS_ARGS%
    goto L_END
:VC141_Professional
    call "%ProgramFiles%\Microsoft Visual Studio\%VCYEAR%\Professional\VC\Auxiliary\Build\%VCVARS_BAT%" %VCVARS_ARGS%
    goto L_END
:VC141_Community
    call "%ProgramFiles%\Microsoft Visual Studio\%VCYEAR%\Community\VC\Auxiliary\Build\%VCVARS_BAT%" %VCVARS_ARGS%
    goto L_END

:L_VC14
    if /I "%CcArch%"=="x64" goto L_VC14x64
    set COMPILER=vc140
    set "PATH=%setcc_base_path%"
    call "%VS140COMNTOOLS%vsvars32.bat"
    set VS140WindowsSdk=%ProgramFiles(x86)%\Windows Kits\8.1
    set "PATH=%VS140WindowsSdk%\bin\x86;%PATH%"
    goto L_END

:L_VC14x64
    set COMPILER=vc140x64
    set "PATH=%setcc_base_path%"
    call "%VS140COMNTOOLS%..\..\vc\bin\amd64\vcvars64.bat"
    set VS140WindowsSdk=%ProgramFiles(x86)%\Windows Kits\8.1
    set "PATH=%VS140WindowsSdk%\bin\x64;%PATH%"
    goto L_END

:L_VC12
    if /I "%CcArch%"=="x64" goto L_VC12x64
    set COMPILER=vc120
    set "PATH=%setcc_base_path%"
    call "%VS120COMNTOOLS%vsvars32.bat"
    goto L_END

:L_VC12x64
    set COMPILER=vc120x64
    set "PATH=%setcc_base_path%"
    call "%VS120COMNTOOLS%..\..\vc\bin\amd64\vcvars64.bat"
    goto L_END

:L_VC11
    if /I "%CcArch%"=="x64" goto L_VC11x64
    set COMPILER=vc110
    set "PATH=%setcc_base_path%"
    call "%VS110COMNTOOLS%vsvars32.bat"
    goto L_END

:L_VC11x64
    set COMPILER=vc110x64
    set "PATH=%setcc_base_path%"
    call "%VS110COMNTOOLS%..\..\vc\bin\amd64\vcvars64.bat"
    goto L_END

:L_VC10
    if /I "%CcArch%"=="x64" goto L_VC10x64
    set COMPILER=vc100
    set "PATH=%setcc_base_path%"
    call "%VS100COMNTOOLS%vsvars32.bat"
    goto L_END

:L_VC10x64
    set COMPILER=vc100x64
    set "PATH=%setcc_base_path%"
    call "%VS100COMNTOOLS%..\..\vc\bin\amd64\vcvarsamd64.bat"
    goto L_END

:L_VC9
    if /I "%CcArch%"=="x64" goto L_VC9x64
    set COMPILER=vc90
    set "PATH=%setcc_base_path%"
    call "%VS90COMNTOOLS%vsvars32.bat"
    goto L_END

:L_VC9x64
    set COMPILER=vc90x64
    set "PATH=%setcc_base_path%"
    call "%VS90COMNTOOLS%..\..\vc\bin\amd64\vcvarsamd64.bat"
    goto L_END

:L_VC8
    if /I "%CcArch%"=="x64" goto L_VC8x64
    set COMPILER=vc80
    set "PATH=%setcc_base_path%"
    call "%VS80COMNTOOLS%vsvars32.bat"
    goto L_END

:L_VC8x64
    set COMPILER=vc80x64
    set "PATH=%setcc_base_path%"
    call "%VS80COMNTOOLS%..\..\vc\bin\amd64\vcvarsamd64.bat"
    goto L_END

:L_VC71
    set COMPILER=vc71
    set "PATH=%setcc_base_path%"
    if /i "%VS71_DIR%"=="" set "VS71_DIR=%ProgramFiles(x86)%\Microsoft Visual Studio .NET 2003"
    if /i "%VS71COMNTOOLS%"=="" set "VS71COMNTOOLS=%VS71_DIR%\Common7\Tools\"
    set "INCLUDE=%VS71_DIR%\SDK\v1.1\include\"
    set "LIB=%VS71_DIR%\SDK\v1.1\Lib\"
    call "%VS71COMNTOOLS%vsvars32.bat"
    goto L_END

:L_VC70
    set COMPILER=vc70
    set "PATH=%setcc_base_path%"
    if /i "%VS70_DIR%"=="" set "VS70_DIR=%ProgramFiles(x86)%\Microsoft Visual Studio .NET"
    if /i "%VS70COMNTOOLS%"=="" set "VS70COMNTOOLS=%VS70_DIR%\Common7\Tools\"
    call "%VS70COMNTOOLS%vsvars32.bat"
    goto L_END

:L_VC6
    set COMPILER=vc60
    set "PATH=%setcc_base_path%"
    if /i "%VS6_DIR%"=="" set "VS6_DIR=%ProgramFiles(x86)%\Microsoft Visual Studio"
    if /i "%VS60COMNTOOLS%"=="" set "VS60COMNTOOLS=%VS6_DIR%\Common\Tools\"
    call "%VS6_DIR%\vc6\bin\vcvars32.bat"
    goto L_END

:: #############################

:L_MSYS2_CLANG
    if /I "%CcArch%"=="x64" goto L_MSYS2_CLANG64
    set COMPILER=msys2clang32
    if /i "%MSYS2_DIR%"=="" set "MSYS2_DIR=c:\msys64"
    set "PATH=%MSYS2_DIR%\clang32\bin;%MSYS2_DIR%\usr\bin;%setcc_base_path%"
    goto :L_END

:L_MSYS2_CLANG64
    set COMPILER=msys2clang64
    if /i "%MSYS2_DIR%"=="" set "MSYS2_DIR=c:\msys64"
    set "PATH=%MSYS2_DIR%\clang64\bin;%MSYS2_DIR%\usr\bin;%setcc_base_path%"
    goto :L_END

:L_MSYS2
    if /I "%CcArch%"=="x64" goto L_MSYS2_64
    set COMPILER=msys2mingw32
    if /i "%MSYS2_DIR%"=="" set "MSYS2_DIR=c:\msys64"
    set "PATH=%MSYS2_DIR%\mingw32\bin;%MSYS2_DIR%\usr\bin;%setcc_base_path%"
    goto :L_END

:L_MSYS2_64
    set COMPILER=msys2ucrt64
    if /i "%MSYS2_DIR%"=="" set "MSYS2_DIR=c:\msys64"
    set "PATH=%MSYS2_DIR%\ucrt64\bin;%MSYS2_DIR%\usr\bin;%setcc_base_path%"
    goto :L_END

:L_CYGWIN
    if /I "%CcArch%"=="x64" goto L_CYGWIN64
    set COMPILER=cygwin32
    if /i "%CYGWIN32_DIR%"=="" set "CYGWIN_DIR32=c:\cygwin"
    set "PATH=%CYGWIN_DIR32%\bin;%setcc_base_path%"
    goto :L_END

:L_CYGWIN64
    set COMPILER=cygwin64
    if /i "%CYGWIN64_DIR%"=="" set "CYGWIN64_DIR=c:\cygwin64"
    set "PATH=%CYGWIN64_DIR%\bin;%setcc_base_path%"
    goto :L_END

:L_ORANGEC
    set COMPILER=orangec
    if /i "%ORANGEC_DIR%"=="" set "ORANGEC_DIR=C:\Program Files (x86)\Orange C 386"
    set "PATH=%ORANGEC_DIR%\bin;%setcc_base_path%"
    goto :L_END

:L_DMC
    set COMPILER=dmc
    if /i "%DMC_DIR%"=="" set "DMC_DIR=c:\dm"
    set "PATH=%DMC_DIR%\bin;%setcc_base_path%"
    goto :L_END

:L_OW19
    set COMPILER=ow19
    set "WATCOM=c:\watcom19"
    goto L_WATCOM
:L_OW20
    set COMPILER=ow20
    set "WATCOM=c:\watcom20"
    goto L_WATCOM
:L_WATCOM
    set COMPILER=watcom
    if /i "%WATCOM%"=="" set "WATCOM=c:\watcom"
    set "PATH=%WATCOM%\BINNT;%WATCOM%\BINW;%setcc_base_path%"
    set "EDPATH=%WATCOM%\EDDAT"
    set "INCLUDE=%WATCOM%\H;%WATCOM%\H\NT;%INCLUDE%"
    set "FINCLUDE=%WATCOM%\SRC\FORTRAN"
    goto :L_END

:L_BCC55
    set COMPILER=bcc55
    if /i "%BCC55_DIR%"=="" set "BCC55_DIR=c:\borland\bcc55"
    set "PATH=%BCC55_DIR%\bin;%setcc_base_path%"
    set "INCLUDE=%BCC55_DIR%\include;%BCC55_DIR%\include\Rw;%BCC55_DIR%\include\psdk"
    goto :L_END

rem ## c ######################################

:L_LSIC86
    set "COMPILER=lsic86"
    if /i "%LSIC86_DIR%"=="" set "LSIC86_DIR=c:\lsic330c"
    set "PATH=%LSIC86_DIR%\BIN;C:\BIN;%windir%\system32;%windir%;"
    set "LIB=%LSIC86_DIR%\LIB"
    set "INCLUDE=%LSIC86_DIR%\INCLUDE"
    set "TEMP=C:\TEMP";
    set "TMP=C:\TEMP";
    goto :L_END

:L_TINYC
    if /I "%CcArch%"=="x64" goto L_TINYC64
    set "COMPILER=tinyc"
    if /i "%TINYC_DIR%"=="" set "TINYC_DIR=c:\tools\tcc-0.9.26win32"
    set "PATH=%TINYC_DIR%;%setcc_base_path%"
    goto :L_END

:L_TINYC64
    set "COMPILER=tinyc64"
    if /i "%TINYC64_DIR%"=="" set "TINYC64_DIR=c:\tools\tcc-0.9.26win64"
    set "PATH=%TINYC64_DIR%;%setcc_base_path%"
    goto :L_END

:L_PELLESC
    if /I "%CcArch%"=="x64" goto L_PELLESC64
    set "COMPILER=pellesc"
    set "PellesC_Dir=%ProgramFiles(x86)%\PellesC"
    set "PATH=%PellesC_Dir%\bin;%setcc_base_path%"
    set "INCLUDE=%PellesC_Dir%\include;%PellesC_Dir%\include\win;%INCLUDE%"
    set "LIB=%PellesC_Dir%\lib;%PellesC_Dir%\lib\win;%LIB%"
    goto :L_END

:L_PELLESC64
    set "COMPILER=pellesc64"
    set "PellesC64_Dir=%ProgramFiles%\PellesC"
    set "PATH=%PellesC64_Dir%\bin;%setcc_base_path%"
    set "INCLUDE=%PellesC64_Dir%\include;%PellesC64_Dir%\include\win;%INCLUDE%"
    set "LIB=%PellesC64_Dir%\lib;%PellesC64_Dir%\lib\win64;%LIB%"
    goto :L_END

:L_LCCWIN
    if /I "%CcArch%"=="x64" goto L_LCCWIN64
    set "COMPILER=lccwin32"
    if /i "%LCCWIN32_DIR%"=="" set "LCCWIN32_DIR=c:\lccwin32"
    set "PATH=%LCCWIN32_DIR%\bin;%setcc_base_path%"
    set "INCLUDE=%LCCWIN32_DIR%\include;%INCLUDE%"
    goto :L_END

:L_LCCWIN64
    set "COMPILER=lccwin64"
    if /i "%LCCWIN64_DIR%"=="" set "LCCWIN64_DIR=c:\lccwin64"
    set "PATH=%LCCWIN64_DIR%\bin;%setcc_base_path%"
    set "INCLUDE=%LCCWIN64_DIR%\include;%INCLUDE%"
    goto :L_END

:L_LCC
    set "COMPILER=lcc"
    if /i "%LCC_DIR%"=="" set "LCC_DIR=c:\tools\lcc"
    set "PATH=%LCC_DIR%\bin;%setcc_base_path%"
    goto :L_END

:L_PCC
    set "COMPILER=pcc"
    if /i "%PCC_DIR%"=="" set "PCC_DIR=c:\tools\pcc"
    set "PATH=%PCC_DIR%\bin;%setcc_base_path%"
    goto :L_END

:L_COINS
    set "COMPILER=coins"
    if /i "%COINS_DIR%"=="" set "COINS_DIR=c:\tools\coins-1.5-en"
    if /i "%CYGWIN32_DIR%"=="" set "CYGWIN_DIR32=c:\cygwin"
    set "PATH=%COINS_DIR%\bin;%CYGWIN_DIR32%\bin;%JAVA_HOME%\bin;%setcc_base_path%"
    goto :L_END

rem ## other ######################################

:L_RUST
    set "COMPILER=rust"
    set "PATH=%USERPATH%\.cargo\bin;%setcc_base_path%"
    goto :L_END

:L_DMD2
    set "COMPILER=dmd2"
    if /i "%DMD2_DIR%"=="" set "DMD2_DIR=c:\D\dmd2"
    if /i "%DMC_DIR%"=="" set "DMC_DIR=c:\dmc"
    set "PATH=%DMD2_DIR%\windows\bin;%DMC_DIR%\bin;%setcc_base_path%"
    goto :L_END

:L_DMD1
    set "COMPILER=dmd1"
    if /i "%DMD1_DIR%"=="" set "DMD1_DIR=c:\D\dmd1"
    if /i "%DMC_DIR%"=="" set "DMC_DIR=c:\dmc"
    set "PATH=%DMD1_DIR%\windows\bin;%DMC_DIR%\bin;%setcc_base_path%"
    goto :L_END

:L_LDC2
    set "COMPILER=ldc2"
    rem call :C_VC141x64
    if /i "%LDC2_DIR%"=="" set "LDC2_DIR=c:\tools\ldc2-1.6.0-beta1-win64-msvc"
    set "PATH=%LDC2_DIR%\bin;%setcc_base_path%"
    goto :L_END

:L_GO
    set "COMPILER=go"
    if /i "%GO_DIR%"=="" set "GO_DIR=c:\go"
    set "PATH=%GO_DIR%\bin;%setcc_base_path%"
    goto :L_END

:L_JAVA
    set "COMPILER=java"
    if /i "JAVA_HOME"=="" set "JAVA_HOME=%ProgramFiles%\Java\jdk1.8.0_152"
    if /i "JRE_DIR"==""   set "JRE_DIR=%ProgramFiles%\Java\jre1.8.0_152"
    set "PATH=%JAVA_HOME%\bin;%JRE_DIR%\bin;%setcc_base_path%"
    if /i not "ANT_HOME"=="" set "PATH=%ANT_HOME%\bin;%PATH%"
    goto :L_END

:L_END
set setcc_base_path=
rem pause
