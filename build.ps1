# RoChatPlus Build Script for Windows
# Usage: .\build.ps1

$projectRoot = "C:\Users\Valen\VSC Projects\chat app"
$buildDir = Join-Path $projectRoot "build"

Write-Host "RoChatPlus Build Script" -ForegroundColor Cyan
Write-Host "================================" -ForegroundColor Cyan
Write-Host ""

# Check cmake
if (-not (Get-Command cmake -ErrorAction SilentlyContinue)) {
    Write-Host "ERROR: cmake not found on PATH" -ForegroundColor Red
    exit 1
}
Write-Host "OK: cmake found" -ForegroundColor Green

# Check for MSVC
$vcVarsBat = "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvarsall.bat"
$useMSVC = $false

if (Test-Path $vcVarsBat) {
    Write-Host "OK: MSVC 2022 found" -ForegroundColor Green
    Write-Host "Loading MSVC environment..." -ForegroundColor Yellow
    
    $envOutput = & cmd /c "`"$vcVarsBat`" x64 && set"
    foreach ($line in $envOutput) {
        if ($line -match '^(.*?)=(.*)$') {
            [System.Environment]::SetEnvironmentVariable($matches[1], $matches[2])
        }
    }
    $generator = "Visual Studio 17 2022"
    $useMSVC = $true
    Write-Host "OK: MSVC environment loaded" -ForegroundColor Green
} else {
    Write-Host "MSVC not found, using MinGW" -ForegroundColor Yellow
    $generator = "MinGW Makefiles"
}

Write-Host ""
Write-Host "Removing old build directory..." -ForegroundColor Yellow
if (Test-Path $buildDir) {
    Remove-Item -Recurse -Force $buildDir
}
New-Item -ItemType Directory -Path $buildDir | Out-Null
Write-Host "OK: Build directory ready" -ForegroundColor Green

Write-Host ""
Write-Host "Configuring with CMake..." -ForegroundColor Yellow
Push-Location $buildDir

if ($useMSVC) {
    cmake .. -G "$generator" -A x64
} else {
    cmake .. -G "$generator"
}

if ($LASTEXITCODE -ne 0) {
    Write-Host "ERROR: CMake configure failed" -ForegroundColor Red
    Pop-Location
    exit 1
}
Write-Host "OK: CMake configure done" -ForegroundColor Green

Write-Host ""
Write-Host "Building project (this may take a few minutes)..." -ForegroundColor Yellow

if ($useMSVC) {
    cmake --build . --config Release
} else {
    cmake --build .
}

if ($LASTEXITCODE -ne 0) {
    Write-Host "ERROR: Build failed" -ForegroundColor Red
    Pop-Location
    exit 1
}
Write-Host "OK: Build successful!" -ForegroundColor Green

Pop-Location

Write-Host ""
Write-Host "================================" -ForegroundColor Cyan
Write-Host "Build Complete" -ForegroundColor Cyan
Write-Host "================================" -ForegroundColor Cyan
Write-Host "Executable location:"
if ($useMSVC) {
    Write-Host "  $buildDir\Release\RoChatPlus.exe" -ForegroundColor White
} else {
    Write-Host "  $buildDir\RoChatPlus.exe" -ForegroundColor White
}
Write-Host ""
Write-Host "Next: Run windeployqt to bundle Qt DLLs" -ForegroundColor Yellow
Write-Host ""
Read-Host "Press Enter to exit"
