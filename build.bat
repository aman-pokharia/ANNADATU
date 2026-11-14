@echo off
setlocal enabledelayedexpansion

echo =======================================================
echo     Compiling ANNADATU - C++ OOPS Project
echo     Authors: Aman Pokharia, Roshan Choudhary, Khushi Gupta
echo =======================================================

:: Check if cl is already in PATH
where cl >nul 2>nul
if %errorlevel% neq 0 (
    echo Setting up Visual Studio MSVC 64-bit environment...
    if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" (
        call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" >nul
    ) else if exist "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat" (
        call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat" >nul
    ) else (
        echo [ERROR] MSVC compiler environment not found!
        pause
        exit /b 1
    )
)

echo Compiling C++ source files...
cl /EHsc /std:c++17 /W3 /O2 /Iinclude /Fe:"annadatu.exe" /Fo:".\\" src\*.cpp

if %errorlevel% equ 0 (
    echo.
    echo =======================================================
    echo   [SUCCESS] Compilation Complete! Output: annadatu.exe
    echo =======================================================
    del *.obj 2>nul
) else (
    echo.
    echo   [ERROR] Compilation failed! Check the compiler errors above.
)
