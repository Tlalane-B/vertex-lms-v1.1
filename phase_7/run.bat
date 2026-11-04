@echo off
title Vertex LMS Launcher
color 0A
cls

echo ===========================================
echo    Vertex LMS - Starting Application...
echo ===========================================

cd /d "%~dp0"

if not exist build (
    echo [INFO] Creating build directory...
    mkdir build
)

cd build

echo [INFO] Configuring project with CMake...
cmake .. -G "Visual Studio 17 2022" -A x64

echo [INFO] Building release executable...
cmake --build . --config Release

if exist Release\VertexLMS.exe (
    echo [SUCCESS] Launching Vertex LMS...
    start "" "Release\VertexLMS.exe"
) else if exist VertexLMS.exe (
    echo [SUCCESS] Launching Vertex LMS...
    start "" "VertexLMS.exe"
) else (
    echo [ERROR] Build failed or executable not found.
    pause
)
