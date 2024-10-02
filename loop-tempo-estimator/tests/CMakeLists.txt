add_compile_definitions(
        CMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)

include(cmake/Catch2.cmake)
include(cmake/minimp3.cmake)

find_package(SndFile REQUIRED)

add_executable(lib-loop-tempo-estimator-test
        AudioFileInfo.h
        AudioFileIO.cpp
        AudioFileIO.h
        MathApproxTest.cpp
        MirFakes.h
        MirTestUtils.cpp
        MirTestUtils.h
        Mp3FileReader.cpp
        Mp3FileReader.h
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
        minimp3
        SndFile::sndfile
        Catch2::Catch2WithMain
)

target_include_directories(lib-loop-tempo-estimator-test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../source
)
