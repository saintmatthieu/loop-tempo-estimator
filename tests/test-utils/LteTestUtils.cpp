/*  SPDX-License-Identifier: GPL-2.0-or-later */
/*!********************************************************************

  Audacity: A Digital Audio Editor

  MirTestUtils.cpp

  Matthieu Hodgkinson

**********************************************************************/
/*

 This file was copied from Audacity 3.5.1,
 https://github.com/audacity/audacity/releases/tag/Audacity-3.5.1.

 Modifications might have been made to remove dependencies on Audacity code and
 when renaming files. The algorithm remains the same.

EDIT: Commit "Simplify and improve algorithm by considering only 4/4" improves
quality of classifier and possibly time performance, too.

 */
#include "LteTestUtils.h"

#include <array>
#include <cmath>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <regex>

namespace
{
const auto datasetRoot =
   std::string(CMAKE_SOURCE_DIR) + "/tests/benchmarking/dataset";
}

std::vector<std::string> LTE::GetBenchmarkingAudioFiles()
{
   const auto pythonScriptPath =
      std::string(CMAKE_SOURCE_DIR) +
      "/tests/benchmarking/download-benchmarking-dataset.py";
   const auto command = "python3 " + pythonScriptPath + " " + datasetRoot;
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

std::string LTE::Pretty(const std::string& filename)
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

void LTE::ProgressBar(int width, int percent)
{
   int progress = (width * percent) / 100;
   std::cout << "[";
   for (int i = 0; i < width; ++i)
      if (i < progress)
         std::cout << "=";
      else
         std::cout << " ";
   std::cout << "] " << std::setw(3) << percent << "%\r";
   std::cout.flush();
}

LTE::OctaveError LTE::GetOctaveError(double expected, double actual)
{
   constexpr std::array<double, 5> factors { 1., 2., .5, 3., 1. / 3 };
   std::vector<OctaveError> octaveErrors;
   std::transform(
      factors.begin(), factors.end(), std::back_inserter(octaveErrors),
      [&](double factor)
      {
         const auto remainder = std::log2(factor * actual / expected);
         return OctaveError { factor, remainder };
      });
   return *std::min_element(
      octaveErrors.begin(), octaveErrors.end(), [](const auto& a, const auto& b)
      { return std::abs(a.remainder) < std::abs(b.remainder); });
}

std::optional<double> LTE::GetBpmFromFilename(const std::string& filename)
{
   // regex matching a forward or backward slash:

   // Regex: <(anything + (directory) separator) or nothing> <2 or 3 digits>
   // <optional separator> <bpm (case-insensitive)> <separator or nothing>
   const std::regex bpmRegex {
      R"((?:.*(?:_|-|\s|\.|/|\\))?(\d+)(?:_|-|\s|\.)?bpm(?:(?:_|-|\s|\.).*)?)",
      std::regex::icase
   };
   std::smatch matches;
   if (std::regex_match(filename, matches, bpmRegex))
      try
      {
         const auto value = std::stoi(matches[1]);
         return 30 <= value && value <= 300 ? std::optional<double> { value } :
                                              std::nullopt;
      }
      catch (const std::invalid_argument& e)
      {
         assert(false);
      }
   return {};
}
