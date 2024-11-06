#pragma once

#include "LteVampBase.h"

class LoopTempoEstimatorVamp final : public LteVampBase
{
public:
   LoopTempoEstimatorVamp(float inputSampleRate);

private:
   std::string GetIdentifier() const override;
   std::string GetName() const override;
   std::string GetDescription() const override;
   int GetPluginVersion() const override;
   OutputList GetOutputDescriptors() const override;
   FeatureSet GetRemainingFeatures(LTE::LteAudioReader&) override;
   ParameterList GetParameterDescriptors() const override;
   float GetParameter(std::string identifier) const override;
   void SetParameter(std::string identifier, float value) override;

private:
   LTE::FalsePositiveTolerance mFalsePositiveTolerance =
      LTE::FalsePositiveTolerance::Strict;
};
