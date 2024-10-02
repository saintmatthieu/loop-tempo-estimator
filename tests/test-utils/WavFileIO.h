/*  SPDX-License-Identifier: GPL-2.0-or-later */
/*!********************************************************************

  Audacity: A Digital Audio Editor

  WavFileIO.h

  Matthieu Hodgkinson

**********************************************************************/
/*

 This file was copied from Audacity 3.5.1,
 https://github.com/audacity/audacity/releases/tag/Audacity-3.5.1.

 Modifications might have been made to remove dependencies on Audacity code and
 when renaming files. The algorithm remains the same.

 */
#pragma once

#include "AudioFileInfo.h"

#include <chrono>
#include <optional>
#include <string>
#include <vector>

struct WavFileIO
{
   static bool Read(
      const std::string& path, std::vector<std::vector<float>>&, AudioFileInfo&,
      const std::optional<std::chrono::seconds>& upTo = std::nullopt);

   static bool Write(
      const std::string& path, const std::vector<std::vector<float>>&,
      int sampleRate);
};
