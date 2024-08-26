$execPath = Join-Path -Path $PSScriptRoot -ChildPath "build\tests\Release\scarftest.exe"

if (Test-Path -Path $execPath) {
    & $execPath $args
} else {
    Write-Host "Executable not found at '$execPath'. Build the executable first before running it."
}
