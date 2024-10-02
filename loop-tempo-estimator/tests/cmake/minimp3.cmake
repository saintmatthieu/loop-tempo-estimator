include(FetchContent)

FetchContent_Declare(
  minimp3
  GIT_REPOSITORY https://github.com/lieff/minimp3.git
  GIT_TAG afb604c06bc8beb145fecd42c0ceb5bda8795144
)
FetchContent_MakeAvailable(minimp3)
# minimp3 doesn't have a release build with binaries, so we need to build it ourselves
add_library(minimp3 INTERFACE)
target_include_directories(minimp3 INTERFACE
        ${minimp3_SOURCE_DIR}
)
