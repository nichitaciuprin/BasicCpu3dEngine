@echo off
setlocal

set root=%~dp0..\..
set batdir=%~dp0
set build=%batdir%build
set deps=%root%\deps

set INCLUDE=
set INCLUDE=%INCLUDE%%deps%\MSVC\include;
set INCLUDE=%INCLUDE%%deps%\WindowsKits\10\Include\10.0.22621.0\ucrt;
set INCLUDE=%INCLUDE%%deps%\WindowsKits\10\Include\10.0.22621.0\um;
set INCLUDE=%INCLUDE%%deps%\WindowsKits\10\Include\10.0.22621.0\shared;
set INCLUDE=%INCLUDE%%deps%\BaseOld;

set LIB=
set LIB=%LIB%%deps%\MSVC\lib\x64;
set LIB=%LIB%%deps%\WindowsKits\10\Lib\10.0.22621.0\um\x64;
set LIB=%LIB%%deps%\WindowsKits\10\Lib\10.0.22621.0\ucrt\x64;

set LINKER_FLAGS=/INCREMENTAL:NO
set SYSTEM_LIBS=user32.lib gdi32.lib winmm.lib d3d11.lib d3dcompiler.lib

set _link=%deps%\MSVC\bin\Hostx64\x64\link.exe
set _cl=%deps%\MSVC\bin\Hostx64\x64\cl.exe

set _optimimisationDisable=/Od
set _optimimisationLevel2=/O2
set _enableWarningsLevel4=/W4
set _treatWarningsAsErrors=/WX
set _hidelogs=/nologo
set _exceptionHandling=/EHsc
set _enablesExtraWarning=/analyze

set _options=^
%_exceptionHandling% ^
%_hidelogs% ^
%_optimimisationLevel2% ^
%_enableWarningsLevel4% ^
%_enablesExtraWarning%

if not exist build mkdir build

%_cl% /nologo /c test.cpp /Fo%build%\test %_options%
@REM %_cl% /nologo /c main.cpp /Fo"%build%\main" %_options%
@REM %_link% /nologo "%build%\test.obj" "%build%\main.obj" /OUT:"%build%\main.exe" %LINKER_FLAGS% %SYSTEM_LIBS%

@REM %_cl% main.cpp %_output% %_options% %LINKER_FLAGS% %SYSTEM_LIBS%

if %errorlevel% neq 0 exit /b %errorlevel%
