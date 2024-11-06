/*  SPDX-License-Identifier: GPL-2.0-or-later */
/*!********************************************************************

  Audacity: A Digital Audio Editor

  StftFrameProvider.cpp

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
#include "StftFrameProvider.h"
#include "LoopTempoEstimator/LteTypes.h"
#include "LteUtils.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <numeric>

namespace LTE
{
namespace
{
constexpr auto twoPi = 2 * 3.14159265358979323846;

double GetHopSize(int sampleRate, long long numSamples)
{
   // Aim for a hop size closest to 10ms, yet dividing `numSamples` to a power
   // of two. This will spare us the need for resampling when we need to get the
   // autocorrelation of the ODF using an FFT.
   const auto idealHopSize = 0.01 * sampleRate;
   const int exponent = std::round(std::log2(numSamples / idealHopSize));
   if (exponent < 0)
      return 0;
   const auto numFrames = 1 << exponent;
   return 1. * numSamples / numFrames;
}
} // namespace

int StftFrameProvider::GetFftSize(double sampleRate)
{
   // 2048 frame size for sample rate 44.1kHz
   return 1 << (11 + (int)std::round(std::log2(sampleRate / 44100.)));
}

int StftFrameProvider::GetNumFrames(double sampleRate, int numSamples)
{
   const auto hopSize = GetHopSize(sampleRate, numSamples);
   return hopSize > 0 ? static_cast<int>(std::round(numSamples / hopSize)) : 0;
}

StftFrameProvider::StftFrameProvider(const LteAudioReader& audio)
    : mAudio { audio }
    , mFftSize { GetFftSize(audio.GetSampleRate()) }
    , mHopSize { GetHopSize(audio.GetSampleRate(), audio.GetNumSamples()) }
    , mWindow { GetNormalizedHann(mFftSize) }
    , mNumFrames { GetNumFrames(audio.GetSampleRate(), audio.GetNumSamples()) }
    , mNumSamples { audio.GetNumSamples() }
{
   assert(mNumFrames == 0 || IsPowOfTwo(mNumFrames));
}

bool StftFrameProvider::GetNextFrame(PffftFloatVector& frame)
{
   if (mNumFramesProvided >= mNumFrames)
      return false;
   frame.resize(mFftSize, 0.f);
   const int firstReadPosition = mHopSize - mFftSize;
   int start = std::round(firstReadPosition + mNumFramesProvided * mHopSize);
   while (start < 0)
      start += mNumSamples;
   const auto end = std::min<long long>(start + mFftSize, mNumSamples);
   const auto numToRead = end - start;
   mAudio.ReadFloats(frame.data(), start, numToRead);
   // It's not impossible that some user drops a file so short that `mFftSize >
   // mNumSamples`. In that case we won't be returning a meaningful
   // STFT, but that's a use case we're not interested in. We just need to make
   // sure we don't crash.
   const auto numRemaining = std::min(mFftSize - numToRead, mNumSamples);
   if (numRemaining > 0)
      mAudio.ReadFloats(frame.data() + numToRead, 0, numRemaining);
   std::transform(
      frame.begin(), frame.end(), mWindow.begin(), frame.begin(),
      std::multiplies<float>());
   ++mNumFramesProvided;
   return true;
}

int StftFrameProvider::GetNumFrames() const
{
   return mNumFrames;
}

int StftFrameProvider::GetSampleRate() const
{
   return mAudio.GetSampleRate();
}

double StftFrameProvider::GetFrameRate() const
{
   return 1. * mAudio.GetSampleRate() / mHopSize;
}

int StftFrameProvider::GetFftSize() const
{
   return mFftSize;
}
} // namespace LTE
