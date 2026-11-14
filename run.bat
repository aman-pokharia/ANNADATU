@echo off
if not exist annadatu.exe (
    echo Binary annadatu.exe not found. Running build.bat first...
    call build.bat
)

if exist annadatu.exe (
    echo Starting ANNADATU Application...
    annadatu.exe
)
