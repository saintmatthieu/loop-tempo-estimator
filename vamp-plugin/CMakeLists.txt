add_library(loop-tempo-estimator-vamp SHARED
    FileUtils.cpp
    FileUtils.h
    FloatVectorLteAudioReader.cpp
    FloatVectorLteAudioReader.h
    LoopTempoEstimatorStepsVamp.cpp
    LoopTempoEstimatorStepsVamp.h
    LoopTempoEstimatorVamp.cpp
    LoopTempoEstimatorVamp.h
    LteVampBase.cpp
    LteVampBase.h
    plugins.cpp
)

add_dependencies(loop-tempo-estimator-vamp vamp-sdk)

target_link_libraries(loop-tempo-estimator-vamp
    PUBLIC
    loop-tempo-estimator
    vamp-sdk
)

if (WIN32)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /EXPORT:vampGetPluginDescriptor")
endif()