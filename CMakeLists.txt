cmake_minimum_required(VERSION 3.12)

project(loop-tempo-estimator)

# Option to build tests:
option(BUILD_TESTS "Build tests" ON)
option(BUILD_VAMP_PLUGIN "Build Vamp plugin" ON)

set(CMAKE_CXX_STANDARD 17)

if (MSVC)
    # Suppress specific MSVC warnings
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_compile_options(
        /wd4244  # Conversion from 'type1' to 'type2', possible loss of data
        /wd4267  # Conversion from 'size_t' to 'type', possible loss of data
        /wd4101  # Unreferenced local variable
        /wd4018  # Signed/unsigned mismatch
        /wd4305  # Truncation from 'type1' to 'type2'
    )
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
    # Suppress equivalent GCC/Clang warnings
    add_compile_options(
        -D_CRT_SECURE_NO_WARNINGS  # Define macro (harmless on non-MSVC compilers)
        -Wno-conversion            # Suppress warnings about conversions
        -Wno-sign-conversion       # Suppress sign conversion warnings
        -Wno-unused-variable       # Suppress unused variable warnings
        -Wno-sign-compare          # Suppress signed/unsigned comparison warnings
        -Wno-float-conversion      # Suppress float conversion warnings
    )
endif()

include(pffft.cmake)

set(SOURCES
   include/LoopTempoEstimator/LoopTempoEstimator.h
   include/LoopTempoEstimator/LteTypes.h
   source/DecimatingLteAudioReader.cpp
   source/DecimatingLteAudioReader.h
   source/GetMeterUsingTatumQuantizationFit.cpp
   source/GetMeterUsingTatumQuantizationFit.h
   source/IteratorX.cpp
   source/IteratorX.h
   source/LoopTempoEstimator.cpp
   source/LteDsp.cpp
   source/LteDsp.h
   source/LteUtils.cpp
   source/LteUtils.h
   source/MapToPositiveHalfIndex.h
   source/MathApprox.h
   source/MemoryX.cpp
   source/MemoryX.h
   source/PowerSpectrumGetter.cpp
   source/PowerSpectrumGetter.h
   source/StftFrameProvider.cpp
   source/StftFrameProvider.h
)

# Create the executable
add_library(loop-tempo-estimator ${SOURCES})

# Link libraries
target_link_libraries(loop-tempo-estimator PUBLIC pffft)

target_include_directories(loop-tempo-estimator
   PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}/include
   PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/source
)

if (BUILD_TESTS)
    add_subdirectory(tests)
endif()

if (BUILD_VAMP_PLUGIN)
    include(vamp-plugin-sdk.cmake)
    add_subdirectory(vamp-plugin)
endif()
