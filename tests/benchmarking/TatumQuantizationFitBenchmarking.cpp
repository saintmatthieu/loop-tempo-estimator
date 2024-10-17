#include "LoopTempoEstimator/LoopTempoEstimator.h"
#include "LteFakes.h"
#include "LteTestUtils.h"
#include "TestLteAudioReader.h"

#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace LTE
{
namespace
{
const auto datasetRoot =
   std::string(CMAKE_SOURCE_DIR) + "/tests/benchmarking/dataset";

std::vector<std::string> GetBenchmarkingAudioFiles()
{
   const auto pythonScriptPath =
      std::string(CMAKE_SOURCE_DIR) +
      "/tests/benchmarking/download-benchmarking-dataset.py";
   const auto command = "python " + pythonScriptPath + " " + datasetRoot;
   const auto returnCode = system(command.c_str());
   if (returnCode != 0)
      throw std::runtime_error("Failed to download benchmarking dataset!");

   std::vector<std::string> files;
   namespace fs = std::filesystem;
   for (const auto& entry : fs::directory_iterator(datasetRoot))
      for (const auto& subEntry : fs::recursive_directory_iterator(entry))
         if (
            subEntry.is_regular_file() && subEntry.path().extension() == ".mp3")
            files.push_back(subEntry.path().string());
   std::sort(files.begin(), files.end());
   return files;
}

std::string Pretty(const std::string& filename)
{
   // Remove the dataset root from the filename ...
   const auto datasetRootLength = datasetRoot.length();
   auto tmp = filename.substr(datasetRootLength + 1);
   // ... and now the .mp3 extension:
   tmp = tmp.substr(0, tmp.length() - 4);
   // Replace backslashes with forward slashes:
   std::replace(tmp.begin(), tmp.end(), '\\', '/');
   return tmp;
}

bool isMP3File(const std::string& filename)
{
   return filename.substr(filename.find_last_of(".") + 1) == "mp3";
}

} // namespace

TEST_CASE("GetRocInfo")
{
   // We use the AUC as a measure of the classifier's performance. With a
   // suitable data set, this helps us detect regressions, and guide fine-tuning
   // of the algorithm. This test should help understand how it works and also
   // make sure that we've implemented that metric correctly :)

   struct Sample
   {
      bool truth;
      double score;
   };

   using Samples = std::vector<Sample>;

   struct Expected
   {
      double areaUnderCurve;
      double threshold;
   };

   struct TestCase
   {
      const Samples samples;
      const double allowedFalsePositiveRate;
      const Expected expected;
   };

   const std::vector<TestCase> testCases {
      // Classifier is upside down. We don't tolerate false positives. The
      // returned threshold is then 100 will satisfy this, but the TPR will also
      // be 0 ...
      TestCase { Samples { { true, 100. }, { false, 200. } }, 0.,
                 Expected { 0., 200. } },

      // Classifier is still upside down. We'll get true positives only if we
      // accept an FPR of 1.
      TestCase { Samples { { true, 100. }, { false, 200. } }, 1.,
                 Expected { 0., 100. } },

      // Now we have a classifier that works. We don't accept false positives.
      TestCase { Samples { { false, 100. }, { false, 150. }, { true, 200. } },
                 0., Expected { 1., 175. } },

      // A random classifier, which should have an AUC of 0.75.
      TestCase {
         Samples { { false, 1. }, { true, 2. }, { false, 3. }, { true, 4. } },
         0.5, Expected { .75, 1.5 } },
   };

   for (const auto& testCase : testCases)
   {
      const auto roc =
         GetRocInfo(testCase.samples, testCase.allowedFalsePositiveRate);
      REQUIRE(roc.areaUnderCurve == testCase.expected.areaUnderCurve);
      REQUIRE(roc.threshold == testCase.expected.threshold);
   }
}

TEST_CASE("GetChecksum")
{
   constexpr auto bufferSize = 5;
   const auto checksum = GetChecksum<bufferSize>(SquareWaveLteAudioReader {});
   REQUIRE(checksum == 0.);
}

namespace
{
constexpr auto precision = std::numeric_limits<double>::digits10 + 1;

template <typename T>
bool ValueIsUnchanged(
   const std::filesystem::path& filePath, T previousValue, T newValue,
   T tolerance = 0)
{
   REQUIRE(std::filesystem::exists(filePath));
   const auto hasChanged = std::abs(newValue - previousValue) > tolerance;
   if (hasChanged)
   {
      std::ofstream file { filePath };
      file << std::setprecision(precision) << newValue;
   }
   return !hasChanged;
}
} // namespace

TEST_CASE("TatumQuantizationFitBenchmarking")
{
   // For this test to run, you will need to set `runLocally` to `true`, you'll
   // need the benchmarking sound files. To get these, just run
   // `download-benchmarking-dataset.py`. This will download the necessary mp3
   // files and place them in the `benchmarking-dataset` directory under this
   // directory.

   // Running this test will update
   // `TatumQuantizationFitBenchmarkingOutput/summary.txt`. The summary contains
   //
   // 1. the AUC metric for regression-testing,
   // 2. the strict- and lenient-mode thresholds,
   // 3. the octave-error RMS (Schreiber, H., et al. (2020)), and
   // 4. the hash of the audio files used.
   //
   // The AUC can only be used for comparison if the hash doesn't change. At the
   // time of writing, the benchmarking can only conveniently be run on the
   // author's machine (Windows), because the files used are not
   // redistributable. Setting up a redistributable dataset that can be used by
   // other developers is currently being worked on.

   // We only observe the results for the most lenient classifier. The other,
   // stricter classifier will yield the same results, only with fewer false
   // positives.

   constexpr auto tolerance = FalsePositiveTolerance::Lenient;
   constexpr int progressBarWidth = 50;
   const auto audioFiles = GetBenchmarkingAudioFiles();
   std::stringstream sampleValueCsv;
   sampleValueCsv
      << "truth,score,tatumRate,bpm,ts,octaveFactor,octaveError,lag,filename\n";

   float checksum = 0.f;
   struct Sample
   {
      bool truth;
      double score;
      std::optional<OctaveError> octaveError;
   };
   std::vector<Sample> samples;
   const auto numFiles = audioFiles.size();
   auto count = 0;
   std::chrono::milliseconds computationTime { 0 };
   std::transform(
      audioFiles.begin(), audioFiles.begin() + numFiles,
      std::back_inserter(samples),
      [&](const std::string& audioFile)
      {
         const TestLteAudioReader audio { audioFile };

         checksum += GetChecksum(audio);
         QuantizationFitDebugOutput debugOutput;
         std::function<void(double)> progressCb;
         const auto now = std::chrono::steady_clock::now();
         GetBpmFromSignal(audio, tolerance, progressCb, &debugOutput);

         computationTime +=
            std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::steady_clock::now() - now);
         ProgressBar(progressBarWidth, 100 * count++ / numFiles);
         const auto expected = GetBpmFromFilename(audioFile);
         const auto truth = expected.has_value();
         const std::optional<OctaveError> error =
            truth && debugOutput.bpm > 0 ?
               std::make_optional(GetOctaveError(*expected, debugOutput.bpm)) :
               std::nullopt;
         sampleValueCsv << (truth ? "true" : "false") << ","
                        << debugOutput.score << ","
                        << 60. * debugOutput.tatumQuantization.numDivisions /
                              debugOutput.audioFileDuration
                        << "," << debugOutput.bpm << ","
                        << (error.has_value() ? error->factor : 0.) << ","
                        << (error.has_value() ? error->remainder : 0.) << ","
                        << debugOutput.tatumQuantization.lag << ","
                        << Pretty(audioFile) << "\n";
         return Sample { truth, debugOutput.score, error };
      });

   {
      std::ofstream timeMeasurementFile { "./timeMeasurement.txt" };
      timeMeasurementFile << computationTime.count() << "ms\n";
   }

   // AUC of ROC curve. Tells how good our loop/not-loop clasifier is.
   const auto rocInfo = GetRocInfo(
      samples, loopClassifierSettings.at(tolerance).allowedFalsePositiveRate);

   const auto strictThreshold =
      GetRocInfo(
         samples, loopClassifierSettings.at(FalsePositiveTolerance::Strict)
                     .allowedFalsePositiveRate)
         .threshold;

   // Get RMS of octave errors. Tells how good the BPM estimation is.
   const auto octaveErrors = std::accumulate(
      samples.begin(), samples.end(), std::vector<double> {},
      [&](std::vector<double> octaveErrors, const Sample& sample)
      {
         if (sample.octaveError.has_value())
            octaveErrors.push_back(sample.octaveError->remainder);
         return octaveErrors;
      });
   const auto octaveErrorStd = std::sqrt(
      std::accumulate(
         octaveErrors.begin(), octaveErrors.end(), 0.,
         [&](double sum, double octaveError)
         { return sum + octaveError * octaveError; }) /
      octaveErrors.size());

   const auto outputDir { std::filesystem::path(__FILE__).parent_path() /
                          "TatumQuantizationFitBenchmarkingOutput" };

   // Verify that the input has not changed between this run and the previous
   // one, or peformance of the algorithm can't be compared either.
   constexpr auto previousChecksum = -177205.40625f;
   REQUIRE(
      ValueIsUnchanged(outputDir / "checksum.txt", previousChecksum, checksum));

   constexpr auto previousAuc = 0.943271221532091;
   constexpr auto comparisonTolerance = 0.01;
   const auto classifierQualityIsUnchanged = ValueIsUnchanged(
      outputDir / "AUC.txt", 0.943271221532091, rocInfo.areaUnderCurve,
      comparisonTolerance);

   // If the algorithm has changed, we need to update the thresholds, too, for
   // the algorithm to meet its false-positive requirement.
   if (!classifierQualityIsUnchanged)
   {
      std::ofstream summaryFile { outputDir / "summary.txt" };
      summaryFile << std::setprecision(precision)
                  << "Strict Threshold: " << strictThreshold << "\n"
                  << "Lenient Threshold: " << rocInfo.threshold;
      // Write sampleValueCsv to a file.
      std::ofstream sampleValueCsvFile { outputDir / "sampleValues.csv" };
      sampleValueCsvFile << sampleValueCsv.rdbuf();
   }

   // If this changed, then some non-refactoring code change happened. It may be
   // for the better or for the worst, depending on how the AUC changed. If it
   // decreased, then we probably have a regression. If it increased, then we
   // still have to mark this as the new reference, to make sure we don't loose
   // the improvement. "Vorwärts immer", like my German colleagues used to say.
   REQUIRE(classifierQualityIsUnchanged);
}
} // namespace LTE
