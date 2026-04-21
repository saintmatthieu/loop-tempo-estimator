#include "LteFakes.h"
#include "LoopTempoEstimator/LoopTempoEstimator.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("GetBpm returns nullopt for silence")
{
   using namespace LTE;
   const SilenceLteAudioReader audio;
   std::function<void(double)> progressCb;
   const auto result =
      GetBpm(audio, FalsePositiveTolerance::Lenient, progressCb);
   REQUIRE(!result.has_value());
}
