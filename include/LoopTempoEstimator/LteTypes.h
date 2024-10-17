/*  SPDX-License-Identifier: GPL-2.0-or-later */
/*!********************************************************************

  Audacity: A Digital Audio Editor

  MirTypes.h

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

#pragma once

#include <array>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include "PowerSpectrumGetter.h"

namespace LTE
{
enum class FalsePositiveTolerance
{
   Strict,
   Lenient,
};

struct OnsetQuantization
{
   double error = 0.;
   int lag = 0;
   int numDivisions = 0;
};

class LteAudioReader
{
public:
   virtual double GetSampleRate() const = 0;
   virtual long long GetNumSamples() const = 0;
   virtual void
   ReadFloats(float* buffer, long long where, size_t numFrames) const = 0;
   double GetDuration() const
   {
      return GetSampleRate() == 0 ? 0. : GetNumSamples() / GetSampleRate();
   }
   virtual ~LteAudioReader() = default;
};

struct QuantizationFitDebugOutput
{
   OnsetQuantization tatumQuantization;
   double bpm = 0;
   double score = 0.;
   std::vector<std::vector<float>> postProcessedStft;
   std::vector<float> rawOdf;
   std::vector<float> movingAverage;
   std::vector<float> odf;
   double odfSr = 0.;
   double audioFileDuration = 0.;
   std::vector<int> odfPeakIndices;
   std::vector<float> odfAutoCorr;
   std::vector<int> odfAutoCorrPeakIndices;
};
} // namespace LTE
