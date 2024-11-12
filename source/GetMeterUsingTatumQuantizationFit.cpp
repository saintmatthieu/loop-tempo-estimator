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
#include <array>
#include <cmath>
#include <iterator>
#include <numeric>
#include <unordered_map>

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
constexpr auto minBarsPerMinute = minBeatsPerMinute / beatsPerBar;

using TatumCountToBarCounts =
   std::unordered_map<int /*num tatums*/, std::vector<int /*num bars*/>>;

std::vector<int> GetPossibleNumberOfBars(double audioFileDuration)
{
   constexpr auto maxBarsPerMinute = maxBeatsPerMinute / beatsPerBar;
   if (audioFileDuration * maxBarsPerMinute / 60 < 1)
      // Even at the fastest realistic tempo, the input isn't long enough to
      // fill a bar. Can't be a loop.
      return {};
   const int minNumBars =
      std::max(std::ceil(audioFileDuration * minBarsPerMinute / 60), 1.);
   const int maxNumBars = std::floor(audioFileDuration * maxBarsPerMinute / 60);
   std::vector<int> possibleNumBars(maxNumBars - minNumBars + 1);
   std::iota(possibleNumBars.begin(), possibleNumBars.end(), minNumBars);
   return possibleNumBars;
}

std::vector<int> GetPossibleTatumsPerBar(double audioFileDuration, int numBars)
{
   constexpr std::array<int, 7> contextFreePossibleTatumsPerBar {
      1,  // Could be an atmospheric pad with one chord per bar
      2,  // Still something atmospheric,
      4,  // Could still be something atmospheric, or some heavy drum rhythm?
      8,  // Very common, could be anything
      12, // Shuffle rhythm, like in Michael Jackson's "The Way You Make Me
          // Feel" ?
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
         // If 1 is the least number of tatums per bar, then these two are
         // equal.
         constexpr auto minTatumsPerMinute = minBarsPerMinute;
         constexpr auto maxTatumsPerMinute = 720;
         if (minTatumsPerMinute < tatumRate && tatumRate < maxTatumsPerMinute)
            possibleTatumsPerBar.push_back(tatumsPerBar);
      });
   return possibleTatumsPerBar;
}

// Gather a collection of possible numbers of bar counts based on the assumption
// that the audio is a loop (hence there must be a round number of bars) and on
// reasonable bar and tatum durations.
TatumCountToBarCounts GetTatumCountToBarCounts(double audioFileDuration)
{

   TatumCountToBarCounts possibleDivHierarchies;
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
   size_t odfSize, int tatumCount, int lag)
{
   std::vector<double> peakDistances(peakIndices.size());
   const auto peakSum =
      std::accumulate(peakValues.begin(), peakValues.end(), 0.);
   std::transform(
      peakIndices.begin(), peakIndices.end(), peakDistances.begin(),
      [&](int peakIndex)
      {
         // range is [0, 1]
         const auto peakPos = 1. * (peakIndex - lag) / odfSize;
         const auto nearestTatumPos =
            std::round(peakPos * tatumCount) / tatumCount;
         // max is 0.5 / tatumCount
         const auto distance = std::abs(peakPos - nearestTatumPos);
         // we normalize back to 1
         return 2 * distance * tatumCount;
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
   const std::vector<int>& tatumCountHypotheses)
{
   const auto quantizations = [&]()
   {
      std::unordered_map<int, OnsetQuantization> quantizations;
      std::transform(
         tatumCountHypotheses.begin(), tatumCountHypotheses.end(),
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

double GetMostLikelyBpmFromQuantizationExperiment(
   const std::vector<float>& odf, int tatumsCount,
   const std::vector<int>& possibleNumBars, double audioFileDuration,
   QuantizationFitDebugOutput* debugOutput)
{
   std::vector<double> scores(possibleNumBars.size());
   std::transform(
      possibleNumBars.begin(), possibleNumBars.end(), scores.begin(),
      [&](int numBars)
      {
         const auto numBeats = numBars * beatsPerBar;
         const auto bpm = 1. * numBeats / audioFileDuration * 60;
         return std::abs(bpm - 120);
      });
   const auto winnerIndex =
      std::max_element(scores.begin(), scores.end()) - scores.begin();
   const auto& numBars = possibleNumBars[winnerIndex];
   const auto numBeats = numBars * beatsPerBar;
   return 60. * numBeats / audioFileDuration;
}

bool IsSingleEvent(
   const std::vector<float>& odf, QuantizationFitDebugOutput* debugOutput)
{
   // Kurtosis is normally the metric of a PDF, which our ODF isn't.
   // If it happens to be a single event, though, it will somewhat look like a
   // normal distribution, with a reasonably low kurtosis.

   // If it's a single event at the very beginning, since our ODF is circular, a
   // relatively high value may figure at the end of the ODF. So what we do is,
   // we find the largest ODF value, and shift it so that it's in the middle.
   // We take this opportunity to normalize it so that it abides by the
   // definition of a PDF.
   const auto pdf = [&]
   {
      const auto len = static_cast<int>(odf.size());
      // For now we make a copy, for simplicity, but we may want to optimize it.
      auto shiftedOdf = odf;
      const auto sum = std::accumulate(odf.begin(), odf.end(), 0.f);
      const auto maxIt = std::max_element(odf.begin(), odf.end());
      const auto shift =
         (len * 3 / 2 - std::distance(odf.begin(), maxIt)) % len;
      std::rotate(
         shiftedOdf.begin(), shiftedOdf.end() - shift, shiftedOdf.end());
      std::transform(
         shiftedOdf.begin(), shiftedOdf.end(), shiftedOdf.begin(),
         [&](float val) { return val / sum; });
      return shiftedOdf;
   }();

   const auto expectedValue = std::accumulate(
      pdf.begin(), pdf.end(), 0.f,
      [i = 0](float result, float val) mutable { return result + i++ * val; });

   auto kurtosis = 0.;
   auto variance = 0.;
   for (auto i = 0; i < pdf.size(); ++i)
   {
      const auto tmp = i - expectedValue;
      variance += tmp * tmp * pdf[i];
      kurtosis += tmp * tmp * tmp * tmp * pdf[i];
   }
   kurtosis /= variance * variance;

   const auto isSingleEvent = kurtosis > 20;
   if (debugOutput)
   {
      debugOutput->kurtosis = kurtosis;
      debugOutput->isSingleEvent = isSingleEvent;
   }

   return isSingleEvent;
}
} // namespace

std::optional<double> GetBpmInternal(
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
      debugOutput->odf = odf;
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

   if (IsSingleEvent(odf, debugOutput))
      return {};

   // Just based on audio duration and our prior knowledge of possible beat and
   // tatum rates, we can get a map of possible tatum counts and, for each of
   // them, the possible number of bars.
   const auto tatumCountToBarCounts =
      GetTatumCountToBarCounts(audioFileDuration);
   if (tatumCountToBarCounts.empty())
      // The file is probably too short to be a loop.
      return {};

   const auto tatumCountHypotheses = [&]()
   {
      std::vector<int> tatumCountHypotheses(tatumCountToBarCounts.size());
      std::transform(
         tatumCountToBarCounts.begin(), tatumCountToBarCounts.end(),
         tatumCountHypotheses.begin(),
         [&](const auto& entry) { return entry.first; });
      return tatumCountHypotheses;
   }();

   // Now we look at the ODF and see how well it matches the different
   // tatum-count hypotheses. The best match is returned, together with the
   // error and the lag.
   const auto experiment = RunQuantizationExperiment(
      odf, peakIndices, peakValues, tatumCountHypotheses);

   // Now that we have a tatum count winner, we still have to decide on the most
   // likely tatum-per-bar (and hence BPM) hypothesis.
   const auto bpm = GetMostLikelyBpmFromQuantizationExperiment(
      odf, experiment.tatumCount,
      tatumCountToBarCounts.at(experiment.tatumCount), audioFileDuration,
      debugOutput);

   const auto score = 1 - experiment.error;

   if (debugOutput)
   {
      debugOutput->tatumQuantization = experiment;
      debugOutput->bpm = bpm;
      debugOutput->score = score;
   }

   return score >= loopClassifierSettings.at(tolerance).threshold ?
             std::make_optional(bpm) :
             std::nullopt;
}
} // namespace LTE
