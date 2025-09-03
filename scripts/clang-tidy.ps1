# Check if clang-tidy is available
if (-not (Get-Command clang-tidy -ErrorAction SilentlyContinue)) {
    Write-Error "Error: clang-tidy not found in PATH"
    exit 1
}

Write-Host "Running clang-tidy..."

# Get the project root directory
$ProjectRoot = Resolve-Path (Join-Path $PSScriptRoot "..")
$buildDir = Join-Path $ProjectRoot "build"

clang-tidy -p $buildDir @args
