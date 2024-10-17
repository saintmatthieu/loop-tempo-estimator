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

#define USE_FILESYSTEM (__has_include(<filesystem>) && _WIN32)

#if USE_FILESYSTEM
#   include <filesystem>
#endif

#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <regex>

namespace LTE
{
void ProgressBar(int width, int percent)
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

OctaveError GetOctaveError(double expected, double actual)
{
   constexpr std::array<double, 5> factors { 1., 2., .5, 3., 1. / 3 };
   std::vector<OctaveError> octaveErrors;
   std::transform(
      factors.begin(), factors.end(), std::back_inserter(octaveErrors),
      [&](double factor) {
         const auto remainder = std::log2(factor * actual / expected);
         return OctaveError { factor, remainder };
      });
   return *std::min_element(
      octaveErrors.begin(), octaveErrors.end(),
      [](const auto& a, const auto& b) {
         return std::abs(a.remainder) < std::abs(b.remainder);
      });
}

std::optional<double> GetBpmFromFilename(const std::string& filename)
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
} // namespace LTE
