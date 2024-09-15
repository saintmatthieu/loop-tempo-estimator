/*  SPDX-License-Identifier: GPL-2.0-or-later */
/*!********************************************************************

  Audacity: A Digital Audio Editor

  Mp3MirAudioReader.cpp

  Alfredo Gonzalez-Martinez

**********************************************************************/
#include "Mp3MirAudioReader.h"
#include "Mp3FileReader.h"
#include "AudioFileInfo.h"

#include <algorithm>
#include <cassert>
#include <exception>
#include <limits>
#include <stdexcept>

namespace MIR
{
Mp3MirAudioReader::Mp3MirAudioReader(
   const std::string& filename, std::optional<double> timeLimit)
{
   AudioFileInfo info;
   std::vector<std::vector<float>> samples;
   if (!Mp3FileReader::Read(filename, samples, info))
      throw std::runtime_error(std::string { "(Mp3MirAudioReader) Failed to read " } + filename);

   mSampleRate = info.sampleRate;
   const auto limit = timeLimit.has_value() ?
                         static_cast<long long>(*timeLimit * info.sampleRate) :
                         std::numeric_limits<long long>::max();
   const auto numFrames = std::min<long long>(info.numFrames, limit);
   mSamples.resize(numFrames);
   if (info.numChannels == 2)
      for (size_t i = 0; i < numFrames; ++i)
         mSamples[i] = (samples[0][i] + samples[1][i]) / 2.f;
   else
      std::copy(
         samples[0].begin(), samples[0].begin() + numFrames,
         mSamples.begin());
}

double Mp3MirAudioReader::GetSampleRate() const
{
   return mSampleRate;
}

long long Mp3MirAudioReader::GetNumSamples() const
{
   return mSamples.size();
}

void Mp3MirAudioReader::ReadFloats(
   float* buffer, long long start, size_t numFrames) const
{
   assert(start >= 0);
   assert(start + numFrames <= mSamples.size());
   std::copy(
      mSamples.begin() + start, mSamples.begin() + start + numFrames, buffer);
}
} // namespace MIR