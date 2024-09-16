/*  SPDX-License-Identifier: GPL-2.0-or-later */
/*!********************************************************************

  Audacity: A Digital Audio Editor

  Mp3FileReader.cpp

  Matthieu Hodgkinson

**********************************************************************/

#include "Mp3FileReader.h"
#include "AudioFileInfo.h"

#define MINIMP3_IMPLEMENTATION
#include "minimp3_ex.h"

bool Mp3FileReader::Read(
   const std::string& path, std::vector<std::vector<float>>& floats,
   AudioFileInfo& info)
{
    mp3dec_ex_t dec;
    int load_result = mp3dec_ex_open(&dec, path.c_str(), MP3D_SEEK_TO_SAMPLE);
    if (load_result != 0) {
        // Failed to open or decode the MP3 file
        return false;
    }

    size_t total_samples = dec.samples; // Total number of samples (including all channels)
    int num_channels = dec.info.channels;
    int sample_rate = dec.info.hz;

    size_t num_frames = total_samples / num_channels;

    // Resize the floats vector to hold num_channels channels
    floats.resize(num_channels);
    for (int c = 0; c < num_channels; ++c) {
        floats[c].reserve(num_frames);
    }

    // Convert the integer PCM data to float and de-interleave
    mp3d_sample_t* buffer = nullptr;
    mp3dec_frame_info_t frameInfo;
    constexpr auto maxSamplesPerFrame = 1024;
    while (const auto numSamples = mp3dec_ex_read_frame(
              &dec, &buffer, &frameInfo, maxSamplesPerFrame))
    {
       for (size_t i = 0; i < numSamples / num_channels; ++i)
          for (int c = 0; c < num_channels; ++c)
          {
             int16_t sample_int16 = buffer[i * num_channels + c];
             float sample_float =
                sample_int16 / 32768.0f; // Normalize to [-1.0, 1.0]
             floats[c].push_back(sample_float);
          }
    }

    // Populate the AudioFileInfo struct
    info.sampleRate = sample_rate;
    info.numChannels = num_channels;
    info.numFrames = num_frames;

    mp3dec_ex_close(&dec);

    return true;
}
