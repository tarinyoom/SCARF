clang-format --version

# Recursively format all .hpp, .tpp, and .cpp files in the 'lib' directory.
$files = Get-ChildItem -Path "lib" -Recurse -File | Where-Object { $_.Extension -in (".hpp", ".tpp", ".cpp") }
foreach ($file in $files) {
	& clang-format --style=Google $file.FullName $args
}
