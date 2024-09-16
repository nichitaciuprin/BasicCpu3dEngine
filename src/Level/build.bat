@echo off
setlocal

set root=%~dp0..\..
set deps=%root%\deps

set flags=-g3 -O0 -Wall -std=c++17 -Wno-missing-braces -pthread -m64

@REM set lib=%lib% -L%deps%\raylib\lib -ldloadhelper -lglmf32 -lm -lopengl32 -lgdi32 -lwinmm -ld3d11 -ld3dcompiler

@REM set lib=%lib% -lpthread -lm

set lib=
set lib=%lib% -L%deps%\raylib\lib -lraylib -lgdi32 -lwinmm
set lib=%lib% -L%deps%\ReactPhysics3D\lib -lreactphysics3d

set include=
set include=%include% -I%deps%\Base\include
set include=%include% -I%deps%\Raylib\include
set include=%include% -I%deps%\RaylibWrap\include
set include=%include% -I%deps%\ReactPhysics3D\include
set include=%include% -I%deps%\ReactPhysics3DWrap\include
set include=%include% -I%~dp0src

set src=
set src=%src% %deps%\Base\BitmapWindow.cpp
set src=%src% %deps%\Base\SysHelperWin.cpp
set src=%src% %deps%\Base\SysHelperWin2.cpp
set src=%src% %deps%\RaylibWrap\RaylibWrap.cpp

if not exist build mkdir build

g++ main.cpp %src% -o build/main.exe %flags% %include% %lib%
