$buildDirectory = Join-Path -Path $PSScriptRoot -ChildPath "build"

if (Test-Path -Path $buildDirectory) {
    Remove-Item -Recurse -Force -Path $buildDirectory
    Write-Host "Cleaned '$buildDirectory'."
} else {
    Write-Host "Nothing to clean, '$buildDirectory' already does not exist."
}
