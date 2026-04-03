cmake_minimum_required(VERSION 3.12)

project(loop-tempo-estimator)

set(CMAKE_CXX_STANDARD 17)

# Use pffft from parent project if available,
# otherwise fetch and build it.
if (NOT TARGET pffft)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/pffft.cmake)
endif()

set(SOURCES
   ${CMAKE_CURRENT_SOURCE_DIR}/DecimatingLteAudioReader.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/DecimatingLteAudioReader.h
   ${CMAKE_CURRENT_SOURCE_DIR}/GetMeterUsingTatumQuantizationFit.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/GetMeterUsingTatumQuantizationFit.h
   ${CMAKE_CURRENT_SOURCE_DIR}/IteratorX.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/IteratorX.h
   ${CMAKE_CURRENT_SOURCE_DIR}/LoopTempoEstimator.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/LteDsp.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/LteDsp.h
   ${CMAKE_CURRENT_SOURCE_DIR}/LteUtils.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/LteUtils.h
   ${CMAKE_CURRENT_SOURCE_DIR}/MapToPositiveHalfIndex.h
   ${CMAKE_CURRENT_SOURCE_DIR}/MathApprox.h
   ${CMAKE_CURRENT_SOURCE_DIR}/PowerSpectrumGetter.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/PowerSpectrumGetter.h
   ${CMAKE_CURRENT_SOURCE_DIR}/StftFrameProvider.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/StftFrameProvider.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/LoopTempoEstimator/LoopTempoEstimator.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/LoopTempoEstimator/LteTypes.h
)

add_library(loop-tempo-estimator ${SOURCES})

target_link_libraries(loop-tempo-estimator PUBLIC pffft)
set_target_properties(loop-tempo-estimator PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(loop-tempo-estimator
   PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}/include
   PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}
)

if (MSVC)
    target_compile_definitions(loop-tempo-estimator PRIVATE
        _DISABLE_CONSTEXPR_MUTEX_CONSTRUCTOR
        _CRT_SECURE_NO_WARNINGS
    )
    target_compile_options(loop-tempo-estimator PRIVATE
        /wd4244 /wd4267 /wd4101 /wd4018 /wd4305
    )
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(loop-tempo-estimator PRIVATE
        -Wno-conversion
        -Wno-sign-conversion
        -Wno-unused-variable
        -Wno-sign-compare
        -Wno-float-conversion
    )
endif()
