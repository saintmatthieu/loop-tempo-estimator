include(FetchContent)

FetchContent_Declare(
  pffft
  GIT_REPOSITORY https://bitbucket.org/jpommier/pffft.git
  GIT_TAG fbc4058602803f40dc554b8a5d2bcc694c005f2f
)
FetchContent_MakeAvailable(pffft)
# pffft doesn't have a release build with binaries, so we need to build it ourselves
add_definitions(-D_USE_MATH_DEFINES)
add_library(pffft 
   ${pffft_SOURCE_DIR}/pffft.c
   ${pffft_SOURCE_DIR}/pffft.h
)
target_include_directories(pffft PUBLIC
   ${pffft_SOURCE_DIR}
)
