# Loop Tempo Estimator Benchmarking

This project contains a benchmarking tool for the Loop Tempo Estimator algorithm. It tests the algorithm's performance on a dataset of audio files and generates performance metrics.

## Setup

Run `download-benchmarking-dataset.py`.

## Running the Test

1. Navigate to the build directory
2. Run the test executable
3. The test will run the Loop Tempo Estimator algorithm on each audio file in the dataset.
4. Once complete, the test will generate or update the following files in the `TatumQuantizationFitBenchmarkingOutput` directory:
- `summary.txt`: Contains performance metrics including AUC, thresholds, and octave error RMS
- `sampleValues.csv`: Detailed results for each audio file processed