@echo off

if not exist .\build\bin\Release\basic_usage.exe (
    echo Error: basic_usage.exe not found. Please run build.bat first.
) else (
	.\build\bin\Release\basic_usage.exe
)
