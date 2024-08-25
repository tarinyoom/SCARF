$buildDirectory = Join-Path -Path $PSScriptRoot -ChildPath "build"

clang-format --style=Google src/*.cpp src/*.hpp src/*.tpp -i

if (-Not (Test-Path -Path $buildDirectory)) {
    New-Item -ItemType Directory -Path $buildDirectory
}
$env:PKG_CONFIG_PATH = $buildDirectory
conan install . --output-folder=build --build=missing

# Perform the cmake build
$originalDirectory = Get-Location
try {
    Set-Location -Path $buildDirectory
	cmake ..
	cmake --build . --config Release
}
finally {
    Set-Location -Path $originalDirectory
}
