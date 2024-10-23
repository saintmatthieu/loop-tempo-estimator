/*  SPDX-License-Identifier: GPL-2.0-or-later */
/*!********************************************************************

Audacity: A Digital Audio Editor

GetMeterUsingTatumQuantizationFit.cpp

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

#include "GetMeterUsingTatumQuantizationFit.h"
#include "LoopTempoEstimator/LoopTempoEstimator.h"
#include "LoopTempoEstimator/LteTypes.h"
#include "LteDsp.h"
#include "LteUtils.h"
#include "MapToPositiveHalfIndex.h"
#include <array>
#include <cassert>
#include <cmath>
#include <map>
#include <numeric>
#include <regex>
#include <unordered_map>
#include <unordered_set>

namespace LTE
{
namespace
{
// We only consider 4/4. An honest attempt was made at considering 3/4 and 6/8,
// but databases with time signature metadata are inexistent, and in the context
// of loops, 4/4 is overwhelmingly more common. It probably is wiser not to
// augment the risk of false positives testing non 4/4 hypotheses. It also
// simplifies the algorithm.
constexpr auto beatsPerBar = 4;

constexpr auto minBeatsPerMinute = 50.;
constexpr auto maxBeatsPerMinute = 200.;

// A map of possible number of tatums to a list of number of bars and beats per
// bar which explain it.
using PossibleDivHierarchies =
   std::unordered_map<int /*num tatums*/, std::vector<int /*num bars*/>>;

std::vector<int> GetPossibleNumberOfBars(double audioFileDuration)
{
   constexpr auto minBarsPerMinute = 15.; // 4s per bar
   constexpr auto maxBarsPerMinute = 60.;
   const int minNumBars =
      std::max(std::round(audioFileDuration * minBarsPerMinute / 60), 1.);
   const int maxNumBars = std::round(audioFileDuration * maxBarsPerMinute / 60);
   std::vector<int> possibleNumBars(maxNumBars - minNumBars + 1);
   std::iota(possibleNumBars.begin(), possibleNumBars.end(), minNumBars);
   return possibleNumBars;
}

std::vector<int> GetPossibleTatumsPerBar(double audioFileDuration, int numBars)
{
   constexpr std::array<int, 7> contextFreePossibleTatumsPerBar {
      1, // Could be an atmospheric pad with one chord per bar
      2, // Still something atmospheric,
      4, // Could still be something atmospheric, or some heavy drum rhythm?
      8, // Very common, could be anything
      12, // Shuffle rhythm, like in Michael Jackson's "The Way You Make Me Feel" ?
      16, // Very common, could be anything
      24, // Take a 16 rhythm, swing it, and you get 24 tatums per bar.
   };
   std::vector<int> possibleTatumsPerBar;
   std::for_each(
      contextFreePossibleTatumsPerBar.begin(),
      contextFreePossibleTatumsPerBar.end(),
      [&](int tatumsPerBar)
      {
         const auto numTatums = tatumsPerBar * numBars;
         const auto tatumRate = 60. * numTatums / audioFileDuration;
         constexpr auto minTatumsPerMinute = 100;
         constexpr auto maxTatumsPerMinute = 700;
         if (minTatumsPerMinute < tatumRate && tatumRate < maxTatumsPerMinute)
            possibleTatumsPerBar.push_back(tatumsPerBar);
      });
   return possibleTatumsPerBar;
}

// Gather a collection of possible numbers of divisions based on the assumption
// that the audio is a loop (hence there must be a round number of bars) and on
// reasonable bar and tatum durations.
PossibleDivHierarchies GetPossibleDivHierarchies(double audioFileDuration)
{

   PossibleDivHierarchies possibleDivHierarchies;
   for (const auto numBars : GetPossibleNumberOfBars(audioFileDuration))
   {
      for (const auto tatumsPerBar :
           GetPossibleTatumsPerBar(audioFileDuration, numBars))
      {
         possibleDivHierarchies[numBars * tatumsPerBar].push_back(numBars);
      }
   }
   return possibleDivHierarchies;
}

int GetOnsetLag(const std::vector<float>& odf, int numTatums)
{
   // Understandably, the first onset of a loop recording isn't typically
   // centered on time 0, or there would be a click at the onset. The entire
   // recording is therefore likely to have a small lag. It is important that we
   // take this lag into account for the quantization distance measure.
   // The code below is equivalent to cross-correlating the odf with a pulse
   // train of frequency `numTatums / odf.size()`. We take the position of the
   // first peak to be the lag.
   const auto pulseTrainPeriod = 1. * odf.size() / numTatums;
   auto max = std::numeric_limits<float>::lowest();
   auto lag = 0;
   while (true)
   {
      auto val = 0.f;
      for (auto i = 0; i < numTatums; ++i)
      {
         const int j = std::round(i * pulseTrainPeriod) + lag;
         val += (j < odf.size() ? odf[j] : 0.f);
      }
      if (val < max)
         break;
      max = val;
      ++lag;
   }
   return lag - 1;
}

// This is the fundament of the algorithm. It gives a weighted average of the
// normalized distance between ODF peaks and the closest tatum. The weights are
// the ODF values at the peaks. The distance is normalized by the tatum
// duration, so that the final value ranges between 0 and 1.
// This can be seen as a performance accuracy measure. The higher the tempo, the
// more accurate the performer has to be for this distance to stay low. In
// general, this is desired. We could nevertheless consider introducing a slight
// tolerance which increases with the tatum rate, as very fast rhythmic patterns
// recorded without post-editing still tend to achieve lower scores.
double GetQuantizationDistance(
   const std::vector<int>& peakIndices, const std::vector<float>& peakValues,
   size_t size, int numDivisions, int lag)
{
   std::vector<double> peakDistances(peakIndices.size());
   const auto peakSum =
      std::accumulate(peakValues.begin(), peakValues.end(), 0.);
   const auto odfSamplesPerDiv = 1. * size / numDivisions;
   std::transform(
      peakIndices.begin(), peakIndices.end(), peakDistances.begin(),
      [&](int peakIndex)
      {
         const auto shiftedIndex = peakIndex - lag;
         const auto closestDiv = std::round(shiftedIndex / odfSamplesPerDiv);
         // Normalized distance between 0 and 1:
         const auto distance =
            (shiftedIndex - closestDiv * odfSamplesPerDiv) / odfSamplesPerDiv;
         // Mutliply by two such that the error spans `[0, 1)`.
         return 2 * std::abs(distance);
      });
   // Calculate the score as the sum of the distances weighted by
   // the odf values:
   const auto weightedAverage =
      std::inner_product(
         peakDistances.begin(), peakDistances.end(), peakValues.begin(), 0.) /
      peakSum;
   return weightedAverage;
}

OnsetQuantization RunQuantizationExperiment(
   const std::vector<float>& odf, const std::vector<int>& peakIndices,
   const std::vector<float>& peakValues,
   const std::vector<int>& possibleNumTatums)
{
   const auto quantizations = [&]()
   {
      std::unordered_map<int, OnsetQuantization> quantizations;
      std::transform(
         possibleNumTatums.begin(), possibleNumTatums.end(),
         std::inserter(quantizations, quantizations.end()),
         [&](int numTatums)
         {
            const auto lag = GetOnsetLag(odf, numTatums);
            const auto distance = GetQuantizationDistance(
               peakIndices, peakValues, odf.size(), numTatums, lag);
            return std::make_pair(
               numTatums, OnsetQuantization { distance, lag, numTatums });
         });
      return quantizations;
   }();

   const auto bestFitIt = std::min_element(
      quantizations.begin(), quantizations.end(),
      [](const std::pair<int, OnsetQuantization>& a,
         const std::pair<int, OnsetQuantization>& b)
      { return a.second.error < b.second.error; });

   const auto error = bestFitIt->second.error;
   const auto mostLikelyNumTatums = bestFitIt->first;
   const auto lag = bestFitIt->second.lag;

   return { error, lag, mostLikelyNumTatums };
}

// To evaluate how likely a certain BPM is, we evaluate how much the ODF repeats
// itself at that beat rate. We do this by looking at the auto-correlation of
// the ODF, "comb-filtering" it at integer multiples of the beat period.
double GetBeatSelfSimilarityScore(
   double odfAutoCorrSampleRate, double bpm,
   const std::vector<float>& odfAutoCorr, int odfAutocorrFullSize,
   QuantizationFitDebugOutput* debugOutput)
{
   // Look for closest peak in `odfAutoCorr`:
   const auto lag = odfAutoCorrSampleRate * 60 / bpm;
   // Traverse all the auto-correlation by steps of `k*lag`, each time
   // finding the closest peak, and average the values. Doing this rather
   // than evaluating only just one peak is more robust to rhythms which
   // miss beats.
   auto periodIndex = 1;
   auto sum = 0.;
   auto numIndices = 0;
   while (true)
   {
      auto j = static_cast<int>(periodIndex++ * lag + .5);
      if (j >= odfAutoCorr.size())
         break;
      while (true)
      {
         const auto i = MapToPositiveHalfIndex(j - 1, odfAutocorrFullSize);
         const auto k = MapToPositiveHalfIndex(j + 1, odfAutocorrFullSize);
         if (
            odfAutoCorr[i] <= odfAutoCorr[j] &&
            odfAutoCorr[j] >= odfAutoCorr[k])
            break;
         j = odfAutoCorr[i] > odfAutoCorr[k] ? i : k;
      }
      sum += odfAutoCorr[j];
      ++numIndices;
      if (debugOutput)
         debugOutput->odfAutoCorrPeakIndices.push_back(j);
   }
   return sum / numIndices;
}

size_t GetBestBarDivisionIndex(
   const std::vector<int>& possibleNumBars, double audioFileDuration,
   int numTatums, const std::vector<float>& odf,
   QuantizationFitDebugOutput* debugOutput)

{
   const auto odfAutoCorr = GetNormalizedCircularAutocorr(odf);
   if (debugOutput)
      debugOutput->odfAutoCorr = odfAutoCorr;
   const auto odfAutocorrFullSize = 2 * (odfAutoCorr.size() - 1);
   assert(IsPowOfTwo(odfAutocorrFullSize));
   const auto odfAutoCorrSampleRate = odfAutocorrFullSize / audioFileDuration;

   std::vector<double> scores(possibleNumBars.size());
   // These (still) only depend on the beat rate. We look at
   // self-similarities at beat intervals, so to say, without examining the
   // contents of the beats. Since several bar divisions may yield the same
   // number of beats, we may be able to re-use some calculations.
   std::unordered_map<int /*numBeats*/, double> autocorrScoreCache;
   std::transform(
      possibleNumBars.begin(), possibleNumBars.end(), scores.begin(),
      [&](int numBars)
      {
         const auto numBeats = numBars * beatsPerBar;
         const auto bpm = 1. * numBeats / audioFileDuration * 60;
         if (!autocorrScoreCache.count(numBeats))
            autocorrScoreCache[numBeats] = GetBeatSelfSimilarityScore(
               odfAutoCorrSampleRate, bpm, odfAutoCorr, odfAutocorrFullSize,
               debugOutput);
         const auto selfSimilarityScore = autocorrScoreCache.at(numBeats);
         return selfSimilarityScore;
      });

   return std::max_element(scores.begin(), scores.end()) - scores.begin();
}

double GetMostLikelyBpmFromQuantizationExperiment(
   const std::vector<float>& odf, int numTatums,
   const std::vector<int>& possibleNumBars, double audioFileDuration,
   QuantizationFitDebugOutput* debugOutput)
{
   const auto winnerIndex = GetBestBarDivisionIndex(
      possibleNumBars, audioFileDuration, numTatums, odf, debugOutput);
   const auto& numBars = possibleNumBars[winnerIndex];
   const auto numBeats = numBars * beatsPerBar;
   return 60. * numBeats / audioFileDuration;
}

bool IsSingleEvent(
   const std::vector<int>& peakIndices, const std::vector<float>& peakValues)
{
   // Detect single-event recordings, e.g. only just one crash cymbal hit. This
   // will translate as one large peak and maybe a few much smaller ones. In
   // that case we can expect the average to be between these two groups.
   const auto peakAvg =
      std::accumulate(peakValues.begin(), peakValues.end(), 0.) /
      peakIndices.size();
   const auto numPeaksAboveAvg = std::count_if(
      peakValues.begin(), peakValues.end(),
      [&](float v) { return v > peakAvg; });
   return numPeaksAboveAvg <= 1;
}
} // namespace

std::optional<double> GetBpm(
   const LteAudioReader& audio, FalsePositiveTolerance tolerance,
   const std::function<void(double)>& progressCallback,
   QuantizationFitDebugOutput* debugOutput)
{
   const auto odf =
      GetOnsetDetectionFunction(audio, progressCallback, debugOutput);
   const auto odfSr =
      1. * audio.GetSampleRate() * odf.size() / audio.GetNumSamples();
   const auto audioFileDuration =
      1. * audio.GetNumSamples() / audio.GetSampleRate();

   const auto peakIndices = GetPeakIndices(odf);
   if (debugOutput)
   {
      debugOutput->audioFileDuration = audioFileDuration;
      debugOutput->odfSr = odfSr;
      debugOutput->odfPeakIndices = peakIndices;
   }

   const auto peakValues = ([&]() {
      std::vector<float> peakValues(peakIndices.size());
      std::transform(
         peakIndices.begin(), peakIndices.end(), peakValues.begin(),
         [&](int i) { return odf[i]; });
      return peakValues;
   })();

   if (IsSingleEvent(peakIndices, peakValues))
      return {};

   const auto possibleDivs = GetPossibleDivHierarchies(audioFileDuration);
   if (possibleDivs.empty())
      // The file is probably too short to be a loop.
      return {};

   const auto possibleNumTatums = [&]()
   {
      std::vector<int> possibleNumTatums(possibleDivs.size());
      std::transform(
         possibleDivs.begin(), possibleDivs.end(), possibleNumTatums.begin(),
         [&](const auto& entry) { return entry.first; });
      return possibleNumTatums;
   }();

   const auto experiment = RunQuantizationExperiment(
      odf, peakIndices, peakValues, possibleNumTatums);

   const auto bpm = GetMostLikelyBpmFromQuantizationExperiment(
      odf, experiment.numDivisions, possibleDivs.at(experiment.numDivisions),
      audioFileDuration, debugOutput);

   const auto score = 1 - experiment.error;

   if (debugOutput)
   {
      debugOutput->tatumQuantization = experiment;
      debugOutput->bpm = bpm;
      debugOutput->odf = odf;
      debugOutput->odfSr = odfSr;
      debugOutput->audioFileDuration = audioFileDuration;
      debugOutput->score = score;
   }

   return score >= loopClassifierSettings.at(tolerance).threshold ?
             std::make_optional(bpm) :
             std::nullopt;
}
} // namespace LTE
