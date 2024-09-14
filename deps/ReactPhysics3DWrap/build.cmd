@echo off
setlocal

set deps=%~f0\..\..
set react=%deps%\ReactPhysics3D

if not exist build mkdir build

g++ -c Rp3dWrap.cpp -o build\Rp3dWrap.obj -I%react%\include
ar -x %react%\lib\libreactphysics3d.a --output=build
ar rcs librp3dwrap.a build\*.obj
