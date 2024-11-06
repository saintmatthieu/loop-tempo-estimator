#include "LoopTempoEstimatorVamp.h"
#include "LoopTempoEstimator/LoopTempoEstimator.h"

LoopTempoEstimatorVamp::LoopTempoEstimatorVamp(float inputSampleRate)
    : LteVampBase(inputSampleRate)
{
}

Vamp::Plugin::ParameterList
LoopTempoEstimatorVamp::GetParameterDescriptors() const
{
   ParameterList list;

   ParameterDescriptor d;
   d.identifier = "threshold";
   d.name = "Threshold";
   d.description =
      "Strict or Lenient.\n"
      "Depending on context, one may be more appropriate than the other.\n"
      "For example, in Audacity, when in the new music mode, the threshold is set to lenient, and strict otherwise,\n"
      "to reduce the risk of false positives.";
   d.unit = "";
   d.minValue = 0;
   d.maxValue = 1;
   d.defaultValue = static_cast<float>(mFalsePositiveTolerance);
   d.isQuantized = true;
   d.quantizeStep = 1;
   for (auto i = 0; i < 2; ++i)
      d.valueNames.push_back(
         static_cast<LTE::FalsePositiveTolerance>(i) ==
               LTE::FalsePositiveTolerance::Strict ?
            "Strict" :
            "Lenient");
   list.push_back(d);

   return list;
}

Vamp::Plugin::OutputList LoopTempoEstimatorVamp::GetOutputDescriptors() const
{
   OutputDescriptor d;
   d.identifier = "tempo";
   d.name = "Tempo";
   d.description = "Tempo (BPM) or 0 if not a loop";
   d.unit = "bpm";
   d.hasFixedBinCount = true;
   d.binCount = 1;
   d.hasKnownExtents = false;
   d.isQuantized = false;
   d.sampleType = OutputDescriptor::VariableSampleRate;
   d.sampleRate = 0;
   d.hasDuration = true;

   return { d };
}

Vamp::Plugin::FeatureSet
LoopTempoEstimatorVamp::GetRemainingFeatures(LTE::LteAudioReader& reader)
{
   const std::optional<double> bpm =
      LTE::GetBpm(reader, mFalsePositiveTolerance, nullptr, nullptr);

   const double duration = reader.GetNumSamples() / m_inputSampleRate;

   Feature f;
   f.hasTimestamp = true;
   f.timestamp = Vamp::RealTime::zeroTime;
   f.hasDuration = true;
   f.duration = Vamp::RealTime::fromSeconds(duration);
   f.values.push_back(bpm.value_or(0));

   FeatureSet fs;
   fs[0].push_back(f);
   return fs;
}

std::string LoopTempoEstimatorVamp::GetIdentifier() const
{
   return "loop-tempo-estimator";
}

std::string LoopTempoEstimatorVamp::GetName() const
{
   return "Loop Tempo Estimator";
}

std::string LoopTempoEstimatorVamp::GetDescription() const
{
   return "Estimate the tempo of a loop, if it is one.";
}

int LoopTempoEstimatorVamp::GetPluginVersion() const
{
   return 1;
}

float LoopTempoEstimatorVamp::GetParameter(std::string identifier) const
{
   if (identifier == "threshold")
      return static_cast<float>(mFalsePositiveTolerance);
   return 0;
}

void LoopTempoEstimatorVamp::SetParameter(std::string identifier, float value)
{
   if (identifier == "threshold")
      mFalsePositiveTolerance = static_cast<LTE::FalsePositiveTolerance>(value);
}
