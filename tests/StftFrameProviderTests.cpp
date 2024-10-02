/*  SPDX-License-Identifier: GPL-2.0-or-later */
/*!********************************************************************

  Audacity: A Digital Audio Editor

  StftFrameProviderTests.cpp

  Matthieu Hodgkinson

**********************************************************************/
/*

 This file was copied from Audacity 3.5.1,
 https://github.com/audacity/audacity/releases/tag/Audacity-3.5.1.

 Modifications might have been made to remove dependencies on Audacity code and
 when renaming files. The algorithm remains the same.

 */
#include "LoopTempoEstimator/LteTypes.h"
#include "LteUtils.h"
#include "StftFrameProvider.h"

#include <catch2/catch_test_macros.hpp>

namespace LTE
{
namespace
{
class TestLteAudioReader : public LteAudioReader
{
public:
   const long long numSamples;

   TestLteAudioReader(long long numSamples)
       : numSamples { numSamples }
   {
   }
   double GetSampleRate() const override
   {
      return 44100;
   };
   long long GetNumSamples() const override
   {
      return numSamples;
   };
   void
   ReadFloats(float* buffer, long long where, size_t numFrames) const override
   {
      REQUIRE(where >= 0);
      REQUIRE(where + numFrames <= numSamples);
   };
};
} // namespace
TEST_CASE("StftFrameProvider")
{
   SECTION("handles empty files")
   {
      StftFrameProvider sut { TestLteAudioReader { 0 } };
      PffftFloatVector frame;
      REQUIRE(!sut.GetNextFrame(frame));
   }
   SECTION("handles very short files")
   {
      StftFrameProvider sut { TestLteAudioReader { 1 } };
      PffftFloatVector frame;
      REQUIRE(!sut.GetNextFrame(frame));
   }
   SECTION("has power-of-two number of frames")
   {
      StftFrameProvider sut { TestLteAudioReader { 123456 } };
      REQUIRE(IsPowOfTwo(sut.GetNumFrames()));
   }
   SECTION("respects LteAudioReader boundaries")
   {
      TestLteAudioReader reader { 123456 };
      StftFrameProvider sut { reader };
      PffftFloatVector frame;
      while (sut.GetNextFrame(frame))
         ;
   }
}
} // namespace LTE
