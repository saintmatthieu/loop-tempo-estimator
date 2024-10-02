add_executable(benchmarking
        TatumQuantizationFitBenchmarking.cpp
)

target_link_libraries(benchmarking PRIVATE
        test-utils
)

find_package(Python3 COMPONENTS Interpreter REQUIRED)

add_custom_command(
        TARGET benchmarking
        POST_BUILD
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/download-benchmarking-dataset.py
)