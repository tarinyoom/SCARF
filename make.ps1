$buildDirectory = Join-Path -Path $PSScriptRoot -ChildPath "build"

# Format source files in library
$files = Get-ChildItem -Path "lib" -Recurse -File | Where-Object { $_.Extension -in (".hpp", ".tpp", ".cpp") }
foreach ($file in $files) {
	& clang-format --style=Google -i $file.FullName
}

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
