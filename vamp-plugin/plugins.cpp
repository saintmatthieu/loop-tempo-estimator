#include <vamp-sdk/PluginAdapter.h>
#include <vamp/vamp.h>

#include "LoopTempoEstimatorStepsVamp.h"
#include "LoopTempoEstimatorVamp.h"

static Vamp::PluginAdapter<LoopTempoEstimatorVamp> loopTempoEstimatorVamp;
static Vamp::PluginAdapter<LoopTempoEstimatorStepsVamp>
   loopTempoEstimatorStepsVamp;

const VampPluginDescriptor*
vampGetPluginDescriptor(unsigned int version, unsigned int index)
{
   if (version < 1)
      return 0;

   switch (index)
   {
   case 0:
      return loopTempoEstimatorStepsVamp.getDescriptor();
   case 1:
      return loopTempoEstimatorVamp.getDescriptor();
   default:
      return 0;
   }
}
