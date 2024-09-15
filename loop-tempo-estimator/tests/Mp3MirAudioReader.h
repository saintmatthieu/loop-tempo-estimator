/*  SPDX-License-Identifier: GPL-2.0-or-later */
/*!********************************************************************

  Audacity: A Digital Audio Editor

  Mp3MirAudioReader.h

  Alfredo Gonzalez-Martinez

**********************************************************************/
#pragma once

#include "MirTypes.h"

#include <optional>
#include <string>
#include <vector>

namespace MIR
{
class Mp3MirAudioReader : public MirAudioReader
{
public:
   Mp3MirAudioReader(
      const std::string& filename, std::optional<double> timeLimit = {});

   double GetSampleRate() const override;
   long long GetNumSamples() const override;
   void
   ReadFloats(float* buffer, long long start, size_t numFrames) const override;

private:
   std::vector<float> mSamples;
   double mSampleRate = 0.;
};
} // namespace MIR