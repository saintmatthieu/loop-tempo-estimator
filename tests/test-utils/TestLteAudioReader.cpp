/*  SPDX-License-Identifier: GPL-2.0-or-later */
/*!********************************************************************

  Audacity: A Digital Audio Editor

  WaveMirAudioReader.cpp

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
#include "TestLteAudioReader.h"
#include "AudioFileIO.h"
#include "AudioFileInfo.h"

#include <cassert>
#include <exception>
#include <stdexcept>

namespace LTE
{
TestLteAudioReader::TestLteAudioReader(
   const std::string& filename, std::optional<double> timeLimit)
{
   AudioFileInfo info;
   std::vector<std::vector<float>> samples;
   if (!AudioFileIO::Read(filename, samples, info))
      throw std::runtime_error(std::string { "(TestLteAudioReader) Failed to read " } + filename);

   const_cast<double&>(mSampleRate) = info.sampleRate;
   const auto limit = timeLimit.has_value() ?
                         static_cast<long long>(*timeLimit * info.sampleRate) :
                         std::numeric_limits<long long>::max();
   auto& mutableSamples = const_cast<std::vector<float>&>(mSamples);
   const auto numFrames = std::min<long long>(info.numFrames, limit);
   mutableSamples.resize(numFrames);
   if (info.numChannels == 2)
      for (size_t i = 0; i < numFrames; ++i)
         mutableSamples[i] = (samples[0][i] + samples[1][i]) / 2.f;
   else
      std::copy(
         samples[0].begin(), samples[0].begin() + numFrames,
         mutableSamples.begin());
}

double TestLteAudioReader::GetSampleRate() const
{
   return mSampleRate;
}

long long TestLteAudioReader::GetNumSamples() const
{
   return mSamples.size();
}

void TestLteAudioReader::ReadFloats(
   float* buffer, long long start, size_t numFrames) const
{
   assert(start >= 0);
   assert(start + numFrames <= mSamples.size());
   std::copy(
      mSamples.begin() + start, mSamples.begin() + start + numFrames, buffer);
}
} // namespace LTE
