/*  SPDX-License-Identifier: GPL-2.0-or-later */
/*!********************************************************************

  Audacity: A Digital Audio Editor

  MusicInformationRetrieval.cpp

  Matthieu Hodgkinson

**********************************************************************/
#include "MusicInformationRetrieval.h"
#include "DecimatingMirAudioReader.h"
#include "GetMeterUsingTatumQuantizationFit.h"
#include "MirTypes.h"
#include "MirUtils.h"
#include "StftFrameProvider.h"

#include "MemoryX.h"

#include <array>
#include <cassert>
#include <cmath>
#include <numeric>
#include <regex>

namespace MIR
{
namespace
{
// Normal distribution parameters obtained by fitting a gaussian in the GTZAN
// dataset tempo values.
static constexpr auto bpmExpectedValue = 126.3333;

constexpr auto numTimeSignatures = static_cast<int>(TimeSignature::_count);

auto RemovePathPrefix(const std::string& filename)
{
   return filename.substr(filename.find_last_of("/\\") + 1);
}

// When we get time-signature estimate, we may need a map for that, since 6/8
// has 1.5 quarter notes per beat.
constexpr std::array<double, numTimeSignatures> quarternotesPerBeat { 2., 1.,
                                                                      1., 1.5 };
} // namespace

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

std::optional<MusicalMeter> GetMusicalMeterFromSignal(
   const MirAudioReader& audio, FalsePositiveTolerance tolerance,
   const std::function<void(double)>& progressCallback,
   QuantizationFitDebugOutput* debugOutput)
{
   if (audio.GetSampleRate() <= 0)
      return {};
   const auto duration = 1. * audio.GetNumSamples() / audio.GetSampleRate();
   if (duration > 60)
      // A file longer than 1 minute is most likely not a loop, and processing
      // it would be costly.
      return {};
   DecimatingMirAudioReader decimatedAudio { audio };
   return GetMeterUsingTatumQuantizationFit(
      decimatedAudio, tolerance, progressCallback, debugOutput);
}
} // namespace MIR
