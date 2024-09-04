set(SOURCES
   DecimatingMirAudioReader.cpp
   DecimatingMirAudioReader.h
   GetMeterUsingTatumQuantizationFit.cpp
   GetMeterUsingTatumQuantizationFit.h
   MirDsp.cpp
   MirDsp.h
   MirProjectInterface.h
   MirTypes.h
   MirUtils.cpp
   MirUtils.h
   MusicInformationRetrieval.cpp
   MusicInformationRetrieval.h
   PowerSpectrumGetter.cpp
   PowerSpectrumGetter.h
   StftFrameProvider.cpp
   StftFrameProvider.h
)

set(LIBRARIES
   lib-utility
   pffft
)

# Create the executable
add_library(loop-tempo-estimator ${SOURCES})

# Link libraries
target_link_libraries(loop-tempo-estimator PUBLIC ${LIBRARIES})

target_include_directories(loop-tempo-estimator PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(lib-utility)
add_subdirectory(tests)