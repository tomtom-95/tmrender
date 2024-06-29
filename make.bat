@echo off

set "VCVARSALL_PATH=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat"
set "COMPILER_PATH=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.40.33807\bin\Hostx64\x86\cl.exe"
set "SOURCE_FILE=.\src\main.cpp"

if "%1" == "test" goto test
if "%1" == "build32" goto build32
REM TODO(tommaso): one day I will build a 64 executable, one day ...
if "%1" == "build64" goto eof
if "%1" == "run" goto run
if "%1" == "clear" goto clear

echo Invalid argument. Use "build32", "build64", "run", or "clear".
goto :eof

REM I give up for now, no 64 bit application
REM TODO(tommaso): see Casey and how he deals with batch file
:build32
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