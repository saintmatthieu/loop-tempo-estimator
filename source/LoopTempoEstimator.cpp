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

EDIT: Commit "Simplify and improve algorithm by considering only 4/4" improves
quality of classifier and possibly time performance, too.

 */
#include "LoopTempoEstimator/LoopTempoEstimator.h"
#include "DecimatingLteAudioReader.h"
#include "GetMeterUsingTatumQuantizationFit.h"
#include "LoopTempoEstimator/LteTypes.h"
#include "LteUtils.h"
#include "StftFrameProvider.h"

#include <array>
#include <cassert>
#include <cmath>
#include <fstream>
#include <numeric>

int LTE::GetFftSize(double sampleRate)
{
   return StftFrameProvider::GetFftSize(
      sampleRate / DecimatingLteAudioReader::GetDecimationFactor(sampleRate));
}

int LTE::GetOdfSize(double sampleRate, int numSamples)
{
   const auto d = DecimatingLteAudioReader::GetDecimationFactor(sampleRate);
   return StftFrameProvider::GetNumFrames(sampleRate / d, numSamples / d);
}

std::optional<double> LTE::GetBpm(
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
   return GetBpmInternal(
      decimatedAudio, tolerance, progressCallback, debugOutput);
}
