add_executable(performance
        TatumQuantizationFitPerformance.cpp
)

target_link_libraries(performance PRIVATE
        test-utils
)
