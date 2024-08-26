# SCARF
A second crack at rendering fluids.
This project is a first-principles view of simulating and rendering fluids.
It has only one dependency, which is FFMPEG, for taking pixel values and converting them to an output .mov file.

# Building and Running
This project has the following requirements:
- cmake >= 3.17
- pkgconfiglite >= 0.28
- conan >= 2.4.1

To build the project, use:

```.\make.ps1```

To run the project, use:

```.\run.ps1```

See the `.github/workflows/windows-build.yaml` file for an example of building this project on a github actions runner.
