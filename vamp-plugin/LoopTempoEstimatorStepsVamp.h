#pragma once

#include "LteVampBase.h"

class LoopTempoEstimatorStepsVamp final : public LteVampBase
{
public:
   LoopTempoEstimatorStepsVamp(float inputSampleRate);

private:
   std::string GetIdentifier() const override;
   std::string GetName() const override;
   std::string GetDescription() const override;
   int GetPluginVersion() const override;
   OutputList GetOutputDescriptors() const override;
   FeatureSet GetRemainingFeatures(LTE::LteAudioReader&) override;
};
