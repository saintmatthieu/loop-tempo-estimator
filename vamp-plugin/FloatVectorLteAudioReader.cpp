#include "FloatVectorLteAudioReader.h"

#include <algorithm>
#include <numeric>

FloatVectorLteAudioReader::FloatVectorLteAudioReader(double sampleRate)
    : mSampleRate { sampleRate }
{
}

void FloatVectorLteAudioReader::Initialize(int stepSize)
{
   mBlockSize = stepSize;
}

void FloatVectorLteAudioReader::Push(const float* const* inputBuffers)
{
   mBuffer.insert(mBuffer.end(), inputBuffers[0], inputBuffers[0] + mBlockSize);
}

void FloatVectorLteAudioReader::RemoveTrailingZeros(int max)
{
   const auto it = std::find_if(
      mBuffer.rbegin(), mBuffer.rbegin() + max, [](float s) { return s != 0; });
   mBuffer.erase(it.base(), mBuffer.end());
}

double FloatVectorLteAudioReader::GetSampleRate() const
{
   return mSampleRate;
}

long long FloatVectorLteAudioReader::GetNumSamples() const
{
   return mBuffer.size();
}

void FloatVectorLteAudioReader::ReadFloats(
   float* buffer, long long where, size_t numFrames) const
{
   std::copy(
      mBuffer.begin() + where, mBuffer.begin() + where + numFrames, buffer);
}
