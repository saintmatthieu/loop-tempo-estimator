include(pffft.cmake)

set(SOURCES
   include/MirTypes.h
   include/MusicInformationRetrieval.h
   include/PowerSpectrumGetter.h
   source/DecimatingMirAudioReader.cpp
   source/DecimatingMirAudioReader.h
   source/GetMeterUsingTatumQuantizationFit.cpp
   source/GetMeterUsingTatumQuantizationFit.h
   source/IteratorX.cpp
   source/IteratorX.h
   source/MapToPositiveHalfIndex.h
   source/MathApprox.h
   source/MemoryX.cpp
   source/MemoryX.h
   source/MirDsp.cpp
   source/MirDsp.h
   source/MirUtils.cpp
   source/MirUtils.h
   source/MusicInformationRetrieval.cpp
   source/PowerSpectrumGetter.cpp
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

add_subdirectory(tests)