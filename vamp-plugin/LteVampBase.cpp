#include "LteVampBase.h"
#include "FileUtils.h"
#include "LoopTempoEstimator/LoopTempoEstimator.h"
#include <thread>
#include <vamp-sdk/PluginAdapter.h>

#ifdef NDEBUG
#error For some obscure reason, the DLL doesn't get loaded by sonic visualiser when built in Release mode. If you can fix this then please remove this error.
#endif

namespace
{
// The total number of output samples pushed by Sonic Visualizer and possibly
// other hosts (such as the simple host example of the vamp sdk) is a multiple
// of this number. The algorithm is sensitive to the exact duration of the loop,
// though, and in truth this probably won't be a multiple of 1024.
// One option is to use a very small size, so that the number of appended zeros
// is negigible compared to the loop duration. This impairs performance, though,
// and still isn't ideal.
// Another option, the one we're choosing here, is to remove the trailing zeros
// once all samples have been pushed by the host, assuming that they are
// zero-padding. (See getRemainingFeatures.)
constexpr auto preferredBlockSize = 1024;
} // namespace

LteVampBase::LteVampBase(float inputSampleRate)
    : Plugin(inputSampleRate)
    , mFftSize(LTE::GetFftSize(inputSampleRate))
    , mSampleCount(LTE::FileUtils::ReadFromAppDataFile())
    , mAudioReader(inputSampleRate)
{
   // This was an unsuccessfuly attempt to make the plugin work in Sonic
   // Visualiser also in Release mode. Doesn't work, but let's leave it here to
   // show that the experiment has already been done.
   (void*)&vampGetPluginDescriptor;
}

std::string LteVampBase::getIdentifier() const
{
   return GetIdentifier();
}

std::string LteVampBase::getName() const
{
   return GetName();
}

std::string LteVampBase::getDescription() const
{
   return GetDescription();
}

int LteVampBase::getPluginVersion() const
{
   return GetPluginVersion();
}

Vamp::Plugin::ParameterList LteVampBase::getParameterDescriptors() const
{
   ParameterList list = GetParameterDescriptors();

   ParameterDescriptor d;
   d.identifier = "attach_hack";
   d.name = "Attach hack (dev only)";
   d.description =
      "Will FREEZE THE HOST until the dev has attached a debugger and modified the blocking condition.";
   d.unit = "";
   d.minValue = 0;
   d.maxValue = 1;
   d.defaultValue = static_cast<float>(mAttachHack);
   d.isQuantized = true;
   d.quantizeStep = 1;
   d.valueNames.push_back("No");
   d.valueNames.push_back("Yes");
   list.push_back(d);

   return list;
}

Vamp::Plugin::FeatureSet LteVampBase::getRemainingFeatures()
{
   // The host has pushed blocks of audio of size preferredBlockSize, likely
   // zero-padding the last one. Since the exact audio duration is fundamental
   // to this algorithm, we need to clean this up.
   mAudioReader.RemoveTrailingZeros(preferredBlockSize - 1);

   // Since the Vamp SDK, for some reason, doesn't communicate the input
   // duration to the plugins, we need a first dry run on this very input to get
   // it.
   if (mSampleCount != mAudioReader.GetNumSamples())
   {
      // This was the dry run. We update the length and request another run.
      LTE::FileUtils::WriteToAppDataFile(mAudioReader.GetNumSamples());
      throw std::runtime_error(
         "\nInput duration updated. Next time should work.");
   }

   return GetRemainingFeatures(mAudioReader);
}

std::string LteVampBase::getMaker() const
{
   return "The Audacity Team";
}

std::string LteVampBase::getCopyright() const
{
   return "GPL";
}

Vamp::Plugin::InputDomain LteVampBase::getInputDomain() const
{
   return TimeDomain;
}

size_t LteVampBase::getPreferredBlockSize() const
{
   return preferredBlockSize;
}

size_t LteVampBase::getPreferredStepSize() const
{
   return preferredBlockSize;
}

float LteVampBase::getParameter(std::string identifier) const
{
   if (identifier == "attack_hack")
      return static_cast<float>(mAttachHack);
   return GetParameter(identifier);
}

void LteVampBase::setParameter(std::string identifier, float value)
{
   if (identifier == "attach_hack")
      mAttachHack = static_cast<bool>(value);
   else
      SetParameter(identifier, value);
}

Vamp::Plugin::OutputList LteVampBase::getOutputDescriptors() const
{
   return GetOutputDescriptors();
}

bool LteVampBase::initialise(size_t channels, size_t stepSize, size_t blockSize)
{
   if (
      channels < getMinChannelCount() || channels > getMaxChannelCount() ||
      stepSize != preferredBlockSize || blockSize != preferredBlockSize)
      return false;

   auto block = mAttachHack;
   while (block)
      std::this_thread::sleep_for(std::chrono::milliseconds(100));

   mAudioReader.Initialize(preferredBlockSize);
   return true;
}

Vamp::Plugin::FeatureSet
LteVampBase::process(const float* const* inputBuffers, Vamp::RealTime)
{
   mAudioReader.Push(inputBuffers);
   return {};
}

void LteVampBase::reset()
{
}

Vamp::Plugin::ParameterList LteVampBase::GetParameterDescriptors() const
{
   return {};
}

float LteVampBase::GetParameter(std::string) const
{
   return 0;
}

void LteVampBase::SetParameter(std::string, float)
{
}
