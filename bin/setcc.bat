@echo off
::
:: This batch-file license: boost software license version 1.0
:: Please adjust to your compiler path if necessary.
::
goto L_START

:L_HELP
@echo USAGE: setcc [COMPILER] [win32/x64]
@echo   COMPILER:
@echo       vc145,vc143,vc142,vc141,vc140,vc120,vc110,vc100,vc90,vc80,vc71
@echo       msys2,msys2clang,mingw,watcom,dmc,orangec,djgpp
goto L_END

:L_START
set "CcName=%~1"
set "CcArch=%~2"

set INCLUDE=
set LIB=

if "%setcc_save_path%"=="" set "setcc_save_path=%path%"
set "setcc_base_path=%setcc_save_path%"

if not "%CcNasmDir%"==""  set "setcc_base_path=%CcNasmDir%;%setcc_base_path%"
if not "%CcPerlDir%"==""  set "setcc_base_path=%CcPerlDir%;%setcc_base_path%"
rem set VCPKG_DEFAULT_TRIPLET=x64-windows-static-md

:: Host architecture
set CcHostArch=x64
if /I "%PROCESSOR_ARCHITECTURE%"=="AMD64" (
    set "CcHostArch=x64"
) else if /I "%PROCESSOR_ARCHITECTURE%"=="ARM64" (
    set "CcHostArch=arm64"
) else if /I "%PROCESSOR_ARCHITECTURE%"=="x86" (
    if defined PROCESSOR_ARCHITEW6432 (
        set "CcHostArch=x64"
    ) else (
        set "CcHostArch=x86"
    )
)

if /i "%ProgramFiles(x86)%"=="" set "ProgramFiles(x86)=%ProgramFiles%"

set COMPILER=
set VcVers_Args=

if "%CcArch%"==""         set CcArch=x64
if "%CcArch%"=="64"       set CcArch=x64
if /i "%CcArch%"=="amd64" set CcArch=x64
if "%CcArch%"=="32"       set CcArch=x86
if /i "%CcArch%"=="win32" set CcArch=x86
if /i "%CcArch%"=="arm64" set CcArch=arm64
if /i "%CcArch%"=="arm"   set CcArch=arm

if /i "%CcName%"=="vc145"      goto L_VC145
if /i "%CcName%"=="vc144"      goto L_VC143
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

if /i "%CcName%"=="vc14.5"     goto L_VC145
if /i "%CcName%"=="vc14.4"     goto L_VC143
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
if /i "%CcName%"=="mingw32"    goto L_MINGW32
if /i "%CcName%"=="gcc"        goto L_MSYS2
if /i "%CcName%"=="msys2clang" goto L_MSYS2_CLANG
if /i "%CcName%"=="clang"      goto L_MSYS2_CLANG
if /i "%CcName%"=="cygwin"     goto L_CYGWIN

if /i "%CcName%"=="watcom"     goto L_WATCOM
if /i "%CcName%"=="ow"         goto L_WATCOM
if /i "%CcName%"=="ow19"       goto L_OW19
if /i "%CcName%"=="ow20"       goto L_OW20
if /i "%CcName%"=="dmc"        goto L_DMC
if /i "%CcName%"=="orangec"    goto L_ORANGEC
if /i "%CcName%"=="occ"        goto L_ORANGEC
if /i "%CcName%"=="borland"    goto L_BCC55
if /i "%CcName%"=="bcc32"      goto L_BCC55
if /i "%CcName%"=="bc55"       goto L_BCC55
if /i "%CcName%"=="bcc101"     goto L_BCC101
if /i "%CcName%"=="embarcadero" goto L_EmbarcaderoC76
if /i "%CcName%"=="embarcaderoC76" goto L_EmbarcaderoC76

if /i "%CcName%"=="djgpp"       goto L_DJGPP
if /i "%CcName%"=="djgpp_msys2" goto L_DJGPP_MSYS2
if /i "%CcName%"=="djgpp_mingw" goto L_DJGPP_MINGW
if /i "%CcName%"=="djgpc98"     goto L_DJGPC98
if /i "%CcName%"=="djgpc98_msys2" goto L_DJGPP_MSYS2
if /i "%CcName%"=="djgpc98_mingw" goto L_DJGPP_MINGW

if /i "%CcName%"=="djgpp6"     goto L_DJGPP6
if /i "%CcName%"=="djgpp7"     goto L_DJGPP7
if /i "%CcName%"=="djgpp8"     goto L_DJGPP8
if /i "%CcName%"=="djgpp9"     goto L_DJGPP9
if /i "%CcName%"=="djgpp10"    goto L_DJGPP10
if /i "%CcName%"=="djgpp12"    goto L_DJGPP12
if /i "%CcName%"=="djgpp14"    goto L_DJGPP14

rem
if /i "%CcName%"=="Z88dk"      goto L_Z88DK
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

@goto L_HELP


rem ## vc ######################################

:L_VC145
    set VcVer=vc145
    set VcYear=18
    set "VsRoot=%ProgramFiles%\Microsoft Visual Studio\%VcYear%"
    goto L_VC14x_J2

:L_VC143
    set VcVer=vc143
    set VcYear=2022
    set "VsRoot=%ProgramFiles%\Microsoft Visual Studio\%VcYear%"
    goto L_VC14x_J2

:L_VC142
    set VcVer=vc142
    set VcYear=2019
    goto L_VC14x_J1

:L_VC141
    set VcVer=vc141
    set VcYear=2017
    goto L_VC14x_J1

:L_VC14x_J1
    set "VsRoot=%ProgramFiles(x86)%\Microsoft Visual Studio\%VcYear%"

:L_VC14x_J2
    set "COMPILER=%VcVer%%CcArch%"
    rem set VcSdkType=10.0.22621.0
    set "VcVers_Args=%CcArch%"
    if /I not "%CcArch%"=="%CcHostArch%" set "VcVers_Args=%CcHostArch%_%CcArch%"
    if /I "%VcVers_Args%"=="x64_x86" set "VcVers_Args=x86"
    rem if /I "%VcVers_Args:~0,3%"=="x64" set "VcVers_Args=amd64%VcVers_Args:~3%"
    set "PATH=%setcc_base_path%"
    set "VsEdition="
    if exist "%VsRoot%\Community\Common7\Tools\VsMSBuildCmd.bat"    set "VsEdition=Community"
    if exist "%VsRoot%\Professional\Common7\Tools\VsMSBuildCmd.bat" set "VsEdition=Professional"
    if exist "%VsRoot%\Enterprise\Common7\Tools\VsMSBuildCmd.bat"   set "VsEdition=Enterprise"
    if exist "%VsRoot%\Insiders\Common7\Tools\VsMSBuildCmd.bat"     set "VsEdition=Insiders"
    if "%VsEdition%"=="" goto L_VC14x_ERROR
    set "VsRoot=%VsRoot%\%VsEdition%"
    call "%VsRoot%\VC\Auxiliary\Build\vcvarsall.bat" %VcVers_Args% %VcSdkType%
    goto L_END
:L_VC14x_ERROR
    set VsRoot=
    echo ERROR: Not found "Microsoft Visual Studio %VcYear%"
    goto L_END

:L_VC14
    set VcVer=vc140
    rem set VcSdkType=8.1
    ::set VcSdkType=10.0.10240.0
    set "VcSdkType="
    for %%V in (10.0.19041.0 10.0.17763.0 10.0.16299.0 10.0.14393.0 10.0.10240.0 10.0.22621.0 10.0.26100.0) do (
        if not defined VcSdkType if exist "%ProgramFiles(x86)%\Windows Kits\10\Lib\%%V\um\%CcArch%\kernel32.lib" set "VcSdkType=%%V"
    )
    if not defined VcSdkType set VcSdkType=10.0.10240.0
    set "COMPILER=%VcVer%%CcArch%"
    set "WindowsSdkDir=%ProgramFiles(x86)%\Windows Kits\10\"
    set "UniversalCRTSdkDir=%ProgramFiles(x86)%\Windows Kits\10\"
    set "WindowsSdkVersion=%VcSdkType%\"
    set "VcVers_Args=%CcArch%"
    if /I not "%CcArch%"=="%CcHostArch%" set "VcVers_Args=%CcHostArch%_%CcArch%"
    if /I "%VcVers_Args%"=="x64_x86" set "VcVers_Args=x86"
    pushd "%VS140COMNTOOLS%..\.."
    set "VcRoot=%CD%"
    popd
    set "PATH=%setcc_base_path%"
    if /I "%VcVers_Args%"=="x86"     call "%VS140COMNTOOLS%vsvars32.bat" %VcSdkType%
    if /I "%VcVers_Args%"=="x64"     call "%VcRoot%\vc\bin\amd64\vcvars64.bat" %VcSdkType%
    if /I "%VcVers_Args%"=="x86_x64" call "%VcRoot%\vc\bin\x86_amd64\vcvarsx86_amd64.bat" %VcSdkType%
    if /I "%VcVers_Args%"=="x64_arm" call "%VcRoot%\vc\bin\amd64_arm\vcvarsamd64_arm.bat" %VcSdkType%
    if /I "%VcVers_Args%"=="x86_arm" call "%VcRoot%\vc\bin\x86_arm\vcvarsx86_arm.bat" %VcSdkType%
    if "%VcSdkType%"=="8.1" set "PATH=%ProgramFiles(x86)%\Windows Kits\%VcSdkType%\bin\x86;%PATH%"
    set "VcSdkType="
    goto L_END

:L_VC12
    set VcVer=vc120
    set "COMPILER=%VcVer%%CcArch%"
    set "PATH=%setcc_base_path%"
    if /I "%CcArch%"=="x64" goto L_VC12x64
    call "%VS120COMNTOOLS%vsvars32.bat"
    goto L_END
:L_VC12x64
    call "%VS120COMNTOOLS%..\..\vc\bin\amd64\vcvars64.bat"
    goto L_END

:L_VC11
    set VcVer=vc110
    set "COMPILER=%VcVer%%CcArch%"
    set "PATH=%setcc_base_path%"
    if /I "%CcArch%"=="x64" goto L_VC11x64
    call "%VS110COMNTOOLS%vsvars32.bat"
    goto L_END
:L_VC11x64
    call "%VS110COMNTOOLS%..\..\vc\bin\amd64\vcvars64.bat"
    goto L_END

:L_VC10
    set VcVer=vc100
    set "COMPILER=%VcVer%%CcArch%"
    set "PATH=%setcc_base_path%"
    if /I "%CcArch%"=="x64" goto L_VC10x64
    call "%VS100COMNTOOLS%vsvars32.bat"
    goto L_END
:L_VC10x64
    call "%VS100COMNTOOLS%..\..\vc\bin\amd64\vcvarsamd64.bat"
    goto L_END

:L_VC9
    set VcVer=vc90
    set "COMPILER=%VcVer%%CcArch%"
    set "PATH=%setcc_base_path%"
    if /I "%CcArch%"=="x64" goto L_VC9x64
    call "%VS90COMNTOOLS%vsvars32.bat"
    goto L_END
:L_VC9x64
    call "%VS90COMNTOOLS%..\..\vc\bin\amd64\vcvarsamd64.bat"
    goto L_END

:L_VC8
    set VcVer=vc80
    set "COMPILER=%VcVer%%CcArch%"
    set "PATH=%setcc_base_path%"
    if /I "%CcArch%"=="x64" goto L_VC8x64
    call "%VS80COMNTOOLS%vsvars32.bat"
    goto L_END
:L_VC8x64
    call "%VS80COMNTOOLS%..\..\vc\bin\amd64\vcvarsamd64.bat"
    goto L_END

:L_VC71
    set VcVer=vc71
    set "COMPILER=%VcVer%%CcArch%"
    set "PATH=%setcc_base_path%"
    if /i "%VS71_ROOT%"=="" set "VS71_ROOT=%ProgramFiles(x86)%\Microsoft Visual Studio .NET 2003"
    if /i "%VS71COMNTOOLS%"=="" set "VS71COMNTOOLS=%VS71_ROOT%\Common7\Tools\"
    set "INCLUDE=%VS71_ROOT%\SDK\v1.1\include\"
    set "LIB=%VS71_ROOT%\SDK\v1.1\Lib\"
    call "%VS71COMNTOOLS%vsvars32.bat"
    goto L_END

:L_VC70
    set VcVer=vc70
    set "COMPILER=%VcVer%%CcArch%"
    set "PATH=%setcc_base_path%"
    if /i "%VS70_ROOT%"=="" set "VS70_ROOT=%ProgramFiles(x86)%\Microsoft Visual Studio .NET"
    if /i "%VS70COMNTOOLS%"=="" set "VS70COMNTOOLS=%VS70_ROOT%\Common7\Tools\"
    call "%VS70COMNTOOLS%vsvars32.bat"
    goto L_END

:L_VC6
    set VcVer=vc60
    set "COMPILER=%VcVer%%CcArch%"
    set "PATH=%setcc_base_path%"
    if /i "%VS6_ROOT%"=="" set "VS6_ROOT=%ProgramFiles(x86)%\Microsoft Visual Studio"
    if /i "%VS60COMNTOOLS%"=="" set "VS60COMNTOOLS=%VS6_ROOT%\Common\Tools\"
    call "%VS6_ROOT%\vc6\bin\vcvars32.bat"
    goto L_END

:: #############################

:L_MSYS2_CLANG
    if /i "%MSYS2_ROOT%"=="" set "MSYS2_ROOT=c:\msys64"
    if /I "%CcArch%"=="x64" goto L_MSYS2_CLANG64
    set COMPILER=msys2clang32
    set "CC=clang.exe"
    set "CXX=clang++.exe"
    set "PATH=%MSYS2_ROOT%\clang32\bin;%MSYS2_ROOT%\usr\bin;%setcc_base_path%"
    goto L_END
:L_MSYS2_CLANG64
    set COMPILER=msys2clang64
    set "PATH=%MSYS2_ROOT%\clang64\bin;%MSYS2_ROOT%\usr\bin;%setcc_base_path%"
    goto L_END

:L_MSYS2
    if /i "%MSYS2_ROOT%"=="" set "MSYS2_ROOT=c:\msys64"
    if /I "%CcArch%"=="x64" goto L_MSYS2_64
    set COMPILER=msys2mingw32
    set "PATH=%MSYS2_ROOT%\mingw32\bin;%MSYS2_ROOT%\usr\bin;%setcc_base_path%"
    goto L_END
:L_MSYS2_64
    set COMPILER=msys2ucrt64
    set "PATH=%MSYS2_ROOT%\ucrt64\bin;%MSYS2_ROOT%\usr\bin;%setcc_base_path%"
    goto L_END

:L_MINGW32
    set COMPILER=mingw32
    if /i "%MINGW_ROOT%"=="" set "MINGW_ROOT=c:\MinGW"
    set "PATH=%MINGW_ROOT%\bin;%MINGW_ROOT%\msys\1.0\bin;%setcc_base_path%"
    goto L_END

:L_CYGWIN
    if /I "%CcArch%"=="x64" goto L_CYGWIN64
    set COMPILER=cygwin32
    if /i "%CYGWIN32_ROOT%"=="" set "CYGWIN_ROOT32=c:\cygwin"
    set "PATH=%CYGWIN_ROOT32%\bin;%setcc_base_path%"
    goto L_END

:L_CYGWIN64
    set COMPILER=cygwin64
    if /i "%CYGWIN64_ROOT%"=="" set "CYGWIN64_ROOT=c:\cygwin64"
    set "PATH=%CYGWIN64_ROOT%\bin;%setcc_base_path%"
    goto L_END

:L_ORANGEC
    set COMPILER=orangec
    if /i not "%ORANGEC_ROOT%"=="" if exist "%ORANGEC_ROOT%\bin\occ.exe" goto L_ORANGE_PATH
    set "ORANGEC_ROOT=%ProgramFiles(x86)%\OrangeC"
    if exist "%ORANGEC_ROOT%\bin\occ.exe" goto L_ORANGE_PATH
    set "ORANGEC_ROOT=%ProgramFiles(x86)%\Orange C 386"
    if exist "%ORANGEC_ROOT%\bin\occ.exe" goto L_ORANGE_PATH
    set "ORANGEC_ROOT=%ProgramFiles(x86)%\OrangeC"
:L_ORANGE_PATH
    set "PATH=%ORANGEC_ROOT%\bin;%setcc_base_path%"
    goto L_END

:L_DMC
    set COMPILER=dmc
    if /i "%DMC_ROOT%"=="" set "DMC_ROOT=c:\dm"
    set "PATH=%DMC_ROOT%\bin;%setcc_base_path%"
    :: https://github.com/tenk-a/cc_for_dmc
    set "PATH=d:\proj\cc_for_dmc\bin;%PATH%"
    set "CC=dmc-cc.exe"
    set "CXX=dmc-cc.exe"
    goto L_END

:L_OW19
    set COMPILER=ow19
    set "WATCOM=c:\watcom1.9"
    goto L_WATCOM_1
:L_OW20
    set COMPILER=ow20
    set "WATCOM=c:\watcom2.0"
    goto L_WATCOM_1
:L_WATCOM
    set COMPILER=watcom
:L_WATCOM_1
    if /i "%WATCOM%"=="" set "WATCOM=c:\watcom"
    :: set "PATH=%WATCOM%\BINNT;%WATCOM%\BINW;%setcc_base_path%"
    set "PATH=%WATCOM%\BINNT64;%setcc_base_path%"
    set "EDPATH=%WATCOM%\EDDAT"
    rem set "LIB=%PATCOM%\lib386;%PATCOM%\lib386\nt;%PATCOM%\lib386\nt\ddk;%PATCOM%\lib386\nt\directx"
    set "INCLUDE=%WATCOM%\H;%WATCOM%\H\NT;%WATCOM%\H\NT\DIRECTX;%WATCOM%\H\DDK;%INCLUDE%"
    set "FINCLUDE=%WATCOM%\SRC\FORTRAN"
    set "WHTMLHELP=%WATCOM%\BINNT\HELP"
    set "WIPFC=%WATCOM%\WIPFC"
    goto L_END

:L_BCC55
    set COMPILER=bcc55
    if /i "%BCC55_ROOT%"=="" set "BCC55_ROOT=c:\tools\borland\bcc55"
    set "PATH=%BCC55_ROOT%\bin;%setcc_base_path%"
    set "INCLUDE=%BCC55_ROOT%\include;%BCC55_ROOT%\include\Rw;%BCC55_ROOT%\include\psdk"
    goto L_END

:L_BCC101
    set COMPILER=bcc101
    if /i "%BCC101_ROOT%"=="" set "BCC101_ROOT=c:\tools\bcc101"
    set "PATH=%BCC101_ROOT%\bin;%setcc_base_path%"
    goto L_END

:L_EmbarcaderoC76
    set COMPILER=EmbarcaderoC76
    if /i "%EMBARCADERO_ROOT%"=="" set "EMBARCADERO_ROOT=%ProgramFiles(x86)%\Embarcadero\Studio\22.0"
    set "PATH=%EMBARCADERO_ROOT%\bin;%setcc_base_path%"
    goto L_END

:L_DJGPC98_MSYS2
:L_DJGPC98
	set "DJDIR=c:\djgpc98"
:L_DJGPP_MSYS2
:L_DJGPP
    if /i "%MSYS2_ROOT%"=="" set "MSYS2_ROOT=c:\msys64"
    if /i "%DJDIR%"=="" set "DJDIR=c:\djgpp"
    set "MSDOSDJGPP_ROOT=i386-pc-msdosdjgpp"
    set "PATH=%DJDIR%\bin;%DJDIR%\%MSDOSDJGPP_ROOT%\bin;%MSYS2_ROOT%\usr\bin;%MSYS2_ROOT%\mingw64\bin;%setcc_base_path%"
    set  GCC_EXEC_PREFIX=%DJDIR%\lib\gcc\
    goto L_END

:L_DJGPC98_MINGW
	set DJDIR=c:\djgpc98
:L_DJGPP_MINGW
    set COMPILER=djgpp
    if /i "%MINGW_ROOT%"=="" set "MINGW_ROOT=c:\MinGW"
    if /i "%DJDIR%"=="" set "DJDIR=c:\djgpp"
    set "MSDOSDJGPP_ROOT=i586-pc-msdosdjgpp"
    set "PATH=%DJDIR%\bin;%DJDIR%\%MSDOSDJGPP_ROOT%\bin;%MINGW_ROOT%\bin;%MINGW_ROOT%\msys\1.0\bin;%setcc_base_path%"
    set  GCC_EXEC_PREFIX=%DJDIR%\lib\gcc\
    goto L_END


:L_DJGPC98_14
    set COMPILER=djgpp-2.05-gcc-14.2-pc98
    goto L_DJGPP_SUB_MSYS2

:L_DJGPP14
    set COMPILER=djgpp-2.05-gcc-14.2
    goto L_DJGPP_SUB_MSYS2

:L_DJGPP12
    set COMPILER=djgpp-2.05-gcc-12.1
    goto L_DJGPP_SUB_MSYS2

:L_DJGPP10
    set COMPILER=djgpp-2.05-gcc-10.2
    goto L_DJGPP_SUB_MSYS2

:L_DJGPP9
    set COMPILER=djgpp-2.05-gcc-9.2
    goto L_DJGPP_SUB_MSYS2

:L_DJGPP_SUB_MSYS2
    if /i "%MSYS2_ROOT%"=="" set "MSYS2_ROOT=c:\msys64"
    if /i "%DJGPP_VER%"=="" set "DJGPP_VER=14.2.0"
    ::set "DJDIR=%MSYS2_ROOT%\usr\local\MinGW64\%COMPILER%"
    set "DJDIR=%MSYS2_ROOT%\usr\local\ucrt64\%COMPILER%"
    set "MSDOSDJGPP_ROOT=i386-pc-msdosdjgpp"
    set "PATH=%DJDIR%\bin;%DJDIR%\%MSDOSDJGPP_ROOT%\bin;%DJDIR%\%MSDOSDJGPP_ROOT%\%DJGPP_VER%;%MSYS2_ROOT%\usr\bin;%MSYS2_ROOT%\mingw64\bin;%setcc_base_path%"
    set  GCC_EXEC_PREFIX=%DJDIR%\lib\gcc\
    goto L_END

::L_DJGPC98
    set COMPILER=djgpp-2.05-gcc-14.2-pc98
    set DJGPP_VER=14.2.0
    goto L_DJGPP_SUB_MINGW

::L_DJGPP
::L_DJGPP14
    set COMPILER=djgpp-2.05-gcc-14.2
    set DJGPP_VER=14.2.0
    goto L_DJGPP_SUB_MINGW

::L_DJGPP12
    set COMPILER=djgpp-2.05-gcc-12.2
    set DJGPP_VER=12.2.0
    goto L_DJGPP_SUB_MINGW

::L_DJGPP10
    set COMPILER=djgpp-2.05-gcc-10.3
    set DJGPP_VER=10.3.0
    goto L_DJGPP_SUB_MINGW

::L_DJGPP9
    set COMPILER=djgpp-2.05-gcc-9.2
    set DJGPP_VER=9.2.0
    goto L_DJGPP_SUB_MINGW

:L_DJGPP8
    set COMPILER=djgpp-2.05-gcc-8.3
    set DJGPP_VER=8.3.0
    goto L_DJGPP_SUB_MINGW

:L_DJGPP7
    set COMPILER=djgpp-2.05-gcc-7.5
    set DJGPP_VER=7.5.0
    goto L_DJGPP_SUB_MINGW

:L_DJGPP6
    set COMPILER=djgpp-2.05-gcc-6.5
    set DJGPP_VER=6.5.0
    goto L_DJGPP_SUB_MINGW

:L_DJGPP_SUB_MINGW
    if /i "%MINGW_ROOT%"=="" set "MINGW_ROOT=c:\MinGW"
    if /i "%DJGPP_VER%"=="" set "DJGPP_VER=14.2.0"
    set "DJDIR=%MINGW_ROOT%\usr\local\%COMPILER%"
    set "MSDOSDJGPP_ROOT=i386-pc-msdosdjgpp"
    set "PATH=%DJDIR%\bin;%DJDIR%\%MSDOSDJGPP_ROOT%\bin;%DJDIR%\%MSDOSDJGPP_ROOT%\%DJGPP_VER%;%MINGW_ROOT%\bin;%MINGW_ROOT%\msys\1.0\bin;%setcc_base_path%"
    set  GCC_EXEC_PREFIX=%DJDIR%\lib\gcc\
    goto L_END


rem ## c ######################################

:L_Z88DK
    set "COMPILER=Z88dk"
    if /i "%Z88DK_HOME%"=="" set "Z88DK_HOME=c:\z88dk"
    set "ZCCCFG=%Z88DK_HOME%\lib\config"
    set "PATH=%Z88DK_HOME%\bin;%setcc_base_path%"
    goto L_END

:L_LSIC86
    set "COMPILER=lsic86"
    if /i "%LSIC86_ROOT%"=="" set "LSIC86_ROOT=c:\lsic330c"
    set "PATH=%LSIC86_ROOT%\BIN;C:\BIN;%windir%\system32;%windir%;"
    set "LIB=%LSIC86_ROOT%\LIB"
    set "INCLUDE=%LSIC86_ROOT%\INCLUDE"
    set "TEMP=C:\TEMP";
    set "TMP=C:\TEMP";
    goto L_END

:L_TINYC
    if /I "%CcArch%"=="x64" goto L_TINYC64
    set "COMPILER=tinyc"
    if /i "%TINYC_ROOT%"=="" set "TINYC_ROOT=c:\tools\tcc-0.9.26win32"
    set "PATH=%TINYC_ROOT%;%setcc_base_path%"
    goto L_END

:L_TINYC64
    set "COMPILER=tinyc64"
    if /i "%TINYC64_ROOT%"=="" set "TINYC64_ROOT=c:\tools\tcc-0.9.26win64"
    set "PATH=%TINYC64_ROOT%;%setcc_base_path%"
    goto L_END

:L_PELLESC
    if /I "%CcArch%"=="x64" goto L_PELLESC64
    set "COMPILER=pellesc"
    set "PellesC_Dir=%ProgramFiles(x86)%\PellesC"
    set "PATH=%PellesC_Dir%\bin;%setcc_base_path%"
    set "INCLUDE=%PellesC_Dir%\include;%PellesC_Dir%\include\win;%INCLUDE%"
    set "LIB=%PellesC_Dir%\lib;%PellesC_Dir%\lib\win;%LIB%"
    goto L_END

:L_PELLESC64
    set "COMPILER=pellesc64"
    set "PellesC64_Dir=%ProgramFiles%\PellesC"
    set "PATH=%PellesC64_Dir%\bin;%setcc_base_path%"
    set "INCLUDE=%PellesC64_Dir%\include;%PellesC64_Dir%\include\win;%INCLUDE%"
    set "LIB=%PellesC64_Dir%\lib;%PellesC64_Dir%\lib\win64;%LIB%"
    goto L_END

:L_LCCWIN
    if /I "%CcArch%"=="x64" goto L_LCCWIN64
    set "COMPILER=lccwin32"
    if /i "%LCCWIN32_ROOT%"=="" set "LCCWIN32_ROOT=c:\lccwin32"
    set "PATH=%LCCWIN32_ROOT%\bin;%setcc_base_path%"
    set "INCLUDE=%LCCWIN32_ROOT%\include;%INCLUDE%"
    goto L_END

:L_LCCWIN64
    set "COMPILER=lccwin64"
    if /i "%LCCWIN64_ROOT%"=="" set "LCCWIN64_ROOT=c:\lccwin64"
    set "PATH=%LCCWIN64_ROOT%\bin;%setcc_base_path%"
    set "INCLUDE=%LCCWIN64_ROOT%\include;%INCLUDE%"
    goto L_END

:L_LCC
    set "COMPILER=lcc"
    if /i "%LCC_ROOT%"=="" set "LCC_ROOT=c:\tools\lcc"
    set "PATH=%LCC_ROOT%\bin;%setcc_base_path%"
    goto L_END

:L_PCC
    set "COMPILER=pcc"
    if /i "%PCC_ROOT%"=="" set "PCC_ROOT=c:\tools\pcc"
    set "PATH=%PCC_ROOT%\bin;%setcc_base_path%"
    goto L_END

:L_COINS
    set "COMPILER=coins"
    if /i "%COINS_ROOT%"=="" set "COINS_ROOT=c:\tools\coins-1.5-en"
    if /i "%CYGWIN32_ROOT%"=="" set "CYGWIN_ROOT32=c:\cygwin"
    set "PATH=%COINS_ROOT%\bin;%CYGWIN_ROOT32%\bin;%JAVA_HOME%\bin;%setcc_base_path%"
    goto L_END

rem ## other ######################################

:L_RUST
    set "COMPILER=rust"
    set "PATH=%USERPATH%\.cargo\bin;%setcc_base_path%"
    goto L_END

:L_DMD2
    set "COMPILER=dmd2"
    if /i "%DMD2_ROOT%"=="" set "DMD2_ROOT=c:\D\dmd2"
    if /i "%DMC_ROOT%"=="" set "DMC_ROOT=c:\dmc"
    set "PATH=%DMD2_ROOT%\windows\bin;%DMC_ROOT%\bin;%setcc_base_path%"
    goto L_END

:L_DMD1
    set "COMPILER=dmd1"
    if /i "%DMD1_ROOT%"=="" set "DMD1_ROOT=c:\D\dmd1"
    if /i "%DMC_ROOT%"=="" set "DMC_ROOT=c:\dmc"
    set "PATH=%DMD1_ROOT%\windows\bin;%DMC_ROOT%\bin;%setcc_base_path%"
    goto L_END

:L_LDC2
    set "COMPILER=ldc2"
    rem call :C_VC141x64
    if /i "%LDC2_ROOT%"=="" set "LDC2_ROOT=c:\tools\ldc2-1.6.0-beta1-win64-msvc"
    set "PATH=%LDC2_ROOT%\bin;%setcc_base_path%"
    goto L_END

:L_GO
    set "COMPILER=go"
    if /i "%GO_ROOT%"=="" set "GO_ROOT=c:\go"
    set "PATH=%GO_ROOT%\bin;%setcc_base_path%"
    goto L_END

:L_JAVA
    set "COMPILER=java"
    if /i "JAVA_HOME"=="" set "JAVA_HOME=%ProgramFiles%\Java\jdk1.8.0_152"
    if /i "JRE_ROOT"==""   set "JRE_ROOT=%ProgramFiles%\Java\jre1.8.0_152"
    set "PATH=%JAVA_HOME%\bin;%JRE_ROOT%\bin;%setcc_base_path%"
    if /i not "ANT_HOME"=="" set "PATH=%ANT_HOME%\bin;%PATH%"
    goto L_END

:L_END
set VcVers_Args=
if /i "%CcArch%"=="x86" set "CcArch=win32"
set setcc_base_path=
rem pause
