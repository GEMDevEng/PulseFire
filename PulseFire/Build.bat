@echo off
REM PulseFire Build Script
REM This script automates the build process for PulseFire

REM Set default values
set PLATFORM=Win64
set CONFIGURATION=Development
set ARCHIVE=false
set CLEAN=false
set DOCS=false
set TESTS=false

REM Parse command line arguments
:parse_args
if "%~1"=="" goto :end_parse_args
if /i "%~1"=="-p" (
    set PLATFORM=%~2
    shift
    shift
    goto :parse_args
)
if /i "%~1"=="--platform" (
    set PLATFORM=%~2
    shift
    shift
    goto :parse_args
)
if /i "%~1"=="-c" (
    set CONFIGURATION=%~2
    shift
    shift
    goto :parse_args
)
if /i "%~1"=="--configuration" (
    set CONFIGURATION=%~2
    shift
    shift
    goto :parse_args
)
if /i "%~1"=="-a" (
    set ARCHIVE=true
    shift
    goto :parse_args
)
if /i "%~1"=="--archive" (
    set ARCHIVE=true
    shift
    goto :parse_args
)
if /i "%~1"=="--clean" (
    set CLEAN=true
    shift
    goto :parse_args
)
if /i "%~1"=="--docs" (
    set DOCS=true
    shift
    goto :parse_args
)
if /i "%~1"=="--tests" (
    set TESTS=true
    shift
    goto :parse_args
)
if /i "%~1"=="-h" (
    goto :show_help
)
if /i "%~1"=="--help" (
    goto :show_help
)
echo Unknown option: %~1
echo Use --help for usage information
exit /b 1

:show_help
echo PulseFire Build Script
echo Usage: Build.bat [options]
echo.
echo Options:
echo   -p, --platform PLATFORM      Target platform (Win64, Mac, Linux)
echo   -c, --configuration CONFIG   Build configuration (Development, Shipping, Debug)
echo   -a, --archive                Create a distributable archive
echo       --clean                  Clean build artifacts before building
echo       --docs                   Generate documentation
echo       --tests                  Run tests
echo   -h, --help                   Show this help message
exit /b 0

:end_parse_args

REM Set UE4 path
set UE_PATH="C:\Program Files\Epic Games\UE_5.4\Engine\Build\BatchFiles\Build.bat"

REM Set output directory
set OUTPUT_DIR=Build\%PLATFORM%\%CONFIGURATION%
if not exist %OUTPUT_DIR% mkdir %OUTPUT_DIR%

REM Clean build artifacts if requested
if "%CLEAN%"=="true" (
    echo Cleaning build artifacts...
    if exist Binaries rmdir /s /q Binaries
    if exist Intermediate rmdir /s /q Intermediate
    if exist Build rmdir /s /q Build
    mkdir %OUTPUT_DIR%
)

REM Build the project
echo Building PulseFire for %PLATFORM% (%CONFIGURATION%)...
call %UE_PATH% PulseFireEditor %PLATFORM% %CONFIGURATION% -project="%CD%\PulseFire.uproject"
if %ERRORLEVEL% neq 0 (
    echo Build failed!
    exit /b 1
)

call %UE_PATH% PulseFire %PLATFORM% %CONFIGURATION% -project="%CD%\PulseFire.uproject"
if %ERRORLEVEL% neq 0 (
    echo Build failed!
    exit /b 1
)

REM Run tests if requested
if "%TESTS%"=="true" (
    echo Running tests...
    REM Add test command here
)

REM Generate documentation if requested
if "%DOCS%"=="true" (
    echo Generating documentation...
    REM Add documentation generation command here
)

REM Create archive if requested
if "%ARCHIVE%"=="true" (
    echo Creating archive...
    set ARCHIVE_NAME=PulseFire-%PLATFORM%-%CONFIGURATION%.zip
    powershell Compress-Archive -Path Binaries,Content -DestinationPath "%OUTPUT_DIR%\%ARCHIVE_NAME%" -Force
    echo Archive created at %OUTPUT_DIR%\%ARCHIVE_NAME%
)

echo Build completed successfully!
exit /b 0
