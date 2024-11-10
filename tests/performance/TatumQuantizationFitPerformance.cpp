#include "LoopTempoEstimator/LoopTempoEstimator.h"
#include "LteFakes.h"
#include "LteTestUtils.h"
#include "TestLteAudioReader.h"

#include <chrono>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

int main()
{
   using namespace LTE;
   constexpr int progressBarWidth = 50;
   const auto audioFiles = GetBenchmarkingAudioFiles();
   const auto numFiles = audioFiles.size();
   auto count = 0;
   auto totalFileDuration = 0.;
   std::chrono::milliseconds computationTime { 0 };
   std::for_each(
      audioFiles.begin(), audioFiles.begin() + numFiles,
      [&](const std::string& audioFile)
      {
         const TestLteAudioReader audio { audioFile };
         std::function<void(double)> progressCb;
         const auto now = std::chrono::steady_clock::now();
         GetBpm(audio, FalsePositiveTolerance::Lenient, progressCb, nullptr);
         computationTime +=
            std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::steady_clock::now() - now);
         ProgressBar(progressBarWidth, 100 * count++ / numFiles);
         totalFileDuration += audio.GetDuration();
      });

   std::cout << std::endl;
   std::cout << "Number of files: " << numFiles << std::endl;
   std::cout << "Accumulated duration: " << totalFileDuration << " s"
             << std::endl;
   std::cout << "Computation time: " << computationTime.count() << " ms"
             << std::endl;
   std::cout << totalFileDuration / computationTime.count() * 1000
             << " times real-time" << std::endl;
   return 0;
}
