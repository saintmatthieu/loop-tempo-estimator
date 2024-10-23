/*  SPDX-License-Identifier: GPL-2.0-or-later */
/*!********************************************************************

  Audacity: A Digital Audio Editor

  MusicInformationRetrieval.h

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

#include <functional>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace LTE
{
class LteAudioReader;

struct LoopClassifierSettings
{
   /*!
    * False positive rate allowed for the classifier.
    */
   const double allowedFalsePositiveRate;

   /*!
    * Classifier score threshold above which the analyzed audio file can be
    */
   const double threshold;
};

/*!
 * Tolerance-dependent thresholds, used internally by
 * `GetMusicalMeterFromSignal` to decide whether to return a null or valid
 * BPM value. The value compared against these are scores which get higher
 * as the signal is more likely to contain music content. They are obtained by
 * running the `TatumQuantizationFitBenchmarking` test case. More information
 * there.
 */
static const std::unordered_map<FalsePositiveTolerance, LoopClassifierSettings>
   loopClassifierSettings {
      { FalsePositiveTolerance::Strict, { .04, 0.8185122569653946 } },
      { FalsePositiveTolerance::Lenient, { .1, 0.8004500873488557 } },
   };

std::optional<double> GetBpmFromSignal(
   const LteAudioReader& source, FalsePositiveTolerance tolerance,
   const std::function<void(double)>& progressCallback,
   QuantizationFitDebugOutput* debugOutput = nullptr);
} // namespace LTE
