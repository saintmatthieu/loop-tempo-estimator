include(FetchContent)

FetchContent_Declare(
  vamp-plugin-sdk
  GIT_REPOSITORY https://github.com/vamp-plugins/vamp-plugin-sdk.git
  GIT_TAG 703438d8e55dbfd1c2021d0bc12e36edd1e227e2
)

set(VAMPSDK_BUILD_SIMPLE_HOST ON CACHE BOOL "Build simple host")

FetchContent_MakeAvailable(vamp-plugin-sdk)
