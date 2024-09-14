@echo off
setlocal

set _build=%_batdir%build
@REM set deps=%_batdir%..\..\deps

set root=%~dp0..\..
set deps=%root%\deps

set INCLUDE=
set INCLUDE=%INCLUDE%%deps%\MSVC\include;
set INCLUDE=%INCLUDE%%deps%\WindowsKits\10\Include\10.0.22621.0\ucrt;
set INCLUDE=%INCLUDE%%deps%\WindowsKits\10\Include\10.0.22621.0\um;
set INCLUDE=%INCLUDE%%deps%\WindowsKits\10\Include\10.0.22621.0\shared;

set LIB=
set LIB=%LIB%%deps%\MSVC\lib\x64;
set LIB=%LIB%%deps%\WindowsKits\10\Lib\10.0.22621.0\um\x64;
set LIB=%LIB%%deps%\WindowsKits\10\Lib\10.0.22621.0\ucrt\x64;

set LINKER_FLAGS=/INCREMENTAL:NO
set SYSTEM_LIBS=user32.lib gdi32.lib winmm.lib d3d11.lib d3dcompiler.lib

set _cl=%deps%\MSVC\bin\Hostx64\x64\cl.exe
set _input=%_maincpp%
set _output=/Fe"%_build%\BoidsDirectX" /Fo"%_build%\BoidsDirectX"
set _optimimisationDisable=/Od
set _optimimisationLevel2=/O2
set _enableWarningsLevel4=/W4
set _treatWarningsAsErrors=/WX
set _hidelogs=/nologo
set _exceptionHandling=/EHsc
set _enablesExtraWarning=/analyze

set _options=
set _options=%_options% %_exceptionHandling%
set _options=%_options% %_hidelogs%
set _options=%_options% %_optimimisationLevel2%
set _options=%_options% %_enableWarningsLevel4%
set _options=%_options% %_enablesExtraWarning%

@REM TODO maybe remove
if exist build rmdir /S /Q build

if not exist build mkdir build

%_cl% main.cpp %_output% %_options% %LINKER_FLAGS% %SYSTEM_LIBS%

if %errorlevel% neq 0 exit /b %errorlevel%