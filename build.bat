@echo off

cmake -S . -B build
if errorlevel 1 exit /b 1

cmake --build build
if errorlevel 1 exit /b 1

build\CapitalAlien\CapitalAlien.exe

echo.
echo Build concluido!