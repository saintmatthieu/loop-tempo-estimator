#pragma once

#include "FloatVectorLteAudioReader.h"
#include <vamp-sdk/Plugin.h>

class LteVampBase : public Vamp::Plugin
{
public:
   LteVampBase(float inputSampleRate);
   virtual ~LteVampBase() override = default;

   // To be overridden
private:
   virtual std::string GetIdentifier() const = 0;
   virtual std::string GetName() const = 0;
   virtual std::string GetDescription() const = 0;
   virtual int GetPluginVersion() const = 0;
   virtual OutputList GetOutputDescriptors() const = 0;
   virtual FeatureSet GetRemainingFeatures(LTE::LteAudioReader&) = 0;

   virtual ParameterList GetParameterDescriptors() const;
   virtual float GetParameter(std::string identifier) const;
   virtual void SetParameter(std::string identifier, float value);

protected:
   const int mFftSize;
   const int mSampleCount;

   // Vamp::Plugin
private:
   std::string getIdentifier() const final override;
   std::string getName() const final override;
   std::string getDescription() const final override;
   std::string getMaker() const final override;
   std::string getCopyright() const final override;
   int getPluginVersion() const final override;
   InputDomain getInputDomain() const final override;
   size_t getPreferredBlockSize() const final override;
   size_t getPreferredStepSize() const final override;
   ParameterList getParameterDescriptors() const final override;
   float getParameter(std::string identifier) const final override;
   void setParameter(std::string identifier, float value) final override;
   OutputList getOutputDescriptors() const final override;
   bool initialise(
      size_t channels, size_t stepSize, size_t blockSize) final override;
   FeatureSet process(
      const float* const* inputBuffers,
      Vamp::RealTime timestamp) final override;
   FeatureSet getRemainingFeatures() final override;
   void reset() final override;

private:
   void RemoveTrailingZeros();

   FloatVectorLteAudioReader mAudioReader;
   bool mAttachHack = false;
};
