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
   # Mp3FileReader.cpp proper integration of mpg123 or minimp3 is required for this guy.
   # Mp3FileReader.h
   StftFrameProviderTests.cpp
   TatumQuantizationFitBenchmarking.cpp
   TatumQuantizationFitVisualization.cpp
   WavFileIO.cpp
   WavFileIO.h
   WavMirAudioReader.cpp
   WavMirAudioReader.h
)

target_link_libraries(lib-loop-tempo-estimator-test PRIVATE
   loop-tempo-estimator
   # mpg123
   sndfile
   Catch2::Catch2WithMain
   )
