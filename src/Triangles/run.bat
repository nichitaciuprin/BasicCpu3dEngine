@echo off
SETLOCAL
set _projectroot=%~dp0

%_projectroot%build\main.exe
if %errorlevel% neq 0 echo === ERROR === %errorlevel%