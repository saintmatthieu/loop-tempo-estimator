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

## Visualization

Some visualization of some aspects of the algorithm was implemented: `TatumQuantizationFitVisualization.cpp` takes a wav file as input, runs the algorithm with debug output, and writes this output to python files, which can afterwards be run to show plots.

1. Set `static constexpr auto runLocally = true;` in MirTestUtils.h (should be committed as `false` to avoid CI running visualization and benchmarking)
2. In `TatumQuantizationFitVisualization.cpp`, Set the value of `const auto wavFile =` to the path of the wav file you want to analyze.
3. Build and run the `lib-loop-tempo-estimator-test` target. (The benchmarking will run and fail if you haven't set it up, but you can ignore it.)
4. In `tests/TatumQuantizationFitVisualization/`, you will find `visualize_debug_output.py` and `visualize_post-processed_STFT.py`. You can execute them using Python.