@echo off

if "%1" == "build" goto build
if "%1" == "run" goto run
if "%1" == "clear" goto clear

echo Invalid argument. Use "build", "run", or "clear".
goto :eof

:build
    echo Building ...
    call cl /nologo /EHsc src/main.cpp
    goto :eof

:run
    echo Executing ...
    call main.exe
    goto :eof

:clear
    echo Clearing executables and obj files ...
    del main.exe
    del main.obj
    del main.pdb
    del main.ilk
    goto :eof

:eof