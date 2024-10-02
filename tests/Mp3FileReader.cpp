/*  SPDX-License-Identifier: GPL-2.0-or-later */
/*!********************************************************************

  Audacity: A Digital Audio Editor

  Mp3FileReader.cpp

  Matthieu Hodgkinson

**********************************************************************/
/*

 This file was copied from Audacity 3.5.1,
 https://github.com/audacity/audacity/releases/tag/Audacity-3.5.1.

 Modifications might have been made to remove dependencies on Audacity code and
 when renaming files. The algorithm remains the same.

 */

#include "Mp3FileReader.h"
#include "AudioFileInfo.h"

#define MINIMP3_IMPLEMENTATION
#include "minimp3_ex.h"

bool Mp3FileReader::Read(
   const std::string& path, std::vector<std::vector<float>>& floats,
   AudioFileInfo& info)
{
   mp3dec_ex_t dec;
   int loadResult = mp3dec_ex_open(&dec, path.c_str(), MP3D_SEEK_TO_SAMPLE);
   if (loadResult != 0)
   {
      // Failed to open or decode the MP3 file
      return false;
   }

   int totalSamples =
      int(dec.samples); // Total number of samples (including all channels)
   int numChannels = dec.info.channels;
   int sampleRate = dec.info.hz;

   int numFrames = totalSamples / numChannels;

   // Resize the floats vector to hold num_channels channels
   floats.resize(numChannels);
   for (int c = 0; c < numChannels; ++c)
   {
      floats[c].reserve(numFrames);
   }

   // Convert the integer PCM data to float and de-interleave
   mp3d_sample_t* buffer = nullptr;
   mp3dec_frame_info_t frameInfo;
   constexpr auto maxSamplesPerFrame = 1024;
   while (const auto numSamples = mp3dec_ex_read_frame(
             &dec, &buffer, &frameInfo, maxSamplesPerFrame))
   {
      for (size_t i = 0; i < numSamples / numChannels; ++i)
         for (int c = 0; c < numChannels; ++c)
         {
            int16_t sampleInt16 = buffer[i * numChannels + c];
            float sampleFloat =
               float(sampleInt16) / 32768.0f; // Normalize to [-1.0, 1.0]
            floats[c].push_back(sampleFloat);
         }
   }

   // Populate the AudioFileInfo struct
   info.sampleRate = sampleRate;
   info.numChannels = numChannels;
   info.numFrames = numFrames;

   mp3dec_ex_close(&dec);

   return true;
}
