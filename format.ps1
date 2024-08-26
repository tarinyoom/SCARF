clang-format --version

# Define the directories to search
$directories = @("lib", "tests", "tools")

# Get all files with the specified extensions in the defined directories
$files = $directories | ForEach-Object {
    Get-ChildItem -Path $_ -Recurse -File | Where-Object { $_.Extension -in (".hpp", ".tpp", ".cpp") }
}

# Format each file using clang-format
foreach ($file in $files) {
    & clang-format --style=Google $file.FullName $args
}
