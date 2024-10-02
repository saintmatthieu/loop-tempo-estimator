add_executable(visualization
        TatumQuantizationFitVisualization.cpp
)

target_link_libraries(visualization PRIVATE
        test-utils
)
