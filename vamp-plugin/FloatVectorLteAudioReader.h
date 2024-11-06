#pragma once

#include "LoopTempoEstimator/LteTypes.h"

/*
 * @brief A silly reader that accumulates the samples it's given in memory and
 * can give them to the algorithm later.
 *
 * This may be heavy on memory in this context, but the LTE::LteAudioReader
 * implementation in a DAW would be reading from disk. Also, the algorithm is
 * offline by design. Not sure why the Vamp SDK went for this real-time looking
 * Vamp::Plugin::process API.
 */
class FloatVectorLteAudioReader final : public LTE::LteAudioReader
{
public:
   FloatVectorLteAudioReader(double sampleRate);

   void Initialize(int blockSize);
   void Push(const float* const* inputBuffers);
   void RemoveTrailingZeros(int max);

   // LTE::LteAudioReader methods we may reuse in the vamp plugin
public:
   long long GetNumSamples() const override;

private:
   double GetSampleRate() const override;
   void
   ReadFloats(float* buffer, long long where, size_t numFrames) const override;

private:
   const double mSampleRate;
   int mBlockSize = 0;
   std::vector<float> mBuffer;
};