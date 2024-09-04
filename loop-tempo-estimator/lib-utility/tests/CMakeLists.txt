# Create a unit-test of the lib-utility library, adding MathApproxTest.cpp as source and using Catch2:

# Create the executable
add_executable(lib-utility-test
    MathApproxTest.cpp
)

# Link libraries
target_link_libraries(lib-utility-test PRIVATE lib-utility Catch2::Catch2WithMain)
