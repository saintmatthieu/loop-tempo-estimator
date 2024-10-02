/*  SPDX-License-Identifier: GPL-2.0-or-later */
/*!********************************************************************

  Audacity: A Digital Audio Editor

  MirUtils.h

  Matthieu Hodgkinson

**********************************************************************/
/*

 This file was copied from Audacity 3.5.1,
 https://github.com/audacity/audacity/releases/tag/Audacity-3.5.1.

 Modifications might have been made to remove dependencies on Audacity code and
 when renaming files. The algorithm remains the same.

 */
#pragma once

#include <algorithm>
#include <vector>

namespace MIR
{
/*!
 * @brief Function to generate numbers whose prime factorization contains only
 * twos or threes
 */
std::vector<int> GetPossibleBarDivisors(int lower, int upper);

std::vector<int> GetPeakIndices(const std::vector<float>& x);

std::vector<float> GetNormalizedHann(int size);

constexpr auto IsPowOfTwo(int x)
{
   return x > 0 && (x & (x - 1)) == 0;
}

//*******************************
//          TESTS
//*******************************

static_assert(!IsPowOfTwo(-2));
static_assert(!IsPowOfTwo(-1));
static_assert(!IsPowOfTwo(0));
static_assert(IsPowOfTwo(1));
static_assert(IsPowOfTwo(2));
static_assert(!IsPowOfTwo(3));
static_assert(IsPowOfTwo(4));
} // namespace MIR
