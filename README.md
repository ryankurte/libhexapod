# Libhexapod

A library for hexapod control.

## Usage

Include [lib/hexapod.cmake](lib/hexapod.cmake) in your CMake project to build the library and add it to an OPTIONAL_LIBS variable. Check out [CMakeLists.txt](CMakeLists.txt) for a working example.

## Status

Forward and inverse kinematics as well as linear (no rotational) gait control working in simulation, not yet physically tested. Body translation is not yet tested.

[![Build Status](https://travis-ci.org/ryankurte/libhexapod.svg?branch=master)](https://travis-ci.org/ryankurte/libhexapod)
[![Release](https://img.shields.io/github/release/ryankurte/libhexapod.svg)](https://github.com/ryankurte/libhexapod)

## Dependencies

- cmake
- make
- gcc (or clang)

Development also requires python3 with numpy and matplotlib.

## Development

1. `git clone github.com:ryankurte/libhexapod.git` to clone the repository 
2. `mkdir build` to create a build directory
3. `cd build` to switch to the build directory 
4. `cmake ..` to initialise cmake 
5. `make` to build the project
6. `./hex-test` to run unit tests
7. `./hex-util` to generate output files
8. `../graph.py` to render output files


------

If you have any questions, comments, or suggestions, feel free to open an issue or a pull request.
