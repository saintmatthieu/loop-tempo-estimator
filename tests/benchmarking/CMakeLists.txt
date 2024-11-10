add_executable(benchmarking
        TatumQuantizationFitBenchmarking.cpp
        RocTest.cpp
)

target_link_libraries(benchmarking PRIVATE
        test-utils
)
