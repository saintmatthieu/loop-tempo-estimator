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

EDIT: Commit "Simplify and improve algorithm by considering only 4/4" improves
quality of classifier and possibly time performance, too.

 */
#pragma once

#include "LoopTempoEstimator/LteTypes.h"

namespace LTE
{
class SquareWaveLteAudioReader : public LteAudioReader
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

} // namespace LTE
