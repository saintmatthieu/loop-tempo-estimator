cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-D_CRT_SECURE_NO_WARNINGS /wd4244 /wd4267 /wd4101 /wd4018 /wd4305)

project(loop-tempo-estimator)

add_subdirectory(loop-tempo-estimator)

add_subdirectory(Catch2)
# add_subdirectory(mpg123) Doesn't have a CMakeLists.txt file, needs a bit more work. Consider the alternative minimp3.

set(BUILD_TESTING OFF)
set(BUILD_PROGRAMS OFF)
set(BUILD_EXAMPLES OFF)
add_subdirectory(libsndfile)

# pffft
add_definitions(-D_USE_MATH_DEFINES)

add_library(pffft
    pffft/pffft.c
    pffft/pffft.h
)

target_include_directories(pffft PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/pffft
)

