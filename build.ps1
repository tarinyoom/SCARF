# Get the current directory where the script is located
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path

# Create the build directory if it doesn't exist
$buildDir = Join-Path -Path $scriptDir -ChildPath "build"
if (-not (Test-Path $buildDir -PathType Container)) {
    New-Item -ItemType Directory -Path $buildDir | Out-Null
}

# Navigate to the build directory
Set-Location $buildDir

# Call cmake to generate build files
cmake ../src

# Return to main directory
Set-Location ..

cmake --build build
