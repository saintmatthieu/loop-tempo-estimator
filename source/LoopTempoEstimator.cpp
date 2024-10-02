/*  SPDX-License-Identifier: GPL-2.0-or-later */
/*!********************************************************************

  Audacity: A Digital Audio Editor

  MusicInformationRetrieval.cpp

  Matthieu Hodgkinson

**********************************************************************/
/*

 This file was copied from Audacity 3.5.1,
 https://github.com/audacity/audacity/releases/tag/Audacity-3.5.1.

 Modifications might have been made to remove dependencies on Audacity code and
 when renaming files. The algorithm remains the same.

 */
#include "LoopTempoEstimator.h"
#include "DecimatingLteAudioReader.h"
#include "GetMeterUsingTatumQuantizationFit.h"
#include "LteTypes.h"
#include "LteUtils.h"
#include "StftFrameProvider.h"

#include "MemoryX.h"

#include <array>
#include <cassert>
#include <cmath>
#include <numeric>

namespace LTE
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

std::optional<MusicalMeter> GetMusicalMeterFromSignal(
   const LteAudioReader& audio, FalsePositiveTolerance tolerance,
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
   DecimatingLteAudioReader decimatedAudio { audio };
   return GetMeterUsingTatumQuantizationFit(
      decimatedAudio, tolerance, progressCallback, debugOutput);
}
} // namespace LTE
