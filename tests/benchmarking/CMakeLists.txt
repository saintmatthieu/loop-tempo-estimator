add_executable(benchmarking
        TatumQuantizationFitBenchmarking.cpp
)

target_link_libraries(benchmarking PRIVATE
        test-utils
)

find_package(Python3 COMPONENTS Interpreter REQUIRED)
