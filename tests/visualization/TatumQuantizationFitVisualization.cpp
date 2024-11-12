#include "LoopTempoEstimator/LoopTempoEstimator.h"
#include "LteFakes.h"
#include "LteTestUtils.h"
#include "TestLteAudioReader.h"

#include <catch2/catch_test_macros.hpp>
#include <fstream>

namespace LTE
{
namespace
{
void printStft(
   std::ofstream& dst, const std::string& name,
   std::vector<std::vector<float>> stft)
{
   dst << name << " = [";
   std::for_each(
      stft.begin(), stft.end(),
      [&](const auto& row)
      {
         dst << "[";
         std::for_each(
            row.begin(), row.end(), [&](float x) { dst << x << ","; });
         dst << "],";
      });
   dst << "]\n";
}
} // namespace

TEST_CASE("TatumQuantizationFitVisualization")
{
   // This test produces python files containing data. Besides being useful for
   // debugging, after you have run this, you can run
   // `visualize_debug_output.py` to visualize the working of the algorithm, or
   // `visualize_post-processed_STFT.py` to visualize the STFT used to produce
   // the ODF.
   const auto wavFile =
      std::string { CMAKE_SOURCE_DIR } +
      "/tests/audio/[Drumbeats (Melodic)] 120 bpm house loop.mp3";
   const TestLteAudioReader audio { wavFile };
   QuantizationFitDebugOutput debugOutput;
   const auto result =
      GetBpm(audio, FalsePositiveTolerance::Lenient, nullptr, &debugOutput);

   std::ofstream debug_output_module {
      std::string(CMAKE_SOURCE_DIR) +
      "/tests/visualization/TatumQuantizationFitVisualization/debug_output.py"
   };
   debug_output_module << "wavFile = \"" << wavFile << "\"\n";
   debug_output_module << "odfSr = " << debugOutput.odfSr << "\n";
   debug_output_module << "audioFileDuration = "
                       << debugOutput.audioFileDuration << "\n";
   debug_output_module << "score = " << debugOutput.score << "\n";
   debug_output_module << "tatumRate = "
                       << 60. * debugOutput.tatumQuantization.tatumCount /
                             debugOutput.audioFileDuration
                       << "\n";
   debug_output_module << "bpm = " << (result.value_or(0.)) << "\n";
   debug_output_module << "lag = " << debugOutput.tatumQuantization.lag << "\n";
   debug_output_module << "isSingleEvent = " << debugOutput.isSingleEvent
                       << "\n";
   debug_output_module << "kurtosis = " << debugOutput.kurtosis << "\n";
   debug_output_module << "odf_peak_indices = [";
   std::for_each(
      debugOutput.odfPeakIndices.begin(), debugOutput.odfPeakIndices.end(),
      [&](int i) { debug_output_module << i << ","; });
   debug_output_module << "]\n";
   PrintPythonVector(debug_output_module, debugOutput.odf, "odf");
   PrintPythonVector(debug_output_module, debugOutput.rawOdf, "rawOdf");
   PrintPythonVector(
      debug_output_module, debugOutput.movingAverage, "movingAverage");

   std::ofstream stft_log_module {
      std::string { CMAKE_SOURCE_DIR } +
      "/tests/visualization/TatumQuantizationFitVisualization/stft_log.py"
   };
   stft_log_module << "wavFile = \"" << wavFile << "\"\n";
   stft_log_module << "sampleRate = " << audio.GetSampleRate() << "\n";
   stft_log_module << "frameRate = " << debugOutput.odfSr << "\n";

   printStft(stft_log_module, "rawStft", debugOutput.stft);
   printStft(stft_log_module, "stft", debugOutput.postProcessedStft);
}
} // namespace LTE
