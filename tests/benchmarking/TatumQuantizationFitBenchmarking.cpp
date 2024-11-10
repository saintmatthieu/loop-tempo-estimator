#include "LoopTempoEstimator/LoopTempoEstimator.h"
#include "LteFakes.h"
#include "LteTestUtils.h"
#include "TestLteAudioReader.h"

#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace LTE
{
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
   // `TatumQuantizationFitBenchmarkingOutput/thresholds.txt`.
   //
   // The AUC can only be used for comparison if the hash doesn't change. At the
   // time of writing, the benchmarking can only conveniently be run on the
   // author's machine (Windows), because the files used are not
   // redistributable. Setting up a redistributable dataset that can be used by
   // other developers is currently being worked on.

   // We only observe the results for the most lenient classifier. The other,
   // stricter classifier will yield the same results, only with fewer false
   // positives.

   constexpr auto lenientTolerance = FalsePositiveTolerance::Lenient;
   constexpr int progressBarWidth = 50;
   const auto audioFiles = GetBenchmarkingAudioFiles();
   std::stringstream sampleValueCsv;
   sampleValueCsv
      << "truth,score,isSingleEvent,kurtosis,tatumRate,bpm,ts,octaveFactor,octaveError,lag,filename\n";

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
   std::transform(
      audioFiles.begin(), audioFiles.begin() + numFiles,
      std::back_inserter(samples),
      [&](const std::string& audioFile)
      {
         const TestLteAudioReader audio { audioFile };

         checksum += GetChecksum(audio);
         QuantizationFitDebugOutput debugOutput;
         std::function<void(double)> progressCb;
         GetBpm(audio, lenientTolerance, progressCb, &debugOutput);

         ProgressBar(progressBarWidth, 100 * count++ / numFiles);
         const auto expected = GetBpmFromFilename(audioFile);
         const auto truth = expected.has_value();
         const std::optional<OctaveError> error =
            truth && debugOutput.bpm > 0 ?
               std::make_optional(GetOctaveError(*expected, debugOutput.bpm)) :
               std::nullopt;
         sampleValueCsv << (truth ? "true" : "false") << ","
                        << debugOutput.score << ","
                        << (debugOutput.isSingleEvent ? "true" : "false") << ","
                        << debugOutput.kurtosis << ","
                        << 60. * debugOutput.tatumQuantization.tatumCount /
                              debugOutput.audioFileDuration
                        << "," << debugOutput.bpm << ","
                        << (error.has_value() ? error->factor : 0.) << ","
                        << (error.has_value() ? error->remainder : 0.) << ","
                        << debugOutput.tatumQuantization.lag << ","
                        << Pretty(audioFile) << "\n";
         return Sample { truth, debugOutput.score, error };
      });

   // AUC of ROC curve. Tells how good our loop/not-loop clasifier is.
   const auto rocInfo = GetRocInfo(
      samples,
      loopClassifierSettings.at(lenientTolerance).allowedFalsePositiveRate);

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
   constexpr auto previousChecksum = -177205.328125f;
   REQUIRE(
      ValueIsUnchanged(outputDir / "checksum.txt", previousChecksum, checksum));

   constexpr auto previousAuc = 0.9124489795918365;
   constexpr auto comparisonTolerance = 0.01;
   const auto classifierQualityIsUnchanged = ValueIsUnchanged(
      outputDir / "AUC.txt", previousAuc, rocInfo.areaUnderCurve,
      comparisonTolerance);

   // If the algorithm has changed, we need to update the thresholds, too, for
   // the algorithm to meet its false-positive requirement.
   if (!classifierQualityIsUnchanged)
   {
      std::ofstream summaryFile { outputDir / "thresholds.txt" };
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
