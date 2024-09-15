# Loop Tempo Estimator Benchmarking

This project contains a benchmarking tool for the Loop Tempo Estimator algorithm. It tests the algorithm's performance on a dataset of audio files and generates performance metrics.

## Setup

Download the benchmarking dataset:
- Open `download-benchmarking-dataset.html` in a web browser, this will download a zip file containing the audio samples
- Extract the contents of the zip file into a folder named `benchmarking-dataset` in the `tests` directory of the project

## Running the Test

1. Navigate to the build directory
2. Run the test executable
3. The test will run the Loop Tempo Estimator algorithm on each audio file in the dataset.
4. Once complete, the test will generate or update the following files in the `TatumQuantizationFitBenchmarkingOutput` directory:
- `summary.txt`: Contains performance metrics including AUC, thresholds, and octave error RMS
- `sampleValues.csv`: Detailed results for each audio file processed