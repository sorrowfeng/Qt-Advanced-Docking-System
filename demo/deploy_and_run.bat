@echo off
setlocal

set QT_DIR=C:\Qt\6.9.0\msvc2022_64
set BUILD_DIR=%~dp0..\build\x64\bin\Release
set EXE=%BUILD_DIR%\AdvancedDockingSystemDemo.exe

if not exist "%EXE%" (
    echo Demo not built. Build Release first:
    echo   cmake --build build --config Release --target AdvancedDockingSystemDemo
    exit /b 1
)

if not exist "%QT_DIR%\bin\windeployqt.exe" (
    echo windeployqt not found at %QT_DIR%\bin\windeployqt.exe
    echo Set QT_DIR in this script to your Qt MSVC path.
    exit /b 1
)

echo Deploying Qt runtime to %BUILD_DIR% ...
"%QT_DIR%\bin\windeployqt.exe" --release --no-translations "%EXE%"
if errorlevel 1 exit /b 1

echo Starting demo ...
start "" "%EXE%"
