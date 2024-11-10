#include "LteTestUtils.h"

#include <catch2/catch_test_macros.hpp>

namespace LTE
{
TEST_CASE("GetRocInfo")
{
   // We use the AUC as a measure of the classifier's performance. With a
   // suitable data set, this helps us detect regressions, and guide fine-tuning
   // of the algorithm. This test should help understand how it works and also
   // make sure that we've implemented that metric correctly :)

   struct Sample
   {
      bool truth;
      double score;
   };

   using Samples = std::vector<Sample>;

   struct Expected
   {
      double areaUnderCurve;
      double threshold;
   };

   struct TestCase
   {
      const Samples samples;
      const double allowedFalsePositiveRate;
      const Expected expected;
   };

   const std::vector<TestCase> testCases {
      // Classifier is upside down. We don't tolerate false positives. The
      // returned threshold is then 100 will satisfy this, but the TPR will also
      // be 0 ...
      TestCase { Samples { { true, 100. }, { false, 200. } }, 0.,
                 Expected { 0., 200. } },

      // Classifier is still upside down. We'll get true positives only if we
      // accept an FPR of 1.
      TestCase { Samples { { true, 100. }, { false, 200. } }, 1.,
                 Expected { 0., 100. } },

      // Now we have a classifier that works. We don't accept false positives.
      TestCase { Samples { { false, 100. }, { false, 150. }, { true, 200. } },
                 0., Expected { 1., 175. } },

      // A random classifier, which should have an AUC of 0.75.
      TestCase {
         Samples { { false, 1. }, { true, 2. }, { false, 3. }, { true, 4. } },
         0.5, Expected { .75, 1.5 } },
   };

   for (const auto& testCase : testCases)
   {
      const auto roc =
         GetRocInfo(testCase.samples, testCase.allowedFalsePositiveRate);
      REQUIRE(roc.areaUnderCurve == testCase.expected.areaUnderCurve);
      REQUIRE(roc.threshold == testCase.expected.threshold);
   }
}
} // namespace LTE
