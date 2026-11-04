@echo off
title Vertex LMS v1.0 - Phase 5 Final Deployment Launcher
color 0A
cls

echo ===================================================
echo    Vertex LMS v1.0 [Phase 5: Production & Final Sign-Off]
echo ===================================================
echo.
echo Select an option:
echo   [1] Launch Vertex LMS Production Executable
echo   [2] Run Automated Verification & Test Suite
echo   [3] Exit
echo.
set /p choice="Enter choice (1-3): "

if "%choice%"=="1" goto run_app
if "%choice%"=="2" goto run_tests
if "%choice%"=="3" goto end

:run_app
cd /d "%~dp0"
if not exist build mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
if exist Release\VertexLMS.exe (
    echo [SUCCESS] Launching Vertex LMS Production Build...
    start "" "Release\VertexLMS.exe"
) else if exist VertexLMS.exe (
    echo [SUCCESS] Launching Vertex LMS...
    start "" "VertexLMS.exe"
) else (
    echo [ERROR] Build failed or executable missing.
    pause
)
goto end

:run_tests
cd /d "%~dp0"
if not exist build mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
echo.
echo ===================================================
echo    Running Phase 5 Final Verification Suite...
echo ===================================================
if exist Release\VertexLMSTests.exe (
    Release\VertexLMSTests.exe
) else if exist VertexLMSTests.exe (
    VertexLMSTests.exe
) else (
    echo [ERROR] Test executable not found.
)
pause
goto end

:end
