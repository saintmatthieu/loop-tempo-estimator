cmake_minimum_required(VERSION 3.12)

project(loop-tempo-estimator)
set(CMAKE_CXX_STANDARD 17)

if (MSVC)
    # Suppress specific MSVC warnings
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_compile_options(
        /wd4244  # Conversion from 'type1' to 'type2', possible loss of data
        /wd4267  # Conversion from 'size_t' to 'type', possible loss of data
        /wd4101  # Unreferenced local variable
        /wd4018  # Signed/unsigned mismatch
        /wd4305  # Truncation from 'type1' to 'type2'
    )
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
    # Suppress equivalent GCC/Clang warnings
    add_compile_options(
        -D_CRT_SECURE_NO_WARNINGS  # Define macro (harmless on non-MSVC compilers)
        -Wno-conversion            # Suppress warnings about conversions
        -Wno-sign-conversion       # Suppress sign conversion warnings
        -Wno-unused-variable       # Suppress unused variable warnings
        -Wno-sign-compare          # Suppress signed/unsigned comparison warnings
        -Wno-float-conversion      # Suppress float conversion warnings
    )
endif()

# Rest of your CMakeLists.txt
add_subdirectory(loop-tempo-estimator)

add_subdirectory(Catch2)
include(minimp3.cmake)

set(BUILD_TESTING OFF)
set(BUILD_PROGRAMS OFF)
set(BUILD_EXAMPLES OFF)
add_subdirectory(libsndfile)

# pffft library setup
add_definitions(-D_USE_MATH_DEFINES)

add_library(pffft
    pffft/pffft.c
    pffft/pffft.h
)

target_include_directories(pffft PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/pffft
)
