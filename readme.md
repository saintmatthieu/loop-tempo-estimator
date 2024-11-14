[![GitHub License](https://img.shields.io/github/license/saintmatthieu/loop-tempo-estimator)](https://github.com/saintmatthieu/loop-tempo-estimator/blob/main/LICENSE)
[![Version](https://img.shields.io/github/v/release/saintmatthieu/loop-tempo-estimator)](https://github.com/saintmatthieu/loop-tempo-estimator/releases/latest)
[![GitHub Actions Workflow Status](https://img.shields.io/github/actions/workflow/status/saintmatthieu/loop-tempo-estimator/vamp-plugin.yml?logo=audacity&label=vamp-plugin)](https://github.com/saintmatthieu/loop-tempo-estimator/actions/workflows/vamp-plugin.yml)

# Loop Tempo Estimation library

[Upcoming talk at the 2024 Audio Developer Conference](https://conference.audio.dev/session/2024/an-efficient-open-source-c-loop-classifier-and-tempo-estimator/)

![An Efficient, Open-Source C++ Loop Classifier and Tempo Estimator](./ADC-2024/An%20Efficient%20Open-Source%20C++%20Loop%20Classifier%20and%20Tempo%20Estimator_TitleCard.png)

## Library

The main product of this repository is the `loop-tempo-estimator` library. It only has a dependency on PFFFT, and should be easy to integrate into your project.

## Vamp plugin

Download the latest `loop-tempo-estimator-vamp` release,<br />or build it yourself by following the instructions of the build section.

Please try it, it's fun ! It does not only show the final tempo estimation, you can also visualize the intermediary analysis steps, such as the compressed STFT or the Onset Detection Function. The image below is of [Sonic Visualizer](https://sonicvisualiser.org/) using the `loop-tempo-estimator` plugin :

![Vamp demo](./ADC-2024/sonic-visualiser.png)

### Note

The first time you run the plugin on a given file, you'll run into this error :

<img src="./ADC-2024/sonic-visualiser-layer-generation-failed.png" width=40%>

Please ignore this and run the plugin again. From now on, and until you run the plugin on a new file, it should work.

The reason for this is that Vamp does not communicate to plugins the audio file duration at initialization time. The algorithm, though, depends on the duration of the file, without which it cannot provide the information needed by Sonic Visualser at initialization time.<br />
Workaround: during the first run, the wrapper will count the number of samples given by the host and cache it.<br />
Next time you run the algorithm on the same file, it will find the the value it needs in the cache and should run properly.

## Presentation

The slides of the ADC-2024 presentation of the algorithm are checked in this repo, and you may run then locally.

Installation prerequisites:
* [Git LFS](https://git-lfs.github.com)
* [Node.js](https://nodejs.org/fr)

From the root of the repository, run:

```sh
git lfs pull
npm install # only the first time
npm run presentation
```

## Benchmarking

The algorithm's development was largely test-driven.<br/>
The `benchmarking` target

1. downloads a carefully selected mixture of 140 loops and non-loops from freesound.org,
2. makes a checksum verification to ensure the files haven't changed since the last run,
3. runs the algorithm on all of them,
4. fails if the ROC AUC has changed.

In that sense, failing isn't necessarily bad, but any non-refactoring changes must be scrutinized before approval.

## Performance

The algorithm is fast. Integration of the algorithm in your product should be painless to the user.<br/>In Audacity, we skip the analysis of files larger than 1mn, because it's then unlikely to be a loop anyway.<br/>So how long does the analysis of a 1mn file take?

TL;DR: **15ms**.

This figure was obtained with this repo's `performance` target, a command-line executable that runs the tempo estimation algorithm on a large set of loops and returns a real-time factor.<br/>At the time of writing, it prints

```sh
Number of files: 140
Accumulated duration: 2664.89s
Computation time: 692 ms
3850.99 times real-time
```

on my laptop, with specs:

- Windows 11
- 12th Gen Intel Core i7-12800HX processor
- 32GB of RAM

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
The test framework uses minimp3 for to read mp3 files. If you want to run the algorithm on some wav file, either convert it to mp3 first, or have [libsndfile](https://github.com/libsndfile/libsndfile) installed and set `USE_LIBSNDFILE` to `ON`.
