#include "LoopTempoEstimatorStepsVamp.h"
#include "LoopTempoEstimator/LoopTempoEstimator.h"
#include <algorithm>
#include <limits>

LoopTempoEstimatorStepsVamp::LoopTempoEstimatorStepsVamp(float inputSampleRate)
    : LteVampBase(inputSampleRate)
{
}

std::string LoopTempoEstimatorStepsVamp::GetIdentifier() const
{
   return "loop_tempo_estimator_steps";
}

std::string LoopTempoEstimatorStepsVamp::GetName() const
{
   return "Loop Tempo Estimator Steps";
}

std::string LoopTempoEstimatorStepsVamp::GetDescription() const
{
   return "Insight into the loop tempo estimation process.";
}

int LoopTempoEstimatorStepsVamp::GetPluginVersion() const
{
   return 1;
}

Vamp::Plugin::OutputList
LoopTempoEstimatorStepsVamp::GetOutputDescriptors() const
{
   OutputList list;

   const auto duration = mSampleCount / m_inputSampleRate;
   const auto odfRate =
      mSampleCount == 0 ?
         0 :
         LTE::GetOdfSize(m_inputSampleRate, mSampleCount) / duration;

   {
      OutputDescriptor d;
      d.identifier = "stft";
      d.name = "1. STFT";
      d.description =
         "Short-time Fourier transform of the looped audio,\n"
         "i.e., the last frame overlaps the end and the beginning of the audio.";
      d.unit = "";
      d.hasFixedBinCount = true;
      d.binCount = mFftSize / 2 + 1;
      d.hasKnownExtents = false;
      d.isQuantized = false;
      d.sampleType = OutputDescriptor::FixedSampleRate;
      d.sampleRate = odfRate;
      list.push_back(d);
   }

   {
      OutputDescriptor d;
      d.identifier = "compressed_stft";
      d.name = "2. Compressed STFT";
      d.description =
         "STFT is logarithmically compressed, c.f. Meinhard Müller's\n"
         "Fundamentals of Music Processing";
      d.unit = "";
      d.hasFixedBinCount = true;
      d.binCount = mFftSize / 2 + 1;
      d.hasKnownExtents = false;
      d.isQuantized = false;
      d.sampleType = OutputDescriptor::FixedSampleRate;
      d.sampleRate = odfRate;
      list.push_back(d);
   }

   {
      OutputDescriptor d;
      d.identifier = "raw_odf";
      d.name = "3. 'Raw' Onset Detection Function";
      d.description = "ODF with magnitude-based novelty measure";
      d.unit = "";
      d.hasFixedBinCount = true;
      d.binCount = 1;
      d.hasKnownExtents = true;
      d.minValue = 0;
      d.maxValue = 1;
      d.isQuantized = false;
      d.sampleType = OutputDescriptor::FixedSampleRate;
      d.sampleRate = odfRate;
      list.push_back(d);
   }

   {
      OutputDescriptor d;
      d.identifier = "moving_average";
      d.name = "4. ODF moving average";
      d.description = "Take a moving average of the ODF";
      d.unit = "";
      d.hasFixedBinCount = true;
      d.binCount = 1;
      d.hasKnownExtents = true;
      d.minValue = 0;
      d.maxValue = 1;
      d.isQuantized = false;
      d.sampleType = OutputDescriptor::FixedSampleRate;
      d.sampleRate = odfRate;
      list.push_back(d);
   }

   {
      OutputDescriptor d;
      d.identifier = "enhanced_odf";
      d.name = "5. Enhanced ODF";
      d.description = "ODF less the moving average";
      d.unit = "";
      d.hasFixedBinCount = true;
      d.binCount = 1;
      d.hasKnownExtents = true;
      d.minValue = 0;
      d.maxValue = 1;
      d.isQuantized = false;
      d.sampleType = OutputDescriptor::FixedSampleRate;
      d.sampleRate = odfRate;
      list.push_back(d);
   }

   {
      OutputDescriptor d;
      d.identifier = "tatum_positions";
      d.name = "6. Tatum Positions";
      d.description = "Tatum Positions (based on estimated tatum rate)";
      d.unit = "";
      d.hasFixedBinCount = true;
      d.binCount = 0;
      d.sampleType = OutputDescriptor::VariableSampleRate;
      d.sampleRate = 0;
      list.push_back(d);
   }

   return list;
}

Vamp::Plugin::FeatureSet
LoopTempoEstimatorStepsVamp::GetRemainingFeatures(LTE::LteAudioReader& reader)
{
   LTE::QuantizationFitDebugOutput debugOutput;

   // The intermediary analysis steps don't refer to the tolerance, so we can
   // use anything.
   constexpr auto arbitraryTolerance = LTE::FalsePositiveTolerance::Strict;

   const std::optional<double> bpm =
      LTE::GetBpm(reader, arbitraryTolerance, nullptr, &debugOutput);

   const double duration = reader.GetNumSamples() / m_inputSampleRate;

   FeatureSet fs;

   for (auto i = 0; i < debugOutput.stft.size(); ++i)
   {
      Feature f;
      f.hasTimestamp = false;
      f.hasDuration = false;
      f.values = debugOutput.stft[i];
      fs[0].push_back(f);
   }

   for (auto i = 0; i < debugOutput.postProcessedStft.size(); ++i)
   {
      Feature f;
      f.hasTimestamp = false;
      f.hasDuration = false;
      f.values = debugOutput.postProcessedStft[i];
      fs[1].push_back(f);
   }

   const auto rawOdfMax =
      *std::max_element(debugOutput.rawOdf.begin(), debugOutput.rawOdf.end());
   {
      for (auto i = 0; i < debugOutput.rawOdf.size(); ++i)
      {
         Feature f;
         f.hasTimestamp = false;
         f.hasDuration = false;
         f.values.push_back(debugOutput.rawOdf[i] / rawOdfMax);
         fs[2].push_back(f);
      }
   }

   for (auto i = 0; i < debugOutput.movingAverage.size(); ++i)
   {
      Feature f;
      f.hasTimestamp = false;
      f.hasDuration = false;
      f.values.push_back(debugOutput.movingAverage[i] / rawOdfMax);
      fs[3].push_back(f);
   }

   for (auto i = 0; i < debugOutput.odf.size(); ++i)
   {
      Feature f;
      f.hasTimestamp = false;
      f.hasDuration = false;
      // For some reason, Sonic Visualiser ignores 0 values of "Discrete Curves"
      // type, which then looks crap.
      const auto v = std::max(
         debugOutput.odf[i] / rawOdfMax, std::numeric_limits<float>::epsilon());
      f.values.push_back(v);
      fs[4].push_back(f);
   }

   const auto& tq = debugOutput.tatumQuantization;
   const auto tatumDuration = duration / tq.tatumCount;
   const auto tatumOffset =
      Vamp::RealTime::fromSeconds(tq.lag / debugOutput.odfSr);
   for (auto i = 0; i < tq.tatumCount; ++i)
   {
      Feature f;
      f.hasTimestamp = true;
      f.timestamp =
         Vamp::RealTime::fromSeconds(i * tatumDuration) + tatumOffset;
      fs[5].push_back(f);
   }

   return fs;
}