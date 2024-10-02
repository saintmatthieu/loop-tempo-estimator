include(cmake/Catch2.cmake)
include(cmake/minimp3.cmake)

add_library(test-utils
        AudioFileInfo.h
        AudioFileIO.cpp
        AudioFileIO.h
        LteFakes.h
        LteTestUtils.cpp
        LteTestUtils.h
        Mp3FileReader.cpp
        Mp3FileReader.h
        TestLteAudioReader.cpp
        TestLteAudioReader.h
        WavFileIO.cpp
        WavFileIO.h
)

target_link_libraries(test-utils PUBLIC
        loop-tempo-estimator
        minimp3
        Catch2::Catch2WithMain
)

option(USE_LIBSNDFILE "Use libsndfile for audio file I/O" OFF)
if (USE_LIBSNDFILE)
        add_compile_definitions(USE_LIBSNDFILE)
        find_package(SndFile REQUIRED)
        target_link_libraries(test-utils PUBLIC
                SndFile::sndfile
        )
endif()

target_include_directories(test-utils PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../source
)

target_compile_definitions(test-utils PUBLIC
        CMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)
