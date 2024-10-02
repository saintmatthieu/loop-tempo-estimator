add_executable(unit-tests
        MathApproxTest.cpp
        StftFrameProviderTests.cpp
)

target_link_libraries(unit-tests PRIVATE
        test-utils
)
