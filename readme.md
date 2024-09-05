# Loop Tempo Estimation library

[Upcoming talk at the 2024 Audio Developer Conference](https://conference.audio.dev/session/2024/an-efficient-open-source-c-loop-classifier-and-tempo-estimator/)

## Building

This project uses CMake as its build system. To build the project, you must first update the submodules:

```sh
git submodule update --init --recursive
```

Then, you can build the project using CMake:

```sh
mkdir build
cd build
cmake ..
cmake --build .
```

## Usage

The main contribution of this repo is just a library, not an executable. It is in the loop-tempo-estimator directory and the entry-point header file is `MusicInformationRetrieval.h`.

However, the project includes a benchmarking framework as well as visualization tools. If you want to make sure that the code works, you may want to look at `TatumQuantizationFitBenchmarking.cpp` and `TatumQuantizationFitVisualization.cpp`.
