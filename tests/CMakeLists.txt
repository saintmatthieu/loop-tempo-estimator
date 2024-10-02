add_compile_definitions(
        CMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)

include(cmake/Catch2.cmake)
include(cmake/minimp3.cmake)

add_executable(lib-loop-tempo-estimator-test
        AudioFileInfo.h
        AudioFileIO.cpp
        AudioFileIO.h
        LteFakes.h
        MathApproxTest.cpp
        LteTestUtils.cpp
        LteTestUtils.h
        Mp3FileReader.cpp
        Mp3FileReader.h
        StftFrameProviderTests.cpp
        TatumQuantizationFitBenchmarking.cpp
        TatumQuantizationFitVisualization.cpp
        TestLteAudioReader.cpp
        TestLteAudioReader.h
        WavFileIO.cpp
        WavFileIO.h
)

target_link_libraries(lib-loop-tempo-estimator-test PRIVATE
        loop-tempo-estimator
        minimp3
        Catch2::Catch2WithMain
)

option(USE_LIBSNDFILE "Use libsndfile for audio file I/O" OFF)
if (USE_LIBSNDFILE)
        add_compile_definitions(USE_LIBSNDFILE)
        find_package(SndFile REQUIRED)
        target_link_libraries(lib-loop-tempo-estimator-test PRIVATE
                SndFile::sndfile
        )
endif()

target_include_directories(lib-loop-tempo-estimator-test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../source
)
