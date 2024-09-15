add_compile_definitions(
   CMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)

add_executable(lib-loop-tempo-estimator-test
   AudioFileInfo.h
   AudioFileIO.cpp
   AudioFileIO.h
   MirFakes.h
   MirTestUtils.cpp
   MirTestUtils.h
    Mp3FileReader.cpp
    Mp3FileReader.h
   MusicInformationRetrievalTests.cpp
   StftFrameProviderTests.cpp
   TatumQuantizationFitBenchmarking.cpp
   TatumQuantizationFitVisualization.cpp
   WavFileIO.cpp
   WavFileIO.h
   WavMirAudioReader.cpp
   WavMirAudioReader.h
        Mp3MirAudioReader.cpp
        Mp3MirAudioReader.h
)

target_link_libraries(lib-loop-tempo-estimator-test PRIVATE
   loop-tempo-estimator
   # mpg123
   minimp3
   sndfile
   Catch2::Catch2WithMain
)
