/*  SPDX-License-Identifier: GPL-2.0-or-later */
/*!********************************************************************

  Audacity: A Digital Audio Editor

  MirFakes.h

  Matthieu Hodgkinson

**********************************************************************/
/*

 This file was copied from Audacity 3.5.1,
 https://github.com/audacity/audacity/releases/tag/Audacity-3.5.1.

 Modifications might have been made to remove dependencies on Audacity code and
 when renaming files. The algorithm remains the same.

 */
#pragma once

#include "MirTypes.h"

namespace MIR
{

class EmptyMirAudioReader : public MirAudioReader
{
   double GetSampleRate() const override
   {
      return 0;
   }
   long long GetNumSamples() const override
   {
      return 0;
   }
   void
   ReadFloats(float* buffer, long long start, size_t numFrames) const override
   {
   }
};

class SquareWaveMirAudioReader : public MirAudioReader
{
   const int period = 8;

   double GetSampleRate() const override
   {
      return 10;
   }
   long long GetNumSamples() const override
   {
      return period * 10;
   }
   void
   ReadFloats(float* buffer, long long where, size_t numFrames) const override
   {
      for (size_t i = 0; i < numFrames; ++i)
         buffer[i] = (where + i) % period < period / 2 ? 1.f : -1.f;
   }
};

} // namespace MIR
