@echo off
setlocal

set root=%~dp0..\..
set deps=%root%\deps
set batdir=%~dp0
set build=%batdir%build

@REM -O (Same as -O1)
@REM -O0 (do no optimize, the default if no optimization level is specified)
@REM -O1 (optimize minimally, favoring compilation time)
@REM -O2 (optimize more, without speed/size tradeoff)
@REM -O3 (optimize even more, favoring speed)
@REM -Ofast (optimize very aggressively to the point of breaking standard compliance, favoring speed. May change program behavior)
@REM -Og (Optimize debugging experience. -Og enables optimizations that do not interfere with debugging. It should be the optimization level of choice for the standard edit-compile-debug cycle, offering a reasonable level of optimization while maintaining fast compilation and a good debugging experience.)
@REM -Os (Optimize for size. -Os enables all -O2 optimizations that do not typically increase code size. It also performs further optimizations designed to reduce code size. -Os disables the following optimization flags: -falign-functions -falign-jumps -falign-loops -falign-labels -freorder-blocks -freorder-blocks-and-partition -fprefetch-loop-arrays -ftree-vect-loop-version)
set optimisationLevel=-Og

set flags=-g3 %optimisationLevel% -std=c17 -pthread -m64

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
@REM set src=%src% %deps%\Base\src\BitmapWindow.cpp
@REM set src=%src% %deps%\Base\src\SysHelperWin.cpp
@REM set src=%src% %deps%\Base\src\SysHelperWin2.cpp
@REM set src=%src% %deps%\RaylibWrap\src\RaylibWrap.cpp

if exist %build% rmdir /S /Q %build%
   mkdir %build%

gcc main2.c %src% -o build/main.exe %flags% %include% %lib%
