# Soundie
This is a test of the FFMPEG library in generating .mov files.

# Building and Running

First, ensure that conan, cmake, and PkgConfig are installed.

Then, install dependencies using:
```
conan install . --output-folder=build --build=missing
```

Then, set the build directory as the `PKG_CONFIG_PATH` environment variable.

Then, navigate to the build directory:
```
cd build
```

and generate build scripts:
```
cmake ..
```

and execute the build itself:
```
cmake --build . --config Release
```

Finally, navigate back to the root and run the program!
```
cd ..
./build/Release/Soundie.exe
```

This will generate a .mov file to your root directory.
