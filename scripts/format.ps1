# Check if clang-format is available
if (-not (Get-Command clang-format -ErrorAction SilentlyContinue)) {
    Write-Error "Error: clang-format not found in PATH"
    exit 1
}

# Get the project root directory
$ProjectRoot = Resolve-Path (Join-Path $PSScriptRoot "..")

Write-Host "Running clang-format on C/C++ files..."

# Find and format all C/C++ source files
$files = Get-ChildItem -Path (Join-Path $ProjectRoot "src"), (Join-Path $ProjectRoot "include") -Recurse -Include "*.c", "*.h", "*.cpp", "*.hpp"
if ($files) {
    clang-format @args $files.FullName
}

Write-Host "clang-format completed successfully!"
