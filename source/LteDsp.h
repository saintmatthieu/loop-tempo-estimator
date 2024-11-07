/*  SPDX-License-Identifier: GPL-2.0-or-later */
/*!********************************************************************

  Audacity: A Digital Audio Editor

  MirDsp.h

  Matthieu Hodgkinson

  DSP utilities used by the Music Information Retrieval code.

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

#include <functional>

namespace LTE
{
class LteAudioReader;
struct QuantizationFitDebugOutput;

std::vector<float> GetOnsetDetectionFunction(
   const LteAudioReader& audio,
   const std::function<void(double)>& progressCallback,
   QuantizationFitDebugOutput* debugInfo);
} // namespace LTE
