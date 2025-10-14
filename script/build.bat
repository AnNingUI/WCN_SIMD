@echo off
REM WCN_SIMD Build Script for Windows with Ninja

setlocal

set BUILD_DIR=build
set BUILD_TYPE=Release
set GENERATOR=Ninja

echo ================================================
echo WCN_SIMD Build Script
echo ================================================
echo.

REM Parse command line arguments
:parse_args
if "%~1"=="" goto check_tools
if /i "%~1"=="clean" (
    echo Cleaning build directory...
    if exist "%BUILD_DIR%" rd /s /q "%BUILD_DIR%"
    shift
    goto parse_args
)
if /i "%~1"=="debug" (
    set BUILD_TYPE=Debug
    shift
    goto parse_args
)
if /i "%~1"=="msbuild" (
    set GENERATOR=Visual Studio 17 2022
    shift
    goto parse_args
)
shift
goto parse_args

:check_tools
echo Checking required tools...
cmake --version >nul 2>&1
if errorlevel 1 (
    echo ERROR: CMake not found. Please install CMake.
    exit /b 1
)

if "%GENERATOR%"=="Ninja" (
    ninja --version >nul 2>&1
    if errorlevel 1 (
        echo ERROR: Ninja not found. Please install Ninja or use 'build.bat msbuild'
        exit /b 1
    )
)

echo Build configuration:
echo   Generator: %GENERATOR%
echo   Build Type: %BUILD_TYPE%
echo.

REM Create build directory
if not exist "%BUILD_DIR%" (
    echo Creating build directory...
    mkdir "%BUILD_DIR%"
)

REM Configure with CMake
echo Configuring project with CMake...
cd "%BUILD_DIR%"
cmake -G "%GENERATOR%" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ..
if errorlevel 1 (
    echo ERROR: CMake configuration failed
    cd ..
    exit /b 1
)

REM Build
echo.
echo Building project...
if "%GENERATOR%"=="Ninja" (
    ninja
) else (
    cmake --build . --config %BUILD_TYPE%
)

if errorlevel 1 (
    echo ERROR: Build failed
    cd ..
    exit /b 1
)

cd ..

REM Copy compile_commands.json for clangd/IDE support
if exist "%BUILD_DIR%\compile_commands.json" (
    copy /Y "%BUILD_DIR%\compile_commands.json" "compile_commands.json" >nul 2>&1
    echo Copied compile_commands.json for IDE support
)

echo.
echo ================================================
echo Build completed successfully!
echo ================================================
echo.
echo Executable location:
if "%GENERATOR%"=="Ninja" (
    echo   %BUILD_DIR%\bin\wcn_simd_example.exe
) else (
    echo   %BUILD_DIR%\bin\%BUILD_TYPE%\wcn_simd_example.exe
)
echo.
echo Run example: 
if "%GENERATOR%"=="Ninja" (
    echo   %BUILD_DIR%\bin\wcn_simd_example.exe
) else (
    echo   %BUILD_DIR%\bin\%BUILD_TYPE%\wcn_simd_example.exe
)

endlocal
