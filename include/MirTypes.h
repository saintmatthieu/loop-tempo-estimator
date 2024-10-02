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

enum class TimeSignature
{
   TwoTwo,
   FourFour,
   ThreeFour,
   SixEight,
   _count
};

inline int GetNumerator(TimeSignature ts)
{
   constexpr std::array<int, static_cast<int>(TimeSignature::_count)>
      numerators = { 2, 4, 3, 6 };
   return numerators[static_cast<int>(ts)];
}

inline int GetDenominator(TimeSignature ts)
{
   constexpr std::array<int, static_cast<int>(TimeSignature::_count)>
      denominators = { 2, 4, 4, 8 };
   return denominators[static_cast<int>(ts)];
}

struct MusicalMeter
{
   const double bpm;
   const std::optional<TimeSignature> timeSignature;
};

struct OnsetQuantization
{
   double error = 0.;
   int lag = 0;
   int numDivisions = 0;
};

class MirAudioReader
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
   virtual ~MirAudioReader() = default;
};

struct QuantizationFitDebugOutput
{
   OnsetQuantization tatumQuantization;
   double bpm = 0;
   std::optional<TimeSignature> timeSignature;
   double score = 0.;
   std::vector<PffftFloatVector> postProcessedStft;
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
