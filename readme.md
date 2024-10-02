# Loop Tempo Estimation library

[Upcoming talk at the 2024 Audio Developer Conference](https://conference.audio.dev/session/2024/an-efficient-open-source-c-loop-classifier-and-tempo-estimator/)

![An Efficient, Open-Source C++ Loop Classifier and Tempo Estimator](./An%20Efficient,%20Open-Source%20C++%20Loop%20Classifier%20and%20Tempo%20Estimator_TitleCard.png)

## Building

Build the project using CMake:

```sh
mkdir build
cd build
cmake .. -DBUILD_TESTS=ON -DUSE_LIBSNDFILE=ON
cmake --build .
```

#### `BUILD_TESTS`
Defaults to `ON`.<br/>If you want to consume this library in your own project, you'll probably want to set it to `OFF`.

#### `USE_LIBSNDFILE`
Relevant if `BUILD_TESTS` is set to `ON`.<br/>
The test framework uses minimp3 for to read mp3 files. If you want to run the algorithm on some wav file, either convert it to mp3 first, or have [libsnfile](https://github.com/libsndfile/libsndfile) installed and set `USE_LIBSNDFILE` to `ON`.

## Usage

The main contribution of this repo is just a library, not an executable.

However, the project includes a benchmarking framework as well as visualization tools. If you want to make sure that the
code works, you may want to look at `TatumQuantizationFitBenchmarking.cpp` and `TatumQuantizationFitVisualization.cpp`.

## Visualization

Some visualization of some aspects of the algorithm was implemented: `TatumQuantizationFitVisualization.cpp` takes a wav
file as input, runs the algorithm with debug output, and writes this output to python files, which can afterwards be run
to show plots.

1. Set `static constexpr auto runLocally = true;` in LteTestUtils.h (should be committed as `false` to avoid CI running
   visualization and benchmarking)
2. In `TatumQuantizationFitVisualization.cpp`, Set the value of `const auto wavFile =` to the path of the wav file you
   want to analyze.
3. Build and run the `lib-loop-tempo-estimator-test` target. (The benchmarking will run and fail if you haven't set it
   up, but you can ignore it.)
4. In `tests/TatumQuantizationFitVisualization/`, you will find `visualize_debug_output.py` and
   `visualize_post-processed_STFT.py`. You can execute them using Python.

E.g. ![image](https://github.com/user-attachments/assets/1e9fe296-cad7-4ad1-a5af-b6dd43c4156c) shows the onset detection
function (blue), the onsets (red) and the tatums (green) as estimated by the algorithm on a 16-bar drum loop.
