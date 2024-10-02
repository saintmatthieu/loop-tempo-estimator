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

 */

#pragma once

#include <algorithm>
#include <functional>
#include <vector>

namespace LTE
{
class LteAudioReader;
struct QuantizationFitDebugOutput;

/*!
 * @brief Get the normalized, circular auto-correlation for a signal `x` whose
 * length already is a power of two. Since the output is symmetric, only the
 * left-hand side is returned, i.e., of size `N/2 + 1`, where `N` is the power
 * of two the input was upsampled to.
 *
 * @pre `x.size()` is a power of two.
 * @post returned vector has size `x.size() / 2 + 1`.
 */
std::vector<float> GetNormalizedCircularAutocorr(const std::vector<float>& x);

std::vector<float> GetOnsetDetectionFunction(
   const LteAudioReader& audio,
   const std::function<void(double)>& progressCallback,
   QuantizationFitDebugOutput* debugInfo);
} // namespace LTE
