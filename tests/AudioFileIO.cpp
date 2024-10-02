/*  SPDX-License-Identifier: GPL-2.0-or-later */
/*!********************************************************************

  Audacity: A Digital Audio Editor

  AudioFileIO.cpp

  Matthieu Hodgkinson

**********************************************************************/
/*

 This file was copied from Audacity 3.5.1,
 https://github.com/audacity/audacity/releases/tag/Audacity-3.5.1.

 Modifications might have been made to remove dependencies on Audacity code and
 when renaming files. The algorithm remains the same.

 */
#include "AudioFileIO.h"
#include "Mp3FileReader.h"
#include "WavFileIO.h"

bool AudioFileIO::Read(
   const std::string& path, std::vector<std::vector<float>>& audio,
   AudioFileInfo& info, const std::optional<std::chrono::seconds>& upTo)
{
   return WavFileIO::Read(path, audio, info, upTo) ||
          Mp3FileReader::Read(path, audio, info);
}
